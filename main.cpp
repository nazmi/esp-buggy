#include "mbed.h"
#include "encoder.h"
#include "motor.h"
#include "sensor.h"
#include <string>

#define DEBUG 0

int main() {

    Motor motor(PC_9,PB_8,PC_8,PC_6,PB_9);
    Encoder wheel_left(PC_3,PC_2);
    Encoder wheel_right(PB_14,PB_13);
    
    BufferedSerial hm10(PA_11,PA_12,115200);
    Sensor sensors(D2,D3,D4,D5,D6,D8,A5,A4,A3,A2,A1,A0 );
    DigitalOut analog_1(PC_5,0), analog_2(PB_1,0), analog_3(PC_4,0);
    

    while(1){

        char c;

        if (hm10.readable()) {

            hm10.read(&c,1);

            switch(c){

                case 'R':
                {
                    int i = 1000;
                    while(i-- > 0){
                        auto distance = sensors.read();
                        if(distance < NO_TRACK){
                            auto s =  std::to_string(distance);
                            hm10.write(&s,sizeof(s)-1);

                        }else{
                            char s[] = "No Track";
                            hm10.write(&s,sizeof(s));
                        }

                        ThisThread::sleep_for(5ms);
                    }
                    break;
                }
                case 'S':
                {
                    while(1){
                        sensors.read();

                        if(hm10.readable()){
                            hm10.read(&c,1);
                            if(c == 'S')
                                break;

                        }
                        ThisThread::sleep_for(100ms);
                    }
                    break;
                }
                case 'W':
                {
                    int i = 1000;
                    while(i-- > 0)
                        sensors.calibrate_white();
                    break;
                }
                case 'B':
                {
                    int i = 1000;
                    while(i-- > 0)
                        sensors.calibrate_black();
                    break;
                }
                default:
                      printf("HM10 sent %c\n",c);

            }
            
            sensors.run = false;
        }

        ThisThread::sleep_for(10ms); 

        
    }

    return 0;

}
