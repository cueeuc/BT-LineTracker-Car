#include  "APP.h"
#include "UART.h"
#include "NVIC.h"
#include "Switch.h"
#include "GPIO.h"


void GPIO_uart_init(){
    P1_MODE_IO_PU(GPIO_Pin_0|GPIO_Pin_1);
    P3_MODE_IO_PU(GPIO_Pin_0|GPIO_Pin_1);
}

void UART1_config(void) {
    COMx_InitDefine		COMx_InitStructure;					//结构定义
    COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//模式, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
    COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//选择波特率发生器, BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
    COMx_InitStructure.UART_BaudRate  = 115200ul;			//波特率, 一般 110 ~ 115200
    COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
    COMx_InitStructure.BaudRateDouble = DISABLE;			//波特率加倍, ENABLE或DISABLE
    UART_Configuration(UART1, &COMx_InitStructure);		//初始化串口1 UART1,UART2,UART3,UART4

  	NVIC_UART1_Init(ENABLE,Priority_0);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
    UART1_SW(UART1_SW_P30_P31);		// 引脚选择, UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}

void UART2_config(void) {
    COMx_InitDefine		COMx_InitStructure;					//结构定义
    COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//模式, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
    COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;			//选择波特率发生器, BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
    COMx_InitStructure.UART_BaudRate  = 115200ul;			//波特率, 一般 110 ~ 115200
    COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
    COMx_InitStructure.BaudRateDouble = DISABLE;			//波特率加倍, ENABLE或DISABLE
    UART_Configuration(UART2, &COMx_InitStructure);		//初始化串口1 UART1,UART2,UART3,UART4

  	NVIC_UART2_Init(ENABLE,Priority_0);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
    UART2_SW(UART2_SW_P10_P11);		// 引脚选择, UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}

//准双向口	P01为例
//P0_MODE_IO_PU(GPIO_Pin_1);
//高阻输入	P01为例
//P0_MODE_IN_HIZ(GPIO_Pin_1);
//漏极开路	P01为例
//P0_MODE_OUT_OD(GPIO_Pin_1);
//推挽输出	P01为例
//P0_MODE_OUT_PP(GPIO_Pin_1);



void sys_init() {

    EA = 1;
    EAXSFR();// 扩展寄存器访问使能
    
    GPIO_uart_init();
    UART1_config();
    UART2_config();
    Key_init();
    Light_init();
    Battery_init();
    Buzzer_init();
    Motor_init();
    Ultrasonic_init();
    Track_init();
}

void main_start() _task_ 0{
    sys_init();
    
    os_create_task(TASK_KEY);
    os_create_task(TASK_UART1);
    os_create_task(TASK_UART2);
    os_create_task(TASK_ULTRASONIC);
    os_create_task(TASK_BATTERY);
    os_delete_task(0);
}

