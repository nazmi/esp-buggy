#include "debug1.h"
#include "encoder.h"
#include "helper.h"
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
    controller.setSpeedController(0.70f, 0.0f, 0.0f);
    controller.setLineController(1.2f, 0.0f, 0.0f);
    controller.setLineLimits(-27.0f, 27.0f);
    controller.setPWMLimits(0.0f, 1.0f);
    controller.setTargetSpeed(2.0f);

    char c;

    while (1) {

        if (hm10.readable()) {

            hm10.read(&c, 1);

            switch (c) {

            case 'R': {
                while (1) {
                    auto distance = sensors.read();
                    if (distance != NO_TRACK) {
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

                motor.setEnable(1);
                motor.setFrequency(1000);
                while (1) {
                    auto distance = sensors.read();
                    DEBUGLOG("Position: %.5f\n", distance);
                    if (distance != NO_TRACK) {
                        auto compute_value = controller.computeSpeed(distance, wheel_left, wheel_right);
                        motor.setDirection('L', compute_value[0].first);
                        motor.setDutycycle('L', compute_value[0].second);
                        motor.setDirection('R', compute_value[1].first);
                        motor.setDutycycle('R', compute_value[1].second);
                        DEBUGLOG("Motor output: [%d,%.5f] [%d,%.5f]\n",
                                 compute_value[0].first, compute_value[0].second,
                                 compute_value[1].first, compute_value[1].second);
                    } else {
                        DEBUGLOG("Counter : %d\n", sensors.getNoTrackCounter());
                        if (sensors.getNoTrackCounter() > 150) {
                            motor.setDutycycle('A', 0);
                            break;
                        }
                    }

                    if (hm10.readable()) {
                        hm10.read(&c, 1);
                        if (c == 'S') {
                            motor.setEnable(0);
                            motor.setDutycycle('A', 0);
                            break;
                        } else if (c == 'Q') {
                            motor.setEnable(0);
                            ThisThread::sleep_for(1s);
                            motor.turnright(45, &motor, &wheel_left, &wheel_right);

                            while (1) {
                                auto _distance = sensors.read();
                                if (_distance < 2.0f)
                                    break;

                                motor.turnright(5, &motor, &wheel_left, &wheel_right);
                            }

                            controller.reset();
                            motor.setDutycycle('A', 0);
                            motor.setEnable(1);
                        }
                    }
                    // ThisThread::sleep_for(1ms);
                }
                break;
            }
            case 'T': {

                motor.setEnable(0);
                float kp, ki, kd;
                while (1) {
                    hm10.read(&kp, sizeof(kp));
                    hm10.read(&ki, sizeof(ki));
                    hm10.read(&kd, sizeof(kd));

                    DEBUGLOG("Kp: %.5f Ki: %.5f Kd: %.5f\n", kp, ki, kd);
                    controller.setSpeedController(kp, ki, kd);
                    break;
                }

                break;
            }
            case 'V': {

                motor.setEnable(0);
                float speed;
                while (1) {
                    hm10.read(&speed, sizeof(speed));

                    DEBUGLOG("Target speed: %.5f\n", speed);
                    controller.setTargetSpeed(speed);
                    break;
                }

                break;
            }
            case 'M': {
                motor.setEnable(1);
                motor.setFrequency(1000);
                motor.setDirection('R', 0);
                motor.setDutycycle('R', 1);
                motor.setDirection('L', 1);
                motor.setDutycycle('L', 1);

                while (1) {
                    DEBUGLOG("%.5f %.5f\n", wheel_left.getPPS(), wheel_right.getPPS());
                    ThisThread::sleep_for(400ms);
                }
                break;
            }
            case 'W': {
                int i = 10000;
                while (i-- > 0)
                    sensors.calibrateWhite();
                break;
            }
            case 'B': {
                int i = 10000;
                while (i-- > 0)
                    sensors.calibrateBlack();
                break;
            }
            default:
                if (std::isalnum(c)) {
                    printf("HM10 sent %c\n", c);
                }
            }

            sensors.s_run = false;
        }
    }

    return 0;
}
