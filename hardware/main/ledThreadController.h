#ifndef LED_THREAD_CONTROLLER_H
#define LED_THREAD_CONTROLLER_H

#include <Arduino.h>

extern TaskHandle_t LedAnimator;

extern const int red;
extern const int green;
extern int currentLed;

void ledThreadSetup();

void animateLed( void * parameter );

void breathLed(int led);

#endif