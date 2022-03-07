/**
 * bt: bluetooth functionality wrapper
 * 
 * Author: Atharva Lele <atharva@pdx.edu>
 */

#include "bt.h"


static BLEServer* pServer = NULL;

// Player 1
static BLECharacteristic* pCharacteristic_p1_name = NULL;
static BLECharacteristic* pCharacteristic_p1_command = NULL;

// Player 2
static BLECharacteristic* pCharacteristic_p2_name = NULL;
static BLECharacteristic* pCharacteristic_p2_command = NULL;

// Broadcast
static BLECharacteristic* pCharacteristic_broadcast_p1 = NULL;
static BLECharacteristic* pCharacteristic_broadcast_p2 = NULL;
static BLECharacteristic* pCharacteristic_broadcast_score1 = NULL;
static BLECharacteristic* pCharacteristic_broadcast_score2 = NULL;

bool playerOne = false;
bool playerTwo = false;


uint32_t value = 0;


class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        
        if(!playerOne && !playerTwo) {
            playerOne = true;
            value = 1;
            pCharacteristic_broadcast_p1->setValue((uint8_t*)&value, 1);
            Serial.println("Ready Player One");
            BLEDevice::startAdvertising();
        }
        else if (!playerTwo) {
            playerTwo = true;
        }
    };

    void onDisconnect(BLEServer* pServer) {
        int avail_1 = atoi(pCharacteristic_broadcast_p1->getValue().c_str());
        int avail_2 = atoi(pCharacteristic_broadcast_p2->getValue().c_str());
        if(avail_1 == 0) {
            playerOne = false;
            BLEDevice::startAdvertising();
        }
        else if (avail_2 == 0) {
            playerTwo = false;
            BLEDevice::startAdvertising();
        }
    }
};

/* Functions */
void btSetup()
{
    BLEDevice::init("Pong");
    BLEAddress address = BLEDevice::getAddress();
    Serial.println(address.toString().c_str());
    

    // Create the BLE Server
    pServer = BLEDevice::createServer();
    // Set onConnect Call Back
    pServer->setCallbacks(new MyServerCallbacks());


    // Broadcaset Service
    BLEService *pService_broadcast = pServer->createService(BROADCAST_SERVICE);
     
    // Broadcast Characteristics
    pCharacteristic_broadcast_p1 = pService_broadcast->createCharacteristic(
                                    P1_AVAILABLE,
                                    BLECharacteristic::PROPERTY_READ   |
                                    BLECharacteristic::PROPERTY_WRITE  |
                                    BLECharacteristic::PROPERTY_NOTIFY |
                                    BLECharacteristic::PROPERTY_INDICATE
                                    );

    pCharacteristic_broadcast_p2 = pService_broadcast->createCharacteristic(
                                    P2_AVAILABLE,
                                    BLECharacteristic::PROPERTY_READ   |
                                    BLECharacteristic::PROPERTY_WRITE  |
                                    BLECharacteristic::PROPERTY_NOTIFY |
                                    BLECharacteristic::PROPERTY_INDICATE
                                    );

    pCharacteristic_broadcast_score1 = pService_broadcast->createCharacteristic(
                                    SCORE_P1,
                                    BLECharacteristic::PROPERTY_READ   |
                                    BLECharacteristic::PROPERTY_WRITE  |
                                    BLECharacteristic::PROPERTY_NOTIFY |
                                    BLECharacteristic::PROPERTY_INDICATE
                                    );

    pCharacteristic_broadcast_score2 = pService_broadcast->createCharacteristic(
                                    SCORE_P2,
                                    BLECharacteristic::PROPERTY_READ   |
                                    BLECharacteristic::PROPERTY_WRITE  |
                                    BLECharacteristic::PROPERTY_NOTIFY |
                                    BLECharacteristic::PROPERTY_INDICATE
                                    );

    // Player Services
    BLEService *pService_p1 = pServer->createService(SERVICE_UUID_PLAYER1);
    BLEService *pService_p2 = pServer->createService(SERVICE_UUID_PLAYER2);
    
    // Player 1 Characterisitics
    pCharacteristic_p1_name = pService_p1->createCharacteristic(
                            NAME_UUID,
                            BLECharacteristic::PROPERTY_READ   |
                            BLECharacteristic::PROPERTY_WRITE  |
                            BLECharacteristic::PROPERTY_NOTIFY |
                            BLECharacteristic::PROPERTY_INDICATE
                            );
    pCharacteristic_p1_name->addDescriptor(new BLE2902());

    pCharacteristic_p1_command = pService_p1->createCharacteristic(
                            COMMAND_UUID,
                            BLECharacteristic::PROPERTY_READ   |
                            BLECharacteristic::PROPERTY_WRITE  |
                            BLECharacteristic::PROPERTY_NOTIFY |
                            BLECharacteristic::PROPERTY_INDICATE
                            );
    pCharacteristic_p1_command->addDescriptor(new BLE2902());


    // Player 2 Characteristics
    pCharacteristic_p2_name = pService_p2->createCharacteristic(
                            NAME_UUID,
                            BLECharacteristic::PROPERTY_READ   |
                            BLECharacteristic::PROPERTY_WRITE  |
                            BLECharacteristic::PROPERTY_NOTIFY |
                            BLECharacteristic::PROPERTY_INDICATE
                            );
    pCharacteristic_p2_name->addDescriptor(new BLE2902());

    pCharacteristic_p2_command = pService_p2->createCharacteristic(
                            COMMAND_UUID,
                            BLECharacteristic::PROPERTY_READ   |
                            BLECharacteristic::PROPERTY_WRITE  |
                            BLECharacteristic::PROPERTY_NOTIFY |
                            BLECharacteristic::PROPERTY_INDICATE
                            );
    pCharacteristic_p2_command->addDescriptor(new BLE2902());


    // Start services
    pService_broadcast->start();
    pService_p1->start();
    pService_p2->start();

    // Setup BLE Advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(BROADCAST_SERVICE);
    pAdvertising->addServiceUUID(SERVICE_UUID_PLAYER1);
    pAdvertising->addServiceUUID(SERVICE_UUID_PLAYER2);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
    
    // Start Advertising
    BLEDevice::startAdvertising();
    
}

void btComm()
{
    if(playerOne) {
        Serial.println(pCharacteristic_p1_name->getValue().c_str());
        delay(10);
    }

    if(playerTwo) {
        Serial.println(pCharacteristic_p1_name->getValue().c_str());
    }
}