                TEXT
Begin:
                movea.l 4(SP),A6
                movea.w #$0500,A5
                adda.l  12(A6),A5
                adda.l  20(A6),A5
                adda.l  28(A6),A5
                move.l  A5,D1
                and.w   #-2,D1
                add.l   A6,D1
                movea.l D1,SP
                move.l  A5,-(SP)
                move.l  A6,-(SP)
                move.l  #$004A0000,-(SP)
                trap    #1
                lea     12(SP),SP

                lea     memory(PC),A6
                moveq   #0,D7
L0000:          bsr     L0001
                addq.w  #1,D7
                tst.w   D0
                bpl.s   L0000

                pea     speichern
                move.w  #9,-(SP)
                trap    #1
                addq.l  #6,SP

                clr.w   -(SP)
                pea     L0002(PC)
                move.w  #$003C,-(SP)         ;FCREATE
                trap    #1
                addq.l  #8,SP
                pea     memory(PC)
                suba.l  #memory,A6           ;Dateil„nge berechnen
                move.l  A6,-(SP)             ;->Dateil„nge
                move.w  D0,-(SP)
                move.w  #$0040,-(SP)         ;FWRITE
                trap    #1
                subq.w  #2,(SP)
                trap    #1
                lea     12(SP),SP
                clr.w   -(SP)                ;PTERM0
                trap    #1
L0001:          move.b  #$41,D0              ;#'A'
                add.b   D7,D0
                move.b  D0,L0004

                pea     reading
                move.w  #9,-(SP)
                trap    #1
                addq.l  #6,SP

                clr.w   -(SP)
                pea     L0003(PC)
                move.w  #$003D,-(SP)         ;FOPEN
                trap    #1
                addq.l  #8,SP
                tst.w   D0
                bmi.s   abbruch
                move.l  A6,-(SP)
                move.l  #$00DFDFDF,-(SP)
                move.w  D0,-(SP)
                move.w  #$003F,-(SP)         ;FREAD
                trap    #1
                adda.l  D0,A6
                subq.w  #1,(SP)
                trap    #1
                lea     12(SP),SP

                move.w  #13,-(SP)            ;CR/LF
                move.w  #2,-(SP)
                trap    #1
                addq.l  #4,SP
                move.w  #10,-(SP)
                move.w  #2,-(SP)
                trap    #1
                addq.l  #4,SP

                moveq   #0,D0
abbruch:        rts

                DATA
speichern:      DC.B " <- Error",13,10,10
                DC.B "Saving: "
L0002:          DC.B "GMAPS.DAT",0
reading:        DC.B "Reading: "
L0003:          DC.B "GMAP"
L0004:          DC.B "X.DAT",0
                BSS
file_len:       DS.L 1
memory:         DS.W 50000
ZUEND:          END
