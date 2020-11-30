//
// Created by S. Jansen.
//

#ifndef ESP32TALLYLIGHT_SERIAL_H
#define ESP32TALLYLIGHT_SERIAL_H

#include <Arduino.h>
#include "serialProtocol.h"
#include "config.h"

namespace SerialCom {
    void handle();

    void flushSerialReadBuffer();

    void wifiSsidSet(const char *buffer, uint8_t length);

    void wifiSsidGet();

    void wifiPasswordSet(const char *buffer, uint8_t length);

    void wifiPasswordGet();

    void handleSsidUpdated();

    void handlePasswordUpdated();

    void bootIntoConfig();

    void restart();
}

#endif //ESP32TALLYLIGHT_SERIAL_H
