#include "APP.h"

#define ULTRA_CHECK_TICKS  100   /* 每 100 tick 测一次距离 (约 0.5 秒) */
#define ULTRA_OBSTACLE_CM  15.0f /* 小于该距离视为障碍 */

/* 巡线时做一次保护性点刹: 测到近障碍就停车。
   巡线任务下一拍会重新接管电机, 所以这是减速保护而非完整避障;
   蓝牙遥控时不介入, 避免和手动控制抢电机。 */
void task_ultrasonic() _task_ TASK_ULTRASONIC
{
    float distance;
    while (1) {
        if (Track_IsEnabled() &&
            Ultrasonic_get_distance(&distance) == 1 &&
            distance < ULTRA_OBSTACLE_CM) {
            Motor_stop();
        }
        os_wait2(K_TMO, ULTRA_CHECK_TICKS);
    }
}
