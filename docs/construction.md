This computer is based on 24V 4PDT relays (Four-Pole-Double-Throw). Each of the relays has a LED indicating whether it on or off.

Power supply has 6.5A/24V output. 24V was selected instead of widely available 12V to reduce the current and make wires thinner.
But this decision has an opposite side: there are no LEDs with integrated resistors for 24V.

## ALU

Relays required for ALU components:

Component | Number of relays
----------|-----------------
Adder | 16
Subtractor | 16
Logic | 16
Shifter | 0
Result register | 13
Decoder | 5
Enable circuits | 19
CY storage | 3
ALU enable | 4
Total | 92

ALU board:

![](../Schemes/ALU%20board.png)

Photo of completed ALU:

![](../Photos/ALU%20complete.JPG)

## Registers module

Relays required for registers:

Component | Number of relays
----------|-----------------
Data | 64
Load | 8
Data enable | 32
Address enable | 8
Total | 112
Without PC | 97
Program counter | 15


### Registers board

![](../Schemes/Registers%20board.png)

## Control board

![](../Schemes/Control%20board.png)

## Memory board

![](../Schemes/Memory%20board.png)
