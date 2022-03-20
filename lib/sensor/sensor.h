#ifndef SENSOR_H
#define SENSOR_H

#include "mbed.h"
#include "arm_math.h"
#include <cstdio>

#define OFFTRACK 0.15
#define PRESCALER 10.0f
#define NO_TRACK 30
#define TRESHOLD 2.8


class Sensor
{
    private:
        BusOut sensors;
        AnalogIn input[6];
        float sensor_data[6] {0.0};
        float noise[6] {0.0};
        float distance {0.0};
        

    public:
        Sensor(PinName p1, PinName p2, PinName p3, PinName p4, PinName p5, PinName p6, PinName in1,PinName in2,PinName in3,PinName in4,PinName in5,PinName in6);
        
        static float weights[6];
        static float treshold[6];
        static float scale_factor[6];
        static bool run;

        float read();
        void calibrate_black();
        void calibrate_white();


};


#endif