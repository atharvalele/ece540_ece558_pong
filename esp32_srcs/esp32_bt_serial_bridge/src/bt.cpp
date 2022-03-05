/**
 * bt: bluetooth functionality wrapper
 * 
 * Author: Atharva Lele <atharva@pdx.edu>
 */

#include "bt.h"

/* Bluetooth Serial Object */
static BluetoothSerial SerialBT;

/* Functions */
void btSetup()
{
    SerialBT.begin("PONG");
}