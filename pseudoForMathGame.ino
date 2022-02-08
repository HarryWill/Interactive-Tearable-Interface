void setup() {
  int signs[4] = [{+},{-},{*},{/},{.}];

}

void loop() {
  //SHOULD HAPPEN ON CUBE 1
  //equation [1] [2] [3] [4] [5] [=6]
  //equationPosition = choose either 1, 3 or 5 randomly
  //integer = generate random whole number and add to array at position 
  //repeat 3 times
  //choose random number between 1 and 4
  //add associated sign to equation in randomly chosen position
  //after repeating twice, run equation and create total
  //save total into position 6 with equals

  //Create device ID using equation
  //equation == [10] [-] [5] [/] [2] [=7.5]
  //e.g. 0x92542001
  //first number 9
  //first sign [2] = -
  //second number 5
  //second sign [4] = /
  //third number 2
  //third final bit = 0
  //second final bit = 0
  //final bit = 1 so cube 1

  //Cube 1 becomes total, Pass device ID 6 out to right
  //Second device reads ID, chooses random part of equation and assigns that ID e.g. third number = ID 5
  //ID is then up dated to remove third number and new ID is passed 0x92540005
  //Next device reads ID, chooses one of remaining segments of passed ID and repeats (no 0s allowed)
  //Repeat
  //Game is complete when each cube is lined up in order of IDs (correct sequence)

  //Each device should display ID on screen
  //Increase ID length by 2 to allow for 2 digit numbers?

  //Fraction functionality? Extra sign "_"
  //e.g 1 - 2_3 = 0.33
  //Correct order would involve cubes 3 4 5 being on top of one another
  //Connect to middle cube?

  //To power of? Extra sign "^"
  //Again in verticle stack?

}
