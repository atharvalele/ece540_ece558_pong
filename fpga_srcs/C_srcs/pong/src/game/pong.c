/**
 * Pong Arcade Game Implementation
 *
 * Author: Atharva Lele <atharva@pdx.edu>
 */

#include "game/pong.h"
#include <hagl.h>

#include "device/timer.h"
#include "font10x20-ISO8859-15.h"

/* Global variables */
volatile u08_t pong_render = 0;
u08_t pong_started = 0;

/* Static Variables */
static vect_2d paddle_pos[2];
static vect_2d ball_pos, ball_speed;

static u08_t player_scores[2];
static s08_t player_that_scored;
static s08_t winner;

static pong_states_t pong_state = PONG_INIT;

/* Static Functions */

// Reset paddles to starting positions
static void paddles_reset(void)
{
    paddle_pos[0].x = PADDLE_0_X_POS;
    paddle_pos[0].y = DISPLAY_HEIGHT / 2;

    paddle_pos[1].x = PADDLE_1_X_POS;
    paddle_pos[1].y = DISPLAY_HEIGHT / 2;
}

// Reset ball to starting position
static void ball_reset(void)
{
    // Reset position
    ball_pos.x = DISPLAY_WIDTH / 2;
    ball_pos.y = DISPLAY_HEIGHT / 2;

    // Reset speed
    ball_speed.x = 0;
    ball_speed.y = 0;
}

// Draw paddles
static void paddle_draw(u08_t paddle, u08_t color)
{
    s16_t x, y;

    // Set paddle positions
    x = paddle_pos[paddle].x;
    y = paddle_pos[paddle].y;

    // Library call to draw the rectangle
    hagl_fill_rectangle(x - (PADDLE_WIDTH / 2), y - (PADDLE_HEIGHT / 2),
                        x + (PADDLE_WIDTH / 2), y + (PADDLE_HEIGHT / 2),
                        color);
}

// Draw ball
static void ball_draw(u08_t color)
{
    hagl_fill_rectangle(ball_pos.x - BALL_RADIUS, ball_pos.y - BALL_RADIUS,
                        ball_pos.x + BALL_RADIUS, ball_pos.y + BALL_RADIUS,
                        color);
}

// Move paddle
static void paddle_move(u08_t paddle, u08_t dir)
{
    s16_t pos;

    // Move paddle by offset, make sure that it doesn't
    // go outside the screen
    if (dir == PADDLE_UP)
        pos = paddle_pos[paddle].y - PADDLE_MOVE_OFFSET;
    else
        pos = paddle_pos[paddle].y + PADDLE_MOVE_OFFSET;

    if (pos > DISPLAY_HEIGHT)
        pos = PADDLE_LOWER_BOUND;
    else if (pos < 0)
        pos = PADDLE_UPPER_BOUND;

    /* Update only if position is changed */
    if (pos != paddle_pos[paddle].y) {
        paddle_draw(paddle, 0);
        paddle_pos[paddle].y = pos;
        paddle_draw(paddle, 0xF);
    }
}

// Move ball
static void ball_move()
{
    // Clear ball
    ball_draw(0);

    // Update position
    ball_pos.x += ball_speed.x;
    ball_pos.y += ball_speed.y / 2;

    // Redraw
    ball_draw(0xF);
}

// Check if ball hit any paddle
static u08_t ball_check_paddle_hit(void)
{
    u08_t ball_went_out = 0;

    if (ball_pos.x < BALL_LEFT_BOUND) {
        if ((ball_pos.y >= (paddle_pos[0].y - (PADDLE_HEIGHT / 2))) &&
            (ball_pos.y <= (paddle_pos[0].y + (PADDLE_HEIGHT / 2)))) {
                ball_speed.x = -ball_speed.x;
                ball_speed.y = (ball_pos.y - paddle_pos[0].y) % 3;
        } else {
            ball_went_out = 1;
            player_that_scored = 1;
        }
        
    } else if (ball_pos.x > BALL_RIGHT_BOUND) {
        if ((ball_pos.y >= (paddle_pos[1].y - (PADDLE_HEIGHT / 2))) &&
            (ball_pos.y <= (paddle_pos[1].y + (PADDLE_HEIGHT / 2)))) {
                ball_speed.x = -ball_speed.x;
                ball_speed.y = (ball_pos.y - paddle_pos[1].y) % 3;
        } else {
            ball_went_out = 1;
            player_that_scored = 0;
        }
    }

    return ball_went_out;
}

// Check if ball hits screen boundary
static void ball_check_boundary_hit(void)
{
    if ((ball_pos.x > BALL_LEFT_BOUND) && (ball_pos.x < BALL_RIGHT_BOUND) &&
        ((ball_pos.y < BALL_UPPER_BOUND) || (ball_pos.y > BALL_LOWER_BOUND))) {
            // Reverse Y-axis direction
            ball_speed.y = -ball_speed.y;
    }
}

// Update scores
static void pong_update_scores(void)
{
    // Placeholder
    // Add player score update functions here
}

static void pong_init_animation(void)
{
    u08_t exit_flag = 0;

    hagl_clear_screen();

    pong_update_scores();

    /* Draw initial positions of paddles & ball */
    paddle_draw(0, 0xF);
    paddle_draw(1, 0xF);

    ball_draw(0xF);

    /* Animate paddle up and down */
    while (1) {
        if (exit_flag == 0) {
            paddle_move(0, PADDLE_UP);
            paddle_move(1, PADDLE_DOWN);
            if (paddle_pos[0].y == PADDLE_UPPER_BOUND)
                exit_flag++;
        } else if (exit_flag == 1) {
            paddle_move(0, PADDLE_DOWN);
            paddle_move(1, PADDLE_UP);
            if (paddle_pos[0].y == PADDLE_LOWER_BOUND)
                exit_flag++;
        } else {
            paddle_move(0, PADDLE_UP);
            paddle_move(1, PADDLE_DOWN);
            if (paddle_pos[0].y == (DISPLAY_HEIGHT / 2)) {
                paddles_reset();
                break;
            }
        }

        delay_ms(30);
    }
}

/* Functions */

// Init function
void pong_init(void)
{
    // Reset paddles and ball position
    paddles_reset();
    ball_reset();

    // Show init animation
    pong_init_animation();

    // Reset scores
    player_scores[0] = 0;
    player_scores[1] = 0;
}

// Game task
void pong_task(void)
{
    u08_t ball_went_out = 0;

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
        if (pong_render) {
            pong_render = 0;
            
            // Move the ball
            ball_move();

            // Check if it hit the paddles
            ball_went_out = ball_check_paddle_hit();

            // If ball went out of bounds, round is over
            if (!ball_went_out) {
                ball_check_boundary_hit();
            } else {
                // Erase paddles & Ball
                paddle_draw(0, 0);
                paddle_draw(1, 0);
                ball_draw(0);

                // Reset positions
                paddles_reset();
                ball_reset();

                // Redraw
                paddle_draw(0, 0xF);
                paddle_draw(1, 0xF);
                ball_draw(0xF);

                // Move on to next state
                pong_state = PONG_ROUND_OVER;
            }
        }
    break;

    case PONG_ROUND_OVER:
        // Increment scores
        player_scores[player_that_scored]++;

        // Check if game over
        if (player_scores[0] == 10) {
            winner = 0;
            pong_state = PONG_GAME_OVER;
        } else if (player_scores[1] == 10) {
            winner = 1;
            pong_state = PONG_GAME_OVER;
        }
    break;

    case PONG_GAME_OVER:
    break;

    default:

    break;
    }
}