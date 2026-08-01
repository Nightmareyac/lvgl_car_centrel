#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "music.h"
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#define FIFO_PATH "/tmp/mplayer_fifo"

// 歌曲索引
Song song_list[] = {
    {"Melody", "Zee Tao", "/userdata/music/melody.mp3", "S:/userdata/music/melody.bmp", "S:/userdata/music/melody1.bmp",
     210},
    {"CuteWoman", "Jay", "/userdata/music/cutewoman.mp3", "S:/userdata/music/cutewoman.bmp",
     "S:/userdata/music/cutewoman1.bmp", 240},
    {"I Missed", "Sun Yanzi", "/userdata/music/whatimiss.mp3", "S:/userdata/music/whatimiss.bmp",
     "S:/userdata/music/whatimiss1.bmp", 260},
};
int song_count = 3;

static int current_song_index = 0;
static bool is_playing        = false;
static pid_t mplayer_pid      = -1;
static time_t play_start_time = 0; // 当前段开始播放的时刻
static time_t pause_elapsed   = 0; // 暂停前已累计秒数

static void stop_player(void)
{
    if(mplayer_pid > 0) {
        kill(mplayer_pid, SIGTERM);
        waitpid(mplayer_pid, NULL, WNOHANG);
        mplayer_pid = -1;
    }
    is_playing      = false;
    play_start_time = 0;
    pause_elapsed   = 0;
}

static void start_player(const char * path)
{
    stop_player();
    if(access(FIFO_PATH, F_OK) != 0) {
        mkfifo(FIFO_PATH, 0666);
    }

    pid_t pid = fork();
    if(pid == 0) {
        execlp("mplayer", "mplayer", "-slave", "-input", "file=/tmp/mplayer_fifo", "-ao", "alsa:device=hw=1.0",
               "-really-quiet", path, NULL);
        _exit(1);
    } else if(pid > 0) {
        mplayer_pid     = pid;
        is_playing      = true;
        play_start_time = time(NULL);
        pause_elapsed   = 0;
    }
}

void music_set_volume(int percent)
{
    if(percent < 0) {
        percent = 0;
    }
    if(percent > 100) {
        percent = 100;
    }

    if(mplayer_pid <= 0) {
        return;
    }

    int fd = open(FIFO_PATH, O_WRONLY | O_NONBLOCK);
    if(fd >= 0) {
        char cmd[64];
        snprintf(cmd, sizeof(cmd), "volume %d 1\n", percent);

        // 尝试写入命令
        write(fd, cmd, strlen(cmd));

        // 记得关闭文件描述符
        close(fd);
        printf("[MUSIC] Non-blocking FIFO sent volume: %d%%\n", percent);
    } else {
        // 如果管道暂时无法写入，静默跳过或简单打印，绝不卡死界面
        perror("[MUSIC] FIFO not ready");
    }
}

void music_jump_to(int index)
{
    if(index < 0 || index >= song_count) return;
    current_song_index = index;
    start_player(song_list[current_song_index].path);
}

void music_init(void)
{
    current_song_index = 0;
    is_playing         = false;
    mplayer_pid        = -1;
    play_start_time    = 0;
    pause_elapsed      = 0;
    signal(SIGCHLD, SIG_IGN);
}

Song * music_get_current_song(void)
{
    if(song_count == 0) return NULL;
    return &song_list[current_song_index];
}

bool music_is_playing(void)
{
    return is_playing;
}

int music_get_progress(void)
{
    Song * cur = music_get_current_song();
    if(!cur || cur->duration <= 0 || mplayer_pid <= 0) 
        return 0;
    time_t elapsed = pause_elapsed + (is_playing ? (time(NULL) - play_start_time) : 0);
    int percent    = (int)(elapsed * 100 / cur->duration);
    return percent > 100 ? 100 : percent;
}

//播放状态切换
void music_play_toggle(void)
{
    Song * cur = music_get_current_song();
    if(!cur) return;

    if(mplayer_pid <= 0) {
        start_player(cur->path);
    } else if(is_playing) {
        pause_elapsed += time(NULL) - play_start_time; // 记录已播放时长
        kill(mplayer_pid, SIGSTOP);
        is_playing = false;
        printf("[MUSIC] paused: %s\n", cur->tittle);
    } else {
        play_start_time = time(NULL); // 重置段开始时刻
        kill(mplayer_pid, SIGCONT);
        is_playing = true;
        printf("[MUSIC] resumed: %s\n", cur->tittle);
    }
}

void music_next(void)
{
    if(song_count == 0) return;
    current_song_index = (current_song_index + 1) % song_count;
    printf("[MUSIC] next: %s\n", song_list[current_song_index].tittle);
    if(mplayer_pid > 0 || is_playing) {
        start_player(song_list[current_song_index].path);
    }
}

void music_prev(void)
{
    if(song_count == 0) return;
    current_song_index--;
    if(current_song_index < 0) current_song_index = song_count - 1;
    printf("[MUSIC] prev: %s\n", song_list[current_song_index].tittle);
    if(mplayer_pid > 0 || is_playing) {
        start_player(song_list[current_song_index].path);
    }
}
