/**
 * Pong Arcade Game Implementation
 * 
 * Author: Atharva Lele <atharva@pdx.edu>
 */

#include "game/pong.h"
#include <hagl.h>

#include "device/timer.h"

/* Variables */
s16_t paddle_1_x = PADDLE_1_X_POS;
s16_t paddle_1_y = 240;
s16_t paddle_2_x = PADDLE_2_X_POS;
s16_t paddle_2_y = 240;

s16_t ball_x = 320, ball_y = 240;

static pong_states_t pong_state = PONG_INIT;

/* Static Functions */
static void pong_paddles_reset(void)
{
    paddle_1_x = PADDLE_1_X_POS;
    paddle_1_y = 240;
    paddle_2_x = PADDLE_2_X_POS;
    paddle_2_y = 240;
}

static void pong_draw_ball(u08_t color)
{
    hagl_fill_rectangle(ball_x-3, ball_y-3,
                        ball_x+3, ball_y+3,
                        color);
}

static void pong_paddle_draw(u08_t paddle, u08_t color)
{
    s16_t x, y;
    if (paddle == 1) {
        x = PADDLE_1_X_POS;
        y = paddle_1_y;
    }
    else {
        x = PADDLE_2_X_POS;
        y = paddle_2_y;
    }

    hagl_fill_rectangle(x - (PADDLE_WIDTH / 2), y - (PADDLE_HEIGHT / 2), x + (PADDLE_WIDTH / 2), y + (PADDLE_HEIGHT / 2), color);
}

static void pong_init_display(void)
{
    u08_t exit_flag = 0;

    hagl_clear_screen();

    /* Draw initial positions of paddles & ball */
    pong_paddle_draw(1, 0xF);
    pong_paddle_draw(2, 0xF);

    pong_draw_ball(0xF);

    /* Animate paddle up and down */
    while (1) {
        if (exit_flag == 0) {
            paddle_1_move(PADDLE_UP);
            paddle_2_move(PADDLE_DOWN);
            if (paddle_1_y == PADDLE_UPPER_BOUND)
                exit_flag++;
        } else if (exit_flag == 1) {
            paddle_1_move(PADDLE_DOWN);
            paddle_2_move(PADDLE_UP);
            if (paddle_1_y == PADDLE_LOWER_BOUND)
                exit_flag++;
        } else {
            paddle_1_move(PADDLE_UP);
            paddle_2_move(PADDLE_DOWN);
            if (paddle_1_y == 240) {
                pong_paddles_reset();
                break;
            }
        }

        delay_ms(30);
    }
}

/* Functions */
void pong_init(void)
{
    pong_init_display();
}

void paddle_1_move(u08_t dir)
{
    s16_t pos;

    if (dir == PADDLE_UP)
        pos = paddle_1_y - PADDLE_MOVE_OFFSET;
    else
        pos = paddle_1_y + PADDLE_MOVE_OFFSET;

    if (pos > DISPLAY_HEIGHT)
        pos = PADDLE_LOWER_BOUND;
    else if (pos < 0)
        pos = PADDLE_UPPER_BOUND;

    if (pos != paddle_1_y) {
        /* Update only if position is changed */
        pong_paddle_draw(1, 0);
        paddle_1_y = pos;
        pong_paddle_draw(1, 0xF);
    }
}

void paddle_2_move(u08_t dir)
{
    s16_t pos;

    if (dir == PADDLE_UP)
        pos = paddle_2_y - PADDLE_MOVE_OFFSET;
    else
        pos = paddle_2_y + PADDLE_MOVE_OFFSET;

    if (pos > DISPLAY_HEIGHT)
        pos = PADDLE_LOWER_BOUND;
    else if (pos < 0)
        pos = PADDLE_UPPER_BOUND;

    if (pos != paddle_2_y) {
        /* Update only if position is changed */
        pong_paddle_draw(2, 0);
        paddle_2_y = pos;
        pong_paddle_draw(2, 0xF);
    }
}

/* Pong State Machine */
void pong_task(void)
{
    switch (pong_state) {
    case PONG_INIT:
        pong_init();
        pong_state = PONG_WAIT_FOR_USER1;
    break;

    case PONG_WAIT_FOR_USER1:
    break;

    case PONG_WAIT_FOR_USER2:
    break;

    case PONG_WAIT_FOR_START:
    break;

    case PONG_GAME_IN_PROGRESS:
    break;

    case PONG_ROUND_OVER:
    break;

    case PONG_GAME_OVER:
    break;

    default:
    break;
    }
}
