#ifndef ENCODER_H
#define ENCODER_H

#include "mbed.h"
#include "QEI.h"
#include <chrono>

/** @brief Gear ratio module.*/
#define GEAR_RATIO 18.75
/** @brief Diameter of the wheel.*/
#define DIAMETER 0.08
/** @brief Distance between two wheels.*/ 
#define WHEEL_DISTANCE 0.212
/** @brief Counts per revolution of encoder.*/
#define COUNTS_PER_REV 256
/** @brief Polling period of encoder.*/
#define POLLING_PERIOD 1ms
/** @brief Accurate PI value.*/
#define M_PI 3.14159265358979323846264338327950288
/** @brief Convert radians to degree.*/
#define radiansToDegrees(angle) ( (angle) * 180.0 / M_PI)

/**
 * @brief Encoder class with helper functions, derived from QEI
 */
class Encoder: public QEI{

    private:
    
    /** @brief Counter for previous pulse. */
    volatile int start_pulse;

    /** @brief Counter for latest pulse.*/
    volatile int end_pulse;

    /** @brief Counter for pulse difference.*/
    volatile int pulse_width; 

    /** @brief Counter for total pulses from start.*/
    volatile int pulse_counter;

    /** @brief Pulse per seconds.*/
    volatile double pulse_per_sec;

    /** @brief Velocity.*/
    volatile double velocity;

    /** @brief Timer for high-precision clocking.*/
    Timer t;
    
    /** @brief Timeout to interrupt every \link POLLING_PERIOD POLLING_PERIOD\endlink.*/
    Timeout to;

    public:

    /**
     * Construct a new Encoder object.
     * @brief Constructor.
     * 
     * @param A Channel A pin.
     * @param B Channel B pin.
     */
    Encoder(PinName A, PinName B);

    /**
     * @brief Circumference of the wheel.
     */
    static constexpr double CIRCUMFERENCE = DIAMETER * M_PI;

    /**
     * @brief Distance covered in full rotation when turning only using one wheel.
     */
    static constexpr double FULL_ROTATION_SIDE_PIVOT = 2 * M_PI * WHEEL_DISTANCE;

    /**
     * @brief Distance covered in full rotation when turning using both wheels.
     */
    static constexpr double FULL_ROTATION_CENTER_PIVOT = M_PI * WHEEL_DISTANCE;

    /**
     *  Start timer and get current pulse. Timeout for every \link POLLING_PERIOD POLLING_PERIOD\endlink.
     * @brief Start the routine to measure pulses periodically.
     */
    void start();

    /**
     * @brief Get current running sum of pulses.
     * 
     * @return int Total number of pulses after start.
     */
    int read_counter();

    /**
     * @brief Reset the running sum of pulses used for \link read_distance() distance\endlink.
     */
    void reset_counter();

    /**
     * @brief Get current velocity.
     * 
     * @return double Velocity of the wheel.
     */
    double read_velocity();

    /**
     * @brief Get total distance travelled.
     * 
     *  The distance can be calculated from known values of \link COUNTS_PER_REV CPR\endlink and \link CIRCUMFERENCE Circumference\endlink
     * which is defined by
     * \f$distance = \frac{pulse\ counter\  \cdot\  cirmcumference}{2\  \cdot\  CPR}\\\f$
     * @return double Total distance (metre) travelled by the wheel.
     */
    double read_distance();

    /**
     * @brief Get current pulses per unit second (PPS).
     * 
     * @return double PPS of the wheel.
     */
    double read_pps();

    /**
     * @brief Get average of angular velocity between two wheels.
     *
     * +ve indicates tilting to the right.
     * -ve indicates tilting to the left.
     * @param right Right Wheel encoder
     * @param left Left Wheel encoder
     * @return double Angular velocity (deg/s) of the buggy.
     */
    static double average_angular(Encoder& right, Encoder& left);

    /**
     * @brief Get average of linear velocity between two wheels.
     * 
     * +ve indicates forward direction.
     * -ve indicates reverse direction.
     * @param right Right Wheel encoder.
     * @param left Left Wheel encoder.
     * @return double Linear velocity (m/s) of the buggy.
     */
    static double average_velocity(Encoder& right, Encoder& left);

    /**
     * @brief Get average of total \link read_distance() distance\endlink between two wheels.
     * 
     * +ve indicates forward direction.
     * -ve indicates reverse direction.
     * @param right Right Wheel encoder.
     * @param left Left Wheel encoder.
     * @return double Average distance of the buggy.
     */
    static double average_distance(Encoder& right, Encoder& left);

    
   private:
    /**
     * @brief Calculate the velocity of the wheel.
     *
     * The velocity can be calculated from known values of \link COUNTS_PER_REV CPR\endlink and \link CIRCUMFERENCE Circumference\endlink
     * which is defined by
     * \f$velocity = \frac{pulse\ width\  \cdot\  cirmcumference}{2\  \cdot\  CPR \ \cdot \ time}\\\f$
     * @param time The elapsed time between two pulse count.
     */
    void calculate_speed(double time);

    /**
     * @brief Stop the timer and get current pulse.
     */
    void stop();


};

#endif