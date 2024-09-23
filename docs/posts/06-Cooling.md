While power supplies are selected, a huge elephant in the room remained - how
are we going to cool these? Well, good news - strictly speaking, we don't have
to, thanks to the miracle of derating.

If we check [the datasheet](https://www.meanwell-web.com/content/files/pdfs/productPdfs/MW/UHP-200(R)/UHP-200(R)-spec.pdf),
there is a derating curve where it tells us that (without any heatsinking, at
70°C, using puny 110V) we can count on about 40% of the power. So, if we don't
go over 80W, everything is awesome. And I would argue that even running two NUC
machines on my 15V power supply (most loaded one) will be under that.

But we can do better. The 1U case doesn't allow for a lot of natural air
circulation. Even though UHP series is fine with being restricted, for their
longevity, it's better if we get rid of at least some of those darn Celsius.

First thing we can add here is an aluminium plate. Adding a small 200x175x3 mm
chunk of aluminium will not only do wonders for distributing heat, but also make
their mounting easier. And it's surprisingly cheap these days to get a precut
plate with holes at the perfect location for the power supplies. I got mine at
[SendCutSend](https://sendcutsend.com/) for $35, delivered. It's not as cheap as
if you have tools and/or patience to do it yourself, but it's not going to break
the bank either.

With the plate in hand, we can use M3x6 screws and nuts to fasten power
supplies. Adding a bit of thermal paste and screwing 4 screws per power supply
is all it takes. Yes, we're not using a plate as big as recommended in the
datasheet, but this isn't nothing to laugh about. Even better, I can actually
use this plate to stiffen the whole case a bit after I mount it. How? 3D printed
rails sound as a good idea.

In addition, I have decided to add a few fans to keep the air circulating so we
don't get even close to overheating. Realistically, as long as we have some air
movement, these power supplies are going to be fine. On other hand, since I am
overbuilding this thing to start with, I can build temperature-sensitive fan
controller too.

But that's a story for the next blog post.
