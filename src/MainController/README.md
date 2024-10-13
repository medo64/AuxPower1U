# AuxPower1U: MainController

Features:
* 5 DC inputs (input 1 has to be 9-15V)
* 5 DC outputs
* voltage and current monitoring (9-60V, 0-8A, per channel)
* temperature monitoring
* OLED output


## Usage

Different functionality can be triggered by different key press lengths:
* <5 seconds:    detail mode for channel (turns off automatically in 60 seconds)
* 5-10 seconds:  reset once button is released (or turns on if output was turned off)
* 10-13 seconds: turn off output permanently
* >13 seconds:   ignore press


## Uart Communication

Current usage will be written to UART twice a second. See example below:

    1: 11.98V 0.40A   4.77W [ON]
    2: 15.40V 3.10A  47.74W [ON]
    3: 15.42V 0.00A   0.00W [OFF]
    4: 15.42V 0.00A   0.00W [OFF]
    5: 54.13V 0.00A   0.00W [OFF]
    T: 23.4C

Each line consists of the space-separated readings. If line starts with a
channel number (1-5), what follows will be readings for the given channel of
voltage (V), current (A), and power (W) followed by a channel status. If line
starts with a letter T, what follows is a temperature. Each line will end with
`CRLF`.

Commands can be given to device by sending command code followed by a channel.
Available commands are:
* `D`: Disable
* `E`: Enable
* `R`: Reset
* `I`: Information

For example, to disable channel 2 use:

    D2


## Test Mode

To enter test mode, keep buttons 1 and 5 pressed while powering board on. Each
subsequent press will then cycle through channel and show voltage/current values
in higher than normal resolution.
