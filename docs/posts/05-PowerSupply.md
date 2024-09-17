When it comes to power supplies for embedded electronics, it's really hard to
beat Meanwell. They are easily available, they have a wide selection, and they are
reasonably priced. Thus, my search for power supply started with [the first Meanwell catalog I found](https://www.meanwell.com/Upload/PDF/catalog_s.pdf).

After testing the [Dell trigger](/2024/09/triggering-dell/) board, my power
supply setup crystallized to 48/55V (for my routers), 15/20V for computers, and
lastly (optionally) 12V for a modem. Why do I say "optionally"? Well, the option
of using a buck regulator for the modem is always open. Since I have my case
dimensions [predetermined](/2024/09/auxpower1u-case-selection/), the choice on
whether to use 2 or 3 power supplies will be mostly driven by their dimensions.

As 1U severely restricts the height, my choice fell onto four power supply
families:
* [LRS-100-XX](https://www.meanwell.com/webapp/product/search.aspx?prod=LRS-100)
* [RSP-150-XX](https://www.meanwell.com/webapp/product/search.aspx?prod=RSP-150)
* [RSP-320-XX](https://www.meanwell.com/webapp/product/search.aspx?prod=RSP-320)
* [UHP-200-XX](https://www.meanwell.com/webapp/product/search.aspx?prod=UHP-200(R))

I wanted to get as small as possible (can I fit 3?), a reasonable amount of
power at each voltage (100W+, especially for 15V needed by computers), a
reasonably small ripple (less than 200mA, if possible), tight voltage tolerance
(1%, ideally), high efficiency (90%+ desired), and lastly robust overload controls
(ideally with auto-recovery). Looking at the catalog, I placed the following
power supplies on the short list:

| Model      | Output       | Power | Tol | Ripple | Eff | Dimensions     | Input      | C Overload  | V Overload  | PFC | Fan | Cost |
|------------|--------------|-------|-----|--------|-----|----------------|------------|-------------|-------------|-----|-----|------|
| LRS-35-12  | 12 V   3.0 A |  35 W | ±1% | 120 mV | 86% |  99 x  82 x 30 | 85-264 Vac | Y (auto)    | Y (repower) |  N  |  N  |  $13 |
| LRS-50-12  | 12 V   4.2 A |  50 W | ±1% | 120 mV | 86% |  99 x  82 x 30 | 85-264 Vac | Y (auto)    | Y (repower) |  N  |  N  |  $14 |
| LRS-100-15 | 15 V   7.0 A | 105 W | ±1% | 120 mV | 90% | 129 x  97 x 30 | 85-264 Vac | Y (auto)    | Y (repower) |  N  |  N  |  $17 |
| LRS-100-48 | 48 V   2.3 A | 100 W | ±1% | 200 mV | 91% | 129 x  97 x 30 | 85-264 Vac | Y (auto)    | Y (repower) |  N  |  N  |  $19 |
| LRS-150-24 | 24 V   6.5 A | 150 W | ±1% | 200 mV | 89% | 159 x  97 x 30 | 85-264 Vac | Y (auto)    | Y (repower) |  N  |  N  |  $19 |
| LRS-150-48 | 48 V   3.3 A | 150 W | ±1% | 200 mV | 90% | 159 x  97 x 30 | 85-264 Vac | Y (auto)    | Y (repower) |  N  |  N  |  $25 |
| RS-50-12   | 12 V   4.2 A |  50 W | ±1% | 120 mV | 84% |  99 x  97 x 36 | 88-264 Vac | Y (auto)    | Y (auto)    |  N  |  N  |  $19 |
| RSP-150-24 | 24 V   6.3 A | 150 W | ±1% | 150 mV | 89% | 199 x  99 x 30 | 85-264 Vac | Y (auto)    | Y (repower) |  Y  |  N  |  $39 |
| RSP-150-48 | 48 V   3.2 A | 150 W | ±1% | 250 mV | 90% | 199 x  99 x 30 | 85-264 Vac | Y (auto)    | Y (repower) |  Y  |  N  |  $43 |
| RSP-320-48 | 48 V   6.7 A | 320 W | ±1% | 240 mV | 90% | 215 x 115 x 30 | 88-264 Vac | Y (auto)    | Y (repower) |  Y  |  Y  |  $54 |
| UHP-200-12 | 12 V  16.7 A | 200 W | ±1% | 240 mV | 93% | 194 x  55 x 26 | 90-264 Vac | Y (auto)    | Y (repower) |  Y  |  N  |  $58 |
| UHP-200-15 | 15 V  13.4 A | 200 W | ±1% | 240 mV | 94% | 194 x  55 x 26 | 90-264 Vac | Y (auto)    | Y (repower) |  Y  |  N  |  $54 |
| UHP-200-24 | 24 V   8.4 A | 200 W | ±1% | 240 mV | 94% | 194 x  55 x 26 | 90-264 Vac | Y (auto)    | Y (repower) |  Y  |  N  |  $57 |
| UHP-200-48 | 48 V   4.2 A | 200 W | ±1% | 240 mV | 94% | 194 x  55 x 26 | 90-264 Vac | Y (auto)    | Y (repower) |  Y  |  N  |  $59 |

After tinkering with a couple of combinations, including using power supplies
from different families, I decided on a homogenous UHP-200-XX setup. They have
ridiculously low profile, high power, and high efficiency. On the downside, they
do have a bit of a ripple and their cost is quite a hit.

Another setup I could go with would still have UHP-200-15, but combined with
LRS-50-12 and LRS-100-48. The downside of this approach would be a bit of a
crowded central setup and less robust power supply.

The three power supplies I ended up with are UHP-200-12, UHP-200-15, and
UHP-200-55. While the last one might be a surprise since I was leaning more
toward 48V, the 55V version has unusually wide adjustment range (45-58V). This means it covers
both 48V and 55V thus allowing me some flexibility.

If you look into their datasheet carefully, one will notice that these power
supplies require quite a big heatsink. But alas, we cannot solve everything
today; there needs to be something for a future me to deal with too. For now, I
need to order these three before I change my mind again.
