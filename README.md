# AuxPower1U

This project is 1U enclosed case with 5 DC outputs that are separately
controllable via front momentary switches with LEDs.

Features:
* 3 power supplies
* 5 separate button controls
* ability to monitor voltage and current
* 2 power inputs (one for UPS, one for non-UPS)


## Components

* [DellTrigger](src/DellTrigger/README.md): Emulating Dell charger for 5.5x2.1mm input
* [FanController](src/FanController/README.md): Optional board to run PWM fans dependant on temperature
* [MainController](src/MainController/README.md): Allowing for monitoring and resetting of lines
