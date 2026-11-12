#line 1 "/mnt/local-ai/Projects/maradur/esp32s3_marauder/WigleFormat.h"
#pragma once

#include <stdint.h>

static const char WIGLE_CSV_COLUMNS[] =
  "MAC,SSID,AuthMode,FirstSeen,Channel,Frequency,RSSI,"
  "CurrentLatitude,CurrentLongitude,AltitudeMeters,AccuracyMeters,"
  "RCOIs,MfgrId,Type";

constexpr uint16_t wigleFrequencyMHz(uint16_t channel) {
  return (channel >= 1 && channel <= 13)
    ? 2407 + (channel * 5)
    : (channel == 14)
      ? 2484
      : (channel >= 32 && channel <= 177)
        ? 5000 + (channel * 5)
        : 0;
}
