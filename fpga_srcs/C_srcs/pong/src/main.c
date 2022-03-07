#include "device/system.h"
#include "device/timer.h"
#include <hagl.h>

#include "game/pong.h"

int main()
{
    /* Initialize peripherals and interrupts */
    system_init();

    /* Initialize display driver */
    hagl_init();

    while (1) {
        /* Game Task */
        pong_task();
        
        /* This block will be triggered per second */
        if (secflag) {
            secflag = 0;
        }
    }
    
    return 0;
}
