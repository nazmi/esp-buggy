#include "motor.h"

Motor::Motor(PinName left, PinName right,
             PinName direction_left, PinName direction_right, PinName enable)
    : m_direction_left(direction_left, 1), m_direction_right(direction_right, 1),
      m_enable(enable, 0), left_motor(left), right_motor(right) {
    setDutycycle('A', 0);
}

void Motor::setEnable(int enable) {

    m_enable = enable;
}

void Motor::setEnable() {

    m_enable = !m_enable;
}

int Motor::getEnable() {

    return m_enable;
}

void Motor::setFrequency(float frequency) {

    m_period = 1.0f / frequency;
    left_motor.period(m_period);
    right_motor.period(m_period);
}

float Motor::getFrequency() const {
    return 1.0f / m_period;
}

void Motor::setDirection(char c, int direction) {

    if (c == 'L') {
        m_direction_left = direction;
    } else if (c == 'R') {
        m_direction_right = direction;
    } else {
        m_direction_right = m_direction_left = direction;
    }
}

void Motor::setDirection(char c) {

    if (c == 'L') {
        m_direction_left = !m_direction_left;
    } else if (c == 'R') {
        m_direction_right = !m_direction_right;
    } else {
        m_direction_right = !m_direction_right;
        m_direction_left = !m_direction_left;
    }
}

std::pair<int, int> Motor::getDirection() const {

    return make_pair(m_direction_left, m_direction_right);
}

void Motor::setDutycycle(char c, float dutycycle) {

    dutycycle = clamp(1 - dutycycle, 0.0f, 1.0f);

    if (c == 'L') {
        left_motor.write(dutycycle);
    } else if (c == 'R') {
        right_motor.write(dutycycle);
    } else {
        left_motor.write(dutycycle);
        right_motor.write(dutycycle);
    }
}

std::pair<float, float> Motor::getDutycycle() {

    return make_pair(1 - left_motor.read(), 1 - right_motor.read());
}

void Motor::forward(const double distance, Motor *const motor, Encoder *const left_encoder, Encoder *const right_encoder) {

    motor->setEnable(0);

    motor->setDutycycle('A', SLOW_PWM);
    motor->setDirection('A', 1);

    left_encoder->resetCounter();
    right_encoder->resetCounter();

    while (Encoder::getAverageDistance(*right_encoder, *left_encoder) < distance) {

        motor->setEnable(1);

        if (left_encoder->getCounter() < right_encoder->getCounter()) {

            auto correction = static_cast<double>(right_encoder->getCounter()) / left_encoder->getCounter();
            motor->setDutycycle('L', SLOW_PWM * correction * 1.05);
            motor->setDutycycle('R', SLOW_PWM);

        } else if (right_encoder->getCounter() < left_encoder->getCounter()) {

            auto correction = static_cast<double>(left_encoder->getCounter()) / right_encoder->getCounter();
            motor->setDutycycle('R', SLOW_PWM * correction);
            motor->setDutycycle('L', SLOW_PWM * 1.05);
        }
    }

    motor->setEnable(0);
}

void Motor::reverse(const double distance, Motor *const motor, Encoder *const left_encoder, Encoder *const right_encoder) {

    motor->setEnable(0);

    motor->setDutycycle('A', SLOW_PWM);
    motor->setDirection('A', 0);

    left_encoder->resetCounter();
    right_encoder->resetCounter();

    while (abs(Encoder::getAverageDistance(*right_encoder, *left_encoder)) < distance) {

        motor->setEnable(1);

        // Encoder growing negatively
        if (left_encoder->getCounter() > right_encoder->getCounter()) {

            auto correction = static_cast<double>(right_encoder->getCounter()) / left_encoder->getCounter();
            motor->setDutycycle('L', SLOW_PWM * correction * 1.05);
            motor->setDutycycle('R', SLOW_PWM);

        } else if (right_encoder->getCounter() > left_encoder->getCounter()) {

            auto correction = static_cast<double>(left_encoder->getCounter()) / right_encoder->getCounter();
            motor->setDutycycle('R', SLOW_PWM * correction);
            motor->setDutycycle('L', SLOW_PWM * 1.05);
        }
    }

    motor->setEnable(0);
}

//  Turn left : Right +ve | LEFT -ve
void Motor::turnleft(const double angle, Motor *const motor, Encoder *const left_encoder, Encoder *const right_encoder) {

    motor->setEnable(0);

    motor->setDutycycle('L', SLOW_PWM);
    motor->setDirection('L', 0);
    motor->setDutycycle('R', SLOW_PWM);
    motor->setDirection('R', 1);

    left_encoder->resetCounter();
    right_encoder->resetCounter();

    while (right_encoder->getDistance() < (Encoder::FULL_ROTATION_CENTER_PIVOT * (angle / 360.0))) {
        motor->setEnable(1);
    }

    motor->setEnable(0);
}

//  Turn right : Right -ve | LEFT +ve
void Motor::turnright(const double angle, Motor *const motor, Encoder *const left_encoder, Encoder *const right_encoder) {

    motor->setEnable(0);

    motor->setDutycycle('L', SLOW_PWM);
    motor->setDirection('L', 1);
    motor->setDutycycle('R', SLOW_PWM);
    motor->setDirection('R', 0);

    left_encoder->resetCounter();
    right_encoder->resetCounter();

    while (left_encoder->getDistance() < (Encoder::FULL_ROTATION_CENTER_PIVOT * (angle / 360.0))) {
        motor->setEnable(1);
    }

    motor->setEnable(0);
}
