#include "mbed.h"
#include "C12832.h"
#include "encoder.h"
#include "motor.h"
#include "joystick.h"
#include "potentiometer.h"

#define DEBUG 0

int main() {

    C12832 lcd(D11, D13, D12, D7, D10);
    Motor motor(PC_9,PC_8,PB_8,PB_9,PB_4);
    Encoder wheel_left(PB_2,PB_15);
    Encoder wheel_right(PB_14,PB_13);

    // Zuhayr Setup
    // Motor motor(PA_15,PB_7,PA_14,PC_13,PC_5);
    // Encoder wheel_left(PC_8,PC_6);
    // Encoder wheel_right(PA_12,PA_11);
    // DigitalOut mode(PA_13,0),mode2(PC_14,0);

    motor.set_frequency(1000);
    motor.set_enable(0);

    if(DEBUG)
    {
        motor.set_dutycycle('A', 0.2);
        motor.set_direction('A', 1);

        Potentiometer pot_r(A1,&motor.right);
        Potentiometer pot_l(A0,&motor.left);
        Joystick joy(A2,A3,D4,&motor);

        lcd.cls();

        wheel_right.start();
        wheel_left.start();
        motor.set_enable(1);

        // TODO : Calibrate distance to 1 metre
        while (1) {
        
        lcd.locate(0, 0);
        lcd.printf("pwm : %.3f %.3f\n",pot_l.read(),pot_r.read());
        lcd.printf("v   : %.3f  %.3f m/s\n", wheel_left.read_velocity(), wheel_right.read_velocity()); 
        lcd.printf("pulses %d %d\n", wheel_left.read_counter(), wheel_right.read_counter());
        lcd.printf("V   : %.3f  Omega: %.3f \n",Encoder::average_velocity(wheel_right, wheel_left),Encoder::average_angular(wheel_right, wheel_left));
       
        }

        motor.set_enable(0);
    
    }
    else
    {

        // Sequence
        // straight + left turns x4
        // left turn once(rotate 90)
        // straight + right turns x4
        ThisThread::sleep_for(2s);
        wheel_left.start();
        wheel_right.start();
        
        for(int i=0;i<4;i++){
            
            Motor::forward(0.5, &motor, &wheel_left, &wheel_right);
            Motor::turnleft(90, &motor, &wheel_left, &wheel_right);

        }

        Motor::turnleft(90, &motor, &wheel_left, &wheel_right);

        for(int i=0;i<4;i++){

            Motor::forward(0.5, &motor, &wheel_left, &wheel_right);
            Motor::turnright(90, &motor, &wheel_left, &wheel_right);

        }


    }


    return 0;

}
