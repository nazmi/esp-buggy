#include "wheelcontrol.h"

WheelControl::WheelControl() : leftcontroller(2.0, 1.2, 0.0002, 0.01),
                               rightcontroller(2.0, 1.2, 0.0002, 0.01),
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

    linecontroller.setOutputLimits(0, m_target);
}

void WheelControl::m_setSpeedLimits() {

    leftcontroller.setInputLimits(0, 2 * m_target);
    rightcontroller.setInputLimits(0, 2 * m_target);
}

void WheelControl::m_setSetPoint(float left_val, float right_val) {

    leftcontroller.setSetPoint(left_val);
    rightcontroller.setSetPoint(right_val);
}

void WheelControl::m_setProcessValue(float left_val, float right_val) {

    leftcontroller.setProcessValue(left_val);
    rightcontroller.setProcessValue(right_val);
}

vector<pff> WheelControl::computeSpeed(float position, const Encoder &left_encoder, const Encoder &right_encoder) {

    bool positionIsPositive{true};
    float abs_position{position};
    float abs_left_encoder_pps{static_cast<float>(abs(left_encoder.read_pps()))};
    float abs_right_encoder_pps{static_cast<float>(abs(right_encoder.read_pps()))};

    // PID works in %, unsigned range so, manipulating data is required.
    // PID outputs only +ve value, no direction can be extracted.
    if (position < 0) {

        abs_position = abs(position);
        positionIsPositive = false;
    }

    // Compute line follower controller
    linecontroller.setProcessValue(abs_position);
    float delta_target = linecontroller.compute();

    // Compute speed controller
    if (positionIsPositive) {

        m_setSetPoint(m_target + delta_target, m_target);

    } else {

        m_setSetPoint(m_target, m_target + delta_target);
    }

    m_setProcessValue(abs_left_encoder_pps, abs_right_encoder_pps);

    float left_output = leftcontroller.compute();
    float right_output = rightcontroller.compute();

    // Special cases when going up of ramp
    // Braking when go down.
    // Producing when go up.
    vector<pff> results{{1.0f, left_output}, {1.0f, right_output}};

    if (abs_left_encoder_pps > m_target && abs_position < 9.0f) {

        results[0].first = -1.0f;
        results[0].second = 0.3;
    }

    if (abs_right_encoder_pps > m_target && abs_position < 9.0f) {

        results[1].first = -1.0f;
        results[1].second = 0.3;
    }

    return results;
}
