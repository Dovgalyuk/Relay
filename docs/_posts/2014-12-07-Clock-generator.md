---
layout: post
title: "Clock generator"
---

Started experimenting on clock generator.
I designed a scheme which consists of 2 relays and RC circuits:

![](/Relay/Schemes/clock.png)

Then I bought two 100 Ohm resistors and two 1500 uF capacitors.

![](/Relay/Photos/Components RC.jpg)

Soldered clock works with 2.66 seconds period:

[![](http://img.youtube.com/vi/7NZI0PF3qEE/0.jpg)](http://www.youtube.com/watch?v=7NZI0PF3qEE)

Time for energizing the capacitor and switch on the relay is about 0.2 sec (as I planned).
And discharging the capacitor and switching off the relay takes 0.93-1.2 sec.
Thus the left relay performs the charging-discharging cycle. It is switched on within ~1.2 sec and off within ~1.4 sec. This is almost perfect square wave.

I'll probably reduce the capacitors and make two speeds of execution (switched by "turbo" button).

