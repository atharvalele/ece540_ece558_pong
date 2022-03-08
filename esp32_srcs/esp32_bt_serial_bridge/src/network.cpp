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

void setupWiFi() {
    // Setup ESP as WIFI Access Point
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);
    IPAddress IP = WiFi.softAPIP();
    Serial.println(IP);
}

void p1Parse(String data) {
    Serial.println(data);
}

void p2Parse(String data) {
    Serial.println(data);
}

void udpListener() {
    if(udp.listen(UPD_PORT)) {
        udp.onPacket([](AsyncUDPPacket packet) {
            if(p1_flag == false && p2_flag == false) {
                p1.ip = packet.remoteIP();
                p1.name = String((char *)packet.data());
                p1_flag = true;
                Serial.println(p1.ip);
                Serial.println(packet.length());
            }

            if(p2_flag == false && p1_flag == true && p1.ip != packet.remoteIP()) {
                p2.ip = packet.remoteIP();
                p2.name = String((char *)packet.data());    
                p2_flag = true;
                Serial.println(p2.ip);
                Serial.println(packet.length());
            }

            if(packet.remoteIP() == p1.ip) {
                Serial.println(packet.length());
                p1Parse(String((char *)packet.data()));
            }
            
            if(packet.remoteIP() == p2.ip) {
                Serial.println(packet.length());
                p2Parse(String((char *)packet.data()));
            }
        });
    }
}