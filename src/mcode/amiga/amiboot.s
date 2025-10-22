
;Amiga-Bootsektor für Return of Medusa (II)
;by Till Bubeck, Ziegeleistr. 28, 7056 Weinstadt
;Korrigiert für Prozessoren 68010-68030 by Christian A. Weber, Zürich
;Dieser Bootsektor lädt die Tracks 1+2 nach $65000 und startet sie ohne
;sie vorher zu relozieren!
;Länge der FATs: 0+0, Directory: 1 Sektor

                OPT P+
                DEFAULT 4

                DC.B 'DOS',0    ;($0) Diskettenkennung
                DC.L 0          ;($4) Checksumme
                DC.L $00000370  ;($8)

                bra.s   start_boot           ;($c)
                nop                          ;($e)

                DC.B 2          ;($10) 2 Fats
                DC.B $10,$00    ;($11) Verzeichnis = 1 Sektor
                DC.B 19,20,21   ;($13)
                DC.B 0,0        ;($16) 0 Sektoren pro FAT
                DC.B 11,0       ;($18) 11 Sektoren pro Track

track_buffer    EQU $00050000   ;Hier den Track hinladen

start_boot:     lea     super_an(PC),A5
                movea.l 4.w,A6               ;ExecBase
                jsr     -30(A6)              ;Supervisor-Modus (NUR SO GEHT'S!)
super_an:       lea     $00080000,SP         ;SSP setzen
                movem.l vbl(PC),D0-D7
                movem.l D0-D7,$00000100.w
                move.l  #$00000100,$0000006C.w ;VBL-Vektor setzen

                move.w  #%0011111111111111,$00DFF09A ;Alles aus
                move.w  #%0011111111111111,$00DFF09C ;Request Bits löschen
                move.w  #%1100000000100000,$00DFF09A ;VBL an
                move    #$2000,SR

                move.w  #$0EEE,D7
fade_out:       move.w  D7,$00DFF180
                beq.s   fade_ende

                clr.w   $00000024.w          ;VBL-Counter rücksetzen
wait_vbl:       cmpi.w  #2,$00000024.w       ;2 VBLs warten
                bne.s   wait_vbl

                sub.w   #$0111,D7
                bra.s   fade_out

fade_ende:      moveq   #0,D0
                moveq   #15,D7
                lea     $00DFF180,A0
clr_loop:       move.l  D0,(A0)+
                dbra    D7,clr_loop

                bsr     select               ;DF0: selektieren

                moveq   #0,D6                ;Track 0 anfahren
                bsr     seek_track

                lea     $00065000,A0
                moveq   #8,D0                ;8 Sektoren noch
                moveq   #3,D5
                bsr.s   read_sektoren
                moveq   #1,D6                ;Track 1
                bsr     seek_track
                moveq   #11,D0
                lea     $00065000+8*512,A0
                bsr.s   read_sektoren

                jmp     $00065000            ;und starten

;Liest ab Startsektor Sektoren ein. Gibt in D5 512*n zurück.
;-> D0: Anzahl der Sektoren
;   D5: Startsektor
;   A0: Ladeadresse
read_sektoren:  movem.l D0-D4/D6-A6,-(SP)

                move.w  D0,-(SP)             ;Anzahl retten
read_error:     bsr.s   read_track           ;Track lesen
                move.w  (SP),D0              ;Anzahl holen
                move.w  D5,D7                ;Aktueller Sektor
                bra.s   ss_dbra
sektor_trans:   tst.l   (A1)                 ;Wurde überhaupt was geladen?
                beq.s   lesefehler
                move.w  track(PC),D1
                cmp.b   1(A1),D1             ;Richtiger Track?
                bne.s   lesefehler
                cmp.b   2(A1),D7             ;Sektor gefunden?
                bne.s   nxt_sektor3
                moveq   #127,D6
                addq.l  #4,A1                ;Miniheader überspringen
ttz:            move.l  (A1)+,(A0)+          ;eintragen
                dbra    D6,ttz
                addq.w  #1,D7                ;nächster Sektor
ss_dbra:        lea     track_buffer,A1
                dbra    D0,sektor_trans

                move.w  (SP)+,D0
                move.w  D0,D5
                mulu    #512,D5              ;Soviel Bytes wurden gelesen
                movem.l (SP)+,D0-D4/D6-A6
                rts

nxt_sektor3:    lea     512+4(A1),A1
                bra.s   sektor_trans

lesefehler:     move.w  track(PC),-(SP)      ;gewünschter Track
                moveq   #0,D6
                bsr     seek_track           ;Restore ausführen
                move.w  (SP)+,D6
                bsr     seek_track           ;und nochmal anfahren
                bra.s   read_error


;Liest einen Track von Diskette ein und dekodiert ihn.
read_track:     movem.l D0-A6,-(SP)

                lea     track_buffer,A0      ;hier hin lesen

                lea     $00DFF000,A5
                move.w  #%1000001000010000,$0096(A5) ;Disk DMA an
                move.w  #%0111111100000000,$009E(A5) ;alle Bits aus
                move.w  #%1001010100000000,$009E(A5) ;MFM+Sync
                move.w  #$4489,$007E(A5)     ;Sync-Wert
                move.w  #$4489,(A0)+         ;1. Sync eintragen
                move.w  #$4000,$0024(A5)     ;Disk-DMA aus
                move.l  A0,$0020(A5)         ;DMA-Adresse
                bsr     wait_ready
                move.w  #12980,D0            ;Anzahl Bytes
                lsr.w   #1,D0
                ori.w   #$8000,D0            ;DMA lesen
                move.w  D0,$0024(A5)
                move.w  D0,$0024(A5)
                bsr     wait_dma

                subq.l  #2,A0                ;Bufferadresse
                bsr.s   decode_track

                movem.l (SP)+,D0-A6
                rts


;Diese Routine decodiert einen Track.
;-> A0.L: Adresse des Tracks
decode_track:   movem.l D0-A6,-(SP)

                movea.l A0,A1                ;hier wieder hin

                lea     12800(A0),A3         ;Ende des Buffers
                move.l  #$55555555,D2        ;zum MFM dekodieren

such_sektor:    cmpi.w  #$4489,(A0)          ;Sync?
                bne.s   no_sync
weitere_sync:   cmpi.w  #$4489,2(A0)         ;2. Sync?
                bne.s   keine_zweite
                addq.l  #2,A0                ;Eine Sync weitergehen
                bra.s   weitere_sync
keine_zweite:   cmpi.b  #$55,2(A0)           ;$FF (Format-Mark)?
                bne.s   no_sync
                cmpi.b  #$55,6(A0)           ;2. Mark?
                bne.s   no_sync

                move.l  2(A0),D0
                move.l  6(A0),D1
                and.l   D2,D0
                and.l   D2,D1
                add.l   D0,D0
                or.l    D1,D0
                move.l  D0,(A1)              ;Buffer Mark merken

                lea     2(A0),A2             ;Sync nicht CRC
                moveq   #40,D0               ;40 Bytes
                bsr.s   calc_crc

                move.l  42(A0),D1
                move.l  46(A0),D3
                and.l   D2,D1
                and.l   D2,D3
                add.l   D1,D1
                or.l    D3,D1
                cmp.l   D1,D0                ;CRC korrekt?
                bne.s   crc_error

                lea     58(A0),A2            ;Datenbuffer
                move.w  #1024,D0
                bsr.s   calc_crc

                move.l  50(A0),D1
                move.l  54(A0),D3
                and.l   D2,D1
                and.l   D2,D3
                add.l   D1,D1
                or.l    D3,D1
                cmp.l   D1,D0                ;CRC korrekt?
                bne.s   crc_error

                lea     58(A0),A2            ;Datenbuffer
                addq.l  #4,A1                ;Header Ok
                bsr.s   decode_sektor

crc_error:
no_sync:        addq.l  #2,A0                ;nächstes Wort
                cmpa.l  A3,A0
                blt.s   such_sektor

                clr.l   (A1)                 ;Ende der Sektoren

                movem.l (SP)+,D0-A6
                rts

;Diese Routine berechnet eine Prüfsumme über den angegebenen Bereich
;-> A2.L: Adresse des Bereichs (wird NICHT erhöht)
;   D0.L: Länge in Bytes
calc_crc:       movem.l D1-A6,-(SP)

                move.w  D0,D1
                lsr.w   #2,D1                ;Langworte
                subq.w  #1,D1
                moveq   #0,D0
crc_loop:       move.l  (A2)+,D2
                eor.l   D2,D0
                dbra    D1,crc_loop

                andi.l  #$55555555,D0        ;Taktbits raus

                movem.l (SP)+,D1-A6
                rts

;Diese Routine dekodiert einen Sektor von A0 nach A1.
;-> A2.L: Quelle
;   A1.L: Ziel (wird erhöht)
decode_sektor:  movem.l D0-A0/A2-A6,-(SP)

                moveq   #127,D7              ;256 Langworte
                move.l  #$55555555,D2
dat_loop:       move.l  (A2)+,D0
                move.l  508(A2),D1
                and.l   D2,D1
                and.l   D2,D0
                add.l   D0,D0
                or.l    D1,D0
                move.l  D0,(A1)+
                dbra    D7,dat_loop

                movem.l (SP)+,D0-A0/A2-A6
                rts

;Routine selektiert Laufwerk df0:.
select:         move.b  #%01111111,$00BFD100 ;DF0: selektieren
                nop
                nop
                move.b  #%01110111,$00BFD100 ;Motor an, DF0:
                nop
                nop
                bra     wait_ready           ;Auf Laufwerk warten

;Diese Routine deselektiert Laufwerk DF0:
deselect:       move.b  #%11111111,$00BFD100 ;DF0: deselektieren
                nop
                nop
                move.b  #%11110111,$00BFD100 ;Motor aus
                rts


;Routine fährt einen bestimmten Track an und selektiert entspr. Seite
;-> D6.W: gewünschter Track
seek_track:     move.w  track(PC),D0         ;Ist dies der 1. Aufruf?
                bpl.s   seek                 ;Nein, direkt anfahren

                move.l  D6,-(SP)             ;gewünschter Track merken
                moveq   #0,D6
                bsr.s   seek                 ;zuerst Restore
                move.l  (SP)+,D6


;Fährt Track an
;-> D6.W: Track
seek:           movem.l D0-A6,-(SP)

                move.w  track(PC),D7         ;aktueller Track
                lsr.w   #1,D7                ;tatsächliche Tracknummer
                lsr.w   #1,D6                ;gewünschter Track
                bcs.s   untere_seite
                bset    #2,$00BFD100         ;Obere Seite
                bra.s   seeken
untere_seite:   bclr    #2,$00BFD100

seeken:         tst.w   D6                   ;Track 0?
                beq.s   restore

seek_loop:      cmp.w   D7,D6                ;Sind wir schon auf Track?
                beq.s   seek_end
                bgt.s   nach_innen
                bsr.s   aussen
                subq.w  #1,D7
                bra.s   seek_loop
nach_innen:     bsr.s   innen
                addq.w  #1,D7
                bra.s   seek_loop

restore:        btst    #4,$00BFE001         ;Kopf auf Spur 0?
                beq.s   seek_end
                bsr.s   aussen
                bra.s   restore

seek_end:       movem.l (SP)+,D0-A6
                lea     track(PC),A0
                move.w  D6,(A0)              ;angekommen
                rts

;Bewegt Kopf auf Track 80 zu:
innen:          movem.l D0-A6,-(SP)
                bsr     wait_ready
                bclr    #1,$00BFD100
                bclr    #0,$00BFD100
                bset    #0,$00BFD100
                bsr.s   minipause
                bsr.s   wait_ready
                movem.l (SP)+,D0-A6
                rts

;Bewegt Kopf auf Track 0 zu:
aussen:         movem.l D0-A6,-(SP)
                bsr.s   wait_ready
                bset    #1,$00BFD100
                bclr    #0,$00BFD100
                bset    #0,$00BFD100
                bsr.s   minipause
                bsr.s   wait_ready
                movem.l (SP)+,D0-A6
                rts

pause:          move.w  #$C000,D7
loop2:          tst.w   $00DFF004            ;Zeit verzögern
                dbra    D7,loop2
                rts

minipause:      move.w  #$2000,D7
mini2:          tst.w   $00DFF004            ;Zeit verzögern
                dbra    D7,mini2
                rts


;Diese Routine wartet auf das Ende einer DMA-Floppyaktion
;<- D0.W: 0=Kein Timeout, -1=Timeout
wait_dma:       movem.l D1-A6,-(SP)

                move.l  #$00020000,D0
                move.w  #$0002,$00DFF09C
wait_loop:      move.w  $00DFF01E,D1
                btst    #1,D1
                bne.s   fertig_hiller
                subq.l  #1,D0
                bne.s   wait_loop

fertig_hiller:  move.w  #$0000,$00DFF024     ;DMA ausschalten
                movem.l (SP)+,D1-A6
                rts

;Wartet auf die Ausführung eines normalen Diskbefehls:
wait_ready:     btst    #5,$00BFE001
                bne.s   wait_ready
                rts

vbl:            addq.w  #1,$00000024.w       ;VBL-Counter erhöhen
                move.w  #%0000000000100000,$00DFF09C ;Request löschen
vbl_rte:        rte

track:          DC.W -1

                END
