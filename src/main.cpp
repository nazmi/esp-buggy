#include "command_parser.h"
#include "config.h"
#include "debug1.h"
#include "encoder.h"
#include "helper.h"
#include "mbed.h"
#include "motor.h"
#include "sensor.h"
#include "wheelcontrol.h"

/**
 * @brief Main entry point for ESP-Buggy line following robot
 * @details Initializes hardware, configures controllers, and processes BLE commands
 * @return 0 on normal exit (unreachable in embedded context)
 */
int main() {
    // Initialize hardware components
    Motor motor(PB_8, PC_9, PC_6, PC_8, PB_9);
    Encoder wheel_left(PC_3, PC_2);
    Encoder wheel_right(PB_14, PB_13);
    wheel_left.start();
    wheel_right.start();

    BufferedSerial hm10(PA_11, PA_12, Config::Serial::BAUD_RATE);
    Sensor sensors(D2, D3, D4, D5, D6, D8, A5, A4, A3, A2, A1, A0);
    DigitalOut analog_1(PC_5, 0), analog_2(PB_1, 0), analog_3(PC_4, 0);

    // Configure PID controllers with default values
    WheelControl controller;
    controller.setSpeedController(Config::PID::SPEED_KP, Config::PID::SPEED_KI, Config::PID::SPEED_KD);
    controller.setLineController(Config::PID::LINE_KP, Config::PID::LINE_KI, Config::PID::LINE_KD);
    controller.setLineLimits(Config::PID::LINE_MIN_LIMIT, Config::PID::LINE_MAX_LIMIT);
    controller.setPWMLimits(Config::PID::PWM_MIN_LIMIT, Config::PID::PWM_MAX_LIMIT);
    controller.setTargetSpeed(Config::PID::DEFAULT_TARGET_SPEED);

    // Create command parser for BLE interface
    CommandParser commandParser(hm10, motor, wheel_left, wheel_right, sensors, controller);

    // Main event loop - process commands as they arrive
    while (true) {
        const CommandResult result = commandParser.processCommand();

        // Handle command results
        switch (result) {
            case CommandResult::READ_ERROR:
                DEBUGLOG("Error reading from BLE serial\n");
                break;
            case CommandResult::INVALID_COMMAND:
                DEBUGLOG("Invalid command received\n");
                break;
            case CommandResult::SUCCESS:
            case CommandResult::EXIT_MODE:
            case CommandResult::CONTINUE:
            default:
                break;
        }

        // Reset debug flag after command processing
        sensors.s_run = false;

        // Small delay to prevent CPU hogging
        ThisThread::sleep_for(std::chrono::milliseconds(Config::Timing::THREAD_SLEEP_MS));
    }

    return 0;
}
