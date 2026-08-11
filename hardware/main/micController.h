
#ifndef MIC_CONTROLLER_H
#define MIC_CONTROLLER_H

#include <Arduino.h>

void setupMic();
bool readMic(void* buffer, size_t maxBytes, size_t* bytesRead);

#endif
