#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "Config.h"


	
#define LED_LEFT  		P07
#define LED_RIGHT	 	P52



// 初始化
void Light_init();

/* 立即置灯 (或灭灯), 并取消尚未结束的定时闪烁 */
void Light_set(u8 on);

/* 读回当前灯状态, 供 A 键翻转使用 */
u8 Light_is_on(void);

/* 亮灯 ticks 个 RTOS tick 后自动熄灭; 立即返回, 不等待 */
void Light_alarm(u8 ticks);

/* 每 1 个 RTOS tick 调用一次, 推进闪烁节奏 */
void Light_tick(void);

void Light_stop(void);

#endif