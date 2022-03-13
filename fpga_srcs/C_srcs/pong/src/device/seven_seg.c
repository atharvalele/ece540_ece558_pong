/**
 * Seven Segment Display
 * 
 * Routines to write to the 7-Segment display on board
 * 
 * Author: Atharva Lele <atharva@pdx.edu>
 */

#include "device/seven_seg.h"

/* Static Variables */
static u08_t num_char_val[10] = {SEG_0, SEG_1, SEG_2, SEG_3, SEG_4,
                                 SEG_5, SEG_6, SEG_7, SEG_8, SEG_9};


/* Functions */

/* Blank out displays */
void seven_seg_blank(void)
{
    // each position corresponds to 4 bytes (32-bit register)
    WRITE_REG(SEVEN_SEG_ADDR, 0, 0xFFFFFFFFUL);
    WRITE_REG(SEVEN_SEG_ADDR, 1, 0xFFFFFFFFUL);
}

/* Write a number to the display */
void seven_seg_disp_num(u08_t num, u08_t pos)
{
    u32_t val = 0xFFFFFFFF;
    u08_t c;

    // Blank the display
    WRITE_REG(SEVEN_SEG_ADDR, pos, val);

    // Generate the value to write
    // Blank out middle two letters
    // Write correct values acc to number
    val &= ~0x00FFFF00;
    c = num_char_val[num / 10];
    val |= (c << 16);
    c = num_char_val[num % 10];
    val |= (c << 8);

    // Write the value
    WRITE_REG(SEVEN_SEG_ADDR, pos, val);
}

/* Write a char to the display */
void seven_seg_disp_char(char c, u08_t pos)
{

}