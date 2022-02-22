#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <IRremote.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 4 
#define SCREEN_ADDRESS 0x3C 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
    Serial.begin(9600);
    display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
}

void loop() {
  text();
}

void text(void) {
  display.clearDisplay();
  display.setTextSize(2); //
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println(F("Test"));
  display.println(F("Test"));
  display.display();
  delay(1000);
}
