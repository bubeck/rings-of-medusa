
;Bootsektor für Signs of Medusa (II)
;by Till Bubeck, Ziegeleistr. 28, 7056 Weinstadt
;Dieser Bootsektor lädt die Tracks 1+2 nach $70000 und startet sie ohne
;sie vorher zu relozieren!
;Länge der FATs: 2+2, Directory: 5 Sektoren (80 Einträge)->1. File auf 1,1

                OPT P+

                move.l  #$0BFFFF,-(SP)
                trap    #13
                addq.l  #4,SP
                btst    #2,D0           ;Control?
                bne     raus_hier

                lea     $080000,SP      ;SP setzen

                clr.l   $24.w           ;VBL-Counter löschen
                lea     $0150.w,A0
                movem.l vbl(PC),D0-D7
                movem.l D0-D7,(A0)      ;Programm übertragen
                move.l  A0,$70.w        ;VBL setzen
                lea     $0150+vbl_rte-vbl.w,A0

                clr.b   $FFFFFA07.w
                andi.b  #%1000000,$FFFFFA09.w ;Alle IRQ's (außer Tastatur) aus

                moveq   #$12,D0         ;disable Mouse
                bsr     send_ikbd
                moveq   #$1A,D0         ;disable Joysticks
                bsr     send_ikbd

                move.w  #$0666,D7
fade_out:       move.w  D7,$FFFF8240.w
                beq.s   fade_ende

                clr.l   $24.w
vbl_wait:       cmpi.w  #2,$24.w        ;2 VBLs warten
                bne.s   vbl_wait

                sub.w   #$0111,D7
                bra.s   fade_out

fade_ende:      moveq   #0,D0
                moveq   #7,D7
                lea     $FFFF8240.w,A0
clr_loop:       move.l  D0,(A0)+
                dbra    D7,clr_loop

                moveq   #$13,D0         ;und alles stoppen
                bsr     send_ikbd

                move.l  #$0E0E0505,$FFFF8800.w ;A: selektieren

                lea     $FFFF8604,A3    ;FDC
                lea     2(A3),A4        ;DMA
                lea     $FFFFFA01,A5    ;MFP

                moveq   #1,D6           ;Starttrack
                bsr.s   seek_track

                moveq   #10,D0          ;10 Sektoren lesen
                lea     $070000,A0      ;hierhin
                bsr.s   read_sektoren

                moveq   #2,D6
                bsr.s   seek_track
                moveq   #10,D0
                bsr.s   read_sektoren

                jmp     -20*512(A0)     ;nach $70000 springen


;Liest ab Startsektor Sektoren ein. Gibt in D5 512*n zurück.
;-> D0: Anzahl der Sektoren
;   A0: Ladeadresse
read_sektoren:  move.w  D0,-(SP)        ;Anzahl retten
read_error:     move.w  (SP),D0         ;Anzahl holen
                mulu    #512,D0
                move.w  D0,D5           ;Anzahl an Bytes merken
                bsr.s   setdma
                bsr     rdtoggle
                move.w  (SP),D7         ;Sektorcounter
                bsr     fdcout
                move.w  #$84,(A4)       ;Sektorregister
                moveq   #1,D7           ;Startsektor holen
                bsr     fdcout
                move.w  #$80,(A4)       ;Commandoregister
                move.w  #$90,D7         ;Read multiple
                bsr.s   fdcout
                bsr     waitfertig
                tst.w   D0              ;Alles Okay?
                bne.s   read_error
                tst.w   (SP)+           ;D0 runternehmen
raus_hier:      rts

restore_fdc:    move.w  #$80,(A4)       ;Kommandoregister
                move.w  #$03,D7         ;Restore
                lea     dflag(PC),A6
                clr.b   (A6)            ;Kein DMA-Transfer
                bsr.s   fdcout
                bsr.s   waitfertig
                cmpi.w  #4,D0           ;Spur 0 erreicht?
                bne.s   restore_fdc

;Fährt Track D6 an, macht ggf. Restore
seek_track:     move.w  #$86,(A4)       ;Datareg.
                move.w  D6,D7           ;Starttrack setzen
                bsr.s   fdcout

                move.w  #$80,(A4)       ;Commandoreg.
                move.w  #$17,D7         ;Seek mit Verify
                lea     dflag(PC),A6
                clr.b   (A6)            ;Kein DMA-Transfer
                bsr.s   fdcout
                bsr.s   waitfertig      ;auf Ausführung warten
                andi.w  #%11000,D0      ;nur CRC/RNF
                bne.s   restore_fdc     ;nicht gefunden, Restore first
                rts

;Setzt die DMA-Adresse
setdma:         pea     (A0)            ;Hier hinlesen
                move.b  3(SP),7(A4)     ;Adresse in DMA
                move.b  2(SP),5(A4)
                move.b  1(SP),3(A4)
                addq.l  #4,SP
                adda.w  D0,A0           ;Endadresse berechnen
                lea     dend(PC),A6
                move.l  A0,(A6)
                lea     dflag(PC),A6
                st      (A6)            ;DMA-Transfer findet statt
                rts

rdtoggle:       move.w  #$90,(A4)
                move.w  #$0190,(A4)
                move.w  #$90,(A4)
                rts

fdcout:         bsr.s   waitfdc
                move.w  D7,(A3)         ;Wert schreiben

waitfdc:        move    SR,-(SP)
                move.l  D0,-(SP)
                moveq   #$30,D0
waitloop:       dbra    D0,waitloop
                move.l  (SP)+,D0
                move    (SP)+,SR
                rts

; wartet auf vollständige Ausführung
; veränderte Register: ?
waitfertig:     move.l  #$060000,D7     ;Timeout Zähler
poll:           btst    #5,(A5)         ;IRQ ausgelöst?
                beq.s   fix_und_fertig
                subq.l  #1,D7
                beq.s   fix_und_fertig  ;Normalerweise Timeout
                lea     dflag(PC),A6
                tst.b   (A6)            ;DMA-Transfer?
                beq.s   poll            ;Nein, weiterwarten

                moveq   #0,D0
                move.b  $FFFF8609,D0
                lsl.w   #8,D0
                move.b  $FFFF860B,D0    ;DMA-Adresse holen
                lsl.l   #8,D0
                move.b  $FFFF860D,D0
                cmp.l   dend(PC),D0
                blt.s   poll

                lea     dflag(PC),A6
                clr.b   (A6)
fix_und_fertig: move.w  (A3),D0         ;FDC-Status lesen
                andi.w  #%11100,D0      ;CRC-Lost etc. isolieren
                move.w  #$80,(A4)       ;Commandoregister
                move.w  #$D0,D7
                bsr.s   fdcout
                move.w  #250,D1
as_time_goes:   dbra    D1,as_time_goes
                rts


send_ikbd:      btst    #1,$FFFFFC00.w  ;ACIA bereit?
                beq.s   send_ikbd       ;nein
                move.b  D0,$FFFFFC02.w  ;und senden
                rts

vbl:            addq.w  #1,$24.w        ;VBL-Counter erhöhen
vbl_rte:        rte

dflag:          DC.B 0          ;Flag, ob DMA-Transfer
                EVEN
dend:           DC.L 0          ;DMA-Endadresse

                END
