#include "hagl_hal.h"
#include "device/display.h"

/**
 * HAGL library HAL provider
 * 
 * Provide basic drawing functions to
 * the drawing library
 * 
 * Author: Atharva Lele <atharva@pdx.edu>
 */


/* HAL init -- enable writing to framebuffer */
bitmap_t *hagl_hal_init(void)
{
    display_init();
    return NULL;
}

/* Clear the screen, basically paint it black */
void hagl_hal_clear_screen(void)
{
    display_clear();
}

/* Draw one pixel on the screen at given location */
void hagl_hal_put_pixel(int16_t x0, int16_t y0, color_t color)
{
    draw_pixel(x0, y0, color);
}

/* Draw horizontal line */
void hagl_hal_hline(int16_t x0, int16_t y0, uint16_t width, color_t color)
{
    display_draw_hline(x0, y0, width, color);
}

/* Draw vertical line */
void hagl_hal_vline(int16_t x0, int16_t y0, uint16_t height, color_t color)
{
    display_draw_vline(x0, y0, height, color);
}

