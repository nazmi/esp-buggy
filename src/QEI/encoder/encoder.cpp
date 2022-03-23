#include "encoder.h"
#include "helper.h"

Encoder::Encoder(PinName A, PinName B) : QEI(A, B, NC, COUNTS_PER_REV) {
    m_pulse_width = 0;
    m_pulse_counter = 0;
    m_velocity = 0;
}

void Encoder::start() {
    m_t.reset();
    m_t.start();
    reset();
    m_start_pulse = Encoder::getPulses();
    m_to.attach(callback(this, &Encoder::stop), POLLING_PERIOD);
}

void Encoder::stop() {
    m_t.stop();
    m_to.detach();
    m_end_pulse = Encoder::getPulses();
    const double time_elapsed = 0.000001 * m_t.elapsed_time().count();
    calculate_speed(time_elapsed);
}

void Encoder::calculate_speed(const double &time_elapsed) {

    m_pulse_width = m_start_pulse - m_end_pulse;
    m_pulse_counter += m_pulse_width;
    m_pulse_per_sec = m_pulse_width / (2.0 * time_elapsed);
    auto rev_per_sec = m_pulse_per_sec / COUNTS_PER_REV;
    m_velocity = rev_per_sec * CIRCUMFERENCE;
    start();
}

int Encoder::read_counter() const {
    return m_pulse_counter;
}

void Encoder::reset_counter() {
    m_pulse_counter = 0;
}

double Encoder::read_velocity() const {
    return m_velocity;
}

double Encoder::read_distance() const {
    return (m_pulse_counter * CIRCUMFERENCE) / (2.0 * COUNTS_PER_REV);
}

double Encoder::read_pps() const {
    return m_pulse_per_sec;
}

double Encoder::average_angular(const Encoder &right_encoder, const Encoder &left_encoder) {
    return radiansToDegrees((right_encoder.read_velocity() - left_encoder.read_velocity()) / WHEEL_DISTANCE);
}

double Encoder::average_velocity(const Encoder &right_encoder, const Encoder &left_encoder) {
    return 0.5 * (right_encoder.read_velocity() + left_encoder.read_velocity());
}

double Encoder::average_distance(const Encoder &right_encoder, const Encoder &left_encoder) {
    return 0.5 * (right_encoder.read_distance() + left_encoder.read_distance());
}
