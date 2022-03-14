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
#define PADDLE_MOVE_OFFSET    20

#define PADDLE_0_X_POS        40
#define PADDLE_1_X_POS        (DISPLAY_WIDTH - PADDLE_0_X_POS)

#define PADDLE_UPPER_BOUND    (PADDLE_HEIGHT / 2)
#define PADDLE_LOWER_BOUND    (DISPLAY_HEIGHT - (PADDLE_HEIGHT / 2))

#define PADDLE_HEIGHT         80
#define PADDLE_WIDTH          10
#define BALL_RADIUS           3

#define BALL_LEFT_BOUND       ((PADDLE_0_X_POS + PADDLE_WIDTH / 2) + BALL_RADIUS + 2)
#define BALL_RIGHT_BOUND      ((PADDLE_1_X_POS - PADDLE_WIDTH / 2) - BALL_RADIUS - 2)
#define BALL_UPPER_BOUND      (BALL_RADIUS / 2)
#define BALL_LOWER_BOUND      (DISPLAY_HEIGHT - (BALL_RADIUS / 2))

#define P0_SCORE_X_POS        10
#define P1_SCORE_X_POS        (DISPLAY_WIDTH - P0_SCORE_X_POS - 20)
#define P_SCORE_Y_POS         10

#define MAX_SCORE             10

#define GAME_RENDER_MS        5

/* Structs & typedefs */
// Game states 
typedef enum {
    PONG_INIT,
    PONG_WAIT_FOR_USERS,
    PONG_WAIT_FOR_START,
    PONG_GAME_START,
    PONG_GAME_IN_PROGRESS,
    PONG_ROUND_OVER,
    PONG_ROUND_OVER_WAIT,
    PONG_GAME_OVER,
    PONG_GAME_OVER_WAIT,
    PONG_RESTART,
    PONG_LAST = 100
} pong_states_t;

// 2D Vector
typedef struct {
    s16_t x;
    s16_t y;
} vect_2d;

/* Variables */
extern volatile u08_t pong_render;
extern u08_t pong_started;


/* Functions */
void pong_init(void);
void pong_task(void);
void pong_set_state(pong_states_t state);
pong_states_t pong_get_state(void);
void paddle_move(u08_t paddle, u08_t dir);

#endif