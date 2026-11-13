#ifndef _BUZZER_H_
#define _BUZZER_H_

#include "Config.h"

void Buzzer_init(void);

void Buzzer_play(u16 hz_value);

// 按照指定音调播放声音
void Buzzer_beep(u16 tone);

void Buzzer_stop(void);

/* 请求播放一次三声报警: 立即返回, 不等待、不调用 RTOS */
void Buzzer_alarm(void);

/* 每 1 个 RTOS tick 调用一次, 推进报警节奏并在结束时自动静音 */
void Buzzer_tick(void);

#endif