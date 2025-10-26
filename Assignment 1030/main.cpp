#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#define LED1 25      
#define LED2 26      
#define LED3 27      
#define BUZZER 14    
#define BTN_RESET 5  
#define SDA_PIN 21   
#define SCL_PIN 22   
#define BTN_MODE 4    // Mode change button connected to GPIO 4

int mode = 0;
bool lastBtn1 = HIGH;
bool lastBtn2 = HIGH;

void setup() {

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(BTN_MODE, INPUT_PULLUP);
  pinMode(BTN_RESET, INPUT_PULLUP);

  // --- OLED Setup ---
  Wire.begin(SDA_PIN, SCL_PIN);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;) ; // stop if OLED not found
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("ESP32 LED Modes");
  display.display();
  delay(1000);
  showMode();
}

void loop() {
  bool btn1 = digitalRead(BTN_MODE);
  bool btn2 = digitalRead(BTN_RESET);

  // --- Cycle button ---
  if (btn1 == LOW && lastBtn1 == HIGH) {
    mode = (mode + 1) % 4;
    tone(BUZZER, 1000, 100);
    showMode();
    delay(200);
  }

  // --- Reset button ---
  if (btn2 == LOW && lastBtn2 == HIGH) {
    mode = 0;
    tone(BUZZER, 500, 100);
    showMode();
    delay(200);
  }

  lastBtn1 = btn1;
  lastBtn2 = btn2;

  // --- LED behavior ---
  switch (mode) {
    case 0: // Both OFF
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      break;

    case 1: // Alternate Blink
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, LOW);
      delay(300);
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, HIGH);
      delay(300);
      break;

    case 2: // Both ON
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, HIGH);
      break;

    case 3: // PWM Fade
      for (int i = 0; i <= 255; i++) {
        ledcWrite(0, i);
        ledcWrite(1, 255 - i);
        delay(5);
      }
      for (int i = 255; i >= 0; i--) {
        ledcWrite(0, i);
        ledcWrite(1, 255 - i);
        delay(5);
      }
      break;
  }
}

// === OLED display update ===
void showMode() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println("Current Mode:");
  switch (mode) {
    case 0: display.println("Both OFF"); break;
    case 1: display.println("Alternate Blink"); break;
    case 2: display.println("Both ON"); break;
    case 3: display.println("PWM Fade"); break;
  }
  display.display();
}
