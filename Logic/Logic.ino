#include <IRremote.h>
#define RECEIVERS 4
#define deviceID 00000000

IRsend sender;
IRrecv *irrecvs[RECEIVERS];
decode_results results;

void setup() {
  Serial.begin(9600);

  irrecvs[0] = new IRrecv(2); // Up Sensor: pin 2
  irrecvs[1] = new IRrecv(3); // Right Sensor: pin 3
  irrecvs[2] = new IRrecv(4); // Down Sensor: pin 4
  irrecvs[3] = new IRrecv(5); // Left Sensor: pin 5

  for (int i = 0; i < RECEIVERS; i++)
    irrecvs[i]->enableIRIn(); //Start recievers
}

int* IDCheck() {
  int IDarray[4] = {};

  for (int i = 0; i < 4; i++) {
    if (irrecvs[i]->decode(&results)) {   //If reciever detects result,
      if (i == 0) {
        Serial.print("Up Sensor: ");      //Print sensor direction
        Serial.println(results.value, HEX); //Print result
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

      IDarray[i] = i;       //Add result to ID array
      irrecvs[i]->resume(); //Resume searching for IDs
    }
  }
  return IDarray;
}


void loop() {

  int* IDs = IDCheck();   //Check for IDs

  if ((IDs[0] == 0) && (IDs[3] == 0) && (IDs[1] != 0) && (IDs[2] != 0)) { //If no neighbors above or to left,
    deviceID == 00000001;                                                 //Become cube 1

  } else {

    for (int i = 0; i == 11; i++) {   //Loop through columns
      for (int x = 0; x == 11; x++) { //Loop through rows

        if (IDs[0] == (000000 + (i * 10) + x)) { //Checks up for cube ID
          deviceID == (000000 + (i * 10) + x + 1); //Save new ID (add 1 to row)
        }

        if (IDs[3] == (000000 + (i * 10) + x)) { //Checks left for cube ID
          deviceID == (000000 + (i * 10 + 1) + x); //Save new ID (add 1 to column)
        }
      }
    }
  }

  if (deviceID != 00000000) { //If no errors or neighbours,
    sendID(deviceID);         //Pass device ID
  }
}

void sendID(int ID) {

  while (true) { /*changedID == false*/
    sender.sendNEC("0x" + ID, 32); //Broadcast cube ID
    //changedID variable check
    delay(500);
  }
}
