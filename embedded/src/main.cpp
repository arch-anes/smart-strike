#include <Arduino.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>

#define S(s)      #s
#define STRING(s) S(s)

uint8_t strike_pin = D2;

ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;

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

void connect_wifi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(STRING(WIFI_SSID), STRING(WIFI_PASS));
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("WiFi Connect Failed! Rebooting...");
        delay(1000);
        ESP.restart();
    }

    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(STRING(WIFI_SSID));
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void configure_server() {
    httpUpdater.setup(&server, "/update", STRING(OTA_USER), STRING(OTA_PASS));

    server.on("/", HTTP_POST, []() {
        if (!server.authenticate(STRING(DEVICE_USER), STRING(DEVICE_PASS))) {
            return server.requestAuthentication();
        }

        handle_strike();
        server.send(200);
    });

    server.onNotFound(handle_not_found);

    server.begin();
    Serial.println("HTTP server started");
}

void setup() {
    Serial.begin(115200);

    connect_wifi();

    MDNS.begin(STRING(DEVICE_NAME));

    pinMode(strike_pin, OUTPUT);
    stop_strike();
    setup_strike_timeout();

    configure_server();
}

void loop() {
    server.handleClient();
}
