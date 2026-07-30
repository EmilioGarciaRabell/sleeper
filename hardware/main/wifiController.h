#ifndef WIFI_CONTROLLER_H
#define WIFI_CONTROLLER_H

#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>

extern const char* ssid;
extern const char* password;

extern const char* serverHost;
extern const uint16_t serverPort;
extern const char* serverPath;
extern WebSocketsClient webSocket;

extern bool isConnected;


void wifiSetup();
void sendAudioChunk(
    const char* serverUrl,
    const uint8_t* data,
    size_t len
);
#endif