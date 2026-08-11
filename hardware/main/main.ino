#include "wifiController.h"
#include "micController.h"
#include "ledThreadController.h"

int16_t samples[512];
size_t bytesRead = 0;
const int button = 2;
int led = 13;
bool recording = false;
bool first = true;

void toggleRecording() {
  if (!digitalRead(button) && !recording) {
    Serial.println("Button pressed! Starting recording...");
    recording = true;
    first = true;
    connectWebSocket(); 
    delay(300); // Debounce
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
  // Check button press at the beginning of the loop
  toggleRecording();

  if (recording) {
    // Update LED indicators for active recording state
    digitalWrite(led, LOW);
    changeColor(GREEN);
    currentAnimation = CIRCLE;

    // Process WebSocket events
    webSocket.loop();

    // Handle server-initiated stop command ("STOP")
    if (serverStopRequested) {
      recording = false;
      serverStopRequested = false;
      webSocket.disconnect();
      Serial.println("Recording stopped by server.");
      return;
    }

    // Stream I2S audio frames once connected
    if (webSocket.isConnected()) {
      if (readMic(samples, sizeof(samples), &bytesRead)) {
        if (bytesRead > 0) {
          sendAudioChunk((const uint8_t*)samples, bytesRead);
        }
      }
    }
  } else {
    // Idle state indicators
    digitalWrite(led, HIGH);
    changeColor(RED);
    currentAnimation = BREATH;

    if (first) {
      Serial.println("Waiting for button...");
      first = false;
    }
  }

  delay(2);
}