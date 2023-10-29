.include "hdr.asm"

.ramsection "ram.data" bank $7E

myindex DSB 1

.ends

.section ".decrement_function" superfree

decrement:
    phb
php

sep #$20                        ; change current bank to 7E
lda #$7e
pha
plb

dec myindex                  ; decrement value

plp
plb
rtl

.ends
