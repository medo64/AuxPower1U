In my previous post, I split all my desires into two categories: requirements
and features; requirements being something mandatory while features being
something that is nice to have. In reality, the line between them isn't really
as clear so you can view this just as an addition to
the [first post](https://www.medo64.com/2024/08/auxpower1u-requirements/).

When it comes to features, the main externally visible functionality of my
consolidated power supply box will be the ability to reset devices from the
front as I currently have with [ResetBox](https://www.medo64.com/resetbox/).

While this is a requirement, the exact button count is a bit more flexible since
the primary function of them is to provide a quick reset of the Internet for my
family when I'm not around. Considering that, the minimum would be something
like this:
* Button 1: Arris SURFboard Modem
* Button 2: Mikrotik hAP ac, Mikrotik Audience, and Mikrotik hAP ax3

However, while I have other means of resetting devices, I also like buttons so
my desired setup might be something like this:
* Button 1: Arris SURFboard Modem
* Button 2: Mikrotik hAP ac, Mikrotik Audience, and Mikrotik hAP ax3
* Button 3: Dell OptiPlex 3050 Micro
* Button 4: Intel NUC

Let's add one more button to account for possible future needs, and this brings
us up to a total of 5. Please note there is no requirement that the button count
matches the physical layout (i.e., one button could reset two different outputs),
but I like to have it setup like that nonetheless. It just simplifies
configuration immensely if I keep that link.

Functionality of buttons is to remain the same as it is for ResetBox; i.e., they
should handle brief touches without any action, and reset should be "hidden"
behind a longer press. Also, if one keps pressing button for a long duration
(e.g., more than 10 seconds), the reset should be cancelled.

I might as well try to include some monitoring for both voltage and current.
This is easy enough to implement, and it would be a shame to miss such
opportunity. As not to go overboard, just monitoring output lines will be
sufficient since I can already see that monitoring both input and output would
take a lot of board space.

In order to display that data, I would definitely like to go with OLED, albeit I
can already see mounting it on the front will be a challenge no matter which
case I select. Thus, I might not bother making it user-visible, but I would
still like to have it on board so my development and troubleshooting can benefit
from it. I trust 128x32 will do.

For real-time monitoring, I would like to have either UART, RS-232, RS-485, or
CAN-bus output. Since each of those requires different components, I would need
to make that decision eventually but not necessarily now. I am tentatively
leaning toward CAN-bus due to its resilience; but let's hold the final decision
for now.

Lastly, having an idea of the temperature might be beneficial. While precision
is not really important, there should be a sensor somewhere on the board that
will give us a ballpark figure of how hot we're running.

That's all I really want from this project. Now, onto figuring out the details.
