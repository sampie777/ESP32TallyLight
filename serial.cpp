//
// Created by S. Jansen.
//

#include "serial.h"

boolean ssidUpdated = false;
boolean passwordUpdated = false;
uint8_t newSsidLength;
uint8_t newPasswordLength;
char *newSsid;
char *newPassword;

void SerialCom::handle() {
    if (ssidUpdated) {
        handleSsidUpdated();
    }
    if (passwordUpdated) {
        handlePasswordUpdated();
    }

    if (!Serial.available()) {
        return;
    }

    char buffer[SERIAL_MESSAGE_MAX_LENGTH] = {0};
    uint8_t length = Serial.readBytesUntil('\n', buffer, SERIAL_MESSAGE_MAX_LENGTH);
    flushSerialReadBuffer();

    if (length < SERIAL_COMMAND_LENGTH) {
        Serial.print("[Serial] Invalid input: ");
        Serial.write(buffer);
        Serial.print(" [");
        Serial.print(length);
        Serial.println("]");
        return;
    }

    if (strncmp(buffer, SERIAL_BOOT_INTO_CONFIG, SERIAL_COMMAND_LENGTH) == 0) {
        return bootIntoConfig();
    } else if (strncmp(buffer, SERIAL_WIFI_SSID_SET, SERIAL_COMMAND_LENGTH) == 0) {
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

void SerialCom::bootIntoConfig() {
    Config::bootIntoConfigSet(true);
}

void SerialCom::flushSerialReadBuffer() {
    while (Serial.available()) {
        Serial.read();
    }
}

void SerialCom::wifiSsidSet(const char *buffer, uint8_t length) {
    newSsidLength = length - SERIAL_COMMAND_LENGTH;
    const char *value = &buffer[SERIAL_COMMAND_LENGTH];

    newSsid = (char *) malloc(newSsidLength + 1);
    strncpy(newSsid, value, newSsidLength);
    newSsid[newSsidLength] = '\0';
    
    ssidUpdated = true;
}

void SerialCom::handleSsidUpdated() {
    ssidUpdated = false;

    // Set
    Config::wifiSsidSet(newSsid, newSsidLength);

    // Check
    const char *valueCheck = Config::wifiSsidGet();

    if (strcmp(newSsid, valueCheck) == 0) {
        Serial.println("[Serial] Value updated");
    } else {
        Serial.print("[Serial] Failed to update value: (new) ");
        Serial.write(newSsid);
        Serial.print(" != (old) ");
        Serial.write(valueCheck);
        Serial.print('\n');
    }

    free((char *) newSsid);
    free((char *) valueCheck);
}

void SerialCom::wifiSsidGet() {
    const char *value = Config::wifiSsidGet();
    Serial.print(SERIAL_WIFI_SSID_GET);
    Serial.print(":");
    Serial.write(value);
    Serial.print('\n');

    free((char *) value);
}

void SerialCom::wifiPasswordSet(const char *buffer, uint8_t length) {
    newPasswordLength = length - SERIAL_COMMAND_LENGTH;
    const char *value = &buffer[SERIAL_COMMAND_LENGTH];

    newPassword = (char *) malloc(newPasswordLength + 1);
    strncpy(newPassword, value, newPasswordLength);
    newPassword[newPasswordLength] = '\0';

    passwordUpdated = true;
}

void SerialCom::handlePasswordUpdated() {
    passwordUpdated = false;

    // Set
    Config::wifiPasswordSet(newPassword, newPasswordLength);

    // Check
    const char *valueCheck = Config::wifiPasswordGet();

    if (strcmp(newPassword, valueCheck) == 0) {
        Serial.println("[Serial] Value updated");
    } else {
        Serial.print("[Serial] Failed to update value: (new) ");
        Serial.write(newPassword);
        Serial.print(" != (old) ");
        Serial.write(valueCheck);
        Serial.print('\n');
    }

    free((char *) newPassword);
    free((char *) valueCheck);
}

void SerialCom::wifiPasswordGet() {
    const char *value = Config::wifiPasswordGet();
    Serial.print(SERIAL_WIFI_PASSWORD_GET);
    Serial.print(":");
    Serial.write(value);
    Serial.print('\n');

    free((char *) value);
}