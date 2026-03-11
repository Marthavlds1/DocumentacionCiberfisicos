#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>

// ====== CAMBIA ESTOS 3 DATOS ======
const char* WIFI_SSID = "Primavera26";
const char* WIFI_PASS = "Ib3r02026pR1m";
const char* STATE_URL = "http://172.22.49.14:5000/api/state";
// ==================================

#define LED_PIN   5
#define LED_COUNT 8

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

unsigned long lastPoll = 0;
const unsigned long POLL_MS = 500;
long lastRev = -1;

uint32_t parseHexColor(const char* s) {
  if (!s || s[0] != '#' || strlen(s) != 7) return strip.Color(0, 0, 0);

  auto hex2 = [](char c) -> int {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return 10 + (c - 'a');
    if (c >= 'A' && c <= 'F') return 10 + (c - 'A');
    return 0;
  };

  int r = hex2(s[1]) * 16 + hex2(s[2]);
  int g = hex2(s[3]) * 16 + hex2(s[4]);
  int b = hex2(s[5]) * 16 + hex2(s[6]);
  return strip.Color(r, g, b);
}

void applyState(uint32_t color, int count) {
  count = constrain(count, 0, LED_COUNT);
  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, (i < count) ? color : 0);
  }
  strip.show();
}

bool fetchAndUpdate() {
  if (WiFi.status() != WL_CONNECTED) return false;

  HTTPClient http;
  http.begin(STATE_URL);
  int code = http.GET();

  if (code != 200) {
    Serial.printf("HTTP error: %d\n", code);
    http.end();
    return false;
  }

  String body = http.getString();
  http.end();

  Serial.println("Respuesta: " + body);

  StaticJsonDocument<256> doc;
  DeserializationError err = deserializeJson(doc, body);
  if (err) {
    Serial.println("JSON error");
    return false;
  }

  const char* colorStr = doc["color"] | "#000000";
  int count = doc["count"] | 0;
  long rev = doc["rev"] | 0;

  if (rev == lastRev) return true;
  lastRev = rev;

  Serial.printf("Nuevo estado → color: %s | count: %d | rev: %ld\n", colorStr, count, rev);

  uint32_t color = parseHexColor(colorStr);
  applyState(color, count);
  return true;
}

void ensureWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;

  Serial.println("Conectando WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  unsigned long t0 = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - t0 < 10000) {
    delay(300);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi conectado! IP: " + WiFi.localIP().toString());
    // Flash verde: conectado
    for (int i = 0; i < LED_COUNT; i++) strip.setPixelColor(i, strip.Color(0, 80, 0));
    strip.show();
    delay(400);
    for (int i = 0; i < LED_COUNT; i++) strip.setPixelColor(i, 0);
    strip.show();
  } else {
    Serial.println("\nNo se pudo conectar al WiFi");
  }
}

void setup() {
  Serial.begin(115200);
  delay(200);

  strip.begin();
  strip.setBrightness(80); // 0-255, ajusta el brillo
  strip.show();

  ensureWiFi();
}

void loop() {
  ensureWiFi();

  unsigned long now = millis();
  if (now - lastPoll >= POLL_MS) {
    lastPoll = now;
    fetchAndUpdate();
  }
}