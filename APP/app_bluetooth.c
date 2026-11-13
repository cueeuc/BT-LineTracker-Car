#include "APP.h"
#include "UART.h"

void task_uart1() _task_ TASK_UART1{
    u8 i;
    while(1) {
        //超时计数：连续5个tick没收到新字节，认为一帧结束
        if(COM1.RX_TimeOut > 0) {
            if(--COM1.RX_TimeOut == 0) {
                if(COM1.RX_Cnt > 0) {
                    for(i=0; i<COM1.RX_Cnt; i++) {
                        // RX1_Buffer[i]存的是接收的每个字节，转发用TX2_write2buff
                        TX2_write2buff(RX1_Buffer[i]);
                    }
                }
                COM1.RX_Cnt = 0;
            }
        }
        os_wait2(K_TMO,1);
    }
}

void task_uart2() _task_ TASK_UART2{
    while(1) {
        //超时计数：连续5个tick没收到新字节，认为一批数据收完
        if(COM2.RX_TimeOut > 0) {
            if(--COM2.RX_TimeOut == 0) {
                if(COM2.RX_Cnt > 0) {
                    Bluetooth_communication();   // 内部自己找帧头, 可处理粘帧
                }
                COM2.RX_Cnt = 0;
            }
        }
        os_wait2(K_TMO,1);
    }
}
