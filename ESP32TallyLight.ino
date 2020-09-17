#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#define HOST_NAME "esp32_1"
#define LED_BUILTIN 2
#define LIGHT_PIN 22
#define IDENT_BUTTON_PIN 23

const char *ssid = "trAP";
const char *password = "H8ppeldep8p";

WebServer server(80);

void handleRoot() {
    digitalWrite(LED_BUILTIN, HIGH);
    server.send(200, "text/html", "<a href='/on'>On</a><br/><a href='/off'>Off</a>");
    digitalWrite(LED_BUILTIN, LOW);
}

void handleLightOn() {
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(LIGHT_PIN, HIGH);
    server.send(200, "text/plain", "on");
    digitalWrite(LED_BUILTIN, LOW);
}

void handleLightOff() {
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(LIGHT_PIN, LOW);
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
}

void setupWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.setHostname(HOST_NAME);
    WiFi.begin(ssid, password);

    // Wait for connection
    Serial.println("");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

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

void setup(void) {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.begin(115200);
    Serial.println("Booting...");

    setupWiFi();
    setupServer();
    setupMDNS();

    pinMode(LIGHT_PIN, OUTPUT);
    digitalWrite(LIGHT_PIN, LOW);
    pinMode(IDENT_BUTTON_PIN, INPUT_PULLUP);

    Serial.println("Ready.");
    digitalWrite(LED_BUILTIN, LOW);
}

void loop(void) {
    server.handleClient();

    handleIdent();
}