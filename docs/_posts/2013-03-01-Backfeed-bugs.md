---
layout: post
title: "Backfeed bugs"
---

Found possible backfeeds in ALU schemes. When input register is connected to the result register through the shifter block (because it consists of set of wires) or while passing result to the data bus they can latch values of each other.

This bug may be fixed by creating a "copy" of the result value and input value for shifter by using spare switches of the relays.

