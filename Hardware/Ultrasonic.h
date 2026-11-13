#ifndef __ULTRISONIC_H__
#define __ULTRISONIC_H__




// trig：单片机发送信号，高速超声波的模组去测距
// echo：高速超声波的模组响应信号
#define 	TRIG	P47
#define 	ECHO	P33
#define     LED1    P27

void Ultrasonic_init();

// 返回 1 表示成功获取距离; 负数表示等待超时或距离超范围
signed char Ultrasonic_get_distance(float *distance);

void Delay20us(void);

#endif