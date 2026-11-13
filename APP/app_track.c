#include "APP.h"

#define TRACK_SPEED   20    /* 巡线速度 */
#define TRACK_TICKS   20    /* 采样间隔 */

/* 巡线开关只有这一个变量。按键和蓝牙都通过下面两个函数操作,
   避免两边各存一份状态, 出现重复创建任务、互锁失效的问题 */
static u8 track_enabled = 0;

void Track_SetEnabled(u8 on)
{
    if (on == track_enabled) return;        /* 状态没变就直接返回, 重复调用不会出问题 */

    track_enabled = on;
    if (on) {
        os_create_task(TASK_TRACK);
    } else {
        os_delete_task(TASK_TRACK);
        Motor_stop();
    }
}

u8 Track_IsEnabled(void)
{
    return track_enabled;
}

void task_track() _task_ TASK_TRACK {
    int res;
    while (1) {
        res = Track_get_position();
        if (res < 0)      Motors_turn(TRACK_SPEED, D_LEFT);
        else if (res > 0) Motors_turn(TRACK_SPEED, D_RIGHT);
        else              Motor_Front(TRACK_SPEED, D_MID);
        os_wait2(K_TMO, TRACK_TICKS);
    }
}
