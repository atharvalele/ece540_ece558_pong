#ifndef __HAGL_HAL_H__
#define __HAGL_HAL_H__

/**
 * HAGL library HAL provider
 * 
 * Author: Atharva Lele <atharva@pdx.edu>
 */

#include "psp_types.h"
#include <bitmap.h>

typedef u16_t color_t;

#define DISPLAY_WIDTH       640
#define DISPLAY_HEIGHT      480
#define DISPLAY_DEPTH       4

#define HAGL_HAS_HAL_INIT
#define HAGL_HAS_HAL_CLEAR_SCREEN
#define HAGL_HAS_HAL_HLINE
#define HAGL_HAS_HAL_VLINE

bitmap_t *hagl_hal_init(void);
void hagl_hal_put_pixel(int16_t x0, int16_t y0, color_t color);
void hagl_hal_clear_screen();
void hagl_hal_hline(int16_t x0, int16_t y0, uint16_t width, color_t color);
void hagl_hal_vline(int16_t x0, int16_t y0, uint16_t height, color_t color);

#endif