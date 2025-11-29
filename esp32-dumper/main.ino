#include <SPI.h>

// Flash SPI Pins
#define CS_PIN    5
#define MOSI_PIN 23
#define MISO_PIN 19
#define CLK_PIN  18

// Flash settings
#define FLASH_SIZE (16 * 1024 * 1024) // 16MB

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("Starting flash dump...");

  // Set SPI pins
  SPI.begin(CLK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);

  // Wake up flash (send 0xAB)
  digitalWrite(CS_PIN, LOW);
  SPI.transfer(0xAB);
  digitalWrite(CS_PIN, HIGH);
  delay(10);

  // Dump
  for (uint32_t addr = 0; addr < FLASH_SIZE; addr += 256) {
    Serial.print("Reading 0x");
    Serial.println(addr, HEX);

    uint8_t cmd[4] = {0x03, (uint8_t)(addr >> 16), (uint8_t)(addr >> 8), (uint8_t)(addr)};
    digitalWrite(CS_PIN, LOW);
    SPI.transfer(cmd, 4);

    for (int i = 0; i < 256; i++) {
      uint8_t val = SPI.transfer(0x00);
      Serial.write(val);  // Raw binary output
    }

    digitalWrite(CS_PIN, HIGH);
    delay(5); // Small delay between reads
  }

  Serial.println("Dump complete.");
}

void loop() {
  // Do nothing
}