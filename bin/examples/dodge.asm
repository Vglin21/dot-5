lda #$d7
sta $1
lda #$d
sta $2
lda #$58
sta $3
lda #$a3
sta $4
lda #$1
sta $5

lda $0
and #$2
beq $f
lda $6
add $2
sta $6
lda #$0
sta $5
lda $1
inc
sta $1

lda $0
and #$4
beq $f
lda $6
add $3
sta $6
lda #$0
sta $5
lda $1
dec
sta $1

lda $5
beq $2
jmp $0

lda $2
and #$f0
bne $c
lda $6
and #$f
sta $2
lda $6
add #$5e
sta $6

lda $3
and #$f0
bne $c
lda $6
and #$f
sta $3
lda $6
add #$99
sta $6

lda $4
and #$f0
bne $c
lda $6
and #$f
sta $4
lda $6
add #$eb
sta $6

lda $1
and #$f0
sub #$d0
beq $8
lda $1
and #$f
add #$d0
sta $1

lda $7
and #$f
bne $10
lda $7
and #$80
bne $6
lda #$85
sta $7
bne $4
lda #$5
sta $7

lda $7
dec
sta $7

lda $7
and #$80
beq $16
lda $2
add #$f
sta $2
lda $3
add #$f
sta $3
lda $4
add #$f
sta $4

lda #$0
beq $12
lda $2
add #$11
sta $2
lda $3
add #$11
sta $3
lda $4
add #$11
sta $4

lda $2
sub $1
beq $c
lda $3
sub $1
beq $6
lda $4
sub $1
bne $4
lda #$1
sta $5

lda #$0
sta $0
lda $0
beq $fc
jmp $1c