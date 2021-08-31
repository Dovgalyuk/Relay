## Fibonacci sequence
```
 ; A, B, or C contain successive elements of the sequence
  ORG 0
  MOVI B, 1    ; 00: 1000 0001 0000 0001 (8101h)
 Label:
  ADD C, B, A  ; 01: 0100 1010 0001 1000 (4B18h)
  MOV A, B     ; 02: 0001 1000 0001 0000 (1810h)
  MOV B, C     ; 03: 0001 1001 0010 0000 (1920h)
  JMP Label    ; 04: 1000 0111 0000 0001 (8701h)
```
```
 int a = 0;
 int b = 1;
 while (1=1)
 {
  c = a + b;
  a = b;
  b = c;
 }
```
[![](https://img.youtube.com/vi/P9jCTSYUaJ8/0.jpg)](https://www.youtube.com/watch?v=P9jCTSYUaJ8)

## Euclid's algorithm
```
  ; A, B - operands, A - result
  ORG 0
  MOVI A, op1 ; 00: 10000000 iiiiiiii
  MOVI B, op2 ; 01: 10000001 iiiiiiii
 Loop:
  OR F, A, A  ; 02: 01101000 00000000
  JMP Z, End  ; 03: 10010111 00001100
  OR F, B, B  ; 04: 01101000 00010001
  JMP Z, End  ; 05: 10010111 00001100
  SUB F, A, B ; 06: 01011000 00000001
  JMP C, L1   ; 07: 10110111 00001010
  SUB A, A, B ; 08: 01011000 00001001
  JMP Loop    ; 09: 10000111 00000010
 L1:
  SUB B, B, A ; 0A: 01011001 00011000
  JMP Loop    ; 0B: 10000111 00000010
 End:
  ADD A, A, B ; 0C: 01001000 00001001
  HALT        ; 0D: 00010000 00000000
```
```
 int a = op1;
 int b = op2;
 while (a != 0 && b != 0)
 {
  if (a < b)
   b -= a;
  else
   a -= b;
 }
 a += b;
```
[![](https://img.youtube.com/vi/RIoIWR7b1Ps/0.jpg)](https://www.youtube.com/watch?v=RIoIWR7b1Ps)

## Calculation of Pi

Using 22/7 fraction to calculate digits of Pi.
```
 int a = 22;
 int b = 7;
 do
 {
  int d = a / b;
  a = (a % b) * 10;
  out(d);
 }
 while (1=1);
```
```
  MOVI A, 22   ; 00:
  MOVI B, 7    ; 01:
 Main:
 Div:
  MOVI D, 0    ; 02:
  MOVI C, 8    ; 03:
 Loop:
  ADD A, A, A  ; 04:
  ADC D, D, D  ; 05:
  SUB F, D, B  ; 06:
  JMP C, Next  ; 07:
  SUB D, D, B  ; 08:
  ADD A, A, 1  ; 09:
 Next:
  SUB C, C, 1  ; 0a:
  JMP NZ, Loop ; 0b:
  ; Display next digit in S
  MOV S, A     ; 0c:
  ADD D, D, D  ; 0d:
  ADD A, D, D  ; 0e:
  ADD A, A, A  ; 0f:
  ADD A, A, D  ; 10:
  JMP Main     ; 11:
```
For 355/113 fraction:
```
  ; var a
  MOVI A, 1    ; 00: 10000000 00000001
  MOVI B, 99   ; 01: 10000001 01100011
  ; var b
  MOVI M, 113  ; 02: 10000100 01110001
 Main:
 Div:
  MOVI D, 0    ; 03: 10000011 00000000
  MOVI C, 16   ; 04: 10000010 00010000
 Loop:
  ADD B, B, B  ; 05: 01001001 00011001
  ADC A, A, A  ; 06: 01000000 00001000
  ADC D, D, D  ; 07: 01000011 00111011
  SUB F, D, M  ; 08: 01011000 00110100
  JMP C, Next  ; 09: 10110111 00001100
  SUB D, D, M  ; 0a: 01011011 00111100
  ADD B, B, 1  ; 0b: 01001001 10011001
 Next:
  SUB C, C, 1  ; 0c: 01011010 10101001
  JMP NZ, Loop ; 0d: 11100111 00000101
  ; Display next digit in S
  MOV S, B     ; 0e: 00011101 00010000
  ; rem * 2 still less than 256
  ADD D, D, D  ; 0f: 01001011 00111011
  MOVI A, 0    ; 10: 10000000 00000000
  ; rem * 4
  ADD B, D, D  ; 11: 01001001 00111011
  ADC A, A, A  ; 12: 01000000 00001000
  ; rem * 8
  ADD B, B, B  ; 13: 01001001 00011001
  ADC A, A, A  ; 14: 01000000 00001000
  ; rem * 10
  ADD B, B, D  ; 15: 01001001 00011011
  ADC A, A, 0  ; 16: 01000000 10001000
  JMP Main     ; 17: 10000111 00000011
```
For 355/113 fraction with simpler division algorithm:
```
  ; var a
  MOVI A, 1    ; 00: 10000000 00000001
  MOVI B, 99   ; 01: 10000001 01100011
  ; var b
  MOVI M, 113  ; 02: 10000100 01110001
 Main:
 Div:
  MOVI L, 0    ; 03: 10000110 00000000
 Loop:
  SUB D, B, M  ; 04: 01011011 00011100
  SBC C, A, 0  ; 05: 01010010 10001000
  JMP C, Exit  ; 06: 10110111 00001011
  ADD L, L, 1  ; 07: 01001110 11101001
  MOV B, D     ; 08: 00011001 00110000
  MOV A, C     ; 09: 00011000 00100000
  JMP Loop     ; 0a: 10000111 00000100
 Exit:
  ; Display next digit in S
  MOV S, L     ; 0b: 00011101 01100000
  ; rem * 2 still less than 256
  ADD D, B, B  ; 0c: 01001011 00011001
  ; rem * 4
  ADD B, D, D  ; 0d: 01001001 00111011
  ADC A, A, A  ; 0e: 01000000 00001000
  ; rem * 8
  ADD B, B, B  ; 0f: 01001001 00011001
  ADC A, A, A  ; 10: 01000000 00001000
  ; rem * 10
  ADD B, B, D  ; 11: 01001001 00011011
  ADC A, A, 0  ; 12: 01000000 10001000
  JMP Main     ; 13: 10000111 00000011
```
[![](https://img.youtube.com/vi/bOOCfx2EN10/0.jpg)](https://youtu.be/bOOCfx2EN10)

## Multiplication subroutines

TODO

### 8x8 -> 16
```
 ; C, D - operands, pair AB - result
  MOVI C, op1
  MOVI D, op2
  MOVI A, 0
  MOVI B, 0
  MOVI M, 0
 Loop:
  SHR C, C
  JMP NC, Next
  ADD A, A, D
  ADC B, B, M
 Next:
  ADD D, D, D
  ADC M, M, M
  OR C, C, C
  JMP NZ,Loop
  HALT
```
```
 long mul8x8to16 (int op1, int op2)
 {
  int c = op1;
  long d = op2;
  long a = 0;
  while (c != 0)
  {
   if (c & 1)
    a += d;
   c = >>c;
   d = <<d;
  }
  return a;
 }
```
```
 long mul8x8to16 (int op1, int op2)
 {
  int c = op1;
  long d = op2;
  long a = 0;
  while (c != 0)
  {
   if (0 >> c)
    a += d;
   d << 0;
  }
  return a;
 }
```
### 8x8 -> 8
```
 ; C, D - operands, A - result
  MOVI C, op1  ; 00: 10000010 iiiiiiii
  MOVI D, op2  ; 01: 10000011 iiiiiiii
  MOVI A, 0    ; 02: 10000000 00000000
 Loop:
  SHR C, C     ; 03: 01111010 -0101100
  JMP NC, Next ; 04: 11000111 00000110
  ADD A, A, D  ; 05: 01001000 00001011
 Next:
  ADD D, D, D  ; 06: 01001011 00111011
  OR F, C, C   ; 07: 01101000 00100010
  JMP NZ,Loop  ; 08: 11100111 00000011
  HALT         ; 09: 00010000 00000000
```
[![](https://img.youtube.com/vi/hHF_ab6-ruQ/0.jpg)](https://www.youtube.com/watch?v=hHF_ab6-ruQ)

## Division subroutines

### 8/8 -> 8
```
 ; D, M - operands, D - result, A - remainder
  MOVI D, op1  ; 00: 10000011 iiiiiiii
  MOVI M, op2  ; 01: 10000100 iiiiiiii
  MOVI A, 0    ; 02: 10000000 00000000
  MOVI C, 0    ; 03: 10000010 00000000
  MOVI B, 8    ; 04: 10000001 00001000
 Loop:
  SHR C, C     ; 05: 01111010 00101100
  ADC D, D, D  ; 06: 01000011 00111011
  ADC A, A, A  ; 07: 01000000 00001000
  SUB F, A, M  ; 08: 01011000 00000100
  ADC C, C, 0  ; 09: 01000010 10101000
  XOR C, C, 1  ; 0a: 01110010 10101001
  JMP Z, Next  ; 0b: 10010111 00001101
  SUB A, A, M  ; 0c: 01011000 00001100
 Next:
  SUB B, B, 1  ; 0d: 01011001 10011001
  JMP NZ, Loop ; 0e: 11100111 00000101
  SHR C, C     ; 0f: 01111010 00101100
  ADC D, D, D  ; 10: 01000011 00111011
  HALT         ; 11: 00010000 00000000
```
```
 long div8x8 (int op1, int op2)
 {
  int d = op1;
  int m = op2;
  int a = 0;
  int b = 8;
  int c = 0;
  do
  {
    a << d << (0 >> c);
    if (a > m)
    {
      a -= m;
      c += 1;
    }
  }
  while (--b);
  d << (0 >> c);
  return d;
 }
```
```
 long div8x8 (int op1, int op2)
 {
  int d = op1;
  int m = op2;
  int a = 0;
  int b = 8;
  do
  {
    a << d << 0;
    if (a > m)
    {
      a -= m;
      d += 1;
    }
  }
  while (--b);
  return d;
 }
```
```
 ; D, M - operands, D - result, A - remainder, B - temp
  MOVI D, op1  ; 00: 10000011 iiiiiiii
  MOVI M, op2  ; 01: 10000100 iiiiiiii
  MOVI A, 0    ; 02: 10000000 00000000
  MOVI B, 8    ; 04: 10000001 00001000
 Loop:
  ADD D, D, D  ; 05:
  ADC A, A, A  ; 06: 01000000 00001000
  SUB F, A, M  ; 07: 01011000 00000100
  JMP C, Next  ; 08:
  SUB A, A, M  ; 09: 01011000 00001100
  ADD D, D, 1  ; 0a:
 Next:
  SUB B, B, 1  ; 0b: 01011001 10011001
  JMP NZ, Loop ; 0c:
  HALT         ; 0d: 00010000 00000000
```
## Games

### Guess the number

#### First edition

Program tries to guess the number (1..100) by showing the guesses in A register and halting.
User must set 'guess' field to non-zero if A is greater than the number and press Start button to continue.
```
  MOVI B, 1       ; 00: 10000001 00000001
  MOVI C, 101     ; 01: 10000010 01100101
 Loop:
  ADD A, B, C     ; 02: 01001000 00011010
  SHR A, A        ; 03: 01111000 00001100
  HALT            ; 04: 00010000 00000000
  MOVI D, guess   ; 05: 10000011 iiiiiiii
  OR F, D, D      ; 06: 01101000 00110011
  JMP NZ, Greater ; 07: 11100111 00001010
  MOV B, A        ; 08: 00011001 00000000
  JMP Loop        ; 09: 10000111 00000010
 Greater:
  MOV C, A        ; 0a: 00011010 00000000
  JMP Loop        ; 0b: 10000111 00000010
```
#### Second edition

Program tries to guess the number (1..100) by showing the guesses in A register and halting.
User must reset D to zero if A is less than the number and press Start button to continue.
```
  MOVI B, 1       ; 00: 10000001 00000001
  MOVI C, 101     ; 01: 10000010 01100101
 Loop:
  ADD A, B, C     ; 02: 01001000 00011010
  SHR A, A        ; 03: 01111000 00001100
  MOVI D, 1       ; 04: 10000011 00000001
  HALT            ; 05: 00010000 00000000
  OR F, D, D      ; 06: 01101000 00110011
  JMP NZ, Greater ; 07: 11100111 00001010
  MOV B, A        ; 08: 00011001 00000000
  JMP Loop        ; 09: 10000111 00000010
 Greater:
  MOV C, A        ; 0a: 00011010 00000000
  JMP Loop        ; 0b: 10000111 00000010
```
### The subtraction game (21 game)

There are 21 matches. Two players take 1..3 matches every turn.
The player who takes the last match wins.
The computer is the second player.

Program ends when A=0x00 or A=0xff. In the first case the winner is computer, in the second case - you are the winner.

#### First edition

When the execution stops, the number of matches is in register A, and
the player's move should be encoded in the lower 8 bits of instruction at address 02
(which is the field 'move').
```
  MOVI A, 21   ; 00: 10000000 00001101
 Loop:
  HALT         ; 01: 00010000 00000000
  MOVI B, move ; 02: 10000001 000000mm
  SUB A, A, B  ; 03: 01011000 00001001
  AND C, A, 3  ; 04: 01100010 10001011
  MOVI Z, C, 1 ; 05: 10010010 00000001
  SUB A, A, C  ; 06: 01011000 00001010
  JMP Loop     ; 07: 10000111 00000001
```
#### Second edition

When the execution stops, player must take the 'matches' - reset 1 to 2 of B, C, D registers.
Every register stores 1 'match' and the computer will subtract non-zero registers from A.
```
  MOVI A, 21   ; 00: 10000000 00010101
 Loop:
  MOVI B, 1    ; 01: 10000001 00000001
  MOVI C, 1    ; 02: 10000010 00000001
  MOVI D, 1    ; 03: 10000011 00000001
  HALT         ; 04: 00010000 00000000
  SUB A, A, B  ; 05: 01011000 00001001
  SUB A, A, C  ; 06: 01011000 00001010
  SUB A, A, D  ; 07: 01011000 00001011
  AND C, A, 3  ; 08: 01100010 10001011
  MOVI Z, C, 1 ; 09: 10010010 00000001
  SUB A, A, C  ; 0a: 01011000 00001010
  JC Exit      ; 0b: 10110111 00001101
  JNZ Loop     ; 0c: 11100111 00000001
 Exit:
  HALT         ; 0d: 00010000 00000000
```

### Tic-tac-toe
#### Board is stored in the registers

Idea by [DimaThenekov](https://github.com/DimaThenekov/)

```
; data storage
; o-o    A: 00|000||101|
; xxo -> B: 00|110||001|
; oxx    C: 00|011||100|
; move encoding
; S: 0000rrcc - row & col (1-3)
;    columns are numbered from right to left
; S: xxxx
;    765x
;    ba9x
;    fedx
; 34 instructions
Start:
    HALT
    ; board registers
    MOVI А, 0
    MOVI B, 0
    MOVI C, 0

; X move
Main_loop:
    ; input cell number into S in the next instruction
    HALT
    MOVI S, 5
    CALL num2reg
    ADD M, M, M
    ADD M, M, M
    ADD M, M, M
    CALL set_reg

; O move
    ; geting seed
    XOR D, A, B
    XOR D, D, C
    ADD D, D, 4
O_move_loop:
    ADD D, D, 1
    ; r=0 or c=0 are invalid values
    AND F, D, 3
    JMP Z, O_move_loop
    SHR S, D
    AND F, S, 6
    JMP Z, O_move_loop

    MOVI S, 0xf
    AND S, S, D
    CALL num2reg
    SUB F, S, 2
    JMP CY, O_move_row1
    JMP Z, O_move_row2
O_move_row3:
    AND F, C, M
    JMP O_move_test
O_move_row1:
    AND F, A, M
    JMP O_move_test
O_move_row2:
    AND F, B, M
O_move_test:
    JMP NZ, O_move_loop
O_move:
    CALL set_reg
    JMP Main_loop

check_win_hrz:
; S - bits to check (0x38 or 0x7)
; 7 instructions
    SUB F, A, S
    JMP Z, Start
    SUB F, B, S
    JMP Z, Start
    SUB F, C, S
    JMP Z, Start
    MOV PC, L

set_reg:
; M(1<<n)
; S(num of reg, 1..3)
; 17 instructions
    SUB F, S, 2
    JMP NC, set_reg_3
    JMP Z, set_reg_2
    OR A, A, M
    JMP check_win
set_reg_2:
    OR B, B, M
    JMP check_win
set_reg_3:
    OR C, C, M
check_win:
    ; vertical test
    AND S, A, B
    AND S, S, C
    JMP NZ, Start
    MOV D, L
    ; horizontal test
    MOVI S, 0x38
    CALL check_win_hrz
    MOVI S, 0x7
    CALL check_win_hrz
    ; TODO: diagonal test
    MOV PC, D

num2reg:
; input: S register(0..f)
; output: M(1<<S mod 4)
;         S(S div 4)
; 6 instructions
    AND M, S, 3
    SUB F, M, 3
    MOVI Z M, 4
    SHR S, S
    SHR S, S
    MOV PC, L
```
