---
layout: post
title: "Found a bug in ROM PCBs"
---


I tried to program my 8-word ROM.
Simplest increment program seem to be working:

[![](http://img.youtube.com/vi/BQ2KYB1ZIaA/0.jpg)](https://www.youtube.com/watch?v=BQ2KYB1ZIaA)

But there is a problem with programs that have intersection in non-zero bits in the instructions:

![](/Relay/Photos/Bug_Memory.jpg)

I forgot to add diodes and the voltage from one selected ROM address passes to other addresses
causing all of them to be selected.

Now I have to reroute PCBs and add diodes to them.

