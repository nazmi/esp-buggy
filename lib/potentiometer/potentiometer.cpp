#include "potentiometer.h"

// Constructor
// AnalogIn , PwmOut
// Start new thread and queue event every X seconds
Potentiometer::Potentiometer(PinName pin,PwmOut* pwm) : inputSignal(pin) {

    this->pwm = pwm;
    t.start(callback(&q, &EventQueue::dispatch_forever));
    q.call_every(POT_PERIOD, callback(this, &Potentiometer::update));
}

// Update value of duty cycle
void Potentiometer::update(){
    *pwm = inputSignal.read();
}

// Return raw volts
float Potentiometer::read_volts(){
    return inputSignal.read()*VDD;
}

// Return normalised value
float Potentiometer::read(){
    return inputSignal.read();
}