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
    
    motor.set_frequency(1000);
    motor.set_enable(0);

    if(DEBUG)
    {
        motor.set_dutycycle('A',0.4);
        motor.set_direction('A', 1);

        Potentiometer pot_r(A1,&motor.right);
        Potentiometer pot_l(A0,&motor.left);
        Joystick joy(A2,A3,D4,&motor);

        lcd.cls();

        wheel_right.start();
        wheel_left.start();
        motor.set_enable(0);

        while (1) {
        lcd.locate(0, 0);
        lcd.printf("pwm : %.3f %.3f\n",pot_l.read(),pot_r.read());
        lcd.printf("v   : %.3f  %.3f m/s\n", wheel_left.read_velocity(), wheel_right.read_velocity()); 
        lcd.printf("pulses %d %d\n", wheel_left.read_counter(), wheel_right.read_counter());
        //lcd.printf("V   : %.3f  Omega: %.3f \n",Encoder::average_velocity(wheel_right, wheel_left),Encoder::average_angular(wheel_right, wheel_left));
       
        }
    
    }
    else
    {

        // Sequence
        // straight + left turns x4
        // left turn once
        // straight + right turns x4
        ThisThread::sleep_for(5s);
        wheel_left.start();
        wheel_right.start();


        //First Square
        vector<double> linear   {0.625   , 0.6  , 0.6  , 0.6};
        vector<double> rotation {90     , 90   , 90   , 170};
        
        for(int i=0;i<linear.size();i++){

            Motor::forward(linear[i], &motor, &wheel_left, &wheel_right);
            Motor::turnright(rotation[i], &motor, &wheel_left, &wheel_right);

        }


        // Backtracking Square
        vector<double> linear2   {0.6   , 0.5  , 0.5  , 0.67};
        vector<double> rotation2 {55     , 50   , 50   , 120};
        
        for(int i=0;i<linear.size();i++){

            Motor::forward(linear2[i], &motor, &wheel_left, &wheel_right);
            Motor::turnleft(rotation2[i], &motor, &wheel_left, &wheel_right);

        }

        


    }


    return 0;

}
