#include "potentiometer.h"

Potentiometer::Potentiometer(PinName pin, float v,Motor* m,char c) : inputSignal(pin), VDD(v) {

    this->motor = m;
    this->side = c;
    // Attach 200ms ticker (5 Hz) to sample
    t.attach(callback(this,&Potentiometer::sample),200ms);

}

float Potentiometer::amplitudeVolts()
{
    return amplitudeNorm()*VDD;
}

float Potentiometer::amplitudeNorm()
{
    return inputSignal.read();
}

void Potentiometer::sample()
{
    currentSampleNorm = amplitudeNorm();
    currentSampleVolts = currentSampleNorm * VDD;

    motor->set_dutycycle(this->side, currentSampleNorm);
}

float Potentiometer::read_volts()
{
    return currentSampleVolts;
}

float Potentiometer::read_norm()
{
    return currentSampleNorm;
}



