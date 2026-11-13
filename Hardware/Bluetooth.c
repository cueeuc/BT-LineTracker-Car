#include "APP.h"
#include "UART.h"

/* 手机 App 每帧固定 8 字节: DD 77 | X Y | A B C D
   X、Y 是摇杆值, A~D 非 0 表示按键按下 */

#define JOYSTICK_MIN      (-100)   /* 摇杆最小值 */
#define JOYSTICK_MAX      ( 100)   /* 摇杆最大值 */
#define ROTATE_SPEED      30       /* 原地旋转速度 */

/* 摇杆编码:
   0 = 有符号补码, 范围 -100~100 (默认)
   1 = 无符号中心偏置, 中心值是 JOYSTICK_CENTER
   如果手机 App 发的是 0~255、中心 128, 把下面改成 1 */
#define JOYSTICK_CENTERED 0
#define JOYSTICK_CENTER   128

/* 上一次收到的 A、D 键电平。
   用来区分"刚按下"和"一直按着", 长按只触发一次 */
static u8 last_key_a = 0;
static u8 last_key_d = 0;

/* 把一个字节转成摇杆值 */
static signed char Bluetooth_joystick_value(u8 raw)
{
    if (JOYSTICK_CENTERED) {
        return (signed char)((int)raw - JOYSTICK_CENTER);
    }
    return (signed char)raw;
}

/* 摇杆值是否在合法范围内 */
static u8 Bluetooth_joystick_valid(u8 raw)
{
    signed char value = Bluetooth_joystick_value(raw);
    return (value >= JOYSTICK_MIN && value <= JOYSTICK_MAX);
}

/* 处理一帧数据 (调用前已经确认帧头正确) */
static void Bluetooth_handle_frame(u8 *frame)
{
    u8 key_a = frame[BT_IDX_KEY_A];
    u8 key_b = frame[BT_IDX_KEY_B];
    u8 key_c = frame[BT_IDX_KEY_C];
    u8 key_d = frame[BT_IDX_KEY_D];

    /* A 键: 只在刚按下的那一帧响一声、翻一次灯 */
    if (key_a && !last_key_a) {
        Buzzer_alarm();
        Light_set(!Light_is_on());
    }

    /* D 键: 只在刚按下的那一帧切换一次巡线 */
    if (key_d && !last_key_d) {
        Track_SetEnabled(!Track_IsEnabled());
    }
    last_key_a = key_a;
    last_key_d = key_d;

    /* 巡线开着的时候, 手动控制全部让位 */
    if (Track_IsEnabled()) return;

    /* B、C 键原地旋转: 同时按下就停车, 否则按住的这边转 */
    if (key_b || key_c) {
        if (key_b && key_c) Motor_stop();
        else                Motor_rotate(ROTATE_SPEED, key_b ? D_LEFT : D_RIGHT);
        return;
    }

    /* 摇杆值不合法就丢掉这一帧, 避免电机乱动 */
    if (!Bluetooth_joystick_valid(frame[BT_IDX_X]) ||
        !Bluetooth_joystick_valid(frame[BT_IDX_Y])) {
        return;
    }

    Motor_move(Bluetooth_joystick_value(frame[BT_IDX_X]),
               Bluetooth_joystick_value(frame[BT_IDX_Y]));
}

/* 从接收缓冲区里找出所有完整的帧, 逐个处理。
   缓冲区里可能粘了多帧, 也可能前面混进杂字节:
   找到帧头 DD 77 就取 8 字节, 找不到就往后挪一个字节 */
void Bluetooth_communication(void)
{
    u8 start = 0;                  /* 当前检查到第几个字节 */
    u8 received = COM2.RX_Cnt;     /* 这次一共收到多少字节 */

    while (start + BT_FRAME_LEN <= received) {
        if (RX2_Buffer[start] == BT_FRAME_HEAD0 &&
            RX2_Buffer[start + 1] == BT_FRAME_HEAD1) {
            Bluetooth_handle_frame(&RX2_Buffer[start]);
            start += BT_FRAME_LEN;   /* 整帧处理完, 跳过 8 字节 */
        } else {
            start++;                 /* 不是帧头, 往后找 */
        }
    }
}
