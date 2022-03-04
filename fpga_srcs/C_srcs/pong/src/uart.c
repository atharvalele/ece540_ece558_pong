/**
 * UART: UART driver for WD EH1
 * 
 * Author: Atharva Lele <atharva@pdx.edu>
 */

#include "commondefs.h"
#include "uart.h"
#include <string.h>

#include "bsp_external_interrupts.h"

char uart_tx_buf[UART_BUF_SIZE];
char uart_rx_buf[UART_BUF_SIZE];

struct sw_fifo_t uart_tx_fifo;
struct sw_fifo_t uart_rx_fifo;

u08_t rx_index;
u08_t rx_flag;
u08_t rx_tout;

/* Initialize UART */
void uart_init(u32_t baudrate)
{
    /* Set DLAB bit in LCR */
    WRITE_REG(UART_BASE_ADDR, UART_LCR_OFFSET, UART_DLAB_BIT);

    /* Set divisor regs  devisor = 27: clock_freq/baud_rate*16 -->> clock = 50MHz, baud=115200*/
    WRITE_REG(UART_BASE_ADDR, UART_BAUDR_OFFSET, ((D_CLOCK_RATE/baudrate) / 16));

    /* 8 data bits, 1 stop bit,_ no parity, clear DLAB, 1-byte deep RX FIFO */
    WRITE_REG(UART_BASE_ADDR, UART_LCR_OFFSET, (UART_LCR_CS8  | UART_LCR_1_STB | UART_LCR_PDIS));
    WRITE_REG(UART_BASE_ADDR, UART_FCR_OFFSET, (UART_FCR_FIFO_BIT| UART_FCR_MODE0_BIT | UART_FCR_FIFO_1_BYTE | UART_FCR_RCVRCLR_BIT | UART_FCR_XMITCLR_BIT));

    /* disable interrupts  */
    WRITE_REG(UART_BASE_ADDR, UART_IER_OFFSET, 0);

    /* Initialize FIFOs */
    sw_fifo_init(&uart_rx_fifo, uart_rx_buf, UART_BUF_SIZE);
    sw_fifo_init(&uart_tx_fifo, uart_tx_buf, UART_BUF_SIZE);

    /* Set timeout */
    rx_tout = UART_RX_TOUT;
}

void uart_interrupt_enable(void)
{
    /* Enable Receiver Data Available interrupt */
    WRITE_REG(UART_BASE_ADDR, UART_IER_OFFSET, (UART_IE_RDA_BIT));
}

/* Send a character over UART: blocking */
void uart_char_send_blocking(const char c)
{
    /* Check for space in FIFO */
    while ((READ_REG(UART_BASE_ADDR, UART_LSR_OFFSET) & UART_LSR_THRE_BIT) == 0);

    WRITE_REG(UART_BASE_ADDR, 0, (c));
}

/* Send a string over UART: blocking */
void uart_str_send_blocking(const char *str)
{
    while (*str)
        uart_char_send(*str++);
}

/* Send a string over UART: interrupt-based */
void uart_str_send(const char *str)
{
    u16_t len = strlen(str);
    u32_t reg;

    if (len > 0)
    {
        sw_fifo_write(&uart_tx_fifo, str, len);

        /* Enable uart TX holding reg empty interrupt */
        reg = READ_REG(UART_BASE_ADDR, UART_IER_OFFSET);
        WRITE_REG(UART_BASE_ADDR, UART_IER_OFFSET, (reg | UART_IE_TXE_BIT));
    }
}

/* Interrupt handler */
void uart_isr(void)
{
    u08_t irq = READ_REG(UART_BASE_ADDR, UART_IIR_OFFSET);

    char c;
    u16_t bytes;
    u32_t reg;

    /* TX holding register empty interrupt */
    if (irq & 0x02) {
        bytes = sw_fifo_read(&uart_tx_fifo, &c, 1);
        if (bytes != 0) {
            WRITE_REG(UART_BASE_ADDR, 0, (c));
        } else {
            /* Disable TXE interrupt */
            reg = READ_REG(UART_BASE_ADDR, UART_IER_OFFSET);
            reg &= ~UART_IE_TXE_BIT;
            WRITE_REG(UART_BASE_ADDR, UART_IER_OFFSET, reg);
        }
    }

    /* Receiver data available interrupt */
    if (irq & 0x04) {
        /* Check if receiver data available, add to buffer, reset timeout counter */
        if((READ_REG(UART_BASE_ADDR, UART_LSR_OFFSET) & UART_LSR_RXRDY_BIT) == 1) {
            c = READ_REG(UART_BASE_ADDR, 0);
            sw_fifo_write(&uart_rx_fifo, &c, 1);
            rx_index++;
            if (rx_index > UART_BUF_SIZE)
                rx_index = 0;
            rx_flag = 1;
            rx_tout = UART_RX_TOUT;
        }
    }
}

void uart_test(void)
{
    char c;
    
    /* Check if receiver data available */
    if((READ_REG(UART_BASE_ADDR, UART_LSR_OFFSET) & UART_LSR_RXRDY_BIT) == 1) {
        c = READ_REG(UART_BASE_ADDR, 0);

        uart_char_send(c);
    }
}