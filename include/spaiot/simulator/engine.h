#pragma once

#include <cstdint>
#include <array>
#include "bus.h"

namespace SpaIotSimulator {

  /**
     @brief Led identifiers

     Used to identify the led to set, clear or toggle.
  */
  enum LedId {
    LedPower = 0,
    LedFilter,
    LedBubble,
    LedHeaterGreen,
    LedHeaterRed,
    NofLeds,
    LedHeater = LedHeaterGreen
  };

  /**
     @brief Button identifiers

     Used to identify the button to read with the button() function and the bits in the return value of the poll() function.
  */
  enum ButtonId {
    BtnPower = 0,
    BtnFilter,
    BtnBubble,
    BtnHeat,
    BtnUp,
    BtnDown,
    BtnFc,
    NofButtons
  };


  /**
     @class Engine
     @brief Engine simulator

     This class simulates the Spa device.
  */
  class Engine {
    public:
      /**
         @brief Constructor

         Sets all the leds off, the display to 20, the buzzer off, the temperature unit to Celcius and the display enabled.
      */
      Engine (int clkPin, int dataOutPin, int nWrPin, int dataInPin);

      /**
         @brief Initialize the engine

         This function must be called once before any other call to the engine.
         It initializes the SPI bus and the engine internal state.
      */
      void begin();

      /**
         @brief Poll the engine

         Execute a full scan of the engine, this is the main function of the engine :
         - transfer the led frame with bool states as sets by setLed() call by the application, then
         - transfer the display frame with the display value as set by setDisplay() call by the application, then
         .

         This sequence is repeated 5 times with a delay of 260 microseconds between each frame.
         Terminates by a scan of the buttons and return the button states.

         @return uint16_t the button states, each bit represents a button state, 1 for pressed, 0 for released, the bit order is defined by the ButtonId enum.
      */
      int poll();

      /**
         @brief Set the Led state which is send by the next poll() call

         @param id Led identifier, see LedId enum
         @param state Led state, true for on, false for off, default is true
      */
      void setLed (int id, bool state = true);

      /**
         @brief Clear the Led state which is send by the next poll() call

         @param id Led identifier, see LedId enum
      */
      void clearLed (int id);

      /**
         @brief Toggle the Led state which is send by the next poll() call

         @param id Led identifier, see LedId enum
      */
      void toggleLed (int id);

      /**
         @brief Get the Led state internally stored

         @param id Led identifier, see LedId enum
      */
      bool led (int id) const;

      /**
         @brief Set the display value which is send by the next poll() call

         @param value Display value, range 0..999, if value is out of range an std::invalid_argument exception  is thrown
      */
      void setDisplay (uint16_t value);

      /**
         @brief Get the display value internally stored

         @return uint16_t Display value, range 0..999
      */
      uint16_t display() const;

      /**
         @brief Enable/disable the display

         The display is enabled by default, this method can be used to disable it.
         In this case the display is unlit and the display value is not sent by the next poll() call.

         @param state true to enable the display, false to disable it
      */
      void enableDisplay (bool state = true);

      /**
         @brief Get the display value internally stored

         @return bool state of the display, true if enabled, false otherwise
      */
      bool isDisplayEnabled() const;

      /**
         @brief Get the Button state internally stored

         @param id Button identifier, see ButtonId enum
      */
      bool button (int id) const;

      /**
         @brief Set the Buzzer state which is send by the next poll() call

         @param state Buzzer state, true for on, false for off, default is true
      */
      void setBuzzer (bool state = true);

      /**
         @brief Get the Buzzer state internally stored

         @return true Buzzer is on
      */
      bool isBuzzing() const;

      /**
         @brief Set the temperature unit to Celcius or Fahrenheit

         This function change the left most digit of the display to 'C' or 'F' depending on the state parameter 
         and the display value is converted to the new unit.

         @param state true for Celcius, false for Fahrenheit
      */
      void setCelcius (bool state = true);

      /**
         @brief Get the temperature unit

         @return true Celcius, false Fahrenheit
      */
      bool isCelcius() const;

      /**
         @brief Convert Celcius to Fahrenheit

         @param c Celcius temperature
         @return int Fahrenheit temperature
      */
      static int celciusToFahrenheit (double f);

      /**
         @brief Convert Fahrenheit to Celcius

         @param f Fahrenheit temperature
         @return int Celcius temperature
      */
      static int fahrenheitToCelcius (double c);

    protected:
      enum DisplayId {
        Display100 = 0,
        Display10,
        Display1,
        DisplayFc,
        NofDisplays
      };
      uint16_t ledFrame (uint16_t idleFrame);
      uint16_t displayFrame (uint16_t idleFrame, int id);
      int scanButtons (uint16_t idleFrame);

    private:
      uint16_t m_display;
      bool m_displayEn;
      bool m_buzzer;
      bool m_celcius;
      std::array<bool, NofLeds> m_led;
      std::array<bool, NofButtons> m_button;
      Bus m_bus;
  };
}
