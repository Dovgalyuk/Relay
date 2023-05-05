
## Teletype output decoder

Decoded only 4 LS bits of the output value:
* 0-9 - digits
* A-E - not used
* F - carriage return

![](/Schemes/Teletype output.png)

## Teletype input register

Reads 4 LS bits of the key pressed on the teletype.
Also sends INT signal to the Control block, which allows to continue
program execution, when CPU is halted.

Keys 0-9 are read as values 0-9.

![](/Schemes/Teletype input.png)
