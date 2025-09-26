#pragma once
#include <Arduino.h>
#include <Preferences.h>

void saveTokenToFlash(const String &token);
String readTokenFromFlash();