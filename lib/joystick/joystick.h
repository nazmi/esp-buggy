#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "mbed.h"
#include "motor.h"

class Joystick {
    private:
        InterruptIn up,down,left,right,fire;
        Motor* motor;

    public:
        Joystick(PinName u, PinName d, PinName l, PinName r, PinName f,Motor* m);
        
        bool up_pressed();
        bool down_pressed();
        bool left_pressed();
        bool right_pressed();
        bool fire_pressed();
        void up_interrupt();
        void down_interrupt();
        void left_interrupt();
        void right_interrupt();
        void fire_interrupt();



    
    
};


#endif