#include "sensor.h"
#include "helper.h"
#include <algorithm>

using std::min_element;

bool Sensor::s_run = false;
std::array<float, 6> Sensor::WEIGHTS = {27, 9, -9, -27, 9, -9};
std::array<float, 6> Sensor::BLACK_TRESHOLD = {0.004934593, 0.014571535, 0.013338404,
                                               0.01000882, 0.015825268, 0.02775455};
std::array<float, 6> Sensor::SCALE_FACTOR = {3.13481219, 3.064968664, 3.252208943,
                                             3.197312711, 3.097903068, 2.695666361};

Sensor::Sensor(PinName p1, PinName p2, PinName p3, PinName p4, PinName p5, PinName p6,
               PinName in1, PinName in2, PinName in3, PinName in4, PinName in5, PinName in6)
    : m_pins(p1, p2, p3, p4, p5, p6),
      m_analog{AnalogIn(in1), AnalogIn(in2), AnalogIn(in3), AnalogIn(in4), AnalogIn(in5), AnalogIn(in6)} {};

float Sensor::read() {

    m_distance = 0;
    Timer t;
    t.start();

    // All off to capture noise
    m_pins.write(0);
    wait_us(25);
    for (size_t i = 0; i < m_analog.size(); ++i) {

        m_noise[i] = m_analog[i].read();
    }

    // Turn on in sequence, reading -= noise
    for (size_t i = 0; i < m_analog.size(); ++i) {

        m_pins.write(1 << i);
        wait_us(25);
        m_reading[i] = m_analog[i].read();
        m_reading[i] = clamp(m_reading[i] - m_noise[i], 0.0f, 1.0f);
    }

    // Process reading[i] -= minimum
    // If reading[i] > BLACK_TRESHOLD, output reading[i]
    float min = *std::min_element(m_reading.begin(), m_reading.end());
    for (size_t i = 0; i < m_analog.size(); ++i) {

        m_reading[i] = clamp(m_reading[i] - min, 0.0f, 1.0f);

        const auto isAboveTreshold = m_reading[i] > BLACK_TRESHOLD[i];
        if (isAboveTreshold) {
            m_reading[i] = m_reading[i] * PRESCALER * SCALE_FACTOR[i];
        } else {
            m_reading[i] = 0;
        }

        // m_reading[i] = clamp(m_reading[i], 0.0f, PRESCALER);
    }

    // Check if any of 6 m_pins m_reading > WHITE_TRESHOLD for white
    const auto isUnderWhiteLine = std::any_of(m_reading.begin(), m_reading.end(),
                                              [](float reading) { return reading > WHITE_TRESHOLD; });
    if (isUnderWhiteLine) {

        // Check if second row is contributing to any m_distance m_reading
        const auto isCenterOn = m_reading[4] > WHITE_TRESHOLD || m_reading[5] > WHITE_TRESHOLD;
        if (isCenterOn) {
            m_distance = arm_weighted_sum_f32(Sensor::WEIGHTS.cbegin(), m_reading.cbegin(), 6);
        } else {
            m_distance = arm_weighted_sum_f32(Sensor::WEIGHTS.cbegin(), m_reading.cbegin(), 4);
        }

    } else {

        m_distance = NO_TRACK;
    }

    t.stop();

    if (!s_run) {
        printf("1,2,3,4,5,6,time,m_distance\n");
        s_run = true;
    }
    for (const auto &reading : m_reading) {
        printf("%.5f,", reading);
    }
    // for (const auto &noise : m_noise) {
    //     printf("%.5f,", noise);
    // }
    printf("%lld,", t.elapsed_time().count() * 1);
    printf("%f\n", m_distance);

    t.reset();

    return m_distance;
}

void Sensor::toggle(const bool on) {

    if (on) {

        m_pins.write(0b111111);
        if (!s_run) {
            printf("On\n");
            s_run = true;
        }

    } else {

        m_pins.write(0);
        if (s_run) {
            printf("Off\n");
            s_run = false;
        }
    }

    wait_us(25);
}

float Sensor::getDistance() const {
    return m_distance;
}

void Sensor::calibrate_black() {

    std::array<float, 6> calibrate_data{};
    m_pins.write(0);
    wait_us(25);
    for (size_t i = 0; i < m_analog.size(); ++i) {

        // m_pins.write(1 << i);
        // wait_us(25);

        calibrate_data[i] = m_analog[i].read();
    }

    if (!s_run) {
        printf("1,2,3,4,5,6\n");
        s_run = true;
    }
    printf("%.5f,%.5f,%.5f,%.5f,%.5f,%.5f\n",
           calibrate_data[0], calibrate_data[1], calibrate_data[2], calibrate_data[3], calibrate_data[4], calibrate_data[5]);
}

void Sensor::calibrate_white() {

    // Turn all off to read m_noise
    m_pins.write(0);
    wait_us(25);
    for (size_t i = 0; i < m_analog.size(); ++i) {
        m_noise[i] = m_analog[i].read();
    }

    for (size_t i = 0; i < m_analog.size(); ++i) {

        m_pins.write(1 << i);
        wait_us(25);

        float adc_value = m_analog[i].read();

        auto isAboveTreshold = adc_value > m_noise[i] && adc_value > BLACK_TRESHOLD[i];
        if (isAboveTreshold) {
            m_reading[i] = (adc_value - m_noise[i]) * PRESCALER;
        } else {
            m_reading[i] = 0;
        }
    }

    if (!s_run) {
        printf("1,2,3,4,5,6\n");
        s_run = true;
    }
    printf("%.5f,%.5f,%.5f,%.5f,%.5f,%.5f\n",
           m_reading[0], m_reading[1], m_reading[2], m_reading[3], m_reading[4], m_reading[5]);
}
