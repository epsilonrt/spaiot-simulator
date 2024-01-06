#include <Arduino.h>
#include <spaiot/simulator/bus.h>

namespace SpaIotSimulator {


  //----------------------------------------------------------------------------
  Bus::Bus (int clkPin, int dataOutPin, int nWrPin, int dataInPin) :
    m_pin {clkPin, dataOutPin, nWrPin, dataInPin} {

  }

  //----------------------------------------------------------------------------
  void Bus::begin() {

    for (int i = 0; i < (m_pin.size() - 1); i++) {

      pinMode (m_pin[i], OUTPUT);
      digitalWrite (m_pin[i], HIGH);
    }
    pinMode (m_pin[SDataIn], INPUT);
  }

  //----------------------------------------------------------------------------
  void Bus::transfer (uint16_t data) {
    uint16_t mask = 1;

    digitalWrite (m_pin[nWR], LOW);

    while (mask) {
      digitalWrite (m_pin[SClk], LOW);
      delayMicroseconds (5);
      digitalWrite (m_pin[SDataOut], (mask & data) != 0);
      delayMicroseconds (2);
      digitalWrite (m_pin[SClk], HIGH);
      delayMicroseconds (3);
      mask <<= 1;
    }
    digitalWrite (m_pin[nWR], HIGH);
  }

  //----------------------------------------------------------------------------
  bool Bus::dataInPin() {
    return digitalRead (m_pin[SDataIn]) != LOW;
  }
}
