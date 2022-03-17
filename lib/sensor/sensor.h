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
        AnalogIn input[6];
        BusOut sensors;
        float sensor_data[6] {0.0};
        float noise[6] {0.0};
        float distance {0.0};
        

    public:
        Sensor(PinName p1, PinName p2, PinName p3, PinName p4, PinName p5, PinName p6, PinName in1,PinName in2,PinName in3,PinName in4,PinName in5,PinName in6);
        
        static float weights[6];
        static constexpr float treshold[6]      { 0.07828519,	0.09378702,	0.10737747,	0.09584597,	0.09437664,	0.13561697 };
        static constexpr float scale_factor[6]  { 1.006342313,	1.019787556,	1.031426983,	1.018563463,	1.014853279,	1.099818972 };
        static bool run;

        float read();
        void calibrate_black();
        void calibrate_white();


};


#endif