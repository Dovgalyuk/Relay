This page describes the schematic of the computer elements. Each of the relays used in this computer has four double-throw switches.

Schematic for representing the relays was taken from Harry Porter's page:

![](https://github.com/Dovgaluk/Relay/blob/master/Schemes/relay.jpg)

[[ABCD registers]]

[[Address registers]]

[[ALU]]

[[Control]]

[[Memory]]

## Blocks interconnections

Computer contains several blocks:
* ALU
* Registers file
* Control
* Memory and IO

All interconnections are made with 25-wire cables and D-sub connector.
25th pin in every connector is attached to the ground.

### ALU - Registers

* 1 - D1:0
* 2 - D1:1
* 3 - D1:2
* 4 - D1:3
* 5 - D1:4
* 6 - D1:5
* 7 - D1:6
* 8 - D1:7
* 9 - D2:0
* 10 - D2:1
* 11 - D2:2
* 12 - D2:3
* 13 - D2:4
* 14 - D2:5
* 15 - D2:6
* 16 - D2:7
* 17-24 - NC
* 25 - GND

### Control - Registers 1

* 1 - D1:0
* 2 - D1:1
* 3 - D1:2
* 4 - D1:3
* 5 - D1:4
* 6 - D1:5
* 7 - D1:6
* 8 - D1:7
* 9 - D2:0
* 10 - D2:1
* 11 - D2:2
* 12 - D2:3
* 13 - D2:4
* 14 - D2:5
* 15 - D2:6
* 16 - D2:7
* 17 - A:0
* 18 - A:1
* 19 - A:2
* 20 - A:3
* 21 - A:4
* 22 - A:5
* 23 - A:6
* 24 - A:7
* 25 - GND

### Control - Registers 2

* 1 - Sel:D1:A
* 2 - Sel:D1:B
* 3 - Sel:D1:C
* 4 - Sel:D1:D
* 5 - Sel:D1:M
* 6 - Sel:D1:S
* 7 - Sel:D1:L
* 8 - Sel:D1:PC
* 9 - Sel:D2:A
* 10 - Sel:D2:B
* 11 - Sel:D2:C
* 12 - Sel:D2:D
* 13 - Sel:D2:M
* 14 - Sel:D2:S
* 15 - Sel:D2:L
* 16 - Sel:D2:PC
* 17 - NC
* 18 - NC
* 19 - NC
* 20 - NC
* 21 - Sel:A:M
* 22 - Sel:A:S
* 23 - Sel:A:L
* 24 - Sel:A:PC
* 25 - GND

### Control - Registers 3

* 1 - LD:A
* 2 - LD:B
* 3 - LD:C
* 4 - LD:D
* 5 - LD:M
* 6 - LD:S
* 7 - LD:L
* 8 - LD:PC
* 9-24 - NC
* 25 - GND

### Control - ALU

* 1 - INSTR13 - OPB2
* 2 - INSTR12 - OPB1
* 3 - INSTR11 - OPB0
* 4 - INSTR2 - OPU1
* 5 - INSTR1 - OPU0
* 6 - Z
* 7 - S
* 8 - CY
* 9 - LD_DST
* 10 - LD_RES
* 11 - SEL_RES
* 12 - SEL_ALU
* 13 - SEL_ALU_D1
* 14 - SEL_ALU_D2
* 15-24 - NC
* 25 - GND

### Control - Memory 1

* 1 - D1:0
* 2 - D1:1
* 3 - D1:2
* 4 - D1:3
* 5 - D1:4
* 6 - D1:5
* 7 - D1:6
* 8 - D1:7
* 9 - D2:0
* 10 - D2:1
* 11 - D2:2
* 12 - D2:3
* 13 - D2:4
* 14 - D2:5
* 15 - D2:6
* 16 - D2:7
* 17 - A:0
* 18 - A:1
* 19 - A:2
* 20 - A:3
* 21 - A:4
* 22 - A:5
* 23 - A:6
* 24 - A:7
* 25 - GND

### Control - Memory 2

* 1 - SEL_MEMA
* 2 - RD_MEM
* 3 - SEL_MEMD
* 4 - WR_MEM
* 5-24 - NC
* 25 - GND
