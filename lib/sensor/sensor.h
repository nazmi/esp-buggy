#ifndef SENSOR_H
#define SENSOR_H

#include "mbed.h"
#include <cstdio>
#include <numeric>
#include <limits>

#define OFFTRACK 0.15
#define PRESCALER 2
#define MAX_DISTANCE 674045041.0

class Sensor
{
    private:
        AnalogIn input[6];
        BusOut sensors;
        float sensor_data[6] {0.0}, noise[6] {0.0};
        float distance {0.0};
        
        // 0 1 2
        // 3 4 5
        static constexpr float weight[6] {47,28,7,-7,-19,-31};
        
        static constexpr float treshold[6] {0.02666367,	0.04516327,	0.03560837,	0.04705085,	0.04266423,	0.03508913};

        static constexpr float scale_factor[6] {1.580680823,	1.382250323,	1.637246257,	1.523113522,	1.633290298,	2.032013615};
        static bool run;
        

    public:
        Sensor(PinName p1, PinName p2, PinName p3, PinName p4, PinName p5, PinName p6, PinName in1,PinName in2,PinName in3,PinName in4,PinName in5,PinName in6);
        
        float read();
        void calibrate_black();
        void calibrate_white();


};


#endif