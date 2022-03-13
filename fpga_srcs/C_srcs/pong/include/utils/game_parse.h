#ifndef __GAME_PARSE_H__
#define __GAME_PARSE_H__
/**
 * Author : Ayush Srivastava <ayush@pdx.edu>
 * Parse Logic of commands of the game
 */
#include <hagl.h>
#include <string.h>
#include "psp_types.h"

#include "game/pong.h"

#include "device/uart.h"
#include "device/timer.h"

#include "utils/printf.h"

void send_message(pong_states_t state, u08_t p1_score, u08_t p2_score);
void comm_task(void);

#endif //__GAME_PARSE_H__