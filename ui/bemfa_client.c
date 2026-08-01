#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "bemfa_client.h"
#include "time.h"

//通信api
#define BEMFA_HOST "tcp.bemfa.com"
#define BEMFA_PORT 8344
#define BEMFA_UID "3396ed7c726d437583ccb9b23e1f579a"


static time_t g_Last_heartbeat = 0;
static int g_sockid            = -1;
static pthread_t recv_thread_id;
static bool g_running = false;

//默认状态设置关闭
BemfaDeviceState g_bemfa_state = {.trunk_open  = false,
                                  .lock_locked = false,
                                  .ac_on       = false,
                                  .frunk_open  = false,
                                  .flash1_turn = false,
                                  .flash2_turn = false,
                                  .flash3_turn = false,
                                  .is_updated  = false};


//心跳函数
void bemfa_send_ping(void)
{
    if(g_sockid >= 0) {
        const char * ping_cmd = "ping\r\n";
        if(send(g_sockid, ping_cmd, strlen(ping_cmd), 0) < 0) {
            perror("[bemfa] heartbeat send failed\n");
            close(g_sockid);
            g_sockid = -1;
        } else {
            printf("[bemfa]send heartbeat\r\n");
            g_Last_heartbeat = time(NULL);
        }
    }
}

//订阅主题
static void bemfa_subscribe_all(void)
{
    char buf[256];
    // 1. 后备箱
    snprintf(buf, sizeof(buf), "cmd=1&uid=%s&topic=mTJT3Afrk006\r\n", BEMFA_UID);
    send(g_sockid, buf, strlen(buf), 0);
    // 2. 车锁
    snprintf(buf, sizeof(buf), "cmd=1&uid=%s&topic=DyQL1Jcsc004\r\n", BEMFA_UID);
    send(g_sockid, buf, strlen(buf), 0);
    // 3. 空调
    snprintf(buf, sizeof(buf), "cmd=1&uid=%s&topic=61SOh2Izr005\r\n", BEMFA_UID);
    send(g_sockid, buf, strlen(buf), 0);
    // 4. 引擎盖
    snprintf(buf, sizeof(buf), "cmd=1&uid=%s&topic=vc8lsGpsp006\r\n", BEMFA_UID);
    send(g_sockid, buf, strlen(buf), 0);
    // 5. 灯1/2/3
    snprintf(buf, sizeof(buf), "cmd=1&uid=%s&topic=4lx3WVhUA002\r\n", BEMFA_UID);
    send(g_sockid, buf, strlen(buf), 0);
    snprintf(buf, sizeof(buf), "cmd=1&uid=%s&topic=6MHL9hbRf002\r\n", BEMFA_UID);
    send(g_sockid, buf, strlen(buf), 0);
    snprintf(buf, sizeof(buf), "cmd=1&uid=%s&topic=EIHBf8Q8A002\r\n", BEMFA_UID);
    send(g_sockid, buf, strlen(buf), 0);
}


static void parse_bemfa_msg(char * recv_buf)
{
    char * topic_ptr = strstr(recv_buf, "topic=");
    char * msg_ptr   = strstr(recv_buf, "msg=");
    if(topic_ptr && msg_ptr) {
        char topic[64] = {0};
        char msg[32]   = {0};

        sscanf(topic_ptr, "topic=%63[^&]", topic);
        sscanf(msg_ptr, "msg=%31[^\r\n]", msg);

        printf("[bemfa push receive] Topic: %s , Msg: %s\n", topic, msg);

        if(strcmp(topic, "mTJT3Afrk006") == 0) { // 后备箱
            g_bemfa_state.trunk_open = (strcmp(msg, "on") == 0 || strcmp(msg, "open") == 0);
            g_bemfa_state.is_updated = true;
        } else if(strcmp(topic, "DyQL1Jcsc004") == 0) { // 车锁
            g_bemfa_state.lock_locked = (strcmp(msg, "lock") == 0 || strcmp(msg, "on") == 0);
            g_bemfa_state.is_updated  = true;
        } else if(strcmp(topic, "61SOh2Izr005") == 0) { // 空调
            g_bemfa_state.ac_on      = (strcmp(msg, "on") == 0);
            g_bemfa_state.is_updated = true;
        } else if(strcmp(topic, "vc8lsGpsp006") == 0) { // 引擎盖
            g_bemfa_state.frunk_open = (strcmp(msg, "on") == 0 || strcmp(msg, "open") == 0);
            g_bemfa_state.is_updated = true;
        } else if(strcmp(topic, "4lx3WVhUA002") == 0) { // 灯1
            g_bemfa_state.flash1_turn = (strcmp(msg, "on") == 0);
            g_bemfa_state.is_updated  = true;
        } else if(strcmp(topic, "6MHL9hbRf002") == 0) { // 灯2
            g_bemfa_state.flash2_turn = (strcmp(msg, "on") == 0);
            g_bemfa_state.is_updated  = true;
        } else if(strcmp(topic, "EIHBf8Q8A002") == 0) { // 灯3
            g_bemfa_state.flash3_turn = (strcmp(msg, "on") == 0);
            g_bemfa_state.is_updated  = true;
        }
    }
}

// 接受数据的子线程
static void * bemfa_recv_thread_func(void * arg)
{
    char buf[512];
    g_Last_heartbeat = time(NULL);

    while(g_running) {
        time_t now = time(NULL);
        if(now - g_Last_heartbeat >= 60) {
            bemfa_send_ping();
        }

        struct timeval tv;
        tv.tv_sec  = 2;
        tv.tv_usec = 0;
        setsockopt(g_sockid, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv));

        memset(buf, 0, sizeof(buf));
        int len = recv(g_sockid, buf, sizeof(buf) - 1, 0);
        if(len > 0) {
            if(strstr(buf, "pong")) {
                continue;
            }
            parse_bemfa_msg(buf);

        } else if(len < 0) {
            continue;
        } else if(len == 0) {
            printf("[bemfa]disconnect,tring to reconnection\n");
            sleep(2);
            bemfa_connect();
        }
    }
    return NULL;
}

int bemfa_connect(void)
{
    if(g_sockid >= 0) {
        return 0;
    }

    struct hostent * he = gethostbyname(BEMFA_HOST);
    if(!he) {
        perror("BEMFA DNS 域名解析失败");
        return -1;
    }

    g_sockid = socket(AF_INET, SOCK_STREAM, 0);
    if(g_sockid < 0) {
        perror("socket failed");
        return -1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port   = htons(BEMFA_PORT);
    server_addr.sin_addr   = *((struct in_addr *)he->h_addr);

    if(connect(g_sockid, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect failed");
        close(g_sockid);
        g_sockid = -1;
        return -1;
    }

    printf("connected to bemfa\n");
    bemfa_subscribe_all();

    if(!g_running) {
        g_running = true;
        pthread_create(&recv_thread_id, NULL, bemfa_recv_thread_func, NULL);
    }

    return 0;
}


//发送
void bemfa_send_msg(const char * topic, const char * msg)
{
    if(g_sockid < 0) {
        if(bemfa_connect() < 0) // 掉线重连
        {
            return;
        }
    }

    char send_buf[256];

    // 拼接bemfa格式
    snprintf(send_buf, sizeof(send_buf), "cmd=2&uid=%s&topic=%s&msg=%s\r\n", BEMFA_UID, topic, msg);
    if(send(g_sockid, send_buf, strlen(send_buf), 0) < 0) {
        perror("send to bemfa failed");
        close(g_sockid);
        g_sockid = -1;
    } else {
        printf("[bemfa] send successful! Topic: %s , Msg: %s\n", topic, msg);
    }
}

void bemfa_disconnect(void)
{
    if(g_sockid >= 0) {
        close(g_sockid);
        g_sockid = -1;
    }
}