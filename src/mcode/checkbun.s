******************************************************************************
***                                                                        ***
***              Bunkerchecker V1.0 fÅr Rings of Medusa II                 ***
***                                                                        ***
***                                                                        ***
***   Ω1991 by Till Bubeck, Ziegeleistr. 28, 7056 Weinstadt, 07151/66437   ***
***                                                                        ***
******************************************************************************

disk_version    EQU 1           ;0=Disk_version , 1=Assemblerversion

max_tuer        EQU 1000
bunker_zahl     EQU 50

DOOR_ZU_START   EQU 32768
DOOR_ZU_ENDE    EQU 35999
DOOR_AUF_START  EQU 36000
DOOR_AUF_ENDE   EQU 38000
KEY_START       EQU 1
KEY_ENDE        EQU 2000

                OPT X+
                DEFAULT 1
                PATH 'D:\MEDUSA\MEDUSA_2.COL\'
                PATH 'BUNKER\MAPS\'
                OUTPUT 'CHECKBUN.TTP'

                movea.l 4(SP),A6        ;Basepageadresse holen
                movea.w #$0100+$0400,A5 ;Grîûe der Basepage + Stackgrîûe (1k)
                adda.l  12(A6),A5       ;+ Grîûe des TEXT-Segments
                adda.l  20(A6),A5       ;+ Grîûe des DATA-Segments
                adda.l  28(A6),A5       ;+ Grîûe des BSS-Segments
                move.l  A5,D1           ;= GesamtlÑnge des Programms
                and.w   #$FFFE,D1       ;LÑnge nun gerade
                add.l   A6,D1           ;+ Programmstart (Basepageadresse)
                movea.l D1,SP           ;Stack endet dort
                move.l  A5,-(SP)        ;ProgrammlÑnge
                move.l  A6,-(SP)        ;Adresse der Basepage
                move.l  #$4A0000,-(SP)  ;Funktionsnummer + Dummyword (0)
                trap    #1              ;Mshrink(0,Basepageadr,PrglÑnge)
                lea     12(SP),SP       ;Nur noch den Stack korrigieren

                IF disk_version=0

                lea     $80(A6),A6      ;Start der Basepage
                moveq   #0,D7
                move.b  (A6)+,D7        ;Anzahl holen
                bne.s   hol_them        ;Es ist was da!

                pea     error(PC)
                move.w  #9,-(SP)
                trap    #1
                addq.l  #6,SP

                move.w  #7,-(SP)
                trap    #1
                addq.l  #2,SP
                bra     check_ende

hol_them:       clr.b   0(A6,D7.w)      ;Null ans Ende
                move.l  A6,protokoll

                ENDC

                lea     bunker_start(PC),A4
                lea     bunker_len(PC),A5
                lea     memory,A6       ;Hier hin lesen
                moveq   #1,D7           ;Mit Bunker 1 starten

lesen_loop:     clr.l   (A4)+
                clr.l   (A5)+

                lea     f_nr(PC),A1
                move.w  D7,D0
                ext.l   D0
                divu    #10,D0
                tst.w   D0
                beq.s   keine_null
                add.w   #'0',D0
                move.b  D0,(A1)+
keine_null:     swap    D0
                add.w   #'0',D0
                move.b  D0,(A1)+
                move.b  #'.',(A1)+
                move.b  #'D',(A1)+
                move.b  #'A',(A1)+
                move.b  #'T',(A1)+
                clr.b   (A1)

                clr.w   -(SP)
                pea     filename(PC)
                move.w  #$3D,-(SP)
                trap    #1              ;Fopen
                addq.l  #8,SP
                tst.w   D0
                bmi.s   nxt_bunker

                move.w  D0,-(SP)

                move.l  A6,-(SP)
                move.l  #$DFDFDF,-(SP)
                move.w  D0,-(SP)
                move.w  #$3F,-(SP)      ;Fread
                trap    #1
                lea     12(SP),SP

                move.l  A6,-4(A4)       ;Adresse merken
                move.l  D0,-4(A5)       ;und LÑnge merken
                adda.l  D0,A6

                move.w  #$3E,-(SP)
                trap    #1              ;Fclose
                addq.l  #4,SP

nxt_bunker:     addq.w  #1,D7
                cmp.w   #bunker_zahl,D7
                ble.s   lesen_loop

                lea     tÅren(PC),A0
                move.w  #(2*max_tuer)-1,D7
clr:            move.b  #-1,(A0)+       ;Alle TÅren und SchlÅssel unbenutzt
                dbra    D7,clr

                clr.w   -(SP)
                move.l  protokoll(PC),-(SP)
                move.w  #$3C,-(SP)
                trap    #1
                addq.l  #8,SP
                move.w  D0,fhandle

                lea     titel(PC),A4
                bsr     write_string

                lea     tÅren(PC),A1
                lea     schlÅssel(PC),A2
                lea     bunker_start(PC),A5
                lea     bunker_len(PC),A6
                lea     stat,A3         ;Hier Statistik aufbauen
                moveq   #1,D7           ;Bunker #0
check_bunker:   movea.l (A5)+,A0        ;Startadresse
                move.l  (A6)+,D6        ;LÑnge
                beq.s   nxt_bunker2
                lsr.l   #1,D6           ;Anzahl Worte berechnen
                subq.l  #2,D6           ;2 Worte Åbersprungen
                addq.l  #4,A0           ;Grîûe Åberspringen
                move.w  D6,(A3)         ;GesamtlÑnge eintragen

check_loop:     move.w  (A0)+,D0        ;Wert holen
                bne.s   kein_leerfeld
                addq.w  #1,2(A3)        ;ein Feld mehr
kein_leerfeld:  cmp.w   #DOOR_ZU_START,D0 ;TÅr Nummer 1 nicht prÅfen
                beq.s   nxt_wert
                bcs.s   keine_tÅr1      ;blt
                cmp.w   #DOOR_ZU_ENDE,D0
                bhi.s   keine_tÅr1      ;bgt

                sub.w   #DOOR_ZU_START,D0 ;TÅrnummer berechnen
check_tÅr:      tst.b   1(A1,D0.w)      ;TÅr schon vorhanden?
                bpl     tÅr_doppelt
                move.b  D7,1(A1,D0.w)   ;Bunkernummer eintragen
                bra.s   nxt_wert

keine_tÅr1:     cmp.w   #DOOR_AUF_START,D0
                beq.s   nxt_wert
                bcs.s   keine_tÅr2
                cmp.w   #DOOR_AUF_ENDE,D0
                bhi.s   keine_tÅr2

                sub.w   #DOOR_AUF_START,D0
                bra.s   check_tÅr

keine_tÅr2:     cmp.w   #KEY_START,D0   ;SchlÅssel 1 nicht berÅcksichtigen
                beq.s   nxt_wert
                bcs.s   keine_key
                cmp.w   #KEY_ENDE,D0
                bhi.s   keine_key

                subq.w  #KEY_START,D0
                tst.b   1(A2,D0.w)      ;Key schon vorhanden?
                bpl     key_doppelt
                move.b  D7,1(A2,D0.w)   ;keynummer eintragen
                tst.b   schlÅssel+131
                bra.s   nxt_wert

keine_key:
nxt_wert:       subq.w  #1,D6
                cmp.w   #1,D6
                bge.s   check_loop

nxt_bunker2:    addq.l  #4,A3           ;Statistik weiter
                addq.w  #1,D7           ;nÑchster Bunker
                cmp.w   #bunker_zahl,D7
                ble     check_bunker

save:           clr.w   -(SP)
                pea     statistik
                move.w  #$3C,-(SP)
                trap    #1
                addq.l  #8,SP
                move.w  D0,-(SP)

                pea     stat
                suba.l  #stat,A3
                move.l  A3,-(SP)        ;LÑnge
                move.w  D0,-(SP)
                move.w  #$40,-(SP)
                trap    #1
                lea     12(SP),SP

                move.w  #$3E,-(SP)
                trap    #1
                addq.l  #4,SP

                lea     tÅren+max_tuer(PC),A3
such_ende:      tst.b   -(A3)
                bmi.s   such_ende

                moveq   #0,D7
                lea     tÅren(PC),A1
                lea     schlÅssel(PC),A2
check_lost_key: tst.b   (A1)            ;TÅr vorhanden?
                bmi.s   tÅr_fehlt
                tst.b   (A2)            ;zugehîriger SchlÅssel da?
                bpl.s   nxt_tÅr         ;ja->Weitermachen

                move.w  D7,D0           ;SchlÅsselnummer
                lea     key_m1(PC),A4
                bsr     itoa
                lea     bun12(PC),A4
                move.b  (A1),D0         ;SchlÅsselnummer
                bsr     itoa

                lea     key_m(PC),A4
                bsr.s   write_string

nxt_tÅr:        addq.l  #1,A1
                addq.l  #1,A2
                addq.w  #1,D7           ;nÑchste TÅr
                cmpa.l  A3,A1           ;Ende der TÅren erreicht?
                ble.s   check_lost_key

                lea     eof(PC),A4
                bsr.s   write_string

                move.w  fhandle(PC),-(SP)
                move.w  #$3E,-(SP)
                trap    #1
                addq.l  #6,SP

check_ende:     clr.w   -(SP)
                trap    #1

tÅr_fehlt:      lea     tÅr_m1(PC),A4
                move.w  D7,D0
                bsr.s   itoa

                lea     tÅr_m(PC),A4
                bsr.s   write_string

                bra.s   nxt_tÅr

tÅr_doppelt:    addq.w  #1,D0
                lea     tÅr1(PC),A4
                bsr.s   itoa
                lea     bun1(PC),A4
                subq.w  #1,D0
                move.b  1(A1,D0.w),D0   ;Bunker1
                bsr.s   itoa
                lea     bun2(PC),A4
                move.b  D7,D0           ;Bunker 2
                bsr.s   itoa

                lea     tÅr_dop(PC),A4
                bsr.s   write_string

                bra     nxt_wert

key_doppelt:    lea     key1(PC),A4
                addq.w  #1,D0
                bsr.s   itoa
                lea     bun11(PC),A4
                subq.w  #1,D0
                move.b  1(A2,D0.w),D0   ;Bunker1
                bsr.s   itoa
                lea     bun21(PC),A4
                move.b  D7,D0           ;Bunker 2
                bsr.s   itoa

                lea     key_dop(PC),A4
                bsr.s   write_string

                bra     nxt_wert

write_string:   movem.l D0-A6,-(SP)

write_loop:     tst.b   (A4)
                beq.s   write_end

                move.l  A4,-(SP)
                move.l  #1,-(SP)
                move.w  fhandle(PC),-(SP)
                move.w  #$40,-(SP)
                trap    #1
                lea     12(SP),SP

                addq.l  #1,A4
                bra.s   write_loop

write_end:      movem.l (SP)+,D0-A6
                rts

;-> D0.W: Nummer
;   A4.L: Adresse
itoa:           movem.l D0-A6,-(SP)

                addq.l  #4,A4
                moveq   #3,D7
itoa_loop:      ext.l   D0
                divu    #10,D0
                swap    D0
                add.w   #'0',D0
                move.b  D0,-(A4)
                swap    D0
                dbra    D7,itoa_loop

                movem.l (SP)+,D0-A6
                rts


                DATA
protokoll:      DC.L output

output:         DC.B 'CON:',0

error:          DC.B 'Bunkerchecker Usage: outputfile',0
statistik:      DC.B 'BUNKER.DAT',0 ;Statistik des Bunkers hier hinein

filename:       DC.B 'BUNKER'   ;gesuchte Files
f_nr:           DC.B 'xx.dat',0

titel:          DC.B 13,10,13,10
                DC.B 'Fehler in den Medusa II Bunkern: ',13,10,13,10,0

tÅr_dop:        DC.B 'TÅr '
tÅr1:           DC.B 'xxxx doppelt in Bunker '
bun1:           DC.B 'xxxx und '
bun2:           DC.B 'xxxx.',13,10,0
key_dop:        DC.B 'SchlÅssel '
key1:           DC.B 'xxxx doppelt in Bunker '
bun11:          DC.B 'xxxx und '
bun21:          DC.B 'xxxx.',13,10,0
key_m:          DC.B 'SchlÅssel fÅr TÅr '
key_m1:         DC.B 'xxxx in Bunker '
bun12:          DC.B 'xxxx fehlt.',13,10,0
tÅr_m:          DC.B 'TÅr '
tÅr_m1:         DC.B 'xxxx nicht verwendet.',13,10,0
eof:            DC.B 13,10,'- EOF -',0

                BSS

fhandle:        DS.W 1

tÅren:          DS.B max_tuer   ;(-1=Unbenutzt, Sonst Bunkernummer)
schlÅssel:      DS.B max_tuer   ;(-1=Unbenutzt; Sonst Bunkernummer)

bunker_start:   DS.L 60         ;Platz fÅr 60 Bunker
bunker_len:     DS.L 60         ;-'-

stat:           DS.B 60000      ;Hier Statistik aufbauen

memory:         DS.L 65000
                END
