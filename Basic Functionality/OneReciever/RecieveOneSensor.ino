#include <IRremote.h>

int recieverpin = 2;

IRrecv irrecv(recieverpin);
decode_results results;

void setup() {
  pinMode(recieverpin, INPUT);
  Serial.begin(9600);
  irrecv.enableIRIn();
  Serial.println("started i think");
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    irrecv.resume();
    delay(400);
  }
}
