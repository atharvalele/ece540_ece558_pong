#ifndef __PONG_H__
#define __PONG_H__

/**
 * Pong Arcade Game Implementation
 * 
 * Author: Atharva Lele <atharva@pdx.edu>
 */

#include "commondefs.h"
#include "psp_types.h"
#include "hagl_hal.h"

/* Defines */
#define PADDLE_UP             0
#define PADDLE_DOWN           1
#define PADDLE_MOVE_OFFSET    5

#define PADDLE_0_X_POS        40
#define PADDLE_1_X_POS        (DISPLAY_WIDTH - PADDLE_0_X_POS)

#define PADDLE_UPPER_BOUND    (PADDLE_HEIGHT / 2)
#define PADDLE_LOWER_BOUND    (DISPLAY_HEIGHT - (PADDLE_HEIGHT / 2))

#define PADDLE_HEIGHT         60
#define PADDLE_WIDTH          10
#define BALL_RADIUS           3

#define MAX_SCORE             10

/* Structs & typedefs */
// Game states 
typedef enum {
    PONG_INIT,
    PONG_WAIT_FOR_USER1,
    PONG_WAIT_FOR_USER2,
    PONG_WAIT_FOR_START,
    PONG_GAME_IN_PROGRESS,
    PONG_ROUND_OVER,
    PONG_GAME_OVER,
    PONG_LAST = 100
} pong_states_t;

// 2D Vector
typedef struct {
    s16_t x;
    s16_t y;
} vect_2d;

/* Variables */


/* Functions */
void pong_init(void);
void pong_task(void);
void pong_paddle_move(u08_t paddle, u08_t dir);

#endif