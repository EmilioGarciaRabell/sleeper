#include "sdCardHandler.h"
#include "wifiController.h"
SdFat SD;
File dataFile;


void setupSD(){
    while(!Serial){
        ; // Wait
    }

    Serial.println("Initializing SD card...");

    if (!SD.begin(SD_CS_PIN)) {
    Serial.println("initialization failed!");
    return;
  }

    Serial.println("initialization done.");

}

void writeToSD(String data){
    dataFile = SD.open("data.txt", FILE_WRITE);
    if (dataFile) {
        dataFile.println(data);
        dataFile.close();
        Serial.println("Data written to SD card.");
    } else {
        Serial.println("error opening data.txt");
    }
}
