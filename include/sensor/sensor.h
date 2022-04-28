#ifndef SENSOR_H
#define SENSOR_H

#include "debug.h"
#include "arm_math.h"
#include "mbed.h"
#include "helper.h"
#include <algorithm>
#include <array>
#include <cstdio>

/** @brief Prescaler to normalised the sensor readings.*/
#define PRESCALER 10.0f
/** @brief Arbitrary value for out of track detection.*/
#define NO_TRACK 30
/** @brief Minimum treshold for white line */
#define WHITE_TRESHOLD 0.8

#ifdef SENSORS_DEBUG
#define _SENSORS_DEBUG 1
#else
#define _SENSORS_DEBUG 0
#endif

/**
 * @brief Sensor class to handle the sensor readings.
 * @details This class handles the sensor readings and provides the data in terms of position from white line.
 * 
 */
class Sensor {
  private:
    /** @brief Array of sensors DigitalOut.*/
    BusOut m_pins;

    /** @brief Array of sensors AnalogIn.*/
    std::array<AnalogIn, 6> m_analog;

    /** @brief Array of sensors readings.*/
    std::array<float, 6> m_reading{};

    /** @brief Array of sensors noise.*/
    std::array<float, 6> m_noise{};

    /** @brief Sensor position.*/
    float m_distance{0};

    /** @brief No track counter.*/
    int m_notrackcounter{0};

  public:
    /**
     * Construct a new Sensor object.
     * @brief Constructor.
     *
     * @param p1,p2,p3,p4,p5,p6 Array of sensors DigitalOut.
     * @param in1,in2,in3,in4,in5,in6 Array of sensors AnalogIn.
     */
    Sensor(PinName p1, PinName p2, PinName p3, PinName p4, PinName p5, PinName p6,
           PinName in1, PinName in2, PinName in3, PinName in4, PinName in5, PinName in6);

    /** @brief Array of constant weights for sensors.*/
    static std::array<float, 6> WEIGHTS;

    /** @brief Array of constant black threshold for sensors.*/
    static std::array<float, 6> BLACK_TRESHOLD;

    /** @brief Array of constant scale factor for sensors.*/
    static std::array<float, 6> SCALE_FACTOR;

    /** @brief Debugging flag for printing headers.*/
    static bool s_run;

    /**
     * @brief Read the sensor values.
     * @details Read the sensor values and normalise them.
     * 
     */
    float read();

    /**
     * @brief Get the distance.
     * @details Get the distance from the center line.
     * 
     * @return float Distance from the center line.
     */
    float getDistance() const;

    /**
     * @brief Get the no track counter value.
     * 
     * @return int m_no_track_counter
     */
    int getNoTrackCounter() const;

    /**
     * @brief Calibrate the sensors on black area of the track.
     * 
     */
    void calibrateBlack();

    /**
     * @brief Calibrate the sensors on white area of the track.
     * 
     */
    void calibrateWhite();

};

#endif
