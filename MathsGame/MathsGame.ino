#include <IRremote.h>
#define RECEIVERS 4

IRsend sender;
IRrecv *irrecvs[RECEIVERS];
decode_results results;
int IDarray[4] = {};
String deviceID = "0";

int availableSigns[5] = {"+", "-", "*", "/", "."};
int numbers[3] = {};
int signs[2] = {};
int equation[6] = {};
int total = 0;
int solution = 0;

boolean assigned = false;

String deviceEquation;

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
    Serial.println("I am cube 1!");

  } else {
    for (int i = 1; i < 13; i++) {                  //Loop through columns
      for (int x = 1; x < 13; x++) {                //Loop through rows

        if ((IDs[0] == i) && (IDs[3] == x)) {       //Checks above and to left
          deviceID = String(i + 1) + String(x);     //Position = {i+1, x}. Cube above so increment row.
          delay(1000);

        } else if ((IDs[0] == 0) && (IDs[3] == x)) {
          deviceID = String(x + 1) + String(1);     //Position = {x+1, 1}. None above, 1 on left so increment column.
          delay(1000);

        } else if ((IDs[3] == 0) && (IDs[0] == i)) {
          deviceID = String(i + 1) + String(1);     //Position = {i+1, 1}. None left, 1 above so increment row.
          delay(1000);
        }
      }
    }
  }

  //If cube has ID and others around are broadcasting that they have IDs, do the same
  int convertedID = deviceID.toInt();
  if ((IDs[0] == (11111100 + convertedID - 1)) || (IDs[1] == (11111100 + convertedID - 10)) || (IDs[2] == (11111100 + convertedID + 1)) || (IDs[3] == (11111100 + convertedID - 10))) {
    if (convertedID != 00 && convertedID != 11) {
      int tempID = 11111100 + convertedID;

      for (int z = 0; z < 10; z++) {
        sender.sendNEC("0x" + tempID, 32);
        delay(300);
      }

    } else if (convertedID == 11) {
      if (IDs[1] == (11111100 + convertedID - 10) || IDs[2] == (11111100 + convertedID + 1)) {
        //Every cube should have a (hopefully) correctly assigned ID!
        //Total on screen
        generateEquation();
      }
      //First cube creates math problem
      //Now need to send equation to other cubes..
    }
  }

  if (deviceID != "0") {   //If no errors and neighbours found
    assigned = true;       //Device has ID
    Serial.println("DEVICE ID: ");
    Serial.println(deviceID);
    sendID(deviceID);  //Pass device ID
  }
}


//Generate math equation. Cube 1's job.
void generateEquation() {
  total = random(20, 101);

  while (solution = ! total) {
    numbers[0] = random(11);
    numbers[1] = random(11);
    numbers[2] = random(11);
    signs[0] = availableSigns[random(5)];
    signs[1] = availableSigns[random(5)];

    solution = numbers[0] + signs[0] + numbers[1] + signs[1] + numbers[2];
  }

  equation[0] = numbers[0];
  equation[1] = signs[0];
  equation[2] = numbers[1];
  equation[3] = signs[1];
  equation[4] = numbers[2];
  equation[5] = total;
  
}

void sendID(String ID) {
  int convertID = ID.toInt();
  for (int z = 0; z < 5; z++) {
    Serial.println("SENDING DEVICE ID: " + ID);
    sender.sendNEC("0x" + convertID, 32);              //Broadcast cube ID
    delay(300);
  }

  if (convertID == 32 || convertID == 16 || convertID == 61) {
    int assignedCode = 11111100 + convertID;
    for (int z = 0; z < 10; z++) {
      sender.sendNEC("0x" + assignedCode, 32);    //Final cube has ID
      delay(300);
    }
  }
}

//for (int y = 0; y < 4; y++) {
//  IDarray[y] = 0;                   //clear ID array
//}
//deviceID = "0";                     //clear device ID
