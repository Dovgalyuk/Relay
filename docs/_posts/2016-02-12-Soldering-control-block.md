---
layout: post
title: "Soldering control block"
---


Found several issues in schematics:
* There is no available contacts for making 1-tick PC selection for update (tick 5). Therefore I will use tick 4 and 5 for selecting PC to address bus for proceeding to the next instruction
* I switched contacts in trigger relays of incrementer, because there are simpler to connect the relay power pin to "inputs" and not to the outputs as on several schemes
* I will also have to add one more S4 signal duplication relay to produce forgotten S4.INC which will be passed to PC Load

