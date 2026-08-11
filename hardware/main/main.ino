#include "wifiController.h"
#include "micController.h"
#include "ledThreadController.h"

int16_t samples[512];
size_t bytesRead = 0;
const int button = 2;
int led = 13;
bool recording = false;
bool first = true;
int retries = 3;

void toggleRecording() {
  if (!digitalRead(button) && !recording) {
    Serial.println("Button pressed! Starting recording...");
    recording = true;
    delay(500); // debounce
  }
}

void setup() {
  pinMode(button, INPUT_PULLUP);
  pinMode(led, OUTPUT);

  Serial.begin(115200);

  setupMic();
  wifiSetup();
  ledThreadSetup();
}

void loop() {
  webSocket.loop();

  if (serverStopRequested) {
    recording = false;
    serverStopRequested = false;
    webSocket.disconnect();
    Serial.println("Recording stopped by server timeout.");
  }
  
  toggleRecording();

  if (recording) {
    digitalWrite(led, LOW);
    changeColor(GREEN);
    currentAnimation = CIRCLE;
    first = true;

    if (!webSocket.isConnected()) {
      retries--;
      Serial.println("Retrying connection...");
      if (!connectWebSocket()) {
        if (retries <= 0) {
          recording = false;
          retries = 3;
          Serial.println("Connection Failed.");
        }
        changeColor(PURPLE);
        currentAnimation = BREATH;
        delay(1000);
        return; 
      }
    }

    retries = 3;

    if (readMic(samples, sizeof(samples), &bytesRead)) {
      if (bytesRead > 0) {
        sendAudioChunk((const uint8_t*)samples, bytesRead);
      }
    }
  } else {
    digitalWrite(led, HIGH);
    changeColor(RED);
    currentAnimation = BREATH;

    if (first) {
      Serial.println("Wating for button...");
      first = false;
    }
  }

  delay(2);
}