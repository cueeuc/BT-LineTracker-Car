#include "APP.h"


void Key_on_keydown() {
    Buzzer_alarm();                          /* 提示音, 不阻塞 */
    Track_SetEnabled(!Track_IsEnabled());    /* 状态由 app_track 统一持有 */
}

void Key_on_keyup() {
    printf("Key UP\n");
}


void task_key() _task_ TASK_KEY{

    while(1) {
        Key_scan(Key_on_keydown,Key_on_keyup);
        os_wait2(K_TMO,1);
    }
}
