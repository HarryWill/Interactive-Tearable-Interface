#include <IRremote.h>
#define RECEIVERS 4

IRsend sender;
IRrecv *irrecvs[RECEIVERS];
decode_results results;
int IDarray[4] = {};
String deviceID = "0";

void setup() {
  Serial.begin(9600);

  irrecvs[0] = new IRrecv(2); // Up Sensor: pin 2
  irrecvs[3] = new IRrecv(3); // Right Sensor: pin 3
  irrecvs[2] = new IRrecv(4); // Down Sensor: pin 4
  irrecvs[1] = new IRrecv(5); // Left Sensor: pin 5

  for (int i = 0; i < RECEIVERS; i++) {
    irrecvs[i]->enableIRIn(); //Start recievers
  }
}

void loop() {
  IDCheck();
}

void IDCheck() {
  for (int i = 0; i < 4; i++) {
    if (irrecvs[i]->decode(&results)) {

      if (i == 0) {
        Serial.print("Up Sensor: ");        //Print sensor direction
        Serial.println(results.value, HEX); //Print result
        delay(300);
      }
      else if (i == 1) {
        Serial.print("Right Sensor: ");
        Serial.println(results.value, HEX);
        delay(300);
      }
      else if (i == 2) {
        Serial.print("Down Sensor: ");
        Serial.println(results.value, HEX);
        delay(300);
      }
      else if (i == 3) {
        Serial.print("Left Sensor: ");
        Serial.println(results.value, HEX);
        delay(300);
      }
      IDarray[i] = results.value; //Add ID to array
      assignIDs(IDarray);
      irrecvs[i]->enableIRIn();
    }
  }
}

void assignIDs(int IDs[4]) {
  Serial.println("ASSIGN IDS RUNNING");

  if ((IDs[0] == 0) && (IDs[3] == 0) && (IDs[1] != 0) && (IDs[2] != 0)) {   //If no neighbors above or to left,
    deviceID = "1";                                                         //Become cube 1
    Serial.println("I am first cube?");
    sendID(deviceID);

  } else {
    for (int i = 1; i < 13; i++) {              //Loop through columns
      for (int x = 1; x < 13; x++) {            //Loop through rows
        
        if ((IDs[0] == i) && (IDs[3] == x)) {   //Checks for cube ID
          deviceID = String(i+1) + String(x+1); 
          delay(1000);
        }
      }
    }
  }

  if (deviceID != "0") {                         //If no errors or neighbours,
    Serial.println("My ID is:");
    Serial.println(deviceID);
    sendID(deviceID);                            //Pass device ID
  }
}

void sendID(String ID) {
  for (int z = 0; z < 11; z++) { /*changedID == false*/
    //sender.sendNEC("0x" + ID, 32);              //Broadcast cube ID
    Serial.println("SENDING CURRENT DEVICE ID: " + ID);
    //changedID variable check
    delay(500);
  }
  
  for (int y = 0; y < 4; y++) {
    IDarray[y] = 0;
  }
  deviceID = "0";
}
