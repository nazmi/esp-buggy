#include "sensor.h"

Sensor::Sensor(PinName p1, PinName p2, PinName p3, PinName p4, PinName p5, PinName p6, PinName in) : sensors(p6,p5,p4,p3,p2,p1), input(in) {

    distance = 0;
    for(int i =0; i < 6; i++)
        sensor_data[i] = 0;

};


float Sensor::read(){

    for(int i=0; i < 6; i++){

        sensors = 1 << i;
        sensor_data[i] = input.read();

        if( i == 3) printf("\n");
        printf("%.9f ", sensor_data[i]);
    }

    distance = 0;

    for(int i =0; i < 6 ; i++){

        distance += sensor_data[i]*weight[i];

    }
    
    printf("%.9f",distance);
    return distance;
    
}


void Sensor::calibrate(){

    Timer t;
    t.start();

    constexpr int points = 1000;
    float sensor_cal_data[6][points] {0};

    for(int n = 0; t.elapsed_time() < 3000ms; n++ ){

        for(int i=0; i < 6; i++){

        sensors = 1 << i;
        sensor_cal_data[i][n] = input.read();

        }
    }

    for(int i=0; i < 6; i++){

        auto p = std::minmax_element(sensor_cal_data[i],sensor_cal_data[i]+1000);

        lower_limit[i] = *p.first;
        upper_limit[i] = *p.second;
    
        printf("Sensor %d : min %.3f max %.3f\n", i, lower_limit[i], upper_limit[i] );

    }

    t.stop();
    t.reset();

}
