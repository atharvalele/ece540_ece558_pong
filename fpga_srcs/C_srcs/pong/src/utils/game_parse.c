/**
 * Pong Game Parse logic for commands on Serial
 * 
 * Author: Ayush Srivastava <ayush@pdx.edu>, Atharva Lele <atharva@pdx.edu>
 */
#include "utils/game_parse.h"

static char message[UART_BUF_SIZE];

static u08_t player, command;

static u08_t name_len;

/* Message Packet 
 * G,1
 * R,1
 * P,0,0/1
 * P,1,0/1
 */
void comm_task(void)
{
    int i = 0;

    // If data received && Receive timeout over && packet length >= 3
    if (rx_flag && !rx_tout && (rx_index >= 3)) {
        // Copy data into buffer
        sw_fifo_read(&uart_rx_fifo, message, rx_index);

        // Parse
        while (i < rx_index) {
            // Game start message
            if ((message[i] == 'G') && (message[i + 1] == ',') && (message[i + 2] == '1')) {
                // Start game
                pong_set_state(PONG_GAME_START);
                i += 3;
            }
            // Round start message
            else if ((message[i] == 'R') && (message[i + 1] == ',') && (message[i + 2] == 'S')) {
                // Start round
                pong_set_state(PONG_GAME_START);
                i += 3;
            }
            // Both users connected, start 1st round
            else if ((message[i] == 'U') && (message[i + 1] == ',') && (message[i + 2] == 'C')) {
                // Start round
                pong_set_state(PONG_WAIT_FOR_START);
                i += 3;
            }
            // Restart from the beginning
            else if ((message[i] == 'S') && (message[i + 1] == 'O')) {
                // Restart from the beginning
                pong_set_state(PONG_INIT);
                break;
            }
            else if ((message[i] == 'R') && (message[i + 1] == 'S') && (message[i + 2] == 'T')) {
                // Reset from game start
                pong_set_state(PONG_RESTART);
                break;
            }
            // Winner name
            else if ((message[i] == 'W')) {
                name_len = 0;
                i += 2;
                // Copy into winner name
                while (message[i] != ',') {
                    winner_name[name_len] = message[i];
                    name_len++;
                    i++;
                    if (i > rx_index)
                        break;
                }
                winner_name[name_len] = '\0';
                write_winner_name();
            }
            // Paddle control message
            else if (message[i] == 'P') {
                if (pong_get_state() == PONG_GAME_IN_PROGRESS) {
                    player = message[i + 2] - '0';
                    command = message[i + 4] - '0';
                    paddle_move(player, command);
                }
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
