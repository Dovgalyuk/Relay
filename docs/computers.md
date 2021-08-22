| [A fistful of relays](https://github.com/Dovgalyuk/Relay)                                  | [Relay/ Harry Porter's](http://web.cecs.pdx.edu/~harry/)                                    | [Relay computer two](http://www.electronixandmore.com/projects/relaycomputertwo/index.html) | [RC-3](http://www.computerculture.org/projects/rc3/)                                        | Simon                                                                                       | [Zusie](http://www.nablaman.com/relay)                                                      | [TIM](http://www.northdownfarm.co.uk/rory/tim/tim-8.htm)                                    | [DUO 14 Premium](http://www.ostracodfiles.com/ostracod/relay.html)                          | [Relay computer "trainer"](http://relaysbc.sourceforge.net/)                                | [i² 8-Bit Relay Computer](http://isquared.weebly.com/)                                      | [Kilian Leonhardt's](http://www.relaiscomputer.de/)                                         | [Der Relaisrechner](http://www.schlaefendorf.de/relaisrechner/dokumentation/index.html)     |[MERCIA](http://www.relaiscomputer.nl/)                                                     |
|--------------------------------------------------------------------------------------------|---------------------------------------------------------------------------------------------|---------------------------------------------------------------------------------------------|---------------------------------------------------------------------------------------------|---------------------------------------------------------------------------------------------|---------------------------------------------------------------------------------------------|---------------------------------------------------------------------------------------------|---------------------------------------------------------------------------------------------|---------------------------------------------------------------------------------------------|---------------------------------------------------------------------------------------------|---------------------------------------------------------------------------------------------|---------------------------------------------------------------------------------------------|---------------------------------------------------------------------------------------------|
Component | Used number of relays ||||||||||||
ALU                 |92|58 |50|64|. |. |12 |. |. |. |. |.|224
Registers           |97|126|55|106|. |. |. |. |. |. |. |.|.
PC unit             |28|108|80|104|. |. |. |. |. |. |. |.|.
Sequencer           |30| 35|16|44|. |. |. |. |. |. |. |.|.
Instruction decoder |46| 50|60|48|. |. |. |. |. |. |. |.|.
Memory unit         |25(*)| 37|20|44|. |. |. |. |. |. |. |.|.
Total               |303 4PDT|415 4PDT|281|418 4PDT|80|. |152 | 55 | 83 | ~300 | 1500 |.|1950 SPDT
Registers |||||||||||||
Data storage|7 8-bit (*)|8 8-bit|3 8-bit|8 8-bit|12 2-bit|3 8-bit |5 8-bit |. |. | 3 8-bit |. | 8 8-bit | 5 10-bit
ALU input/output|8 8-bit (*)|4 8-bit|3 8-bit|4 8-bit|6 2-bit|3 8-bit |. |. |. | 2 8-bit |. | 8 8-bit | 3 10-bit
Memory pointer|3 8-bit (*)|1 16-bit|-|1 16-bit| - |2 12-bit |. |. |. |. |. | 8 8-bit | 1 10-bit
Other registers|8-bit PC |1 16-bit|. |1 16-bit|11 2-bit|1 12-bit |. |3-bit PC | 8-bit PC, 1-bit carry, 4-bit output |. |. | 12-bit PC | 10-bit PC, return, jump
Total |8 8-bit (*)|8 8-bit|3 8-bit|8 8-bit|15 2-bit|3 8-bit, 3 12-bit | 5 8-bit |. | 3 | 3 8-bit | 16 11-bit | 8 8-bit, 12-bit PC, 8-bit shadow ALU output | 8 10-bit
Shadow registers|1 8-bit|1 16-bit|1 16-bit|1 16-bit|2 2-bit|. |. |. |. |. |. | 1 8-bit ALU output | 1 10-bit
Instructions
Instruction width, bits | 16(*) | 8-24 | . | 8-24 | . | . | . | 6 | 32 | 8/16 | 18 | 16 | 10
Total | 21(*)| 19 | 15 | 23 | 9 | 34 | 16 | 3 | 31 | 15 | 13 | ?? |29 available, 21 used
Memory |||||||||||||
Address bus width | 8 program/data (*) | 16 program/data | 16 program/data | 16 program/data | - | 16 program/data | 4 data (12 bytes of memory on capacitors) + 4 program (16 bytes on dip switches) | 3 program + 2 data | 8 program + 8 data | 8 program + 8 data | 11 program | 12 program, 9 data, 8 IO |10 program/data
Installed memory | . | . | . | . | . | . | . | . | . |. |. | . | 496 bytes ROM, 512 bytes RAM

PC unit includes PC register, PC incrementer, jump register, instruction register.
