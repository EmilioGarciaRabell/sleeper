#include "ledThreadController.h"

TaskHandle_t LedAnimator = NULL;
const int red = A7;
const int green = A6;

int currentLed = red;

void ledThreadSetup(){
    pinMode(red, OUTPUT);
    pinMode(green, OUTPUT);
    xTaskCreatePinnedToCore(
        animateLed,
        "LedAnimator",
        2048,           
        NULL,
        1,              
        &LedAnimator,
        0

    );
    Serial.println("Led thread created");
}

void animateLed( void * parameter ){
    for (;;){
        breathLed(currentLed);
    }
}

void breathLed(int led){
  for (int i = 5; i<255; i++){
    analogWrite(led, i);
    delay(5);
  }

  for (int i = 255; i > 0; i--){
    analogWrite(led,i);
    delay(5);
  }
}
