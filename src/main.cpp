#include "encoder.h"
#include "mbed.h"
#include "motor.h"
#include "sensor.h"
#include "wheelcontrol.h"
#include <string>

int main() {

    Motor motor(PB_8, PC_9, PC_6, PC_8, PB_9);
    Encoder wheel_left(PC_3, PC_2);
    Encoder wheel_right(PB_14, PB_13);
    wheel_left.start();
    wheel_right.start();

    BufferedSerial hm10(PA_11, PA_12, 115200);
    Sensor sensors(D2, D3, D4, D5, D6, D8, A5, A4, A3, A2, A1, A0);
    DigitalOut analog_1(PC_5, 0), analog_2(PB_1, 0), analog_3(PC_4, 0);

    WheelControl controller;
    controller.setSpeedController(0.75f, 0.0f, 0.0004f);
    controller.setLineController(1.2f, 0.0f, 0.0f);
    controller.setLineLimits(-27.0f, 27.0f);
    controller.setPWMLimits(0.0f, 1.0f);
    controller.setTargetSpeed(1.5f);

    char c;
    Timer test;

    while (1) {

        if (hm10.readable()) {

            hm10.read(&c, 1);

            switch (c) {

            case 'R': {
                while (1) {
                    auto distance = sensors.read();
                    if (distance < NO_TRACK) {
                        char s[9];
                        snprintf(s, sizeof(s), "%2.5f", distance);
                        hm10.write(&s, sizeof(s));
                    } else {
                        char s[] = "No Track";
                        hm10.write(&s, sizeof(s));
                    }

                    if (hm10.readable()) {
                        hm10.read(&c, 1);
                        if (c == 'R') {
                            break;
                        }
                    }

                    ThisThread::sleep_for(50ms);
                }
                break;
            }
            case 'S': {

                motor.set_enable(1);
                motor.set_frequency(1000);
                while (1) {
                    auto distance = sensors.read();
                    printf("Position: %.5f\n", distance);
                    if (distance != NO_TRACK) {
                        auto compute_value = controller.computeSpeed(distance, wheel_left, wheel_right);
                        motor.set_direction('L', compute_value[0].first);
                        motor.set_dutycycle('L', compute_value[0].second);
                        motor.set_direction('R', compute_value[1].first);
                        motor.set_dutycycle('R', compute_value[1].second);
                        printf("Motor output: [%d,%.5f] [%d,%.5f]\n",
                               compute_value[0].first, compute_value[0].second,
                               compute_value[1].first, compute_value[1].second);
                        // printf("Counter : %d\n", sensors.getNoTrackCounter());
                    } else {
                        // printf("Counter : %d\n", sensors.getNoTrackCounter());
                        if (sensors.getNoTrackCounter() > 150) {
                            motor.set_dutycycle('A', 0);
                            break;
                        }
                    }

                    if (hm10.readable()) {
                        hm10.read(&c, 1);
                        if (c == 'S') {
                            motor.set_enable(0);
                            motor.set_dutycycle('A', 0);
                            break;
                        } else if (c == 'Q') {
                            motor.set_enable(0);
                            ThisThread::sleep_for(1s);
                            motor.turnright(172, &motor, &wheel_left, &wheel_right);
                            controller.reset();
                            motor.set_dutycycle('A', 0);
                            motor.set_enable(1);
                        }
                    }
                    // ThisThread::sleep_for(1ms);
                }
                break;
            }
            case 'T': {

                motor.set_enable(0);
                float kp, ki, kd;
                while (1) {
                    hm10.read(&kp, sizeof(kp));
                    hm10.read(&ki, sizeof(ki));
                    hm10.read(&kd, sizeof(kd));

                    printf("Kp: %.5f Ki: %.5f Kd: %.5f\n", kp, ki, kd);
                    controller.setSpeedController(kp, ki, kd);
                    break;
                }

                break;
            }
            case 'V': {

                motor.set_enable(0);
                float speed;
                while (1) {
                    hm10.read(&speed, sizeof(speed));

                    printf("Target speed: %.5f\n", speed);
                    controller.setTargetSpeed(speed);
                    break;
                }

                break;
            }
            case 'M': {
                motor.set_enable(1);
                motor.set_frequency(1000);
                motor.set_direction('R', 0);
                motor.set_dutycycle('R', 1);
                motor.set_direction('L', 1);
                motor.set_dutycycle('L', 1);

                while (1) {
                    printf("%.5f %.5f\n", wheel_left.read_pps(), wheel_right.read_pps());
                    ThisThread::sleep_for(400ms);
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
                if (isalnum(c)) {
                    printf("HM10 sent %c\n", c);
                }
            }

            sensors.s_run = false;
        }
    }

    return 0;
}
