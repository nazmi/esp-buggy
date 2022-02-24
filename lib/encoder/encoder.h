#ifndef ENCODER_H
#define ENCODER_H

#include "mbed.h"
#include "QEI.h"
#include <chrono>

#define GEAR_RATIO 18.75
/* From calibration */
#define DIAMETER 0.0841958 
/* From solidworks */
//#define DIAMETER 0.0820352 
#define WHEEL_DISTANCE 0.2021388
#define COUNTS_PER_REV 256
#define POLLING_PERIOD 10ms
#define M_PI 3.14159265358979323846264338327950288
#define radiansToDegrees(angle) (angle * 180.0 / M_PI)


class Encoder: public QEI{

    private:
    int start_pulse, end_pulse, pulse_width; 
    int pulse_counter;
    double velocity;
    double rpm;
    Timer t;
    Timeout to;

    void calculate_speed(double time_sec);
    void stop();

    public:
    Encoder(PinName A, PinName B);

    static constexpr double CIRCUMFERENCE = DIAMETER * M_PI;
    static constexpr double FULL_ROTATION_SIDE_PIVOT = 2 * M_PI * WHEEL_DISTANCE;
    static constexpr double FULL_ROTATION_CENTER_PIVOT = M_PI * WHEEL_DISTANCE;

    void start();
    void reset_counter();

    double read_velocity();
    double read_rpm();
    double read_distance();
    int read_counter();

    static double average_angular(Encoder& right, Encoder& left);
    static double average_velocity(Encoder& right, Encoder& left);
    static double average_distance(Encoder& right, Encoder& left);
    
    

};

#endif