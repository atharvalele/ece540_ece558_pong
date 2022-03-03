#ifndef _SW_FIFO_H
#define _SW_FIFO_H

/**
 * Generic FIFO implementation
 * 
 * Author: Atharva Lele <atharva@pdx.edu>
 */

#include "psp_types.h"

/* Struct of the FIFO, an array must be passed externally to
 * initialize the FIFO. This allows for having variable
 * size arrays for different appliations
 */
struct sw_fifo_t {
    char *buf;
    u16_t head;
    u16_t tail;
    u16_t size;
};

void sw_fifo_init(struct sw_fifo_t *fifo, char *buf, u16_t size);
u16_t sw_fifo_read(struct sw_fifo_t *fifo, char *buf, u16_t num_bytes);
u16_t sw_fifo_write(struct sw_fifo_t *fifo, const char *buf, u16_t num_bytes);
u16_t sw_fifo_get_free_space(struct sw_fifo_t *fifo);

#endif