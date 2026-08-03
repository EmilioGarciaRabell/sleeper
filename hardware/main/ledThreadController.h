#ifndef LED_THREAD_CONTROLLER_H
#define LED_THREAD_CONTROLLER_H

#include <Arduino.h>
#include <stdio.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        6 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 45 // Popular NeoPixel ring size

 enum Colors {
  BLUE,
  RED,
  GREEN,
  PURPLE
};

enum Animation{
  BREATH,
  CIRCLE
};


extern TaskHandle_t LedAnimator;

extern const int red;
extern const int green;
extern int currentLed;
extern enum Animation currentAnimation;

extern int rgb[3];

void ledThreadSetup();

void animateLed( void * parameter );

void breathLed(int led);

void changeColor(enum Colors color);

void cycleStrip(); 

void breathStrip();

#endif