As part of my home server setup, I have a few devices that have "free-floating"
power supplies. For example, my modem, my wireless PoE adapters, and a few test
boxes all have their power supply pushed into rack's nooks and crannies. Even
worse, since I want to have the capability to reset them, they are connected via
[ResetBox](https://www.medo64.com/resetbox/) (or its [type-C variant](https://www.medo64.com/2023/05/type-c-power-delivery-as-passive-poe-source/))
making cable situation even messier. It's way beyond time to sort that out!

In this blog series, I will go over what's needed to design a nice 1U box that
can fit all (or as many as I can) different power supplies togethe. Their
outputs should be resettable by physical buttons on the device's front. Since I
actually didn't finish the project as I'm typing this, expect the series not
only to last a few months while I gather all necessary equipment but also for my
"specifications" to shift slightly as I discover new things or rethink my old
ways.

At first, let's look at what power supplies we have currently:
* `12V` `25W`: Arris SURFboard Modem
* `22V-57V` `20W`: Mikrotik hAP ac
* `24-57V` `30W`: Mikrotik Audience
* `12-28V` `40W`: Mikrotik hAP ax3
* `19.5V` `65W`: Dell OptiPlex 3050 Micro
* `12-19V` `65W`: Intel NUC
* `54V` `15W`: Netgear GS305EPP (150W max)

If I squint hard enough, there are three distinct power supplies to use there;
the first one being a simple 12V power supply for modem. Power usage will be
really low on this one, so any hardware we place will run from it too.

To the second power supply, I had to give a bit more thought. Without question,
it has to run of the UPS but its target voltage is a bit of an unknown.
Currently, I am running my hAP ac and Audience from a 20V type-C power supply,
and Audience doesn't seem to love it as any minor transient causes reset. And
yes, officially Audience doesn't run that low, but I got lucky, I guess.
Previously, I was running my WiFi routers on 48V and both were fine with that;
so there are my two daya points. Considering other devices, I was leaning toward
selecting 24V as a second power supply.

However, that leaves my NetGear PoE switch a bit of a loner and outside of The
Box. For it, I would need to provide a proper 55V PoE power supply or at least
48V if stars align. The downside of this approach is that it leaves my Mikrotik
hAP ax3 either on 12V or for the last, non-UPS power supply. But the upside is
that it allows for a bit of future-proofing.

That leaves 2 non-UPS computers. Why are they not using UPS? Well, in my setup,
UPS power is really limited and is reserved for only two categories: my main
server and my Internet delivery devices (modem, router, WiFi APs). Anything else
just needs to handle a power loss. These two computers fall into "anything else"
category.

Intel NUC, we can already see, is flexible with a power supply specification.
When it comes to standard industrial voltages, it can handle both 12V and 15V
inputs. However, it Dell bretheren officially are not that flexible. While there
is a possibility my 3050 micro would work on 15V, anything higher is a no-go.
And since Dell authenticates its chargers, to figure this out, I'd need to trick
it first into accepting such voltage - all of which smells like another
sub-project. :)

If Dell doesn't want to cooperate, the only way forward would be to use one of
many [buck modules](https://www.amazon.com/dp/B07HRBT529/) intended for RVs that
brings 24V to 19V. I would really like to avoid this, if possible, because the
last thing I need is yet another power supply. However, it's good to have
options. And yes, ideally, I would find a 19V power supply; but I have a feeling
that finding one that can be properly mounted inside a 1U case is not going to
happen.

This brings us to the following rough power supply distribution (with a bit of
derating on power specification):
* `12V` `150W` (`100W` if I kick out Mikrotik hAP ax3 from my network):
  - `25W`: Arris SURFboard Modem
  - `25W`: Control boards
  - `40W`: Mikrotik hAP ax3

* `15V`/`19V`/`24V` `200W` (`150W` should be realistically more than fine):
  - `65W`: Dell OptiPlex 3050 Micro
  - `65W`: Intel NUC

* `48V`/`55V` `100W` (going higher than this might be good for the future PoE devices):
  - `20W`: Mikrotik hAP ac
  - `30W`: Mikrotik Audience
  - `15W`: Netgear GS30s5EPP

There is an alternative at a bit lower voltage:

* `12V` `75W`
  - `25W`: Arris SURFboard Modem
  - `25W`: Control boards

* `15V`/`19V`/`24V` `200W` (`150W` should be realistically more than fine):
  - `65W`: Dell OptiPlex 3050 Micro
  - `65W`: Intel NUC

* `24V` `150W`
  - `20W`: Mikrotik hAP ac
  - `30W`: Mikrotik Audience
  - `40W`: Mikrotik hAP ax3

* Out-of-scope
  - `15W`: Netgear GS30s5EPP

I am strongly leaning toward option 1, but option 2 is a good alternative. And
yes, Mikrotik is not as power hungry as it seems above; I've never seen it reach
its maximum power usage. However, since I really love my network, I use those
numbers to bring an additional margin to the dimensioning process. If I find a
nice power supply that's slightly below what I need, I will get it and not worry
about it. But, before I get to that, this fudged accounting provides more
visibility into what brings the most value.

For various protection circuits, I', going to rely onto power supplies to
protect themselves. Thus, at minimum, I expect any selected power supply to have
over-voltage, over-current, short-circuit, and over-temperature protection
built-in.

When it comes to controlling this, I would say that ability to reset my Modem
and WiFi is a must. And these can be two buttons as I want to be able to
separately restart modem. For the hAP ax3, I don't care as much since it has
"under test" status in my network at this time. But let's argue that it needs to
be a separate button. At this time, I do control each of my computers inside the
rack via smart plug so moving them into The Box, I might want to see a button
for each, but I wouldn't really mind if both go down at the same time. This
brings the total number of reset buttons to somewhere between a minimum of three
and a maximum of seven.

Lastly, this blog post leaves us with the following action items I will probably
get around to solving:
* Check can Netgear GS305EPP work on 48V
* Make trigger board for Dell
* Check does Dell Optiplex 3050 Micro work on 15V
