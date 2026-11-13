#include "Light.h"
#include "GPIO.h"
#include "Config.h"

/* 灯只有一份状态: led_on 是当前亮灭, led_ticks 是"亮多久"的倒计时。
   手动置灯和定时闪烁都改同一份状态, 不会互相打架。 */
static u8 led_on    = 0;
static u8 led_ticks = 0;      /* 0 表示没有定时 */

// 初始化
void Light_init(){
    P0_MODE_OUT_PP(GPIO_Pin_7);
    P5_MODE_OUT_PP(GPIO_Pin_2);
    LED_LEFT = LED_RIGHT = 0;
}

void Light_set(u8 on)
{
    led_on = on;
    led_ticks = 0;                    /* 手动置灯优先, 取消定时 */
    LED_LEFT = LED_RIGHT = on;
}

u8 Light_is_on(void)
{
    return led_on;
}

void Light_alarm(u8 ticks)
{
    if (ticks == 0) { Light_stop(); return; }
    Light_set(1);
    led_ticks = ticks;
}

void Light_tick(void)
{
    if (led_ticks == 0) return;
    if (--led_ticks == 0) Light_stop();
}

void Light_stop()
{
    Light_set(0);
}
