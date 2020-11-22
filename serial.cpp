//
// Created by S. Jansen.
//

#include "serial.h"

void SerialCom::handle() {
    if (!Serial.available()) {
        return;
    }

    char buffer[SERIAL_MESSAGE_MAX_LENGTH] = {0};
    uint8_t length = Serial.readBytesUntil('\n', buffer, SERIAL_MESSAGE_MAX_LENGTH);
    flushSerialReadBuffer();

    if (length < SERIAL_COMMAND_LENGTH) {
        return;
    }

    if (strncmp(buffer, SERIAL_WIFI_SSID_SET, SERIAL_COMMAND_LENGTH) == 0) {
        return wifiSsidSet(buffer, length);
    } else if (strncmp(buffer, SERIAL_WIFI_SSID_GET, SERIAL_COMMAND_LENGTH) == 0) {
        return wifiSsidGet();
    } else if (strncmp(buffer, SERIAL_WIFI_PASSWORD_SET, SERIAL_COMMAND_LENGTH) == 0) {
        return wifiPasswordSet(buffer, length);
    } else if (strncmp(buffer, SERIAL_WIFI_PASSWORD_GET, SERIAL_COMMAND_LENGTH) == 0) {
        return wifiPasswordGet();
    }

    Serial.print("[Serial] Invalid input: ");
    Serial.write(buffer);
    Serial.print(" [");
    Serial.print(length);
    Serial.println("]");
}

void SerialCom::flushSerialReadBuffer() {
    while (Serial.available()) {
        Serial.read();
    }
}

void SerialCom::wifiSsidSet(const char *buffer, uint8_t length) {
    uint8_t valueLength = length - SERIAL_COMMAND_LENGTH;
    const char *value = &buffer[SERIAL_COMMAND_LENGTH];

    // Set
    Config::wifiSsidSet(value, valueLength);

    // Check
    const char *valueCheck = Config::wifiSsidGet();

    if (strcmp(value, valueCheck) == 0) {
        Serial.println("[Serial] Value updated");
        return;
    }

    Serial.print("[Serial] Failed to update value: (new) ");
    Serial.write(value);
    Serial.print(" != (old) ");
    Serial.write(valueCheck);
    Serial.print('\n');

    free((char *) valueCheck);
}

void SerialCom::wifiSsidGet() {
    const char *value = Config::wifiSsidGet();
    Serial.write(value);
    Serial.print('\n');

    free((char *) value);
}

void SerialCom::wifiPasswordSet(const char *buffer, uint8_t length) {
    uint8_t valueLength = length - SERIAL_COMMAND_LENGTH;
    const char *value = &buffer[SERIAL_COMMAND_LENGTH];

    // Set
    Config::wifiPasswordSet(value, valueLength);

    // Check
    const char *valueCheck = Config::wifiPasswordGet();

    if (strcmp(value, valueCheck) == 0) {
        Serial.println("[Serial] Value updated");
        return;
    }

    Serial.print("[Serial] Failed to update value: (new) ");
    Serial.write(value);
    Serial.print(" != (old) ");
    Serial.write(valueCheck);
    Serial.print('\n');

    free((char *) valueCheck);
}

void SerialCom::wifiPasswordGet() {
    const char *value = Config::wifiPasswordGet();
    Serial.write(value);
    Serial.print('\n');

    free((char *) value);
}