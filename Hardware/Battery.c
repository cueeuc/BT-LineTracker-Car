#include "Battery.h"
#include 	"GPIO.h"
#include	"ADC.h"
#include	"NVIC.h"

/******************* AD配置函数 *******************/
static void	ADC_config(void)
{
	ADC_InitTypeDef		ADC_InitStructure;		//结构定义

	ADC_InitStructure.ADC_SMPduty   = 31;		//ADC 模拟信号采样时间控制, 0~31（注意： SMPDUTY 一定不能设置小于 10）
	ADC_InitStructure.ADC_CsSetup   = 0;		//ADC 通道选择时间控制 0(默认),1
	ADC_InitStructure.ADC_CsHold    = 1;		//ADC 通道选择保持时间控制 0,1(默认),2,3
	ADC_InitStructure.ADC_Speed     = ADC_SPEED_2X8T;		//设置 ADC 工作时钟频率	ADC_SPEED_2X1T~ADC_SPEED_2X16T
	ADC_InitStructure.ADC_AdjResult = ADC_RIGHT_JUSTIFIED;	//ADC结果调整,	ADC_LEFT_JUSTIFIED,ADC_RIGHT_JUSTIFIED
	ADC_Inilize(&ADC_InitStructure);		//初始化
	ADC_PowerControl(ENABLE);				//ADC电源开关, ENABLE或DISABLE
	NVIC_ADC_Init(DISABLE,Priority_0);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
}


// 初始化
void Battery_init(){
    P1_MODE_IN_HIZ(GPIO_Pin_3);
    ADC_config();
}
/* ADC 满量程参考电压与电池分压比: 请按板上实际基准修改。
   参考电压若与实际不符, 电压读数会成比例偏差, 低电报警会误报。 */
#define ADC_VREF       2.5f
#define BAT_DIVIDER    6.1f

// 获取电池电压
float Battery_get_voltage(){
    u16 adc_value = Get_ADCResult(ADC_CH3);
    return (adc_value * ADC_VREF / 4095.0f) * BAT_DIVIDER;
}
