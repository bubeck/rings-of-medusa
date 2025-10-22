
;Install-Amiga:
;installiert (Im Zeichen der Medusa) auf dem Amiga..
;(C) 1990 by Till Bubeck!

;Bootsektor 0/0-0/1
;Direktory 0/2
;Loader von 0/3-1/3
;Medusa.prg ab Track 1/4

buffer          EQU $C0B000

                ORG $015000
                DEFAULT 9       ;Amigatool

                move    #$2000,SR

                lea     message,A1
                moveq   #9,D0
                trap    #1

                lea     message,A1
                moveq   #7,D0
                trap    #1
                cmp.w   #'+',D0
                bne.s   ohne_format

                bsr     format_disk

ohne_format:    lea     buffer,A0
                lea     bootsek_fn,A1
                move.l  #1024,D1
                moveq   #0,D0
                bsr     trap_1          ;Boot_Sektor lesen

                lea     buffer,A0
                bsr     executable      ;Bootsektor ausführbar machen

                move.w  #511,-(SP)
                move.w  #2,-(SP)
                move.w  #0,-(SP)
                move.w  #0,-(SP)
                move.w  #1,-(SP)
                move.w  #0,-(SP)
                move.w  #0,-(SP)
                pea     buffer
                bsr     mcode70         ;Bootsektor schreiben
                lea     18(SP),SP
                tst.w   disk_error
                bmi     abbruch

                move.w  #3,first_sek    ;hier beginnen die Daten

                lea     loader_dir,A0
                lea     loader_fn,A1
                bsr     put_file
                tst.w   disk_error
                bmi     abbruch

                lea     rom0_dir,A0
                lea     rom0_fn,A1
                bsr     put_file
                tst.w   disk_error
                bmi     abbruch

                lea     rom1_dir,A0
                lea     rom1_fn,A1
                bsr     put_file
                tst.w   disk_error
                bmi     abbruch

                lea     rom2_dir,A0
                lea     rom2_fn,A1
                bsr     put_file
                tst.w   disk_error
                bmi     abbruch

                moveq   #0,D1
                lea     ram_medusa_fn,A1
                moveq   #$3D,D0         ;Fopen
                bsr     trap_1

                lea     medusa_fn,A1
                tst.w   D0              ;File auf Ramdisk vorhanden?
                bmi.s   load

                move.w  D0,D1           ;Fhandle
                moveq   #$3E,D0         ;Fclose
                lea     ram_medusa_fn,A1
                bsr     trap_1

                lea     ram_medusa_fn,A1
load:           lea     medusa_dir,A0
                bsr     put_file
                tst.w   disk_error
                bmi     abbruch
                move.w  #511,-(SP)
                move.w  #3,-(SP)        ;Sektor
                move.w  #0,-(SP)        ;Track
                move.w  #0,-(SP)
                move.w  #3,-(SP)        ;Sektor
                move.w  #0,-(SP)        ;Track
                move.w  #0,-(SP)
                pea     dir
                bsr     mcode70         ;Directory schreiben
                lea     18(SP),SP

abbruch:        illegal

                move.w  #511,-(SP)
                move.w  #11,-(SP)       ;Sektor
                move.w  #1,-(SP)        ;Track
                move.w  #0,-(SP)
                move.w  #1,-(SP)        ;Sektor
                move.w  #1,-(SP)        ;Track
                move.w  #0,-(SP)
                pea     hlpbuf
;                bsr     mcode68         ;Track 1 lesen
                lea     18(SP),SP

zum_st:         lea     st_adresse(PC),A1
                move.l  #$300000,D0
                moveq   #3,D7
schreib_long:   moveq   #-1,D1          ;Alle Bits setzen
                rol.l   #8,D0
                move.b  D0,D1
                rol.w   #4,D1           ;in die Mitte schieben
                move.w  D1,(A1)+
                dbra    D7,schreib_long
                clr.w   (A1)            ;Ende

                lea     track_buffer,A0 ;ab hier übertragen
;                lea     hlpbuf,A0
;                lea     original,A0
                move.l  #40000,D1       ;Länge des Speicherbereichs
                lea     st_adresse(PC),A1 ;hier hin schreiben
                move.w  #$80,D0         ;Send_2_ST
                trap    #1

                move.w  $2001,D0

alert:          rts

;Bringt File auf Amiga-Diskette und trägt ins Directory ein:
;-> A0.L: Directory-Eintragsname
;   A1.L: Atari-Filename
put_file:       move.l  A0,-(SP)

                lea     buffer,A0
                move.l  #470000,D1
                moveq   #0,D0
                bsr     trap_1          ;Loader lesen

                movea.l (SP)+,A0
                bsr     insert_dir      ;im Directory eintragen

                ext.l   D2
                ext.l   D1
                divu    sek_per_track,D2 ;in Tracks und Sektoren zerlegen
                divu    sek_per_track,D1

                move.w  #511,-(SP)
                swap    D1
                addq.w  #1,D1           ;Sektoren starten bei Eins
                move.w  D1,-(SP)        ;Sektor
                swap    D1
                move.w  D1,-(SP)        ;Track
                move.w  #0,-(SP)
                swap    D2
                addq.w  #1,D2           ;Sektoren starten bei Eins
                move.w  D2,-(SP)        ;Sektor
                swap    D2
                move.w  D2,-(SP)        ;Track
                move.w  #0,-(SP)
                pea     buffer
                bsr     mcode70         ;File schreiben
                lea     18(SP),SP
                rts

insert_dir:     movea.l directory,A1
dir_loop:       move.b  (A0)+,(A1)+
                bne.s   dir_loop

                movea.l directory,A1
                move.l  D0,D1           ;Länge der Datei in Bytes
                move.b  D1,28(A1)
                lsr.l   #8,D1
                move.b  D1,29(A1)
                lsr.l   #8,D1
                move.b  D1,30(A1)
                lsr.w   #8,D1
                move.b  D1,31(A1)

                move.w  first_sek,D1
                move.b  D1,26(A1)
                lsr.w   #8,D1
                move.b  D1,27(A1)       ;1. Datensektor eintragen

                move.w  first_sek,D2    ;Start dieser Datei

                divu    #512,D0         ;Anzahl an Sektoren für Datei
                addq.w  #1,D0           ;+1 als Rest

                add.w   D0,first_sek    ;nächste Schreibposition
                move.w  first_sek,D1    ;Ende der Datei
                subq.w  #1,D1           ;nur bis eins davor

                addi.l  #32,directory   ;nächsten Eintrag
                rts

;Macht einen Bootsektor ausführbar
;-> A0.L: Zeiger auf die beiden Bootsektoren
executable:     movem.l D0-A6,-(SP)

                movea.l A0,A2

                move.w  #255,D7
                moveq   #0,D1
loop1:          add.l   (A0)+,D1
                bcc.s   no_carry1
                addq.l  #1,D1
no_carry1:      dbra    D7,loop1
                not.l   D1
                add.l   4(A2),D1
                bcc.s   no_carry3
                addq.l  #1,D1
no_carry3:      move.l  D1,4(A2)        ;und eintragen

                movem.l (SP)+,D0-A6
                rts

;Dies ist ein ausführbarer Amiga-Bootsektor
boot:           DC.B 'DOS',0
checksum:       DC.L 0
root:           DC.L $0370

color:          move.w  D0,$DFF180
                addq.w  #1,D0
                bra.s   color


;Dies ist die Diskettenfunktion. Ist der Modus 0, wird das TOS zum Laden
;benutzt, sonst eine eigene Diskettenroutine. Diese Routine prüft vor dem
;Laden, ob ein entsprechendes Archiv vorhanden ist, bzw. fordert zur Einlage
;der jeweiligen Diskette auf. Gibt die tatsächliche Länge der gelesenen Daten
;zurück.
;long loaddisk(archiv,offset,länge,adr)
;-> 18(a6): Ladeadresse
;   14(a6): Länge der zu ladenden Daten
;   10(a6): Offset innerhalb des Archivs
;    8(a6): archiv [0..n]
mcode61:        link    A6,#0
                movem.l D1-A5,-(SP)

                tst.w   amigatool       ;Vom ST holen?
                beq     use_amigatool   ;ja

                move.w  8(A6),-(SP)     ;Nummer der Diskette
                bsr     mcode69         ;User soll ggf. Diskette wechseln
                addq.l  #2,SP

                move.l  10(A6),D0       ;Offset innerhalb Archiv
                add.l   14(A6),D0       ;Erstes freies Byte
                subq.l  #1,D0           ;Letztes benutztes Byte
                divu    #512,D0         ;Anzahl an Sektoren vom Start
                swap    D0
                move.w  D0,-(SP)        ;End_Offset
                swap    D0
                add.w   first_sek,D0    ;Archiv startet hier
                ext.l   D0
                divu    sek_per_track,D0 ;10 Sektoren pro Track
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
                add.w   first_sek,D0    ;Archiv startet hier
                ext.l   D0
                divu    sek_per_track,D0 ;10 Sektoren pro Track
                swap    D0              ;Sektornummer
                addq.w  #1,D0
                move.w  D0,-(SP)        ;Startsektor
                swap    D0
                move.w  D0,-(SP)        ;Starttrack
                clr.w   -(SP)           ;A:
                move.l  18(A6),-(SP)    ;Ladeadresse
                bsr     mcode68         ;Floppy_Read
                lea     18(SP),SP
                move.l  14(A6),D0       ;Länge der Daten zurückgeben

ende_load:      movem.l (SP)+,D1-A5
                unlk    A6
                rts

use_amigatool:  move.w  8(A6),D0        ;Archivnummer holen
                addi.w  #'0',D0
                move.b  D0,rom_nummer

try_again1:     moveq   #0,D1           ;Modus
                lea     rom_grfx(PC),A1
                moveq   #$3D,D0         ;Datei öffnen
                bsr     trap_1
                move.w  D0,fhandle
                bpl.s   datei_offen

hoch:           moveq   #$0F,D0
                bsr     flash_amiga
                bra.s   hoch

datei_offen:    move.l  10(A6),D1       ;Offset
                move.w  fhandle(PC),D2
                lea     rom_grfx(PC),A1
                moveq   #$42,D0         ;Lseek
                bsr     trap_1

                movea.l 18(A6),A0       ;Ladeadresse
                move.l  14(A6),D1       ;Länge
                move.w  fhandle(PC),D2
                lea     rom_grfx(PC),A1
                moveq   #$3F,D0         ;Fread
                bsr     trap_1
                move.l  D0,D7           ;Länge merken

                move.w  fhandle(PC),D1
                lea     rom_grfx(PC),A1
                moveq   #$3E,D0
                bsr     trap_1          ;Fclose

                move.l  D7,D0           ;und Länge zurückgeben
                bra.s   ende_load

;Einsprung in den Amigatool-Treiber:
trap_1:         movem.l D1-A6,-(SP)
                trap    #1
                movem.l (SP)+,D1-A6
                rts

rom_grfx:       DC.B 'G:\MEDUSA\MEDUSA_2.COL\ROM_GRFX.'
rom_nummer:     DC.B 'x',0
                EVEN
fhandle:        DC.W 0

;Diese Routine speichert einen Spielstand auf Diskette und zwar immer genau
;32256 Byte. (7 Tracks)
;int save_data(nr,adr)
mcode64:        link    A6,#0
                movem.l D1-A5,-(SP)

                move.w  #-1,-(SP)
                bsr     mcode69         ;Bitte Datendisk einlegen
                addq.l  #2,SP

                move.w  #512,-(SP)
                move.w  #9,-(SP)
                move.w  8(A6),D0
                mulu    #7,D0
                addi.w  #6+3,D0
                move.w  D0,-(SP)
                clr.w   -(SP)
                move.w  #1,-(SP)
                subq.w  #6,D0
                move.w  D0,-(SP)
                clr.w   -(SP)           ;A:
                move.l  10(A6),-(SP)
                bsr     mcode70         ;Daten schreiben
                lea     18(SP),SP

                movem.l (SP)+,D1-A5
                unlk    A6
                rts

;Diese Routine lädt einen Spielstand von Diskette und zwar immer genau
;32256 Bytes. (7 Tracks)
;int load_data(nr,adr)
mcode65:        link    A6,#0
                movem.l D1-A5,-(SP)

                move.w  #-1,-(SP)
                bsr     mcode69         ;Bitte Datendisk einlegen
                addq.l  #2,SP

                move.w  #512,-(SP)
                move.w  #9,-(SP)
                move.w  8(A6),D0
                mulu    #7,D0
                addi.w  #6+3,D0
                move.w  D0,-(SP)
                clr.w   -(SP)
                move.w  #1,-(SP)
                subq.w  #6,D0
                move.w  D0,-(SP)
                clr.w   -(SP)           ;A:
                move.l  10(A6),-(SP)
                bsr     mcode68
                lea     18(SP),SP

                movem.l (SP)+,D1-A5
                unlk    A6
                rts

;void floppy_read(void *adr,int laufwerk,int strack,int ssektor,int soffset,
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

                bsr     select          ;DF0: selektieren

                movea.l 8(A6),A2        ;Ladeadresse

                move.w  14(A6),D6       ;Starttrack
                bsr     seek_track

                moveq   #1,D0           ;1 Sektor lesen
                lea     bitfeld,A0      ;hierhin
                bsr     read_sektoren

                move.w  14(A6),D4       ;Starttrack
                move.w  20(A6),D5       ;Endtrack
                cmp.w   D4,D5           ;nur auf einem Track lesen?
                beq.s   nur_ein_track

mehr_als_einer: bsr     trans_start     ;Ab Startoffset übertragen

                addq.w  #1,16(A6)       ;nächsten Sektor geht's weiter
                move.w  sek_per_track,D0
                cmp.w   16(A6),D0       ;Startsektor 10?
                bge.s   no_step         ;Nein, also kein Step

vorne_anfangen: move.w  #1,16(A6)       ;Wieder bei Sektor #1 starten
                addq.w  #1,14(A6)       ;Auf nächsten Track gehen
                move.w  14(A6),D6
                bsr     seek_track      ;Track anfahren

no_step:        move.w  14(A6),D4       ;Starttrack
                cmp.w   20(A6),D4       ;Endtrack erreicht?
                beq.s   letzter_track

read_again2:    move.w  sek_per_track,D0
                addq.w  #1,D0
                sub.w   16(A6),D0       ;Anzahl der zu lesenden Sektoren
                movea.l A2,A0           ;Ab Ladeadresse laden
                bsr     read_sektoren
                adda.w  D5,A2           ;Bytes überspringen
                bra.s   vorne_anfangen

letzter_track:  move.w  22(A6),D0       ;Endsektor
                sub.w   16(A6),D0       ;-Startsektor
                beq.s   nur_ein_sektor
                move.w  D0,-(SP)        ;Anzahl der Sektoren retten
                movea.l A2,A0           ;Ladeadresse
                bsr     read_sektoren
                adda.w  D5,A2           ;Überspringen
                move.w  (SP)+,D0        ;Anzahl der gelesenen Sektoren
                add.w   D0,16(A6)       ;=Nächster Sektor

nur_ein_sektor: moveq   #1,D0
                lea     bitfeld,A0
                bsr     read_sektoren   ;Letzten Sektor lesen
                lea     bitfeld,A0
                move.w  24(A6),D7       ;Endoffset
trans3:         move.b  (A0)+,(A2)+     ;Rest übertragen
                dbra    D7,trans3
                bra.s   aus_is

nur_ein_track:  move.w  22(A6),D0       ;Endsektor
                sub.w   16(A6),D0       ;-Startsektor
                bne     mehr_als_einer

                lea     bitfeld,A0
                adda.w  18(A6),A0       ;+Startoffset
                move.w  24(A6),D7       ;Endoffset
                sub.w   18(A6),D7       ;-Startoffset
trans4:         move.b  (A0)+,(A2)+
                dbra    D7,trans4

aus_is:         bsr     deselect        ;Laufwerk wieder aus

                movem.l (SP)+,D0-A5
                unlk    A6
                rts


;Überträgt 1. Sektor ab Startoffset bis Sektorende
trans_start:    move.w  18(A6),D0       ;Startoffset
                lea     bitfeld,A0
                adda.w  D0,A0           ;Bytes überspringen
                move.w  #512,D1         ;Soviel Bytes kommen
                sub.w   D0,D1           ;Offset abziehen=Anzahl an Bytes
                bra.s   trans1
trans2:         move.b  (A0)+,(A2)+
trans1:         dbra    D1,trans2
                rts

;Liest ab Startsektor Sektoren ein. Gibt in D5 512*n zurück.
;-> D0: Anzahl der Sektoren
;   A0: Ladeadresse
read_sektoren:  movem.l D0-D4/D6-A6,-(SP)

                move.w  D0,-(SP)        ;Anzahl retten
read_error:     bsr     read_track      ;Track lesen
                move.w  (SP),D0         ;Anzahl holen
                move.w  16(A6),D7       ;Aktueller Sektor
                subq.w  #1,D7           ;Beim Amiga startet die Zählung bei 0
                bra.s   ss_dbra
sektor_trans:   tst.l   (A1)            ;Wurde überhaupt was geladen?
                beq.s   lesefehler
                move.w  track,D1
                cmp.b   1(A1),D1        ;Richtiger Track?
                bne.s   lesefehler
                cmp.b   2(A1),D7        ;Sektor gefunden?
                bne.s   nxt_sektor3
                addq.l  #4,A1           ;Miniheader überspringen
                moveq   #127,D6
ttz:            move.l  (A1)+,(A0)+     ;eintragen
                dbra    D6,ttz
                addq.w  #1,D7           ;nächster Sektor
ss_dbra:        lea     track_buffer,A1
                dbra    D0,sektor_trans

                move.w  (SP)+,D0
                move.w  D0,D5
                mulu    #512,D5         ;Soviel Bytes wurden gelesen
                movem.l (SP)+,D0-D4/D6-A6
                rts

nxt_sektor3:    lea     512+4(A1),A1
                bra.s   sektor_trans

lesefehler:     move.w  track,-(SP)     ;gewünschter Track
                moveq   #0,D6
                bsr     seek_track      ;Restore ausführen
                move.w  (SP)+,D6
                bsr     seek_track      ;und nochmal anfahren
                bra     read_error


;Liest einen Track von Diskette ein und dekodiert ihn.
read_track:     movem.l D0-A6,-(SP)

read_again:     lea     track_buffer,A0 ;hier hin lesen

                lea     $DFF000,A5
                move.w  #%1000001000010000,$96(A5) ;Disk DMA an
                move.w  #%111111100000000,$9E(A5) ;alle Bits aus
                move.w  #%1001010100000000,$9E(A5) ;MFM+Sync
                move.w  #$4489,$7E(A5)  ;Sync-Wert
                move.w  #$4489,(A0)+    ;1. Sync eintragen
                move.w  #$4000,$24(A5)  ;Disk-DMA aus
                move.l  A0,$20(A5)      ;DMA-Adresse
                bsr     wait_ready
                move.w  #12980,D0       ;Anzahl Bytes
                lsr.w   #1,D0
                or.w    #$8000,D0       ;DMA lesen
                move.w  D0,$24(A5)
                move.w  D0,$24(A5)
                bsr     wait_dma
                move.w  D0,timeout

                subq.l  #2,A0           ;Bufferadresse
                bsr.s   decode_track

                moveq   #11,D7
                bra.s   sek_dbra1
sek_loop:       tst.l   (A0)            ;Buffer zuende?
                beq.s   read_again
                cmp.b   2(A0),D7
                beq.s   sek_dbra1       ;Jawohl, gefunden
                lea     512+4(A0),A0
                bra.s   sek_loop
sek_dbra1:      lea     track_buffer,A0
                dbra    D7,sek_loop

                movem.l (SP)+,D0-A6
                rts


;Diese Routine decodiert einen Track.
;-> A0.L: Adresse des Tracks
decode_track:   movem.l D0-A6,-(SP)

                movea.l A0,A1           ;hier wieder hin

                lea     12800(A0),A3    ;Ende des Buffers
                move.l  #$55555555,D2   ;zum MFM dekodieren

such_sektor:    cmpi.w  #$4489,(A0)     ;Sync?
                bne.s   no_sync
weitere_sync:   cmpi.w  #$4489,2(A0)    ;2. Sync?
                bne.s   keine_zweite
                addq.l  #2,A0           ;Eine Sync weitergehen
                bra.s   weitere_sync
keine_zweite:   cmpi.b  #$55,2(A0)      ;$FF (Format-Mark)?
                bne.s   no_sync
                cmpi.b  #$55,6(A0)      ;2. Mark?
                bne.s   no_sync

                move.l  2(A0),D0
                move.l  6(A0),D1
                and.l   D2,D0
                and.l   D2,D1
                lsl.l   #1,D0
                or.l    D1,D0
                move.l  D0,(A1)         ;Buffer Mark merken

                lea     2(A0),A2        ;Sync nicht CRC
                moveq   #40,D0          ;40 Bytes
                bsr.s   calc_crc

                move.l  42(A0),D1
                move.l  46(A0),D3
                and.l   D2,D1
                and.l   D2,D3
                lsl.l   #1,D1
                or.l    D3,D1
                cmp.l   D1,D0           ;CRC korrekt?
                bne.s   crc_error

                lea     58(A0),A2       ;Datenbuffer
                move.w  #1024,D0
                bsr.s   calc_crc

                move.l  50(A0),D1
                move.l  54(A0),D3
                and.l   D2,D1
                and.l   D2,D3
                lsl.l   #1,D1
                or.l    D3,D1
                cmp.l   D1,D0           ;CRC korrekt?
                bne.s   crc_error

                lea     58(A0),A2       ;Datenbuffer
                addq.l  #4,A1           ;Header Ok
                bsr.s   decode_sektor

crc_error:
no_sync:        addq.l  #2,A0           ;nächstes Wort
                cmpa.l  A3,A0
                blt     such_sektor

                clr.l   (A1)            ;Ende der Sektoren

                movem.l (SP)+,D0-A6
                rts

;Diese Routine berechnet eine Prüfsumme über den angegebenen Bereich
;-> A2.L: Adresse des Bereichs (wird NICHT erhöht)
;   D0.L: Länge in Bytes
calc_crc:       movem.l D1-A6,-(SP)

                move.w  D0,D1
                lsr.w   #2,D1           ;Langworte
                subq.w  #1,D1
                moveq   #0,D0
crc_loop:       move.l  (A2)+,D2
                eor.l   D2,D0
                dbra    D1,crc_loop

                and.l   #$55555555,D0   ;Taktbits raus

                movem.l (SP)+,D1-A6
                rts

;Diese Routine dekodiert einen Sektor von A0 nach A1.
;-> A2.L: Quelle
;   A1.L: Ziel (wird erhöht)
decode_sektor:  movem.l D0-A0/A2-A6,-(SP)

                moveq   #127,D7         ;256 Langworte
                move.l  #$55555555,D2
dat_loop:       move.l  (A2)+,D0
                move.l  508(A2),D1
                and.l   D2,D1
                and.l   D2,D0
                lsl.l   #1,D0
                or.l    D1,D0
                move.l  D0,(A1)+
                dbra    D7,dat_loop

                movem.l (SP)+,D0-A0/A2-A6
                rts


;User soll eine bestimmte Diskette einlegen.
;void insert_disk(int nr);
;-> 8(A6): Diskettennummer [0..n] -1=Datendiskette
mcode69:        link    A6,#0
                movem.l D0-A5,-(SP)

                move.w  8(A6),D0        ;gewünschte Disk
                cmp.w   disk_drin,D0    ;Ist die etwa noch drin?
                beq     nichts

insert_again:   move.w  #511,-(SP)      ;End_Offset
                move.w  #1,-(SP)        ;End_Sektor
                clr.w   -(SP)           ;End_Track
                clr.w   -(SP)           ;Start_Offset
                move.w  #1,-(SP)        ;Start_Sektor
                clr.w   -(SP)           ;Start_Track
                clr.w   -(SP)           ;A:
                pea     bitfeld+512
                bsr     mcode68         ;Bootsektor lesen
                lea     18(SP),SP
                lea     bitfeld+512,A0
                move.b  $19(A0),D0
                lsl.w   #8,D0
                move.b  $18(A0),D0
                move.w  D0,sek_per_track ;Sektoren pro Track
                move.b  $12(A0),D0
                lsl.w   #8,D0
                move.b  $11(A0),D0      ;Verzeichnisgröße
                ext.l   D0
                divu    #16,D0
                move.w  D0,D1           ;Anzahl an Sektoren fürs Directory
                swap    D0
                tst.w   D0
                beq.s   geht_auf
                addq.w  #1,D1           ;+1 für den Rest
geht_auf:       move.b  $17(A0),D0
                lsl.w   #8,D0
                move.b  $16(A0),D0      ;Größe der FAT
                add.w   D0,D0           ;da 2 FATs
                addq.w  #2,D0           ;für Bootsektor
                move.w  D0,D2
                add.w   D1,D2           ;+Größe des Directory=erster Datensektor
                move.w  D2,first_sek
                ext.l   D0
                divu    sek_per_track,D0 ;10 Sektoren/Track

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
                pea     bitfeld+1024+512
                bsr     mcode68         ;Directory lesen
                lea     18(SP),SP

                move.w  8(A6),D0
                bpl.s   archiv_disk
                moveq   #0,D0
                bra.s   donne
archiv_disk:    addi.w  #'0',D0
donne:          move.b  D0,rom+8        ;Nummer des Archivs eintragen

                lea     bitfeld+1024+512,A0
                lea     rom,A1
                moveq   #15,D6          ;16 Einträge pro Directory
nxt2:           moveq   #0,D7
kein_ende:      move.b  0(A0,D7.w),D0   ;Zeichen von Diskette lesen
                cmp.b   0(A1,D7.w),D0   ;ROM_GRFX.x?
                bne.s   nxt_entry
                addq.w  #1,D7
                tst.b   0(A1,D7.w)
                bne.s   kein_ende
                move.b  27(A0),D0
                lsl.w   #8,D0
                move.b  26(A0),D0       ;Startcluster der Datei holen
                move.w  D0,first_sek    ;1. Sektor der Daten

                tst.w   8(A6)           ;Sollte Datendisk eingelegt werden?
                bmi.s   falsche_disk

nichts:         move.w  8(A6),disk_drin

                movem.l (SP)+,D0-A5
                unlk    A6
                rts

nxt_entry:      lea     32(A0),A0       ;Einen Eintrag weiter
                dbra    D6,nxt2

                tst.w   8(A6)           ;Sollte Datendisk eingelegt werden?
                bmi.s   nichts          ;Ja, also okay

falsche_disk:   bsr     deselect        ;Alle Lichter aus
                move.w  8(A6),D0        ;Archivnummer
                bmi.s   datendiskette
                addi.w  #'A',D0
                move.b  D0,please+20

                lea     please,A0
                jsr     alert           ;und Alert ausgeben

                bra     insert_again

datendiskette:  lea     please_data,A0
                jsr     alert
                bra     insert_again

please:         DC.B "PLEASE INSERT DISK 'x' INTO DRIVE A:",0
please_data:    DC.B "PLEASE INSERT YOUR SAVE GAME DISK INTO DRIVE A:",0
rom:            DC.B 'ROM_GRFXx',0

                EVEN


;Diese Funktion schreibt eine Reihe von Sektoren auf die Diskette.
;Achtung! Dies geschieht unter Mithilfe von hlpbuf, d.h. hlpbuf wird zerstört!
;Desweiteren wird ein Verify durchgeführt.

;void floppy_write(void *adr,int strack,int ssektor,int soffset,
;                          int etrack,int esektor,int eoffset);
;-> 24(a6): eoffset
;   22(a6): esektor
;   20(a6): etrack
;   18(a6): soffset
;   16(a6): ssektor
;   14(a6): strack
;   12(a6): Laufwerk (Bit 0=Laufwerk, Bit 1=Seite)
;    8(a6): adr
;<- 0=Okay, Sonst Fehler
mcode70:        link    A6,#0
                movem.l D0-A5,-(SP)

                clr.w   disk_error      ;kein Fehler bisher

                bsr     select          ;DF0: selektieren

                movea.l 8(A6),A2        ;Speicheradresse

                move.w  14(A6),D6       ;Starttrack
                bsr     seek_track

                moveq   #1,D0           ;1. Sektor lesen
                lea     bitfeld,A0
                bsr     read_sektoren

                move.w  14(A6),D4       ;Starttrack
                move.w  20(A6),D5       ;Endtrack
                cmp.w   D4,D5           ;nur auf einem Track lesen?
                beq     nur_ein_track2

mehr_als_einer2:
                move.w  18(A6),D0       ;Startoffset
                lea     bitfeld,A0
                adda.w  D0,A0           ;Bytes überspringen
                move.w  #512,D1         ;Soviel Bytes kommen
                sub.w   D0,D1           ;Offset abziehen=Anzahl an Bytes
                bra.s   trans33
trans44:        move.b  (A2)+,(A0)+     ;In Buffer eintragen
trans33:        dbra    D1,trans44

                moveq   #1,D0
                lea     bitfeld,A0      ;hier steht der neue Sektor
                bsr     write_sektoren  ;und Sektor wieder zurückschreiben

                addq.w  #1,16(A6)       ;nächsten Sektor geht's weiter
                move.w  sek_per_track,D0
                cmp.w   16(A6),D0       ;Startsektor 10?
                bge.s   no_step2        ;Nein, also kein Step

vorne_anfangen2:move.w  #1,16(A6)       ;Wieder bei Sektor #1 starten
                addq.w  #1,14(A6)       ;Auf nächsten Track gehen
                move.w  14(A6),D6
                bsr     seek_track      ;Track anfahren

no_step2:       move.w  14(A6),D4       ;Starttrack
                cmp.w   20(A6),D4       ;Endtrack erreicht?
                beq.s   letzter_track2

                move.w  sek_per_track,D0
                addq.w  #1,D0
                sub.w   16(A6),D0       ;Anzahl der zu schreibenden Sektoren
                movea.l A2,A0           ;Ab Speicheradresse speichern
                bsr     write_sektoren
                adda.w  D5,A2           ;Bytes überspringen
                bra.s   vorne_anfangen2

letzter_track2: move.w  22(A6),D0       ;Endsektor
                sub.w   16(A6),D0       ;-Startsektor
                beq.s   nur_ein_sektor2
                move.w  D0,-(SP)        ;Anzahl der Sektoren retten
                movea.l A2,A0           ;Ladeadresse
                bsr.s   write_sektoren
                adda.w  D5,A2           ;Überspringen
                move.w  (SP)+,D0        ;Anzahl der gelesenen Sektoren
                add.w   D0,16(A6)       ;=Nächster Sektor

nur_ein_sektor2:moveq   #1,D0
                lea     bitfeld,A0
                bsr     read_sektoren   ;Letzten Sektor lesen
                lea     bitfeld,A0
                move.w  24(A6),D7       ;Endoffset
trans6:         move.b  (A2)+,(A0)+     ;Rest übertragen
                dbra    D7,trans6

                moveq   #1,D0
                lea     bitfeld,A0
                bsr     write_sektoren
                bra.s   aus_is2

nur_ein_track2: move.w  22(A6),D0       ;Endsektor
                sub.w   16(A6),D0       ;-Startsektor
                bne     mehr_als_einer2

                lea     bitfeld,A0
                adda.w  18(A6),A0       ;+Startoffset
                move.w  24(A6),D7       ;Endoffset
                sub.w   18(A6),D7       ;-Startoffset
trans5:         move.b  (A2)+,(A0)+
                dbra    D7,trans5

                moveq   #1,D0
                lea     bitfeld,A0
                bsr     write_sektoren

aus_is2:        bsr     deselect

                movem.l (SP)+,D0-A5
                unlk    A6

                move.w  disk_error,D0   ;Fehlercode zurückgeben
                rts


;Schreibt ab Startsektor Sektoren. Gibt in D5 512*n zurück.
;-> D0: Anzahl der Sektoren
;   A0: Ladeadresse
write_sektoren: movem.l D0-A6,-(SP)

                move.w  #5,retries      ;5 Versuche

                tst.w   D0              ;wenigstens 1 Sektor?
                ble     schreib_ende

                cmp.w   sek_per_track,D0 ;Alle Sektoren der Disk?
                bne.s   single

                lea     track_buffer,A1
                move.w  track,D1
                moveq   #0,D7
bau_track:      move.b  #$FF,(A1)+
                move.b  D1,(A1)+        ;Trackgerüst aufbauen
                move.b  D7,(A1)+
                lea     513(A1),A1
                addq.w  #1,D7
                cmp.w   sek_per_track,D7
                blt.s   bau_track
                clr.l   (A1)            ;Ende der Liste

                move.w  D0,-(SP)        ;Anzahl auf Stack
                bra.s   einfüllen

single:         move.w  D0,-(SP)
rewrite:        bsr     read_track      ;Track einlesen+decode
                move.w  (SP),D0         ;Anzahl holen

einfüllen:      move.w  16(A6),D7       ;Startsektor
                subq.w  #1,D7           ;beim Amiga startet die Sektor-Zählung bei 0
                bra.s   sektor_dbra

sektor_loop:    tst.l   (A1)            ;Track zuende?
                beq.s   schreibfehler   ;Sektor nicht gefunden

                cmp.b   2(A1),D7        ;aktueller Sektor?
                bne.s   nxt_sektor

                addq.l  #4,A1           ;Miniheader überspringen

sektor_copy:    moveq   #127,D6
trans17:        move.l  (A0)+,(A1)+     ;Sektor eintragen
                dbra    D6,trans17
                addq.w  #1,D7           ;nächster Sektor
sektor_dbra:    lea     track_buffer,A1 ;von vorne suchen
                dbra    D0,sektor_loop
                bra.s   schreib_track

nxt_sektor:     lea     512+4(A1),A1
                bra.s   sektor_loop

;In den gelesenen Daten befindet sich nicht der Sektor der geschrieben
;werden soll.
schreibfehler:  move.w  #$F0,D0
                bsr     flash_amiga
                move.w  track,-(SP)
                moveq   #0,D6
                bsr     seek_track
                move.w  (SP)+,D6
                bsr     seek_track

                subq.w  #1,retries      ;1 Versuch weniger
                bpl.s   rewrite         ;nochmal

                move.w  #-1,disk_error  ;Fehler!
                tst.w   (SP)+           ;D0 runterholen
                bra     schreib_ende

schreib_track:  tst.w   (SP)+           ;Anzahl runterholen

                move.w  #2,retries      ;3 Versuche

write_again:    lea     track_buffer,A0
                lea     hlpbuf,A1
                bsr     code_track      ;Track codieren
                lea     hlpbuf,A5
                moveq   #0,D1
                bsr     write_track     ;und wegschreiben

                lea     track_buffer,A0
                lea     hlpbuf,A1
                move.w  sek_per_track,D7
                mulu    #(4+512)/4,D7
                bra.s   fdj_dbra
fdj:            move.l  (A0)+,(A1)+     ;Track merken
fdj_dbra:       dbra    D7,fdj

                bsr     read_track      ;Track wieder einlesen

                moveq   #11,D7          ;Sektorcounter=11
                bra.s   sek_dbra
verify_sek:     tst.l   (A1)
                beq.s   verify_error1
                cmp.b   2(A1),D7        ;Sektor gefunden?
                bne.s   nxt_sek

                lea     track_buffer,A0
disk_loop:      tst.l   (A0)
                beq     verify_error2   ;Sektor nicht gefunden
                cmp.b   2(A0),D7
                bne.s   nxt_ss

                addq.l  #4,A0
                addq.l  #4,A1           ;Miniheader überspringen

                moveq   #127,D5
verify_loop2:   cmpm.l  (A0)+,(A1)+
                bne.s   verify_error3
                dbra    D5,verify_loop2

sek_dbra:       lea     hlpbuf,A1
                dbra    D7,verify_sek

                bra.s   schreib_ende    ;Alles Okay

nxt_ss:         lea     512+4(A0),A0
                bra.s   disk_loop

nxt_sek:        lea     512+4(A1),A1
                bra.s   verify_sek

schreib_ende:   movem.l (SP)+,D0-A6
                move.w  D0,D5
                mulu    #512,D5         ;Anzahl zurückgeben
                rts

verify_error1:  move.w  #-2,disk_error
                move.w  #$F0,D0
                bra     verify_error
verify_error2:  move.w  #-3,disk_error
                move.w  #$0FF0,D0
                bra     verify_error
verify_error3:  move.w  #-4,disk_error
                move.w  #$0F00,D0

verify_error:   bsr     flash_amiga

                lea     track_buffer,A1
                lea     hlpbuf,A0
                move.w  sek_per_track,D7
                mulu    #(4+512)/4,D7
                bra.s   fdj2_dbra
fdj2:           move.l  (A0)+,(A1)+     ;Track zurückschreiben
fdj2_dbra:      dbra    D7,fdj2

                subq.w  #1,retries
                bpl     write_again

                tst.w   amigatool       ;Amigatool vorhanden?
                bne.s   no_tool9

                lea     ver_error,A1
                moveq   #9,D0
                bsr     trap_1

no_tool9:       bra     schreib_ende

;Schreibt Track auf Diskette (Routine aus Amigacopy)
;-> D0.W: Anzahl an Bytes zu schreiben
;   D1.W: Synchronisiert mit Index oder nicht (<>0=Synchronisiert)
;   A5.L: Adresse
write_track:    movem.l D2-D3,-(SP)
                move.w  D1,D2
                move.w  D0,D3
                bsr     test_change     ;Diskettenwechsel feststellen
                tst.l   D0
                bmi     write_error
                move.b  $BFE001,D0
                btst    #3,D0           ;Write Protect?
                bne     L00B8           ;->Fehler->RTS
                bra     write_error
L00B8:          lsr.w   #1,D3
                move.w  #2,$DFF09C      ;Flags rücksetzen
                move.l  A5,$DFF020      ;Adresse setzen
                move.w  #$8210,$DFF096
                move.w  #$7F00,$DFF09E
                move.w  #$8100,$DFF09E
                cmpi.w  #$50,track      ;Track 80?
                bcs     L00B9
                move.w  #$A000,$DFF09E  ;Sonst Precomp
L00B9:          move.w  #$4000,$DFF024  ;DMA aus
                cmp.w   #0,D2
                beq     L00BA           ;synchronisiertes Schreiben?
                bsr     wait_index
                tst.l   D0
                bmi     write_error
L00BA:          or.w    #$C000,D3
                move.w  D3,$DFF024      ;und schreiben
                move.w  D3,$DFF024
                clr.l   D0
                move.l  #$020000,D1
L00BB:          move.w  $DFF01E,D2
                btst    #1,D2
                bne.s   L00BD
                subq.l  #1,D1
                bne.s   L00BB
write_error:    move.l  #-1,D0
L00BD:          move.w  #$4000,$DFF024
                movem.l (SP)+,D2-D3
                rts


;Testet einen Diskettenwechsel:
test_change:    clr.l   D0
                move.b  $BFE001,D0
                btst    #2,D0
                bne     L0081
                move.l  #-1,D0
L0081:          rts

;Wartet 6 Umdrehungen
wait_6:         movem.l D0-A6,-(SP)

                moveq   #5,D7
index_loop:     bsr     wait_index
                dbra    D7,index_loop

                movem.l (SP)+,D0-A6
                rts

;Wartet auf den Index-Impuls:
wait_index:     move.l  D1,-(SP)
                clr.l   D0
                move.l  #$030000,D1
                move.b  $BFDD00,D0
L0088:          move.b  $BFDD00,D0
                btst    #4,D0           ;Index Impuls da?
                bne.s   L0089           ;ja->zurück
                sub.l   #1,D1
                bne.s   L0088           ;weiter warten
                move.l  #-1,D0
L0089:          move.l  (SP)+,D1
                rts


;Diese Routine codiert einen Track ins MFM-Format:
;-> A0.L: Adresse des unverschlüsselten Tracks
;   A1.L: Adresse des MFM-Tracks
;<- D0.L: Länge des Tracks in Bytes
code_track:     movem.l D1-A6,-(SP)

                move.w  #1000/4,D0
track_gap:      move.l  #$AAAAAAAA,(A1)+ ;Lücke vor den Track machen
                dbra    D0,track_gap

                movea.l A0,A2           ;Trackadresse merken
                moveq   #0,D7           ;Startsektor
sek_loop3:      movea.l A2,A0           ;Trackadresse holen
sek_loop2:      tst.l   (A0)            ;Trackende erreicht?
                beq.s   code_end
                cmp.b   2(A0),D7        ;richtiger Sektor?
                bne.s   nxt_sektor2

                moveq   #11,D0
                sub.w   D7,D0           ;Anzahl an Sektoren bis zum Trackende
                move.b  D0,3(A0)        ;eintragen

                bsr.s   code_sektor     ;Diesen Sektor codieren
                addq.w  #1,D7           ;nächster Sektor
                cmp.w   #12,D7
                blt.s   sek_loop3
                bra.s   code_end

nxt_sektor2:    lea     512+4(A0),A0
                bra.s   sek_loop2

code_end:       move.l  A1,D0           ;Trackende
                addq.l  #2,D0           ;+2
                movem.l (SP)+,D1-A6
                sub.l   A1,D0           ;-Trackstart=Länge
                rts

;Diese Routine codiert einen Sektor ins MFM-Format
;-> A0.L: Adresse des uncodierten Sektors (wird erhöht)
;   A1.L: Adresse des MFM-Buffers (wird erhöht)
code_sektor:    movem.l D0-D7/A2-A6,-(SP)

                movea.l A1,A5           ;Adresse merken

                moveq   #7,D7
                moveq   #0,D0
code_0:         bsr     code_long       ;Header löschen
                dbra    D7,code_0

                movea.l A5,A1           ;zurück auf Header
                moveq   #0,D0
                bsr     code_long       ;$aaaaaaaa eintragen
                move.l  #$44894489,4(A5) ;Sync eintragen

                move.l  (A0)+,D0        ;Sektorheader
                lea     8(A5),A1        ;Position des Headers
                bsr     code_long

                lea     8(A5),A2        ;Start des Headers
                moveq   #40,D0          ;40 Bytes
                bsr     calc_crc
                lea     48(A5),A1
                bsr     code_long       ;CRC eintragen

                move.w  #512,D0         ;Sektorlänge
                lea     64(A5),A1       ;Zeiger hinter Header
                bsr.s   code_block

                lea     64(A5),A2
                move.w  #1024,D0        ;Länge in MFM Format
                bsr     calc_crc

                lea     56(A5),A1
                bsr     code_long       ;CRC eintragen

                lea     512(A0),A0      ;Sektor überspringen
                lea     64+1024(A5),A1  ;Header+MFM überspringen

                movem.l (SP)+,D0-D7/A2-A6
                rts

;Kodiert einen 512 Bytes großen Datenblock ins MFM-Format
;-> A0.L: Adresse des unverschlüsselten Sektors
;   A1.L: Adresse des MFM-Formats
;   D0.W: Länge des unverschlüsselten in Bytes
code_block:     movem.l D0-A6,-(SP)

                movea.l A0,A2
                movea.l A1,A3
                moveq   #0,D2
                move.w  D0,D2           ;Blocklänge
                move.w  D0,D1           ;Blocklänge
                lsl.w   #2,D1
                or.w    #8,D1           ;Wert für Blitterstart
                bsr     wait_blitter    ;warten bis Blitter fertig
                bsr.s   blitter_code
                movea.l A3,A0           ;Zeiger auf codierten Block
                bsr     randsetzen
                adda.l  D2,A0           ;Zeiger auf 1. Lücke
                bsr     randsetzen
                adda.l  D2,A0           ;Zeiger auf Blockende
                bsr     randsetzen

                movem.l (SP)+,D0-A6
                rts

;Diese Routine führt einen Blittertransfer durch (Blockkodierung)
;-> A0.L: Quelle
;   A1.L: Ziel
;   D0.W: Länge des Quelle
;   D1.W: Blitterstartwert
blitter_code:   movem.l D2-D3/A2-A4,-(SP)

                moveq   #0,D2
                move.w  D0,D2
                move.w  D1,D3

                movea.l A0,A2
                movea.l A1,A3
                bsr     blitmask

                lea     $DFF000,A4
                move.l  A2,$4C(A4)      ;Zeiger auf Source B
                move.l  A2,$50(A4)      ;-"- A
                move.l  A3,$54(A4)      ;Zeiger auf Destination
                move.w  #$1DB1,$40(A4)  ;Blittercontrol 0
                move.w  #0,$42(A4)      ;Blittercontrol 1
                move.w  D3,D0
                bsr     start_blit
                move.l  A3,$4C(A4)      ;Zeiger auf Source B
                move.l  A2,$50(A4)      ;Zeiger auf Source A
                move.l  A3,$54(A4)      ;Zeiger auf Destination
                move.w  #$2D8C,$40(A4)  ;Blittercontrol 0
                move.w  D3,D0
                bsr.s   start_blit

                movea.l A2,A0           ;Zeiger auf Quelle
                movea.l A3,A1           ;Zeiger auf Ziel
                adda.l  D2,A0           ;Zeiger auf Ende der Quelle
                subq.l  #2,A0           ;Zeiger auf letztes Wort
                adda.l  D2,A1
                adda.l  D2,A1           ;Zeiger auf Ende des Ziel
                subq.l  #2,A1
                move.l  A0,$4C(A4)      ;Source B
                move.l  A0,$50(A4)      ;Source A
                move.l  A1,$54(A4)      ;Destination
                move.w  #$0DB1,$40(A4)  ;Blittercontrol 0
                move.w  #$1002,$42(A4)  ;Blittercontrol 1
                move.w  D3,D0
                bsr.s   start_blit

                movea.l A2,A0
                movea.l A3,A1
                adda.l  D2,A1
                move.l  A1,$4C(A4)      ;Source B
                move.l  A0,$50(A4)      ;Source A
                move.l  A1,$54(A4)      ;Destination
                move.w  #$1D8C,$40(A4)  ;Blittercontrol 0
                move.w  #0,$42(A4)      ;Blittercontrol 1
                move.w  D3,D0
                bsr.s   start_blit

                movem.l (SP)+,D2-D3/A2-A4
                rts

;Maskenregister für Blitter setzen
blitmask:       lea     $DFF000,A0
                moveq   #0,D0
                lea     $44(A0),A1      ;Zeiger auf 1. Maskword
                move.l  #-1,(A1)
                lea     $62(A0),A1      ;Zeiger auf Modulo
                move.l  D0,(A1)+
                move.w  D0,(A1)+        ;Blittermodulo setzen
                addq.l  #8,A1
                move.w  #$5555,(A1)     ;Blittersource C
                rts

;Blitter starten und auf dessen Ende warten
start_blit:     move.w  D0,$DFF058      ;GO!

wait_blitter:   btst    #6,$DFF002
                bne.s   wait_blitter
                rts


;Codiert ein Langwort ins MFM-Format:
;-> D0.L: Langwort
;   A1.L: Schreibadresse (wird erhöht)
code_long:      movem.l D0-D7,-(SP)

                move.l  D0,D3
                lsr.l   #1,D0
                bsr.s   code_1
                move.l  D3,D0
                bsr.s   code_1
                bsr.s   randsetzen

                movem.l (SP)+,D0-D7
                rts

code_1:         and.l   #$55555555,D0
                move.l  D0,D2
                eori.l  #$55555555,D2
                move.l  D2,D1
                lsl.l   #1,D2
                lsr.l   #1,D1
                bset    #31,D1
                and.l   D2,D1
                or.l    D1,D0
                btst    #0,-1(A1)
                beq.s   no_set
                bclr    #31,D0
no_set:         move.l  D0,(A1)+
                rts

randsetzen:     move.b  (A1),D0
                btst    #0,-1(A1)
                bne.s   kein_4
                btst    #6,D0
                bne.s   kein_6
                bset    #7,D0
                bra.s   kein_5
kein_4:         bclr    #7,D0
kein_5:         move.b  D0,(A1)
kein_6:         rts




;Formatiert eine ganze Diskette:
format_disk:    movem.l D0-A6,-(SP)

                bsr     select          ;DF0: selektieren

                move.w  #0,D0
format_loop:    bsr     format_track
                addq.w  #1,D0
                cmp.w   #160,D0
                blt.s   format_loop

                bsr     deselect        ;und ausmachen

                movem.l (SP)+,D0-A6
                rts

;Diese Routine formatiert einen Track:
;-> D0.W: Tracknummer
format_track:   movem.l D0-A6,-(SP)

                move.w  D0,D6
                bsr     seek_track      ;Track anfahren

                lea     track_buffer,A0
                moveq   #0,D7
sek_loop89:     move.b  #$FF,(A0)+
                move.b  D0,(A0)+        ;Tracknummer
                move.b  D7,(A0)+
                clr.b   (A0)+           ;Gap
                moveq   #127,D6
fill_sek:       move.l  #'Till',(A0)+
                dbra    D6,fill_sek
                addq.w  #1,D7
                cmp.w   #11,D7
                bne.s   sek_loop89
                clr.l   (A0)+

                lea     hlpbuf,A0
                move.w  #20000/4,D7
fill:           move.l  #$AAAAAAAA,(A0)+ ;komplett leeren Track
                dbra    D7,fill
                lea     hlpbuf,A5
                moveq   #0,D1
                move.w  #20000,D0       ;Soviel Bytes schreiben
                bsr     write_track

                lea     track_buffer,A0 ;Ab hier stehen die Sektoren
                lea     hlpbuf,A1
                bsr     code_track      ;Track codieren
                lea     hlpbuf,A5
                moveq   #0,D1
                bsr     write_track     ;und wegschreiben

                movem.l (SP)+,D0-A6
                rts


;Bringt den Amiga zum Flashen
flash:          move.l  D0,-(SP)

                moveq   #-1,D0
flash_loop:     move.w  D0,$DFF180
                dbra    D0,flash_loop

wait_lll:       dbra    D0,wait_lll

                move.l  (SP)+,D0
                rts


;Routine selektiert Laufwerk df0:.
select:         move.b  #%1111111,$BFD100 ;DF0: selektieren
                nop
                nop
                move.b  #%1110111,$BFD100 ;Motor an, DF0:
                nop
                nop
                bsr     wait_ready      ;Auf Laufwerk warten
                bra     wait_6          ;Noch 6 Umdrehungen warten

;Diese Routine deselektiert Laufwerk DF0:
deselect:       move.b  #%11111111,$BFD100 ;DF0: deselektieren
                nop
                nop
                move.b  #%11110111,$BFD100 ;Motor aus
                rts


;Routine fährt einen bestimmten Track an und selektiert entspr. Seite
;-> D6.W: gewünschter Track
seek_track:     tst.w   track           ;Ist dies der 1. Aufruf?
                bpl.s   seek            ;Nein, direkt anfahren

                move.l  D6,-(SP)        ;gewünschter Track merken
                moveq   #0,D6
                bsr     seek            ;zuerst Restore
                move.l  (SP)+,D6


;Fährt Track an
;-> D6.W: Track
seek:           movem.l D0-A6,-(SP)

                tst.w   amigatool
                bne.s   kein_tool1

                move.w  D6,D0
                ext.l   D0
                divu    #100,D0
                add.w   #'0',D0
                move.b  D0,track_nr
                swap    D0
                ext.l   D0
                divu    #10,D0
                add.w   #'0',D0
                move.b  D0,track_nr+1
                swap    D0
                add.w   #'0',D0
                move.b  D0,track_nr+2
                moveq   #9,D0
                lea     track_str,A1
                bsr     trap_1

kein_tool1:     move.w  track(PC),D7    ;aktueller Track
                lsr.w   #1,D7           ;tatsächliche Tracknummer
                lsr.w   #1,D6           ;gewünschter Track
                bcs.s   untere_seite
                bset    #2,$BFD100      ;Obere Seite
                bra.s   seeken
untere_seite:   bclr    #2,$BFD100

seeken:         tst.w   D6              ;Track 0?
                beq.s   restore

seek_loop:      cmp.w   D7,D6           ;Sind wir schon auf Track?
                beq.s   seek_end
                bgt.s   nach_innen
                bsr.s   aussen
                subq.w  #1,D7
                bra.s   seek_loop
nach_innen:     bsr.s   innen
                addq.w  #1,D7
                bra.s   seek_loop

restore:        btst    #4,$BFE001      ;Kopf auf Spur 0?
                beq.s   seek_end
                bsr.s   aussen
                bra.s   restore

seek_end:       movem.l (SP)+,D0-A6
                move.w  D6,track        ;angekommen
                rts

;Bewegt Kopf auf Track 80 zu:
innen:          movem.l D0-A6,-(SP)
                bsr     wait_ready
                bclr    #1,$BFD100
                bclr    #0,$BFD100
                bset    #0,$BFD100
                bsr.s   minipause
                bsr     wait_ready
                movem.l (SP)+,D0-A6
                rts

;Bewegt Kopf auf Track 0 zu:
aussen:         movem.l D0-A6,-(SP)
                bsr     wait_ready
                bset    #1,$BFD100
                bclr    #0,$BFD100
                bset    #0,$BFD100
                bsr.s   minipause
                bsr     wait_ready
                movem.l (SP)+,D0-A6
                rts

pause:          move.w  #$C000,D7
loop2:          dbra    D7,loop2
                rts

minipause:      move.w  #$2000,D7
mini2:          dbra    D7,mini2
                rts

wait_a_little:  move.l  D0,-(SP)
                move.w  #$0180,D0
wait77:         nop
                dbra    D0,wait77
                move.l  (SP)+,D0
                rts

;Diese Routine wartet auf das Ende einer DMA-Floppyaktion
;<- D0.W: 0=Kein Timeout, -1=Timeout
wait_dma:       movem.l D1-A6,-(SP)

                move.l  #$020000,D0
                move.w  #$02,$DFF09C
wait_loop:      move.w  $DFF01E,D1
                btst    #1,D1
                bne.s   fertig
                subq.l  #1,D0
                bne.s   wait_loop

fertig:         move.w  #$00,$DFF024    ;DMA ausschalten
                movem.l (SP)+,D1-A6
                rts

;Wartet auf die Ausführung eines normalen Diskbefehls:
wait_ready:     btst    #5,$BFE001
                bne.s   wait_ready
                rts

flash_amiga:    movem.l D0-A6,-(SP)

                moveq   #2,D6
                moveq   #-1,D7
loop5:          move.w  D0,$DFF180
                move.w  #0,$DFF180
                dbra    D7,loop5
                dbra    D6,loop5

                move.w  #0,$DFF180      ;Hintergrundfarbe schwarz

                movem.l (SP)+,D0-A6
                rts

bootsek_fn:     DC.B 'E:\MEDUSA\MEDUSA_2.COL\MCODE\AMIGA\AMIBOOT.B',0
loader_fn:      DC.B 'E:\MEDUSA\MEDUSA_2.COL\MCODE\AMIGA\LOADER.ABS',0
loader_dir:     DC.B 'LOADER  PRG',0
ram_medusa_fn:  DC.B 'O:\MEDUSA_A.PRG',0
medusa_fn:      DC.B 'E:\MEDUSA\MEDUSA_2.COL\C\MEDUSA_A.PRG',0
medusa_dir:     DC.B 'MEDUSA  PRG',0
rom0_fn:        DC.B 'D:\MEDUSA.DAT\MEDUSA_2.COL\ROM_GRFX.0',0
rom0_dir:       DC.B 'ROM_GRFX0  ',0
rom1_fn:        DC.B 'D:\MEDUSA.DAT\MEDUSA_2.COL\ROM_GRFX.1',0
rom1_dir:       DC.B 'ROM_GRFX1  ',0
rom2_fn:        DC.B 'D:\MEDUSA.DAT\MEDUSA_2.COL\ROM_GRFX.2',0
rom2_dir:       DC.B 'ROM_GRFX2  ',0

message:        DC.B 27,"E"
                DC.B 'Installationsprogramm "The Return of Medusa"',13,10
                DC.B '(C) 1991 by Till Bubeck',13,10,10
                DC.B 'Wenn die Disk auch formatiert werden soll, bitte "+" drücken. ',13,10,0

track_str:      DC.B 13,'Track: '
track_nr:       DC.B 'xxx',0
ver_error:      DC.B '    <- Verify Error!',13,10,0

amigatool:      DC.W 0          ;Kein Amigatool vorhanden (für den Loader)
track_at_buffer:DC.W -1         ;Track im Buffer (ungültig)
first_sek:      DC.W 10         ;nächster Schreibstart (0..n)
sek_per_track:  DC.W 11

disk_drin:      DC.W -10        ;Welche Disk ist jetzt im Laufwerk? (Noch keine)
track:          DC.W -1         ;Trackposition unbestimmt

directory:      DC.L dir        ;Pointer auf Directory
dir:            DS.B 512        ;Hier Directory aufbauen

                BSS

format:         DS.W 1

disk_error:     DS.W 1          ;Disketten Fehler
retries:        DS.W 1          ;Anzahl der Schreibversuche

st_adresse:     DS.W 10

timeout:        DS.W 1

till_buff:      DS.B 16384
track_buffer:   DS.B 16384      ;Trackbuffer

verify_buffer:  DS.B 11*516

hlpbuf:         DS.B 32000      ;Schreib-Hilfsspeicher
bitfeld:        DS.B 4000

                END
