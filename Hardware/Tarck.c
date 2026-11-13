#include "Tarck.h"
#include "GPIO.h"


void Track_init() {
    //准双向口
    P0_MODE_IO_PU(GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4);
}


int Track_get_position() {
    
    static int last_coordinate = 0;//上一次的坐标
    int cur_coordinate = 0;//当前坐标
    u8  cnt = 0;//标志位

//    if(OUT1 == 1) {// 左1	-64
//        cur_coordinate += -64;
//        cnt++;
//    }
    if(OUT2 == 1) {// 左2	-32
        cur_coordinate += -32;
        cnt++;
    }
    if(OUT3 == 1) {// 中	0
        cur_coordinate += 0;
        cnt++;
    }
    if(OUT4 == 1) {// 右1	32
        cur_coordinate += 32;
        cnt++;
    }
//    if(OUT5 == 1) {// 右2	64
//        cur_coordinate += 64;
//        cnt++;
//    }
    if(cnt == 0) {
        return last_coordinate;
    }
    cur_coordinate = cur_coordinate / cnt;
    last_coordinate = cur_coordinate;
    return cur_coordinate;
}
