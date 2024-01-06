#pragma once

#include <spaiot/simulator/engine.h>

namespace SpaIotSimulator {

  enum ShiftRegFlag {
    D4_POWER = 1 << 15,
    S1_FILTER = 1 << 14,
    DSP2 = 1 << 13, // <
    S2_BUBBLE = 1 << 12,
    C = S2_BUBBLE,
    G = 1 << 11,
    DSP1_2 = 1 << 10, // <
    DSP1_3 = 1 << 9, // <
    S4_DOWN = 1 << 8,
    D2_HEAT_R = S4_DOWN,
    B = S4_DOWN,
    BUZ = 1 << 7, // <
    D2_HEAT_G = 1 << 6,
    F = D2_HEAT_G,
    S3_POWER = 1 << 5,
    D1_BUBBLE = S3_POWER,
    A = S3_POWER,
    DSP1_1 = 1 << 4, // <
    S5_UP = 1 << 3,
    D3_FILTER = S5_UP,
    E = S5_UP,
    S6_FC = 1 << 2,
    D = S6_FC,
    LED = 1 << 1, // <
    S7_HEAT = 1 << 0,
    DP = S7_HEAT
  };

  const uint16_t IdleFrame = 0xFFFF & ~BUZ;
  const uint16_t S1FilterFrame = IdleFrame & ~S1_FILTER;
  const uint16_t S2BubbleFrame = IdleFrame & ~S2_BUBBLE;
  const uint16_t S3PowerFrame = IdleFrame & ~S3_POWER;
  const uint16_t S4DownFrame = IdleFrame & ~S4_DOWN;
  const uint16_t S5UpFrame = IdleFrame & ~S5_UP;
  const uint16_t S6FcFrame = IdleFrame & ~S6_FC;
  const uint16_t S7HeaterFrame = IdleFrame & ~S7_HEAT;

  const uint16_t LedMask = D4_POWER | D3_FILTER | D1_BUBBLE | D2_HEAT_G | D2_HEAT_R;

  const uint16_t DigitMask =  A + B + C + D + E + F + G + DP;
  const uint16_t Digit0 =  D + E + F + A + B + C;       // 0
  const uint16_t Digit1 =   E + F;                      // 1
  const uint16_t Digit2 =   D + E + G + B + A;          // 2
  const uint16_t Digit3 =   D + E + G + F + A;          // 3
  const uint16_t Digit4 =   E + G + F + C;              // 4
  const uint16_t Digit5 =   D + C + G + F + A;          // 5
  const uint16_t Digit6 =   D + C + B + A + F + G;      // 6
  const uint16_t Digit7 =   D + E + F;                  // 7
  const uint16_t Digit8 =   D + E + F + A + B + C + G;  // 8
  const uint16_t Digit9 =   D + E + F + A + C + G;      // 9
  const uint16_t DigitF =   D + C + G + B + DP;         // °F
  const uint16_t DigitC =   D + C + B + A + DP;         // °C

  const uint16_t LedFlag[] = { (uint16_t) D4_POWER, (uint16_t) D3_FILTER,
                               (uint16_t) D1_BUBBLE, (uint16_t) D2_HEAT_G,
                               (uint16_t) D2_HEAT_R
                             };

  const uint16_t ButtonFlag[] = { (uint16_t) S1_FILTER, (uint16_t) S7_HEAT,
                                  (uint16_t) S5_UP, (uint16_t) S4_DOWN,
                                  (uint16_t) S2_BUBBLE, (uint16_t) S3_POWER,
                                  (uint16_t) S6_FC
                                };

  const uint16_t DisplayFlag[] = { (uint16_t) DSP1_3, (uint16_t) DSP1_2,
                                   (uint16_t) DSP1_1, (uint16_t) DSP2
                                 };
}
