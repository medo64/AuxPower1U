# AuxPower1U: MainController

Features:
* 5 DC inputs (input 1 has to be 9-15V)
* 5 DC outputs
* voltage and current monitoring
* temperature monitoring
* OLED output


## Usage

Different functionality can be triggered by different key press lengths:
* <3 seconds:    detail mode for channel (turns off automatically in 60 seconds)
* 3-10 seconds:  reset once button is released (or turns on if output was turned off)
* 10-15 seconds: turn off output permanently
* >15 seconds:   ignore press


## Test Mode

To enter test mode, keep buttons 1 and 5 pressed while powering board on. Each
subsequent press will then cycle through channel and show voltage/current values
in higher than normal resolution.
