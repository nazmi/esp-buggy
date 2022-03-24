#ifndef SENSOR_H
#define SENSOR_H

#include "arm_math.h"
#include "mbed.h"
#include <algorithm>
#include <array>
#include <cstdio>

#define OFFTRACK 0.15
#define PRESCALER 10.0f
#define NO_TRACK 30
#define WHITE_TRESHOLD 0.8

class Sensor {
  private:
    BusOut m_pins;
    std::array<AnalogIn, 6> m_analog;
    std::array<float, 6> m_reading{};
    std::array<float, 6> m_noise{};
    float m_distance{0};

  public:
    Sensor(PinName p1, PinName p2, PinName p3, PinName p4, PinName p5, PinName p6, PinName in1, PinName in2, PinName in3, PinName in4, PinName in5, PinName in6);

    static std::array<float, 6> WEIGHTS;
    static std::array<float, 6> BLACK_TRESHOLD;
    static std::array<float, 6> SCALE_FACTOR;
    static bool s_run;

    float read();
    void toggle(const bool on);
    float getDistance() const;
    void calibrate_black();
    void calibrate_white();
};

#endif