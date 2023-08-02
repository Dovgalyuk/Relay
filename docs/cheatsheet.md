## Registers

Order of registers: ABCDMSLP

## Peripheral addresses

* 0x80 - 0x8f: two 8-bit input switches (first for even addresses and second for odd)
* 0x90 - 0x9f: teletype (input - keyboard, output - printing)

## Instructions set

* 000: A / empty - jump always
* 001: Z - jump if zero (Z=1)
* 010: NS - jump if positive (S=0)
* 011: C - jump if carry (C=1)
* 100: NC - jump if not carry (C=0)
* 101: S - jump if negative (S=1)
* 110: NZ - jump if not zero (Z=0)

```
00010--- -------- HALT
00000--- -------- NOP
00101ddd -----aaa LOAD d, a
00100ddd iiiiiiii LOAD G, Imm
00111sss -----aaa STORE G, P
00110sss iiiiiiii STORE G, Imm
1ccc1111 aaaaaaaa CALL Cond, Imm
1ccc0111 aaaaaaaa JMP Cond, Imm
1ccc0ddd iiiiiiii MOVI Cond, Gd, Imm8
00011ddd -sss---- MOV Gd, Gs
01bbbddd ixxxryyy OP Gd, Gs1, Op2
01111ddd -sssruu0 OP Gd, Gs
```

### Binary ops

0. ADC
1. ADD
2. SBC
3. SUB
4. AND
5. OR
6. XOR

### Unary ops

0. NOT - bitwise not
1. SHR - shift to right
2. ROR - rotate to right
3. RCR - rotate through CY to right
