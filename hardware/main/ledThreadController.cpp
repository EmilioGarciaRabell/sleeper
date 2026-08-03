#include "ledThreadController.h"

TaskHandle_t LedAnimator = NULL;
const int red = A7;
const int green = A6;

int currentLed = red;

float speedFactor = 0.03; 

int rgb[3] = {0, 0, 0};
extern enum Animation currentAnimation = BREATH;

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

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
  pixels.begin(); // INITIALIZE NeoPixel strip object
  pixels.show();

    for (;;){
        switch (currentAnimation){
            case BREATH:
                breathStrip();
                break;
            case CIRCLE:
                cycleStrip();
                break;
        }
    }
}

void changeColor(enum Colors color) {
    switch (color) {
        case GREEN:
            rgb[0] = 0;   rgb[1] = 255; rgb[2] = 0;
            break;
        case BLUE:
            rgb[0] = 0;   rgb[1] = 0;   rgb[2] = 255;
            break;
        case RED:
            rgb[0] = 255; rgb[1] = 0;   rgb[2] = 0;
            break;
        case PURPLE:
            rgb[0] = 128; rgb[1] = 0;   rgb[2] = 128;
            break;
        default:
            rgb[0] = 0;   rgb[1] = 0;   rgb[2] = 0;
            break;
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

void breathStrip(){
    
    for (float i = 0; i < 2 * PI; i += 0.05) {
        // sin() goes from -1 to 1. (sin() + 1) / 2 goes smoothly from 0 to 1.
        float brightnessMultiplier = (sin(i * speedFactor * 100) + 1.0) / 2.0; 

        // Scale the base RGB color by the current breathing brightness factor
        int r = rgb[0] * brightnessMultiplier;
        int g = rgb[1] * brightnessMultiplier;
        int b = rgb[2] * brightnessMultiplier;

        // Apply the color to all pixels
        for (int p = 0; p < NUMPIXELS; p++) {
            pixels.setPixelColor(p, pixels.Color(r, g, b));
        }
        
        pixels.show();
        delay(23); // Controls the framerate of the animation
    }
}

void cycleStrip() {
    int r = rgb[0];
    int g = rgb[1];
    int b = rgb[2];

    // Loop through each pixel one by one
    for (int p = 0; p < NUMPIXELS; p++) {
        // Clear all pixels first
        pixels.clear();

        // Turn on the current pixel using the active RGB values
        pixels.setPixelColor(p, pixels.Color(r, g, b));
        
        // Push the changes to the strip
        pixels.show();
        
        // Delay controls how fast the light moves from one LED to the next
        delay(100); 
    }
}