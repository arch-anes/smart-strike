#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>

uint8_t strike_pin = D3;

ESP8266WebServer server(80);

void handle_not_found() {
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
}

void start_strike() {
    digitalWrite(strike_pin, HIGH);
}

void stop_strike() {
    digitalWrite(strike_pin, LOW);
}

void ICACHE_RAM_ATTR handle_strike_timeout() {
    stop_strike();
}

void clear_strike_timeout() {
    constexpr uint8_t strike_timeout_s      = 5;
    constexpr uint32_t strike_timeout_ticks = strike_timeout_s * 10000000 / 32;

    noInterrupts();
    timer1_write(strike_timeout_ticks);
    interrupts();
}

void setup_strike_timeout() {
    timer1_attachInterrupt(stop_strike);
    timer1_enable(TIM_DIV256, TIM_EDGE, TIM_SINGLE);
    clear_strike_timeout();
}

void handle_strike() {
    clear_strike_timeout();
    start_strike();
}

void configure_server() {
    server.on("/", []() { server.send(200, "text/plain", "hello world"); });

    server.on("/strike", []() {
        server.send(200, "text/plain", "struck");
        handle_strike();
    });

    server.onNotFound(handle_not_found);
}

void setup() {
    pinMode(strike_pin, OUTPUT);

    stop_strike();

    setup_strike_timeout();

    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.println("");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(WIFI_SSID);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    if (MDNS.begin("esp8266")) {
        Serial.println("MDNS responder started");
    }

    configure_server();

    server.begin();
    Serial.println("HTTP server started");
}

void loop() {
    server.handleClient();
    MDNS.update();
}
