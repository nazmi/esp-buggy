#include "mbed.h"
#include "C12832.h"
#include "encoder.h"
#include "motor.h"


int main()
{
 

    C12832 lcd(D11, D13, D12, D7, D10);
    Encoder wheel_l(D6,D5);
    Encoder wheel_r(D8,D7);
    Motor motor(PC_8,PC_6,D13,D12,1);
    
    motor.set_direction('A', 1);
    motor.set_frequency(25000);
    motor.set_dutycycle('A', 0.5);
    
 
    while(1){

        lcd.printf("Left Wheel Speed : %f",wheel_l.read_speed());
        lcd.printf("Right Wheel Speed : %f",wheel_r.read_speed());

    

    }

    return 0;

}