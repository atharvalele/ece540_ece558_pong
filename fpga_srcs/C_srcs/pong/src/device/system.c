/**
 * system: interrupts and other system configuration for RVfpga
 * 
 * Author: Atharva Lele <atharva@pdx.edu>
 */

#include "device/system.h"
#include "device/timer.h"
#include "device/uart.h"

/* Initialize interrupt controller */
void interrupts_init(void)
{
    u32_t uiSourceId;

    /* Register IRQ vector table */
    pspInterruptsSetVectorTableAddress(&M_PSP_VECT_TABLE);

    /* Set external interrupt table address in CSR */
    pspExternalInterruptSetVectorTableAddress(G_Ext_Interrupt_Handlers);

    /* Put the Generation-Register in its initial state (no external interrupts are generated) */
    bspInitializeGenerationRegister(D_PSP_EXT_INT_ACTIVE_HIGH);

    for (uiSourceId = D_BSP_FIRST_IRQ_NUM; uiSourceId <= D_BSP_LAST_IRQ_NUM; uiSourceId++) {
        /* Clear trigger */
        bspClearExtInterrupt(uiSourceId);
    }

    /* Set Standard priority order */
    pspExtInterruptSetPriorityOrder(D_PSP_EXT_INT_STANDARD_PRIORITY);

    /* Set interrupts threshold to minimal (== all interrupts should be served) */
    pspExtInterruptsSetThreshold(M_PSP_EXT_INT_THRESHOLD_UNMASK_ALL_VALUE);

    /* Set the nesting priority threshold to minimal (== all interrupts should be served) */
    pspExtInterruptsSetNestingPriorityThreshold(M_PSP_EXT_INT_THRESHOLD_UNMASK_ALL_VALUE);
}

/* Function to set up and external interrupt */
void ExternalIntLine_Initialization(u32_t uiSourceId, u32_t priority, pspInterruptHandler_t pTestIsr)
{
    /* Set Gateway Interrupt type (Level) */
    pspExtInterruptSetType(uiSourceId, D_PSP_EXT_INT_LEVEL_TRIG_TYPE);

    /* Set gateway Polarity (Active high) */
    pspExtInterruptSetPolarity(uiSourceId, D_PSP_EXT_INT_ACTIVE_HIGH);

    /* Clear the gateway */
    pspExtInterruptClearPendingInt(uiSourceId);

    /* Set IRQ priority */
    pspExtInterruptSetPriority(uiSourceId, priority);

    /* Enable IRQ interrupts in the PIC */
    pspExternalInterruptEnableNumber(uiSourceId);

    /* Register ISR */
    G_Ext_Interrupt_Handlers[uiSourceId] = pTestIsr;
}

/* Initialize peripheral interrupts */
static void peripheral_interrupts_enable()
{
    /* Setup UART IRQ */
    ExternalIntLine_Initialization(1, 6, uart_isr);
    uart_interrupt_enable();

    /* Setup Timer IRQ */
    ExternalIntLine_Initialization(3, 6, timer_isr);
    WRITE_REG(SELECT_INT, 0, 0x02);

    /* Enable interrupts */
    pspInterruptsEnable();                              /* Enable all interrupts in mstatus CSR */
    M_PSP_SET_CSR(D_PSP_MIE_NUM, D_PSP_MIE_MEIE_MASK);  /* Enable external interrupts in mie CSR */
}

/* System Init */
void system_init(void)
{
    uart_init(115200);
    timer_init();
    interrupts_init();
    peripheral_interrupts_enable();
}