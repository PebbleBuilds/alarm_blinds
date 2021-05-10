#include "BlindsServoSM.h"
#include <arduino-timer.h>

CBlindsServoSM::CBlindsServoSM(int iServoPin)
{
  m_iServoPin = iServoPin;

  m_state = NOT_MOVING;

  m_iDownCmd = 0;
  m_iUpCmd = 180;
  m_iStillCmd = 90;
  m_iCurrCmd = m_iStillCmd;

  m_iCurrentPosition = 0;
  m_iUpPosition = 0;
  m_iServoDir = m_iUpCmd;

  Servo m_servo;
}

CBlindsServoSM::~CBlindsServoSM()
{
}

void CBlindsServoSM::Setup()
{
  m_servo.attach(m_iServoPin);
  m_servo.write(m_iStillCmd);
}

void CBlindsServoSM::Update()
{
  // Handle SM
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
        SetState(AUTO_UP);
      }
      break;
    case(MANUAL_DOWN):
      m_iCurrCmd = m_iDownCmd;
      break;
    case(MANUAL_UP):
      m_iCurrCmd = m_iUpCmd;
      break;
  }

  // Write to servo based on state
  m_servo.write(m_iCurrCmd);

  // Calculate time elapsed since last run
  unsigned long ulCurrentMicros = micros();
  unsigned long ulMicrosElapsed;
  if(ulCurrentMicros > m_ulLastMicros) // check for overflow
  {
    ulMicrosElapsed = ulCurrentMicros - m_ulLastMicros;
  }
  else
  {
    ulMicrosElapsed = 0; // just skip updating if there was an overflow
  }
  m_ulLastMicros = ulCurrentMicros;
  int iMicrosElapsed = int(ulMicrosElapsed);

  // Update "position" based on time elapsed
  if (m_iCurrCmd == m_iDownCmd)
  {
    m_iCurrentPosition -= iMicrosElapsed;
  }
  if (m_iCurrCmd == m_iUpCmd)
  {
    m_iCurrentPosition += iMicrosElapsed;
  }
}
