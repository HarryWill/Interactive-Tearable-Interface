#include "MultipleRecievers.h"
#include "IRremote.h"

MultipleRecievers::MultipleRecievers(){
  irrecvs[0] = new IRrecv(2); // Up Sensor: pin 2
  irrecvs[1] = new IRrecv(3); // Right Sensor: pin 3
  irrecvs[2] = new IRrecv(4); // Down Sensor: pin 4
  irrecvs[3] = new IRrecv(5); // Left Sensor: pin 5

  for (int i = 0; i < RECEIVERS; i++)
    irrecvs[i]->enableIRIn();
}

MultipleRecievers:RetrieveIDs(){
  int* IDCheck()
  int IDarray[4] = {};

  for (int i = 0; i < 4; i++) {
    if (irrecvs[i]->decode(&results)) {   
      if (i == 0) {
        Serial.print("Up Sensor: ");      
        Serial.println(results.value, HEX); 
      }
      else if (i == 1) {
        Serial.print("Right Sensor: ");
        Serial.println(results.value, HEX);
      }
      else if (i == 2) {
        Serial.print("Down Sensor: ");
        Serial.println(results.value, HEX);
      }
      else if (i == 3) {
        Serial.print("Left Sensor: ");
        Serial.println(results.value, HEX);
      }

      IDarray[i] = results.value; 
      irrecvs[i]->resume(); 
    }
  }
  return IDarray;
}
