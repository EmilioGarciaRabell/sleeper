#include "wifiController.h"
#include "micController.h"
#include "ledThreadController.h"


int32_t samples[512];
const int button = 2;

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

  changeColor(RED);
  size_t bytesRead = 0;
  currentAnimation = BREATH;

  // Print only one time
  if (first){
    Serial.println("Wating for button...");
    first = false;
  }

  // Check if button was pressed
  toggleRecording();
  
  // If the button is pressed start recording and sending requests
  while(recording){
    // change LED status
    changeColor(GREEN);
    currentAnimation = CIRCLE;
    first = true;

    webSocket.loop();
    
    // If the socket is connected read the mic and send the bytes read
    if (webSocket.isConnected()){    
      
      if(readMic(samples, sizeof(samples), &bytesRead)) {
        // send data with client
        if (bytesRead > 0){
          sendAudioChunk(serverHost, (const uint8_t*)samples, bytesRead);
        }
      }

      // retries = 3; // (Commented out)

    }
    else{
      // --- RETRY LOGIC COMMENTED OUT FOR TESTING ---
      /*
      retries --;
      Serial.println("Retrying connection...");
      if (retries <= 0){
        recording = false;
        Serial.println("Connection Failed.");
      }
      changeColor(PURPLE);
      currentAnimation = BREATH;
      delay(1000);
      */
    }
  }
 
  delay(2);
  // retries = 3; // (Commented out)
}

// Function to check the status of the button and change the recording state
void toggleRecording(){
  if (!digitalRead(button)){
    Serial.println("Button pressed!");
    recording = !recording;
    delay(500);
  }
}

