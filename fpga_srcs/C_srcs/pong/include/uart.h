#ifndef __UART_H__
#define __UART_H__

/**
 * UART: UART driver for WD EH1
 * 
 * Author: Atharva Lele <atharva@pdx.edu>
 */

#include "common_types.h"

#include "bsp_mem_map.h"

/* Defines for UART */
#define UART_BASE_ADDR          0x80002000
#define UART_BAUDR_OFFSET       0x00
#define UART_IER_OFFSET         0x01
#define UART_FCR_OFFSET         0x02
#define UART_LCR_OFFSET         0x03
#define UART_LSR_OFFSET         0x05

#define BAUD_RATE               (115200)
#define UART_LCR_CS8            (0x03)  /* 8 bits data size */
#define UART_LCR_1_STB          (0x00)  /* 1 stop bit */
#define UART_LCR_PDIS           (0x00)  /* parity disable */

#define UART_LSR_THRE_BIT       (0x20)
#define UART_FCR_FIFO_BIT       (0x01)  /* enable XMIT and RCVR FIFO */
#define UART_FCR_RCVRCLR_BIT    (0x02)  /* clear RCVR FIFO */
#define UART_FCR_XMITCLR_BIT    (0x04)  /* clear XMIT FIFO */
#define UART_FCR_MODE0_BIT      (0x00)  /* set receiver in mode 0 */
#define UART_FCR_MODE1_BIT      (0x08)  /* set receiver in mode 1 */
#define UART_FCR_FIFO_8_BIT     (0x80)  /* 8 bytes in RCVR FIFO */
#define UART_DLAB_BIT           (0x80)  /* DLAB bit in LCR */

#define UART_IE_RDA_BIT         (0x00)  /* Received Data Available */
#define UART_IE_TXE_BIT         (0x01)  /* TX Reg Empty */

/* Initialize UART */
void uart_init(u32_t baudrate);

/* Send char over UART */
void uart_char_send(const char c);

/* Sent string over UART */
void uart_str_send(const char *str);

#endif
