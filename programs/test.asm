  .inesprg 1
  .ineschr 1
  .inesmap 0
  .inesmir 1

  .bank 0
  .org $8000

RESET:
    sei
    cld

    lda #$01
    cmp #$00
branch_label:
    bne branch_label


    lda #$10
    sta $F0
    lda #$81
    sta $F1

    jmp [$00F0]

loop:
    jmp loop

NMI:
    inc $00
    rti

  .org $8110
label:
    lda #$DE
    sta $00
    lda #$AD
    sta $01
    jmp loop

  .bank 1
  .org $FFFA

  .dw NMI
  .dw RESET
  .dw 0

