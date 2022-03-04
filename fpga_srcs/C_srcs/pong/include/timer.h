#ifndef __TIMER_H__
#define __TIMER_H__

/**
 * timer: 1ms timer setup & interrupt
 * 
 * Author: Atharva Lele <atharva@pdx.edu>
 */

#include "commondefs.h"
#include "psp_types.h"

/* PTC Register Addresses */
#define PTC_BASE_ADDR           0x80001200

#define PTC_CNTR_OFFSET         0x00
#define PTC_HRC_OFFSET          0x01
#define PTC_LRC_OFFSET          0x02
#define PTC_CNTRL_OFFSET        0x03

#define PTC_CNTRL_EN_BIT        0x01
#define PTC_CNTRL_OE_BIT        0x08
#define PTC_CNTRL_SINGLE_BIT    0x10
#define PTC_CNTRL_IE_BIT        0x20
#define PTC_CNTRL_INT_ACK_BIT   0x40

/* Global flags */
extern volatile u08_t secflag;

/* Functions */
void timer_init(void);
void timer_isr(void);

#endif