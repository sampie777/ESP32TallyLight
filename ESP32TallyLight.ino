#define USING_ARDUINO false

#include <Arduino.h>

#if !USING_ARDUINO

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#endif

#include "config.h"
#include "serial.h"

#define HOST_NAME "esp32_2"
#define LED_BUILTIN 2
#define LIGHT_PIN 27
#define IDENT_BUTTON_PIN 26
#define IDENT_BUTTON_GND_PIN 33

uint8_t tallyLightStatus = LOW;

#if !USING_ARDUINO
WebServer server(80);

void handleRoot() {
    digitalWrite(LED_BUILTIN, HIGH);
    server.send(200, "text/html", "<a href='/on'>On</a><br/><a href='/off'>Off</a>");
    digitalWrite(LED_BUILTIN, LOW);
}

void handleLightOn() {
    digitalWrite(LED_BUILTIN, HIGH);
    tallyLightStatus = HIGH;
    digitalWrite(LIGHT_PIN, tallyLightStatus);
    server.send(200, "text/plain", "on");
    digitalWrite(LED_BUILTIN, LOW);
}

void handleLightOff() {
    digitalWrite(LED_BUILTIN, HIGH);
    tallyLightStatus = LOW;
    digitalWrite(LIGHT_PIN, tallyLightStatus);
    server.send(200, "text/plain", "off");
    digitalWrite(LED_BUILTIN, LOW);
}

void handleNotFound() {
    digitalWrite(LED_BUILTIN, HIGH);
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i = 0; i < server.args(); i++) {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
    digitalWrite(LED_BUILTIN, LOW);
}

void handleIdent() {
    if (digitalRead(IDENT_BUTTON_PIN)) {
        return;
    }

    Serial.println("Identifying IP address...");

    IPAddress ipAddress = WiFi.localIP();
    uint8_t lastByte = ipAddress.operator[](3);

    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(LIGHT_PIN, LOW);
    delay(1000);

    for (int8_t i = 7; i >= 0; i--) {
        digitalWrite(LED_BUILTIN, HIGH);
        digitalWrite(LIGHT_PIN, HIGH);
        if ((lastByte >> i) & 1) {
            delay(500);
        } else {
            delay(100);
        }
        digitalWrite(LED_BUILTIN, LOW);
        digitalWrite(LIGHT_PIN, LOW);
        delay(500);
    }

    delay(1000);
    digitalWrite(LIGHT_PIN, tallyLightStatus);
}

void setupWiFi() {
    const char *ssid = Config::wifiSsidGet();
    const char *password = Config::wifiPasswordGet();

    WiFi.mode(WIFI_STA);
    WiFi.setHostname(HOST_NAME);
    WiFi.begin(ssid, password);

    // Wait for connection
    Serial.print("Connecting to WiFi: ");
    Serial.print(ssid);
    while (WiFi.status() != WL_CONNECTED) {
        SerialCom::handle();

        delay(1000);
        Serial.print(".");
        digitalWrite(LIGHT_PIN, tallyLightStatus);
        tallyLightStatus = !tallyLightStatus;
    }
    tallyLightStatus = LOW;
    digitalWrite(LIGHT_PIN, tallyLightStatus);

    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("MAC address: ");
    Serial.println(WiFi.macAddress());
}

void setupServer() {
    server.on("/", handleRoot);
    server.on("/on", handleLightOn);
    server.on("/off", handleLightOff);

    server.onNotFound(handleNotFound);

    server.begin();
    Serial.println("HTTP server started");
}

void setupMDNS() {
    if (!MDNS.begin(HOST_NAME)) return;

    Serial.println("MDNS responder started");
    // Add service to MDNS-SD
    MDNS.setInstanceName("My ESP32 server");
    MDNS.addService("_http", "_tcp", 80);
    MDNS.addServiceTxt("_http", "_tcp", "board", "ESP32");
}

#endif

void setup(void) {
    Serial.begin(115200);
    Serial.println("Booting...");

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    pinMode(LIGHT_PIN, OUTPUT);
    digitalWrite(LIGHT_PIN, LOW);

    pinMode(IDENT_BUTTON_PIN, INPUT_PULLUP);
    pinMode(IDENT_BUTTON_GND_PIN, OUTPUT);
    digitalWrite(IDENT_BUTTON_GND_PIN, LOW);

    Config::setup();

    // Give some time for serial to send commands
    delay(1000);
    SerialCom::handle();

    if (!Config::bootIntoConfigGet()) {
        Serial.println("Setting up wifi etc.");
#if !USING_ARDUINO
        setupWiFi();
        setupServer();
        setupMDNS();
#endif
    }

    Serial.println("Ready.");
    digitalWrite(LED_BUILTIN, LOW);
}

void loop(void) {
    SerialCom::handle();

    if (!Config::bootIntoConfigGet()) {
#if !USING_ARDUINO
        server.handleClient();

        handleIdent();
#endif
    }
}
