#include "sensor.h"

bool Sensor::run = false;

Sensor::Sensor(PinName p1, PinName p2, PinName p3, PinName p4, PinName p5, PinName p6, PinName in1,PinName in2,PinName in3,PinName in4,PinName in5,PinName in6) : sensors(p1,p2,p3,p4,p5,p6), input{AnalogIn(in1),AnalogIn(in2),AnalogIn(in3),AnalogIn(in4),AnalogIn(in5),AnalogIn(in6)} {};


float Sensor::read(){

    
    distance = 0;
    Timer t;
    t.start();
    sensors.write(0);
    for(int i=0;i<6;i++){

        float reading = input[i].read();
        
        if(reading > treshold[i])
            noise[i]= reading;
        else
            noise[i] = 0;
    
    }

    for(int i=0; i < 6; i++){

        sensors.write(1 << i);
        wait_us(135);
        float reading = input[i].read();

        if(reading > noise[i] && reading > treshold[i])
            sensor_data[i] = (reading - noise[i]) * PRESCALER * scale_factor[i];
        else if(reading > treshold[i] && reading < noise[i])
             sensor_data[i] = reading * PRESCALER * scale_factor[i];
        else
            sensor_data[i] = 0;
        
    }


    if(std::accumulate(sensor_data,sensor_data+6,0.0) > 0.1*PRESCALER){

        for(int i =0; i < 6 ; i++){

            distance += sensor_data[i]*weight[i];

        }

    }else {

            distance = MAX_DISTANCE;

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
    if(run == false) printf("1,2,3,4,5,6\n"); run=true;
    for(int i=0; i < 6; i++){

        sensors.write(1<<i);
        wait_us(135);
        calibrate_data[i] = input[i].read();

    }

    printf("%.5f,%.5f,%.5f,%.5f,%.5f,%.5f\n",calibrate_data[0],calibrate_data[1],calibrate_data[2],calibrate_data[3],calibrate_data[4],calibrate_data[5]);
    
}

void Sensor::calibrate_white(){


    sensors.write(0);
    for(int i=0;i<6;i++){

        float reading = input[i].read();
        
        if(reading > treshold[i])
            noise[i]= reading;
        else
            noise[i] = 0;
    
    }


    // sensors.write(0b111111);
    // wait_us(135);

    for(int i=0; i < 6; i++){

        sensors.write(1 << i);
        wait_us(135);

        float reading = input[i].read();

        if(reading > noise[i] && reading > treshold[i])
            sensor_data[i] = (reading - noise[i]) * PRESCALER;
        else if(reading > treshold[i] && reading < noise[i])
            sensor_data[i] = reading * PRESCALER;
        else
            sensor_data[i] = 0;

    }

    if(run == false) printf("1,2,3,4,5,6\n"); run=true;
    printf("%.5f,%.5f,%.5f,%.5f,%.5f,%.5f\n",sensor_data[0],sensor_data[1],sensor_data[2],sensor_data[3],sensor_data[4],sensor_data[5]);
    
}