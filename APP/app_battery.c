#include "APP.h"

#define BATTERY_CHECK_TICKS   100   /* 每 100 tick 采一次电压 (约 0.5 秒) */
#define BATTERY_LOW_V         10.5f /* 低电阈值 */
#define BATTERY_BLINK_TICKS   20    /* 每次闪灯持续时间 */

/* 这个任务只做两件事:
   1) 每 tick 推进蜂鸣器/指示灯的节奏 (硬件层不等待, 不阻塞任何任务)
   2) 周期性检测电量, 低电时请求一次声光报警 */
void task_battery() _task_ TASK_BATTERY {
    u8 check_count = 0;
    u8 is_low;
    u8 last_is_low = 0;
    float voltage;

    while (1) {
        Buzzer_tick();
        Light_tick();

        if (++check_count >= BATTERY_CHECK_TICKS) {
            check_count = 0;
            voltage = Battery_get_voltage();
            is_low = (voltage <= BATTERY_LOW_V);

            if (is_low) {
                Buzzer_alarm();                     /* 请求三声, 立即返回 */
                Light_alarm(BATTERY_BLINK_TICKS);  /* 闪一下, 立即返回 */
            }
            if (is_low != last_is_low) {                /* 只在状态变化时打印 */
                if (is_low) printf("Battery LOW\n");
                else     printf("Battery OK\n");
                last_is_low = is_low;
            }
        }
        os_wait2(K_TMO, 1);
    }
}
