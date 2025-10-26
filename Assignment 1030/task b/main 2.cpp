#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define LED_PIN 25
#define BUZZER_PIN 14
#define BUTTON_PIN 4  // Button connected as per JSON

// Variables for press detection
unsigned long pressStartTime = 0;
bool buttonPressed = false;
bool ledState = false;

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Button active LOW

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 25);
  display.println("System Ready...");
  display.display();
  delay(1000);
}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN);

  // Detect button press start
  if (buttonState == LOW && !buttonPressed) {
    buttonPressed = true;
    pressStartTime = millis();
  }

  // Detect button release
  if (buttonState == HIGH && buttonPressed) {
    buttonPressed = false;
    unsigned long pressDuration = millis() - pressStartTime;

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    if (pressDuration < 1500) {
      // SHORT PRESS → Toggle LED
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
      display.setCursor(15, 25);
      display.println("Short Press Detected");
      display.setCursor(25, 40);
      display.println(ledState ? "LED: ON" : "LED: OFF");
    } else {
      // LONG PRESS → Play buzzer
      display.setCursor(15, 25);
      display.println("Long Press Detected");
      display.setCursor(25, 40);
      display.println("Playing tone...");
      tone(BUZZER_PIN, 1000, 1000); // 1 kHz tone for 1s
    }

    display.display();
  }
}
