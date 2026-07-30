#include "wifiController.h"

const char* ssid = "LEGR 9871";//"TP-Link_50FB"; 
const char* password = "7h)2153K";

const char* serverHost = "192.168.137.209";
const uint16_t serverPort = 5001;
const char* serverPath = "/receive_data";

bool isConnected = true;
WebSocketsClient webSocket;



void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.println("[WSc] Disconnected!");
            isConnected = false;
            break;
        case WStype_CONNECTED:
            Serial.printf("[WSc] Connected to url: %s\n", payload);
            break;
        case WStype_TEXT:
            Serial.printf("[WSc] Text payload: %s\n", payload);
            break;
        case WStype_BIN:
            Serial.printf("[WSc] Binary payload length: %u\n", length);
            break;
        default:
            break;
    }
}

void wifiSetup() {
    

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" CONNECTED");

    // server connection
    webSocket.begin(serverHost, serverPort, serverPath);
    webSocket.onEvent(webSocketEvent);
    webSocket.setReconnectInterval(5000);
}




void sendAudioChunk(
    const char* serverUrl,
    const uint8_t* data,
    size_t len
)
{
    
    bool success = webSocket.sendBIN(data, len);
    
   
}
