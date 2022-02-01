#include <IRremote.h>

IRsend sender;

void setup() {
  Serial.begin(9600);
}

void loop() {
  sender.sendNEC(0x11111111, 32);
  delay(500);
}
