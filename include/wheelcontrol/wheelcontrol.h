#ifndef WHEEL_CONTROL_H
#define WHEEL_CONTROL_H

#include "mbed.h"
#include "PID.h"
#include <utility>
#include <vector>

typedef pair<float, float> pff;

class WheelControl {
  private:
    float m_target;
    PID leftcontroller;
    PID rightcontroller;
    PID linecontroller;

    void m_setLineOutputLimits();
    void m_setSpeedLimits();
    void m_setSetPoint(float left_val, float right_val);
    void m_setProcessValue(float left_val, float right_val);

  public:
    WheelControl();

    void setSpeedController(float Kc, float tauI, float tauD, float interval);
    void setLineController(float Kc, float tauI, float tauD, float interval);

    void setLineLimits(float low, float high);
    void setPWMLimits(float low, float high);
    void setTargetSpeed(float speed);

    vector<pff> computeSpeed(float position, const Encoder &left_encoder, const Encoder &right_encoder);
};

#endif  // WHEEL_CONTROL_H
