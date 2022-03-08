#ifndef SENSOR_H
#define SENSOR_H

#include "mbed.h"
#include <algorithm>
#include <cstdio>

#define OFFTRACK 0.15
#define VDD 3.3

class Sensor
{
    private:
        AnalogIn input;
        BusOut sensors;
        float sensor_data[6], upper_limit[6], lower_limit[6];
        float distance;

        // 0 1 2
        // 3 4 5
        static constexpr float weight[6] {-40, 0, 40 , -20, 0, 20};
        

    public:
        Sensor(PinName p1, PinName p2, PinName p3, PinName p4, PinName p5, PinName p6, PinName in);
        
        float read();
        void calibrate();


};


#endif