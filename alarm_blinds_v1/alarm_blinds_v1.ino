#include <RTClib.h>
#include <Adafruit_Keypad.h>
#include "BlindsServoSM.h"

//////////////////////////////////
// Declarations
//////////////////////////////////

namespace main{

  // CONSTANTS
  int iDelay = 10; // Delay period in ms
  int iServoPin = 11;
  const bool DEBUG = true;

    // KEYPAD
  const int ROWS = 4; // rows
  const int COLS = 3; // columns
  //define the symbols on the buttons of the keypads
  byte keys[ROWS][COLS] = {
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'},
    {'*','0','#'}
  };
  byte rowPins[ROWS] = {3, 8, 7, 5}; //connect to the row pinouts of the keypad
  byte colPins[COLS] = {4, 2, 6}; //connect to the column pinouts of the keypad

  //initialize an instance of class NewKeypad
  Adafruit_Keypad customKeypad = Adafruit_Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

  // SERVO
  CBlindsServoSM servoSM(iServoPin, iDelay);

  // RTC
  DS1302 rtc(A3, A5, A4); // CE, SCK, SDA
}

//////////////////////////////////
// Setup
//////////////////////////////////
void setup() {
  using namespace main;
  customKeypad.begin();
  if (DEBUG) {Serial.begin(9600);}
  servoSM.Setup();

  // Initialize RTC and set date/time to compile time
  rtc.begin();
  rtc.adjust(DateTime(__DATE__, __TIME__));
}

//////////////////////////////////
// Loop
//////////////////////////////////
void loop() {
  using namespace main;
  delay(iDelay);
  if(DEBUG)
  {
    Serial.println(servoSM.GetState());
  }
  servoSM.Update();
  customKeypad.tick();

  while(customKeypad.available()){
    keypadEvent e = customKeypad.read();
    if(e.bit.EVENT == KEY_JUST_RELEASED) {
      switch ((char)e.bit.KEY){
        // Move blinds to "down" position
        case '*':
          if (DEBUG) {Serial.println("Down");}
          servoSM.SetState(CBlindsServoSM::MANUAL_DOWN);
          break;
        // Stop blinds
        case '0':
          if (DEBUG) {Serial.println("Still");}
          servoSM.SetState(CBlindsServoSM::NOT_MOVING);
          break;
        // Move blinds to "up" position
        case '#':
          if (DEBUG) {Serial.println("Up");}
          servoSM.SetState(CBlindsServoSM::MANUAL_UP);
          break;
        // Set down (reference position)
        case '1':
          if (DEBUG) {Serial.println("Set Down Position");}
          servoSM.SetDownPosition();
          break;
        // Set up (reference position)
        case '2':
          if (DEBUG) {Serial.println("Set Up Position");}
          servoSM.SetUpPosition();
          break;
        // Auto up
        case '3':
          if (DEBUG) {Serial.println("Auto up");}
          servoSM.SetState(CBlindsServoSM::AUTO_UP);
          break;
        // Auto down
        case '6':
          if (DEBUG) {Serial.println("Auto down");}
          servoSM.SetState(CBlindsServoSM::AUTO_DOWN);
          break;
        default:
          if (DEBUG) {Serial.println("Unrecognized");}
          break;
      }
    }
  }
}
