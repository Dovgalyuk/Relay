# Compiler for a fistful of relays computer

## Language features

### Types

int - signed byte value

long - signed two-byte value

uint - unsigned byte value

ulong - unsigned two-byte value

arrays (require RAM to be implemented)

### Literals

Decimal - 123

Hexadecimal - 0xff

Binary - 0b0011

### Arithmetic

add (+)

subtract (-)

and (&)

or (|)

xor (^)

### Working with carry



#### Shift operator

ROR, SHR, RCR

Rvalue arg:

C style: a = b >> 1;

Prefix: a = >> b;

Postfix: a = b >>;

Array: a = b[6:1]

Tuple: (r, cy) = << a;


Lvalue arg:

Pipe: a >> b >> c;

Carry as a result: a << d << (0 >> c);

Pipe with carry: >>a >>b <<c;

Pipe with carry2: b=>>a -> c+=d -> e=<<f;


Bidirectional:

a = >>b -> c = >>d

### Assignments

### Branches

### Goto

### Loops

### Functions

Function may return two values (divmod).

Function may return CY and a register together.

## Code samples

### Fibonacci sequence

```
byte a = 0;
byte b = 1;
byte c;
while (1==1)
{
    c = a + b;
    OUT(c);
    a = b;
    b = c;
}
```

### Euclid's algorithm

```
byte a = op1;
byte b = op2;
while (a != 0 && b != 0)
{
    if (a < b)
        b -= a;
    else
        a -= b;
}
a += b;
OUT(a);
```

### Calculation of Pi

22/7 fraction

```
 int a = 22;
 int b = 7;
 do
 {
  int d = a / b; // div8x8->8
  a = (a % b) * 10;
  out(d);
 }
 while (1=1);
```

355/113 fraction

```
 long a = 355;
 int b = 113;
 do
 {
  int d = a / b; // div16x8->16 and crop
  a = (a % b) * 10;
  out(d);
 }
 while (1=1);
```

### Multiplication

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

long mul8x8to16 (int op1, int op2)
{
    int c = op1;
    long d = op2;
    long a = 0;
    while (c != 0)
    {
        // result code of >> is CY
        if (0 >> c)
            a += d;
        d << 0;
    }
    return a;
}
```

### Division

```
int div8x8 (int op1, int op2)
{
    int d = op1;
    int m = op2;
    int a = 0;
    int b = 8;
    int c = 0;
    do
    {
        // CY pipe
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

int div8x8 (int op1, int op2)
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

### Guess the number game

```
int a = 1;
int b = 101;
do
{
    int guess = a + b;
    // pipe shift
    0 >> guess;
    int yes = IN();
    if (yes == 0)
    {
        HALT;
    }
    // check sign here
    if (yes > 0)
        b = guess;
    else
        a = guess;
}
while (1 == 1);
```

### Tic-tac-toe game

Board storage is an array.

```
// board storage
int board[9];

void check_win()
{
    int win = 0;
    int t;
    for (int i = 0 ; i < 3 ; ++i)
    {
        t = board[i * 3];
        if (t != 0 && t == board[i * 3 + 1] && t == board[i * 3 + 2])
            win = t;
        t = board[i];
        if (t != 0 && t == board[i + 3] && t == board[i + 6])
            win = t;
    }
    t = board[0];
    if (t != 0 && t == board[4] && t == board[8])
        win = t;
    t = board[2];
    if (t != 0 && t == board[4] && t == board[6])
        win = t;
    if (win != 0)
    {
        OUT(win);
        HALT;
    }
}

void out_board()
{
    for (int i = 0 ; i < 9 ; ++i)
        OUT(board[i]);
}

void move()
{
    int p;
    do
    {
        p = RAND(9);
    }
    while (board[p] != 0)
    board[p] = 2;
}

do
{
    // input move
    int y = IN();
    int x = IN();
    // modify board
    board[x + 3 * y] = 1;
    // output the board
    out_board();
    // check the winner
    check_win();
    // computer's move
    move();
    // output the board
    out_board();
    // check the winner
    check_win();
}
while (1 == 1)
```
