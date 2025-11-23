#ifndef CONFIG_H
#define CONFIG_H

/**
 * @brief Configuration constants for ESP-Buggy robot
 * @details Centralizes all configuration values to eliminate magic numbers
 *          and improve maintainability. All values can be tuned here.
 */
namespace Config {

    // Serial Communication
    namespace Serial {
        constexpr unsigned int BAUD_RATE = 115200;
        constexpr unsigned int READ_BUFFER_SIZE = 128;
    }

    // Motor Control
    namespace Motor {
        constexpr float PWM_FREQUENCY = 1000.0f;
        constexpr float SLOW_DUTY_CYCLE = 0.3f;
        constexpr float HALF_DUTY_CYCLE = 0.5f;
        constexpr float FAST_DUTY_CYCLE = 0.6f;
        constexpr float CORRECTION_FACTOR = 1.05f;
    }

    // PID Control
    namespace PID {
        // Speed Controller Default Gains
        constexpr float SPEED_KP = 0.70f;
        constexpr float SPEED_KI = 0.0f;
        constexpr float SPEED_KD = 0.0f;

        // Line Following Controller Default Gains
        constexpr float LINE_KP = 1.2f;
        constexpr float LINE_KI = 0.0f;
        constexpr float LINE_KD = 0.0f;

        // Control Limits
        constexpr float LINE_MIN_LIMIT = -27.0f;
        constexpr float LINE_MAX_LIMIT = 27.0f;
        constexpr float PWM_MIN_LIMIT = 0.0f;
        constexpr float PWM_MAX_LIMIT = 1.0f;

        // Target Speed (PPS - Pulses Per Second)
        constexpr float DEFAULT_TARGET_SPEED = 2.0f;
    }

    // Sensor Configuration
    namespace Sensor {
        constexpr float PRESCALER = 10.0f;
        constexpr float NO_TRACK_VALUE = 30.0f;
        constexpr float WHITE_THRESHOLD = 0.8f;
        constexpr int CALIBRATION_ITERATIONS = 10000;
        constexpr unsigned int SENSOR_WAIT_US = 25;
    }

    // Line Following Behavior
    namespace LineFollow {
        constexpr int NO_TRACK_THRESHOLD = 150;
        constexpr unsigned int SENSOR_READ_INTERVAL_MS = 50;
        constexpr unsigned int MANUAL_MODE_REPORT_INTERVAL_MS = 400;
    }

    // Emergency Recovery
    namespace Recovery {
        constexpr double SEARCH_ANGLE_DEGREES = 45.0;
        constexpr float LINE_FOUND_THRESHOLD = 2.0f;
        constexpr double SEARCH_INCREMENT_DEGREES = 5.0;
        constexpr unsigned int RECOVERY_DELAY_MS = 1000;
    }

    // Timing
    namespace Timing {
        constexpr unsigned int THREAD_SLEEP_MS = 1;
    }

} // namespace Config

#endif // CONFIG_H
