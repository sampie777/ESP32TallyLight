cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
set(PROJECT_NAME ESP32TallyLight)
project(${PROJECT_NAME})

set(${CMAKE_PROJECT_NAME}_SKETCH ESP32TallyLight.ino)

#### Uncomment below additional settings as needed.
# set(${CMAKE_PROJECT_NAME}_BOARD mega)
# set(${CMAKE_PROJECT_NAME}_PORT /dev/ttyACM0)
# set(mega.build.mcu atmega2560)
# set(mega.upload.protocol wiring)
# set(mega.upload.speed 115200)

include_directories(/usr/share/arduino/libraries/WiFiClient)
include_directories(/home/prive/.arduino15/packages/esp32/hardware/esp32/1.0.4/libraries)
#include_directories(/home/prive/Arduino/libraries/Adafruit_GFX_Library)

generate_arduino_firmware(${CMAKE_PROJECT_NAME})
