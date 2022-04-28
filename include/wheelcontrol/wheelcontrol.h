#ifndef WHEEL_CONTROL_H
#define WHEEL_CONTROL_H

#include "debug1.h"
#include "mbed.h"
#include "PID.h"
#include <utility>
#include <vector>

/** @brief Maximum speed in pps that the robot can go.*/
#define ENCODER_LIMIT 4000.0f
/** @brief Pair<int,float> as pair<direction,PWMOut>.*/
typedef pair<int, float> pif;

/**
 * @brief Controller class to handle speed and steering of the buggy.
 */
class WheelControl {
  private:
    /** @brief PID controller for left speed.*/
    PID m_leftcontroller;

    /** @brief PID controller for right speed.*/
    PID m_rightcontroller;

    /** @brief PID controller for line position.*/
    PID m_linecontroller;

    /** @brief PID output for \link m_linecontroller line controller\endlink.*/
    float m_target;

    /** @brief Set setpoint for \link m_leftcontroller left controller\endlink and \link m_rightcontroller right controller\endlink.*/
    void m_setSetPoint(float left_val, float right_val);
    /** @brief Set process value for \link m_leftcontroller left controller\endlink and \link m_rightcontroller right controller\endlink.*/
    void m_setProcessValue(float left_val, float right_val);

  public:
    /**
     * Construct a new Wheel Control object.
     * @brief Constructor.
     * @details Initializes three PID controllers with default values.
     */
    WheelControl();

    /**
     * @brief Set the speed controller tuning parameters.
     * 
     * @param Kc Propotional gain.
     * @param tauI Integral gain.
     * @param tauD Derivative gain.
     */
    void setSpeedController(float Kc, float tauI, float tauD);

    /**
     * @brief Set the line controller tuning parameters.
     * 
     * @param Kc Propotional gain.
     * @param tauI Integral gain.
     * @param tauD Derivative gain.
     */
    void setLineController(float Kc, float tauI, float tauD);

    /**
     * @brief Set the line controller input limits.
     * 
     * @param low Minimum input value.
     * @param high Maximum input value.
     */
    void setLineLimits(float low, float high);

    /**
     * @brief Set the PWM controller output limits.
     * 
     * @param low Minimum output value.
     * @param high Maximum output value.
     */
    void setPWMLimits(float low, float high);

    /** @brief Set the target speed in m/s.
     * 
     * @param speed Target speed in m/s.
     */
    void setTargetSpeed(float speed);

    /** @brief Reset the PID controllers to initial state.*/
    void reset();

    /**
     * @brief Compute the speed in pps for the left and right motors.
     * 
     * @param position Position of the white line in milimetres.
     * @param left_encoder Left encoder.
     * @param right_encoder Right encoder.
     * @return vector<pif> Vector of pair of direction, PWM outputs for left and right motor.
     */
    vector<pif> computeSpeed(float position, const Encoder &left_encoder, const Encoder &right_encoder);
};

#endif  // WHEEL_CONTROL_H
