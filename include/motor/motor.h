#ifndef MOTOR_H
#define MOTOR_H

#include "encoder.h"
#include "helper.h"
#include "mbed.h"
#include <utility>

/** @brief Fast duty cycle constant.*/
#define FAST_PWM 0.6
/** @brief Normal duty cycle constant.*/
#define HALF_PWM 0.5
/** @brief Slow duty cycle constant.*/
#define SLOW_PWM 0.3

/**
 * @brief Motor class to control the motors.
 * @details This class includes configuration method of left and right motors. It also features cruising methods to move forward, backwards, turn left and turn left.
 *
 */
class Motor {

  private:
    /** @brief Direction for the left motor. */
    DigitalOut m_direction_left;
    /** @brief Direction for the right motor. */
    DigitalOut m_direction_right;
    /** @brief Enable pin to motor driver board. */
    DigitalOut m_enable;
    /** @brief Period of PWM output.
     *
     *\f${m\\_period} = \frac{1}{frequency}\\\f$
     */
    float m_period;

  public:
    /** @brief Left motor PwmOut.*/
    PwmOut left_motor;
    /** @brief Right motor PwmOut.*/
    PwmOut right_motor;

    /**
     * @brief Constructor.
     *
     * Construct a motor object.
     * Using unipolar mode (0), pins are sunk to ground instead of using DigitalOut.write(0).
     * @param left Left motor pin.
     * @param right Right motor pin.
     * @param direction_left Direction left motor.
     * @param direction_right Direction right motor.
     * @param enable Enable pin.
     */
    Motor(PinName left, PinName right, PinName direction_left, PinName direction_right, PinName enable);

    /**
     * @brief Set the \link m_enable enable pin\endlink of the motor driver board, given the enable.
     *
     * @param enable
     * - 1 Enabled
     * - 0 Disabled
     */
    void set_enable(int enable);

    /**
     * @overload
     * @brief Toggle the \link m_enable enable pin\endlink of the motor driver board, if no argument is given.
     *
     */
    void set_enable();

    /**
     * @brief Get the \link m_enable enable status\endlink of the motor driver board.
     *
     * @return int Enable status.
     * - 1 Enabled.
     * - 0 Disabled.
     */
    int get_enable();

    /**
     * @brief Set the \link m_period frequency\endlink of the PwmOut.
     *
     * @param frequency Frequency of the PWM.
     */
    void set_frequency(float frequency);

    /**
     * @brief Get the \link m_period frequency\endlink of the PwmOut.
     *
     * @return float Frequency of the PWM.
     */
    float get_frequency() const;

    /**
     * @brief Set the direction of the motors, provided the direction.
     *
     * @param c Which motor direction to change.
     * - 'L' for \link m_direction_left left motor\endlink.
     * - 'R' for \link m_direction_right right motor\endlink.
     * - otherwise for both motors.
     * @param direction
     * - 1 forward.
     * - 0 reverse.
     */
    void set_direction(char c, int direction);

    /**
     * @overload
     * @brief Toggle the direction of the motors, if no direction given.
     *
     * @param c Which motor direction to change.
     * - 'L' for \link m_direction_left left motor\endlink.
     * - 'R' for \link m_direction_right right motor\endlink.
     * - otherwise for both motors.
     */
    void set_direction(char c);

    /**
     * @brief Get the direction of the motors.
     *
     * @return std::pair<int,int> { #m_direction_left, #m_direction_right }.
     */
    std::pair<int, int> get_direction() const;

    /**
     * @brief Set the duty cycle of the motors.
     *
     * @param c Which motor to set duty cycle.
     * - 'L' Left motor.
     * - 'R' Right motor.
     * - otherwise both motors.
     * @param dutycycle Duty cycle in percentange within [0.0,1.0]. If duty cycle is out of range, it saturates at 0 and 1.
     *
     */
    void set_dutycycle(char c, float dutycycle);

    /**
     * @brief Get the duty cycle of the motors.
     *
     * @return std::pair<float,float> {Left motor duty cycle, Right motor duty cycle}.
     */
    std::pair<float, float> get_dutycycle();

    /**
     * @name Cruising Methods
     * All cruising methods to control the motors include forward, reverse, turnleft and turnright.
     * All methods apply the following sequence:
     *
     * - Disable the enable pin.
     * - Set the direction of the motors.
     * - Set the duty cycle of the motors.
     * - Reset the counter of the encoders.
     * - Toggle enable pin.
     *
     * - When Encoder::read_distance() > distance : Disable enable pin.
     *
     *
     */

    ///@{
    /**
     * @brief Control the motor to cruise forward.
     *
     * @param distance Distance to travelled before stopping.
     * @param motor Motor object.
     * @param left_encoder Left Encoder object.
     * @param right_encoder Right Encoder object.
     */
    static void forward(const double distance, Motor *const motor, Encoder *const left_encoder, Encoder *const right_encoder);

    /**
     * @brief Control the motor to cruise backwards.
     *
     * @param distance Distance to travelled before stopping.
     * @param motor Motor object.
     * @param left_encoder Left Encoder object.
     * @param right_encoder Right Encoder object.
     */
    static void reverse(const double distance, Motor *const motor, Encoder *const left_encoder, Encoder *const right_encoder);

    /**
     * @brief Control the motor to turn left.
     *
     * @param angle Angle to make before stopping.
     * @param motor Motor object.
     * @param left_encoder Left Encoder object.
     * @param right_encoder Right Encoder object.
     */
    static void turnleft(const double angle, Motor *const motor, Encoder *const left_encoder, Encoder *const right_encoder);

    /**
     * @brief Control the motor to turn right.
     *
     * @param angle Angle to make before stopping.
     * @param motor Motor object.
     * @param left_encoder Left Encoder object.
     * @param right_encoder Right Encoder object.
     */
    static void turnright(const double angle, Motor *const motor, Encoder *const left_encoder, Encoder *const right_encoder);

    ///@}
};

#endif