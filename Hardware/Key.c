#include "Key.h"
#include "GPIO.h"

extern void Key_on_keydown(void);
extern void Key_on_keyup(void);

void Key_init() {
    //开漏输出
    P0_MODE_OUT_OD(GPIO_Pin_5);
}

static u8 last_key_level = 1;   // 上一次的按键电平, 默认松开
// 扫描按键
#if 0
void Key_scan() {
    if(last_key_level != KEY) {
        last_key_level = KEY;
        if(last_key_level == DOWN) {
            Key_on_keydown();
        } else {
            Key_on_keyup();
        }
    }
}
#endif


/* 连续 KEY_DEBOUNCE_TICKS 次采样 (5ms x 4 = 20ms) 电平一致才确认,
   避免机械抖动让一次按键触发多次回调 */
#define KEY_DEBOUNCE_TICKS  4

static u8 debounce_count = 0;   /* 连续与当前电平不一致的次数 */

void Key_scan(void (*down)(void), void (*up)(void)){
    u8 now = KEY;

    if (now == last_key_level) {          /* 电平没变, 重新计数 */
        debounce_count = 0;
        return;
    }
    if (++debounce_count < KEY_DEBOUNCE_TICKS) return;   /* 还在抖动, 继续观察 */

    debounce_count = 0;
    last_key_level = now;
    if (now == DOWN) {
        if (down != NULL) down();
    } else {
        if (up != NULL) up();
    }
}