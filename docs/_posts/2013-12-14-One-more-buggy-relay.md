---
layout: post
title: "One more buggy relay"
---

I almost finished wiring of the ALU module. After wiring arithmetic blocks' outputs to the result register I noticed, that some of the result bits do not receive values from the adder.

Here is the adder, which computed 0xff as a result:
![](/Relay/Photos/Bug01_Adder.jpg)

Result register shows that two bits in 0xff are zeroes:
![](/Relay/Photos/Bug01_Result.jpg)

The root cause of the bug was in the buggy relay - two of the contacts of the relay in the adder enable circuit did not passed the current from inputs to outputs.

Fortunately, there were 3 spare switches in the relay for the CY in the enable circuit. I just duplicated the wires from the one of the enable relays to the CY one:
![](/Relay/Photos/Bug01_Fix.jpg)

