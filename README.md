# Embedded Systems Inha University <img src="https://upload.wikimedia.org/wikipedia/en/b/be/InhaUniversityEmblem.png" width="90" title="Inha Logo">


This project will provide all information on how to setup the STM32L476VG Microcontroller used during the
lab sessions in combination with the [Keil IDE](https://www.keil.com/demo/eval/arm.htm).
Furthermore all Lab Tasks will be solved and improved with some comments for better understanding.

## Keil

Download & Install Keil according to the websie instructions.
Afterwards search in the board tab for:
```
476vg
```
and update the following packages on the right side:
* Keil::STM32L4xx_DFP 
*  ARM::CMSIS 
* ARM::CMSIS-Driver 
* Keil::MDK-Middleware

After installing the USB-driver in order to debug the microcontroller must be applied.
In the default configuration it can be found at the following location:
```
C:\Keil_v5\ARM\STLink\USBDriver\dpinst_amd64.exe
```
Afterwards the Keil µVision IDE can be used.
Create a new project µVision Project. Search for the used microcontroller.
Afterwards add following configuration:
* **CMSIS**: Core
* **Device**: Startup

Finally go to *Options for Target*.
There switch to the *Debug* Tab and change the debugger to **ST-Link Debugger**
Afterwards click on *settings* and change the target clock to *4 MHz*

## Install Project
In order to use this project simply clone it and afterwards open the project files in the Keil IDE
```
git clone https://github.com/Klimm-Max/EmbeddedSystems.git
```

## Lab Sessions
The lab sessions will tackle different problems and introduce basic concepts in programming with embedded systems

### Lab 1
This lab focusses on the basic concepts for working with embedded systems.
It will use *GPIO*-Pins to enable the 2 LEDs on the board.
It will implement a *Delay*-function using the SysTick-Interrupt in order to controll the LEDs even more.
Furthermore it increases the clock-speed of the microcontroller up to 80MHz using HSI

### Lab 2
This lab will introduce the *USART2*-technologie in order to transmit between the PC and the microcontroller.
Furthermore it introduces the *GPIO* for using the onboard Joystick. These can be used to either enable or disable both LEDs independently using the direction of the joystick.
* Left 		= Enable Red LED
* Up 		= Disable Red LED
* Right 	= Enable Green LED
* Down	 = Disable Green LED
Additionaly both LEDs can be disabled by pressing the joystick.
##### Task 1
Write a program that will print the direction of the joystick into the *putty*-Terminal e.g. *u* when the joystick towards the top is pressed
##### Task 2
Write a program that will replace the basic functionality of lab2 by using *USART2*.
Instead of pressing the joystick, the *putty*-Terminal should be used to write the command and then be processed on the board to either enable or disable the LEDs.
It can be improved even more by accepting string e.g. *down* instead of just a single *d*

### Lab 3