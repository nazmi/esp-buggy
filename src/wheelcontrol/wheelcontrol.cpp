#include "wheelcontrol.h"

WheelControl::WheelControl() : m_leftcontroller(1.0f, 0.0f, 0.0f, 0.01f),
                               m_rightcontroller(1.0f, 0.0f, 0.0f, 0.01f),
                               m_linecontroller(1.0f, 0.0f, 0.0f, 0.01f) {

    m_leftcontroller.setInputLimits(0, ENCODER_LIMIT);
    m_rightcontroller.setInputLimits(0, ENCODER_LIMIT);
}

void WheelControl::setSpeedController(float Kc, float tauI, float tauD) {

    m_leftcontroller.setTunings(Kc, tauI, tauD);
    m_rightcontroller.setTunings(Kc, tauI, tauD);
}

void WheelControl::setLineController(float Kc, float tauI, float tauD) {

    m_linecontroller.setTunings(Kc, tauI, tauD);
}

void WheelControl::setLineLimits(float low, float high) {

    m_linecontroller.setInputLimits(low, high);
}

void WheelControl::setPWMLimits(float low, float high) {

    m_leftcontroller.setOutputLimits(low, high);
    m_rightcontroller.setOutputLimits(low, high);
}

void WheelControl::setTargetSpeed(float speed) {

    if (m_target < 0)
        return;

    m_target = clamp(speed_to_pps(speed), 0.0f, ENCODER_LIMIT);
    m_linecontroller.setOutputLimits(0, m_target);
}

void WheelControl::reset() {

    m_leftcontroller.reset();
    m_rightcontroller.reset();
    m_linecontroller.reset();
}

void WheelControl::m_setSetPoint(float left_val, float right_val) {

    m_leftcontroller.setSetPoint(left_val);
    m_rightcontroller.setSetPoint(right_val);
}

void WheelControl::m_setProcessValue(float left_val, float right_val) {

    m_leftcontroller.setProcessValue(left_val);
    m_rightcontroller.setProcessValue(right_val);
}

vector<pif> WheelControl::computeSpeed(float position, const Encoder &left_encoder, const Encoder &right_encoder) {

    float abs_position{abs(position)};
    float abs_left_encoder_pps{static_cast<float>(abs(left_encoder.getPPS()))};
    float abs_right_encoder_pps{static_cast<float>(abs(right_encoder.getPPS()))};

    DEBUGLOG("Encoder PPS: [%.5f %.5f]\n", abs_left_encoder_pps, abs_right_encoder_pps);
    // Compute line follower controller
    m_linecontroller.setProcessValue(position);
    DEBUGLOG("PID OUTPUT:       PV      SP     Error    Derror  Output  ScaledOutput\n");
    DEBUGLOG("Line output : ");

    // Positive position : Controller output is negative
    // Negative position : Controller output is positive
    float delta_target = m_linecontroller.compute();
    float delta_left = m_target - delta_target;
    float delta_right = m_target + delta_target;

    // Adaptive Set Point
    if (abs_position < 0.5f) {

        m_setSetPoint(m_target, m_target);

    } else if (abs_position > 15.0f) {

        m_setSetPoint(delta_left - 0.4 * delta_target, delta_right + 0.4 * delta_target);

    } else {

        m_setSetPoint(delta_left, delta_right);

    }

    m_setProcessValue(abs_left_encoder_pps, abs_right_encoder_pps);
    DEBUGLOG("Left output : ");
    float left_output = clamp(m_leftcontroller.compute(), 0.0f, 1.0f);
    DEBUGLOG("Right output: ");
    float right_output = clamp(m_rightcontroller.compute(), 0.0f, 1.0f);

    // Special cases when going up of ramp
    // Braking when go down.
    // Producing when go up.
    vector<pif> output{{1, left_output}, {1, right_output}};

    if (abs_left_encoder_pps > m_target && abs_position < 1.0f) {

        output[0].first = 0;
        output[0].second = 0.1f;
    }

    if (abs_right_encoder_pps > m_target && abs_position < 1.0f) {

        output[1].first = 0;
        output[1].second = 0.1f;
    }

    return output;
}
