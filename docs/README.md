# Embedded Systems Project 2021/2022 {#mainpage}

## Line Following Robot (LFR) 🚓 {#intro}

[![Build GCC_ARM](https://github.com/nazmi/esp-buggy/actions/workflows/build.yml/badge.svg)](https://github.com/nazmi/esp-buggy/actions/workflows/build.yml)
[![Style Check](https://github.com/nazmi/esp-buggy/actions/workflows/linter.yml/badge.svg)](https://github.com/nazmi/esp-buggy/actions/workflows/linter.yml)
[![C++ Syntax Check](https://github.com/nazmi/esp-buggy/actions/workflows/cppcheck.yml/badge.svg)](https://github.com/nazmi/esp-buggy/actions/workflows/cppcheck.yml)
[![Doxygen Action](https://github.com/nazmi/esp-buggy/actions/workflows/publish.yml/badge.svg)](https://github.com/nazmi/esp-buggy/actions/workflows/publish.yml)  
Hi! I'm [Nazmi Ropi](https://uk.linkedin.com/in/nazmi-ropi), the author of this software documentation. One of the objectives of this year's project is to produce a high average speed line following buggy without compromising its robustness. Also, my approach towards software development is to implement Object-Oriented-Programming features to abstract any technical details to the user. Details of technical demonstrations were also documented on [its page](#journey).

## Dependencies 📌 {#dependencies}

[**Mbed OS 6.15.1**](https://github.com/ARMmbed/mbed-os/releases/tag/mbed-os-6.15.1)  
[**QEI**](https://os.mbed.com/users/WiredHome/code/QEI/) by [David Smart](https://os.mbed.com/users/WiredHome/)   and [Aaron Berk](https://os.mbed.com/users/aberk/)  
[**PID**](https://os.mbed.com/users/aberk/code/PID/) by [Aaron Berk](https://os.mbed.com/users/aberk/)  

## Installation 📋 {#installation}

1. Install the latest [Mbed Studio](https://os.mbed.com/studio/) or use the online [Arm Keil Studio](https://studio.keil.arm.com/).
2. Import this repository.

   ```bash
   git clone https://github.com/nazmi/esp-buggy
   ```

3. Compile *main.cpp* using the compiler.
4. Upload the binary file to NUCLEO-F401RE through USB.

@note You need to replace some of the MACROS values, and PID parameters based on your specifications.

## Run on Docker 🐋 {#docker}

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

@note Attach to the running container in VSCode, navigate to ```/root/esp-buggy``` and start developing.

## Troubleshooting 🩺 {#troubleshoot}

1. Mbed Studio 1.4.3 has linter problems when using GCC_ARM, but it can compile the program. Use MbedStudio 1.4.1 or compile with ARMC6 toolchain to solve this problem.
2. Some of the libraries might not work if you want to import and use them on Mbed OS 6, I have made modifications to some of them to replace the deprecated functions from Mbed 2. This can be easily done by referring to the latest API list from  [Mbed OS 6 documentation](https://os.mbed.com/docs/mbed-os/v6.15/introduction/index.html).

@note You can use the joystick, potentiometer and LCD on the application shield to help debug your program.

## Components 🛒 {#components}

You can buy any component you like, but the table below will help if you want to build the robot without thinking about the selection of the parts.

|Component|Quantity
--|--
|[NUCLEO-F401RE](https://uk.farnell.com/stmicroelectronics/nucleo-f401re/nucleo-board-mcu/dp/2394223)| 1|
|[RS Pro DC %Motor](https://uk.rs-online.com/web/p/dc-motors/2389737/)|2|
|[Broadcom Magnetic Incremental %Encoder](https://uk.farnell.com/broadcom-limited/aeat-601b-f06/encoder-incremental-3-ch-12000rpm/dp/2467469)| 2
|[%Motor Driver Board](https://uk.farnell.com/maxim-integrated-products/maxrefdes89/ref-design-board-dc-motor-driver/dp/2523609)|1|
|[TCRT5000](https://www.robotshop.com/en/aptinex-raykha-s8-tcrt5000-based-8-channel-reflective-sensor-array.html)| 1|
|[HM-10 BLE](https://www.robotshop.com/uk/sunfounder-bluetooth-40-hm-10-master-slave-module.html)|1|

## Contributing 👨‍💻 {#contribute}

Contributors are welcome. Please create a [pull request](https://github.com/nazmi/esp-buggy/pulls) and raise any [issues](https://github.com/nazmi/esp-buggy/issues) to discuss with.  

## Contributors 👷‍♂️ {#contributors}

Special thanks to my supervisor and every group member that helped in this project (in no particular order).

*Dr Subhasish Chakraborty*  
*Ibrahim, Euan, Joseph, Zech, Eric*

## Credits 💚 {#credits}

This documentation theme is from [doxygen-awesome-css](https://github.com/jothepro/doxygen-awesome-css).
