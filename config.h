//
// Created by S. Jansen.
//

#ifndef ESP32TALLYLIGHT_CONFIG_H
#define ESP32TALLYLIGHT_CONFIG_H

#include <Arduino.h>
#include <EEPROM.h>

#define EEPROM_WIFI_SSID_ADDRESS 0
#define EEPROM_WIFI_PASSWORD_ADDRESS 64

namespace Config {
    void wifiSsidSet(const char *input, uint8_t length);

    const char *wifiSsidGet();

    void wifiPasswordSet(const char *input, uint8_t length);

    const char *wifiPasswordGet();
}
#endif //ESP32TALLYLIGHT_CONFIG_H
