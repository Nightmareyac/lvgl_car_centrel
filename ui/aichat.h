/**
 * @file    aichat.h
 * @brief   AI 对话模块头文件
 * @details 提供 AI 对话初始化接口，支持多轮循环对话。
 */

#ifndef UI_AICHAT_H
#define UI_AICHAT_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化 AI 对话模块
 * @details 绑定 UI 事件回调并启动后台网络线程。
 *          在 events_init.c 中被调用。
 */
void ai_init(void);

#ifdef __cplusplus
}
#endif

#endif /* UI_AICHAT_H */
