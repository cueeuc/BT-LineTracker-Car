#ifndef __KEY_H__
#define __KEY_H__



#define KEY		P05
#define USE_KEYDOWN	1 
#define USE_KEYUP	1

#define DOWN    0
#define UP      1


// 初始化
void Key_init();

// 扫描按键，函数指针回调函数版本
// 不要和Key_scan()同时使用
void Key_scan(void (*down)(void), void (*up)(void));

#endif