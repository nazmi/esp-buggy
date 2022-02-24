#ifndef ENCODER_H
#define ENCODER_H

#include "mbed.h"
#include "QEI.h"
#include <chrono>

#define GEAR_RATIO 18.75
#define DIAMETER 0.0841958 /* From calculation */
//#define DIAMETER 0.0820352 /* From solidworks */
#define WHEEL_DISTANCE 0.2021388
#define CPR 256
#define M_PI 3.14159265358979323846264338327950288
#define RAD_TO_DEGREE 57.295779513082


class Encoder: public QEI{

    private:
    std::chrono::microseconds time_ms;
    int start_pulse,end_pulse,pulse_width; 
    int pulse_counter;
    double velocity;
    double rpm;
    Timer t;
    Timeout to;

    

    void calculate_speed();
    void stop();

    public:
    Encoder(PinName A, PinName B);

    static constexpr double C = DIAMETER*M_PI;

    void start();
    void reset_counter();

    double read_velocity();
    double read_rpm();
    double read_distance();
    int read_counter();

    static double average_angular(Encoder& right,Encoder& left);
    static double average_velocity(Encoder& right,Encoder& left);
    static double average_distance(Encoder& right,Encoder& left);
    
    

};

#endif