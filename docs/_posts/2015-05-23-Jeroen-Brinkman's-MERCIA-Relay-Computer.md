---
layout: post
title: "Jeroen Brinkman's MERCIA Relay Computer"
---

[MERCIA](http://www.relaiscomputer.nl/) means Mijn Eenvoudige Relais Computer In Aanbouw, or My Simple Relay Computer Under Construction. It is a Dutch acronym for a project that is taking me more than a year, and the finish is not even in sight.

It is not finished yet. Project site provides documented computer design.

MERCIA in a nutshell
* 10 bit word
* 16 bytes I/O memory
* 496 bytes ROM
* 512 bytes RAM
* ±1700 relays
* 9 panels 50cm*120cm
* >50.000 parts

![](http://www.relaiscomputer.nl/images/Architectuur12.png)

In this computer registers and instruction have 10-bit width. ALU also functions as incrementor for the program counter and is also used to do the addition for relative addressing. It is also implements fast multiplication and division with assistance of ROM subroutines.

Author has already implemented ROM and RAM blocks:

![](http://www.relaiscomputer.nl/images/HomePic.jpg)
