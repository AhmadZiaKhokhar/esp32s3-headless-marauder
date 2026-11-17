#pragma once

#include "configs.h"

#ifdef HAS_ONBOARD_WS2812
#include <FastLED.h>

class OnboardWs2812LED {
public:
    void begin();
    void attack();
    void sniff();
    void off();

private:
    CRGB pixel[1];
    void setColor(const CRGB& color);
};

extern OnboardWs2812LED onboard_ws2812_led;
#endif
