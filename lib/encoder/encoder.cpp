#include "encoder.h"
using namespace std::chrono;

// Constructor
// p1 Channel A
// p2 Channel B
// Resets counter and velocity to zero
Encoder::Encoder(PinName p1, PinName p2): QEI(p1,p2,NC,CPR){
    pulse_width = 0;
    pulse_counter = 0;
    velocity = 0;
}

// Start the timer interrupt every 10 ms
void Encoder::start(){
    t.start();
    start_pulse = Encoder::getPulses();
    to.attach(callback(this,&Encoder::stop),10ms);

}


void Encoder::stop(){
    t.stop();
    end_pulse = Encoder::getPulses();
    time_ms = t.elapsed_time();
    calculate_speed();
}

// Calculate the speed of wheel
// v = pulse * CPR * Circumference
// TODO : check formula
void Encoder::calculate_speed(){

    pulse_width = end_pulse-start_pulse;
    pulse_counter += pulse_width;
    auto time_sec = 0.001 * time_ms.count();
    velocity = (pulse_width/(2.0*CPR*time_sec))*C;

    reset();
    start();
}

// Reset pulse counter to zero
void Encoder::reset_counter(){
    pulse_counter = 0;
}

// Return the speed
double Encoder::read_velocity(){
    return velocity;
}

// Return distance covered by a wheel
double Encoder::read_distance(){
    return (pulse_counter/(2.0*CPR))*C;
}

// Return pulse counter
int Encoder::read_counter(){
    return pulse_counter;
}

// Return the angular velocity between left and right wheel
// +ve indicate rotation to the right
// Access using Scope resolution Encoder::average_angular(right,left)
double Encoder::average_angular(Encoder &right,Encoder &left){
    return (right.read_velocity()-left.read_velocity())/WHEEL_DISTANCE;
}

// Return the average velocity of left and right wheel
// +ve indicate forward direction
// Access using Scope resolution Encoder::average_velocity(right,left)
double Encoder::average_velocity(Encoder &right,Encoder &left){
    return 0.5*(right.read_velocity()+left.read_velocity());
}

// Return the average distance covered
double Encoder::average_distance(Encoder &right,Encoder &left){
    return 0.5*(right.read_distance()+left.read_distance());
}





