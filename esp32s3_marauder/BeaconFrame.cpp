#line 1 "/mnt/local-ai/Projects/maradur/esp32s3_marauder/BeaconFrame.cpp"
#include "BeaconFrame.h"

bool setBeaconFrameChannel(
  uint8_t* frame,
  size_t frame_size,
  size_t ssid_length,
  uint8_t channel
) {
  const size_t channel_index = 50 + ssid_length;
  if ((frame == nullptr) || (channel_index >= frame_size))
    return false;

  frame[channel_index] = channel;
  return true;
}
