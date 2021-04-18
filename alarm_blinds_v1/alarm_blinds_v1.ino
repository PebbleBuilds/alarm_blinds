#include <Adafruit_Keypad.h>
#include <Servo.h>

//////////////////////////////////
// Definitions
//////////////////////////////////

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


// __________DEBUG__________
#define DEBUG true

// __________SERVO STATE MACHINE_________

class CBlindsServoSM {
  public:
    CBlindsServoSM(int iServoPin, int iPeriod);
    ~CBlindsServoSM;

    void SetState(EState state) {m_state = state;}
    EState GetState() {return m_state};
    void Update();

  private:
    Servo m_servo;
    int m_iServoPin;
    int m_iPeriod; // period to update with, in ms

    // Position and direction info. Position measured in ms travelled in that direction.
    int m_iCurrentPosition; // When iPosition is 0, blinds are down.
    int m_iUpPosition; // When iPosition = iUpPosition, blinds are up.
    int m_iServoDir; // Current direction servo is moving in 

    const int m_iDownCmd;
    const int m_iUpCmd;
    const int m_iStillCmd;
    int m_iCurrCmd;

    enum EState {
      NOT_MOVING,
      AUTO_DOWN,
      AUTO_UP,
      AUTO_UP_WINDUP,
      MANUAL_DOWN,
      MANUAL_UP
    }
    EState m_state;
}

CBlindsServoSM::CBlindsServoSM(int iServoPin, int iPeriod)
{
  m_iServoPin = iServoPin;
  m_iPeriod = iPeriod;

  m_state = NOT_MOVING;

  m_iDownCmd = 0;
  m_iUpCmd = 180;
  m_iStillCmd = 90;
  m_iCurrCmd = m_iStillCmd;

  m_iCurrentPosition = 0;
  m_iUpPosition = 0;
  m_iServoDir = m_iUpCmd;

  m_servo = new Servo();
  m_servo.attach(iServoPin);
  m_servo.write(iStillCmd);
}

~CBlindsServoSM::CBlindsServoSM()
{
  delete m_servo;
}

CBlindsServoSM::Update()
{
  switch(m_state)
  {
    case(NOT_MOVING):
      m_iCurrCmd = m_iStillCmd;
      break;
    case(AUTO_DOWN):
      m_iCurrCmd = m_iDownCmd;
      if(m_iCurrentPosition <= 0)
      {
        SetState(NOT_MOVING);
        break;
      }
    case(AUTO_UP):
      m_iCurrCmd = m_iUpCmd;
      if(m_iCurrentPosition >= m_iUpPosition)
      {
        SetState(NOT_MOVING);
        break;
      }
    case(AUTO_UP_WINDUP):
      m_iCurrCmd = m_iDownCmd;
      if(m_iCurrentPosition <= -500)
      {
        SetState(AUTO_UP)
      }
      break;
    case(MANUAL_DOWN):
      m_iCurrCmd = m_iDownCmd;
      break;
    case(MANUAL_UP):
      m_iCurrCmd = m_iUpCmd;
      break;
  }

  m_servo.write(m_iCurrCmd);
  if (m_iCurrCmd == m_iDownCmd)
  {
    m_iCurrentPosition -= m_iPeriod;
  }
  if (m_iCurrCmd == m_iUpCmd)
  {
    m_iCurrentPosition += m_iPeriod;
  }
}

//////////////////////////////////
// Setup
//////////////////////////////////
void setup() {
// ___________KEYPAD____________
  customKeypad.begin();

// __________SERVO______________
  serv.attach(servoPin);
  serv.write(STILL);

// __________DEBUG__________
  if (DEBUG) {Serial.begin(9600);}
}

//////////////////////////////////
// Loop
//////////////////////////////////
void loop() {
  // put your main code here, to run repeatedly:
  customKeypad.tick();

  while(customKeypad.available()){
    keypadEvent e = customKeypad.read();
    if(e.bit.EVENT == KEY_JUST_RELEASED) {
      switch ((char)e.bit.KEY){
        // Move blinds to "down" position
        case '*':
          if (DEBUG) {Serial.println("Down");}
          break;
        // Stop blinds
        case '0':
          if (DEBUG) {Serial.println("Still");}
          break;
        // Move blinds to "up" position
        case '#':
          if (DEBUG) {Serial.println("Up");}
          break;
        // Set down (reference position)
        case '1':
          iCurrentPosition = 0;
        default:
          if (DEBUG) {Serial.println("Unrecognized");}
          break;
      }
    }
  }

  serv.write(iServoDir);
  if(iServoDir == UP)
  {
    iCurrentPosition += iPeriod;
  }
  if(iServoDir == DOWN)
  {
    iCurrentPosition -= iPeriod;
  }
  delay(iPeriod);
}
