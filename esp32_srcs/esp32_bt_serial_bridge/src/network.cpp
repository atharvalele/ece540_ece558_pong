/**
 * upd: upd functionality wrapper
 * 
 * Author: Ayush Srivastava <ayush@pdx.edu>
 */

#include "network.h"

const char * ssid = "PONG";
const char * password = "12345678";

static player_t p1, p2;

static AsyncUDP udp;

static bool p1_flag = false, p2_flag = false;

String message;

void setupWiFi() {
    // Setup ESP as WIFI Access Point
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);
    IPAddress IP = WiFi.softAPIP();
    Serial.println(IP);
}

void parsePacket(AsyncUDPPacket packet) {

    if ((p1_flag == false) && (p2_flag == false)) {
        p1.ip = packet.remoteIP();
        p2.name = packet.readString();
        p1_flag = true;
        //Serial.println(p1.ip);
        return;
    }

    if ((p2_flag == false) && (p1_flag == true) && (p1.ip != packet.remoteIP())) {
        p2.ip = packet.remoteIP();
        p2.name = packet.readString();   
        p2_flag = true;
        sendName();
        //Serial.println(p2.ip);
        return;
    }

    if ((p2_flag == true) && (p2_flag == true)) {
        if (packet.remoteIP() == p1.ip) {
            //Serial.println(packet.length());
            sendMessage(P1, packet.readString());
            // @TODO: Send message over serial to rvfpga
        } 
        else if (packet.remoteIP() == p2.ip) {
            //Serial.println(packet.length());
            sendMessage(P2, packet.readString());
            // @TODO: Send message over serial to rvfpga
        }
    }
}



void udpListener() {
    if (udp.listen(UDP_RX_PORT)) {
        udp.onPacket([](AsyncUDPPacket packet) {
            packet.setTimeout(0);
            parsePacket(packet);
        });
    }
}

// Send Packet to devices
void sendName() {
    AsyncUDPMessage message;
    message.printf("%s,%s,%s,%s", CONTROLLER, INIT, p1.name.c_str(), p2.name.c_str());
    udp.sendTo(message, p2.ip, UDP_TX_PORT);
    udp.sendTo(message, p1.ip, UDP_TX_PORT);
}

void broadcastMessage(String receivedMessage) {
    AsyncUDPMessage message;
    message.println(receivedMessage);
    udp.sendTo(message, p2.ip, UDP_TX_PORT);
    udp.sendTo(message, p1.ip, UDP_TX_PORT);
}


/**
 * 
 * Messages Received
 * PLAYER,P1/P2,COMMAND,UP/DOWN
 */
void sendMessage (int player, String packet) {
    String messageString;
    switch (player)
    {
        case P1: 
            messageString = PLAYER + P1 + String(COMMAND) + packet;
            Serial.println(messageString);
            break;
        case P2:
            messageString = PLAYER + P2 + String(COMMAND) + packet;
            Serial.println(messageString);
            break;
    }
}