void setup() {
  Serial.begin(115200);                       // USB Serial (Arduino IDE)
  Serial2.begin(115200, SERIAL_8N1, 16, 17); // UART2 RX=16, TX=17
  Serial.println("UART passthrough ready");
}

void loop() {
  // 1) Von USB-Serial empfangen und an Serial2 senden
  if (Serial.available()) {
    char c = Serial.read();
    Serial2.write(c);
  }

  // 2) Von Serial2 empfangen und an USB-Serial senden
  if (Serial2.available()) {
    char c = Serial2.read();
    Serial.write(c);
  }
}
