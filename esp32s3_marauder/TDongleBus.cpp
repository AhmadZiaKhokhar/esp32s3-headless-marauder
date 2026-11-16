#line 1 "/mnt/local-ai/Projects/maradur/esp32s3_marauder/TDongleBus.cpp"
#include "TDongleBus.h"

#include <Arduino.h>

void deselectTDongleSharedSpi(uint8_t tftCsPin, uint8_t sdCsPin) {
  // The APA102 shares the TFT/SD clock and data lines on shipped T-Dongles.
  pinMode(tftCsPin, OUTPUT);
  digitalWrite(tftCsPin, HIGH);
  pinMode(sdCsPin, OUTPUT);
  digitalWrite(sdCsPin, HIGH);
}
