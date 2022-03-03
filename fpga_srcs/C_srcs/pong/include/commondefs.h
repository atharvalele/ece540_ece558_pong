#ifndef __COMMONDEFS_H__
#define __COMMONDEFS_H__

/**
 * Common Defines and macros
 * 
 * Author: Atharva Lele <atharva@pdx.edu>
 */

/* Write to a register */
#define WRITE_REG(REG, OFFSET, VAL)     (*((volatile unsigned int*)(REG + (4 * OFFSET))) = VAL)

/* Read from a register */
#define READ_REG(REG, OFFSET)           (*((volatile unsigned int*)(REG + (4 * OFFSET))))

#endif