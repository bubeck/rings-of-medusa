
;************************************************************************
;* Header-Library, Programmrumpf                                        *
;* Ω1989 by ‰-soft, written by Markus Fritze           18.06.1988 01:30 *
;************************************************************************
                movea.l 4(SP),A6             ;Basepageadresse holen
                movea.w #$0100+$0400,A5      ;Grîûe der Basepage + Stackgrîûe (1k)
                adda.l  12(A6),A5            ;+ Grîûe des TEXT-Segments
                adda.l  20(A6),A5            ;+ Grîûe des DATA-Segments
                adda.l  28(A6),A5            ;+ Grîûe des BSS-Segments
                move.l  A5,D1                ;= GesamtlÑnge des Programms
                and.w   #$FFFE,D1            ;LÑnge nun gerade
                add.l   A6,D1                ;+ Programmstart (Basepageadresse)
                movea.l D1,SP                ;Stack endet dort
                move.l  A5,-(SP)             ;ProgrammlÑnge
                move.l  A6,-(SP)             ;Adresse der Basepage
                move.l  #$004A0000,-(SP)     ;Funktionsnummer + Dummyword (0)
                trap    #1                   ;Mshrink(0,Basepageadr,PrglÑnge)
                lea     12(SP),SP            ;Nur noch den Stack korrigieren

                lea     titel(PC),A0
                bsr     printstring

                bsr     crawcin
                move.w  D0,format

                moveq   #'1',D0              ;Bitte Disk A einlegen
                bsr     insert

                cmpi.w  #'+',format
                bne.s   kein_format

                bsr     format_disk

kein_format:    bsr     wipe_disk

                lea     bootprogramm(PC),A6
                lea     boot_prg(PC),A5
                bsr     read_file

                moveq   #0,D7
                lea     bootsektor(PC),A0
                move.w  #254,D0
add_loop:       add.w   (A0)+,D7             ;Checksumme berechnen
                dbra    D0,add_loop

                move.w  #$1234,D1
                sub.w   D7,D1
                move.w  D1,(A0)              ;und eintragen

                move.w  #1,-(SP)             ;Count
                clr.w   -(SP)                ;Seite
                clr.w   -(SP)                ;Track
                move.w  #1,-(SP)             ;Sektor
                clr.w   -(SP)                ;A:
                clr.l   -(SP)
                pea     bootsektor(PC)
                move.w  #9,-(SP)
                trap    #14                  ;Flopwrite (Bootsektor)
                lea     20(SP),SP
                bmi     abbruch

                lea     loader(PC),A5
                lea     memory(PC),A6
                bsr     read_file
                tst.l   D0
                bmi     abbruch
                lea     loader_a(PC),A5
                bsr     write_file

                lea     medusa_1(PC),A5
                bsr     read_file
                tst.l   D0
                bpl.s   schreiben

                lea     medusa_2(PC),A5
                bsr     read_file
                tst.l   D0
                bmi     abbruch

schreiben:      lea     medusa_a(PC),A5
                bsr     write_file

                lea     rom_grfx1(PC),A5
                bsr     read_file
                tst.l   D0
                bmi     ende
                lea     rom_grfx1a(PC),A5
                bsr     write_file

                move.w  #'1',laufwerk
grafix_loop:    move.b  laufwerk+1(PC),rom_grfx_x
                move.b  laufwerk+1(PC),rom_grfx_ax
                lea     rom_grfx1(PC),A5
                lea     memory(PC),A6
                bsr.s   read_file
                move.l  D0,file_len
                tst.l   D0                   ;Konnte File gelesen werden?
                bmi.s   abbruch

                move.w  laufwerk(PC),D0
                addq.w  #1,D0
                bsr     insert

                cmpi.w  #'+',format
                bne.s   kein_format2

                bsr     format_disk

kein_format2:   bsr     wipe_disk

                move.b  laufwerk+1(PC),bootsektor+9 ;Bootsektor nicht ausfÅhrbar

                move.w  #1,-(SP)             ;Count
                clr.w   -(SP)                ;Seite
                clr.w   -(SP)                ;Track
                move.w  #1,-(SP)             ;Sektor
                clr.w   -(SP)                ;A:
                clr.l   -(SP)
                pea     bootsektor(PC)
                move.w  #9,-(SP)
                trap    #14                  ;Flopwrite (Bootsektor)
                lea     20(SP),SP
                tst.w   D0
                bmi.s   abbruch

                lea     memory(PC),A6
                move.l  file_len(PC),D0
                lea     rom_grfx1a(PC),A5
                bsr.s   write_file

                addq.w  #1,laufwerk
                bra.s   grafix_loop

ende:           clr.w   -(SP)
                trap    #1                   ;Alles Okay

abbruch:        lea     fatal_error(PC),A0
                bsr     printstring

                bsr     crawcin

                clr.w   -(SP)
                trap    #1


;List ein File
;-> A5.L: Adresse des Filenamens
;   A6.L: Ladeadresse
;<- D0.L: FilelÑnge
read_file:      lea     reading(PC),A0
                bsr     printstring

                movea.l A5,A0
                bsr     printstring

                clr.w   -(SP)
                move.l  A5,-(SP)
                move.w  #$003D,-(SP)
                trap    #1
                addq.l  #8,SP
                tst.w   D0
                bmi.s   file_error

                move.w  D0,-(SP)

                move.l  A6,-(SP)
                move.l  #$00DFDFDF,-(SP)
                move.w  D0,-(SP)
                move.w  #$003F,-(SP)
                trap    #1
                lea     12(SP),SP
                move.l  D0,D7
                bmi.s   file_error

                move.w  #$003E,-(SP)
                trap    #1
                addq.l  #4,SP

                lea     crlf(PC),A0
                bsr.s   printstring

                move.l  D7,D0                ;LÑnge zurÅckgeben
                rts

file_error:     lea     fileerror(PC),A0
                bsr.s   printstring
                moveq   #-1,D0               ;Error!
                rts

;Schreibt ein File
;-> A5.L: Adresse des Filenamens
;   A6.L: Ladeadresse
;   D0.L: FilelÑnge
write_file:     move.l  D0,D7                ;FilelÑnge merken

                lea     writing(PC),A0
                bsr.s   printstring

                movea.l A5,A0
                bsr.s   printstring

                clr.w   -(SP)
                move.l  A5,-(SP)
                move.w  #$003C,-(SP)
                trap    #1
                addq.l  #8,SP
                tst.w   D0
                bmi.s   file_error_fatal

                move.w  D0,-(SP)

                move.l  A6,-(SP)             ;Adresse
                move.l  D7,-(SP)             ;FilelÑnge
                move.w  D0,-(SP)             ;Fhandle
                move.w  #$0040,-(SP)
                trap    #1
                lea     12(SP),SP
                move.l  D0,D7
                bmi.s   file_error_fatal

                move.w  #$003E,-(SP)
                trap    #1
                addq.l  #4,SP
                tst.w   D0
                bmi.s   file_error_fatal

                lea     crlf(PC),A0
                bsr.s   printstring

                move.l  D7,D0                ;LÑnge zurÅckgeben
                rts

file_error_fatal:lea    fatal_error(PC),A0
                bsr.s   printstring
                bsr.s   crawcin
                moveq   #-1,D0
                rts

printstring:    move.l  A0,-(SP)
                move.w  #9,-(SP)
                trap    #1
                addq.l  #6,SP
                rts

insert:         move.b  D0,drive

                lea     insert_disk(PC),A0
                bsr.s   printstring

crawcin:        move.w  #1,-(SP)
                trap    #1                   ;Taste holen
                addq.l  #2,SP

                cmp.w   #27,D0               ;Escape?
                beq     ende                 ;->Abbruch
                rts


;Lîscht gesamte Disk
wipe_disk:      lea     format_memory(PC),A0
                move.w  #15000/4,D7
clear:          clr.l   (A0)+
                dbra    D7,clear

                move.w  #9,-(SP)             ;Count
                clr.w   -(SP)                ;Seite
                clr.w   -(SP)                ;Track
                move.w  #2,-(SP)             ;Sektor
                clr.w   -(SP)                ;A:
                clr.l   -(SP)
                pea     format_memory(PC)
                move.w  #9,-(SP)
                trap    #14                  ;Flopwrite (Bootsektor)
                lea     20(SP),SP
                tst.w   D0
                bmi.s   file_error_fatal
                rts

format_disk:    moveq   #0,D7
                moveq   #0,D6
format_loop:    move.l  D7,D0
                divu    #10,D0
                add.w   #'0',D0
                move.b  D0,track+8
                swap    D0
                add.w   #'0',D0
                move.b  D0,track+9

                lea     track(PC),A0
                bsr     printstring

                lea     spiral_tab(PC),A0
                move.w  D6,D0
                moveq   #9,D5                ;10 Sektoren pro Track
spiral:         move.w  D0,(A0)
                addq.w  #1,(A0)+             ;Sektoren starten bei Eins
                addq.w  #1,D0                ;Spiralisierung
                ext.l   D0
                divu    #10,D0
                swap    D0
                dbra    D5,spiral

                addq.w  #7,D6                ;Spiralfaktor
                ext.l   D6
                divu    #10,D6
                swap    D6

                clr.w   -(SP)                ;Virgin pattern
                move.l  #$87654321,-(SP)     ;Magic
                move.w  #-1,-(SP)            ;Interleave=Spiralisierung
                clr.w   -(SP)                ;Side
                move.w  D7,-(SP)             ;Track
                move.w  #10,-(SP)            ;SpT
                clr.w   -(SP)                ;A:
                pea     spiral_tab(PC)
                pea     format_memory(PC)
                move.w  #$000A,-(SP)
                trap    #14                  ;Format Track
                lea     $001A(SP),SP
                tst.w   D0
                bmi     file_error_fatal

                addq.w  #1,D7
                cmp.w   #79,D7
                ble.s   format_loop

                lea     crlf(PC),A0
                bra     printstring

                DATA
boot_prg:       DC.B 'STBOOT.B',0
medusa_1:       DC.B 'O:\MEDUSA.PRG',0
medusa_2:       DC.B 'E:\MEDUSA\MEDUSA_2.COL\C\MEDUSA.PRG',0
medusa_a:       DC.B 'A:\MEDUSA.PRG',0
loader:         DC.B 'LOADER.ABS',0
loader_a:       DC.B "A:\LOADER.ABS",0
rom_grfx1:      DC.B 'D:\MEDUSA.DAT\MEDUSA_2.COL\ROM_GRFX.'
rom_grfx_x:     DC.B '0',0
rom_grfx1a:     DC.B 'A:\ROM_GRFX.'
rom_grfx_ax:    DC.B '0',0

titel:          DC.B 27,"E"
                DC.B 'Installierprogramm fÅr Rings of Medusa II - Atari ST',13,10
                DC.B 'Ω 1991 by Till Bubeck, Ziegeleistr. 28, 7056 Weinstadt',13,10,10
                DC.B "Zum Formatieren der Disketten bitte '+' drÅcken: ",0

insert_disk:    DC.B 13,10,10,"Bitte Disk "
drive:          DC.B 'X einlegen (Return).',13,10,0

track:          DC.B 13,'Track: 00',0

reading:        DC.B 13,10,'Reading: ',0
writing:        DC.B 'Writing: ',0
fatal_error:    DC.B ' <- Fatal Error! (Return)',13,10,0
fileerror:      DC.B " <- Error!"
crlf:           DC.B 13,10,0

                EVEN
bootsektor:     bra.s   bootprogramm
                DC.B "RoM #2 A:"
                DC.B 0,2,2,1,0,2,$50,0,$20
                DC.B 3,$F8,2,0,10,0,1,0,0,0
bootprogramm:   DS.B 512

                BSS
file_len:       DS.L 1          ;LÑnge des aktuellen Archivs
laufwerk:       DS.W 1
format:         DS.W 1
spiral_tab:     DS.W 20         ;Zum Spiralisieren der Disketten

format_memory:  DS.B 20000      ;zum Formatieren ein Buffer
memory:         DS.L 65000      ;520 Kbyte
                DS.L 65000

                END
