  .inesprg 1
  .ineschr 1
  .inesmap 0
  .inesmir 1

  .bank 0
  .org $8000

RESET:
    sei
    cld
    lda #$7F
    jsr sub
    lda #$00

loop:
    jmp loop

sub:
    adc #$11
    ldx #$FF
    rts

NMI:
    inc $00
    rti

  .bank 1
  .org $FFFA

  .dw NMI
  .dw RESET
  .dw 0

