#ifndef _APP_H
#define _APP_H

#include "Key.h"
#include "Tarck.h"
#include "Buzzer.h"
#include "Motor.h"
#include "Light.h"
#include "Config.h"
#include "Battery.h"
#include "Ultrasonic.h"
#include "Bluetooth.h"


#define TASK_KEY        1
#define TASK_UART1      2
#define TASK_UART2      3
#define TASK_TRACK      4
#define TASK_ULTRASONIC 5
#define TASK_BATTERY    6

// 1/2/3/5/6 上电常驻, 4 由按键或手机 D 键动态创建

/* 巡线开关: 唯一状态保存在 app_track.c, 按键与蓝牙都只走这两个接口 */
void Track_SetEnabled(u8 on);
u8   Track_IsEnabled(void);






#endif