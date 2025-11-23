#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include "config.h"
#include "encoder.h"
#include "mbed.h"
#include "motor.h"
#include "sensor.h"
#include "wheelcontrol.h"
#include <cctype>
#include <cstring>

/**
 * @brief Command codes received from BLE interface
 */
namespace CommandCode {
    constexpr char READ_SENSOR = 'R';        ///< Read and report sensor values
    constexpr char START_LINE_FOLLOW = 'S';  ///< Start line following mode
    constexpr char TUNE_PID = 'T';           ///< Tune PID speed controller
    constexpr char SET_VELOCITY = 'V';       ///< Set target velocity
    constexpr char MANUAL_MODE = 'M';        ///< Manual motor control mode
    constexpr char CALIBRATE_WHITE = 'W';    ///< Calibrate sensors on white surface
    constexpr char CALIBRATE_BLACK = 'B';    ///< Calibrate sensors on black surface
    constexpr char EMERGENCY_RECOVERY = 'Q'; ///< Emergency track recovery
}

/**
 * @brief Result status from command execution
 */
enum class CommandResult {
    SUCCESS,           ///< Command executed successfully
    CONTINUE,          ///< Continue in current mode
    EXIT_MODE,         ///< Exit current mode
    INVALID_COMMAND,   ///< Command not recognized
    READ_ERROR         ///< Error reading from serial
};

/**
 * @brief Handles parsing and execution of BLE commands
 * @details Separates command handling logic from main loop,
 *          improving testability and maintainability
 */
class CommandParser {
  private:
    BufferedSerial &m_serial;
    Motor &m_motor;
    Encoder &m_wheel_left;
    Encoder &m_wheel_right;
    Sensor &m_sensors;
    WheelControl &m_controller;

    /**
     * @brief Read a single character from serial with validation
     * @param[out] c Character read
     * @return true if read successful, false otherwise
     */
    bool readChar(char &c) const;

    /**
     * @brief Read a float value from serial with validation
     * @param[out] value Float value read
     * @return true if read successful, false otherwise
     */
    bool readFloat(float &value) const;

    /**
     * @brief Write a null-terminated string to serial
     * @param str String to write
     * @return Number of bytes written
     */
    ssize_t writeString(const char *str) const;

    // Command handlers
    CommandResult handleReadSensor();
    CommandResult handleStartLineFollow();
    CommandResult handleTunePID();
    CommandResult handleSetVelocity();
    CommandResult handleManualMode();
    CommandResult handleCalibrateWhite();
    CommandResult handleCalibrateBlack();

  public:
    /**
     * @brief Constructor
     * @param serial Serial interface for BLE communication
     * @param motor Motor controller instance
     * @param wheel_left Left wheel encoder
     * @param wheel_right Right wheel encoder
     * @param sensors Sensor array instance
     * @param controller Wheel control (PID) instance
     */
    CommandParser(BufferedSerial &serial, Motor &motor,
                  Encoder &wheel_left, Encoder &wheel_right,
                  Sensor &sensors, WheelControl &controller);

    /**
     * @brief Check if a command is available and execute it
     * @return CommandResult indicating execution status
     */
    CommandResult processCommand();

    /**
     * @brief Check if serial data is available
     * @return true if data can be read
     */
    bool isCommandAvailable() const;
};

#endif // COMMAND_PARSER_H
