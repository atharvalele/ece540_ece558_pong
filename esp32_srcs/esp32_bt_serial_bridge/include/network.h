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

#define UDP_TX_PORT 2001
#define UDP_RX_PORT 2000

/**
 * Packet Strucrure 
 * Game Start G,1
 * Round Start R,1
 * Player Commands P,0/1,0/1
 */


// Nexys Packets
#define GAME_START   "G"
#define ROUND_START  "R"
#define BROADCAST    "B"

// ESP UDP Message Packets
#define SERVER_READY "S"
#define P1           0
#define P2           1

// Messages to NEXYS
#define PLAYER "P"

// Deprecated Packet Structure 
#define INIT "1"
#define COMMAND "COMMAND,"

#define SCORE "SCORE"

struct player_t
{
    IPAddress ip;
    String name;
};

void setupWiFi();
void udpListener();
void parsePacket(AsyncUDPPacket packet);
void sendPlayerConnected();
void sendGameStart();
void sendRoundStart();
void sendName();
void sendMessagePong(int player, String packet);
void broadcastMessage(String receivedMessage);
#endif