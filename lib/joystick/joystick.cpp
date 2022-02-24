#include "joystick.h"

// Constructor
// u - pin up
// d - down pin
// motor - motor object passed by pointer
Joystick::Joystick(PinName u, PinName d, PinName f, Motor* m): up(u), down(d), fire(f){
    
    this->motor = m;
    up.rise(callback(this,&Joystick::up_interrupt));
    down.rise(callback(this,&Joystick::down_interrupt));
    fire.rise(callback(this,&Joystick::fire_interrupt));

}

// Functions to return state of button
bool Joystick::up_pressed(){

    return up;

}

bool Joystick::down_pressed(){

    return down;

}

bool Joystick::fire_pressed(){

    return fire;

}

//  Handles the interrupt
// button   direction   motor
// up       flip        left
// down     flip        right

// fire     flip        enable

void Joystick::up_interrupt(){
    motor->set_direction('L');
}

void Joystick::down_interrupt(){
    motor->set_direction('R');
}

void Joystick::fire_interrupt(){
    motor->set_enable();
}
