#include <IRremote.h>
#define RECEIVERS 4

//Supporting screens
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 4
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Initialising IR sender and reciever objects
IRsend sender;
IRrecv *irrecvs[RECEIVERS];
decode_results results;

String deviceID = "0";
int IDarray[4] = {};
int partInEquation = 0;

int availableSigns[5] = {"+", "-", "*", "/", "."};
int numbers[3] = {};
int signs[2] = {};
int total = 0;
int solution = 0;

boolean assignedID = false;
boolean assignedMath = false;
boolean correctRight = false;
boolean correctLeft = false;

////CODE LIST////

//12345678 = NO ID
//11-> 16||61||32 = DEVICE ID
//000000## = ASSIGNED ID
//1000 - 6000 = EQUATION PART
//11111111 = CORRECT NEIGHBOURS


void setup() {
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);

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

//Checks IR recievers for codes
void IDCheck() {
  if (deviceID == "0") {                    //If no ID, broadcast No-ID code
    sender.sendNEC(0x12345678, 32);
    Serial.println("Sent No-ID code");
  }

  for (int i = 0; i < 4; i++) {
    if (irrecvs[i]->decode(&results)) {

      if (assignedID == false && assignedMath == false) {        //If device has no ID or equation, attempt to assign ID
        if (i == 0) {
          Serial.print("Up Sensor: ");        //Print sensor direction
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
        IDarray[i] = results.value; //Add ID to array
        assignIDs(IDarray);
        irrecvs[i]->enableIRIn();   //Continue checking for IDs


      } else if (assignedID == true && assignedMath == true && correctRight == false) {    //If device has ID and equation, broadcast part in equation

        if (partInEquation == 6000) {
          //delay(5000);                                                    /////////////Delay here to give user time to find solution?////////////
          for (int i = 0; i < 20; i++) {
            sender.sendNEC("0x" + partInEquation, 32);
            delay(300);
          }
        } else {
          String strResult = String(results.value);                        //If not last cube, check for correct neighbour to right
          if (i == 1 && strResult.toInt() == (partInEquation + 1000)) {
            correctRight = true;
          } else if (i == 1 && strResult.toInt() != (partInEquation + 1000)) {
            correctRight = false;
          }
        }

      } else if (assignedID == true && assignedMath == false) {           //If device has ID but no equation, check left for equation
        if (i == 3) {
          Serial.print("Equation: ");
          Serial.println(results.value, HEX);
          equationLogic(String(results.value));
        }
        
      } else if (correctRight == true) {           //congrats, correct neighbour to right!
        int correctCode = 0;
        displayText("Correct to right!");          //////Continue to display the "section" aswell//////

        //Currently (maybe): cube can decide if right neighbour is correct
        //Next step: Go left to right, broadcasting correct neighbour ID
        //Each cube adds one to ID therefore if total recieves correct ID + 5, each cube is in order

        if (partInEquation == 1000){
          correctCode = 01010001;
          for (int i = 0; i < 20; i++) {
            sender.sendNEC("0x" + correctCode, 32);
            }
          } else if (i == 3){
            if (results.value == 01010001){
              correctCode = (01010001 + 1);
            } else if (results.value == 01010002){
              /////////////////////////////REEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
            }
        }
      }
    }
  }
}

//Method which uses incoming codes to decide on device ID
void assignIDs(int IDs[4]) {
  Serial.println("Trying to Assign ID");

  if ((IDs[0] == 0) && (IDs[3] == 0) && (IDs[1] == 12345678) && (IDs[2] == 12345678)) {   //If no neighbors above or to left and 2 unassigned neighbours attached
    deviceID = "11";                                                        //Become cube in grid position {1,1}
    displayText("Cube 1");

  } else {
    for (int i = 1; i < 13; i++) {                  //Loop through columns
      for (int x = 1; x < 13; x++) {                //Loop through rows

        if ((IDs[0] == i) && (IDs[3] == x)) {       //Checks above and to left
          deviceID = String(i + 1) + String(x);     //Position = {i+1, x}. Cube above so increment row.
          displayText("Cube " + deviceID);

        } else if ((IDs[0] == 0) && (IDs[3] == x)) {
          deviceID = String(x + 1) + String(1);     //Position = {x+1, 1}. None above, 1 on left so increment column.
          displayText("Cube " + deviceID);

        } else if ((IDs[3] == 0) && (IDs[0] == i)) {
          deviceID = String(i + 1) + String(1);     //Position = {i+1, 1}. None left, 1 above so increment row.
          displayText("Cube " + deviceID);
        }
      }
    }
  }

  //If cube has ID and others around are broadcasting that they have IDs, do the same
  int intID = deviceID.toInt();
  if (IDs[1] == (11111100 + intID - 10)) {
    if (intID != 0 && intID != 11) {
      int assignedIDCode = 11111100 + intID;
      displayText("Cube " + assignedIDCode);

      for (int z = 0; z < 20; z++) {
        sender.sendNEC("0x" + assignedIDCode, 32);    //Telling neighbours that cube has ID assigned
        delay(300);
      }
      assignedID = true;

      //Repeats loop now listening for equation

    } else if (intID == 11 && IDs[1] == (11111100 + intID + 10)) { //If first cube
      //Every cube should have a (hopefully) correctly assigned ID!
      String equation = generateEquation(); //First cube creates math problem
      String total = equation.substring(6); //Find total in string
      partInEquation = 6000;
      displayText("= " + total);
      sendMath(equation);

      //Begin listening for correct neighbour
    }
  } else if (intID != 0) {   //If no neighbours are sending assignedIDCode

    Serial.println("DEVICE ID: ");
    Serial.println(deviceID);
    sendID(deviceID);  //Pass device ID
  }
}

//Generates math equation
String generateEquation() {
  Serial.println("Math time");
  total = random(20, 101);
  Serial.println(total);

  while (solution = ! total) {    //Test random numbers until solution is found which equals total
    numbers[0] = random(11);
    numbers[1] = random(11);
    numbers[2] = random(11);
    signs[0] = availableSigns[random(5)];
    signs[1] = availableSigns[random(5)];
    solution = numbers[0] + signs[0] + numbers[1] + signs[1] + numbers[2];
  }

  String strNum0 = String(numbers[0]);
  String strNum1 = String(numbers[0]);
  String strNum2 = String(numbers[0]);

  if (numbers[0] < 10) {
    strNum0 = "0" + String(numbers[0]);    //Adding 0 infront of number to make equation 8 characters
  }
  if (numbers[1] < 10) {
    strNum1 = "0" + String(numbers[1]);
  }
  if (numbers[2] < 10) {
    strNum1 = "0" + String(numbers[2]);
  }
  String equationString = strNum0 + String(signs[0]) + strNum1 + String(signs[1]) + strNum2 + String(total);
  Serial.println("Equation: " + equationString);
  total = 0;
  solution = 0;
  return equationString;
}

void equationLogic(String equation) {
  String section;

  if (deviceID = ! "11") {
    int randomSection = 0;
    boolean done = false;

    while (done == false) {
      randomSection = random(1, 6);
      if (randomSection == 1) {
        section = equation.substring(0) + equation.substring(1);
        if (section = ! "00") {
          done = true;
          equation.replace(section, "00");
          partInEquation = 1000;
        }

      } else if (randomSection == 2) {
        section = equation.substring(2);
        if (section = ! "0") {
          done = true;
          equation.replace(section, "0");
          partInEquation = 2000;
        }

      } else if (randomSection == 3) {
        section = equation.substring(3) + equation.substring(4);
        if (section = ! "00") {
          done = true;
          equation.replace(section, "00");
          partInEquation = 3000;
        }

      } else if (randomSection == 4) {
        section = equation.substring(4);
        if (section = ! "0") {
          done = true;
          equation.replace(section, "0");
          partInEquation = 4000;
        }

      } else if (randomSection == 5) {
        section = equation.substring(5) + equation.substring(6);
        if (section = ! "00") {
          done = true;
          equation.replace(section, "00");
          partInEquation = 5000;
        }
      }
    }
    assignedMath = true;
    displayText(section);

    if (deviceID != "32" && deviceID != "16" && deviceID == "61") { //if not last cube
      sendMath(section);
    }
  }
  //Begin listening for correct neighbours!
}

//Broadcast cube ID
void sendID(String ID) {
  int intID = ID.toInt();

  if (intID == 32 || intID == 16 || intID == 61) {  //If final cube
    int assignedIDCode = 11111100 + intID;
    for (int z = 0; z < 10; z++) {
      sender.sendNEC("0x" + assignedIDCode, 32); //Tell neighbours that final cube has ID
      delay(300);
    }
  } else {  //Else broadcast ID
    for (int z = 0; z < 5; z++) {
      Serial.println("SENDING DEVICE ID: " + ID);
      sender.sendNEC("0x" + intID, 32);
      delay(300);
    }
  }
}

//Broadcast equation
void sendMath(String equation) {
  String trimmedEquation = equation.substring(0) + equation.substring(1) + equation.substring(2) + equation.substring(3) + equation.substring(4) + equation.substring(5);
  int intEquation = trimmedEquation.toInt();

  for (int z = 0; z < 20; z++) {
    Serial.println("SENDING EQUATION: " + trimmedEquation);
    sender.sendNEC("0x" + intEquation, 32);
    delay(300);
  }
}

//Displays passed string on screen
void displayText(String deviceSection) {
  display.clearDisplay();
  display.setTextSize(2); //
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(deviceSection);
  display.display();
}
