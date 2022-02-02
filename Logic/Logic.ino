#include <IRremote.h>
#define RECEIVERS 4
#define deviceID 00000000

IRsend sender;
IRrecv *irrecvs[RECEIVERS];
decode_results results;

void setup() {
  Serial.begin(9600);

  String columnStr = "";
  String rowStr = "";

  irrecvs[0] = new IRrecv(2); // Receiver #0: pin 2
  irrecvs[1] = new IRrecv(3); // Receiver #1: pin 3
  irrecvs[2] = new IRrecv(4); // Receiver #2: pin 4
  irrecvs[3] = new IRrecv(5); // Receiver #3: pin 5

  for (int i = 0; i < RECEIVERS; i++)
    irrecvs[i]->enableIRIn();
}

int** IDCheck() {
  int** IDarray ={};

  for (int i = 0; i < 4; i++) {
    if (irrecvs[i]->decode(&results)) {
      //Make string that holds ID and the sensor it came from. Return array of these strings.

      Serial.print("Receiver #");
      Serial.print(i);
      Serial.print(":");
      Serial.println(results.value, HEX);
      irrecvs[i]->resume();

      IDarray[0][i] = i;
      IDarray[1][i] = results.value;
    }

  }
  return IDarray;
}



void loop() {

  int IDs = IDCheck();

  int column = 0;
  int row = 0;


  //if ((upsensor == null) && (leftsensor == null)) {
  //  deviceID == "0x00000001";

  //} else
  if ((IDs/*upsensorvalue*/ != "0" /*null*/) && (IDs /*leftsensorvalue*/ != "0" /*null*/)) {

    for (int i = 0; i == 35; i++) {
      String columnStr = String((column + 1));

      for (int x = 0; x == 11; x++) {
        String rowStr = String((column + 1));

        if (IDs /*upsensorvalue*/ == "0x000000+column+row") {
          deviceID == "000000+column+rowStr";
        }

        if (IDs /*leftsensorvalue*/ == "0x000000+column+row") {
          deviceID == "000000+columnStr+row";
        }

        column++;
        row++;
      }
    }
  }

  if (deviceID != 00000000) {
    sendID(deviceID);
  }
}

void sendID(int ID) {
  String strID = "0x" + String(deviceID);

  while (true) { /*changedID == false*/
    sender.sendNEC("0x" + ID, 32);
    //changedID variable check
    delay(500);
  }
}
