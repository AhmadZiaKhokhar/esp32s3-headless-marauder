#line 1 "/mnt/local-ai/Projects/maradur/esp32s3_marauder/CensorText.h"
#pragma once

#include <stddef.h>

namespace CensorText {
bool looksLikeMac(const char* value);
void name(char* value);
void mac(char* value);
void identifier(char* value);
}
