/**
 * UART: UART driver for WD EH1
 * 
 * Author: Atharva Lele <atharva@pdx.edu>
 */

#include "commondefs.h"
#include "uart.h"

/* Initialize UART */
void uart_init(u32_t baudrate)
{
    /* Set DLAB bit in LCR */
    WRITE_REG(UART_BASE_ADDR, UART_LCR_OFFSET, UART_DLAB_BIT);

    /* Set divisor regs  devisor = 27: clock_freq/baud_rate*16 -->> clock = 50MHz, baud=115200*/
    WRITE_REG(UART_BASE_ADDR, UART_BAUDR_OFFSET, ((D_CLOCK_RATE/baudrate) / 16));

    /* 8 data bits, 1 stop bit,_ no parity, clear DLAB */
    WRITE_REG(UART_BASE_ADDR, UART_LCR_OFFSET, (UART_LCR_CS8  | UART_LCR_1_STB | UART_LCR_PDIS));
    WRITE_REG(UART_BASE_ADDR, UART_FCR_OFFSET, (UART_FCR_FIFO_BIT| UART_FCR_MODE0_BIT | UART_FCR_FIFO_8_BIT | UART_FCR_RCVRCLR_BIT | UART_FCR_XMITCLR_BIT));

    /* disable interrupts  */
    WRITE_REG(UART_BASE_ADDR, UART_IER_OFFSET, 0);
}

/* Send a character over UART */
void uart_char_send(const char c)
{
    /* Check for space in FIFO */
    while ((READ_REG(UART_BASE_ADDR, UART_LSR_OFFSET) & UART_LSR_THRE_BIT) == 0);

    WRITE_REG(UART_BASE_ADDR, 0, (c));
}

/* Send a string over UART */
void uart_str_send(const char *str)
{
    while (*str)
        uart_char_send(*str++);
}