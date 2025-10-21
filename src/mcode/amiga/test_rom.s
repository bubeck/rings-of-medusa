
                ORG $00030000
                DEFAULT 9

                move.w  #%0111111111111111,$00DFF09A ;Alle IRQs aus
                move.w  #%0111111111111111,$00DFF09C ;Keine Requests mehr
                move.w  #%0111111111111111,$00DFF096 ;Alle DMA's aus
                move.w  #%1000001000000000,$00DFF096 ;Gesamt DMA an

                move.b  #240,$00BFE601       ;TBLO
                move.b  #55,$00BFE701        ;TBHI

                move.b  $00BFEF01,D0         ;CrB
                and.b   #%10010101,D0        ;Inmode=00 -> Z„hlt Taktzyklen
                or.b    #%00000001,D0        ;Start setzen
                move.b  D0,$00BFEF01         ;Und Timer starten

                move.b  #%10000010,$00BFED01 ;ICR, Timer B kann IRQ ausl”sen

                move    #$2000,SR

                moveq   #2,D0                ;lied nr 1
                lea     music+$0F90,A0       ;zeiger auf songfile
                jsr     music

                moveq   #0,D0                ;volume setzen          (000 = laut)
                jsr     music+8              ;volle lautst„rke       (100 = leise)

                jsr     music+12             ;sfx initialisieren

                move.l  #level2,$00000068.w
                move.w  #%1100000000001000,$00DFF09A ;CIA-A IRQ erlauben

                move.w  #$0FFF,$00DFF180

                moveq   #30,D1
                moveq   #-2,D0
wa:             dbra    D0,wa
                dbra    D1,wa

                clr.w   $00DFF180

                moveq   #1,D0                ;sfx nummer 7
                jsr     music+16

waitmouse:      btst    #6,$00BFE001
                bne.s   waitmouse

                move.w  #$000F,$00DFF096
                move.w  #$7FFF,$00DFF09A
                illegal


level3:         jsr     music+4
                move.w  #%0000000000100000,$00DFF09C
                rte

level2:         move.l  D0,-(SP)
                jsr     music+4
                move.b  $00BFED01,D0
                move.w  #%0000000000001000,$00DFF09C ;Request l”schen
                move.l  (SP)+,D0
                rte

ct:             DS.W 1

music:          PATH 'E:\MEDUSA\MEDUSA_2.COL\'
                PATH 'MCODE\AMIGA\'
                IBYTES 'ROM_II_G.IMG'

                EVEN

                END
