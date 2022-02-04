#include <IRremote.h>
#define RECEIVERS 4

IRsend sender;
IRrecv *irrecvs[RECEIVERS];
decode_results results;
int IDarray[4] = {};
String deviceID = "0";

void setup() {
  Serial.begin(9600);

  irrecvs[0] = new IRrecv(4); // Up Sensor: pin 4
  irrecvs[1] = new IRrecv(5); // Right Sensor: pin 5
  irrecvs[2] = new IRrecv(6); // Down Sensor: pin 6
  irrecvs[3] = new IRrecv(7); // Left Sensor: pin 7

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
    deviceID = "11";                                                        //Become cube in grid position {1,1}
    Serial.println("I am first cube?");
    sendID(deviceID);

  } else {
    for (int i = 1; i < 13; i++) {                  //Loop through columns
      for (int x = 1; x < 13; x++) {                //Loop through rows
        
        if ((IDs[0] == i) && (IDs[3] == x)) {       //Checks above and to left
          deviceID = String(i+1) + String(x);       //Position = {i+1, x}. Cube above so increment row.
          delay(1000);
          
        } else if ((IDs[0] == 0) && (IDs[3]== x)){
          deviceID = String(x+1) + String(1);       //Position = {x+1, 1}. None above, 1 on left so increment column.
          delay(1000);
          
        } else if ((IDs[3] == 0) && (IDs[0] == i)){
          deviceID = String(i+1) + String(1);       //Position = {i+1, 1}. None left, 1 above so increment row.
          delay(1000);
        }
      }
    }
  }

  if (deviceID != "0") {                         //If no errors and neighbours found
    Serial.println("DEVICE ID: ");
    Serial.print(deviceID);
    sendID(deviceID);                            //Pass device ID
  }
}

void sendID(String ID) {
  int convertID = ID.toInt();
  for (int z = 0; z < 11; z++) {
    Serial.println("SENDING CURRENT DEVICE ID: " + ID);    
    sender.sendNEC("0x" + convertID, 32);              //Broadcast cube ID
    delay(500);
  }
  
  for (int y = 0; y < 4; y++) {
    IDarray[y] = 0;                   //clear ID array
  }
  deviceID = "0";                     //clear device ID
}
