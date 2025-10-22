;************************************************************************
;* Loader für Medusa.prg
;* Lädt MEDUSA.PRG an die Ladeadresse, reloziert es und springt es an
;* ╜1990 by Till Bubeck, Ziegeleistr. 28, 7056 Weinstadt, 07151/66437
;************************************************************************

disk_version    EQU 0           ;0=Disk_Version,1=Testversion
ladeadresse     EQU $00000300

                IF disk_version=0
                ORG $00070000
                DEFAULT 4
                OPT X-
                ELSE
                OPT X+
                DEFAULT 1

                clr.l   -(SP)
                move.w  #$0020,-(SP)
                trap    #1

                clr.w   -(SP)
                pea     -1.w
                pea     -1.w
                move.w  #5,-(SP)
                trap    #14             ;Low Resolution
                lea     12(SP),SP

                st      $0000043E.w     ;Floppy ist aktiv
                ENDC

                bra     drüber

                DC.B 'Dear Cracker!    "The Return of Medusa" is written'
                DC.B ' by Chaos, Inc. of Delta Force (member of THE UNION). '
                DC.B 'So please do NOT crack "Medusa"!     Thanx! '

drüber:         lea     bild(PC),A0
                movem.l (A0)+,D0-D7     ;Farbpalette holen
                movem.l D0-D7,$FFFF8240.w

                lea     $FFFF8200,A1    ;Adresse VIDEO-Counter
                movep.w 1(A1),D0
                lsl.l   #8,D0
                movea.l D0,A1           ;Adresse Screen
                lea     56*160+32(A1),A1
                moveq   #54,D7          ;Anzahl Scanlines
show2:          moveq   #21,D6          ;Breite in Longs
show1:          move.l  (A0)+,(A1)+
                dbra    D6,show1        ;"Rings of Medusa II"
                lea     72(A1),A1
                dbra    D7,show2

                movea.l D0,A1           ;Adresse Screen
                lea     150*160+40(A1),A1
                moveq   #8,D7           ;Anzahl Scanlines
show3:          moveq   #17,D6          ;Breite in Longs
show4:          move.l  (A0)+,(A1)+
                dbra    D6,show4        ;"A T.Bubeck and T.Zimmermann Game"
                lea     88(A1),A1
                dbra    D7,show3

                move    #$2700,SR       ;IRQs sperren

                lea     memtest(PC),A0
                lea     $00000200,A1
trans:          move.l  (A0),(A1)       ;Prüfprogramm schreiben
                cmpm.l  (A0)+,(A1)+     ;richtig geschrieben?
                bne.s   memerror
                cmpa.l  #laden,A0
                blt.s   trans

tt_weiter:      lea     laden(PC),A0
                move.l  A0,8.w          ;Buserror setzen
                move.l  A0,12.w         ;Adresserror setzen

                jmp     $00000200.w     ;und Speicher prüfen

memtest:        lea     $00000300.w,A0
                move.l  #$55555555,D2   ;Prüfmuster
                move.l  #$AAAAAAAA,D3
test:           move.l  (A0),D0
                move.l  D2,(A0)
                cmp.l   (A0),D2
                bne.s   memerror
                move.l  D3,(A0)
                cmp.l   (A0),D3
                bne.s   memerror
                move.l  D0,(A0)+        ;und alten Inhalt wieder hin
                bra.s   test

memerror:       jmp     laden

laden:          subq.l  #4,A0           ;Höchste Speicheradresse
                move.l  A0,$00000148.w  ;hier merken
                cmpa.l  #512*1024-10,A0 ;midestens 512 Kbyte?
                bge.s   enough_mem

                pea     less_mem(PC)
                move.w  #9,-(SP)
                trap    #1
                addq.l  #6,SP
warte:          bra.s   warte

enough_mem:     move.l  $00000070.w,old_vbl
                move.l  #vbl,$00000070.w

                move    #$2300,SR       ;IRQ freigeben

                moveq   #-1,D0
                move.l  D0,-(SP)
                move.l  D0,-(SP)
                move.l  D0,-(SP)
                move.w  #16,-(SP)       ;Keyboard-Scancodes holen
                trap    #14
                lea     14(SP),SP
                lea     scancodes(PC),A6
                cmp.w   #16,D0          ;Etwa noch Boot-Roms?
                beq.s   boot_roms
                movea.l D0,A6           ;Adresse des Parameterblocks retten
                movea.l 8(A6),A6        ;Adresse der CAPS-Lock Tabelle

                move.w  #11,-(SP)
                trap    #1              ;Cconis
                addq.l  #2,SP
                tst.w   D0              ;Zeichen da?
                beq.s   boot_roms
                move.w  #7,-(SP)
                trap    #1              ;Zeichen holen
                addq.l  #2,SP
                move.l  #512*1024,D7    ;512 Kbyte
                cmp.w   #'0',D0
                beq.s   set_mem
                add.l   D7,D7           ;1 Mbyte
                cmp.w   #'1',D0
                beq.s   set_mem
                cmp.w   #'5',D0
                bne.s   kein_50
                bsr     vsync
                bset    #1,$FFFF820A.w  ;50 Hz an
kein_50:        cmp.w   #'6',D0
                bne.s   boot_roms
                bsr     vsync
                bclr    #1,$FFFF820A.w  ;60 Hz an
                bra.s   boot_roms

set_mem:        move.l  D7,$00000148.w  ;Spezielle Speicherkonfiguration setzen

boot_roms:      lea     $00000200.w,A1  ;Hier sollen die Scancodes hin
                moveq   #127,D7         ;128 Bytes pro Tabelle
trans_scan:     move.b  (A6)+,(A1)+
                dbra    D7,trans_scan

                lea     $00000280.w,A0
                movem.l trap_handler(PC),D0-D7/A1-A6
                movem.l D0-D7/A1-A6,(A0)
                move.l  A0,$00000084.w  ;Trap #1
                move.l  A0,$00000088.w  ;Trap #2
                move.l  A0,$000000B4.w  ;Trap #13
                move.l  A0,$000000B8.w  ;Trap #14

                move.l  old_vbl(PC),$00000070.w

                move.w  #511,-(SP)      ;End_Offset
                move.w  #1,-(SP)        ;End_Sektor
                clr.w   -(SP)           ;End_Track
                clr.w   -(SP)           ;Start_Offset
                move.w  #1,-(SP)        ;Start_Sektor
                clr.w   -(SP)           ;Start_Track
                clr.w   -(SP)           ;A:
                pea     bitfeld+512(PC)
                bsr     mcode68         ;Bootsektor lesen
                lea     18(SP),SP

                lea     bitfeld+512(PC),A0
                move.b  $0012(A0),D0
                lsl.w   #8,D0
                move.b  $0011(A0),D0    ;Verzeichnisgröße
                ext.l   D0
                divu    #16,D0
                move.w  D0,D1           ;Anzahl an Sektoren fürs Directory
                swap    D0
                tst.w   D0              ;noch ein Rest übrig?
                beq.s   geht_auf
                addq.w  #1,D1           ;+1 für den Rest
geht_auf:       move.b  $0017(A0),D0
                lsl.w   #8,D0
                move.b  $0016(A0),D0    ;Größe der FAT
                add.w   D0,D0           ;da 2 FATs
                addq.w  #1,D0           ;für Bootsektor
                move.w  D0,D2           ;Beginn des Directorys
                add.w   D1,D2           ;+Größe des Directory=erster Datensektor
                move.w  D2,first_sek
                ext.l   D0
                divu    #10,D0          ;10 Sektoren/Track

                move.w  #511,-(SP)      ;End_Offset
                swap    D0
                addq.w  #1,D0           ;Sektoren starten bei Eins
                move.w  D0,-(SP)        ;End_Sektor
                swap    D0
                move.w  D0,-(SP)        ;End_Track
                clr.w   -(SP)           ;Start_Offset
                swap    D0
                move.w  D0,-(SP)        ;Start_Sektor
                swap    D0
                move.w  D0,-(SP)        ;Start_Track
                clr.w   -(SP)           ;A:
                pea     bitfeld+1024+512(PC)
                bsr     mcode68         ;Directory lesen
                lea     18(SP),SP

                lea     bitfeld+1024+512(PC),A0
                lea     medusa(PC),A1
                moveq   #15,D6          ;16 Einträge pro Directory
nxt2:           moveq   #0,D7
kein_ende:      move.b  0(A0,D7.w),D0   ;Zeichen von Diskette lesen
                cmp.b   0(A1,D7.w),D0   ;MEDUSA.PRG?
                bne.s   nxt_entry
                addq.w  #1,D7
                tst.b   0(A1,D7.w)
                bne.s   kein_ende
                move.b  27(A0),D0
                lsl.w   #8,D0
                move.b  26(A0),D0       ;Startcluster der Datei holen
                subq.w  #2,D0
                add.w   D0,D0           ;ein Cluster hat 2 Sektoren
                add.w   D0,first_sek    ;1. Sektor der Daten
                move.b  31(A0),D0
                lsl.w   #8,D0
                move.b  30(A0),D0
                lsl.l   #8,D0
                move.b  29(A0),D0       ;LÄnge der Datei holen
                lsl.l   #8,D0
                move.b  28(A0),D0
                move.l  D0,medusa_len
                bra.s   load_medusa

nxt_entry:      lea     32(A0),A0       ;Einen Eintrag weiter
                dbra    D6,nxt2

flash:          move.w  #$0777,$FFFF8240.w
flash_err:      not.w   $FFFF8240.w
                bra.s   flash_err

load_medusa:    pea     prg_header(PC)
                pea     28.w            ;28 Bytes lesen
                clr.l   -(SP)           ;Offset 0
                clr.w   -(SP)           ;Archiv
                bsr     mcode61         ;und lesen
                lea     14(SP),SP

                movea.l textlen(PC),A0
                adda.l  datalen,A0      ;Länge des Programms

                pea     ladeadresse+256 ;Hier hin laden
                move.l  A0,-(SP)        ;Länge
                pea     28.w            ;Header überspringen
                clr.w   -(SP)           ;Archiv
                bsr     mcode61         ;Programm lesen
                lea     14(SP),SP

                movea.w #28,A0          ;Größe des Headers
                adda.l  textlen,A0
                adda.l  datalen,A0
                lea     ladeadresse+256,A1 ;Hier startet das Programm
                adda.l  A0,A1           ;Start des BSS
                adda.l  symlen,A0       ;=Offset zur Relozinfo

                pea     (A1)            ;Ladeadresse
                move.l  medusa_len(PC),D0
                sub.l   A0,D0           ;-Startoffset=Länge der Daten
                move.l  D0,-(SP)        ;Länge
                pea     (A0)            ;Startoffset der Relozinfo
                clr.w   -(SP)           ;Archiv
                bsr     mcode61
                lea     14(SP),SP

                lea     ladeadresse+256,A0 ;Hier startet das Programm
                move.l  A0,D0           ;Als Relozwert
                tst.l   (A1)            ;Relozdaten vorhanden?
                beq.s   no_reloz        ;Nein
                adda.l  (A1)+,A0        ;Erste zu relozierende Adresse
reloz:          add.l   D0,(A0)         ;relozieren
no1:            moveq   #0,D6
                move.b  (A1)+,D6        ;nächsten Wert holen
                beq.s   no_reloz
                cmp.b   #1,D6           ;254 überspringen?
                beq.s   eins
                adda.l  D6,A0           ;Soviel Bytes weitergehen
                bra.s   reloz
eins:           lea     254(A0),A0
                bra.s   no1

no_reloz:       lea     ladeadresse,A0  ;Hier liegt die Basepage
                lea     256(A0),A1      ;Startadresse Text
                move.l  A1,8(A0)        ;eintragen
                move.l  textlen(PC),12(A0)
                adda.l  textlen,A1      ;Startadresse DATA
                move.l  A1,16(A0)
                move.l  datalen(PC),20(A0)
                adda.l  datalen,A1
                move.l  A1,24(A0)       ;Startadresse BSS
                move.l  bsslen(PC),28(A0)
                lea     128(A0),A1
                move.l  A1,32(A0)       ;Zeiger auf DTA
                clr.l   36(A0)          ;kein Parent-PD
                clr.l   40(A0)          ;reserved
                clr.l   44(A0)          ;kein Environment

                lea     ladeadresse+256,A0 ;Startadresse Text
                pea     -256(A0)        ;Adresse der Basepage auf Stack
                jsr     (A0)            ;und los gehts

;void floppy_read(void *adr,int strack,int ssektor,int soffset,
;                          int etrack,int esektor,int eoffset);
;-> 24(a6): eoffset
;   22(a6): esektor
;   20(a6): etrack
;   18(a6): soffset
;   16(a6): ssektor
;   14(a6): strack
;   12(a6): Laufwerk (Bit 0=Laufwerk, Bit 1=Seite)
;    8(a6): adr

mcode68:        link    A6,#0
                movem.l D0-A5,-(SP)

                move.w  12(A6),D0       ;Laufwerk holen
                bsr     select          ;selektieren

                lea     $FFFF8604,A3    ;FDC
                lea     2(A3),A4        ;DMA
                lea     $FFFFFA01,A5    ;MFP
                movea.l 8(A6),A2        ;Ladeadresse

                move.w  14(A6),D6       ;Starttrack
                bsr     seek_track

                moveq   #1,D0           ;1 Sektor lesen
                lea     bitfeld(PC),A0  ;hierhin
                bsr     read_sektoren

                move.w  14(A6),D4       ;Starttrack
                move.w  20(A6),D5       ;Endtrack
                cmp.w   D4,D5           ;nur auf einem Track lesen?
                beq.s   nur_ein_track

mehr_als_einer: bsr     trans_start     ;Ab Startoffset übertragen

                addq.w  #1,16(A6)       ;nächsten Sektor geht's weiter
                cmpi.w  #11,16(A6)      ;Startsektor 10?
                bne.s   no_step         ;Nein, also kein Step

vorne_anfangen: move.w  #1,16(A6)       ;Wieder bei Sektor #1 starten
                addq.w  #1,14(A6)       ;Auf nächsten Track gehen
                move.w  14(A6),D6
                bsr     seek_track      ;Track anfahren

no_step:        move.w  14(A6),D4       ;Starttrack
                cmp.w   20(A6),D4       ;Endtrack erreicht?
                beq.s   letzter_track

read_again2:    moveq   #11,D0
                sub.w   16(A6),D0       ;Anzahl der zu lesenden Sektoren
                movea.l A2,A0           ;Ab Ladeadresse laden
                bsr.s   read_sektoren
                adda.w  D5,A2           ;Bytes überspringen
                bra.s   vorne_anfangen

letzter_track:  move.w  22(A6),D0       ;Endsektor
                sub.w   16(A6),D0       ;-Startsektor
                beq.s   nur_ein_sektor
                move.w  D0,-(SP)        ;Anzahl der Sektoren retten
                movea.l A2,A0           ;Ladeadresse
                bsr.s   read_sektoren
                adda.w  D5,A2           ;Überspringen
                move.w  (SP)+,D0        ;Anzahl der gelesenen Sektoren
                add.w   D0,16(A6)       ;=Nächster Sektor

nur_ein_sektor: moveq   #1,D0
                lea     bitfeld(PC),A0
                bsr.s   read_sektoren   ;Letzten Sektor lesen
                lea     bitfeld(PC),A0
                move.w  24(A6),D7       ;Endoffset
trans3:         move.b  (A0)+,(A2)+     ;Rest übertragen
                dbra    D7,trans3
                bra.s   aus_is

nur_ein_track:  move.w  22(A6),D0       ;Endsektor
                sub.w   16(A6),D0       ;-Startsektor
                bne.s   mehr_als_einer

                lea     bitfeld(PC),A0
                adda.w  18(A6),A0       ;+Startoffset
                move.w  24(A6),D7       ;Endoffset
                sub.w   18(A6),D7       ;-Startoffset
trans4:         move.b  (A0)+,(A2)+
                dbra    D7,trans4

aus_is:         move.b  #-1,flop_aktiv  ;Transfer beendet
                movem.l (SP)+,D0-A5
                unlk    A6
                rts


;Liest ab Startsektor Sektoren ein. Gibt in D5 512*n zurück.
;-> D0: Anzahl der Sektoren
;   A0: Ladeadresse
read_sektoren:  move.w  D0,-(SP)        ;Anzahl retten
read_error:     move.w  (SP),D0         ;Anzahl holen
                mulu    #512,D0
                move.w  D0,D5           ;Länge merken (für nachher)
                bsr     setdma
                bsr     rdtoggle
                move.w  (SP),D7         ;Sektorcounter
                bsr     fdcout
                move.w  #$0084,(A4)     ;Sektorregister
                move.w  16(A6),D7       ;Startsektor holen
                bsr     fdcout
                move.w  #$0080,(A4)     ;Commandoregister
                move.w  #$0090,D7       ;Read multiple
                bsr     fdcout
                bsr     waitfertig
                tst.w   D0              ;Alles Okay?
                bne.s   read_error
                tst.w   (SP)+           ;D0 runternehmen
                rts

;Überträgt 1. Sektor ab Startoffset bis Sektorende
trans_start:    move.w  18(A6),D0       ;Startoffset
                lea     bitfeld(PC),A0
                adda.w  D0,A0           ;Bytes überspringen
                move.w  #512,D1         ;Soviel Bytes kommen
                sub.w   D0,D1           ;Offset abziehen=Anzahl an Bytes
                bra.s   trans1
trans2:         move.b  (A0)+,(A2)+
trans1:         dbra    D1,trans2
                rts

restore_fdc:    move.w  #$0080,(A4)     ;Kommandoregister
                move.w  #$0003,D7       ;Restore
                clr.b   dflag           ;Kein DMA-Transfer
                bsr.s   fdcout
                bsr.s   waitfertig
                cmpi.w  #4,D0           ;Spur 0 erreicht?
                bne.s   restore_fdc

;Fährt Track D6 an, macht ggf. Restore
seek_track:     move.w  #$0086,(A4)     ;Datareg.
                move.w  D6,D7           ;Starttrack setzen
                bsr.s   fdcout
                move.w  #$0080,(A4)     ;Commandoreg.
                move.w  #$0017,D7       ;Seek mit Verify
                clr.b   dflag           ;Kein DMA-Transfer
                bsr.s   fdcout
                bsr.s   waitfertig      ;auf Ausführung warten
                andi.w  #%0000000000011000,D0 ;nur CRC/RNF
                bne.s   restore_fdc     ;nicht gefunden, Restore first
                rts

;Setzt die DMA-Adresse
setdma:         pea     (A0)            ;Hier hinlesen
                move.b  3(SP),7(A4)     ;Adresse in DMA
                move.b  2(SP),5(A4)
                move.b  1(SP),3(A4)
                addq.l  #4,SP
                move.l  A0,dend
                ext.l   D0
                add.l   D0,dend         ;End-Adresse berechnen
                move.b  #1,dflag        ;DMA-Transfer findet statt
                rts

rdtoggle:       move.w  #$0090,(A4)
                move.w  #$0190,(A4)
                move.w  #$0090,(A4)
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
waitfertig:     move.l  #$00060000,D7   ;Timeout Zähler
poll:           btst    #5,(A5)         ;IRQ ausgelöst?
                beq.s   fix_und_fertig
                subq.l  #1,D7
                beq.s   timeout         ;Normalerweise Timeout
                tst.b   dflag           ;DMA-Transfer?
                beq.s   poll            ;Nein, weiterwarten

                moveq   #0,D0
                move.b  $FFFF8609,D0
                lsl.w   #8,D0
                move.b  $FFFF860B,D0    ;DMA-Adresse holen
                lsl.l   #8,D0
                move.b  $FFFF860D,D0
                cmp.l   dend(PC),D0
                blt.s   poll

                clr.b   dflag
fix_und_fertig: move.w  (A3),D0         ;FDC-Status lesen
fertig2:        andi.w  #%0000000000011100,D0 ;CRC-Lost etc. isolieren
                move.w  #$0080,(A4)     ;Commandoregister
                move.w  #$00D0,D7
                bsr.s   fdcout
                move.w  #250,D1
as_time_goes:   dbra    D1,as_time_goes
                rts

timeout:        movem.l D0-D1,-(SP)

                move.w  #$0700,$FFFF8240.w
                moveq   #15,D1
flsh:           not.w   $FFFF8240.w
                dbra    D0,flsh
                dbra    D1,flsh

                movem.l (SP)+,D0-D1
                moveq   #-1,D0          ;Fehler!
                bra.s   fertig2

;Selektiert ein Laufwerk. In D0:
;Bit 0: Laufwerk
;Bit 1: Seite
select:         clr.b   flop_aktiv      ;Jetzt ist Floppy aktiv

                move    SR,-(SP)
                ori     #$0700,SR
                move.w  D0,D1
                lsr.w   #1,D0           ;Seite runterschieben
                andi.w  #%0000000000000001,D0 ;Seite isolieren
                andi.w  #%0000000000000001,D1 ;Laufwerk isolieren
                addq.w  #1,D1
                add.w   D1,D1
                or.w    D0,D1           ;Seite einodern
                eori.w  #7,D1
                move.b  #14,$FFFF8800
                move.b  $FFFF8802,D0
                andi.w  #$00F8,D0
                or.w    D1,D0
                move.b  D0,$FFFF8802

                move    (SP)+,SR
                rts

;Dies ist die Diskettenfunktion. Ist der Modus 0, wird das TOS zum Laden
;benutzt, sonst eine eigene Diskettenroutine. Diese Routine prüft vor dem
;Laden, ob ein entsprechendes Archiv vorhanden ist, bzw. fordert zur Einlage
;der jeweiligen Diskette auf. Gibt die tatsächliche Länge der gelesenen Daten
;zurück.
;long loaddisk(modus,archiv,offset,länge,adr)
;-> 18(a6): Ladeadresse
;   14(a6): Länge der zu ladenden Daten
;   10(a6): Offset innerhalb des Archivs
;    8(a6): archiv [0..n]
mcode61:        link    A6,#0
                movem.l D1-A5,-(SP)

                move.l  10(A6),D0       ;Offset innerhalb Archiv
                add.l   14(A6),D0       ;Erstes freies Byte
                subq.l  #1,D0           ;Letztes benutztes Byte
                divu    #512,D0         ;Anzahl an Sektoren vom Start
                swap    D0
                move.w  D0,-(SP)        ;End_Offset
                swap    D0
                add.w   first_sek(PC),D0 ;Archiv startet hier
                ext.l   D0
                divu    #10,D0          ;10 Sektoren pro Track
                swap    D0              ;Sektornummer
                addq.w  #1,D0
                move.w  D0,-(SP)        ;End_Sektor
                swap    D0
                move.w  D0,-(SP)        ;End_Track
                move.l  10(A6),D0       ;Startoffset
                divu    #512,D0
                swap    D0
                move.w  D0,-(SP)        ;Startoffset
                swap    D0
                add.w   first_sek(PC),D0 ;Archiv startet hier
                ext.l   D0
                divu    #10,D0          ;10 Sektoren pro Track
                swap    D0              ;Sektornummer
                addq.w  #1,D0
                move.w  D0,-(SP)        ;Startsektor
                swap    D0
                move.w  D0,-(SP)        ;Starttrack
                clr.w   -(SP)           ;A:
                move.l  18(A6),-(SP)    ;Ladeadresse
                bsr     mcode68         ;Floppy_Read
                lea     18(SP),SP

                movem.l (SP)+,D1-A5
                unlk    A6
                rts

vsync:          move.w  vbl_ct(PC),D0
wait_sync:      cmp.w   vbl_ct(PC),D0
                beq.s   wait_sync
                rts

vbl:            addq.w  #1,vbl_ct
                rte

trap_handler:   lea     pointer(PC),A0
                addq.w  #4,(A0)
                adda.w  (A0)+,A0
                move.l  2(SP),(A0)

                movem.l D0-A6,-(SP)

                move.w  #$0000,$FFFF8240.w
                moveq   #15,D0
loop:           not.w   $FFFF8240.w
                dbra    D1,loop
                dbra    D0,loop

                movem.l (SP)+,D0-A6
                rte

pointer:        DC.W 0

medusa:         DC.B 'MEDUSA  PRG',0
                PATH 'E:\MEDUSA\MEDUSA_2.COL\MCODE'
                PATH 'ATARI'
scancodes:      IBYTES 'SCANCODE.IMG'
                EVEN

;Palette of SIGNS_OF.IFF: Saved by NEOchrome V2.18 by Chaos, Inc.
bild:           DC.W $0000,$0636,$0744,$0753,$0773,$0751,$0763,$0775
                DC.W $0002,$0003,$0215,$0326,$0545,$0655,$0757,$0777

*    pixels/scanline    = $00B0 (bytes/scanline: $0058)
*  # scanlines (height) = $0037
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0040,$0080,$0000,$00C0,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$00D0,$0060,$0000,$00F0
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0034,$0078,$0000,$007C,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$007A,$002C,$0000,$006E
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0016,$003B,$0000,$0033,$C000,$0000,$0000,$C000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$003F,$0011,$0004,$0031
                DC.W $3000,$C000,$0000,$F000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0002,$0017,$0008,$0018,$D000,$6000,$0000,$7800
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$001C,$000A,$0001,$0058
                DC.W $B400,$D800,$0000,$1C00,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0020,$0000,$0000,$0070,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $001F,$0005,$0000,$085C,$1A00,$6C00,$C000,$0E00
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0020
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0800,$0000,$0000,$0800,$0813,$000E,$0001,$0C1E
                DC.W $9680,$DB00,$E000,$0380,$0000,$0000,$0000,$0004
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0021,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$8000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$1800
                DC.W $021A,$0407,$0001,$0F5E,$EB40,$AD80,$9000,$81C0
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0020,$0000,$0000,$0072
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $1000,$0000,$0000,$3800,$06B6,$030C,$0001,$0FFC
                DC.W $F2A0,$91C0,$CE00,$C0E0,$0000,$0000,$0000,$0010
                DC.W $0000,$0000,$0000,$0100,$0000,$0000,$0000,$0180
                DC.W $0050,$0020,$0000,$00FA,$0000,$0000,$0000,$0002
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$3000,$0000,$0000,$7800
                DC.W $0534,$03CD,$0003,$0FFF,$9550,$A6E0,$DB00,$6078
                DC.W $0000,$0000,$0000,$0020,$0000,$0000,$0000,$0200
                DC.W $0100,$0000,$0000,$0380,$00A0,$0040,$0000,$00F4
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0001
                DC.W $5000,$2000,$0000,$F000,$00E1,$0179,$0007,$077F
                DC.W $4F68,$79B0,$9C80,$6438,$0000,$0000,$0000,$00C0
                DC.W $0000,$0000,$0000,$0000,$0100,$0000,$0000,$0380
                DC.W $00C8,$0060,$0000,$01EC,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$1000,$0000,$0000,$0000,$0000
                DC.W $0001,$0000,$0000,$0007,$A000,$4000,$0000,$F000
                DC.W $015B,$00A1,$0007,$0787,$0DA8,$6850,$9E00,$6E1E
                DC.W $0080,$0000,$0000,$01C0,$0000,$0000,$0000,$0400
                DC.W $0280,$0100,$0000,$07E0,$0130,$00C0,$0000,$03F8
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$2000
                DC.W $0000,$0000,$0000,$0000,$0006,$0001,$0000,$001F
                DC.W $C000,$8000,$0000,$E000,$0177,$00D9,$0003,$01C2
                DC.W $2B70,$FBE8,$9C00,$7FEE,$0300,$0000,$0000,$0780
                DC.W $0000,$0000,$0000,$0000,$0340,$0180,$0000,$07F0
                DC.W $02C0,$01A0,$0000,$0FF0,$0000,$0000,$0000,$0004
                DC.W $2000,$0000,$0000,$F000,$0000,$0000,$0000,$0800
                DC.W $0019,$0007,$0000,$007F,$4000,$8000,$0000,$E000
                DC.W $00AA,$0076,$000D,$E1E0,$8AEA,$5B1C,$BC00,$77FF
                DC.W $0500,$0200,$0000,$0F00,$0000,$0000,$0000,$0000
                DC.W $00E0,$0280,$0100,$07F0,$0D20,$0340,$0080,$1FF0
                DC.W $0000,$0000,$0000,$0003,$D000,$2000,$0000,$F000
                DC.W $0000,$0000,$0000,$0BC0,$0066,$001F,$0000,$01FF
                DC.W $8000,$0000,$0000,$C001,$E054,$003A,$0007,$F870
                DC.W $F696,$930C,$3800,$33FF,$1000,$0C00,$0000,$3C00
                DC.W $0000,$0000,$0000,$0000,$07D8,$02E0,$0100,$1FFF
                DC.W $2580,$1C80,$0300,$7FE0,$0083,$0000,$0000,$0083
                DC.W $A000,$C000,$0000,$E000,$13C0,$0000,$0000,$37E0
                DC.W $0139,$00FA,$0004,$07FF,$0001,$0000,$0000,$C007
                DC.W $182D,$E01B,$0000,$FC78,$7504,$5E02,$9800,$17BE
                DC.W $2800,$1800,$0000,$7800,$0000,$0000,$0000,$0000
                DC.W $0BA6,$0638,$0140,$7E7F,$B400,$7280,$0F00,$FFC0
                DC.W $015A,$0087,$0000,$01FF,$C000,$0000,$0000,$C002
                DC.W $2420,$03C0,$0000,$7FF0,$05D4,$03C6,$0038,$0FFF
                DC.W $C006,$0001,$0000,$F00F,$9416,$980D,$6000,$FE7C
                DC.W $9D00,$F600,$7800,$078B,$B000,$7800,$0000,$F800
                DC.W $0000,$0000,$0000,$2003,$25D5,$1D16,$02E8,$FE4F
                DC.W $5180,$CD00,$3E00,$FFB0,$036D,$019A,$0000,$03FB
                DC.W $4000,$0000,$0000,$E005,$CA50,$2660,$0180,$FFF8
                DC.W $0A2A,$060D,$01F0,$1FFF,$300A,$C006,$0001,$FF1F
                DC.W $AA0D,$FC03,$5000,$5FFF,$AF01,$6A00,$1000,$0383
                DC.W $7000,$D000,$0000,$D800,$0000,$0000,$0000,$0007
                DC.W $D4F5,$32FD,$0FFA,$FC63,$9680,$7700,$1800,$D3C8
                DC.W $0494,$02FE,$0100,$87F7,$6005,$8000,$0000,$FC0F
                DC.W $35E8,$CFF0,$0080,$FCBC,$1537,$0CD9,$03E0,$3FFC
                DC.W $2F1B,$B00F,$C001,$FFF9,$A5C2,$AE01,$F800,$8FFF
                DC.W $D382,$B501,$0800,$8187,$A000,$E000,$0000,$B800
                DC.W $0000,$0000,$0000,$0001,$116E,$0FEE,$0095,$FD31
                DC.W $2DF8,$EE00,$3000,$A7FF,$0B59,$067F,$0180,$BFCF
                DC.W $980A,$E005,$0000,$FEBF,$A954,$DD58,$03E0,$F95E
                DC.W $34B3,$13BF,$0FC3,$7F9B,$C8E7,$EF1F,$B007,$BFF6
                DC.W $4B20,$5CC0,$F000,$1FF7,$B40A,$6E87,$0100,$E19E
                DC.W $F000,$C000,$0000,$7000,$0000,$0000,$0000,$0000
                DC.W $18AF,$059A,$02C7,$3FB9,$EB4A,$BCCC,$0030,$8FFF
                DC.W $66AB,$0F7C,$01A0,$FDAC,$E415,$780E,$0000,$7F7E
                DC.W $662A,$9EBC,$0340,$F20F,$4A4C,$2673,$1F81,$FE18
                DC.W $732A,$77DE,$E801,$6FF8,$AAD5,$B7E3,$C000,$1FFF
                DC.W $2FAB,$38DF,$C201,$F8F9,$2000,$4000,$8000,$F000
                DC.W $0000,$0000,$0000,$000F,$C655,$3356,$0DEB,$FC0D
                DC.W $4951,$ED7A,$1EA4,$C787,$E979,$1F77,$00C0,$F850
                DC.W $7B2F,$DC1B,$8000,$9FFB,$9C92,$F56C,$0E00,$F027
                DC.W $BCA8,$6CDB,$1708,$F42E,$1953,$8B6F,$F680,$1EFE
                DC.W $3FEB,$17D6,$C280,$339E,$F6FD,$ED4D,$4016,$4C44
                DC.W $C000,$8000,$0000,$E000,$0000,$0000,$0000,$0001
                DC.W $1AD7,$0685,$017A,$FF23,$D2C4,$1ECD,$797A,$CB43
                DC.W $FE75,$3E6F,$0F83,$EE21,$2DD7,$763C,$C001,$07F0
                DC.W $6955,$F9B8,$8600,$E1FF,$5659,$DA7B,$219C,$F018
                DC.W $3B76,$2BE3,$D780,$03BE,$3C7F,$5D7B,$CEE1,$0C69
                DC.W $CBAB,$D66A,$F01D,$C609,$4000,$7000,$8000,$F000
                DC.W $0000,$0000,$0000,$0000,$073A,$017A,$00C5,$7F11
                DC.W $3537,$B737,$58CC,$CE05,$B4CA,$77FF,$1804,$D060
                DC.W $D6BF,$2B6B,$0001,$03E1,$B3E9,$AAF0,$DD80,$01BF
                DC.W $3CE3,$AEE3,$413C,$EC20,$35F2,$93E7,$FC80,$10BE
                DC.W $121F,$739F,$DC73,$1313,$5966,$7FE4,$803B,$0F21
                DC.W $A000,$C000,$0000,$E000,$0000,$0000,$0000,$0000
                DC.W $03E5,$06D4,$01AF,$7F19,$EE88,$C7B8,$39C7,$C6B1
                DC.W $2A89,$E7DA,$300C,$AE08,$9F51,$FDBC,$0007,$FDF0
                DC.W $51FA,$DDFD,$27E0,$006F,$A4A4,$FEE7,$0118,$FC00
                DC.W $AAB6,$87D7,$780C,$0664,$4505,$664D,$9836,$0784
                DC.W $56D1,$3BD3,$006C,$FE41,$4000,$8000,$0000,$C000
                DC.W $0000,$0000,$0000,$0000,$1749,$0D33,$03CE,$3E18
                DC.W $8EA3,$EF9A,$11C7,$C631,$D942,$DB57,$24ED,$8C40
                DC.W $23A3,$5FFA,$C00C,$5FE1,$2A92,$F7DF,$0C78,$801B
                DC.W $5482,$EEF7,$0108,$FC06,$AD2F,$CA6F,$701E,$078A
                DC.W $4C4C,$6EDC,$9837,$0B04,$3BA4,$25B5,$C0CA,$1C83
                DC.W $8000,$0000,$0000,$C030,$0000,$0000,$0000,$0000
                DC.W $0CBA,$00DE,$0305,$2E11,$B123,$D31A,$0CC7,$C631
                DC.W $1595,$35BF,$CBC0,$C190,$3AFA,$BDA5,$E00E,$2FA1
                DC.W $5865,$E5AF,$0E50,$1101,$F515,$AFF6,$0008,$BC42
                DC.W $4298,$F75C,$1833,$0580,$76D4,$57DD,$8C63,$0440
                DC.W $9D66,$DF57,$7188,$1523,$4020,$8000,$0000,$E070
                DC.W $0000,$0000,$0000,$0000,$175D,$0DB7,$0008,$7C31
                DC.W $705D,$A36D,$1CC6,$E634,$E674,$F9EB,$C000,$C03E
                DC.W $556A,$DED6,$3001,$17C0,$655F,$BF6A,$4290,$10C2
                DC.W $D5AD,$EFDE,$0000,$FC46,$4104,$E1E9,$1E30,$000E
                DC.W $4AAE,$BBB1,$C6C0,$2298,$2A39,$EECD,$1992,$0843
                DC.W $4040,$8000,$0000,$E0E0,$0000,$0000,$0000,$0000
                DC.W $0AE6,$0739,$0084,$7E31,$4D63,$BAFF,$04C5,$E671
                DC.W $3C86,$3BE5,$F00C,$3623,$AD75,$66EF,$1003,$07E1
                DC.W $2EB3,$FBED,$8500,$88E7,$7D1C,$DFEB,$0000,$DC6E
                DC.W $C280,$F7F6,$181D,$00C0,$255C,$DD65,$8286,$3035
                DC.W $90D4,$35FA,$0B07,$C047,$A180,$C000,$0000,$F3C0
                DC.W $0000,$0000,$0000,$0000,$06AB,$017D,$0002,$1F39
                DC.W $B4BD,$1B75,$04C6,$F714,$3771,$F7C8,$0E1C,$0601
                DC.W $4ABA,$FC76,$2001,$0FF0,$534D,$FD7A,$8280,$18FB
                DC.W $ABA9,$7F57,$0001,$E07D,$5149,$77B5,$880E,$05E0
                DC.W $60A9,$2EDA,$C30C,$386B,$4D92,$2F5B,$72AF,$8003
                DC.W $5280,$6100,$8000,$FF80,$0000,$0000,$0000,$0000
                DC.W $0376,$00DD,$0000,$0FDC,$0AD5,$9CA6,$0008,$F880
                DC.W $DA99,$F6BE,$01C5,$F090,$B55D,$E83B,$4000,$0FF8
                DC.W $6D36,$F6AF,$01C0,$3C7F,$D543,$3D9F,$0203,$F0FB
                DC.W $B582,$D6FF,$0800,$0330,$6AB6,$5B57,$8D98,$0811
                DC.W $B729,$3ABD,$C547,$0001,$AD00,$3200,$C000,$FF80
                DC.W $0000,$0000,$0000,$0000,$01A5,$0073,$0000,$03FF
                DC.W $D654,$0D5B,$0020,$7D62,$5AA6,$A5AF,$0050,$FC18
                DC.W $D826,$701D,$8000,$1FFC,$B2E8,$7D77,$0080,$1C3F
                DC.W $EA8D,$9EFD,$0102,$F860,$7155,$DBEE,$0E00,$4070
                DC.W $634D,$BDEA,$C215,$3040,$C995,$FEEF,$0003,$0080
                DC.W $1200,$DC00,$A000,$3F00,$0000,$0000,$0000,$0000
                DC.W $0052,$0029,$0000,$01EB,$A5EE,$C375,$0000,$FF45
                DC.W $AAA9,$5176,$0000,$5F04,$309B,$F806,$0000,$1DFE
                DC.W $9BB5,$ED78,$0000,$CF1F,$359A,$CFF7,$0000,$FC30
                DC.W $AB20,$FFCF,$0400,$E1FA,$959A,$EAF5,$0108,$38C0
                DC.W $2778,$DFF6,$0003,$13F0,$A800,$FC00,$0000,$3E00
                DC.W $0000,$0000,$0000,$0000,$0070,$0018,$0000,$00FB
                DC.W $5133,$A0DE,$0000,$B99E,$0794,$A87F,$0000,$FFDF
                DC.W $DC0D,$3803,$0000,$8EBF,$5DAD,$BA76,$0000,$FBE7
                DC.W $4B4D,$36F9,$0002,$FE1B,$4659,$AF86,$1000,$F3FE
                DC.W $2A45,$C5BA,$0000,$7CE2,$D28F,$2FF9,$0002,$38F8
                DC.W $5400,$B800,$0000,$3C00,$0000,$0000,$0000,$0000
                DC.W $0000,$0014,$0000,$007D,$82DC,$6163,$0000,$FB7F
                DC.W $B55A,$E22D,$0000,$FFEF,$9A02,$6401,$0000,$E75F
                DC.W $B2EF,$CF9D,$0000,$FFFD,$52D2,$81BF,$0000,$FF8F
                DC.W $22AA,$17D5,$0800,$F8FD,$44A6,$8359,$0000,$EE7F
                DC.W $27B6,$CDCD,$0000,$FDFC,$E800,$7000,$0000,$7C00
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$003D
                DC.W $45B7,$12C0,$0000,$FEF7,$2E96,$9008,$0000,$FEBA
                DC.W $2D01,$5A00,$0000,$FBAB,$2D28,$C2C7,$0000,$FFFF
                DC.W $B0B0,$E06E,$0000,$FFE7,$11D5,$0A7A,$0400,$3C7F
                DC.W $8349,$00F0,$0000,$C67F,$CBDB,$0766,$0000,$FF7E
                DC.W $D000,$A000,$0000,$B800,$0000,$0000,$0000,$0000
                DC.W $0019,$0000,$0000,$0019,$0A89,$0700,$0000,$BFCB
                DC.W $944B,$4806,$0000,$FC5F,$1288,$0F00,$0000,$5FDD
                DC.W $D2AF,$2010,$0000,$FFFF,$CA2D,$341A,$0000,$FF7B
                DC.W $0AFA,$07AC,$0000,$9FAF,$02F4,$0128,$0000,$072F
                DC.W $04A7,$03FD,$0000,$CFFD,$C000,$6000,$0000,$7000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0018
                DC.W $3751,$0D80,$0000,$FDFB,$2D65,$0003,$0000,$BF6F
                DC.W $8520,$03C0,$0000,$AFF8,$341A,$0800,$0000,$FEBF
                DC.W $B78B,$0006,$0000,$FFDE,$4554,$82F8,$0000,$EFFE
                DC.W $0128,$00D0,$0000,$03FC,$0356,$00AF,$0000,$07FF
                DC.W $A000,$C000,$0000,$E000,$0000,$0000,$0000,$0000
                DC.W $0008,$0000,$0000,$0008,$0A01,$0700,$0000,$3FC1
                DC.W $9A23,$0000,$0000,$BFA7,$02A0,$8100,$0000,$D7E0
                DC.W $0804,$0000,$0000,$3C9F,$0002,$0001,$0000,$B787
                DC.W $A2A8,$C050,$0000,$F7FC,$00D0,$0000,$0000,$01F8
                DC.W $00AE,$001B,$0000,$03FB,$4000,$8000,$0000,$E000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $1722,$0200,$0000,$9FB2,$4100,$0000,$0000,$5FC1
                DC.W $C100,$0000,$0000,$CB80,$0400,$0000,$0000,$0E06
                DC.W $0000,$0000,$0000,$0001,$5050,$E000,$0000,$FAF8
                DC.W $0000,$0000,$0000,$00D0,$002D,$001E,$0000,$00FF
                DC.W $8000,$0000,$0000,$C000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0002,$0000,$0200,$0000,$3760
                DC.W $0000,$0000,$0000,$0980,$0008,$0000,$0000,$3108
                DC.W $0000,$0000,$0000,$0601,$0000,$0000,$0000,$8000
                DC.W $6400,$1800,$0000,$FE50,$0000,$0000,$0000,$0000
                DC.W $0056,$003C,$0000,$00FF,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0200,$0000,$0000,$4310,$0000,$0000,$0000,$0C40
                DC.W $0000,$0000,$0000,$0040,$C000,$0000,$0000,$C100
                DC.W $0000,$0000,$0000,$4000,$1900,$0600,$0000,$7F80
                DC.W $0000,$0000,$0000,$0000,$00AA,$0074,$0000,$01FE
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$8204
                DC.W $0000,$0000,$0000,$0001,$0000,$0000,$0000,$0100
                DC.W $0800,$0000,$0000,$0880,$0000,$0000,$0000,$0800
                DC.W $0600,$0000,$0000,$1FD0,$0000,$0000,$0000,$0000
                DC.W $02A0,$01C4,$0000,$07FE,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0200,$0000,$0000,$0200,$0000,$0000,$0000,$1FFF
                DC.W $0080,$0000,$0000,$3FFF,$0000,$0000,$0000,$E3F1
                DC.W $0000,$0000,$0000,$F077,$4000,$0000,$0000,$FFFE
                DC.W $0000,$0000,$0000,$F100,$0584,$0200,$0000,$0FE4
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0200
                DC.W $1151,$0EAE,$0000,$1FFF,$2222,$1DDD,$0000,$BFFF
                DC.W $A2C1,$5D30,$0000,$FFF9,$5155,$AE22,$0000,$FF7F
                DC.W $1350,$ECAE,$0000,$FFFF,$9100,$6000,$0000,$FB00
                DC.W $0200,$0000,$0000,$1F84,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$040A,$175A,$08AD,$1159
                DC.W $AD68,$EB6E,$1591,$E722,$2C2A,$AAED,$5510,$A6DF
                DC.W $EAAA,$5AF7,$ED00,$F9DD,$B411,$B554,$CAAF,$9359
                DC.W $6A00,$6C00,$9000,$9E00,$1000,$0000,$0000,$3A00
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $13FA,$0BFC,$04A8,$1BFF,$1747,$3FB7,$1109,$3BF7
                DC.W $D7D7,$FFD9,$5120,$FB5F,$FA55,$FD5D,$A822,$FF49
                DC.W $BFEC,$DFF7,$88A8,$FDF7,$9500,$F600,$0800,$6F80
                DC.W $0000,$0000,$0000,$1000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0002
                DC.W $0000,$0000,$0000,$0000,$01E5,$19E5,$06FA,$1BF3
                DC.W $0AC3,$AAF3,$150D,$A677,$C915,$E917,$57E8,$E74F
                DC.W $F501,$F581,$AA7E,$F3D5,$69EE,$7DFF,$8AA0,$3DFB
                DC.W $6A70,$6B80,$F400,$67FF,$0000,$0000,$0000,$4000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $13FA,$0BFC,$04A8,$1BFF,$1747,$3FB7,$1109,$3BF7
                DC.W $D797,$FF99,$5160,$FB9F,$FB77,$FC77,$A82A,$FF7F
                DC.W $BFE8,$DFF7,$88A2,$FDFD,$D900,$FC00,$9000,$FF00
                DC.W $0000,$0000,$0000,$8000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$140A,$0F5A,$04AD,$1F59
                DC.W $B168,$FB7E,$1589,$FB3E,$B12A,$BBAD,$D550,$BBDF
                DC.W $EE2A,$5C7F,$E822,$FE7F,$B450,$B559,$CAEE,$9353
                DC.W $2500,$4600,$9800,$6F00,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0B11,$04AE,$0000,$1FBF,$2A36,$15C9,$0000,$BFFF
                DC.W $2A81,$D570,$0000,$FFF9,$1255,$EC2A,$0000,$FE7F
                DC.W $1311,$ECEE,$0000,$FFFF,$4800,$9000,$0000,$DD00
                DC.W $0000,$0000,$0000,$0010,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000,$0040,$0000,$0000,$01FF
                DC.W $0080,$0000,$0000,$3FFF,$8000,$0000,$0000,$FFF1
                DC.W $4000,$0000,$0000,$F077,$4040,$0000,$0000,$F1FF
                DC.W $0000,$0000,$0000,$F800,$0000,$0000,$0000,$0000
                DC.W $0000,$0000,$0000,$0000

*    pixels/scanline    = $0090 (bytes/scanline: $0048)
*  # scanlines (height) = $0009
                DC.W $0000,$0000,$0000,$0007,$0008,$0000,$0000,$8FBF
                DC.W $0080,$0000,$0000,$FFFF,$0000,$0000,$0000,$C1E1
                DC.W $0000,$0000,$0000,$F7FD,$0040,$0000,$0000,$F7FF
                DC.W $0000,$0000,$0000,$DBDF,$0000,$0000,$0000,$F87F
                DC.W $0000,$0000,$0000,$FB78,$0004,$0003,$0000,$000F
                DC.W $88A2,$071D,$0000,$DFFF,$A632,$59CD,$0000,$FFFF
                DC.W $C121,$00C0,$0000,$E3F3,$1455,$E3A8,$0000,$FFFF
                DC.W $5511,$A2EE,$0000,$FFFF,$526D,$8992,$0000,$FFFF
                DC.W $6042,$9839,$0000,$FCFF,$6A88,$9170,$0000,$FFFC
                DC.W $000B,$0003,$0004,$001C,$5248,$6B6A,$8495,$F8E6
                DC.W $096B,$AB6B,$5594,$A732,$D292,$2199,$4060,$B73F
                DC.W $48EA,$6DFD,$9300,$1C57,$AAB6,$F7B5,$00CA,$5D93
                DC.W $AD80,$D9AD,$0252,$766D,$1595,$7655,$882A,$6FEE
                DC.W $9555,$BBD6,$4068,$6ECF,$0014,$000F,$0000,$003B
                DC.W $A977,$B5BF,$4211,$7DFB,$F75A,$FFBE,$5101,$FBEE
                DC.W $5429,$DA50,$2180,$BF7F,$2995,$B357,$40A8,$BB52
                DC.W $159A,$5DEF,$A288,$49FD,$527F,$77FF,$8812,$25B7
                DC.W $EB72,$ECFF,$9000,$AFDD,$6AD0,$EEE0,$1140,$A4F8
                DC.W $0013,$000B,$0007,$003B,$508F,$5CEF,$A311,$3DE7
                DC.W $EEC4,$EEEA,$5111,$E66E,$ABD5,$C776,$0048,$7EEF
                DC.W $1780,$9BC0,$603F,$BED5,$0055,$0055,$FFAA,$5533
                DC.W $0192,$0192,$FFFF,$55B7,$8A4D,$8BCD,$F433,$A7DD
                DC.W $8A28,$8A2C,$F5D0,$AA9C,$0016,$000F,$0004,$003F
                DC.W $C977,$E5BF,$8211,$FDFB,$F75B,$FFBF,$5101,$FBEF
                DC.W $5349,$DF90,$2020,$BEDF,$28BD,$B77D,$422A,$BDFF
                DC.W $B79A,$F7EF,$AA88,$FFFD,$DA71,$DFF9,$AA56,$FFF9
                DC.W $CB7E,$CCFF,$B00A,$CFDF,$F5D0,$FFE0,$5140,$FFF8
                DC.W $0011,$000A,$0004,$003B,$2A08,$372A,$C2DD,$7F6A
                DC.W $896A,$AB6A,$DD95,$AB32,$9495,$22B6,$4148,$BF2F
                DC.W $466A,$E6FF,$5BA8,$EC7F,$AAB8,$FFBD,$A2CA,$FF9D
                DC.W $A802,$FDAB,$8A54,$FDAD,$9591,$D654,$A82A,$EFE5
                DC.W $1555,$BBD6,$5168,$BFCF,$000A,$0004,$0000,$001E
                DC.W $4562,$829D,$0000,$EFFF,$2232,$DDCD,$0000,$FFFF
                DC.W $2228,$41D0,$0000,$F7FD,$AC55,$53AA,$0000,$FFFF
                DC.W $5515,$AAEA,$0000,$FFFF,$55A9,$AA56,$0000,$FFFF
                DC.W $4045,$B83A,$0000,$FCFF,$AA88,$5170,$0000,$FFFC
                DC.W $0000,$0000,$0000,$000F,$0008,$0000,$0000,$C7DF
                DC.W $8880,$0000,$0000,$FFFF,$0000,$0000,$0000,$63F8
                DC.W $0000,$0000,$0000,$FFFD,$0040,$0000,$0000,$F7FF
                DC.W $0000,$0000,$0000,$DFFF,$0000,$0000,$0000,$F87F
                DC.W $0000,$0000,$0000,$FBF8

less_mem:       DC.B 'Bad RAM detected.',13,10
                DC.B 'Contact your local dealer.',0

                EVEN
                BSS

old_vbl:        DS.L 1

vbl_ct:         DS.W 1          ;VBL-Counter

prg_header:     DS.W 1          ;$601a
textlen:        DS.L 1
datalen:        DS.L 1
bsslen:         DS.L 1
symlen:         DS.L 1
                DS.L 2          ;reserved
                DS.W 1          ;ph_flag

medusa_len:     DS.L 1          ;Länge von MEDUSA.PRG
first_sek:      DS.W 1          ;1. Sektor von Medusa.prg
flop_aktiv:     DS.B 1          ;Floppy ist nicht aktiv
dflag:          DS.B 1          ;Flag, ob DMA-Transfer
dend:           DS.L 1          ;DMA-Endadresse
bitfeld:        DS.W 2048
                END
