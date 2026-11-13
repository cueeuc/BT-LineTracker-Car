#ifndef _BLUETOOTH_H
#define _BLUETOOTH_H

/* 手机App固定8字节帧: DD 77 | X Y | A B C D */
#define BT_FRAME_HEAD0  0xDD
#define BT_FRAME_HEAD1  0x77
#define BT_FRAME_LEN    8

/* 帧内字节下标，避免散落的魔法数字 */
typedef enum {
    BT_IDX_HEAD0 = 0,
    BT_IDX_HEAD1,
    BT_IDX_X,
    BT_IDX_Y,
    BT_IDX_KEY_A,
    BT_IDX_KEY_B,
    BT_IDX_KEY_C,
    BT_IDX_KEY_D
} BT_Index;

void Bluetooth_communication(void);

#endif
