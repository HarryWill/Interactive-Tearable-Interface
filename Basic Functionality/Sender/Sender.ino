#include <IRremote.h>

IRsend sender;

void setup() {
  Serial.begin(9600);
}

void loop() {
  sender.sendNEC(0x1, 32);
  delay(500);
}
