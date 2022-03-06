#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "common_types.h"
#include "commondefs.h"

/**
 * display: ability to draw one pixel on the screen
 * 
 * Author: Ayush Srivastava <ayush@pdx.edu>
 */

#define VGA_BASE_ADDR           0x80003000
#define VGA_ENABLE_OFFSET       0x0
#define VGA_PIXEL_ADR_OFFSET    0x1
#define VGA_PIXEL_DAT_OFFSET    0x2

#define VGA_ROW                 640
#define VGA_COL                 480

#define PIXEL_ON                0xF
#define PIXEL_OFF               0x0

void display_init();
void draw_pixel(u16_t row, u16_t col,  u08_t value);
u32_t calculate_frame_addr(u16_t row, u16_t col);
void create_divider();
#endif //__DISPLAY_H__