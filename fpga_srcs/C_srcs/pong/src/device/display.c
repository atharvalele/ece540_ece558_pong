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

static inline u32_t calculate_frame_addr(u16_t x, u16_t y)
{
    return ((y * VGA_ROW) + x);
}

void draw_pixel(u16_t x, u16_t y, u08_t value) 
{
    u32_t addr = calculate_frame_addr(x, y);
    WRITE_REG(VGA_BASE_ADDR, VGA_PIXEL_ADR_OFFSET, addr);
    WRITE_REG(VGA_BASE_ADDR, VGA_PIXEL_DAT_OFFSET, value);
}

void create_divider()
{
    for (int i = 0; i < 640; i++)
        draw_pixel(i, 240, PIXEL_ON);
}