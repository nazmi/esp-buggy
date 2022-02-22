#include "motor.h"

// Constructor
// l            -  PWM Motor Left
// r            - PWM Motor Right
// directionL   - direction motor left
// directionR   - direction motor right
// enable       - enable motor driver board
Motor::Motor(PinName l,PinName r,int directionL,int directionR,int enable): left(l),right(r){
    this->directionL = directionL;
    this->directionR = directionR;
    this->enable = enable;
}

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

// Set duty cycle of motor
// L indicates left motor
// R indicates right motor
// otherwise both
void Motor::set_dutycycle(char c,float dutycycle){

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





