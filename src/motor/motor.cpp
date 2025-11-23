#include "motor.h"
#include <cmath>
#include <utility>

Motor::Motor(PinName left, PinName right,
             PinName direction_left, PinName direction_right, PinName enable)
    : m_direction_left(direction_left, 1), m_direction_right(direction_right, 1),
      m_enable(enable, 0), m_period(1.0f / Config::Motor::PWM_FREQUENCY),
      left_motor(left), right_motor(right) {
    setDutycycle('A', 0.0f);
    setFrequency(Config::Motor::PWM_FREQUENCY);
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

void Motor::executeLinearMovement(const double distance, Motor *const motor,
                                  Encoder *const left_encoder, Encoder *const right_encoder,
                                  const int direction) {
    motor->setEnable(0);
    motor->setDutycycle('A', Config::Motor::SLOW_DUTY_CYCLE);
    motor->setDirection('A', direction);

    left_encoder->resetCounter();
    right_encoder->resetCounter();

    const bool isForward = (direction == 1);

    while (true) {
        const double avgDistance = std::abs(Encoder::getAverageDistance(*right_encoder, *left_encoder));
        if (avgDistance >= distance) {
            break;
        }

        motor->setEnable(1);

        const int leftCount = left_encoder->getCounter();
        const int rightCount = right_encoder->getCounter();

        // Determine which wheel is faster (considering direction)
        const bool leftNeedsCorrectionForward = isForward && (leftCount < rightCount);
        const bool leftNeedsCorrectionReverse = !isForward && (leftCount > rightCount);
        const bool rightNeedsCorrectionForward = isForward && (rightCount < leftCount);
        const bool rightNeedsCorrectionReverse = !isForward && (rightCount > leftCount);

        if (leftNeedsCorrectionForward || leftNeedsCorrectionReverse) {
            const auto correction = static_cast<double>(rightCount) / (leftCount + 1);
            motor->setDutycycle('L', Config::Motor::SLOW_DUTY_CYCLE * correction * Config::Motor::CORRECTION_FACTOR);
            motor->setDutycycle('R', Config::Motor::SLOW_DUTY_CYCLE);
        } else if (rightNeedsCorrectionForward || rightNeedsCorrectionReverse) {
            const auto correction = static_cast<double>(leftCount) / (rightCount + 1);
            motor->setDutycycle('R', Config::Motor::SLOW_DUTY_CYCLE * correction);
            motor->setDutycycle('L', Config::Motor::SLOW_DUTY_CYCLE * Config::Motor::CORRECTION_FACTOR);
        }
    }

    motor->setEnable(0);
}

void Motor::forward(const double distance, Motor *const motor, Encoder *const left_encoder, Encoder *const right_encoder) {
    executeLinearMovement(distance, motor, left_encoder, right_encoder, 1);
}

void Motor::reverse(const double distance, Motor *const motor, Encoder *const left_encoder, Encoder *const right_encoder) {
    executeLinearMovement(distance, motor, left_encoder, right_encoder, 0);
}

void Motor::executeRotationalMovement(const double angle, Motor *const motor,
                                      Encoder *const left_encoder, Encoder *const right_encoder,
                                      const int turnDirection) {
    motor->setEnable(0);

    motor->setDutycycle('L', Config::Motor::SLOW_DUTY_CYCLE);
    motor->setDutycycle('R', Config::Motor::SLOW_DUTY_CYCLE);

    if (turnDirection == 0) {
        // Turn left: Right forward, Left reverse
        motor->setDirection('L', 0);
        motor->setDirection('R', 1);
    } else {
        // Turn right: Left forward, Right reverse
        motor->setDirection('L', 1);
        motor->setDirection('R', 0);
    }

    left_encoder->resetCounter();
    right_encoder->resetCounter();

    const double targetDistance = Encoder::FULL_ROTATION_CENTER_PIVOT * (angle / 360.0);
    Encoder *const activeEncoder = (turnDirection == 0) ? right_encoder : left_encoder;

    while (activeEncoder->getDistance() < targetDistance) {
        motor->setEnable(1);
    }

    motor->setEnable(0);
}

//  Turn left : Right +ve | LEFT -ve
void Motor::turnleft(const double angle, Motor *const motor, Encoder *const left_encoder, Encoder *const right_encoder) {
    executeRotationalMovement(angle, motor, left_encoder, right_encoder, 0);
}

//  Turn right : Right -ve | LEFT +ve
void Motor::turnright(const double angle, Motor *const motor, Encoder *const left_encoder, Encoder *const right_encoder) {
    executeRotationalMovement(angle, motor, left_encoder, right_encoder, 1);
}
