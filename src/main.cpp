#include "encoder.h"
#include "mbed.h"
#include "motor.h"
#include "sensor.h"
#include <string>

int main() {

    Motor motor(PC_9, PB_8, PC_8, PC_6, PB_9);
    Encoder wheel_left(PC_3, PC_2);
    Encoder wheel_right(PB_14, PB_13);

    BufferedSerial hm10(PA_11, PA_12, 115200);
    Sensor sensors(D2, D3, D4, D5, D6, D8, A5, A4, A3, A2, A1, A0);
    DigitalOut analog_1(PC_5, 0), analog_2(PB_1, 0), analog_3(PC_4, 0);
    char c;

    while (1) {

        if (hm10.readable()) {

            hm10.read(&c, 1);

            switch (c) {

            case 'R': {
                int i = 200;
                while (i-- > 0) {
                    auto distance = sensors.read();
                    if (distance < NO_TRACK) {
                        char s[9];
                        snprintf ( s, 9, "%2.5f", distance );
                        hm10.write(&s, sizeof(s));
                    } else {
                        char s[] = "No Track";
                        hm10.write(&s, sizeof(s));
                    }

                    ThisThread::sleep_for(50ms);
                }
                break;
            }
            case 'S': {
                while (1) {
                    sensors.toggle(true);

                    if (hm10.readable()) {
                        hm10.read(&c, 1);
                        if (c == 'S'){
                            sensors.toggle(false);
                            break;
                        }
                            
                    }
                    ThisThread::sleep_for(100ms);
                }
                break;
            }
            case 'W': {
                int i = 10000;
                while (i-- > 0)
                    sensors.calibrate_white();
                break;
            }
            case 'B': {
                int i = 10000;
                while (i-- > 0)
                    sensors.calibrate_black();
                break;
            }
            default:
                printf("HM10 sent %c\n", c);
            }

            sensors.s_run = false;
        }

        ThisThread::sleep_for(10ms);
    }

    return 0;
}
