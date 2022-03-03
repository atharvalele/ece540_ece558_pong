#ifndef __SYSTEM_H__
#define __SYSTEM_H__

/**
 * system: interrupts and other system configuration for RVfpga
 * 
 * Author: Atharva Lele <atharva@pdx.edu>
 */

#include "psp_api.h"
#include "bsp_mem_map.h"
#include "bsp_external_interrupts.h"
#include "psp_ext_interrupts_eh1.h"

extern D_PSP_DATA_SECTION D_PSP_ALIGNED(1024) pspInterruptHandler_t G_Ext_Interrupt_Handlers[8];

/* Init everything */
void system_init(void);

/* Init interrupt system */
void interrupts_init(void);

#endif