#include <SPI.h>
#include "SdFat.h"


#define SD_CS_PIN SS

extern SdFat SD;
extern File dataFile;

void setupSD();

void writeToSD(String data);


void sendWavFile(const char* filename);