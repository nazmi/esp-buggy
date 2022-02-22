#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include "mbed.h"
#include "motor.h"

class Potentiometer
{
    private:
        AnalogIn inputSignal;
        float VDD, currentSampleNorm, currentSampleVolts;
        char side;
        Motor* motor;
        Ticker t;
        

    public:
        Potentiometer(PinName pin, float v,Motor* m,char c);

        float amplitudeVolts();
        float amplitudeNorm();
        void sample();
        float read_volts();
        float read_norm();

};


#endif