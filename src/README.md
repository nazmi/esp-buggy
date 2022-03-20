# Embedded Systems Project 2021/2022

## Line Following Robot (LFR)
![Doxygen Page](https://github.com/nazmi/esp-buggy/actions/workflows/publish.yml/badge.svg) 
![Build GCC_ARM](https://github.com/nazmi/esp-buggy/actions/workflows/build.yml/badge.svg)  
Hi! I'm [Nazmi Ropi](https://uk.linkedin.com/in/nazmi-ropi), the author of this software documentation. One of the objectives of this year's project is to produce a high average speed line following buggy without compromising its robustness. Also, my approach towards software development is to implement Object-Oriented-Programming features to abstract any technical details to the user. Details of technical demonstrations were also documented on [its page](#journey).

## Dependencies

[**Mbed OS 6.15**](https://os.mbed.com/mbed-os/releases/)   
[**QEI**](https://os.mbed.com/users/WiredHome/code/QEI/) by [Aaron Berk](https://os.mbed.com/users/aberk/)  
[**PID**](https://os.mbed.com) by [Aaron Berk](https://os.mbed.com)  

## Installation
1. Install the latest [Mbed Studio](https://os.mbed.com/studio/) for your compatible platform.
2.  Clone this repository. 
	 ```console
	git clone https://github.com/nazmi/esp-buggy
	```
@warning This will not work because the repository is private.

3. Compile *main.cpp* using Mbed Studio.
4. Upload the binary file to NUCLEO-F401RE through USB.

@note You need to replace some of the MACROS values, and PID parameters based on your specifications.

## Troubleshooting
Some of the libraries might not work if you want to import and use them on Mbed OS 6, I have made modifications to some of them to replace the deprecated functions from Mbed 2. This can be easily done by referring to the latest API list from  [Mbed OS 6 documentation](https://os.mbed.com/docs/mbed-os/v6.15/introduction/index.html).

@note You can use the joystick, potentiometer and LCD on the application shield to help debug your program.

## Components
You can buy any component you like, but the table below will help if you want to build the robot without thinking about the selection of the components.

|Component|Quantity
--|--
|NUCLEO-F401RE| 1|
|RS Pro DC %Motor|2|
|Broadcom Magnetic Incremental %Encoder| 2
|%Motor Driver Board|1|
|TCRT5000| 6|
|HM 10 BLE|1|

## Contributors
Special thanks to my supervisor and every group member that helped in this project (in no particular order).

*Dr Subhasish Chakraborty*  
*Ibrahim, Euan, Joseph, Zech, Eric*
