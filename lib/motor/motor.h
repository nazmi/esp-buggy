#ifndef MOTOR_H
#define MOTOR_H

#include "mbed.h"

#define GEAR_RATIO 18.75
#define UNIPOLAR 0
#define BIPOLAR 1


class Motor{

    private:
    PwmOut left;
    PwmOut right;
    int enable;
    int directionL;
    int directionR;
    float period;
    static const int mode = UNIPOLAR;

    public:
    
    Motor(PinName l,PinName r,int directionL,int directionR,int enable);

    void set_direction(char c,int direction);
    void set_frequency(int freq);
    void set_dutycycle(char c,float dutycycle);

    

};


#endif