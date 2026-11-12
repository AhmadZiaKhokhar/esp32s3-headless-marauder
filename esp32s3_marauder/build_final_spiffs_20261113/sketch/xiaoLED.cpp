#line 1 "/mnt/local-ai/Projects/maradur/esp32s3_marauder/xiaoLED.cpp"
#include "xiaoLED.h"

void xiaoLED::RunSetup() {
    pinMode(XIAO_LED_PIN, OUTPUT);

if (!settings_obj.loadSetting<bool>("EnableLED")) {
    digitalWrite(XIAO_LED_PIN, HIGH);
    return;
}

delay(50);

  digitalWrite(XIAO_LED_PIN, LOW);
  delay(500);
  digitalWrite(XIAO_LED_PIN, HIGH);
  delay(250);
  digitalWrite(XIAO_LED_PIN, LOW);
  delay(500);
  digitalWrite(XIAO_LED_PIN, HIGH);
  delay(250);
  digitalWrite(XIAO_LED_PIN, LOW);
  delay(500);
  digitalWrite(XIAO_LED_PIN, HIGH);
}

void xiaoLED::attackLED() {
  if (!settings_obj.loadSetting<bool>("EnableLED"))
    return;
    
  digitalWrite(XIAO_LED_PIN, HIGH);
  delay(300);
  digitalWrite(XIAO_LED_PIN, LOW);
}

void xiaoLED::sniffLED() {
  if (!settings_obj.loadSetting<bool>("EnableLED"))
    return;
    
  digitalWrite(XIAO_LED_PIN, HIGH);
  delay(300);
  digitalWrite(XIAO_LED_PIN, LOW);
}

void xiaoLED::offLED() {
  if (!settings_obj.loadSetting<bool>("EnableLED"))
    return;
    
  digitalWrite(XIAO_LED_PIN, HIGH);
}

void xiaoLED::main() {
  // do nothing
}