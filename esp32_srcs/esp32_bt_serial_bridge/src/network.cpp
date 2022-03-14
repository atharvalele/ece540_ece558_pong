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

static bool i_done = false;
static bool game_on = false;
static bool round_on = false;

String message, messageBuffer;

void setupWiFi() {
    // Setup ESP as WIFI Access Point
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);
    IPAddress IP = WiFi.softAPIP();
    Serial.println(IP);
}

void parsePacket(AsyncUDPPacket packet) {
    packet.setTimeout(0);
    if ((p1_flag == false) && (p2_flag == false)) {
        p1.ip = packet.remoteIP();
        p1.name = packet.readString();
        p1_flag = true;
        Serial.println(p1.name);
        return;
    }

    if ((p2_flag == false) && (p1_flag == true) && (p1.ip != packet.remoteIP())) {
        p2.ip = packet.remoteIP();
        p2.name = packet.readString();   
        p2_flag = true;
        Serial.println(p2.name);
        sendPlayerConnected();
        delay(500);
        sendName();
        return;
    }

    if ((p2_flag == true) && (p2_flag == true)) {
        messageBuffer = packet.readString();
        // If received packet is "SO" start over game
        if (messageBuffer == "SO") {
            Serial.println("Received Start Over");
            sendStartOver();
            return;
        }
        // if ((packet.readString().c_str()[0] == 'S') && (packet.readString().c_str()[1] == 'O')) {
        //     Serial.println("Received Start Over");
        //     sendStartOver();
        // }

        // If received packet is RST replay game with same players 
        //if ((packet.readString().c_str()[0] == 'R') && (packet.readString().c_str()[1] == 'S') && (packet.readString().c_str()[2] == 'T')) { 
        if (messageBuffer == "RST") {
            Serial.println("Received Replay");
            sendReplay();
            return;
        }

        if (packet.remoteIP() == p1.ip) {
            //Serial.println(packet.length());
            if ((game_on == false) && (round_on == false)) {
                sendGameStart();
                game_on = true;
            }

            if ((round_on == false)) {
                sendRoundStart();
                round_on = true;
            }
            sendMessagePong(P1, messageBuffer);
            // @TODO: Send message over serial to rvfpga
        } 
        else if (packet.remoteIP() == p2.ip) {
            //Serial.println(packet.length());

            if ((round_on == false)) {
                sendRoundStart();
                round_on = true;
            }

            sendMessagePong(P2, messageBuffer);
            // @TODO: Send message over serial to rvfpga
        }
    }
}

void sendPlayerConnected() {
    String message = "U,C";
    Serial.println(message);
    Serial1.println(message);
}

void sendGameStart() {
    String message = "G,1";
    Serial.println(message);
    Serial1.println(message);
}

void sendRoundStart() {
    String message = "R,S";
    Serial.println(message);
    Serial1.println(message);
}

void sendStartOver() {
    String startOver = "SO,";
    Serial.println(startOver);
    Serial1.println(startOver);
    game_on = false;
    round_on = false;
    p2_flag = false;
    p1_flag = false;
    AsyncUDPMessage gameAgain;
    gameAgain.println(startOver);
    udp.sendTo(gameAgain, p1.ip, UDP_TX_PORT);
    udp.sendTo(gameAgain, p2.ip, UDP_TX_PORT);
}

void sendReplay() {
    String startOver = "RST";
    Serial.println(startOver);
    Serial1.println(startOver);
    game_on = false;
    round_on = false;
    AsyncUDPMessage gameAgain;
    gameAgain.printf("%s,%s,%s",startOver.c_str(),p1.name.c_str(), p2.name.c_str());
    udp.sendTo(gameAgain, p1.ip, UDP_TX_PORT);
    udp.sendTo(gameAgain, p2.ip, UDP_TX_PORT);
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
    //Serial.println("Sending Names back");
    message.printf("%s,%s,%s", SERVER_READY, p1.name.c_str(), p2.name.c_str());
    udp.sendTo(message, p2.ip, UDP_TX_PORT);
    udp.sendTo(message, p1.ip, UDP_TX_PORT);
}

/**
 * Bypass Nexys Message received
 * over UDP 
 */
void broadcastMessage(String receivedMessage) {
    AsyncUDPMessage message;
    if (receivedMessage.c_str()[0] == 'I') {
        i_done = true;
    }
    else if (receivedMessage.c_str()[0] == 'G') {
        if ((receivedMessage.c_str()[4] -'0') == 0) {
            receivedMessage = "G,O," + p1.name;
            Serial.printf("W,%s,", p1.name.c_str());
            Serial1.printf("W,%s,", p1.name.c_str());
        } else {
            receivedMessage = "G,O," + p2.name;
            Serial.printf("W,%s,",p2.name.c_str());
            Serial1.printf("W,%s,",p2.name.c_str());
        }
        game_on = false;
        round_on = false;
    }
    else if (receivedMessage.c_str()[0] == 'B') {
        round_on = false;
    }
    Serial.println("Got from NEXYS");
    Serial.println(receivedMessage);
    message.println(receivedMessage);
    udp.sendTo(message, p2.ip, UDP_TX_PORT);
    udp.sendTo(message, p1.ip, UDP_TX_PORT);
}


/**
 * 
 * Messages Received from PLAYER
 * P.0/1,0/1
 */
void sendMessagePong (int player, String packet) {
    String messageString;
    // Construct Message to Send to NEXYS
    switch (player)
    {
        case P1: 
            messageString = PLAYER + String(",") + String(P1) + "," + packet;
            Serial.println(messageString);
            Serial1.println(messageString);
            break;
        case P2:
            messageString = PLAYER + String(",") + String(P2) + "," + packet;
            Serial.println(messageString);
            Serial1.println(messageString);
            break;
    }
}