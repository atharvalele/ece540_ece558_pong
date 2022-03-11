#ifndef __UPD_H__
#define __UDP_H__

/**
 * upd: upd functionality wrapper
 * 
 * Author: Ayush Srivastava <ayush@pdx.edu>
 */

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncUDP.h>

#define UDP_TX_PORT         2001
#define UDP_RX_PORT         2000

// Packet Structure 
// Origin, Init, PlayerName1, PlayerName 2
// Origin, Score, Score 1, Score 2
// Player, PlayerID, Command, UP/DOWN


#define CONTROLLER          "NEXYS"
#define INIT                "INIT"
#define COMMAND             "COMMAND,"
#define PLAYER              "PLAYER,"
#define SCORE               "SCORE"
#define P1                  1
#define P2                  2

// Message Construction Switches
#define INIT_SWITCH         0
#define BROADCAST_SWITCH    1
#define PLAYER_SWITCH       2


struct player_t {
    IPAddress ip;
    String name;
};

void setupWiFi();
void udpListener();
void parsePacket(AsyncUDPPacket packet);
void sendName();
void sendMessage (int player, String packet);
void broadcastMessage(String receivedMessage);
#endif