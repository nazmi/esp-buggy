#include "motor.h"
#include <utility>

// Constructor
// r - PWM Motor Right
// l -  PWM Motor Left
// dR - direction motor right
// dL - direction motor left
// enable_pin - enable motor driver board
Motor::Motor(PinName r,PinName l,PinName dr,PinName dl,PinName s): 
            right(r), left(l), directionR(dr, 1), directionL(dl, 1), state(s, 0) {}

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
// adjusted due to right encoder not working
void Motor::forward(double distance, Motor* motor, Encoder* left, Encoder* right){

    motor->set_enable(0);

    motor->set_dutycycle('L', SLOW_PWM);
    motor->set_dutycycle('R', SLOW_PWM);
    motor->set_direction('A', 1);
    left->reset_counter();
    right->reset_counter();

    // while(abs(left->read_distance()) < distance) { 
    //     motor->set_enable(1);
    // }

    while(abs(Encoder::average_distance(*right,*left)) < distance) { 
        motor->set_enable(1);

        if(left->read_counter() < right->read_counter()){

            auto correction = (double) right->read_counter() / (double) left->read_counter();
            motor->set_dutycycle('L',SLOW_PWM*correction*1.04);
            motor->set_dutycycle('R',SLOW_PWM);


        }
        else if (right->read_counter() < left->read_counter()){

            auto correction = (double) left->read_counter() / (double) right->read_counter();
            motor->set_dutycycle('R',SLOW_PWM*correction);
            motor->set_dutycycle('L',SLOW_PWM*1.04);

        }

    }

    motor->set_enable(0);

}

void Motor::reverse(double distance, Motor* motor, Encoder* left, Encoder* right){

    motor->set_enable(0);

    motor->set_dutycycle('L', SLOW_PWM);
    motor->set_dutycycle('R', SLOW_PWM);
    motor->set_direction('A', 0);
    left->reset_counter();
    right->reset_counter();

    // while(abs(left->read_distance()) < distance) { 
    //     motor->set_enable(1);
    // }

    while(abs(Encoder::average_distance(*right,*left)) < distance) { 
        motor->set_enable(1);
    }

    motor->set_enable(0);

}

//  Turn left : Right ON | LEFT OFF
void Motor::turnleft(double angle, Motor* motor, Encoder* left, Encoder* right){

    motor->set_enable(0);
    left->reset_counter();
    right->reset_counter();
    // motor->set_dutycycle('L', SLOW_PWM);
    // motor->set_direction('L', 0);
    // motor->set_dutycycle('R', SLOW_PWM);
    // motor->set_direction('R', 1);
    
    

    // while(abs(left->read_distance()) < ( Encoder::FULL_ROTATION_CENTER_PIVOT * (angle/360.0) ) ) { 
    //     motor->set_enable(1);
    // }
    motor->set_dutycycle('L', 0);
    motor->set_dutycycle('R', HALF_PWM);
    motor->set_direction('R', 1);
    while(abs(right->read_distance()) < ( Encoder::FULL_ROTATION_SIDE_PIVOT * (angle/360.0) ) ) { 
        motor->set_enable(1);
    }
    motor->set_enable(0);

}

//  Turn right : Right OFF | LEFT ON
void Motor::turnright(double angle,Motor* motor,Encoder* left,Encoder* right){

    motor->set_enable(0);
    left->reset_counter();
    right->reset_counter();

    // motor->set_dutycycle('L', SLOW_PWM);
    // motor->set_direction('L', 1);
    // motor->set_dutycycle('R', SLOW_PWM);
    // motor->set_direction('R', 0);
   
    // while(abs(left->read_distance()) < ( Encoder::FULL_ROTATION_CENTER_PIVOT * ( angle / 360.0 ) ) ) { 
    //     motor->set_enable(1);
    // }

    motor->set_dutycycle('R', 0);
    motor->set_dutycycle('L', HALF_PWM);
    motor->set_direction('L', 1);
    while(abs(left->read_distance()) < ( Encoder::FULL_ROTATION_SIDE_PIVOT * (angle/360.0) ) ) { 
        motor->set_enable(1);
    }
    motor->set_enable(0);

}