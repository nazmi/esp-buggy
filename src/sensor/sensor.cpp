#include "sensor.h"
#include <algorithm>
#include <cstdio>

bool Sensor::s_run = false;
std::array<float, 6> Sensor::WEIGHTS = {27, 9, -9, -27, 9, -9};
std::array<float, 6> Sensor::BLACK_TRESHOLD = {0.000576735, 0.008243035, 0.010218155, 0.004252569, 0.007279201, 0.023050267};
std::array<float, 6> Sensor::SCALE_FACTOR = {3.63554, 3.51805, 3.77641, 3.40797, 3.50128, 2.81837};

Sensor::Sensor(PinName p1, PinName p2, PinName p3, PinName p4, PinName p5, PinName p6,
               PinName in1, PinName in2, PinName in3, PinName in4, PinName in5, PinName in6)
    : m_pins(p1, p2, p3, p4, p5, p6),
      m_analog{AnalogIn(in1), AnalogIn(in2), AnalogIn(in3), AnalogIn(in4), AnalogIn(in5), AnalogIn(in6)} {};

float Sensor::read() {

    m_distance = 0;

    // All off to capture noise
    m_pins.write(0);
    wait_us(Config::Sensor::SENSOR_WAIT_US);
    for (size_t i = 0; i < m_analog.size(); ++i) {
        m_noise[i] = m_analog[i].read();
    }

    // Turn on in sequence, reading -= noise
    for (size_t i = 0; i < m_analog.size(); ++i) {
        m_pins.write(1 << i);
        wait_us(Config::Sensor::SENSOR_WAIT_US);
        m_reading[i] = m_analog[i].read();
        m_reading[i] = clamp(m_reading[i] - m_noise[i], 0.0f, 1.0f);
    }

    // Process reading[i] -= minimum
    // If reading[i] > BLACK_TRESHOLD, output reading[i]
    const float min = *std::min_element(m_reading.begin(), m_reading.end());
    for (size_t i = 0; i < m_analog.size(); ++i) {
        m_reading[i] = clamp(m_reading[i] - min, 0.0f, 1.0f);

        const bool isAboveThreshold = m_reading[i] > BLACK_TRESHOLD[i];
        if (isAboveThreshold) {
            m_reading[i] = m_reading[i] * Config::Sensor::PRESCALER * SCALE_FACTOR[i];
        } else {
            m_reading[i] = 0.0f;
        }
    }

    // Check if any of 6 sensors detect white line
    const auto isUnderWhiteLine = std::any_of(m_reading.begin(), m_reading.end(),
                                              [](const float reading) {
                                                  return reading > Config::Sensor::WHITE_THRESHOLD;
                                              });
    if (isUnderWhiteLine) {
        m_notrackcounter = 0;

        // Count how many sensors are actively detecting
        const int activeSensorCount = std::count_if(m_reading.begin(), m_reading.end(),
                                                    [](const float reading) {
                                                        return reading > Config::Sensor::WHITE_THRESHOLD;
                                                    });

        // Check if second row (rear sensors) is contributing
        const auto isCenterOn = m_reading[4] > Config::Sensor::WHITE_THRESHOLD ||
                               m_reading[5] > Config::Sensor::WHITE_THRESHOLD;

        // Fixed: Handle single sensor detection by always using appropriate sensor array
        // If only one sensor detects and it's from the second row, still use 6 sensors
        // If second row contributes, use all 6 sensors; otherwise use only front 4
        if (isCenterOn || activeSensorCount == 1) {
            m_distance = arm_weighted_sum_f32(Sensor::WEIGHTS.data(), m_reading.data(), 6);
        } else {
            m_distance = arm_weighted_sum_f32(Sensor::WEIGHTS.data(), m_reading.data(), 4);
        }

    } else {
        m_distance = Config::Sensor::NO_TRACK_VALUE;
        m_notrackcounter++;
    }

    // Debugging Stuff
    if (_SENSORS_DEBUG) {
        if (!s_run) {
            DEBUGLOG("1,2,3,4,5,6,time,m_distance\n");
            s_run = true;
        }

        for (const auto &reading : m_reading) {
            DEBUGLOG("%.5f,", reading);
        }
        DEBUGLOG("%f\n", m_distance);
    }

    return m_distance;
}

float Sensor::getDistance() const {
    return m_distance;
}

int Sensor::getNoTrackCounter() const {
    return m_notrackcounter;
}

void Sensor::calibrateBlack() {
    std::array<float, 6> calibrate_data{};
    wait_us(Config::Sensor::SENSOR_WAIT_US);

    // Read sensors one by one
    for (size_t i = 0; i < m_analog.size(); ++i) {
        m_pins.write(1 << i);
        wait_us(Config::Sensor::SENSOR_WAIT_US);
        calibrate_data[i] = m_analog[i].read();
    }

    // Print into csv format
    if (!s_run) {
        printf("1,2,3,4,5,6\n");
        s_run = true;
    }
    printf("%.5f,%.5f,%.5f,%.5f,%.5f,%.5f\n",
           calibrate_data[0], calibrate_data[1], calibrate_data[2],
           calibrate_data[3], calibrate_data[4], calibrate_data[5]);
}

void Sensor::calibrateWhite() {
    // Turn all off to read noise
    m_pins.write(0);
    wait_us(Config::Sensor::SENSOR_WAIT_US);
    for (size_t i = 0; i < m_analog.size(); ++i) {
        m_noise[i] = m_analog[i].read();
    }

    // Read sensors one by one
    for (size_t i = 0; i < m_analog.size(); ++i) {
        m_pins.write(1 << i);
        wait_us(Config::Sensor::SENSOR_WAIT_US);

        const float adc_value = m_analog[i].read();

        const bool isAboveThreshold = adc_value > m_noise[i] && adc_value > BLACK_TRESHOLD[i];
        if (isAboveThreshold) {
            m_reading[i] = (adc_value - m_noise[i]) * Config::Sensor::PRESCALER;
        } else {
            m_reading[i] = 0.0f;
        }
    }

    // Print for csv format
    if (!s_run) {
        printf("1,2,3,4,5,6\n");
        s_run = true;
    }
    printf("%.5f,%.5f,%.5f,%.5f,%.5f,%.5f\n",
           m_reading[0], m_reading[1], m_reading[2], m_reading[3], m_reading[4], m_reading[5]);
}
