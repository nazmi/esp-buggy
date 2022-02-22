#ifndef ENCODER_H
#define ENCODER_H

#include "mbed.h"
#include "QEI.h"
#include <chrono>

#define DIAMETER 0.08
#define M_PI 3.14159265358979323846264338327950288


class Encoder: public QEI{

    private:
    std::chrono::microseconds time_ms;
    int start_pulse,end_pulse,pulse_width; 
    int pulse_counter;
    double velocity;
    Timer t;
    Timeout to;

    static constexpr int CPR = 256;
    static constexpr double C = DIAMETER*M_PI;

    void calculate_speed();
    void stop();

    public:
    Encoder(PinName A, PinName B);

    void start();

    void reset_counter();
    double read_speed();
    double read_distance();
    int read_counter();
    

};

#endif