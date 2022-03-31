#ifndef WHEEL_CONTROL_H
#define WHEEL_CONTROL_H

#include "mbed.h"
#include "PID.h"
#include <utility>
#include <vector>

#define OFFSET 27.0f
#define ENCODER_LIMIT 2000.0f
typedef pair<int, float> pif;

class WheelControl {
  private:
    float m_target;
    PID leftcontroller;
    PID rightcontroller;
    PID linecontroller;

    void m_setSetPoint(float left_val, float right_val);
    void m_setProcessValue(float left_val, float right_val);

  public:
    WheelControl();

    void setSpeedController(float Kc, float tauI, float tauD, float interval);
    void setLineController(float Kc, float tauI, float tauD, float interval);

    void setLineLimits(float low, float high);
    void setPWMLimits(float low, float high);
    void setTargetSpeed(float speed);

    vector<pif> computeSpeed(float position, const Encoder &left_encoder, const Encoder &right_encoder);
};

#endif  // WHEEL_CONTROL_H
