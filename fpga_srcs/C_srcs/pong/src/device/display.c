/**
 * display: functions to draw supported drawing constructs
 * 
 * Pixel, Horizontal Line, Vertical Line supported
 * 
 * Authors: Ayush Srivastava <ayush@pdx.edu>, Atharva Lele <atharva@pdx.edu>
 */

#include "device/display.h"

/* Functions to accelerate drawing of horizontal / vertical lines */
static inline void draw_pixel_auto_inc_hline(u08_t val)
{
    WRITE_REG(VGA_BASE_ADDR, VGA_PIXEL_HL_INC_OFFSET, val);
}

static inline void draw_pixel_auto_dec_hline(u08_t val)
{
    WRITE_REG(VGA_BASE_ADDR, VGA_PIXEL_HL_DEC_OFFSET, val);
}

static inline void draw_pixel_auto_inc_vline(u08_t val)
{
    WRITE_REG(VGA_BASE_ADDR, VGA_PIXEL_VL_INC_OFFSET, val);
}

static inline void draw_pixel_auto_dec_vline(u08_t val)
{
    WRITE_REG(VGA_BASE_ADDR, VGA_PIXEL_VL_DEC_OFFSET, val);
}

void display_init() 
{
    /* Enable write for frame buffer */
    WRITE_REG(VGA_BASE_ADDR, VGA_WRITE_ENABLE_OFFSET, 1);
}

void display_deinit()
{
    /* Enable write for frame buffer */
    WRITE_REG(VGA_BASE_ADDR, VGA_WRITE_ENABLE_OFFSET, 0);
}

/* Calculate which pixel to turn ON */
static inline u32_t calculate_frame_addr(u16_t x, u16_t y)
{
    return ((y * VGA_ROWS) + x);
}

/* Write address, and pixel */
void draw_pixel(u16_t x, u16_t y, u08_t value) 
{
    u32_t addr = calculate_frame_addr(x, y);
    WRITE_REG(VGA_BASE_ADDR, VGA_PIXEL_ADR_OFFSET, addr);
    WRITE_REG(VGA_BASE_ADDR, VGA_PIXEL_DAT_OFFSET, value);
}

/* Clear display */
void display_clear()
{
    volatile u16_t i, j;

    draw_pixel(0, 0, 0);
    
    for (i = 0; i < 640; i++) {
        for (j = 0; j < 480; j++) {
            draw_pixel_auto_inc_hline(0);
        }
    }
}

/* Draw horizontal line */
void display_draw_hline(s16_t x0, s16_t y0, u16_t width, u08_t val)
{
    volatile u16_t w = width - 1;

    draw_pixel(x0, y0, val);

    while (w--) {
        draw_pixel_auto_inc_hline(val);
    }
}

/* Draw vertical line */
void display_draw_vline(s16_t x0, s16_t y0, u16_t height, u08_t val)
{
    volatile u16_t h = height - 1;
    draw_pixel(x0, y0, val);

    while (h--) {
        draw_pixel_auto_inc_vline(val);
    }
}
