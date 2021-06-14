#include <RTClib.h>
#include <Adafruit_Keypad.h>
#include "BlindsServoSM.h"
#include <arduino-timer.h>
#include <Wire.h>

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
  CBlindsServoSM servoSM(iServoPin);

  // RTC
  // DS1302 rtc(A3, A5, A4); // CE, SCK, SDA
  // static unsigned int uiMillisSinceLastAction; 

  // Alarm
  const int alarmPin = A2;
}

//////////////////////////////////
// Setup
//////////////////////////////////
void setup() {
  using namespace main;
  Wire.begin();
  customKeypad.begin();
  if (DEBUG) {Serial.begin(9600);}
  servoSM.Setup();

  // Initialize RTC and set date/time to compile time
  //rtc.begin();
  //if (!rtc.isrunning())
  //{
  //  rtc.adjust(DateTime(2014,1,1,3,0,0));
  //}

  // Alarm
  pinMode(alarmPin, INPUT);
}

//////////////////////////////////
// Loop
//////////////////////////////////
void loop() {
  using namespace main;

  // Upkeep
  delay(iDelay);
  servoSM.Update();
  customKeypad.tick();

  // Check RTC
  // DateTime now = rtc.now();

  if(DEBUG)
  {
    Serial.println("State:");
    Serial.println(servoSM.GetState());
    Serial.println("Position:");
    Serial.println(servoSM.GetPosition());
    //Serial.print("RTC time:");
    //Serial.print(now.hour(), DEC);
    //Serial.print(":");
    //Serial.println(now.minute(), DEC);
    //Serial.println("RTC running?");
    //Serial.println(rtc.isrunning());
    Serial.println("Alarm analog read:");
    Serial.println(analogRead(alarmPin));
  }
  
  // Only check if 100 seconds have passed since the last action
  //if(uiMillisSinceLastAction >= 100000)
  //{
  //  if(now.hour() == 6 && now.minute() == 30)
  //  {
  //    servoSM.SetState(CBlindsServoSM::AUTO_UP_WINDUP);
  //  }
  //  if(now.hour() == 20 && now.minute() == 0)
  //  {
  //    servoSM.SetState(CBlindsServoSM::AUTO_DOWN);
  //  }
  //}
  //else
  //{
  //  uiMillisSinceLastAction += iDelay;
  //}

  // Check alarm pin
  if(analogRead(A2) >= 50)
  {
    servoSM.SetState(CBlindsServoSM::AUTO_UP_WINDUP);
  }

  // Handle Keypad inputs
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
          if (DEBUG) {Serial.println("Auto up windup");}
          servoSM.SetState(CBlindsServoSM::AUTO_UP_WINDUP);
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
