/**
 * Generic FIFO implementation
 * 
 * Author: Atharva Lele <atharva@pdx.edu>
 */

#include "utils/sw_fifo.h"

/* Initialize the FIFO with given size */
void sw_fifo_init(struct sw_fifo_t *fifo, char *buf, u16_t size)
{
    fifo->head = 0;
    fifo->tail = 0;
    fifo->size = size;
    fifo->buf = buf;
}

/* 
 * Read a number of bytes from the FIFO
 * Returns the number of bytes read
 */
u16_t sw_fifo_read(struct sw_fifo_t *fifo, char *buf, u16_t num_bytes)
{
    u16_t bytes_read;
    char *temp_ptr;

    temp_ptr = buf;

    /* Start reading */
    for (bytes_read = 0; bytes_read < num_bytes; bytes_read++) {
        /* Check if data is available */
        if (fifo->tail != fifo->head) {
            *temp_ptr = fifo->buf[fifo->tail];
            /* Increment buffer pointer and FIFO tail */
            temp_ptr++;
            fifo->tail++;
            /* Wrap around */
            if (fifo->tail == fifo->size) {
                fifo->tail = 0;
            }
        } else {
            /* No more data to read */
            return bytes_read;
        }
    }
    /* We read all bytes */
    return num_bytes;
}

/* Write a number of bytes to the FIFO */
u16_t sw_fifo_write(struct sw_fifo_t *fifo, const char *buf, u16_t num_bytes)
{
    u16_t bytes_written;
    const char *temp_ptr;

    temp_ptr = buf;

    for (bytes_written = 0; bytes_written < num_bytes; bytes_written++) {
        /* Check if there is space in the buffer */
        if (((fifo->head + 1) == fifo->tail) ||
            (((fifo->head+1) == fifo->size) && (fifo->tail == 0))) {
                /* No more space to write data */
                return bytes_written;
        } else {
            fifo->buf[fifo->head] = *temp_ptr;
            /* Increment head and buffer ptr */
            temp_ptr++;
            fifo->head++;
            /* Wrap around */
            if (fifo->head == fifo->size) {
                fifo->head = 0;
            }
        }
    }
    /* We wrote all the bytes */
    return num_bytes;
}

/* Returns the current amount of free space in the FIFO */
u16_t sw_fifo_get_free_space(struct sw_fifo_t *fifo)
{
    if (fifo->head == fifo->tail) {
        /* FIFO Empty */
        return fifo->size;
    } else if (((fifo->head + 1) == fifo->tail) ||
            (((fifo->head+1) == fifo->size) && (fifo->tail == 0))) {
                /* FIFO Full */
        return 0;
    } else {
        /*
         * Subtract 1 as we use that as we sacrifice 1 position as a flag
         * to check if the FIFO is full/empty
         */
        if (fifo->tail < fifo->head) {
            return (fifo->size - fifo->head - 1);
        } else {
            return (fifo->tail - fifo->head - 1);
        }
    }
}