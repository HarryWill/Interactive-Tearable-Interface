
#define deviceID = "0x00000000"

void setup() {
  int IDs = array[4];
}

void loop() {

  
  IDs = IDCheck();

  if ((upsensor == null) && (leftsensor == null) && (no cube is already 1) {
  deviceID == "0x00000001";
  sendNEC(0x00000001.toInt);
  }
  
  int column = 0;
  int row = 0;
  
  for (int i = 0; i == 35; i++) {
    
  String columnStr = (column + 1).toString;
  String rowStr = (row + 1).toString;

    if (upsensor.decode == ("0x000000+column+row").toInt {
    deviceID == "0x000000+column+rowStr";
    sendID((deviceID).toInt);
    }

    if (leftsensor.decode == ("0x000000+column+row").toInt {
    deviceID == "0x000000+columnStr+row";
    sendID((deviceID).toInt);
    }

    column++;
    loopcount++;
  }
}

void sendID(deviceID) {
  while (changeCheckVariable == false) {
    sendNEC(deviceID);
    delay(500);
    changeCheckVariable();
  }
}

void IDCheck() {
  int foundIDs = array[4];
  //some check to see if new ID is detected
  //probably loop through the fetch ID loop from recieverMultipleSensors
  return foundIDs;
}

}
