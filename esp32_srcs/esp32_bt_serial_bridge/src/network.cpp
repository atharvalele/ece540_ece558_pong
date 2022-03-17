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
    // Setup the SSID and Password
    WiFi.softAP(ssid, password);
    IPAddress IP = WiFi.softAPIP();
    Serial.println(IP);
}

void parsePacket(AsyncUDPPacket packet) {
    packet.setTimeout(0);
    // Assign Player 1 IP and Name
    if ((p1_flag == false) && (p2_flag == false)) {
        p1.ip = packet.remoteIP();
        p1.name = packet.readString();
        // On first message set flag true
        p1_flag = true;
        Serial.println(p1.name);
        return;
    }

    // Wait for Player 2 to connect and send names
    // No data from Player 1 is accepted till player 2 connects
    if ((p2_flag == false) && (p1_flag == true) && (p1.ip != packet.remoteIP())) {
        p2.ip = packet.remoteIP();
        p2.name = packet.readString();   
        // Set flag true 
        p2_flag = true;
        Serial.println(p2.name);
        // Send Player Connected Status to Nexys
        sendPlayerConnected();
        delay(500);
        // Send names to both android devices
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
        // If received packet is "RST" replay game
        if (messageBuffer == "RST") {
            Serial.println("Received Replay");
            sendReplay();
            return;
        }

        // Send commands received from player 1 to nexys
        if (packet.remoteIP() == p1.ip) {
            //Serial.println(packet.length());
            // Check if game hasn't started
            if ((game_on == false) && (round_on == false)) {
                sendGameStart();
                game_on = true;
            }

            // Check if round has started
            if ((round_on == false)) {
                sendRoundStart();
                round_on = true;
            }
            sendMessagePong(P1, messageBuffer);
            // @TODO: Send message over serial to rvfpga
        } 

        // Send commands received from player 2 to nexys
        else if (packet.remoteIP() == p2.ip) {
            //Serial.println(packet.length());

            // Check if round has started
            if ((round_on == false)) {
                sendRoundStart();
                round_on = true;
            }

            sendMessagePong(P2, messageBuffer);
            // @TODO: Send message over serial to rvfpga
        }
    }
}
/* Helper Functions */

// Send Player Connected to Nexys
void sendPlayerConnected() {
    String message = "U,C";
    Serial.println(message);
    Serial1.println(message);
}

// Send Game Start Command to Nexys
void sendGameStart() {
    String message = "G,1";
    Serial.println(message);
    Serial1.println(message);
}

// Send Round Start command to Nexys
void sendRoundStart() {
    String message = "R,S";
    Serial.println(message);
    Serial1.println(message);
}

// Send Start Over command to Nexys
void sendStartOver() {
    String startOver = "SO,";
    Serial.println(startOver);
    Serial1.println(startOver);
    // Set the Game flags to false
    game_on = false;
    round_on = false;
    // Reset Player 1 and 2 Status
    p2_flag = false;
    p1_flag = false;
    AsyncUDPMessage gameAgain;
    gameAgain.println(startOver);
    // Synchronize Android Fragments
    udp.sendTo(gameAgain, p1.ip, UDP_TX_PORT);
    udp.sendTo(gameAgain, p2.ip, UDP_TX_PORT);
}

// Send Replay Command to Nexys
void sendReplay() {
    String startOver = "RST";
    Serial.println(startOver);
    Serial1.println(startOver);
    // Reset Game Flags 
    game_on = false;
    round_on = false;
    AsyncUDPMessage gameAgain;
    gameAgain.printf("%s,%s,%s",startOver.c_str(),p1.name.c_str(), p2.name.c_str());
    // Synchtonize Android Fragments
    udp.sendTo(gameAgain, p1.ip, UDP_TX_PORT);
    udp.sendTo(gameAgain, p2.ip, UDP_TX_PORT);
}

// UDP Listener for constantly listenting on the UDP_RX_PORT
void udpListener() {
    if (udp.listen(UDP_RX_PORT)) {
        udp.onPacket([](AsyncUDPPacket packet) {
            packet.setTimeout(0);
            // Call Parse Packet
            parsePacket(packet);
        });
    }
}

// Send Names to devices
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
    // Check for Initialization Done message
    if (receivedMessage.c_str()[0] == 'I') {
        i_done = true;
    }
    // Check for Game Over Message from Nexys
    else if (receivedMessage.c_str()[0] == 'G') {
        // Check if player 1 won or player 2 won
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
    // Check for Broadcast message from the Nexys
    else if (receivedMessage.c_str()[0] == 'B') {
        round_on = false;
    }
    // Check for the Haptic Feedback message from the Nexys 
    else if (receivedMessage.c_str()[0] == 'H') {
        if (receivedMessage.c_str()[2] == '0') {
            // Send Vibration Message to Player 1
            message.println("V,1");
            udp.sendTo(message, p1.ip, UDP_TX_PORT);
            return;
        } else {
            // Send Vibration Message to Player 2
            message.println("V,1");
            udp.sendTo(message, p2.ip, UDP_TX_PORT);
            return;
        }
    }

    Serial.println("Got from NEXYS");
    Serial.println(receivedMessage);
    message.println(receivedMessage);
    // Transmit message to Both android devices
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