#include <exception>
#include <cmath>
#include <Arduino.h>
#include "engine_p.h"

namespace SpaIotSimulator {

  const uint16_t FreezeTime = 260;
  const uint16_t DigitFlag[] = {
    Digit0,
    Digit1,
    Digit2,
    Digit3,
    Digit4,
    Digit5,
    Digit6,
    Digit7,
    Digit8,
    Digit9
  };

  //----------------------------------------------------------------------------
  //
  //                            Engine Class
  //
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  Engine::Engine (int clkPin, int dataOutPin, int nWrPin, int dataInPin) :
    m_display (20),
    m_displayEn (true),
    m_buzzer (false),
    m_celcius (true),
    m_bus (clkPin, dataOutPin, nWrPin, dataInPin) {

    m_led.fill (false);
    m_button.fill (false);
  }

  //----------------------------------------------------------------------------
  void Engine::begin() {

    m_bus.begin();
  }

  //----------------------------------------------------------------------------
  int Engine::poll() {
    uint16_t idle = IdleFrame | (m_buzzer ? BUZ : 0);

    for (int it = 0; it < 5; it++) {

      // Leds
      m_bus.transfer (ledFrame (idle));
      delayMicroseconds (FreezeTime);

      for (int id = 0; id < NofDisplays; id++) {

        m_bus.transfer (idle);
        m_bus.transfer (displayFrame (idle, id));
        delayMicroseconds (FreezeTime);
      }

      m_bus.transfer (idle);
      delayMicroseconds (FreezeTime);
    }

    return scanButtons (idle);
  }

  //----------------------------------------------------------------------------
  bool Engine::led (int i) const {

    return m_led.at (i);
  }

  //----------------------------------------------------------------------------
  void Engine::setLed (int i, bool state) {

    m_led.at (i) = state;
  }

  //----------------------------------------------------------------------------
  void Engine::clearLed (int i) {

    setLed (i, false);
  }

  //----------------------------------------------------------------------------
  void Engine::toggleLed (int i) {

    setLed (i, !led (i));
  }

  //----------------------------------------------------------------------------
  bool Engine::button (int i) const {

    return m_button.at (i);
  }

  //----------------------------------------------------------------------------
  bool Engine::isBuzzing() const {

    return m_buzzer;
  }

  //----------------------------------------------------------------------------
  void Engine::setBuzzer (bool state) {

    m_buzzer = state;
  }

  //----------------------------------------------------------------------------
  bool Engine::isCelcius() const {

    return m_celcius;
  }

  //----------------------------------------------------------------------------
  void Engine::setCelcius (bool state) {

    if (state != m_celcius) {

      m_display = state ? fahrenheitToCelcius (m_display) :
                  celciusToFahrenheit (m_display);
      m_celcius = state;
    }
  }

  //----------------------------------------------------------------------------
  uint16_t Engine::display() const {

    return m_display;
  }

  //----------------------------------------------------------------------------
  void Engine::setDisplay (uint16_t value) {

    if (value >= 1000) {
      throw std::invalid_argument ("display value out of range");
    }
    m_display = value;
  }

  //----------------------------------------------------------------------------
  bool Engine::isDisplayEnabled() const {

    return m_displayEn;
  }

  //----------------------------------------------------------------------------
  void Engine::enableDisplay (bool state) {

    m_displayEn = state;
  }

  //------------------------------------------------------------------------------
  // static
  int Engine::celciusToFahrenheit (double t) {

    return std::lround ( (9 * t) / 5 + 32);
  }

  //------------------------------------------------------------------------------
  // static
  int Engine::fahrenheitToCelcius (double t) {

    return std::lround ( ( (t - 32) * 5) / 9);
  }
  //----------------------------------------------------------------------------
  // protected
  uint16_t Engine::ledFrame (uint16_t frame) {

    for (int id = 0; id < NofLeds; id++) {

      if (m_led[id]) {

        frame &= ~ (LedFlag[id]);
      }
    }
    frame &= ~LED;
    return frame;
  }

  //----------------------------------------------------------------------------
  // protected
  uint16_t Engine::displayFrame (uint16_t frame, int id) {

    if (id == DisplayFc) {

      if (m_celcius) {

        frame &= ~DigitC;
      }
      else {

        frame &= ~DigitF;
      }
    }
    else {
      if (m_displayEn) {

        int bcd[] = { m_display / 100, m_display / 10, m_display % 10};
        bcd[Display10] -= bcd[Display100] * 10;

        frame &= ~DigitFlag[bcd[id]];
      }
    }
    frame &= ~DisplayFlag[id];
    return frame;
  }

  //----------------------------------------------------------------------------
  // protected
  int Engine::scanButtons (uint16_t frame) {
    int rc = 0;
    // Scan order flags
    // S1_FILTER, S7_HEAT, S5_UP, S4_DOWN, S2_BUBBLE, S3_POWER, S6_FC
    static const int buttonIndex[NofButtons] = {BtnFilter, BtnHeat, BtnUp,
                                                BtnDown, BtnBubble, BtnPower,
                                                BtnFc
                                               };

    for (int f = 0; f < NofButtons; f++) {
      int i = buttonIndex[f];

      m_bus.transfer (frame & ~ButtonFlag[f]);
      delayMicroseconds (5);
      m_button[i] = ! m_bus.dataInPin();
      rc |= m_button[i] ? 1 << i : 0;
    }
    return rc;
  }

}
