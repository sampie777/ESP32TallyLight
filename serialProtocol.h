//
// Created by S. Jansen.
//

// Serial commands can be send using the following syntax:
//   <command><value>\n
//
// Example of setting the Wifi password to '1234':
//   wps1234\n
//
// Example of getting the set Wifi password:
//   wpg\n
// This will (among debug logging) return the set Wifi password in plain text:
//   1234
//

#ifndef ESP32TALLYLIGHT_SERIALPROTOCOL_H
#define ESP32TALLYLIGHT_SERIALPROTOCOL_H

#define SERIAL_COMMAND_LENGTH 3
#define SERIAL_MESSAGE_MAX_LENGTH 64

// First `SERIAL_COMMAND_LENGTH` chars are the command to perform:
#define SERIAL_RESTART "rst"
#define SERIAL_BOOT_INTO_CONFIG "cnf"
#define SERIAL_WIFI_SSID_SET "wss"
#define SERIAL_WIFI_SSID_GET "wsg"
#define SERIAL_WIFI_PASSWORD_SET "wps"
#define SERIAL_WIFI_PASSWORD_GET "wpg"

#endif //ESP32TALLYLIGHT_SERIALPROTOCOL_H
