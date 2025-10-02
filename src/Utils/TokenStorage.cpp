#include "TokenStorage.h"

Preferences prefs;

void saveTokenToFlash(const String &token) {
    prefs.begin("escotilha", false);
    Serial.print("Salvando token na flash: ");
    Serial.println(token);
    prefs.putString("api_token", token);
    prefs.end();
}

String readTokenFromFlash() {
    prefs.begin("escotilha", true);
    String token = prefs.getString("api_token", "");
    Serial.print("Lido da flash: '");
    Serial.print(token);
    Serial.println("'");
    prefs.end();
    return token;
}
