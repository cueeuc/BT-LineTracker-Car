#ifndef _MOTOR_H
#define _MOTOR_H

#define LIMIT   0.2

#define PERIOD  (MAIN_Fosc / 1000)

// 左前轮 left  forward
#define 	LF_P		P16
#define 	LF_N		P17

// 右前轮 right forward
#define 	RF_P		P14
#define 	RF_N		P15

// 左后轮 left backward
#define 	LB_P		P22
#define 	LB_N		P23

// 右后轮 right backward
#define 	RB_P		P20
#define 	RB_N		P21

typedef struct Motor_speed{
    signed char LF;//左前
    signed char LR;//左后
    signed char RF;//右前
    signed char RR;//右后
}MS;


typedef enum{
    D_LEFT,D_MID,D_RIGHT
}Dir;

//void Motor_Front(signed char speed,Dir mode_f);
//void Motor_Back(signed char speed,Dir mode_b);
void Motor_move(signed char x, signed char y);
void Motor_stop(void);
void Motor_init(void);
void Motor_drive(void);
void Motor_Front(signed char speed,Dir mode_f);
void Motors_turn(signed char speed ,  Dir mode);
void Motor_rotate(signed char speed,Dir mode_r);
void Motor_translation(signed char speed,Dir mode_t);

#endif