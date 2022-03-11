#include "mbed.h"
#include "C12832.h"
#include "encoder.h"
#include "motor.h"
#include "joystick.h"
#include "potentiometer.h"
#include "sensor.h"

#define DEBUG 0

int main() {

    C12832 lcd(D11, D13, D12, D7, D10);
    Motor motor(PC_9,PB_8,PC_8,PC_6,PB_9);
    Encoder wheel_left(PC_3,PC_2);
    Encoder wheel_right(PB_14,PB_13);
    
    BufferedSerial hm10(PA_11,PA_12,115200);
    Sensor sensors(PA_9,PB_10,PB_4,PB_5,PB_3,PA_10,PC_0,PC_1,PB_0,PA_4,PA_1,PA_0 );
    //DigitalOut n1(PC_5,0),n2(PB_1,0),n3(PC_4,0);
    //Potentiometer pot_r(A1,&motor.right);
    //Potentiometer pot_l(A0,&motor.left);
    //Joystick joy(A2,A3,D4,&motor);
    
    motor.set_enable(0);
    motor.set_dutycycle('A', 0);
    motor.set_frequency(1000);
    
    while(1){

        char c;

        if (hm10.readable()) {

            hm10.read(&c,1);

            switch(c){

                case 'R':
                {
                    int i = 1000;
                    while(i-- > 0)
                        sensors.read();
                    sensors.run = false;
                    break;
                }
                case 'W':
                {
                    int i = 1000;
                    while(i-- > 0)
                        sensors.calibrate_white();
                    sensors.run = false;
                    break;
                }
                case 'B':
                {
                    int i = 1000;
                    while(i-- > 0)
                        sensors.calibrate_black();
                    sensors.run = false;
                    break;
                }
                default:
                      printf("HM10 sent %c\n",c);

            }
            
            
        } 
    
    }
    
    return 0;

}
