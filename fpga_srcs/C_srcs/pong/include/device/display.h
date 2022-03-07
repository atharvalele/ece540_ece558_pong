#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "common_types.h"
#include "commondefs.h"

/**
 * display: ability to draw one pixel on the screen
 * 
 * Authors: Ayush Srivastava <ayush@pdx.edu>, Atharva Lele <atharva@pdx.edu>
 */

#define VGA_BASE_ADDR                   0x80003000
#define VGA_WRITE_ENABLE_OFFSET         0x0
#define VGA_PIXEL_ADR_OFFSET            0x1
#define VGA_PIXEL_DAT_OFFSET            0x2
#define VGA_PIXEL_HL_INC_OFFSET         0x3
#define VGA_PIXEL_HL_DEC_OFFSET         0x4
#define VGA_PIXEL_VL_INC_OFFSET         0x5
#define VGA_PIXEL_VL_DEC_OFFSET         0x6

#define VGA_ROWS                        640
#define VGA_COLS                        480

#define PIXEL_ON                        0xF
#define PIXEL_OFF                       0x0

void display_init();
void display_clear();
void display_fill();

void draw_pixel(u16_t row, u16_t col,  u08_t value);
void display_draw_hline(s16_t x0, s16_t y0, u16_t width, u08_t val);
void display_draw_vline(s16_t x0, s16_t y0, u16_t height, u08_t val);

#endif //__DISPLAY_H__