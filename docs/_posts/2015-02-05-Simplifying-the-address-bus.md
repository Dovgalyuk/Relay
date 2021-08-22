---
layout: post
title: "Simplifying the address bus"
---


I've decided to shrink down PC register and address bus.
For now it will be 8 bits in width. Then the computer will have opportunity to address 256 instructions.
With clock rate 5Hz it will take 0.2*10*256 = 512 seconds to execute them all. This is rather slow.
That's why there is no need to have larger bus. Anyway, I can always add some segment registers to switch between memory banks.

Such a simplification will lead to removing 12/16-bit commands and reworking some of the currently created schemes.
I'll probably get rid of one GPR and reuse it as PC. That would also simplify everything.
And I also should find something to rework LOAD/STORE instructions.

