#include "motor.h"
#include <utility>

// Constructor
// l -  PWM Motor Left
// r - PWM Motor Right
// dL - direction motor left
// dR - direction motor right
// enable_pin - enable motor driver board
Motor::Motor(PinName l,PinName r,PinName dL,PinName dR,PinName s): 
            left(l), right(r), directionL(dL, 1), directionR(dR, 1), state(s, 0) {}

// Set direction of motor
// L indicates left motor
// R indicates right motor
// otherwise both
void Motor::set_direction(char c,int direction){

    if  (c == 'L')      
        this->directionL = direction;
    else if(c == 'R')   
        this->directionR = direction;
    else
    {
        this->directionR = this->directionL = direction;
    }
}

// Flip direction of motor
// L indicates left motor
// R indicates right motor
// otherwise both
void Motor::set_direction(char c){

    if(c == 'L')
        this->directionL = !this->directionL;
    else if(c == 'R')
        this->directionR = !this->directionR;
    else
    {
        this->directionR = !this->directionR;
        this->directionL = !this->directionL;
    }
}

// Get pair of direction <left,right>
pair<int,int> Motor::get_direction(){

    return make_pair(this->directionL, this->directionR);
}

// Set frequency of PWM
void Motor::set_frequency(float freq){

    this->period = 1.0 / freq;
    left.period(this->period);
    right.period(this->period);

}

// Get current frequency
float Motor::get_frequency(){
    return 1.0 / period;
}

// Set enable bit of motor driver
void Motor::set_enable(int state){

    this->state = state;

}

// Flip enable if not given value
void Motor::set_enable(){

    this->state = !this->state;

}

// Get enable state
int Motor::get_enable(){
    return state;
}

// Set duty cycle of motor
// L indicates left motor
// R indicates right motor
// otherwise both

// Duty cycle applied and motor out is inverted
void Motor::set_dutycycle(char c, float dutycycle){

    if(dutycycle > 1)
        dutycycle = 1.0;
    else if(dutycycle < 0) 
        dutycycle = 0;

    if(c == 'L')
        left.write(1 - dutycycle);
    else if(c == 'R')
        right.write(1 - dutycycle);
    else
    {
        left.write(1 - dutycycle);
        right.write(1 - dutycycle);

    }
}

// Get pair of dutycycle <left,right>
pair<float,float> Motor::get_dutycycle(){

    return make_pair( 1 - left.read() , 1 - right.read() );

}


// Cruising methods
void Motor::forward(double distance, Motor* motor, Encoder* left, Encoder* right){

    motor->set_enable(0);

    motor->set_dutycycle('A', SLOW_PWM);
    motor->set_direction('A', 1);
    left->reset_counter();
    right->reset_counter();

    while(Encoder::average_distance(*right, *left) < distance) { motor->set_enable(1); }

    motor->set_enable(0);

}

void Motor::reverse(double distance, Motor* motor, Encoder* left, Encoder* right){

    motor->set_enable(0);

    motor->set_dutycycle('A', SLOW_PWM);
    motor->set_direction('A', 0);
    left->reset_counter();
    right->reset_counter();

    while(Encoder::average_distance(*right, *left) > -distance) { motor->set_enable(1); }

    motor->set_enable(0);

}

//  Turn left : Right ON | LEFT OFF
void Motor::turnleft(double angle, Motor* motor, Encoder* left, Encoder* right){

    motor->set_enable(0);

    motor->set_dutycycle('L', 0);
    motor->set_dutycycle('R', SLOW_PWM);
    motor->set_direction('A', 1);
    left->reset_counter();
    right->reset_counter();

    while(right->read_distance() < ( Encoder::FULL_ROTATION_SIDE_PIVOT * (angle/360.0) ) ) { motor->set_enable(1); }

    motor->set_enable(0);

}

//  Turn right : Right OFF | LEFT ON
void Motor::turnright(double angle,Motor* motor,Encoder* left,Encoder* right){

    motor->set_enable(0);

    motor->set_dutycycle('L', SLOW_PWM);
    motor->set_dutycycle('R', 0);
    motor->set_direction('A', 1);
    left->reset_counter();
    right->reset_counter();

    while(left->read_distance() < ( Encoder::FULL_ROTATION_SIDE_PIVOT * ( angle / 360.0 ) ) ) { motor->set_enable(1); }

    motor->set_enable(0);

}