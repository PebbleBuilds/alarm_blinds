#include <Adafruit_Keypad.h>
#include <Servo.h>


// ___________KEYPAD____________
const byte ROWS = 4; // rows
const byte COLS = 3; // columns
//define the symbols on the buttons of the keypads
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {3, 8, 7, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 2, 6}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Adafruit_Keypad customKeypad = Adafruit_Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// __________SERVO______________
Servo serv;
const int servoPin = 11;

#define DOWN 0
#define UP 180
#define STILL 90

// __________DEBUG__________
#define DEBUG true

void setup() {
// ___________KEYPAD____________
  customKeypad.begin();

// __________SERVO______________
  serv.attach(servoPin);
  serv.write(STILL);

// __________DEBUG__________
  if (DEBUG) {Serial.begin(9600);}
}

void loop() {
  // put your main code here, to run repeatedly:
  customKeypad.tick();

  while(customKeypad.available()){
    keypadEvent e = customKeypad.read();
    if(e.bit.EVENT == KEY_JUST_RELEASED) {
      switch ((char)e.bit.KEY){
        case '*':
          serv.write(DOWN);
          if (DEBUG) {Serial.println("Down");}
          break;
        case '0':
          serv.write(STILL);
          if (DEBUG) {Serial.println("Still");}
          break;
        case '#':
          serv.write(UP);
          if (DEBUG) {Serial.println("Up");}
          break;
        default:
          if (DEBUG) {Serial.println("Unrecognized");}
          break;
      }
    }
  }

  delay(10);
}
