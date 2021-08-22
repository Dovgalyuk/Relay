---
layout: post
title: "A fistful of bugs"
---

Today I finished wiring of the ALU module (excluding wires to output socket).
And I made several attempts to fix or replace faulty relays:
* First, I made parallel connections for two switches in result register
* Second, I replaced (twice!) the relay in the result enable circuit.
* Then I changed connection of one of the input pins of the result enable - now it is connected to the "holding" pin of the result (not to the duplicated +V as on the scheme).
* Next bug was in AND module - poor contact in one of the relays prevented evaluating 0 bit of the result of the 'and' operation.

And the last of the found bugs was not fixed - I found that SUB module sometimes does not pass CY from 0 bit to the output of 1 bit (e.g. when 0 SUB 1 is calculated). But this bug does not manifest itself in every test. It seems that contact is not too bad - sometimes the current can flow through it.

