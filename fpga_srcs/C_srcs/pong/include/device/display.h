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

#define VGA_ROWS                        640
#define VGA_COLS                        480

#define PIXEL_ON                        0xF
#define PIXEL_OFF                       0x0

void display_init();
void draw_pixel(u16_t row, u16_t col,  u08_t value);
void create_divider();
#endif //__DISPLAY_H__