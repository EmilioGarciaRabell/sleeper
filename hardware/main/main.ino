#include "wifiController.h"
#include "micController.h"
#include "ledThreadController.h"


int32_t samples[512];
const int button = 4;

bool recording = false;

bool first = true;

int retries = 3;

void setup() {

  pinMode(button, INPUT_PULLUP);

  Serial.begin(115200); 

  setupMic();
  wifiSetup();
  ledThreadSetup();
  
  }

void loop() {

  currentLed = red; // Led control variable

  size_t bytesRead = 0;

  // Print only one time
  if (first){
    Serial.println("Wating for button...");
    first = false;
  }

  // Check if button was pressed
  toggleRecording();
  
  // If the button is pressed start recording and seding requests
  while(recording){
    // change LED status
    currentLed = green;

    first = true;

    webSocket.loop();

    // check if the socket is connected, if not try to reconnect 
    if (!isConnected){
      retries --;
      Serial.println("Retrying connection...");
      if (retries <= 0){
        recording = false;
        Serial.println("Connection Failed.");
      }
      delay(1000);
    }

    // If the socket is connected read the mic and send the bytes read
    if (webSocket.isConnected()){    
      if(readMic(samples, sizeof(samples), &bytesRead)) {
        // send data with client
        if (bytesRead > 0){
          sendAudioChunk(serverHost, (const uint8_t*)samples, bytesRead);
          
        }
      }
    }
  }
 
  delay(2);
   
}

// Function to check the status of the button and change the recording state
void toggleRecording(){
  if (!digitalRead(button)){
    Serial.println("Button pressed!");
    recording = !recording;
    delay(500);
  }
}

