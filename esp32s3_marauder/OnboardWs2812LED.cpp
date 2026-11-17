#include "OnboardWs2812LED.h"

#ifdef HAS_ONBOARD_WS2812

void OnboardWs2812LED::begin() {
    FastLED.addLeds<WS2812B, ONBOARD_WS2812_PIN, GRB>(pixel, 1)
        .setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(255);
    off();
}

void OnboardWs2812LED::setColor(const CRGB& color) {
    pixel[0] = color;
    FastLED.show();
}

void OnboardWs2812LED::attack() {
    setColor(CRGB::Red);
}

void OnboardWs2812LED::sniff() {
    setColor(CRGB::Blue);
}

void OnboardWs2812LED::off() {
    setColor(CRGB::Black);
}

#endif
