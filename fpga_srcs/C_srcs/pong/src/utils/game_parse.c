/**
 * Pong Game Parse logic for commands on Serial
 * 
 * Author Ayush Srivastava (you@domain.com)
 */
#include "utils/game_parse.h"

 //  rx_index rx_flag & rx_tout
static char message[UART_BUF_SIZE];

static u08_t player, command;
/* Message Packet 
 * @TODO Replace P1 and P2 on the esp side code
 *       Make a decisin on UP and Down Preferrably 0/1
 * G,1
 * R,1
 * P,0,0/1
 * P,1,0/1
 */
void comm_task(void)
{
    int i = 0;
    if (rx_flag && !rx_tout) {
        // Copy data into buffer
        sw_fifo_read(&uart_rx_fifo, message, rx_index);

        // Parse
        while (i <= rx_index) {
            if (message[i] == 'G') {
                // TODO: State Change Implementation
                // state = PONG_GAME_START
                //uart_str_send("GAME_START");
                i += 3;
            }
            else if (message[i] == 'R') {
                // TODO: State Change Implementation
                //uart_str_send("ROUND_START");
                i += 3;
            }
            else if (message[i] == 'P') {
                player = message[2] - '0';
                command = message[4] - '0';
                paddle_move(player, command);
                i += 5;
            }
            else {
                i++;
            }
        }

        // Reset Index and flags
        rx_index = 0;
        rx_flag = 0;
    }
}

void send_message(pong_states_t state, u08_t p1_score, u08_t p2_score)
{
    char game_status[10] = {0};
   
    switch(state) 
    {
        case PONG_INIT:
            break;
        case PONG_WAIT_FOR_USERS:
        break;
        case PONG_WAIT_FOR_START:
            break;
        case PONG_GAME_IN_PROGRESS:
            break;
        case PONG_LAST:
            break;
        case PONG_GAME_OVER:
            sprintf(game_status, "G,%d", p1_score);
            uart_str_send(game_status);
            break;
        case PONG_ROUND_OVER:
            sprintf(game_status, "R,%d,%d", p1_score, p2_score);
            uart_str_send(game_status);
            break;
    }
}