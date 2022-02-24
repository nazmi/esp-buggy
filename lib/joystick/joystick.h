#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "mbed.h"
#include "motor.h"
#include "DebounceIn.h"

class Joystick {
    private:
        DebounceIn up, down, fire;
        Motor* motor;

    public:
        Joystick(PinName u, PinName d, PinName f, Motor* m);
        
        bool up_pressed();
        bool down_pressed();
        bool fire_pressed();

        void up_interrupt();
        void down_interrupt();
        void fire_interrupt();
        
};


#endif