#include <Arduino.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <MQTT.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>

#define S(s)      #s
#define STRING(s) S(s)

uint8_t strike_pin = D2;

ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;

auto net    = WiFiClientSecure();
auto client = MQTTClient(256);

static const char IOT_CERT_CA[] PROGMEM      = STRING(CERT_CA);
static const char IOT_CERT_CRT[] PROGMEM     = STRING(CERT_CRT);
static const char IOT_CERT_PRIVATE[] PROGMEM = STRING(CERT_PRIVATE);

const auto iotTrustedCA = BearSSL::X509List(IOT_CERT_CA);
const auto iotCert      = BearSSL::X509List(IOT_CERT_CRT);
const auto iotPrivKey   = BearSSL::PrivateKey(IOT_CERT_PRIVATE);

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

void messageHandler(String& topic, String& payload) {
    // Serial.println("incoming: " + topic + " - " + payload);
    handle_strike();
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

// Set time via NTP, as required for x.509 validation
void set_clock() {
    configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

    Serial.print("Waiting for NTP time sync: ");
    time_t now = time(nullptr);
    while (now < 8 * 3600 * 2) {
        delay(500);
        Serial.print(".");
        now = time(nullptr);
    }
    Serial.println("");
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);
    Serial.print("Current time: ");
    Serial.print(asctime(&timeinfo));
}

void configure_mqtt() {
    set_clock();

    net.setTrustAnchors(&iotTrustedCA);
    net.setClientRSACert(&iotCert, &iotPrivKey);

    client.begin(STRING(ENDPOINT), 8883, net);
    client.onMessage(messageHandler);

    Serial.println("Connecting to IoT server");
    constexpr auto MAXIMUM_RETRIES = 10U;
    for (auto i = 0U; !client.connect(STRING(DEVICE_NAME)) && i < MAXIMUM_RETRIES; ++i) {
        Serial.print(".");
        delay(100);
    }

    if (!client.connected()) {
        Serial.println("IoT Timeout!");
        return;
    }

    client.subscribe(STRING(SUB_TOPIC));
    Serial.println("IoT connected!");
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

    server.on("/restart", HTTP_POST, []() {
        server.send(200);
        ESP.restart();
    });

    server.onNotFound(handle_not_found);

    server.begin();
    Serial.println("HTTP server started");
}

void setup() {
    Serial.begin(115200);

    connect_wifi();

    if (!MDNS.begin(STRING(DEVICE_NAME))) {
        Serial.println("Error setting up MDNS responder!");
    }

    pinMode(strike_pin, OUTPUT);
    stop_strike();
    setup_strike_timeout();

    configure_server();
    configure_mqtt();
}

void loop() {
    client.loop();
    server.handleClient();
    delay(250);
}
