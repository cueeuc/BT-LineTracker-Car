#include "Buzzer.h"
#include "NVIC.h"
#include "Config.h"
#include "PWM.h"
#include "Switch.h"
#include "GPIO.h"

u16 code FREQS[] = {
    262, 294, 330, 349, 392, 440, 494,       // L1~L7 (低音)
    523, 587, 659, 698, 784, 880, 988,       // N1~N7 (中音)
    1047, 1175, 1319, 1397, 1568, 1760, 1976, // H1~H7 (高音)
};

/* 三声下行音, 频率从高到低 */
static const u16 code ALARM_TONES[3] = { 659*2, 587*2, 523*2 };

#define ALARM_SOUND_TICKS  20   /* 每声响多久 */
#define ALARM_PAUSE_TICKS  10   /* 每声之间停多久 */

#define ALARM_IDLE      0       /* 没在报警 */
#define ALARM_SOUNDING  1       /* 正在响 */
#define ALARM_PAUSING   2       /* 间隔静音 */

static u8 alarm_state = ALARM_IDLE;
static u8 alarm_index = 0;      /* 当前是第几声, 0~2 */
static u8 alarm_ticks = 0;      /* 当前状态还剩多少 tick */


/******************* IO配置函数 *******************/
static void	GPIO_config(void)
{
    //推挽输出
    P3_MODE_OUT_PP(GPIO_Pin_4);
}

/***************  PWM配置函数 *****************/
static void	PWM_config(u16 hz_output)
{
    PWMx_InitDefine		PWMx_InitStructure;

    u16 period = (MAIN_Fosc / hz_output);

    PWMx_InitStructure.PWM_Mode    =	CCMRn_PWM_MODE1;	//模式,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
    PWMx_InitStructure.PWM_Duty    =  0;								//PWM占空比时间, 0~Period
    PWMx_InitStructure.PWM_EnoSelect  = ENO8P;	//输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
    PWM_Configuration(PWM8, &PWMx_InitStructure);

    PWMx_InitStructure.PWM_Period   = period-1;					//周期时间,   0~65535
    PWMx_InitStructure.PWM_DeadTime = 0;					//死区发生器设置, 0~255
    PWMx_InitStructure.PWM_MainOutEnable= ENABLE;			//主输出使能, ENABLE,DISABLE
    PWMx_InitStructure.PWM_CEN_Enable   = ENABLE;			//使能计数器, ENABLE,DISABLE
    PWM_Configuration(PWMB, &PWMx_InitStructure);			//初始化PWM通用寄存器,  PWMA,PWMB


    PWM8_SW(PWM8_SW_P34);			//PWM4_SW_P16_P17,PWM4_SW_P26_P27,PWM4_SW_P66_P67,PWM4_SW_P34_P33

    // 初始化PWMA/B的中断
    NVIC_PWM_Init(PWMB,DISABLE,Priority_0);
}

void Buzzer_init(void) {
    EAXSFR(); /* 扩展寄存器使能 */
    GPIO_config();
    PWM_config(1000);
}

// 按照指定频率播放声音
void Buzzer_play(u16 hz_value) {
    u32 period;
    u16 duty;

    if (hz_value == 0) { Buzzer_stop(); return; }   /* 防除零 */
    period = (u32)MAIN_Fosc / hz_value;
    if (period > 65535UL) period = 65535UL;          /* 防 u16 溢出 */

    PWMB_AutoReload((u16)(period - 1));   /* 周期 */
    duty = (u16)(period * 0.02f);         /* 0.02 决定音量 */
    PWMB_Duty8(duty);
    PWMB_CC8E_Enable();
}

void Buzzer_beep(u16 tone) { // 1,2,3,4,5,6,7,8
    if (tone == 0) {        // 如果是休止符
        Buzzer_stop();      // 直接静音
        return;
    }
    Buzzer_play(FREQS[tone - 1]);
}

void Buzzer_stop(void) {
    PWMB_CC8E_Disable(); // 单独关闭PWMB的通道8输出
}

void Buzzer_alarm(void)
{
    alarm_state = ALARM_SOUNDING;
    alarm_index = 0;
    alarm_ticks = ALARM_SOUND_TICKS;
    Buzzer_play(ALARM_TONES[0]);
}

void Buzzer_tick(void)
{
    if (alarm_state == ALARM_IDLE) return;
    if (--alarm_ticks != 0) return;

    if (alarm_state == ALARM_SOUNDING) {
        /* 这一声响完了, 先静音 */
        Buzzer_stop();
        alarm_state = ALARM_PAUSING;
        alarm_ticks = ALARM_PAUSE_TICKS;
        return;
    }

    /* 静音结束, 该响下一声了 */
    alarm_index++;
    if (alarm_index >= 3) {
        alarm_state = ALARM_IDLE;      /* 三声都响完 */
        return;
    }
    Buzzer_play(ALARM_TONES[alarm_index]);
    alarm_state = ALARM_SOUNDING;
    alarm_ticks = ALARM_SOUND_TICKS;
}