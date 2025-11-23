#include "command_parser.h"
#include "debug1.h"
#include <cstdio>

CommandParser::CommandParser(BufferedSerial &serial, Motor &motor,
                             Encoder &wheel_left, Encoder &wheel_right,
                             Sensor &sensors, WheelControl &controller)
    : m_serial(serial), m_motor(motor), m_wheel_left(wheel_left),
      m_wheel_right(wheel_right), m_sensors(sensors), m_controller(controller) {}

bool CommandParser::isCommandAvailable() const {
    return m_serial.readable();
}

bool CommandParser::readChar(char &c) const {
    const ssize_t bytesRead = m_serial.read(&c, 1);
    return (bytesRead == 1);
}

bool CommandParser::readFloat(float &value) const {
    const ssize_t bytesRead = m_serial.read(&value, sizeof(value));
    return (bytesRead == static_cast<ssize_t>(sizeof(value)));
}

ssize_t CommandParser::writeString(const char *str) const {
    const size_t len = strlen(str);
    return m_serial.write(str, len);
}

CommandResult CommandParser::processCommand() {
    if (!isCommandAvailable()) {
        return CommandResult::CONTINUE;
    }

    char command;
    if (!readChar(command)) {
        return CommandResult::READ_ERROR;
    }

    switch (command) {
        case CommandCode::READ_SENSOR:
            return handleReadSensor();
        case CommandCode::START_LINE_FOLLOW:
            return handleStartLineFollow();
        case CommandCode::TUNE_PID:
            return handleTunePID();
        case CommandCode::SET_VELOCITY:
            return handleSetVelocity();
        case CommandCode::MANUAL_MODE:
            return handleManualMode();
        case CommandCode::CALIBRATE_WHITE:
            return handleCalibrateWhite();
        case CommandCode::CALIBRATE_BLACK:
            return handleCalibrateBlack();
        default:
            if (std::isalnum(command)) {
                printf("HM10 sent %c\n", command);
            }
            return CommandResult::INVALID_COMMAND;
    }
}

CommandResult CommandParser::handleReadSensor() {
    while (true) {
        const auto distance = m_sensors.read();

        if (distance != Config::Sensor::NO_TRACK_VALUE) {
            char buffer[16];
            snprintf(buffer, sizeof(buffer), "%2.5f", distance);
            writeString(buffer);
        } else {
            writeString("No Track");
        }

        if (isCommandAvailable()) {
            char c;
            if (readChar(c) && c == CommandCode::READ_SENSOR) {
                break;
            }
        }

        ThisThread::sleep_for(std::chrono::milliseconds(Config::LineFollow::SENSOR_READ_INTERVAL_MS));
    }
    return CommandResult::EXIT_MODE;
}

CommandResult CommandParser::handleStartLineFollow() {
    m_motor.setEnable(1);
    m_motor.setFrequency(Config::Motor::PWM_FREQUENCY);

    while (true) {
        const auto distance = m_sensors.read();
        DEBUGLOG("Position: %.5f\n", distance);

        if (distance != Config::Sensor::NO_TRACK_VALUE) {
            const auto compute_value = m_controller.computeSpeed(distance, m_wheel_left, m_wheel_right);
            m_motor.setDirection('L', compute_value[0].first);
            m_motor.setDutycycle('L', compute_value[0].second);
            m_motor.setDirection('R', compute_value[1].first);
            m_motor.setDutycycle('R', compute_value[1].second);

            DEBUGLOG("Motor output: [%d,%.5f] [%d,%.5f]\n",
                     compute_value[0].first, compute_value[0].second,
                     compute_value[1].first, compute_value[1].second);
        } else {
            DEBUGLOG("Counter : %d\n", m_sensors.getNoTrackCounter());
            if (m_sensors.getNoTrackCounter() > Config::LineFollow::NO_TRACK_THRESHOLD) {
                m_motor.setDutycycle('A', 0.0f);
                break;
            }
        }

        // Check for stop command or emergency recovery
        if (isCommandAvailable()) {
            char c;
            if (!readChar(c)) {
                continue;
            }

            if (c == CommandCode::START_LINE_FOLLOW) {
                m_motor.setEnable(0);
                m_motor.setDutycycle('A', 0.0f);
                break;
            } else if (c == CommandCode::EMERGENCY_RECOVERY) {
                // Emergency track recovery
                m_motor.setEnable(0);
                ThisThread::sleep_for(std::chrono::milliseconds(Config::Recovery::RECOVERY_DELAY_MS));
                Motor::turnright(Config::Recovery::SEARCH_ANGLE_DEGREES, &m_motor,
                                &m_wheel_left, &m_wheel_right);

                // Search for line
                while (true) {
                    const auto _distance = m_sensors.read();
                    if (_distance < Config::Recovery::LINE_FOUND_THRESHOLD) {
                        break;
                    }
                    Motor::turnright(Config::Recovery::SEARCH_INCREMENT_DEGREES, &m_motor,
                                    &m_wheel_left, &m_wheel_right);
                }

                m_controller.reset();
                m_motor.setDutycycle('A', 0.0f);
                m_motor.setEnable(1);
            }
        }
    }
    return CommandResult::EXIT_MODE;
}

CommandResult CommandParser::handleTunePID() {
    m_motor.setEnable(0);

    float kp, ki, kd;
    if (!readFloat(kp) || !readFloat(ki) || !readFloat(kd)) {
        return CommandResult::READ_ERROR;
    }

    // Validate PID parameters (basic sanity check)
    if (kp < 0.0f || ki < 0.0f || kd < 0.0f) {
        DEBUGLOG("Invalid PID values: Kp: %.5f Ki: %.5f Kd: %.5f\n", kp, ki, kd);
        return CommandResult::INVALID_COMMAND;
    }

    DEBUGLOG("Kp: %.5f Ki: %.5f Kd: %.5f\n", kp, ki, kd);
    m_controller.setSpeedController(kp, ki, kd);

    return CommandResult::SUCCESS;
}

CommandResult CommandParser::handleSetVelocity() {
    m_motor.setEnable(0);

    float speed;
    if (!readFloat(speed)) {
        return CommandResult::READ_ERROR;
    }

    // Validate speed (basic sanity check)
    if (speed < 0.0f || speed > 100.0f) {
        DEBUGLOG("Invalid target speed: %.5f\n", speed);
        return CommandResult::INVALID_COMMAND;
    }

    DEBUGLOG("Target speed: %.5f\n", speed);
    m_controller.setTargetSpeed(speed);

    return CommandResult::SUCCESS;
}

CommandResult CommandParser::handleManualMode() {
    m_motor.setEnable(1);
    m_motor.setFrequency(Config::Motor::PWM_FREQUENCY);
    m_motor.setDirection('R', 0);
    m_motor.setDutycycle('R', 1.0f);
    m_motor.setDirection('L', 1);
    m_motor.setDutycycle('L', 1.0f);

    // Manual mode runs indefinitely until interrupted
    while (true) {
        DEBUGLOG("%.5f %.5f\n", m_wheel_left.getPPS(), m_wheel_right.getPPS());
        ThisThread::sleep_for(std::chrono::milliseconds(Config::LineFollow::MANUAL_MODE_REPORT_INTERVAL_MS));
    }

    return CommandResult::EXIT_MODE;
}

CommandResult CommandParser::handleCalibrateWhite() {
    for (int i = 0; i < Config::Sensor::CALIBRATION_ITERATIONS; ++i) {
        m_sensors.calibrateWhite();
    }
    return CommandResult::SUCCESS;
}

CommandResult CommandParser::handleCalibrateBlack() {
    for (int i = 0; i < Config::Sensor::CALIBRATION_ITERATIONS; ++i) {
        m_sensors.calibrateBlack();
    }
    return CommandResult::SUCCESS;
}
