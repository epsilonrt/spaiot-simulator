#pragma once

#include <array>

namespace SpaIotSimulator {

  /**
     @class Bus
     @brief Spa bus

     This class simulates the SPI bus used by the Spa device.
  */
  class Bus {
    public:
      /**
         @brief Constructor

         Initialize the bus state, no hardware configuration is set (call begin() to set the hardware configuration)

         @param clkPin Clock pin number, this pin sample the data pin
         @param dataOutPin Data output pin number, the frame is outputed on this pin
         @param nWrPin write pin number, this pin is set to low when the data is outputed by dataOutPin, high when reading dataInPin
         @param dataInPin Data input pin number, this pin permit to read buttons states
      */
      Bus (int clkPin, int dataOutPin, int nWrPin, int dataInPin);

      /**
         @brief Initialize the pins of the bus

         The output pins are set to high, the input pin is set to input.
      */
      void begin();

      /**
         @brief Transfer a frame on the bus

         The nWR pin is set to low, the frame is outputed on the SDataOut pin sample on the SClk pin, the nWR pin is set to high.
         The timing is in concordance with the Spa device.
         @param data The frame to transfer
      */
      void transfer (uint16_t data);

      /**
         @brief Read the data in pin state

         @return true if the data in pin is high
      */
      bool dataInPin ();

    private:
      enum Pins {
        SClk = 0,
        SDataOut,
        nWR,
        SDataIn // must be the last one
      };
      std::array < int, SDataIn + 1 > m_pin;
  };

}
