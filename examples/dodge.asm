reset
  lda #$D7
  sta $1
  lda #$d
  sta $2
  lda #$58
  sta $3
  lda #$a3
  sta $4
  lda #$1
  sta $5

start
  lda $0
  and #$2
  beq moveLeft
  lda $6
  add $2
  sta $6
  lda #$0
  sta $5
  lda $1
  inc
  sta $1

moveLeft
  lda $0
  and #$4
  beq checkPause
  lda $6
  add $3
  sta $6
  lda #$0
  sta $5
  lda $1
  dec
  sta $1

checkPause
  lda $5
  beq moveP1
  jmp reset

moveP1
  lda $2
  and #$f0
  bne moveP2
  lda $6
  and #$f
  sta $2
  lda $6
  add #$5e
  sta $6

moveP2
  lda $3
  and #$f0
  bne moveP3
  lda $6
  and #$f
  sta $3
  lda $6
  add #$99
  sta $6

moveP3
  lda $4
  and #$f0
  bne checkBound
  lda $6
  and #$f
  sta $4
  lda $6
  add #$eb
  sta $6

checkBound
  lda $1
  and #$f0
  sub #$d0
  beq checkDir
  lda $1
  and #$f
  add #$d0
  sta $1

checkDir
  lda $7
  and #$f
  bne decCounter
  lda $7
  and #$80
  bne dir0
  lda #$85
  sta $7
  bne decCounter
dir0
  lda #$5
  sta $7

decCounter
  lda $7
  dec
  sta $7

movePLeft
  lda $7
  and #$80
  beq movePRight
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
  beq collisionP1
movePRight
  lda $2
  add #$11
  sta $2
  lda $3
  add #$11
  sta $3
  lda $4
  add #$11
  sta $4

collisionP1
  lda $2
  sub $1
  beq collisionTrue
  lda $3
  sub $1
  beq collisionTrue
  lda $4
  sub $1
  bne startWait
collisionTrue
  lda #$1
  sta $5

startWait
  lda #$0
  sta $0
wait
  lda $0
  beq wait
  jmp start