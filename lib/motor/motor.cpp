#include "motor.h"

// Constructor
// l -  PWM Motor Left
// r - PWM Motor Right
// dL - direction motor left
// dR - direction motor right
// enable_pin - enable motor driver board
Motor::Motor(PinName l,PinName r,PinName dL,PinName dR,PinName enable_pin): 
            left(l), right(r), directionL(dL,1), directionR(dR,1), enable(enable_pin, 0) {}

// Set direction of motor
// L indicates left motor
// R indicates right motor
// otherwise both
void Motor::set_direction(char c,int direction){

    if(c == 'L')
        this->directionL = direction;
    else if(c == 'R')
        this->directionR = direction;
    else
    {
        this->directionR = this->directionL = direction;
    }
}

// Set frequency of PWM
void Motor::set_frequency(int freq){

    this->period = 1.0/freq;
    left.period(this->period);
    right.period(this->period);

}

// Set enable bit of motor driver
void Motor::set_enable(int enable){

    this->enable = enable;

}

// Flip enable if not given
void Motor::set_enable(){

    this->enable = !this->enable;

}



// Set duty cycle of motor
// L indicates left motor
// R indicates right motor
// otherwise both
void Motor::set_dutycycle(char c,float dutycycle){

    if(dutycycle > 1) 
        dutycycle = 1.0;
    else if(dutycycle < 0) 
        dutycycle = 0;

    if(c == 'L')
        left.write(dutycycle);
    else if(c == 'R')
        right.write(dutycycle);
    else
    {
        left.write(dutycycle);
        right.write(dutycycle);

    }
}





