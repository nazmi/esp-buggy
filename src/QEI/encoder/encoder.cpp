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
    m_start_pulse = getPulses();
    m_to.attach(callback(this, &Encoder::m_stop), POLLING_PERIOD);
}

void Encoder::m_stop() {
    m_t.stop();
    m_to.detach();
    m_end_pulse = getPulses();
    const double time_elapsed = 0.000001 * m_t.elapsed_time().count();
    m_calculateSpeed(time_elapsed);
}

void Encoder::m_calculateSpeed(const double &time_elapsed) {

    m_pulse_width = m_start_pulse - m_end_pulse;
    m_pulse_counter += m_pulse_width;
    m_pulse_per_sec = m_pulse_width / (2.0 * time_elapsed);
    double rev_per_sec = m_pulse_per_sec / COUNTS_PER_REV;
    m_velocity = rev_per_sec * CIRCUMFERENCE;
    start();
}

int Encoder::getCounter() const {
    return m_pulse_counter;
}

void Encoder::resetCounter() {
    m_pulse_counter = 0;
}

double Encoder::getVelocity() const {
    return m_velocity;
}

double Encoder::getDistance() const {
    return (m_pulse_counter * CIRCUMFERENCE) / (2.0 * COUNTS_PER_REV);
}

double Encoder::getPPS() const {
    return 2 * m_pulse_per_sec;
}

double Encoder::getAverageAngular(const Encoder &right_encoder, const Encoder &left_encoder) {
    return radiansToDegrees((right_encoder.getVelocity() - left_encoder.getVelocity()) / WHEEL_DISTANCE);
}

double Encoder::getAverageVelocity(const Encoder &right_encoder, const Encoder &left_encoder) {
    return 0.5 * (right_encoder.getVelocity() + left_encoder.getVelocity());
}

double Encoder::getAverageDistance(const Encoder &right_encoder, const Encoder &left_encoder) {
    return 0.5 * (right_encoder.getDistance() + left_encoder.getDistance());
}
