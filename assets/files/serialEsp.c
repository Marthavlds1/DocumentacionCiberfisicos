// --------- Pines ----------
#define BTN_PIN 2
#define LED_PIN 3

bool lastBtnState = HIGH;
bool ledState = false;

void setup() {
  Serial.begin(115200);

  pinMode(BTN_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);
}

void loop() {
  bool btnState = digitalRead(BTN_PIN);

  // Detectar flanco (cuando se presiona)
  if (lastBtnState == HIGH && btnState == LOW) {
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);

    // Enviar estado a GUI
    if (ledState)
      Serial.println("LED_ON");
    else
      Serial.println("LED_OFF");

    delay(200); // antirebote simple
  }

  lastBtnState = btnState;

  // --------- Comandos desde GUI ----------
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    if (cmd == "ON") {
      ledState = true;
      digitalWrite(LED_PIN, HIGH);
      Serial.println("LED_ON");
    }
    if (cmd == "OFF") {
      ledState = false;
      digitalWrite(LED_PIN, LOW);
      Serial.println("LED_OFF");
    }
  }
}
