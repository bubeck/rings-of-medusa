******************************************************************************
***                                                                        ***
***                öberprÅft die Medusa-Disketten                          ***
***                                                                        ***
***                                                                        ***
***   Ω1990 by Till Bubeck, Ziegeleistr. 28, 7056 Weinstadt, 07151/66437   ***
***                                                                        ***
******************************************************************************

                OPT X+

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

                pea     message(PC)
                move.w  #9,-(SP)
                trap    #1
                addq.l  #6,SP

                move.w  #7,-(SP)
                trap    #1
                addq.l  #2,SP

                moveq   #0,D7
                moveq   #0,D6
trackloop:      move.w  #10,-(SP)
                clr.w   -(SP)           ;Seite 0
                move.w  D7,-(SP)        ;Track
                move.w  #1,-(SP)        ;Sektor
                clr.w   -(SP)           ;A:
                clr.l   -(SP)
                pea     buffer(PC)
                move.w  #8,-(SP)
                trap    #14
                lea     20(SP),SP
                tst.w   D0
                bmi.s   fatal

                lea     buffer(PC),A0
                move.w  #512*10/2-1,D0
check:          move.w  (A0)+,D1
                eor.w   D1,D6
                dbra    D0,check

                addq.w  #1,D7
                cmp.w   #80,D7
                blt.s   trackloop

                pea     ready(PC)
                move.w  #9,-(SP)
                trap    #1
                addq.l  #6,SP

                move.w  D6,D0
                bsr.s   int_out

                pea     hex_string
                move.w  #9,-(SP)
                trap    #1
                addq.l  #6,SP

ende:           move.w  #7,-(SP)
                trap    #1
                addq.l  #2,SP

                clr.w   -(SP)
                trap    #1

fatal:          pea     fatal_error(PC)
                move.w  #9,-(SP)
                trap    #1
                addq.l  #6,SP

                bra.s   ende

int_out:        moveq   #3,D7
                lea     hex_tab(PC),A0
                lea     hex_string(PC),A1
hexloop:        rol.w   #4,D0
                move.w  D0,D1
                and.w   #15,D1
                move.b  0(A0,D1.w),(A1)+
                dbra    D7,hexloop
                rts

hex_tab:        DC.B '0123456789ABCDEF'
message:        DC.B 27,"E"
                DC.B 'DiskettenprÅfprogramm Rings of Medusa 2 - Atari',13,10
                DC.B 'Ω 1991 by Till Bubeck, Weinstadt',13,10,10

                DC.B 'Bitte zu ÅberprÅfende Diskette einlegen:',0

fatal_error:    DC.B 13,10,10,"Fataler Lesefehler -> Disk defekt...."

ready:          DC.B 13,10,10
                DC.B 'PrÅfsumme: $',0

                BSS
hex_string:     DS.B 100
buffer:         DS.B 10*512
                END
