#ifndef MOTOR_H
#define MOTOR_H

#include "mbed.h"
#include "encoder.h"
#include <utility>


/** @brief Fixed fast duty cycle.*/
#define FAST_PWM 0.55
/** @brief Fixed normal duty cycle.*/
#define HALF_PWM 0.45
/** @brief Fixed slow duty cycle.*/
#define SLOW_PWM 0.25

/**
 * @brief Motor class to control the motors.
 * 
 */
class Motor{

    private:
    /** @brief Enable pin to motor driver board. */
    DigitalOut state;
    /** @brief Direction for left motor. */
    DigitalOut directionL;
    /** @brief Direction for right motor. */
    DigitalOut directionR;
    /** @brief Period of PWM output. 
     * 
    *\f$period = \frac{1}{frequency}\\\f$
    */
    float period;

    public:

    /** @brief Left motor PwmOut.*/
    PwmOut left;
    /** @brief Right motor PwmOut.*/
    PwmOut right;
    
    /**
     * @brief Constructor.
     * 
     * Construct a motor object.
     * Using unipolar mode (0), pins are sunk to ground instead of using DigitalOut.
     * @param l Left motor pin.
     * @param r Right motor pin.
     * @param dL Direction left motor.
     * @param dR Direction right motor.
     * @param s Enable pin.
     */
    Motor(PinName l, PinName r, PinName dL, PinName dR, PinName s);
    
    /**
     * @brief Set the \link state enable pin \endlink of the motor driver board, given the state.
     * 
     * @param state 
     * - 1 Enabled
     * - 0 Disabled
     */
    void set_enable(int state);

    /**
     * @overload
     * @brief Toggle the \link state enable pin \endlink of the motor driver board, if no state is given.
     * 
     */
    void set_enable();

    /**
     * @brief Get the \link state enable status \endlink of the motor driver board.
     * 
     * @return int Enable status.
     * - 1 Enabled.
     * - 0 Disabled.
     */
    int get_enable();
 
    /**
     * @brief Set the \link period frequency \endlink of the PwmOut.
     * 
     * @param freq Frequency of the PWM.
     */
    void set_frequency(float freq);

    /**
     * @brief Get the \link period frequency \endlink of the PwmOut.
     * 
     * @return float Frequency of the PWM.
     */
    float get_frequency();

    /**
     * @brief Set the direction of the motors, provided the direction.
     * 
     * @param c Which motor direction to change.
     * - 'L' for \link directionL left motor \endlink.
     * - 'R' for \link directionR right motor \endlink.
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
     * - 'L' for \link directionL left motor \endlink.
     * - 'R' for \link directionR right motor \endlink.
     * - otherwise for both motors. 
     */
    void set_direction(char c);

    /**
     * @brief Get the direction of the motors.
     * 
     * @return pair<int,int> {\link directionL Left motor direction \endlink, \link directionR Right motor direction \endlink}.
     */
    pair<int,int> get_direction();

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
     * @return pair<float,float> {Left motor duty cycle, Right motor duty cycle}.
     */
    pair<float,float> get_dutycycle();

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
     * - When \link Encoder::read_distance() read_distance\endlink > distance : Disable enable pin.
     * 
     * 
    */
   
    ///@{
    /**
     * @brief Control the motor to cruise forward.
     * 
     * @param distance Distance to travelled before stopping.
     * @param motor Motor object.
     * @param left Left \link Encoder encoder\endlink object.
     * @param right Right \link Encoder encoder\endlink object.
     */
    static void forward(double distance , Motor* motor, Encoder* left,  Encoder* right);

    /**
     * @brief Control the motor to cruise backwards.
     * 
     * @param distance Distance to travelled before stopping.
     * @param motor Motor object.
     * @param left Left \link Encoder encoder\endlink object.
     * @param right Right \link Encoder encoder\endlink object.
     */
    static void reverse(double distance , Motor* motor, Encoder* left,  Encoder* right);

    /**
     * @brief Control the motor to turn left.
     * 
     * @param angle Angle to make before stopping.
     * @param motor Motor object.
     * @param left Left \link Encoder encoder\endlink object.
     * @param right Right \link Encoder encoder\endlink object.
     */
    static void turnleft(double angle   , Motor* motor, Encoder* left,  Encoder* right);

    /**
     * @brief Control the motor to turn right.
     * 
     * @param angle Angle to make before stopping.
     * @param motor Motor object.
     * @param left Left \link Encoder encoder\endlink object.
     * @param right Right \link Encoder encoder\endlink object.
     */
    static void turnright(double angle  , Motor* motor, Encoder* left,  Encoder* right);

    ///@}
    
    

};


#endif