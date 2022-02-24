#ifndef MOTOR_H
#define MOTOR_H

#include "mbed.h"
#include "encoder.h"


#define UNIPOLAR 0
#define BIPOLAR 1
#define FAST_PWM 0.7
#define HALF_PWM 0.5
#define SLOW_PWM 0.3


class Motor{

    private:
    DigitalOut state;
    DigitalOut directionL;
    DigitalOut directionR;
    float period;
    static const int mode = UNIPOLAR; // useless because sink to ground is enough

    public:
    PwmOut left;
    PwmOut right;
    
    Motor(PinName l, PinName r, PinName dL, PinName dR, PinName s);
    
    // Setters and getters
    void set_direction(char c, int direction);
    void set_direction(char c);
    pair<int,int> get_direction();

    void set_frequency(float freq);
    float get_frequency();

    void set_enable(int state);
    void set_enable();
    int get_enable();

    void set_dutycycle(char c, float dutycycle);
    pair<float,float> get_dutycycle();


    // Cruising methods
    static void forward(double distance , Motor* motor, Encoder* left,  Encoder* right);
    static void reverse(double distance , Motor* motor, Encoder* left,  Encoder* right);
    static void turnleft(double angle   , Motor* motor, Encoder* left,  Encoder* right);
    static void turnright(double angle  , Motor* motor, Encoder* left,  Encoder* right);
    

};


#endif