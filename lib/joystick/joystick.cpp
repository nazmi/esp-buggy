#include "joystick.h"

// Constructor
// u - pin up
// d - down pin
// l - left pin
// r - right pin
// f - fire pin
// motor - motor object passed by pointer
Joystick::Joystick(PinName u, PinName d, PinName l, PinName r, PinName f,Motor* motor): up(u), down(d), left(l), right(r), fire(f) {
    
    this->motor = motor;
    up.rise(callback(this,&Joystick::up_interrupt));
    down.rise(callback(this,&Joystick::down_interrupt));
    left.rise(callback(this,&Joystick::left_interrupt));
    right.rise(callback(this,&Joystick::right_interrupt));
    fire.rise(callback(this,&Joystick::fire_interrupt));

}

// Functions to return state of button
bool Joystick::up_pressed(){

    return up;

}

bool Joystick::down_pressed(){

    return down;

}

bool Joystick::left_pressed(){

    return left;

}
bool Joystick::right_pressed(){

    return right;

}

bool Joystick::fire_pressed(){

    return fire;

}

//  Handles the interrupt
// button   direction   motor
// up       forward     left
// down     reverse     left
// left     forward     right
// right    reverse     right

// fire     enable      flip

void Joystick::up_interrupt(){
    motor->set_direction('L',1);
}

void Joystick::down_interrupt(){
    motor->set_direction('L',0);
}

void Joystick::left_interrupt(){
    motor->set_direction('R',1);
}

void Joystick::right_interrupt(){
    motor->set_direction('R',0);
}

void Joystick::fire_interrupt(){
    motor->set_enable();
}



