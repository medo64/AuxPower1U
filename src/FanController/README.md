# AuxPower1U: FanController

Allows for PWM control of 4 fans based on temperature.

Ranges:
* < 40 °C: 10% PWM speed
* 40-60 °C: 10-50% PWM speed (linear increase)
* 60-70 °C: 50% PWM speed
* >70 °C: 100% PWM speed
