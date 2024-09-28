I haven't tried to build myself a fan controller, I swear. My original plan
was to get one of many PC fan boards and call it a day. However, I was surprised
at how little of those fan controllers could properly work without PC. Even
worse, most of their fan profiles would cause my 1U fans to scream. So, making my
own became easier solution than figuring how to make the existing ones work.

First, about the fans. I had [1U case](https://www.digikey.com/en/products/detail/bud-industries/PRM-14460/655427)
that was made of quite thick plastic. That left only 36.576 millimeters to work
with. If we round down this overly precise number in order to account for some
tolerances, the biggest fan I can place here is 36x36 mm in size. And that's
definitely not a standard size you'll find in your desktop computer. However,
it's standard enough for there to be multiple fans of that size. I opted to got
with [Delta FFB03612EHN](https://www.delta-fan.com/FFB03612EHNYCL.html)
primarily due to its 36x36x28 mm dimensions that would just fit into my case.
But my second concern was not less valuable - it was a PWM controlled fan.

When it comes to fans, you have 3-pin ones that always run at the same speed and
the only way to slow them down is to lower the voltage. This wasn't what I was
interested in. I wanted 4-pin fans that have PWM signal for speed control. And
my Deltas were just such fan.

To control pretty much any fan speed, you need to have a 25kHz PWM signal. There
is quite a bit of latitude allowed (for Delta fans, it's usually 20-30 kHz) but
different fans have different tolerances, so keeping it as close to 25 kHz is
probably the best idea. Fan speed itself is not controlled by PWM frequency but
by duty cycle. If the duty cycle is 50%, you run fan at 50%; if the duty cycle
is 25%, fan runs at 25%; if the duty cycle is 75%, you get the drill...

[Officially](https://www.intel.com/content/dam/support/us/en/documents/intel-nuc/intel-4wire-pwm-fans-specs.pdf),
a duty cycle control needs to be done by an open-drain output. That is, you only
ever pull the PWM signal down, letting it go to the fan's internal 5V pull-up
for the rest of a PWM cycle.

Keeping this in mind, I went to search for a microcontroller I could use for the
project. It had to be low-pin count, so I don't waste a lot of PCB space; it had
to have hardware PWM, so I don't need to bit-bang; it had to have open-drain PWM
output; it had to have an analog input for temperature; and lastly, I had to
have one available in my drawers.

Looking at Microchip PICs I had available, I quite quickly went toward the
[PIC12F1501](https://www.microchip.com/en-us/product/pic12f1501). This one is a
little gem. It has not one, but four PWM channels thus allowing me to control 4
fans in a completely independent manner. It also has 4 ADC channels (some pins
overlap) thus allowing for an external temperature sensor (I had MCP9701A lying
around). It even has an internal temperature sensor which is the fact I noticed
only once I already had my PCBs made, so I ended up not using it.

But you will notice it didn't have one thing I needed - an open-drain PWM
output. However, since the pull-up value within the fan is specified to be
a maximum of 5.25 V, I was reasonably sure I could ignore that. Any voltage
differential would be small and thus any extra current going through pull-up
resistor would be way lower than the current it already had to carry during its
"off" cycle when it gets connected directly to GND. In short, I was willing to
ignore this part of the specification.

After messing around in KiCAD for a while, I had my design finished and it was
off to my sponsor, [PCBWay](https://www.pcbway.com), to manufacture the PCBs.
But let's continue that part of fan controller story in the next post.
