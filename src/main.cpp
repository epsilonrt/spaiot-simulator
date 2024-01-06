#include <iostream>
#include <cstdlib>
#include <csignal>
#include <map>
#include <spaiot-simulator.h>

using namespace SpaIotSimulator;

// convert string to pin number, return -1 if invalid
int strToPin (const char *str);
// Handle Ctrl+C and SIGTERM
void signalHandler (int sig);
void setDevice (int id, bool state);

// The engine instance is global to be able to handle signals
Engine *engine = nullptr;
uint16_t tempValue = 0;

int main (int argc, char *argv[]) {
  int dataOutPin, clkPin, nWrPin,  dataInPin;

  if (argc == 5) {

    dataOutPin = strToPin (argv[1]);
    clkPin  = strToPin (argv[2]);
    nWrPin   = strToPin (argv[3]);
    dataInPin   = strToPin (argv[4]);
  }

  if (argc != 5 || dataOutPin < 0 || clkPin < 0 || nWrPin < 0 || dataInPin < 0) {

    std::cerr << "Usage: " <<  argv[0] << " dataOutPin clkPin nWrPin dataInPin" << std::endl;
    exit (EXIT_FAILURE);
  }

  engine = new  Engine (dataOutPin, clkPin, nWrPin, dataInPin);
  engine->begin();

  signal (SIGINT, signalHandler);
  signal (SIGTERM, signalHandler);
  std::cout << "Press Ctrl+C to abort ..." << std::endl;

  tempValue = engine->display();
  int previousButtons = 0;

  for (;;) {
    int buttonStates = engine->poll();
    int buttonChanges = previousButtons ^ buttonStates;

    if (buttonChanges) {

      for (int buttonId = 0; buttonChanges != 0; buttonId++) {
        int buttonFlag = (1 << buttonId);

        if (buttonChanges & buttonFlag) {
          bool buttonState = (buttonStates & buttonFlag) != 0;

          setDevice (buttonId, buttonState);
          buttonChanges &= ~ buttonFlag;
        }
      }
      previousButtons = buttonStates;
    }

  }
  return 0;
}

// -----------------------------------------------------------------------------
int strToPin (const char *str) {
  int pin = -1;

  if (str) {
    long lnum;
    char *end;
    errno = 0;
    lnum = strtol (str, &end, 10);
    if (errno == 0 && *end == '\0' && lnum >= 0 && lnum <= 255) {

      pin = lnum;
    }
    else {

      std::cerr << "Invalid pin number: " << str << std::endl;
    }
  }

  return pin;
}

// -----------------------------------------------------------------------------
void
signalHandler (int sig) {

  // endwin();
  // nocbreak();
  engine->enableDisplay (false);
  setDevice (BtnPower, true);
  engine->poll();
  setDevice (BtnPower, false);
  engine->poll();
  delete engine;
  std::cout << std::endl << "Have a nice day !" << std::endl;
  exit (EXIT_SUCCESS);
}

// -----------------------------------------------------------------------------
void setDevice (int id, bool state) {

  static const std::map<int, const char *> labels = {
    {BtnPower, "Power"},
    {BtnFilter, "Filter"},
    {BtnHeat, "Heat"},
    {BtnBubble, "Bubble"},
    {BtnFc, "F/C"},
    {BtnUp, "Up"},
    {BtnDown, "Down"}
  };

  if (state) {

    engine->setBuzzer (true);
    switch (id) {
      case BtnFilter:
      case BtnHeat:
      case BtnBubble:
        if (engine->led (LedPower) == false) {

          break;
        }
      case BtnPower:
        engine->toggleLed (id);
        if (id == LedPower &&  !engine->led (LedPower)) {

          engine->clearLed (LedFilter);
          engine->clearLed (LedBubble);
          engine->clearLed (LedHeater);
        }
        break;

      case BtnFc:
        tempValue = engine->isCelcius() ? Engine::celciusToFahrenheit (tempValue) : Engine::fahrenheitToCelcius (tempValue);
        engine->setCelcius (!engine->isCelcius());
        break;

      case BtnUp:
        if (engine->led (LedPower) && tempValue < 99) {
          tempValue++;
          engine->setDisplay (tempValue);
        }
        break;

      case BtnDown:
        if (engine->led (LedPower) && tempValue > 0) {
          tempValue--;
          engine->setDisplay (tempValue);
        }
        break;

      default:
        break;
    }

  }
  else {

    engine->setBuzzer (false);
  }
  std::cout << labels.at (id) << " " << (state ? "press" : "release") << std::endl;
}
