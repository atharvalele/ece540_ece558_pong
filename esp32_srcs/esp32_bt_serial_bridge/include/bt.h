#ifndef __BT_H__
#define __BT_H__

/**
 * bt: bluetooth functionality wrapper
 * 
 * Author: Atharva Lele <atharva@pdx.edu>
 *         Ayush Srivastava <ayush@pdx.edu>
 */

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include "BluetoothSerial.h"

// Player Service
#define SERVICE_UUID_PLAYER1        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define SERVICE_UUID_PLAYER2        "f3f4e3f4-9dad-11ec-b909-0242ac120002"

// Player Characteristics
#define NAME_UUID                   "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define COMMAND_UUID                "997e1c1b-6561-4cfc-b95e-614a61061568"

// Broadcast Service and Characteristics
#define BROADCAST_SERVICE           "6664570b-df5a-47c1-948f-6bffcca0e89a"
#define P1_AVAILABLE                "3ba8d42a-59a0-4929-9164-4748cc4f6f38"
#define P2_AVAILABLE                "34841d8f-2ff8-4299-9d8d-9557f23f0086"
#define SCORE_P1                    "8a084c4a-7e58-4ead-b870-5cb5e081a0fa"
#define SCORE_P2                    "a9337d8e-f3fe-4f8a-b50f-f97f6c8cc077"

/* Functions */
void btSetup();
void btComm();
#endif