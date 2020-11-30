//
// Created by S. Jansen.
//

#include "config.h"

void Config::setup() {
    EEPROM.begin(EEPROM_SIZE);
}

void EEPROMupdate(const int address, const char value) {
    if (EEPROM.read(address) == (uint8_t) value) {
        return;
    }

    EEPROM.write(address, value);
    EEPROM.commit();
}

void Config::wifiSsidSet(const char *input, uint8_t length) {
    // Debug
    Serial.print("[Config] Set Wifi Ssid = ");
    Serial.write(input);
    Serial.print(" [");
    Serial.print(length);
    Serial.println("]");

    EEPROMupdate(EEPROM_WIFI_SSID_ADDRESS, length);

    for (int i = 0;
         i < length && (EEPROM_WIFI_SSID_ADDRESS + 1 + i) < EEPROM_WIFI_PASSWORD_ADDRESS;
         i++) {

        EEPROMupdate(EEPROM_WIFI_SSID_ADDRESS + 1 + i, input[i]);
    }
}

const char *Config::wifiSsidGet() {
    uint8_t length = EEPROM.read(EEPROM_WIFI_SSID_ADDRESS);

    // If value hasn't been initialized in the EEPROM
    if (length > 64) {
        length = 0;
    }

    char *output = (char *) malloc(length + 1);

    for (int i = 0; i < length; i++) {
        output[i] = EEPROM.read(EEPROM_WIFI_SSID_ADDRESS + 1 + i);
    }
    output[length] = '\0';

    // Debug
    Serial.print("[Config] Get Wifi Ssid = ");
    Serial.write(output);
    Serial.print(" [");
    Serial.print(length);
    Serial.println("]");

    return output;
}

void Config::wifiPasswordSet(const char *input, const uint8_t length) {
    // Debug
    Serial.print("[Config] Set Wifi password = ");
    Serial.write(input);
    Serial.print(" [");
    Serial.print(length);
    Serial.println("]");

    EEPROMupdate(EEPROM_WIFI_PASSWORD_ADDRESS, length);

    for (int i = 0; i < length; i++) {
        EEPROMupdate(EEPROM_WIFI_PASSWORD_ADDRESS + 1 + i, input[i]);
    }
}

const char *Config::wifiPasswordGet() {
    uint8_t length = EEPROM.read(EEPROM_WIFI_PASSWORD_ADDRESS);

    // If value hasn't been initialized in the EEPROM
    if (length > 64) {
        length = 0;
    }

    char *output = (char *) malloc(length + 1);

    for (int i = 0; i < length; i++) {
        output[i] = EEPROM.read(EEPROM_WIFI_PASSWORD_ADDRESS + 1 + i);
    }
    output[length] = '\0';

    // Debug
    Serial.print("[Config] Get Wifi password = ");
    Serial.write(output);
    Serial.print(" [");
    Serial.print(length);
    Serial.println("]");

    return output;
}

boolean bootIntoConfig = false;

void Config::bootIntoConfigSet(boolean value) {
    bootIntoConfig = value;
}

boolean Config::bootIntoConfigGet() {
    return bootIntoConfig;
}
