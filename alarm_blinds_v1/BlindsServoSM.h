#include <Servo.h>

class CBlindsServoSM {
  public:
    CBlindsServoSM(int iServoPin, int iPeriod);
    ~CBlindsServoSM();

    enum EState {
      NOT_MOVING,
      AUTO_DOWN,
      AUTO_UP,
      AUTO_UP_WINDUP,
      MANUAL_DOWN,
      MANUAL_UP
    };

    void SetState(EState state) {m_state = state;}
    EState GetState() {return m_state;}
    void Update();
    void SetDownPosition() {m_iCurrentPosition = 0;}
    void SetUpPosition() {m_iUpPosition = m_iCurrentPosition;}

  private:
    Servo m_servo;
    int m_iServoPin;
    int m_iPeriod; // period to update with, in ms. delay enforced outside class.

    // Position and direction info. Position measured in ms travelled in that direction.
    int m_iCurrentPosition; // When iPosition is 0, blinds are down.
    int m_iUpPosition; // When iPosition = iUpPosition, blinds are up.
    int m_iServoDir; // Current direction servo is moving in 

    int m_iDownCmd;
    int m_iUpCmd;
    int m_iStillCmd;
    int m_iCurrCmd;
    EState m_state;
};