#include "TokenStorage.h"

Preferences prefs;

void saveTokenToFlash(const String &token) {
    prefs.begin("escotilha", false);
    prefs.putString("api_token", token);
    prefs.end();
}

String readTokenFromFlash() {
    prefs.begin("escotilha", true);
    String token = prefs.getString("api_token", "");
    prefs.end();
    return token;
}
