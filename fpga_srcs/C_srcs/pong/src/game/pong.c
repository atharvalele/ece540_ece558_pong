/**
 * Pong Arcade Game Implementation
 *
 * Author: Atharva Lele <atharva@pdx.edu>
 */

#include "game/pong.h"
#include <hagl.h>

#include "device/seven_seg.h"
#include "device/timer.h"
#include "device/uart.h"
#include "font10x20-ISO8859-15.h"

#include "utils/printf.h"

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

static char message[10];

wchar_t winner_name[50];

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


// Move ball
static void ball_move()
{
    // Clear ball
    ball_draw(0);

    // Update position
    ball_pos.x += ball_speed.x;
    ball_pos.y += ball_speed.y;

    // Redraw
    ball_draw(0xF);
}

// Get ball Y-speed
static s16_t ball_get_y_speed(s16_t ball_y_pos, s16_t paddle_y_pos)
{
    s16_t offset = ball_y_pos - paddle_y_pos;

    if (offset > 5)
        return 2;
    else if (offset > 0 && offset < 5)
        return 1;
    else if (offset < 0 && offset > -5)
        return -1;
    else if (offset < -5)
        return -2;
    else
        return 0;
}

// Check if ball hit any paddle
static u08_t ball_check_paddle_hit(void)
{
    u08_t ball_went_out = 0;

    // Check if the ball hit either paddle
    if (ball_pos.x < BALL_LEFT_BOUND) {
        if ((ball_pos.y >= (paddle_pos[0].y - (PADDLE_HEIGHT / 2))) &&
            (ball_pos.y <= (paddle_pos[0].y + (PADDLE_HEIGHT / 2)))) {
                ball_speed.x = -ball_speed.x;
                ball_speed.y = ball_get_y_speed(ball_pos.y, paddle_pos[0].y);

                // Send ball hit message
                uart_str_send("H,0");
        } else {
            // Ball didn't hit, went out of bounds,
            // player 1 scores
            ball_went_out = 1;
            player_that_scored = 1;
        }
        
    } else if (ball_pos.x > BALL_RIGHT_BOUND) {
        if ((ball_pos.y >= (paddle_pos[1].y - (PADDLE_HEIGHT / 2))) &&
            (ball_pos.y <= (paddle_pos[1].y + (PADDLE_HEIGHT / 2)))) {
                ball_speed.x = -ball_speed.x;
                ball_speed.y = ball_get_y_speed(ball_pos.y, paddle_pos[1].y);

                // Send ball hit message
                uart_str_send("H,1");
        } else {
            // Ball didn't hit, went out of bounds,
            // player 0 scores
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
    // Write score to screen
    hagl_put_char(player_scores[0] + '0', P0_SCORE_X_POS, P_SCORE_Y_POS, 0xF, font10x20_ISO8859_15);
    hagl_put_char(player_scores[1] + '0', P1_SCORE_X_POS, P_SCORE_Y_POS, 0xF, font10x20_ISO8859_15);

    // Write score to 7-segment display
    seven_seg_disp_num(player_scores[0], 1);
    seven_seg_disp_num(player_scores[1], 0);
}

static void pong_init_animation(void)
{
    u08_t exit_flag = 0;

    u32_t i = 1;
    u08_t seven_seg_flag = 0;

    hagl_clear_screen();

    /* Draw initial positions of paddles & ball */
    paddle_draw(0, 0xF);
    paddle_draw(1, 0xF);
    ball_draw(0xF);


    /* Animate paddle up and down */
    while (1) {
        // Animate 7-seg display
        WRITE_REG(SEVEN_SEG_ADDR, 0, i);
        WRITE_REG(SEVEN_SEG_ADDR, 1, i);
        if (!seven_seg_flag) {
            i <<= 1;
            i |= 1;
            if (i >= 0x80000000) {
                seven_seg_flag = 1;
            }
        } else {
            i >>= 1;
            if (i == 0) {
                seven_seg_flag = 0;
                i = 1;
            }
        }
        
        if (exit_flag == 0) {
            paddle_move(0, PADDLE_UP);
            paddle_move(1, PADDLE_DOWN);
            if (paddle_pos[0].y <= PADDLE_UPPER_BOUND)
                exit_flag++;
        } else if (exit_flag == 1) {
            paddle_move(0, PADDLE_DOWN);
            paddle_move(1, PADDLE_UP);
            if (paddle_pos[0].y >= PADDLE_LOWER_BOUND)
                exit_flag++;
        } else {
            paddle_move(0, PADDLE_UP);
            paddle_move(1, PADDLE_DOWN);
            if (paddle_pos[0].y <= PADDLE_UPPER_BOUND) {
                paddle_draw(0, 0);
                paddle_draw(1, 0);
                paddles_reset();
                paddle_draw(0, 0xF);
                paddle_draw(1, 0xF);
                break;
            }
        }
        delay_ms(30);
    }
}

/* Reset paddles, ball, and redraw them */
static void reinit_paddles_ball(void)
{
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
}

/* Game over screen */
static void pong_display_game_over(void)
{
    wchar_t game_over[] = L"GAME OVER!";

    // Clear screen
    hagl_clear_screen();

    // Draw rectangle
    hagl_draw_rounded_rectangle(20, 20, DISPLAY_WIDTH - 20, DISPLAY_HEIGHT - 20, 10, 0xF);

    // Write Game over text
    hagl_put_text(game_over, (DISPLAY_WIDTH / 2) - 50, (DISPLAY_HEIGHT / 2) - 100,
                  0xF, font10x20_ISO8859_15);
}


/* Functions */

/* Write winner text on screen */
void write_winner_name(void)
{
    wchar_t win_text[] = L"Winner:";

    hagl_put_text(win_text, (DISPLAY_WIDTH / 2) - 80, (DISPLAY_HEIGHT / 2) + 100,
                  0xF, font10x20_ISO8859_15);

    hagl_put_text(winner_name, (DISPLAY_WIDTH / 2) - 0, (DISPLAY_HEIGHT / 2) + 100,
                  0xF, font10x20_ISO8859_15);
}

// Move paddle
void paddle_move(u08_t paddle, u08_t dir)
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

// Init function
void pong_init(void)
{
    // Reset paddles and ball position
    paddles_reset();
    ball_reset();

    // Blank out seven seg
    seven_seg_blank();

    // Show init animation
    pong_init_animation();

    // Reset scores
    player_scores[0] = 0;
    player_scores[1] = 0;
    winner = -1;
    
    // Start displaying scores on screen
    pong_update_scores();
}

// Game task
void pong_task(void)
{
    u08_t ball_went_out = 0;

    // Pong state machine

    switch (pong_state) {
    case PONG_INIT:
        pong_init();
        // Send Init Done message to ESP
        uart_str_send("I,D");
        pong_state = PONG_WAIT_FOR_USERS;
    break;

    case PONG_WAIT_FOR_USERS:
        // Wait here until pushed to next state
    break;

    case PONG_WAIT_FOR_START:
        // Wait here until pushed to next state
    break;

    case PONG_GAME_START:
        // Throw the ball in the direction of the 
        // player that won the last round
        // throw to the left if initial round
        if (player_that_scored == 1) {
            ball_speed.x = 1;
        } else {
            ball_speed.x = -1;
        }
        ball_speed.y = 0;

        // Signal start of game
        pong_started = 1;
        pong_state = PONG_GAME_IN_PROGRESS;
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
                // Reset all game objects
                reinit_paddles_ball();
                // Move on to next state
                pong_state = PONG_ROUND_OVER;
            }
        }
    break;

    case PONG_ROUND_OVER:
        // Increment scores
        player_scores[player_that_scored]++;

        // Check if game over
        if (player_scores[0] == MAX_SCORE) {
            winner = 0;
            pong_state = PONG_GAME_OVER;
        } else if (player_scores[1] == MAX_SCORE) {
            winner = 1;
            pong_state = PONG_GAME_OVER;
        } else {
            // Update game score on screen
            pong_update_scores();
            sprintf(message, "B,%d,%d", player_scores[0], player_scores[1]);

            uart_str_send(message);

            // Go to round over wait
            pong_state = PONG_ROUND_OVER_WAIT;
        }
    break;

    case PONG_ROUND_OVER_WAIT:
        // Wait here until pushed into GAME IN PROGRESS state by comm_task()
    break;

    case PONG_GAME_OVER:
        // Send over UART
        sprintf(message, "G,O,%d", winner);
        uart_str_send(message);
        
        // Display game over
        pong_display_game_over();

        // Blank out 7-segment display
        seven_seg_blank();

        // Reset game objects
        reinit_paddles_ball();

        // Give the ball a speed
        ball_speed.x = 1;

        pong_state = PONG_GAME_OVER_WAIT;
    break;

    case PONG_GAME_OVER_WAIT:
        // Wait here until pushed into next state by comm_task()
        if (pong_render) {
            pong_render = 0;
            ball_move();
            ball_check_paddle_hit();
        }
    break;

    case PONG_RESTART:
        // Clear screen
        hagl_clear_screen();

        // Reset ball and scores but don't start from animation
        reinit_paddles_ball();
        player_scores[0] = 0;
        player_scores[1] = 0;
        pong_update_scores();

        pong_state = PONG_WAIT_FOR_START;
    break;

    default:
    break;
    }
}

// Update state
void pong_set_state(pong_states_t state)
{
    pong_state = state;
}

// Get state
pong_states_t pong_get_state(void)
{
    return pong_state;
}