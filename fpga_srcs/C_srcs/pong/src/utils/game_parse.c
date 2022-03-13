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
            // Paddle control message
            else if (message[i] == 'P') {
                player = message[i + 2] - '0';
                command = message[i + 4] - '0';
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
