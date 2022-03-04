/**
 * timer: 1ms timer setup & interrupt
 * 
 * Author: Atharva Lele <atharva@pdx.edu>
 */

#include "device/timer.h"
#include "device/uart.h"
#include "psp_api.h"
#include "bsp_external_interrupts.h"

/* Global vars */
volatile u08_t secflag = 0;

volatile u16_t msec = 0;

#define RPTC_CNTR       0x80001200
#define RPTC_HRC        0x80001204
#define RPTC_LRC        0x80001208
#define RPTC_CTRL       0x8000120c

void timer_init(void)
{
    /* Load timer value: Clocked @ 50MHz --> 50k count for 1ms tick */
    WRITE_REG(PTC_BASE_ADDR, PTC_LRC_OFFSET, 50000);

    /* Reset counter to 0 */
    WRITE_REG(PTC_BASE_ADDR, PTC_CNTR_OFFSET, 0);

    /* ACK interrupt */
    WRITE_REG(PTC_BASE_ADDR, PTC_CNTRL_OFFSET, PTC_CNTRL_INT_ACK_BIT);

    /* Enable single shot mode and interrupt bit */
    WRITE_REG(PTC_BASE_ADDR, PTC_CNTRL_OFFSET, (PTC_CNTRL_EN_BIT | PTC_CNTRL_SINGLE_BIT | PTC_CNTRL_IE_BIT));
}

/* Timer ISR */
void timer_isr(void)
{
    /* Re-init with needed values */
    timer_init();

    /* Stop the generation of the specific external interrupt */
    bspClearExtInterrupt(3);

    /* UART Timeout */
    uart_timeout();

    msec++;
    if (msec >= 1000) {
        msec = 0;
        secflag = 1;
    }
}