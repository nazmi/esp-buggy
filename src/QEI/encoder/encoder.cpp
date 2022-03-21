#include "encoder.h"
using namespace std::chrono;

Encoder::Encoder(PinName p1, PinName p2): QEI(p1 ,p2, NC, COUNTS_PER_REV){
    pulse_width = 0;
    pulse_counter = 0;
    velocity = 0;
}

void Encoder::start(){
    t.reset();
    t.start();
    reset();
    start_pulse = Encoder::getPulses();
    to.attach(callback(this,&Encoder::stop),POLLING_PERIOD);
    
}

void Encoder::stop(){
    t.stop();
    end_pulse = Encoder::getPulses();
    double time_sec = 0.000001 * t.elapsed_time().count();
    calculate_speed(time_sec);
}

void Encoder::calculate_speed(double time_sec){

    pulse_width = start_pulse - end_pulse;
    pulse_counter += pulse_width;
    pulse_per_sec = pulse_width / (2.0 * time_sec);
    auto rev_per_sec = pulse_per_sec / COUNTS_PER_REV;
    velocity = rev_per_sec * CIRCUMFERENCE;
    start();
}

int Encoder::read_counter(){
    return pulse_counter;
}

void Encoder::reset_counter(){
    pulse_counter = 0;
}

double Encoder::read_velocity(){
    return velocity;
}

double Encoder::read_distance(){
    return (pulse_counter * CIRCUMFERENCE) / (2.0 * COUNTS_PER_REV);
}

double Encoder::read_pps(){
    return pulse_per_sec;
}

double Encoder::average_angular(Encoder &right,Encoder &left){
    return radiansToDegrees( ( right.read_velocity() - left.read_velocity() ) / WHEEL_DISTANCE );
}

double Encoder::average_velocity(Encoder &right,Encoder &left){
    return 0.5*( right.read_velocity() + left.read_velocity() );
}

double Encoder::average_distance(Encoder &right,Encoder &left){
    return 0.5*( right.read_distance() + left.read_distance() );
}





