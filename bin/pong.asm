lda #$7f
sta $3

lda $0
and #$8
beq $6
lda $1
add #$10
sta $1

lda $0
and #$10
beq $6
lda $1
sub #$10
sta $1

lda $1
add #$10
sta $2

lda $6
beq $9
lda $5
inc
sta $5
lda #$0
beq $5
lda $5
dec
sta $5

lda $7
beq $a
lda $5
add #$10
sta $5
lda #$0
beq $6
lda $5
sub #$10
sta $5

lda $5
and #$f0
bne $6
lda #$1
sta $7
bne $8
sub #$f0
bne $4
lda #$0
sta $7

lda $5
and #$f
bne $12
lda $6
beq $20
lda $7
beq $1c
lda $5
sub #$10
sta $5
lda #$0
bne $12
sub #$f
bne $e
lda $6
bne $a
lda $7
bne $6
lda $5
add #$10
sta $5

lda $5
and #$f0
sta $0

lda $5
dec
sub $1
bne $7
lda #$1
sta $6
dec
sta $7

lda $5
dec
sub $2
bne $6
lda #$1
sta $6
sta $7

lda $5
inc
sub $3
bne $6
lda #$0
sta $6
sta $7

lda $5
inc
sub $4
bne $7
lda #$0
sta $6
inc
sta $7

lda $3
and #$f0
sub $0
bne $a
lda $7
bne $6
lda $3
sub #$10
sta $3

lda $4
and #$f0
sub $0
bne $a
lda $7
beq $6
lda $3
add #$10
sta $3

lda $3
add #$10
sta $4

lda #$0
sta $0
lda $0
beq $fc
jmp $c