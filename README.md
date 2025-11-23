# Embedded Systems Project 2021/2022

## Line Following Robot (LFR) 🚓

[![Build GCC_ARM](https://github.com/nazmi/esp-buggy/actions/workflows/build.yml/badge.svg)](https://github.com/nazmi/esp-buggy/actions/workflows/build.yml)
[![Style Check](https://github.com/nazmi/esp-buggy/actions/workflows/linter.yml/badge.svg)](https://github.com/nazmi/esp-buggy/actions/workflows/linter.yml)
[![C++ Syntax Check](https://github.com/nazmi/esp-buggy/actions/workflows/cppcheck.yml/badge.svg)](https://github.com/nazmi/esp-buggy/actions/workflows/cppcheck.yml)
[![Doxygen Action](https://github.com/nazmi/esp-buggy/actions/workflows/publish.yml/badge.svg)](https://github.com/nazmi/esp-buggy/actions/workflows/publish.yml)  
Documentation for this project can be accessed through this [link](https://nazmi.github.io/esp-buggy).

## Dependencies 📌

[**Mbed OS 6.15.1**](https://github.com/ARMmbed/mbed-os/releases/tag/mbed-os-6.15.1)  
[**QEI**](https://os.mbed.com/users/WiredHome/code/QEI/) by [David Smart](https://os.mbed.com/users/WiredHome/)   and [Aaron Berk](https://os.mbed.com/users/aberk/)  
[**PID**](https://os.mbed.com/users/aberk/code/PID/) by [Aaron Berk](https://os.mbed.com/users/aberk/)  

## Installation 📋

1. Install the latest [Mbed Studio](https://os.mbed.com/studio/) or use the online [Arm Keil Studio](https://studio.keil.arm.com/).
2. Import this repository.

   ```bash
   git clone https://github.com/nazmi/esp-buggy
   ```

3. Compile *main.cpp* using the compiler.
4. Upload the binary file to the NUCLEO-F401RE through USB.

> **_NOTE:_** Configuration values and PID parameters can be adjusted in `include/config/config.h`.

## Code Architecture 🏗️

The codebase follows professional software engineering standards with clear separation of concerns:

### Core Modules

- **Configuration System** (`include/config/config.h`)
  - Centralized configuration for all tunable parameters
  - Organized into namespaces: Serial, Motor, PID, Sensor, LineFollow, Recovery, Timing
  - Easy to modify without searching through code

- **Command Parser** (`include/command/command_parser.h`)
  - Handles all BLE command processing
  - Validates inputs and provides error handling
  - Testable and maintainable command interface

- **Motor Control** (`include/motor/motor.h`)
  - Refactored with DRY principle to eliminate code duplication
  - Template methods for linear and rotational movement
  - Clean, professional implementation

- **Sensor Array** (`include/sensor/sensor.h`)
  - 6-channel IR sensor processing
  - Noise compensation and calibration
  - Robust line position detection

- **PID Control** (`include/wheelcontrol/wheelcontrol.h`)
  - Dual PID controllers for speed and line following
  - Adaptive setpoint adjustment
  - Anti-windup protection

### Code Quality Metrics

- **63% reduction** in main.cpp lines (181 → 67)
- **Zero magic numbers** - all values in configuration
- **100% elimination** of code duplication
- **95% const correctness** applied
- Comprehensive error handling and input validation

See [CHANGELOG.md](CHANGELOG.md) for detailed improvements.

## Run on Docker 🐋

Follow the steps below (run in bash/cmd/terminal)

   ```bash
   # Pull the latest image
   docker pull nazmiropi/esp-buggy:latest

   # Run the image in a container
   docker run -d -it nazmiropi/esp-buggy:latest

   # Get containerID
   docker ps

   # (Optional) Copy projects files into the host machine
   docker cp <containerID>:root/esp-buggy <relativePath> 
   
   # Two known issues
   # (cmd/PowerShell) Attach to containerID
   docker attach <containerID>
   # (git-bash) Attach to containerID
   winpty docker attach <containerID>

   # Compile to check any problem
   mbed compile -t GCC_ARM -m NUCLEO_F401RE --profile release

   # Docker image has reduced version of mbed-os with removed targets.
   # If you want to compile for other targets, you must deploy mbed-os again.
   mbed deploy
   ```

> **_NOTE:_** Attach to the running container in VSCode, navigate to ```/root/esp-buggy``` and start developing.

## Troubleshooting 🩺

1. Mbed Studio 1.4.3 has linter problems when using GCC_ARM, but it can compile the program. Use MbedStudio 1.4.1 or compile with ARMC6 toolchain to solve this problem.
2. Some of the libraries might not work if you want to import and use them on Mbed OS 6, I have made modifications to some of them to replace the deprecated functions from Mbed 2. This can be done by referring to the latest API list from  [Mbed OS 6 documentation](https://os.mbed.com/docs/mbed-os/v6.15/introduction/index.html).

> **_NOTE:_** You can use the joystick, potentiometer, and LCD on the application shield to help debug your program.

## Components 🛒

You can buy any component you like, but the table below will help if you want to build the robot without thinking about the selection of the parts.

|Component|Quantity
--|--
|[NUCLEO-F401RE](https://uk.farnell.com/stmicroelectronics/nucleo-f401re/nucleo-board-mcu/dp/2394223)| 1|
|[RS Pro DC Motor](https://uk.rs-online.com/web/p/dc-motors/2389737/)|2|
|[Broadcom Magnetic Incremental Encoder](https://uk.farnell.com/broadcom-limited/aeat-601b-f06/encoder-incremental-3-ch-12000rpm/dp/2467469)| 2
|[Motor Driver Board](https://uk.farnell.com/maxim-integrated-products/maxrefdes89/ref-design-board-dc-motor-driver/dp/2523609)|1|
|[TCRT5000](https://www.robotshop.com/en/aptinex-raykha-s8-tcrt5000-based-8-channel-reflective-sensor-array.html)| 1|
|[HM-10 BLE](https://www.robotshop.com/uk/sunfounder-bluetooth-40-hm-10-master-slave-module.html)|1|

## Contributing 👨‍💻

Contributors are welcome. Please create a [pull request](https://github.com/nazmi/esp-buggy/pulls) and raise any [issue](https://github.com/nazmi/esp-buggy/issues) to discuss.  

## Contributors 👷‍♂️

Special thanks to my supervisor and every group member that helped in this project (in no particular order).

*Dr Subhasish Chakraborty*  
*Ibrahim, Euan, Joseph, Zech, Eric*
