#include "sensor.h"
#include <algorithm>

bool Sensor::run = false;
float Sensor::weights[6] =  {27,9,-9,-27,9,-9};

Sensor::Sensor(PinName p1, PinName p2, PinName p3, PinName p4, PinName p5, PinName p6, PinName in1,PinName in2,PinName in3,PinName in4,PinName in5,PinName in6) : sensors(p1,p2,p3,p4,p5,p6), input{AnalogIn(in1),AnalogIn(in2),AnalogIn(in3),AnalogIn(in4),AnalogIn(in5),AnalogIn(in6)} {};


template<class T>
constexpr const T& clamp( const T& v, const T& lo, const T& hi)
{
    return std::less<T>{}(v, lo) ? lo : std::less<T>{}(hi, v) ? hi : v;
}




float Sensor::read(){

    distance = 0;
    Timer t;
    t.start();

    // All off to capture noise
    sensors.write(0);
    wait_us(25);
    for(int i=0;i<6;i++){

        noise[i] = input[i].read();

    }

    // Turn led on one by one
    for(int i=0; i < 6; i++){

        sensors.write(1 << i);
        wait_us(25);
        float reading = input[i].read();

        if(reading > noise[i] && reading > treshold[i])
            sensor_data[i] = (reading - noise[i]) * PRESCALER * scale_factor[i];
        else
            sensor_data[i] = 0;

        sensor_data[i] = clamp(sensor_data[i],0.0f,PRESCALER);
        
    }

    // Check if any of 6 sensors reading > TRESHOLD for white
    if(std::any_of(sensor_data,sensor_data+6,[](float i){ return i > TRESHOLD; })){

        
        float distance_4 = arm_weighted_sum_f32(Sensor::weights,sensor_data,4);
        distance = distance_4;

        //Check if second row is contributing to any distance reading
        if(sensor_data[4] > TRESHOLD || sensor_data[5] > TRESHOLD ){
            
            float distance_2 = arm_weighted_sum_f32(weights+4,sensor_data+4,2);
            float coeff = 0.5;
            distance = coeff * distance_4 + (1-coeff) * distance_2;

        }        
      

    }else {

        distance = NO_TRACK;

    }

    
    
    t.stop();
    
    
    if(run == false) printf("1,2,3,4,5,6,time,distance\n"); run=true;
    printf("%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,",sensor_data[0],sensor_data[1],sensor_data[2],sensor_data[3],sensor_data[4],sensor_data[5]);
    printf("%lld,", t.elapsed_time().count()*1);
    printf("%f\n",distance);
    
    t.reset();

    return distance;
    
}


void Sensor::calibrate_black(){

    float calibrate_data[6] {0};
    
    for(int i=0; i < 6; i++){

        sensors.write(1<<i);
        wait_us(25);
        calibrate_data[i] = input[i].read();

    }

    if(run == false) printf("1,2,3,4,5,6\n"); run=true;
    printf("%.5f,%.5f,%.5f,%.5f,%.5f,%.5f\n",calibrate_data[0],calibrate_data[1],calibrate_data[2],calibrate_data[3],calibrate_data[4],calibrate_data[5]);
    
}

void Sensor::calibrate_white(){

    // Turn all off to read noise
    sensors.write(0);
    wait_us(25);
    for(int i=0;i<6;i++){

        noise[i]= input[i].read();
    
    }

    for(int i=0; i < 6; i++){

        sensors.write(1 << i);
        wait_us(25);

        float reading = input[i].read();

        if(reading > noise[i] && reading > treshold[i])
            sensor_data[i] = (reading - noise[i]) * PRESCALER;
        else
            sensor_data[i] = 0;

    }

    if(run == false) printf("1,2,3,4,5,6\n"); run=true;
    printf("%.5f,%.5f,%.5f,%.5f,%.5f,%.5f\n",sensor_data[0],sensor_data[1],sensor_data[2],sensor_data[3],sensor_data[4],sensor_data[5]);
    
}