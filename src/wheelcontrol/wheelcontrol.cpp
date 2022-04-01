#include "wheelcontrol.h"

WheelControl::WheelControl() : leftcontroller(2.0, 1.2, 0.0002, 0.01),
                               rightcontroller(2.0, 1.2, 0.0002, 0.01),
                               linecontroller(1.6, 0.00001, 0.0000001, 0.01) {

    leftcontroller.setInputLimits(0, ENCODER_LIMIT);
    rightcontroller.setInputLimits(0, ENCODER_LIMIT);
}

void WheelControl::setSpeedController(float Kc, float tauI, float tauD) {

    leftcontroller.setTunings(Kc, tauI, tauD);
    rightcontroller.setTunings(Kc, tauI, tauD);
}

void WheelControl::setLineController(float Kc, float tauI, float tauD) {

    linecontroller.setTunings(Kc, tauI, tauD);
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

    m_target = clamp(speed_to_pps(speed), 0.0f, ENCODER_LIMIT);
    linecontroller.setOutputLimits(0, m_target);
}

void WheelControl::reset() {

    leftcontroller.reset();
    rightcontroller.reset();
    linecontroller.reset();
}

void WheelControl::m_setSetPoint(float left_val, float right_val) {

    leftcontroller.setSetPoint(left_val);
    rightcontroller.setSetPoint(right_val);
}

void WheelControl::m_setProcessValue(float left_val, float right_val) {

    leftcontroller.setProcessValue(left_val);
    rightcontroller.setProcessValue(right_val);
}

vector<pif> WheelControl::computeSpeed(float position, const Encoder &left_encoder, const Encoder &right_encoder) {

    // bool positionIsPositive{true};
    float abs_position{abs(position)};
    float abs_left_encoder_pps{static_cast<float>(abs(left_encoder.read_pps()))};
    float abs_right_encoder_pps{static_cast<float>(abs(right_encoder.read_pps()))};

    // Record the position value so controller output can be manipulated
    // if (position < 0) {
    //     positionIsPositive = false;
    // }

    // Compute line follower controller
    linecontroller.setProcessValue(position);
    printf("PID OUTPUT:       PV      SP     Error    Derror  Output  ScaledOutput\n");
    printf("Line output : ");

    // Positive position : Controller output is negative
    // Negative position : Controller output is positive
    float delta_target = linecontroller.compute();
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
    printf("Left output : ");
    float left_output = clamp(leftcontroller.compute(), 0.0f, 1.0f);
    printf("Right output: ");
    float right_output = clamp(rightcontroller.compute(), 0.0f, 1.0f);

    // Special cases when going up of ramp
    // Braking when go down.
    // Producing when go up.
    vector<pif> results{{1, left_output}, {1, right_output}};

    if (abs_left_encoder_pps > m_target && abs_position < 1.0f) {

        results[0].first = 0;
        results[0].second = 0.1f;
    }

    if (abs_right_encoder_pps > m_target && abs_position < 1.0f) {

        results[1].first = 0;
        results[1].second = 0.1f;
    }

    return results;
}
