#include "device/system.h"
#include "device/timer.h"

int main()
{
    /* Initialize peripherals and interrupts */
    system_init();

    while (1) {
        /* This block will be triggered per second */
        if (secflag) {
            secflag = 0;
        }
    }
    
    return 0;
}
