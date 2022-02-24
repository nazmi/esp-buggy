#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include "mbed.h"
#include "motor.h"

#define VDD 3.3
#define POT_PERIOD 10s

class Potentiometer
{
    private:
        AnalogIn inputSignal;
        PwmOut* pwm;
        Thread t;
        EventQueue q;
        

    public:
        Potentiometer(PinName pin,PwmOut* pwm);
        
        void update();
        float read_volts();
        float read();
        

};


#endif