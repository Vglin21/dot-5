  lda #$1
  sta $3
  
  lda $3
  bne $e
  lda $4
  dec
  sta $4
  lda $5
  dec
  sta $5
  lda #$0
  beq $7
  dec
  sta $4
  lda #$8
  sta $5
  
  lda $6
  add #$d6
  sta $6
  
  lda $7
  beq $3
  dec
  sta $7
  
  lda $0
  and #$10
  beq $12
  lda #$0
  sta $3
  lda $6
  add $4
  sta $6
  lda $7
  bne $4
  lda #$4
  sta $7
  
  lda $0
  and #$8
  beq $16
  lda #$0
  sta $3
  lda $6
  add $4
  sta $6
  lda $7
  bne $6
  lda #$d2
  sta $1
  sta $2
  bne $8
  lda #$c2
  sta $1
  add #$10
  sta $2
  
  lda $7
  beq $a
  lda #$b2
  sta $1
  add #$10
  sta $2
  bne $e
  lda $1
  sub #$d2
  beq $8
  lda #$c2
  sta $1
  add #$10
  sta $2
  
  lda $4
  and #$f
  bne $10
  lda $6
  and #$1
  bne $6
  lda #$cf
  sta $4
  bne $4
  lda #$df
  sta $4
  
  lda $5
  and #$f
  bne $10
  lda $6
  and #$1
  bne $6
  lda #$cf
  sta $5
  bne $4
  lda #$df
  sta $5
  
  lda $4
  sub $1
  bne $4
  lda #$1
  sta $3
  
  lda $4
  sub $2
  bne $4
  lda #$1
  sta $3
  
  lda $5
  sub $1
  bne $4
  lda #$1
  sta $3
  
  lda $5
  sub $2
  bne $4
  lda #$1
  sta $3
  
  lda #$0
  sta $0
  lda $0
  beq $fc
  jmp $c