#include "wheelcontrol.h"

WheelControl::WheelControl() : leftcontroller(1.2, 1.2, 0.00012, 0.01),
                               rightcontroller(1.2, 1.2, 0.00012, 0.01),
                               linecontroller(1.6, 0.00001, 0.0000001, 0.01) {

    leftcontroller.setMode(1);
    rightcontroller.setMode(1);
    linecontroller.setMode(1);
}

void WheelControl::setSpeedController(float Kc, float tauI, float tauD, float interval) {

    leftcontroller.setTunings(Kc, tauI, tauD);
    leftcontroller.setInterval(interval);

    rightcontroller.setTunings(Kc, tauI, tauD);
    rightcontroller.setInterval(interval);
}

void WheelControl::setLineController(float Kc, float tauI, float tauD, float interval) {

    linecontroller.setTunings(Kc, tauI, tauD);
    linecontroller.setInterval(interval);
}

void WheelControl::setLineLimits(float low, float high) {

    linecontroller.setInputLimits(low, high);
}

void WheelControl::setPWMLimits(float low, float high) {

    leftcontroller.setOutputLimits(low, high);
    rightcontroller.setOutputLimits(low, high);
}

void WheelControl::setTargetSpeed(float speed) {

    if (m_target < 0)
        return;

    m_target = speed_to_pps(speed);
    m_setLineOutputLimits();
    m_setSpeedLimits();

}

void WheelControl::m_setLineOutputLimits() {

    linecontroller.setOutputLimits(-m_target, m_target);
}

void WheelControl::m_setSpeedLimits() {

    leftcontroller.setInputLimits(0.0f, 2 * m_target);
    rightcontroller.setInputLimits(0.0f, 2 * m_target);
}

void WheelControl::m_setSetPoint(float left_val, float right_val) {

    leftcontroller.setSetPoint(left_val);
    rightcontroller.setSetPoint(right_val);
}

void WheelControl::m_setProcessValue(float left_val, float right_val) {

    leftcontroller.setProcessValue(left_val);
    rightcontroller.setProcessValue(right_val);
}

pair<float, float> WheelControl::computeSpeed(float position, const Encoder &left_encoder, const Encoder &right_encoder) {

    linecontroller.setProcessValue(position);
    auto delta_distance = linecontroller.compute();
    m_setSetPoint(m_target + delta_distance, m_target - delta_distance);
    m_setProcessValue(static_cast<float>(left_encoder.read_pps()), static_cast<float>(right_encoder.read_pps()));
    return { leftcontroller.compute() , rightcontroller.compute() };
}
