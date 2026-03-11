#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>

// ╔═══════════════════════════════════════════════════╗
// ║   CAMBIA ESTOS 3 DATOS ANTES DE FLASHEAR         ║
// ╚═══════════════════════════════════════════════════╝
const char* WIFI_SSID = "Primavera26";
const char* WIFI_PASS = "Ib3r02026pR1m";
const char* STATE_URL = "http://172.22.48.32:5000/api/state";
// ════════════════════════════════════════════════════

#define LED_PIN    5
#define LED_COUNT  8
#define BRIGHTNESS 80     // 0–255

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

unsigned long lastPoll = 0;
const unsigned long POLL_MS = 100;
long lastRev = -1;

// ─── Helpers ──────────────────────────────────────────────────────────────────

uint32_t parseHexColor(const char* s) {
  if (!s || s[0] != '#' || strlen(s) != 7) return strip.Color(0, 0, 0);
  auto h = [](char c) -> uint8_t {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return 10 + (c - 'a');
    if (c >= 'A' && c <= 'F') return 10 + (c - 'A');
    return 0;
  };
  return strip.Color(
    h(s[1]) * 16 + h(s[2]),
    h(s[3]) * 16 + h(s[4]),
    h(s[5]) * 16 + h(s[6])
  );
}

void applyState(uint32_t color, int count) {
  count = constrain(count, 0, LED_COUNT);
  for (int i = 0; i < LED_COUNT; i++)
    strip.setPixelColor(i, (i < count) ? color : 0);
  strip.show();
}

bool fetchAndUpdate() {
  if (WiFi.status() != WL_CONNECTED) return false;

  HTTPClient http;
  http.begin(STATE_URL);
  http.setTimeout(3000);
  int code = http.GET();

  if (code != 200) {
    Serial.printf("[HTTP] Error: %d\n", code);
    http.end();
    return false;
  }

  String body = http.getString();
  http.end();

  StaticJsonDocument<256> doc;
  if (deserializeJson(doc, body)) {
    Serial.println("[JSON] Error de parseo");
    return false;
  }

  const char* colorStr = doc["color"] | "#000000";
  int  count = doc["count"] | 0;
  long rev   = doc["rev"]   | 0;

  if (rev == lastRev) return true;   // sin cambios
  lastRev = rev;

  Serial.printf("[LED] rev=%ld  color=%s  count=%d\n", rev, colorStr, count);
  applyState(parseHexColor(colorStr), count);
  return true;
}

void ensureWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;

  Serial.print("[WiFi] Conectando");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  unsigned long t0 = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - t0 < 10000) {
    delay(300); Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[WiFi] Conectado — IP: " + WiFi.localIP().toString());
    // Flash verde = conectado
    for (int i = 0; i < LED_COUNT; i++) strip.setPixelColor(i, strip.Color(0, 80, 0));
    strip.show(); delay(400);
    for (int i = 0; i < LED_COUNT; i++) strip.setPixelColor(i, 0);
    strip.show();
  } else {
    Serial.println("\n[WiFi] Fallo — revisa SSID/contraseña");
    // Flash rojo = error
    for (int i = 0; i < LED_COUNT; i++) strip.setPixelColor(i, strip.Color(80, 0, 0));
    strip.show(); delay(400);
    for (int i = 0; i < LED_COUNT; i++) strip.setPixelColor(i, 0);
    strip.show();
  }
}

// ─── Setup / Loop ─────────────────────────────────────────────────────────────

void setup() {
  Serial.begin(115200);
  delay(200);
  strip.begin();
  strip.setBrightness(BRIGHTNESS);
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
