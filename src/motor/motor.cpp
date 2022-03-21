#include "motor.h"


Motor::Motor(PinName r,PinName l,PinName dr,PinName dl,PinName s): 
            right(r), left(l), directionR(dr, 1), directionL(dl, 1), state(s, 0) {}

void Motor::set_enable(int state){

    this->state = state;

}

void Motor::set_enable(){

    this->state = !this->state;

}

int Motor::get_enable(){
    return state;
}

void Motor::set_frequency(float freq){

    this->period = 1.0 / freq;
    left.period(this->period);
    right.period(this->period);

}

float Motor::get_frequency(){
    return 1.0 / period;
}

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

pair<int,int> Motor::get_direction(){

    return make_pair(this->directionL, this->directionR);
}

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

pair<float,float> Motor::get_dutycycle(){

    return make_pair( 1 - left.read() , 1 - right.read() );

}

void Motor::forward(double distance, Motor* motor, Encoder* left, Encoder* right){

    motor->set_enable(0);

    motor->set_dutycycle('A', SLOW_PWM);
    motor->set_direction('A', 1);

    left->reset_counter();
    right->reset_counter();

    while(Encoder::average_distance(*right,*left) < distance) {

        motor->set_enable(1);

        if(left->read_counter() < right->read_counter()){

            auto correction = (double) right->read_counter() / left->read_counter();
            motor->set_dutycycle('L',SLOW_PWM * correction * 1.05);
            motor->set_dutycycle('R',SLOW_PWM);


        }
        else if (right->read_counter() < left->read_counter()){

            auto correction = (double) left->read_counter() / right->read_counter();
            motor->set_dutycycle('R',SLOW_PWM * correction);
            motor->set_dutycycle('L',SLOW_PWM * 1.05);

        }

    }

    motor->set_enable(0);

}

void Motor::reverse(double distance, Motor* motor, Encoder* left, Encoder* right){

    motor->set_enable(0);

    motor->set_dutycycle('A', SLOW_PWM);
    motor->set_direction('A', 0);

    left->reset_counter();
    right->reset_counter();

    while(abs(Encoder::average_distance(*right,*left)) < distance) {

        motor->set_enable(1);

        // Encoder growing negatively
        if(left->read_counter() > right->read_counter()){

            auto correction = (double) right->read_counter() / left->read_counter();
            motor->set_dutycycle('L',SLOW_PWM * correction * 1.05);
            motor->set_dutycycle('R',SLOW_PWM);


        }
        else if (right->read_counter() > left->read_counter()){

            auto correction = (double) left->read_counter() / right->read_counter();
            motor->set_dutycycle('R',SLOW_PWM * correction);
            motor->set_dutycycle('L',SLOW_PWM * 1.05);

        }

    }

    motor->set_enable(0);

}

//  Turn left : Right +ve | LEFT -ve
void Motor::turnleft(double angle, Motor* motor, Encoder* left, Encoder* right){

    motor->set_enable(0);

    motor->set_dutycycle('L', SLOW_PWM);
    motor->set_direction('L', 0);
    motor->set_dutycycle('R', SLOW_PWM);
    motor->set_direction('R', 1);

    left->reset_counter();
    right->reset_counter();
    
    while(right->read_distance() < ( Encoder::FULL_ROTATION_CENTER_PIVOT * ( angle / 360.0 ) ) ) { 
        motor->set_enable(1);
    }
   
    motor->set_enable(0);

}

//  Turn right : Right -ve | LEFT +ve
void Motor::turnright(double angle,Motor* motor,Encoder* left,Encoder* right){

    motor->set_enable(0);
    
    motor->set_dutycycle('L', SLOW_PWM);
    motor->set_direction('L', 1);
    motor->set_dutycycle('R', SLOW_PWM);
    motor->set_direction('R', 0);

    left->reset_counter();
    right->reset_counter();
    
    while(left->read_distance() < ( Encoder::FULL_ROTATION_CENTER_PIVOT * ( angle / 360.0 ) ) ) { 
        motor->set_enable(1);
    }
   
    motor->set_enable(0);

}