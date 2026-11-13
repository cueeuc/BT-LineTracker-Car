#include "Motor.h"
#include "GPIO.h"
#include "PWM.h"
#include "Config.h"
#include "NVIC.h"
#include "Switch.h"


signed char Motor_Speed(signed char speed) {
    return speed /2 + 50;
}

/***************  PWM配置函数 *****************/
void PWM_Motor_config(MS ms_spend)
{
    PWMx_InitDefine		PWMx_InitStructure;

    //整数相除结果还是整数，会被截断
    //错误写法：(value/100) * PERIOD  正确写法：(value* PERIOD) /100

    // 配置PWM1 右后轮
    PWMx_InitStructure.PWM_Mode    		= CCMRn_PWM_MODE1;	//模式,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
    PWMx_InitStructure.PWM_Duty    		= (Motor_Speed(ms_spend.RR) * PERIOD)/100;	//PWM占空比时间, 0~Period
    PWMx_InitStructure.PWM_EnoSelect	=  (ms_spend.RR != 0) ? (ENO1P | ENO1N) : 0;	//输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
    PWM_Configuration(PWM1, &PWMx_InitStructure);			//初始化PWM

    // 配置PWM2 左后轮
    PWMx_InitStructure.PWM_Mode    		= CCMRn_PWM_MODE1;	//模式,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
    PWMx_InitStructure.PWM_Duty    		= (Motor_Speed(ms_spend.LR) * PERIOD)/100;	//PWM占空比时间, 0~Period
    PWMx_InitStructure.PWM_EnoSelect    = (ms_spend.LR != 0) ? (ENO2P | ENO2N) : 0; //输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
    PWM_Configuration(PWM2, &PWMx_InitStructure);			//初始化PWM

    // 配置PWM3 右前轮
    PWMx_InitStructure.PWM_Mode    =	CCMRn_PWM_MODE1;
    PWMx_InitStructure.PWM_Duty    =  (Motor_Speed(ms_spend.RF) * PERIOD)/100;
    PWMx_InitStructure.PWM_EnoSelect  = (ms_spend.RF != 0) ? (ENO3P | ENO3N) : 0;
    PWM_Configuration(PWM3, &PWMx_InitStructure);

    // 配置PWM4 左前轮
    PWMx_InitStructure.PWM_Mode    		= CCMRn_PWM_MODE1;	//模式,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
    PWMx_InitStructure.PWM_Duty    		= (Motor_Speed(ms_spend.LF) * PERIOD)/100;	//PWM占空比时间, 0~Period
    PWMx_InitStructure.PWM_EnoSelect    =  (ms_spend.LF != 0) ? (ENO4P | ENO4N) : 0;	//输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
    PWM_Configuration(PWM4, &PWMx_InitStructure);

    PWMx_InitStructure.PWM_Period   = PERIOD-1;
    PWMx_InitStructure.PWM_DeadTime = 0;
    PWMx_InitStructure.PWM_MainOutEnable= ENABLE;
    PWMx_InitStructure.PWM_CEN_Enable   = ENABLE;
    PWM_Configuration(PWMA, &PWMx_InitStructure);

    PWM1_SW(PWM1_SW_P20_P21);
    PWM2_SW(PWM2_SW_P22_P23);
    PWM3_SW(PWM3_SW_P14_P15);
    PWM4_SW(PWM4_SW_P16_P17);

    NVIC_PWM_Init(PWMA,DISABLE,Priority_0);
}

void GPIO_Motor_Cofig() {
    // P14 15 16 17
    P1_MODE_IO_PU(GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
    // P20 21 22 23
    P2_MODE_IO_PU(GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);

    LF_P=LF_N=RF_P=RF_N=LB_P=LB_N=RB_P=RB_N=0;
}

void Motor_init() {
    GPIO_Motor_Cofig();
}

void Motor_stop(void) {
    MS ms_s = {0};
    PWM_Motor_config(ms_s);
}

void Motor_Front(signed char speed, Dir mode_f) {
    MS ms_f = {0};
    if(mode_f == D_LEFT) {
        ms_f.LF = 0;
        ms_f.LR = speed;
        ms_f.RF = speed;
        ms_f.RR = 0;
    } else if(mode_f == D_RIGHT) {
        ms_f.LF = speed;
        ms_f.LR = 0;
        ms_f.RF = 0;
        ms_f.RR = speed;
    } else {
        ms_f.LF = speed;
        ms_f.LR = speed;
        ms_f.RF = speed;
        ms_f.RR = speed;
    }
    PWM_Motor_config(ms_f);
}

signed char Limitspeed(int speed){
    if(speed > 100){
        return 100;
    }
    if(speed < -100){
        return -100;
    }
    return speed;
}

void Motor_move(signed char x, signed char y){
    MS ms_m = {0};
    // LIMIT 是浮点缩放系数, 显式转成 char 后赋给轮速
    ms_m.LF = (signed char)(LIMIT*Limitspeed(y+x));
    ms_m.LR = (signed char)(LIMIT*Limitspeed(y-x));
    ms_m.RF = (signed char)(LIMIT*Limitspeed(y-x));
    ms_m.RR = (signed char)(LIMIT*Limitspeed(y+x));
    PWM_Motor_config(ms_m);
}



//void Motor_Back(char speed,Dir mode_b) {
//    MS ms_b = {0};
//    if(mode_b == D_LEFT) {
//        ms_b.LF = -speed;
//        ms_b.LR = 0;
//        ms_b.RF = 0;
//        ms_b.RR = -speed;
//    } else if(mode_b == D_RIGHT) {
//        ms_b.LF = 0;
//        ms_b.LR = -speed;
//        ms_b.RF = -speed;
//        ms_b.RR = 0;
//    } else {
//        ms_b.LF = -speed;
//        ms_b.LR = -speed;
//        ms_b.RF = -speed;
//        ms_b.RR = -speed;
//    }
//    PWM_Motor_config(ms_b);
//}

void Motor_translation(signed char speed, Dir mode_t) {
    MS ms_t = {0};
    if(mode_t == D_LEFT) {
        ms_t.LF = -speed;
        ms_t.LR = speed;
        ms_t.RF = speed;
        ms_t.RR = -speed;
    } else if(mode_t == D_RIGHT) {
        ms_t.LF = speed;
        ms_t.LR = -speed;
        ms_t.RF = -speed;
        ms_t.RR = speed;
    }
    PWM_Motor_config(ms_t);
}

void Motor_rotate(signed char speed, Dir mode_r) {
    MS ms_r = {0};
    if(mode_r == D_LEFT) {
        ms_r.LF = speed;
        ms_r.LR = speed;
        ms_r.RF = -speed;
        ms_r.RR = -speed;
    } else if(mode_r == D_RIGHT) {
        ms_r.LF = -speed;
        ms_r.LR = -speed;
        ms_r.RF = speed;
        ms_r.RR = speed;
    }
    PWM_Motor_config(ms_r);
}

void Motors_turn(signed char speed, Dir mode) {
	MS ms = {0};
	
	if (mode == D_LEFT) { // 左
		ms.LF = 0;
		ms.LR = 0;
		ms.RF = speed;
		ms.RR = speed;
	} else if (mode == D_RIGHT) { // 右
		ms.LF = speed;
		ms.LR = speed;
		ms.RF = 0;
		ms.RR = 0;
	}
	
	PWM_Motor_config(ms);
}

void Motor_drive() {
    u8 speed = 30;
    static u8 flag = 1;
    if(flag>3) flag = 1;
    switch(flag) {
    case 1:
        Motor_rotate(speed,D_LEFT);
        break;
    case 2:
        Motor_stop();
        break;
    case 3:
        Motor_rotate(speed,D_RIGHT);
        break;
    default:
        break;
    }
    flag++;
}