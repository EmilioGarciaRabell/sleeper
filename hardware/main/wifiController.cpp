#include "wifiController.h"

const char* ssid = "INFINITUMEC5C_2.4";
const char* password = "F5gfK65KNC";

const char* serverHost = "192.168.1.119";
const uint16_t serverPort = 5000;
const char* serverPath = "/receive_data";

bool isConnected = false;
bool serverStopRequested = false;
WebSocketsClient webSocket;

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case WStype_TEXT:
            {
                // Convert the incoming payload to a String
                String message = "";
                for (size_t i = 0; i < length; i++) {
                    message += (char)payload[i];
                }

                if (message == "STOP") {
                    Serial.println("Received STOP from server!");
                    serverStopRequested = true;  // <--- THIS TRIGGERS THE STOP IN YOUR MAIN LOOP
                }
            }
            break;
        case WStype_DISCONNECTED:
            Serial.println("[WSc] Disconnected!");
            break;
        case WStype_CONNECTED:
            Serial.println("[WSc] Connected!");
            break;
        default:
            break;
    }
}

void wifiSetup() {
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }

    webSocket.begin(serverHost, serverPort, serverPath);
    webSocket.onEvent(webSocketEvent);
    webSocket.setReconnectInterval(5000);
}

bool connectWebSocket() {
    unsigned long start = millis();

    while (!webSocket.isConnected() && (millis() - start) < 15000) {
        webSocket.loop();
        delay(20);
    }

    return webSocket.isConnected();
}

void sendAudioChunk(const uint8_t* data, size_t len) {
    if (!webSocket.isConnected()) {
        return;
    }

    webSocket.sendBIN(data, len);
}