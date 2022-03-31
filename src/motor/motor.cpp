#include "motor.h"

Motor::Motor(PinName left, PinName right,
             PinName direction_left, PinName direction_right, PinName enable)
    : m_direction_left(direction_left, 1), m_direction_right(direction_right, 1),
      m_enable(enable, 0), left_motor(left), right_motor(right) {
    set_dutycycle('A', 0);
}

void Motor::set_enable(int enable) {

    m_enable = enable;
}

void Motor::set_enable() {

    m_enable = !m_enable;
}

int Motor::get_enable() {

    return m_enable;
}

void Motor::set_frequency(float frequency) {

    m_period = 1.0f / frequency;
    left_motor.period(m_period);
    right_motor.period(m_period);
}

float Motor::get_frequency() const {
    return 1.0f / m_period;
}

void Motor::set_direction(char c, int direction) {

    if (c == 'L') {
        m_direction_left = direction;
    } else if (c == 'R') {
        m_direction_right = direction;
    } else {
        m_direction_right = m_direction_left = direction;
    }
}

void Motor::set_direction(char c) {

    if (c == 'L') {
        m_direction_left = !m_direction_left;
    } else if (c == 'R') {
        m_direction_right = !m_direction_right;
    } else {
        m_direction_right = !m_direction_right;
        m_direction_left = !m_direction_left;
    }
}

std::pair<int, int> Motor::get_direction() const {

    return make_pair(m_direction_left, m_direction_right);
}

void Motor::set_dutycycle(char c, float dutycycle) {

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

std::pair<float, float> Motor::get_dutycycle() {

    return make_pair(1 - left_motor.read(), 1 - right_motor.read());
}

void Motor::forward(const double distance, Motor *const motor, Encoder *const left_encoder, Encoder *const right_encoder) {

    motor->set_enable(0);

    motor->set_dutycycle('A', SLOW_PWM);
    motor->set_direction('A', 1);

    left_encoder->reset_counter();
    right_encoder->reset_counter();

    while (Encoder::average_distance(*right_encoder, *left_encoder) < distance) {

        motor->set_enable(1);

        if (left_encoder->read_counter() < right_encoder->read_counter()) {

            auto correction = static_cast<double>(right_encoder->read_counter()) / left_encoder->read_counter();
            motor->set_dutycycle('L', SLOW_PWM * correction * 1.05);
            motor->set_dutycycle('R', SLOW_PWM);

        } else if (right_encoder->read_counter() < left_encoder->read_counter()) {

            auto correction = static_cast<double>(left_encoder->read_counter()) / right_encoder->read_counter();
            motor->set_dutycycle('R', SLOW_PWM * correction);
            motor->set_dutycycle('L', SLOW_PWM * 1.05);
        }
    }

    motor->set_enable(0);
}

void Motor::reverse(const double distance, Motor *const motor, Encoder *const left_encoder, Encoder *const right_encoder) {

    motor->set_enable(0);

    motor->set_dutycycle('A', SLOW_PWM);
    motor->set_direction('A', 0);

    left_encoder->reset_counter();
    right_encoder->reset_counter();

    while (abs(Encoder::average_distance(*right_encoder, *left_encoder)) < distance) {

        motor->set_enable(1);

        // Encoder growing negatively
        if (left_encoder->read_counter() > right_encoder->read_counter()) {

            auto correction = static_cast<double>(right_encoder->read_counter()) / left_encoder->read_counter();
            motor->set_dutycycle('L', SLOW_PWM * correction * 1.05);
            motor->set_dutycycle('R', SLOW_PWM);

        } else if (right_encoder->read_counter() > left_encoder->read_counter()) {

            auto correction = static_cast<double>(left_encoder->read_counter()) / right_encoder->read_counter();
            motor->set_dutycycle('R', SLOW_PWM * correction);
            motor->set_dutycycle('L', SLOW_PWM * 1.05);
        }
    }

    motor->set_enable(0);
}

//  Turn left : Right +ve | LEFT -ve
void Motor::turnleft(const double angle, Motor *const motor, Encoder *const left_encoder, Encoder *const right_encoder) {

    motor->set_enable(0);

    motor->set_dutycycle('L', SLOW_PWM);
    motor->set_direction('L', 0);
    motor->set_dutycycle('R', SLOW_PWM);
    motor->set_direction('R', 1);

    left_encoder->reset_counter();
    right_encoder->reset_counter();

    while (right_encoder->read_distance() < (Encoder::FULL_ROTATION_CENTER_PIVOT * (angle / 360.0))) {
        motor->set_enable(1);
    }

    motor->set_enable(0);
}

//  Turn right : Right -ve | LEFT +ve
void Motor::turnright(const double angle, Motor *const motor, Encoder *const left_encoder, Encoder *const right_encoder) {

    motor->set_enable(0);

    motor->set_dutycycle('L', SLOW_PWM);
    motor->set_direction('L', 1);
    motor->set_dutycycle('R', SLOW_PWM);
    motor->set_direction('R', 0);

    left_encoder->reset_counter();
    right_encoder->reset_counter();

    while (left_encoder->read_distance() < (Encoder::FULL_ROTATION_CENTER_PIVOT * (angle / 360.0))) {
        motor->set_enable(1);
    }

    motor->set_enable(0);
}
