#include "mbed.h"
#include "C12832.h"
#include "encoder.h"
#include "motor.h"
#include "joystick.h"
#include "potentiometer.h"
#include <chrono>
#include <vector>

#define DEBUG 0

int main() {

    C12832 lcd(D11, D13, D12, D7, D10);
    Motor motor(PC_9,PB_8,PC_8,PC_6,PB_9);
    Encoder wheel_left(PC_3,PC_2);
    Encoder wheel_right(PB_14,PB_13);
    
    BufferedSerial hm10(PA_11,PA_12);
    //Potentiometer pot_r(A1,&motor.right);
    //Potentiometer pot_l(A0,&motor.left);
    //Joystick joy(A2,A3,D4,&motor);
    
    motor.set_enable(0);
    motor.set_dutycycle('A', 0);
    motor.set_frequency(1000);
    
    while(1){

        if(hm10.readable()){
            
            char c = ' ';
            hm10.read(&c,1);

            if(c != ' '){

                lcd.cls();
                lcd.locate(0, 0);
                lcd.printf("HM10 sent %c \n",c);

            }

        }

    }

    return 0;

}
