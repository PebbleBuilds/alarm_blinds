#include "BlindsServoSM.h"
#include <Servo.h>

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

  m_servo = Servo();
  m_servo.attach(iServoPin);
  m_servo.write(m_iStillCmd);
}

CBlindsServoSM::~CBlindsServoSM()
{
}

void CBlindsServoSM::Update()
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
