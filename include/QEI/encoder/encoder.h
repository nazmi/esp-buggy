#ifndef ENCODER_H
#define ENCODER_H

#include "QEI.h"
#include "mbed.h"
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
#define POLLING_PERIOD 10ms

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif
/**
 * @brief Encoder class with helper functions, derived from QEI.
 * @details This class call a function periodically to measure the speed of wheel. It features return values in terms of pulse per second, distance, and velocity.
 */
class Encoder : public QEI {
  private:
    /** @brief Counter for previous pulse. */
    volatile int m_start_pulse;

    /** @brief Counter for latest pulse.*/
    volatile int m_end_pulse;

    /** @brief Counter for pulse difference.*/
    volatile int m_pulse_width;

    /** @brief Counter for total pulses from start.*/
    volatile int m_pulse_counter;

    /** @brief Pulse per unit second.*/
    volatile double m_pulse_per_sec;

    /** @brief Velocity.*/
    volatile double m_velocity;

    /** @brief Timer for high-precision clocking.*/
    Timer m_t;

    /** @brief Timeout to interrupt every #POLLING_PERIOD.*/
    Timeout m_to;

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
    static constexpr double FULL_ROTATION_SIDE_PIVOT = 2.0 * M_PI * WHEEL_DISTANCE;

    /**
     * @brief Distance covered in full rotation when turning using both wheels.
     */
    static constexpr double FULL_ROTATION_CENTER_PIVOT = M_PI * WHEEL_DISTANCE;

    /**
     *  Start timer and get current pulse. Timeout for every #POLLING_PERIOD.
     * @brief Start the routine to measure pulses periodically.
     */
    void start();

    /**
     * @brief Get current running \link m_pulse_counter sum of pulses\endlink.
     *
     * @return int Total number of pulses after start.
     */
    int read_counter() const;

    /**
     * @brief Reset the running \link m_pulse_counter sum of pulses\endlink used for read_distance().
     */
    void reset_counter();

    /**
     * @brief Get \link m_velocity current velocity\endlink.
     *
     * @return double Velocity of the wheel.
     */
    double read_velocity() const;

    /**
     * @brief Get total distance travelled.
     *
     * The distance can be calculated from known values of #COUNTS_PER_REV and #CIRCUMFERENCE
     * which is defined by
     * \f$distance = \frac{pulse\ counter\  \cdot\  cirmcumference}{2\  \cdot\  CPR}\\\f$
     * @return double Total distance (metre) travelled by the wheel.
     */
    double read_distance() const;

    /**
     * @brief Get current \link m_pulse_per_sec  pulses per unit second (PPS)\endlink.
     *
     * @return double PPS of the wheel.
     */
    double read_pps() const;

    /**
     * @brief Get average of angular \link m_velocity velocity\endlink between two wheels.
     *
     * +ve indicates tilting to the right.
     * -ve indicates tilting to the left.
     * @param right_encoder Right Wheel encoder
     * @param left_encoder Left Wheel encoder
     * @return double Angular velocity \link radiansToDegrees(const T &angle) (deg/s)\endlink of the buggy.
     */
    static double average_angular(const Encoder &right_encoder, const Encoder &left_encoder);

    /**
     * @brief Get average of linear \link m_velocity velocity\endlink between two wheels.
     *
     * +ve indicates forward direction.
     * -ve indicates reverse direction.
     * @param right_encoder Right Wheel encoder.
     * @param left_encoder Left Wheel encoder.
     * @return double Linear velocity (m/s) of the buggy.
     */
    static double average_velocity(const Encoder &right_encoder, const Encoder &left_encoder);

    /**
     * @brief Get average of total \link read_distance() distance\endlink between two wheels.
     *
     * +ve indicates forward direction.
     * -ve indicates reverse direction.
     * @param right_encoder Right Wheel encoder.
     * @param left_encoder Left Wheel encoder.
     * @return double Average distance of the buggy.
     */
    static double average_distance(const Encoder &right_encoder, const Encoder &left_encoder);

  private:
    /**
     * @brief Calculate the \link m_velocity velocity\endlink of the wheel.
     *
     * The velocity can be calculated from known values of #COUNTS_PER_REV CPR and #CIRCUMFERENCE
     * which is defined by
     * \f$velocity = \frac{pulse\ width\  \cdot\  cirmcumference}{2\  \cdot\  CPR \ \cdot \ time}\\\f$
     * @param time_elapsed The elapsed time between two pulse count.
     */
    void calculate_speed(const double &time_elapsed);

    /**
     * @brief Stop the timer and get current pulse.
     */
    void stop();
};

#endif
