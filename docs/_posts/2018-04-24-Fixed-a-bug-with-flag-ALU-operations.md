---
layout: post
title: "Fixed a bug with flag ALU operations"
---


Now the ALU instructions without the writeback work normally.
I fixed the control block by adding one more relay.
There were other methods of fixing with more spaghetty wiring, but I prefer to make the wiring compact and have some redundancy for the possible fixing in the future.

