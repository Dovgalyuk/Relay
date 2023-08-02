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

But not working yet (not enough code memory)

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

set_reg:
; M(1<<n)
; S(num of reg, 1..3)
; 31 instructions
    SUB F, S, 2
    JMP NC, set_reg_3
    JMP Z, set_reg_2
    OR A, A, M
    MOV D, A
    JMP check_win
set_reg_2:
    OR B, B, M
    MOV D, B
    JMP check_win
set_reg_3:
    OR C, C, M
    MOV D, C
check_win:
    ; vertical test
    AND S, A, B
    AND S, S, C
    JMP NZ, Start
    MOV D, L
    ; horizontal test
    MOVI S, 0x38
    SUB F, D, S
    JMP Z, Start
    SUB F, D, 7
    JMP Z, Start
    ; diagonal1 test
    SHR S, A
    AND S, S, B
    SHR S, S
    AND S, S, C
    JMP NZ, Start
    ; diagonal2 test
    SHR S, C
    AND S, S, B
    SHR S, S
    AND S, S, A
    JMP NZ, Start

    MOV PC, L

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

#### Board is not stored at all

Computer moves first (to the center). After that, it chooses the cell, using last human's move.
Valideness of the moves is not checked.

The order of moves always leads to the victory or tie.

Numbering of the cells:
```
012
783
654
```

```
@input()
{
    HALT();
    return INPUT();
}

@win(x)
{
    PRINT(x);
    PRINT(9);
    PRINT(9);
    HALT();
}

@check_win(m, p)
{
    x = (m + 4) & 7;
    if (x != p)
        win(x);
}

@move_left(p)
{
    m = (p - 1) & 7;
    PRINT(m);
    return m;
}

@main()
{
    PRINT(8);
    m = move_left(input());
    p = input();
    check_win(m, p);
    // computer moved to the corner
    if (Z(m & 1))
    {
        m = move_left(m);
        p = input();
        check_win(m, p);
        win(m - 1); // & 7?
    }
    m = move_left(p);
    p = input();
    check_win(m, p);
    m = move_left(p);
    p = input();
    check_win(m, p);
    // tie
    PRINT(9);
    HALT();
}
```

```
main:
    MOVI A, 8       00: 10000000 00001000
    STORE A, 0x90   01: 00110000 10010000
    HALT            02: 00010000 00000000
    LOAD A, 0x90    03: 00100000 10010000
    STORE A, 0x90   04: 00110000 10010000
    CALL move_left  05: 10001111 00011100
    MOV C, A        06: 00011010 00000000
    CALL load       07: 10001111 00101010
    CALL check_win  08: 10001111 00100000
    TST C, 1        09: 01100000 10100001
    JMP NZ, tie     0a: 11100111 00010010
    MOV A, C        0b: 00011000 00100000
    CALL move_left  0c: 10001111 00011100
    MOV C, A        0d: 00011010 00000000
    CALL load       0e: 10001111 00101010
    CALL check_win  0f: 10001111 00100000
    SUB A, C, 1     10: 01011000 10101001
    JMP win         11: 10000111 00100101
tie:
    MOV A, B        12: 00011000 00010000
    CALL move_left  13: 10001111 00011100
    CALL load       14: 10001111 00101010
    CALL check_win  15: 10001111 00100000
    MOV A, B        16: 00011000 00010000
    CALL move_left  17: 10001111 00011100
    CALL load       18: 10001111 00101010
    CALL check_win  19: 10001111 00100000
    MOVI A, 9       1a: 10000000 00001001
    STORE A, 0x90
    HALT            1b: 00010000 00000000
move_left:
    SUB A, A, 1     1c: 01011000 10001001
    AND A, A, 7     1d: 01100000 10001111
    STORE A, 0x90   1e: 00110000 10010000
    RET             1f: 00011111 01100000
check_win:
    ADD A, A, 4     20: 01001000 10001100
    AND A, A, 7     21: 01100000 10001111
    CMP A, B        22: 01011000 00000001
    JMP NZ, win     23: 11100111 00100101
    RET             24: 00011111 01100000
win:
    STORE A, 0x90   25: 00110000 10010000
    MOVI A, 9       26: 10000000 00001001
    STORE A, 0x90   27: 00110000 10010000
    STORE A, 0x90   28: 00110000 10010000
    HALT            29: 00010000 00000000
load:
    HALT            2a: 00010000 00000000
    LOAD B, 0x90    2b: 00100001 10010000
    STORE B, 0x90   2c: 00110001 10010000
    RET             2d: 00011111 01100000
```

### Lonely queen

Trying to move chess queen into the lower left corner by
moving it left and down.

Initial position:
```
800000000
700000100
600000000
500000000
400000000
300000000
200000000
100000000
012345678
```

```
@check_end(x, y, p)
{
    // print field
    i = 8;
    while (i > 0)
    {
        PRINT(i);
        j = 1;
        while (j < 9)
        {
            if (i == y && j == x)
                PRINT(1);
            else
                PRINT(0);
            j += 1;
        }
        i -= 1;
        PRINT('\n');
    }
    j = 0;
    while (j < 9)
    {
        PRINT(j);
        j += 1;
    }
    PRINT('\n');
    // check game end
    if (x == 1 && y == 1)
    {
        PRINT(p);
        PRINT('\n');
        HALT;
    }
}

@main()
{
    x = 6;
    y = 7;
    while (1)
    {
        @check_end(x, y, 2);
        // player's move
        do
        {
            a = INPUT();
            b = INPUT();
        }
        while (a >= x || b >= y || !(a == b || !a || !b) || !(a | b));
        x -= a;
        y -= b;
        @check_end(x, y, 1);
        // computer's move
        // TODO: choose move
        if (x > 1)
            x -= 1;
        else
            y -= 1;
    }
}
```
