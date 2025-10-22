;************************************************************************
;* Send-Prg - Sendet Medusa.prg aus dem aktuellen Pfad zum Amiga
;* (C)1989 by Σ-soft, written by Markus Fritze           18.06.1988 01:30 *
;************************************************************************

ladeadresse     EQU $00065000   ;Adresse 'LOADER.ABS' im Amiga

                movea.l 4(SP),A6             ;Basepageadresse holen
                movea.w #$0100+$0400,A5      ;Größe der Basepage + Stackgröße (1k)
                adda.l  12(A6),A5            ;+ Größe des TEXT-Segments
                adda.l  20(A6),A5            ;+ Größe des DATA-Segments
                adda.l  28(A6),A5            ;+ Größe des BSS-Segments
                move.l  A5,D1                ;= Gesamtlänge des Programms
                and.w   #$FFFE,D1            ;Länge nun gerade
                add.l   A6,D1                ;+ Programmstart (Basepageadresse)
                movea.l D1,SP                ;Stack endet dort
                move.l  A5,-(SP)             ;Programmlänge
                move.l  A6,-(SP)             ;Adresse der Basepage
                move.l  #$004A0000,-(SP)     ;Funktionsnummer + Dummyword (0)
                trap    #1                   ;Mshrink(0,Basepageadr,Prglänge)
                lea     12(SP),SP            ;Nur noch den Stack korrigieren

                clr.w   -(SP)
                pea     loader(PC)
                move.w  #$003D,-(SP)
                trap    #1
                addq.l  #8,SP
                move.w  D0,fhandle
                bmi.s   nicht_da

                pea     memstart(PC)
                move.l  #$00DFDFDF,-(SP)
                move.w  fhandle(PC),-(SP)
                move.w  #$003F,-(SP)
                trap    #1
                lea     12(SP),SP
                move.l  D0,loader_len

                move.w  fhandle(PC),-(SP)
                move.w  #$003E,-(SP)
                trap    #1
                addq.l  #4,SP

                clr.w   memstart+2           ;Amigatool ist vorhanden

                lea     memstart(PC),A0      ;ab hier senden
                movea.l #ladeadresse,A1      ;Amiga-Adresse
                move.l  loader_len(PC),D1
                moveq   #1,D0                ;Put Data
                trap    #8

                lea     memstart(PC),A0
                movea.l #ladeadresse,A1
                moveq   #3,D0
                trap    #8                   ;Execute

nicht_da:       clr.w   -(SP)
                trap    #1

                DATA
loader:         DC.B 'E:\MEDUSA\MEDUSA_2.COL\MCODE\AMIGA\LOADER.ABS',0

                BSS

fhandle:        DS.W 1
loader_len:     DS.L 1

memstart:       DS.L 60000      ;240 Kbyte
                DS.L 60000      ;240 Kbyte
                END
