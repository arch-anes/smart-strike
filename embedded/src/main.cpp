#include <Arduino.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <MQTT.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <limits>

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

String client_id;

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
    Serial.println("incoming: " + topic + " - " + payload);
    handle_strike();
}

void check_wifi() {
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("WiFi Connect Failed! Rebooting...");
        delay(1000);
        ESP.restart();
    }
}

void setup_wifi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(STRING(WIFI_SSID), STRING(WIFI_PASS));

    check_wifi();

    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(STRING(WIFI_SSID));
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

// Set time via NTP, as required for x.509 validation
void setup_clock() {
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

void setup_mdns() {
    if (!MDNS.begin(STRING(DEVICE_NAME))) {
        Serial.println("Error setting up MDNS responder!");
    }
}

void setup_mqtt_client() {
    net.setTrustAnchors(&iotTrustedCA);
    net.setClientRSACert(&iotCert, &iotPrivKey);

    client.begin(STRING(ENDPOINT), 8883, net);
    client.onMessage(messageHandler);

    Serial.print("Connecting to IoT server with device id: ");
    Serial.println(client_id);
    constexpr auto MAXIMUM_RETRIES = 10U;
    for (auto i = 0U; !client.connect(client_id.c_str()) && i < MAXIMUM_RETRIES; ++i) {
        Serial.print(".");
        delay(100);
    }

    if (!client.connected()) {
        Serial.println("IoT Timeout!");
        return;
    }

    client.subscribe(STRING(SUB_TOPIC), 1);
    Serial.println("IoT connected!");
}

void setup_http_server() {
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

void generate_client_id() {
    const auto random_id = secureRandom(std::numeric_limits<long>::max());
    client_id            = STRING(DEVICE_NAME) + String("-") + random_id;
}

void setup() {
    Serial.begin(115200);

    setup_wifi();

    setup_mdns();

    setup_clock();

    pinMode(strike_pin, OUTPUT);
    stop_strike();
    setup_strike_timeout();

    setup_http_server();

    generate_client_id();
    setup_mqtt_client();
}

void loop() {
    check_wifi();

    if (!client.loop()) {
        setup_mqtt_client();
    }
    delay(10);

    server.handleClient();
    delay(10);
}
