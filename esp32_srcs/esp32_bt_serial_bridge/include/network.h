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

#define UPD_PORT    1234

struct player_t {
    IPAddress ip;
    String name;
};

void setupWiFi();
void udpListener();
void p2Parse(String data);
void p1Parse(String data);

#endif