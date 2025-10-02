#ifndef TOKEN_PROVISIONING_H
#define TOKEN_PROVISIONING_H

#include <Arduino.h>

class TokenProvisioning {
public:
    static String provisionToken(const String &serialNumber);
};

#endif
