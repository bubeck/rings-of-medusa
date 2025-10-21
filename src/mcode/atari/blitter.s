
;Blitteransteuerung

;Dies ist ein kommentierter Auszug aus TOS 1.4
;Es stellt die Auswertung der Eingabewerte fr Line_A #7 BitBlt dar.

;½ 1991 by Till Bubeck, Ziegeleistr. 28, 7056 Weinstadt

                OPT X+
                DEFAULT 1


                TEXT
Begin:          adda.w  #76,A6               ;Ans Ende des Parameterblocks gehen
                move.w  -62(A6),D0           ;Source X1
                move.w  -60(A6),D5           ;Source Y1
                move.w  -48(A6),D2           ;Destination X1
                move.w  -46(A6),D7           ;Destination Y1
                move.w  D0,D4
                move.w  D2,D6
                move.w  -74(A6),D1           ;H”he in Pixeln
                subq.w  #1,D1
                add.w   D1,D5                ;Source Y2
                add.w   D1,D7                ;Destination Y2
                move.w  -76(A6),D1           ;Breite in Pixeln
                subq.w  #1,D1
                add.w   D1,D4                ;Source X2
                add.w   D1,D6                ;Destination X2
                move.w  D4,-18(A6)
                move.w  D5,-16(A6)           ;Source Y2
                move.w  D6,-10(A6)
                move.w  D7,-8(A6)            ;Destination Y2
                bra     blitter

                DC.B $4A,$49,$4D,$20,$4C,$4F,$56,$45
                DC.B $53,$20,$4A,$45,$4E,$45,$41,$4E
                DC.B $45,$00

L0000:          DC.W %0000000000000000
L0001:          DC.W %1000000000000000,%1100000000000000,%1110000000000000,%1111000000000000
                DC.W %1111100000000000,%1111110000000000,%1111111000000000,%1111111100000000
                DC.W %1111111110000000,%1111111111000000,%1111111111100000,%1111111111110000
                DC.W %1111111111111000,%1111111111111100,%1111111111111110,%1111111111111111

blitter:        lea     $00FF8A3C,A5         ;Blitter Opcoderegister
                moveq   #$0F,D1
                move.w  D2,D5                ;Destination X1
                and.w   D1,D5
                move.w  D5,D7                ;merken
                add.w   D5,D5
                move.w  L0000(PC,D5.w),D3    ;Maske holen
                not.w   D3                   ;und negieren
                swap    D3
                move.w  D6,D5                ;Destination X2
                and.w   D1,D5
                add.w   D5,D5
                move.w  L0001(PC,D5.w),D3    ;Maske holen
                move.w  D0,D5                ;Source X1
                and.w   D1,D5
                sub.w   D5,D7                ;Skew berechnen
                lsr.w   #4,D0                ;Source X1 BB berechnen
                lsr.w   #4,D4                ;Source X2 -'-
                lsr.w   #4,D2                ;Destination X1 -'-
                lsr.w   #4,D6                ;Destination X2 -'-
                move.w  D4,D1                ;Source X2 BB
                sub.w   D0,D4                ;Breite in BBs-1
                muls    -54(A6),D1           ;Offset zum n„chsten Word dergleichen Plane (Source)
                move.w  -16(A6),D5           ;Source Y2
                muls    -52(A6),D5           ;Breite Quelle in Bytes
                add.l   D1,D5                ;Offset im Screen
                movea.l -58(A6),A2           ;Anfangsadresse Quelle
                lea     0(A2,D5.l),A0        ;+Offset=Ende der Quelle
                move.w  D6,D1                ;Destination X2 merken
                sub.w   D2,D6                ;Dest X2-Dest X1=Breite im Ziel
                bgt.s   L0002                ;Mehr als 1 Wort zu kopieren?
                move.l  D3,D5                ;Maske
                swap    D5                   ;vertauschen
                and.l   D5,D3                ;und nur noch eine Maske
L0002:          muls    -40(A6),D1           ;Offset zum n„chsten Wort dergleichen Plane (Dest)
                move.w  -8(A6),D5            ;Destination Y2
                move.w  D5,-24(A6)           ;merken
                muls    -38(A6),D5           ;Breite des Zielrasters in Bytes
                add.l   D1,D5                ;=Offset
                movea.l -44(A6),A3           ;Anfangsadresse Zielraster
                lea     0(A3,D5.l),A1        ;+Offset=Ende des Zielbereichs

                moveq   #0,D1                ;Status=Descending Modus
                cmpa.l  A1,A0                ;Ascending oder Descending Mode?
                bgt.s   L0003
                bne.s   L0004                ;Sind beide etwa gleich?
                tst.w   D7                   ;Skew positiv?
                bge.s   L0004
L0003:          move.w  D0,D1                ;Source X1
                muls    -54(A6),D1           ;Offset zum n„chsten Wort dergleichen Plane
                move.w  -60(A6),D5           ;Source Y1
                muls    -52(A6),D5           ;Breite Quelle in Bytes
                add.l   D1,D5                ;=Offset
                lea     0(A2,D5.l),A0        ;=Startadresse des Quellrasters
                move.w  D2,D1                ;Destination X1 BB
                muls    -40(A6),D1
                move.w  -46(A6),D5
                move.w  D5,-24(A6)
                muls    -38(A6),D5
                add.l   D1,D5
                lea     0(A3,D5.l),A1        ;=Startadresse des Zielrasters
                moveq   #8,D1                ;Status=Ascending Mode
                swap    D3                   ;Maske umdrehen
L0004:          move.w  #-1,-18(A5)          ;mittlere Maske
                move.w  D3,-20(A5)           ;Masken setzen
                swap    D3
                move.w  D3,-16(A5)           ;andere Maske auch setzen

                move.w  D6,D0                ;Breite im Ziel BB's-1
                addq.w  #1,D0                ;Breite in BBs
                move.w  D0,-6(A5)            ;Breite in BBs setzen
                tst.w   D7                   ;Skew positiv? (nach rechts schieben?)
                bge.s   L0005
                addq.w  #2,D1                ;im Status merken
L0005:          cmp.w   D4,D6                ;Breite Quelle=Breite Ziel?
                bne.s   L0006
                addq.w  #4,D1                ;im Status merken
L0006:          move.w  -54(A6),D2           ;Offset zum n„chsten Wort dergleichen Plane (Source)
                move.w  -40(A6),D3           ;Offset zum n„chsten Wort dergleichen Plane (Dest)
                move.w  D4,D0                ;Breite Quelle in BBs
                muls    D2,D4                ;mal Offsets
                neg.w   D4
                add.w   -52(A6),D4           ;Breite der Quelle in Bytes=Modulo Wert Quelle
                add.w   D6,D0                ;Breite Ziel in BBs
                muls    D3,D6
                neg.w   D6
                add.w   -38(A6),D6           ;=Modulo Wert im Ziel

                btst    #3,D1                ;Descending Mode?
                bne.s   L0007
                neg.w   D2
                neg.w   D3                   ;Dann alles umdrehen
                neg.w   D4
                neg.w   D6
L0007:          move.w  D6,-12(A5)           ;Modulo Ziel setzen
                move.w  D4,-26(A5)           ;Modulo Quelle setzen
                tst.w   D0                   ;wenigstens 1 BB zu transferieren?
                bne.s   L0008
                move.w  D7,D2
                add.w   #$0010,D1            ;Nein, im Status merken
L0008:          and.w   #$000F,D7            ;Skew
                or.w    skew_tabelle(PC,D1.w),D7 ;je nach Status den Skew umrechnen
                move.w  D3,-14(A5)
                move.w  D2,-28(A5)           ;Offset zum n„chsten Wort dergleichen Plane (Source)
                bne.s   L000A
                bset    #7,D7                ;im Skew das Vorzeichen setzen
                bra.s   L000A

skew_tabelle:   or.w    D0,D0
                or.l    D0,D0
                divu    D0,D0
                or.b    D0,D0
                or.w    D0,D0
                or.l    D0,D0
                or.b    D0,D0
                divu    D0,D0
                or.b    D0,D0
                or.b    D0,D0
                or.b    D0,D0
                or.b    D0,D0
                or.b    D0,D0
                or.b    D0,D0
                or.b    D0,D0
                or.b    D0,D0

L000A:          movea.w -50(A6),A2           ;Offset zur n„chsten Plane (Source)
                movea.w -36(A6),A3           ;Offset zur n„chsten Plane (Dest)
                move.w  -68(A6),D2           ;Hintergrundfarbe
                move.w  -70(A6),D3           ;Vordergrundfarbe
                move.w  -74(A6),D4           ;H”he in Pixeln
                move.w  -72(A6),D5           ;Anzahl der Farbplanes
                move.w  #$0200,-2(A5)        ;HOP-Register (Nur Source-Daten)
                movea.l -34(A6),A4           ;Zeiger auf 16-Bit-Masken
                move.l  A4,D6
                beq     naechste_plane       ;sind welche vorhanden?
                ori.w   #$0100,-2(A5)
                move.w  -30(A6),D0
                subq.w  #2,D0
                bne.s   L0010
                move.w  -26(A6),D0
                and.w   #-2,D0
                cmp.w   #$001E,D0
                bne.s   L0010
                move.w  -24(A6),D0           ;Masken Berechnung
                and.w   #$000F,D0
                rol.w   #8,D0
                or.w    D0,D7
                move.w  -28(A6),D0
                bne.s   L000D
                moveq   #0,D6
                bra.s   L000E
L000B:          move.l  A0,-24(A5)           ;Source-Adresse
                move.l  A1,-10(A5)           ;Destination-Adress
                move.w  D4,-4(A5)            ;Y-Count
                clr.w   D6
                lsr.w   #1,D3                ;Vordergrundfarbe
                addx.w  D6,D6
                lsr.w   #1,D2                ;Hintergrundfarbe
                addx.w  D6,D6
                move.b  -66(A6,D6.w),-1(A5)  ;log.Verknpfung
                move.w  D7,(A5)              ;Transfer starten
                adda.w  A2,A0
                adda.w  A3,A1                ;und auf n„chste Plane gehen
L000C:          tas.b   (A5)                 ;Blitter fertig?
                nop
                bmi.s   L000C
L000D:          move.l  A4,D6                ;Masken vorhanden?
                beq.s   naechste_plane       ;Nein->Weitermachen
L000E:          lea     -60(A5),A5
                move.l  (A4)+,(A5)+
                move.l  (A4)+,(A5)+
                move.l  (A4)+,(A5)+
                move.l  (A4)+,(A5)+
                move.l  (A4)+,(A5)+
                move.l  (A4)+,(A5)+
                move.l  (A4)+,(A5)+
                move.l  (A4)+,(A5)+
                lea     28(A5),A5
                movea.l D6,A4
                adda.w  D0,A4
naechste_plane: dbra    D5,L000B
                rts

L0010:          move.w  -30(A6),D0
                move.w  -24(A6),D1
                muls    D0,D1
                move.w  D1,-24(A6)
                move.w  -26(A6),D1
                movea.l -34(A6),A4
                tst.w   D6
                bge.s   L0015
                neg.w   D0
                bra.s   L0015
L0011:          move.l  A0,-24(A5)
                move.l  A1,-10(A5)
                clr.w   D6
                lsr.w   #1,D3
                addx.w  D6,D6
                lsr.w   #1,D2
                addx.w  D6,D6
                move.b  -66(A6,D6.w),-1(A5)
                swap    D2
                swap    D3
                move.w  -24(A6),D3
                moveq   #1,D6
                bra.s   L0014
L0012:          move.w  D3,D2
                add.w   D0,D3
                and.w   D1,D2
                move.w  0(A4,D2.w),-60(A5)
                move.w  D6,-4(A5)
                move.w  D7,(A5)
L0013:          nop
                tas.b   (A5)
                bmi.s   L0013
L0014:          dbra    D4,L0012
                adda.w  A2,A0
                adda.w  A3,A1
                adda.w  -28(A6),A4
                swap    D2
                swap    D3
                move.w  -74(A6),D4
L0015:          dbra    D5,L0011
                rts
                rts
ZUEND:          END
