/**
 * display: ability to draw one pixel on the screen
 * 
 * Author: Ayush Srivastava <ayush@pdx.edu>
 */

#include "device/display.h"

void display_init() 
{
    /* Enable write for frame buffer */
    /* From testing we can leave it enabled */
    WRITE_REG(VGA_BASE_ADDR, VGA_ENABLE_OFFSET, 1);

    for(int i = 0; i < 640; i++)
        for(int j = 0; j < 480; j++)
            draw_pixel(i, j, PIXEL_OFF);
}

u32_t calculate_frame_addr(u16_t row, u16_t col)
{
    return ((row * VGA_ROW) + col);
}

void draw_pixel(u16_t row, u16_t col, u08_t value) 
{
    WRITE_REG(VGA_BASE_ADDR, VGA_PIXEL_ADR_OFFSET, calculate_frame_addr(row, col));
    WRITE_REG(VGA_BASE_ADDR, VGA_PIXEL_DAT_OFFSET, value);
}

void create_divider()
{
    for (int i = 0; i < 640; i++)
        draw_pixel(i, 240, PIXEL_ON);
}