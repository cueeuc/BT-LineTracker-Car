#include "Ultrasonic.h"
#include 	"GPIO.h"
#include	"Delay.h"


void Ultrasonic_init(){
    P4_MODE_IO_PU(GPIO_Pin_7);
    TRIG = 0;
    P3_MODE_IN_HIZ(GPIO_Pin_3);
    P2_MODE_OUT_PP  (GPIO_Pin_7);
}

void Delay10us(void)	//@24.000MHz
{
	unsigned char data i;

	i = 78;
	while (--i);
}



signed char Ultrasonic_get_distance(float *distance){
    u16 timeout = 0;
    TRIG = 1;
    Delay10us();
    Delay10us();
    TRIG = 0;
    
    while(ECHO==0){
        Delay10us();
        timeout++;
        if(timeout>500){
            return -1;
        }
    }
    timeout = 0;
    while (ECHO==1){
        Delay10us();
        timeout++;
        if(timeout>3000){
            return -2;
        }
    }
    
    *distance = ((timeout*0.01) * 34)/2;
    if (*distance < 2 || *distance > 400) return -3;
    
    return 1;   
}