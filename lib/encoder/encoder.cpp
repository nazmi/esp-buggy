#include "encoder.h"
using namespace std::chrono;

Encoder::Encoder(PinName p1, PinName p2): QEI(p1,p2,NC,CPR){
    pulse_width = 0;
    pulse_counter = 0;
    velocity = 0;
}

void Encoder::start(){
    t.start();
    start_pulse = Encoder::getPulses();
    to.attach(callback(this,&Encoder::stop),10ms);

}

void Encoder::stop(){
    t.stop();
    time_ms = t.elapsed_time();
    end_pulse = Encoder::getPulses();
    calculate_speed();
}

void Encoder::calculate_speed(){

    pulse_width = end_pulse-start_pulse;
    pulse_counter += pulse_width;
    auto time_sec = 0.001 * time_ms.count();
    velocity = (pulse_width/(2.0*CPR*time_sec))*C;

    reset();
    start();
}

void Encoder::reset_counter(){
    pulse_counter = 0;
}

double Encoder::read_speed(){
    return velocity;
}

double Encoder::read_distance(){
    return (pulse_counter/(2.0*CPR))*C;
}

int Encoder::read_counter(){
    return pulse_counter;
}





