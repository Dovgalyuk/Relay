---
layout: post
title: "Fixed a bug in ALU sequencing"
---


I tried to execute a division subroutine and encountered couple of bugs:
* The "flags only" version of the ALU instructions didn't work
* There was a kind of backfeed in ALU

I fixed the second one by shortening some of the synchronization signals and now modified division subroutine works.
Now I need to fix the first bug and probably I'll need to add one more relay to the control block.

