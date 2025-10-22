
;Dies ist das Werbeprogramm für 'Punish your Machine'
;(C)1991 by Till Bubeck, Weinstadt

;Hallo Steffen!
;Der Screen enthält keine Trap-Aufrufe (außer den beiden Supervisor-Calls).
;Er greift auf keine Variablen außerhalb von Text, Data, BSS, I/O zu.
;Ich denke, daß er also problemlos laufen müßte.
;Dieses Programm enthält einige Variablen und Routinen, welche gar nicht
;benützt werden. Da sie aber nicht viel Platz brauchen, laß sie einfach drin.
;
;Ich habe noch Probleme:
;       unterer Border: ist die Routine okay?
;       manchmal werden beim Initialisieren die Planes verschoben...
;               evtl. einfach das auf LOW-Rez schalten ausbauen, da der
;                  Loader ja eh in Low ankommt.,...,


                OPT X+

                clr.l   -(SP)
                move.w  #$0020,-(SP)
                trap    #1
                addq.l  #6,SP
                move.l  D0,save_ssp

                move.l  #screen+256,D0
                clr.b   D0
                move.l  D0,scr1

                movea.l D0,A0
                move.w  #(32000+60*160)/4-1,D7
löschen:        clr.l   (A0)+           ;Bildschirm löschen
                dbra    D7,löschen

                clr.w   -(SP)           ;Nicht kopieren
                pea     buff
                pea     font
                pea     scrolltext
                move.w  #200,-(SP)
                move.l  scr1,-(SP)
                bsr     mcode32         ;Init_Scroller
                lea     20(SP),SP

                move.w  #1,-(SP)
                pea     load_music(PC)
                bsr     mcode63         ;Hippelmusic an
                addq.l  #6,SP

                bsr     mcode34

                move.l  scr1,-(SP)
                bsr     mcode12         ;Show-Screen
                addq.l  #4,SP

                lea     picture_tab(PC),A6
pic_loop2:      movea.l (A6)+,A0        ;Adresse des Bildes holen
                cmpa.l  #-1,A0
                beq.s   start_prg

;Bild konvertieren:

                lea     50*34(A0),A0    ;Auf Bildstart gehen

                move.w  #3999,D7
ice_00:         moveq   #3,D6
ice_01:         move.w  (A0)+,D4        ;Wert holen
                moveq   #3,D5
ice_02:         add.w   D4,D4
                addx.w  D0,D0
                add.w   D4,D4
                addx.w  D1,D1
                add.w   D4,D4
                addx.w  D2,D2
                add.w   D4,D4
                addx.w  D3,D3
                dbra    D5,ice_02
                dbra    D6,ice_01

                movem.w D0-D3,-8(A0)
                dbra    D7,ice_00

                bra.s   pic_loop2

start_prg:

ende_pics:      lea     picture_tab(PC),A6
pic_loop:       movea.l (A6)+,A0
                cmpa.l  #-1,A0
                beq.s   ende_pics

                bsr.s   show_pic        ;Bild anzeigen

                move.w  #550,D7
wait_sync:      bsr     vsync

                move.b  $FFFFFC02.w,D0  ;Taste holen
                cmp.b   old_taste,D0    ;Immer noch keine neue Taste?
                beq.s   no_taste

                move.b  D0,old_taste
                cmp.b   #50,D0          ;'M'
                bne.s   no_music

                addq.w  #1,music_nr
                cmpi.w  #2,music_nr
                blt.s   okay
                clr.w   music_nr
okay:           move.w  music_nr(PC),D0
                lsl.w   #2,D0
                lea     music_adr(PC),A0

                move.w  #1,-(SP)        ;Musik Nummer 1
                move.l  0(A0,D0.w),-(SP)
                bsr     mcode63
                addq.l  #6,SP
                moveq   #0,D0

no_music:       cmpi.b  #57,D0          ;Space gedrückt?
                beq.s   prg_ende

no_taste:       dbra    D7,wait_sync

                bra.s   pic_loop

prg_ende:       bsr     music_off
                bsr     mcode35

                move.l  save_ssp,-(SP)
                move.w  #$0020,-(SP)
                trap    #1
                addq.l  #6,SP

                clr.w   -(SP)
                trap    #1

save_ssp:       DC.L 0

;Diese Routine zeigt ein Bild aus Medusa II an
;-> A0.L: Adresse des Bildes
show_pic:       move.l  A0,-(SP)
                bsr     mcode25         ;fade_out

                bsr     mcode24         ;Alle Raster aus

                movea.l (SP),A0         ;Adresse des Bildes
raster_loop:    move.w  (A0)+,-(SP)     ;Rasterzeile
                move.l  A0,-(SP)        ;und Palette
                bsr     mcode23         ;Set-Raster
                addq.l  #6,SP

                lea     32(A0),A0       ;nächster Raster
                tst.w   (A0)            ;Letzter Raster?
                bne.s   raster_loop

                move.w  #200,-(SP)
                pea     font_pal
                bsr     mcode23
                addq.l  #6,SP

                move.w  #201,-(SP)
                pea     font_pal
                bsr     mcode23
                addq.l  #6,SP

                movea.l (SP)+,A0        ;Adresse des Bildes
                lea     50*34(A0),A0    ;auf Bildstart gehen
                movea.l scr1,A1
                move.w  #7999,D7
trans_pic:      move.l  (A0)+,(A1)+
                dbra    D7,trans_pic

                bsr     mcode26         ;fade_in
                rts

anz_raster      EQU 20          ;max. Raster pro Bild vorher 50
scanlines       EQU 151         ;Anzahl der Scanlines beim Scrollen
digisound       EQU 82

tastatur_write  EQU $00000140   ;Hier wird der Schreibpointer gespeichert


;Schreibt Zeichen in Low-Res auf den Bildschirm an Position X/Y, in der Farbe 15
;C-Definition: char_fast(x1,y1,zeichen)
;int x,y;
;char zeichen;
mcode1:         movem.l D0-A6,-(SP)     ; Register retten

                andi.w  #$FFFC,64(SP)   ;auf gerade 4'er X
                lea     fnt_buf(PC),A2  ;Adresse des Zeichensatz

                movea.l logbase,A0      ;Basis des Bildschirms
                move.w  66(SP),D0       ;Bildschirmzeile
                mulu    #160,D0         ;160 Bytes/Bildschirmzeile
                adda.l  D0,A0
                move.w  64(SP),D0
                move.w  D0,D1           ;x sichern
                lsr.w   #4,D0           ;/16
                lsl.w   #3,D0
                adda.w  D0,A0           ;Adresse der 4 Words
                andi.w  #$000C,D1       ;0/4/8/12
                cmpi.w  #4,D1
                beq.s   po4
                cmpi.w  #8,D1
                beq.s   po8
                cmpi.w  #12,D1
                beq     po12

oben:           moveq   #0,D0
                move.b  69(SP),D0       ;Zeichen holen
                bsr     wandel          ;in internen Zeichensatz
                moveq   #4,D6           ;5 Zeilen
                moveq   #15,D5          ;zum Aus-anden
zeil_loop:      move.w  D0,D1
                lsr.w   #1,D1
                move.b  0(A2,D1.w),D3
                btst    #0,D0           ;gerades Zeichen?
                beq.s   gerade          ;ja!
                lsl.b   #4,D3           ;nur die untersten 4 Bit
                bra.s   zeil_it
gerade:         andi.b  #$F0,D3         ;in die obersten 4 Bit
zeil_it:        moveq   #0,D7           ;Bei Plane 0 starten
plane_loop:     and.b   D5,(A0)         ;Hintergrund löschen
                or.b    D3,(A0)         ;Zeichen einodern
                addq.l  #2,A0           ;eine Plane weiter
                and.b   D5,(A0)         ;Hintergrund löschen
                or.b    D3,(A0)         ;Zeichen einodern
                addq.l  #2,A0           ;eine Plane weiter
                and.b   D5,(A0)         ;Hintergrund löschen
                or.b    D3,(A0)         ;Zeichen einodern
                addq.l  #2,A0           ;eine Plane weiter
                and.b   D5,(A0)         ;Hintergrund löschen
                or.b    D3,(A0)         ;Zeichen einodern

                lea     154(A0),A0      ;Zeile tiefer
                lea     40(A2),A2       ;Zeile tiefer
                dbra    D6,zeil_loop
                bra.s   ende

po8:            addq.l  #1,A0           ;8 Pixel weiter
                bra.s   oben

po4:            moveq   #0,D0
                move.b  69(SP),D0       ;Zeichen holen
                bsr.s   wandel          ;in internen Zeichensatz
                moveq   #4,D6           ;5 Zeilen
                move.l  #$000000F0,D5   ;zum Aus-Anden
zeil2:          move.w  D0,D1
                lsr.w   #1,D1
                move.b  0(A2,D1.w),D3
                btst    #0,D0           ;gerades Zeichen?
                beq.s   gerade2         ;ja!
                andi.b  #$0F,D3         ;nur die untersten 4 Bit
                bra.s   zeil_it2
gerade2:        lsr.b   #4,D3           ;in die obersten 4 Bit
zeil_it2:       moveq   #0,D7           ;Bei Plane 0 starten
plane2:         and.b   D5,(A0)
                or.b    D3,(A0)         ;Zeichen einodern
                addq.l  #2,A0           ;eine Plane weiter
                and.b   D5,(A0)
                or.b    D3,(A0)         ;Zeichen einodern
                addq.l  #2,A0           ;eine Plane weiter
                and.b   D5,(A0)
                or.b    D3,(A0)         ;Zeichen einodern
                addq.l  #2,A0           ;eine Plane weiter
                and.b   D5,(A0)
                or.b    D3,(A0)         ;Zeichen einodern

                lea     154(A0),A0      ;Zeile tiefer
                lea     40(A2),A2       ;Zeile tiefer
                dbra    D6,zeil2
                bra.s   ende


po12:           addq.l  #1,A0           ;8 Pixel weiter
                bra.s   po4

wandel:         cmpi.w  #$009A,D0       ;'Ü'
                beq.s   ueh
                cmpi.w  #$008E,D0       ;'Ä'
                beq.s   aeh
                cmpi.w  #$0099,D0       ;'Ö'
                beq.s   oeh
                subi.w  #' ',D0
                rts

ueh:            move.w  #'#'-' ',D0
                rts
oeh:            move.w  #'$'-' ',D0
                rts
aeh:            move.w  #'&'-' ',D0
                rts

ende:           movem.l (SP)+,D0-A6
                rts

;Schreibt Zeichen in Low-Res auf den Bildschirm an Position X/Y
;C-Definition: char_out(farbe,hintergrund,x,y,zeichen)
;int farbe,....
mcode2:         movem.l D0-A6,-(SP)     ; Register retten

                andi.w  #$FFFC,68(SP)   ;auf gerade 4'er X
                lea     fnt_buf(PC),A2  ;Adresse des Zeichensatzes

                movea.l logbase,A0      ;Basis des Bildschirms
                move.w  70(SP),D0       ;Bildschirmzeile
                mulu    #160,D0         ;160 Bytes/Bildschirmzeile
                adda.l  D0,A0
                move.w  68(SP),D0       ;x holen
                move.w  D0,D1           ;x sichern
                lsr.w   #4,D0           ;/16
                lsl.w   #3,D0
                adda.w  D0,A0           ;Adresse der 4 Words
                andi.w  #$000C,D1       ;0/4/8/12
                cmpi.w  #4,D1
                beq.s   po42
                cmpi.w  #8,D1
                beq.s   po82
                cmpi.w  #12,D1
                beq     po122

oben2:          moveq   #0,D0
                move.b  73(SP),D0       ;Zeichen holen
                bsr.s   wandel          ;in internen Zeichensatz
                moveq   #4,D6           ;5 Zeilen
                moveq   #15,D5          ;zum Aus-anden
                move.l  #$000000F0,D4   ;zum Einodern des Hintergrundes
zeil_loop3:     move.w  D0,D1
                lsr.w   #1,D1
                move.b  0(A2,D1.w),D3
                btst    #0,D0           ;gerades Zeichen?
                beq.s   gerade3         ;ja!
                lsl.b   #4,D3           ;nur die untersten 4 Bit
                bra.s   zeil_it3
gerade3:        andi.b  #$F0,D3         ;in die obersten 4 Bit
zeil_it3:       moveq   #0,D7           ;Bei Plane 0 starten
plane_loop3:    btst    D7,67(SP)       ;Plane im Hintergrund gesetzt?
                beq.s   loesch_back3
                or.b    D4,(A0)         ;Hintergrund setzen
                btst    D7,65(SP)       ;Schriftfarbe an?
                bne.s   fertig3         ;auch an, nichts machen
                not.b   D3              ;invertieren
                and.b   D3,(A0)         ;Schrift wieder löschen
                not.b   D3              ;zurück invertieren
                bra.s   fertig3
loesch_back3:   and.b   D5,(A0)         ;Hintergrund löschen
                btst    D7,65(SP)       ;Schriftplane an?
                beq.s   fertig3         ;auch nicht, fertig
                or.b    D3,(A0)         ;Zeichen einodern

fertig3:        addq.l  #2,A0           ;eine Plane weiter
                addq.w  #1,D7
                cmpi.w  #4,D7
                bne.s   plane_loop3

                lea     160-8(A0),A0    ;Zeile tiefer
                lea     40(A2),A2       ;Zeile tiefer
                dbra    D6,zeil_loop3
                bra.s   ende2

po82:           addq.l  #1,A0           ;8 Pixel weiter
                bra.s   oben2

po42:           moveq   #0,D0
                move.b  73(SP),D0       ;Zeichen holen
                bsr     wandel          ;in internen Zeichensatz
                moveq   #4,D6           ;5 Zeilen
                move.l  #$000000F0,D5   ;zum Aus-Anden
                moveq   #15,D4          ;zum Setzen des Hintergrund
zeil4:          move.w  D0,D1
                lsr.w   #1,D1
                move.b  0(A2,D1.w),D3
                btst    #0,D0           ;gerades Zeichen?
                beq.s   gerade4         ;ja!
                andi.b  #$0F,D3         ;nur die untersten 4 Bit
                bra.s   zeil_it4
gerade4:        lsr.b   #4,D3           ;in die obersten 4 Bit
zeil_it4:       moveq   #0,D7           ;Bei Plane 0 starten
plane4:         btst    D7,67(SP)       ;Plane im Hintergrund gesetzt?
                beq.s   loesch_back4
                or.b    D4,(A0)         ;Hintergrund setzen
                btst    D7,65(SP)       ;Schriftfarbe an?
                bne.s   fertig4         ;auch an, nichts machen
                not.b   D3              ;invertieren
                and.b   D3,(A0)         ;Schrift wieder löschen
                not.b   D3              ;zurück invertieren
                bra.s   fertig4
loesch_back4:   and.b   D5,(A0)         ;Hintergrund löschen
                btst    D7,65(SP)       ;Schriftplane an?
                beq.s   fertig4         ;auch nicht, fertig
                or.b    D3,(A0)         ;Zeichen einodern

fertig4:        addq.l  #2,A0           ;eine Plane weiter
                addq.w  #1,D7
                cmpi.w  #4,D7
                bne.s   plane4

                lea     152(A0),A0      ;Zeile tiefer
                lea     40(A2),A2       ;Zeile tiefer
                dbra    D6,zeil4
                bra.s   ende2

po122:          addq.l  #1,A0           ;8 Pixel weiter
                bra.s   po42

ende2:          movem.l (SP)+,D0-A6
                rts


;Schreibt Zeichen auf den Bildschirm (logbase) an Position X/Y.
;Dabei scheint durch die Buchstaben der Hintergrund durch (Transparent)
;void char_trans(int farbe,int x,int y,char zeichen)
;-> 14(a6): zeichen
;   12(a6): y
;   10(a6): x
;    8(a6): Farbe
mcode4:         link    A6,#0
                movem.l D0-A5,-(SP)     ; Register retten

                andi.w  #$FFFC,10(A6)   ;auf gerade 4'er X
                lea     fnt_buf(PC),A2  ;Adresse des Zeichensatzes

                movea.l logbase,A0      ;Basis des Bildschirms
                move.w  12(A6),D0       ;Bildschirmzeile
                mulu    #160,D0         ;160 Bytes/Bildschirmzeile
                adda.l  D0,A0
                move.w  10(A6),D0       ;x holen
                move.w  D0,D1           ;x sichern
                lsr.w   #4,D0           ;/16
                lsl.w   #3,D0
                adda.w  D0,A0           ;Adresse der 4 Words
                andi.w  #$000C,D1       ;0/4/8/12
                cmpi.w  #4,D1
                beq.s   po43
                cmpi.w  #8,D1
                beq.s   po83
                cmpi.w  #12,D1
                beq     po123

oben3:          moveq   #0,D0
                move.b  15(A6),D0       ;Zeichen holen
                bsr     wandel          ;in internen Zeichensatz
                moveq   #4,D6           ;5 Zeilen
                moveq   #15,D5          ;zum Aus-anden
                move.l  #$000000F0,D4   ;zum Einodern des Hintergrundes
zeil_loop33:    move.w  D0,D1
                lsr.w   #1,D1           ;halbieren
                move.b  0(A2,D1.w),D3   ;Matrix der Zeichens holen
                btst    #0,D0           ;gerades Zeichen?
                beq.s   gerade33        ;ja!
                lsl.b   #4,D3           ;nur die untersten 4 Bit
                bra.s   zeil_it33
gerade33:       andi.b  #$F0,D3         ;in die obersten 4 Bit
zeil_it33:      move.w  D3,D1
                not.w   D1              ;invertiert
                moveq   #0,D7           ;Bei Plane 0 starten
plane_loop33:   btst    D7,9(A6)        ;Schriftfarbe an?
                beq.s   ausis1          ;auch nicht, fertig
                or.b    D3,(A0)         ;Zeichen einodern
                bra.s   fertig33
ausis1:         and.b   D1,(A0)         ;und weglöschen
fertig33:       addq.l  #2,A0           ;eine Plane weiter
                addq.w  #1,D7
                cmpi.w  #4,D7
                bne.s   plane_loop33

                lea     152(A0),A0
                lea     40(A2),A2
                dbra    D6,zeil_loop33
                bra.s   ende23

po83:           addq.l  #1,A0           ;8 Pixel weiter
                bra.s   oben3

po43:           moveq   #0,D0
                move.b  15(A6),D0       ;Zeichen holen
                bsr     wandel          ;in internen Zeichensatz
                moveq   #4,D6           ;5 Zeilen
                move.l  #$000000F0,D5   ;zum Aus-Anden
                moveq   #15,D4          ;zum Setzen des Hintergrund
zeil43:         move.w  D0,D1
                lsr.w   #1,D1
                move.b  0(A2,D1.w),D3
                btst    #0,D0           ;gerades Zeichen?
                beq.s   gerade43        ;ja!
                andi.b  #$0F,D3         ;nur die untersten 4 Bit
                bra.s   zeil_it43
gerade43:       lsr.b   #4,D3           ;in die obersten 4 Bit
zeil_it43:      move.w  D3,D1
                not.w   D1
                moveq   #0,D7           ;Bei Plane 0 starten
plane43:        btst    D7,9(A6)        ;Schriftplane an?
                beq.s   ausis2          ;auch nicht, fertig
                or.b    D3,(A0)         ;Zeichen einodern
                bra.s   fertig43
ausis2:         and.b   D1,(A0)         ;und weglöschen
fertig43:       addq.l  #2,A0           ;eine Plane weiter
                addq.w  #1,D7
                cmpi.w  #4,D7
                bne.s   plane43

                lea     152(A0),A0
                lea     40(A2),A2
                dbra    D6,zeil43
                bra.s   ende23

po123:          addq.l  #1,A0           ;8 Pixel weiter
                bra.s   po43

ende23:         movem.l (SP)+,D0-A5
                unlk    A6
                rts


;Initialisiert die ATARI-Hardware etc.
;C-Definition: init_hardware()

mcode3:         movem.l D0-A6,-(SP)

                move.l  #spurios,$00000060 ;NMI auf RTE setzen
                move.b  #%01001000,$FFFFFA17 ;Software-End-of-IRQ setzen

no_set:         movem.l (SP)+,D0-A6
                rts

stringout:      moveq   #0,D2
                move.b  (A0)+,D2        ;Zeichen holen
                beq.s   stringend
                move.w  D2,-(SP)
                move.w  D1,-(SP)        ;y
                move.w  D0,-(SP)        ;x
                bsr     mcode1          ;Fast-out
                addq.l  #6,SP
                addq.w  #4,D0           ;x+=4
                bra.s   stringout
stringend:      rts

spurios:        rte                     ;Tritt NMI auf, weitermachen

;Zeigt Screen an
;C-Definition: showscreen(adr)
;long adr;          /* ADresse des neuen Screens */

mcode12:        link    A6,#0
                movem.l D0-A6,-(SP)

                move.l  8(A6),D0        ;Adresse holen
                move.l  D0,$0000044E    ;log. Adresse setzen
                move.l  D0,logbase      ;auch als Logbase setzen
                lsr.l   #8,D0
                movea.w #$8201,A0       ;Adresse Shifter
                movep.w D0,0(A0)

vsync24:        bsr     vsync

                movem.l (SP)+,D0-A6
                unlk    A6
                rts

;Vertauscht die beiden Bildschirm, zeigt Screen1 an und macht Vsync
switchscreens:  move.l  scr2,D0
                move.l  scr1,scr2
                move.l  D0,scr1

                move.l  D0,-(SP)
                bsr.s   mcode12         ;Show_Screen
                addq.l  #4,SP
                rts

;Funktion kopiert anzahl Bildschirmzeilen von quelle nach ziel
mcode15:        link    A6,#0
                movem.l D0-A5,-(SP)

                movea.l 8(A6),A0        ;QUelle
                movea.l 12(A6),A1       ;Ziel
                move.w  16(A6),D7       ;Anzahl
                subq.w  #1,D7           ;wegen DBRA
                moveq   #40,D6          ;jeweils 40 Bytes
copy_loop:      movem.l (A0)+,D0-D5/A2-A5 ;40 Bytes holen
                movem.l D0-D5/A2-A5,(A1)
                adda.l  D6,A1
                movem.l (A0)+,D0-D5/A2-A5 ;40 Bytes holen
                movem.l D0-D5/A2-A5,(A1)
                adda.l  D6,A1
                movem.l (A0)+,D0-D5/A2-A5 ;40 Bytes holen
                movem.l D0-D5/A2-A5,(A1)
                adda.l  D6,A1
                movem.l (A0)+,D0-D5/A2-A5 ;40 Bytes holen
                movem.l D0-D5/A2-A5,(A1)
                adda.l  D6,A1
                dbra    D7,copy_loop

                movem.l (SP)+,D0-A5
                unlk    A6
                rts


; Mfp-Definitions:

iera            EQU $00FFFA07   ; Interrupt Enable RegisterA
isra            EQU $00FFFA0F   ; Interrupt In-Service Register
imra            EQU $00FFFA13   ; Interrupt Mask Register
imrb            EQU imra+2
timerbcr        EQU $00FFFA1B   ; TimerB-Kontrollregister
timerbdr        EQU $00FFFA21   ; TimerB-Datenregister

;Diese Funktion schaltet das HBL-System ein (einmal zu Beginn des Programms)
mcode34:        movem.l D0-A6,-(SP)

                bsr     mcode24         ;clear_raster
                bsr     mcode55         ;und neu ausrechnen
                bsr     rst22rst1       ;und zurück kopieren
                move.l  #rasters1,rasters ;und 1. System wieder anzeigen

                move.l  $00000120,old_timer_b
                move.l  $00000070,old_vbl

                andi.b  #$FE,$FFFFFA07  ;Timer B IRQ sperren
                andi.b  #%11011111,$FFFFFA09 ;200Hz sperren

                clr.b   $FFFFFA1B       ;Timer B STop!

                move.l  #sync_hbl,$00000120
                move.l  #vbl,$00000070

                ori.b   #1,$FFFFFA07    ;Timer B zulassen
                ori.b   #1,$FFFFFA13    ;nicht maskieren
                bclr    #3,$FFFFFA03    ;Timer B am Ende der Rasterzeile bitte

                bsr     vsync
                clr.b   $FFFF8260       ;LOW-Rez einschalten

                movem.l (SP)+,D0-A6
                rts


;Diese Funktion schaltet die Rasterroutinen wieder aus
mcode35:        movem.l D0-A6,-(SP)

                bclr    #0,iera         ;Timer B kein IRQ
                clr.b   timerbcr        ;TimerB stoppen

                move.l  old_vbl(PC),$00000070
                move.l  old_timer_b,$00000120

                bset    #5,$FFFFFA09    ;200Hz IRQ wieder an

                movem.l (SP)+,D0-A6
                rts

;Übermittelt einen String an den IKBD (reentrant)
;-> A0.L: Stringadresse
;  D1.W: Länge-1
send_ikbd_str:  move.b  (A0)+,D0
                bsr.s   send_ikbd
                dbra    D1,send_ikbd_str
                rts

;Übermittelt ein Byte an die Tastatur (reentrant)
;-> D0.B: Wert, der gesendet werden soll
send_ikbd:      btst    #1,$FFFFFC00    ;.w  ;ACIA bereit?
                beq.s   send_ikbd       ;Nein, warten
                move.b  D0,$FFFFFC02    ;.w  ;und abschicken
                rts


;Holt ein Zeichen aus dem Tastaturbuffer
;Ist kein Zeichen vorhanden, bringt die Routine -1 zurück
mcode57:        movem.l D1-A6,-(SP)

                move.l  #-1,zeichen

                move.w  head(PC),D1
                cmp.w   tail(PC),D1
                beq.s   nichts_da2

                move    SR,-(SP)
                move    #$2700,SR
                move.w  head(PC),D1
                addq.w  #4,D1
                cmpi.w  #80,D1
                bcs.s   buffer_ende
                moveq   #0,D1
buffer_ende:    lea     key_buffer(PC),A0
                moveq   #0,D0
                move.l  0(A0,D1.w),D0   ;Zeichen holen
                move.l  D0,zeichen
                move.w  D1,head
                move    (SP)+,SR

nichts_da2:     move.l  zeichen(PC),D0
                movem.l (SP)+,D1-A6
                rts

zeichen:        DC.L 0
head:           DC.W 0
tail:           DC.W 0
;Bit 0=Shift, 1=Alt, 2=CTRL:
shift:          DC.B 0          ;Nichts gedrückt
                EVEN
scancode:       DC.L $00000200  ;Zeiger auf Scancode-Tabelle (Vom Loader)
key_buffer:     DS.B 84


;Dies ist die VBL-Routine aus NEOchrome
vbl:            addq.l  #1,vbl_ct       ;Ein VBL mehr
                addq.l  #1,$000004BA    ;200Hz erhöhen
                move    #$2700,SR       ;Erstmal Raster initialisieren
                movem.l D0-A6,-(SP)

                movea.l rasters,A1      ;von hier die Raster holen
                movem.l 2(A1),D0-D7     ;VBL-Palette holen
                movem.l D0-D7,$FFFF8240 ;und setzen

                lea     34(A1),A1       ;ersten Raster nehmen
                move.l  A1,raster_pos   ;Wieder von oben anfangen

                move.w  -34(A1),D0      ;Rasteroffset holen

                clr.b   $FFFFFA1B       ;Timer B Stop!
                move.b  D0,$FFFFFA21    ;Hier beginnt der 1. Raster
                move.b  #8,$FFFFFA1B    ;Timer B Go!

vbl_end:        move    #$2300,SR       ;Neuen VBL wieder zulassen

                bsr     mcode33         ;Do_Scroller

                movem.l (SP)+,D0-A6
                rte

old_vbl:        DC.L 0



;Dies ist die neue HBL-Routine, die nicht mit Blitter arbeitet, dafür aber
;absolut flackerfrei arbeitet
sync_hbl:       movem.l D0-A1,register

                movea.l raster_pos(PC),A0 ;Position des nächsten Rasters
naechster_raster:clr.b  $FFFFFA1B.w     ;Timer B Stop!
                move.w  (A0)+,D0        ;Position des nächsten Rasters
                cmpi.w  #1,D0           ;Abstand eins?
                beq.s   kurzer_raster   ;ja, dann im IRQ bleiben
                move.b  D0,$FFFFFA21.w  ;Nächsten Raster setzen
                move.b  #8,$FFFFFA1B.w  ;und wieder starten

                movem.l (A0)+,D1-D7/A1  ;Farben holen
                move.l  A0,raster_pos   ;und merken

cpsync_loop:    cmp.b   $FFFFFA21.w,D0  ;synchonisieren
                beq.s   cpsync_loop

                nop
                nop
                nop
                nop                     ;Flackern verhindern
                nop
                nop

                movem.l D1-D7/A1,$FFFF8240.w ;Farben setzen

end_irq:        movem.l register(PC),D0-A1
                bclr    #0,$FFFFFA0F.w  ;In-Service-Bit löschen
                rte

raster_pos:     DC.L 0          ;Position des Rastersystems

kurzer_raster:  clr.b   $FFFFFA21.w     ;Timer setzen
                move.b  #8,$FFFFFA1B.w  ;und wieder starten

get_colors:     movem.l font_pal,D1-D7/A1 ;Farben holen

cpsync_loop56:  tst.b   $FFFFFA21.w     ;synchonisieren
                beq.s   cpsync_loop56

                bchg    #1,$FFFF820A.w  ;Rand auf

                REPT 18
                nop
                ENDR

                bchg    #1,$FFFF820A.w

                movem.l D1-D7/A1,$FFFF8240.w ;Farben setzen
                bra.s   end_irq


;Platz zum Retten der Register bei Sync_HBL
register:       DS.L 10


;Funktion zum Setzen einer Palette in einer bestimmten Rasterzeile
;C-Definition: set_raster(pal_adr,zeile,res)
mcode23:        link    A6,#0
                movem.l D0-A6,-(SP)

                lea     hbl_system,A0

                move.w  12(A6),D6
                beq.s   vbl_setzen

;Ist derselbe Raster schon vorhanden?
                move.w  #anz_raster-2,D7
nicht_der:      lea     34(A0),A0       ;Nächsten Eintrag nehmen
                cmp.w   (A0),D6         ;Ist dieser Raster an dieser Position?
                beq.s   vbl_setzen      ;ja, also Palette setzen
                dbra    D7,nicht_der

;Nein->Neuen Rastereintrag nehmen:
                lea     hbl_system,A0
besetzt:        lea     34(A0),A0       ;Einen Eintrag weitergehen
                tst.w   (A0)            ;Raster frei?
                bne.s   besetzt

vbl_setzen:     move.w  12(A6),(A0)+    ;Freier Eintrag->Position eintragen
                movea.l 8(A6),A1        ;Farbpalette
                cmpa.l  #0,A1           ;Keine Rasterpalette angegeben? (5)
                beq.s   del_raster      ;ja->Raster löschen (5)
                moveq   #15,D7          ;16 Worte übertragen
copy11:         move.w  (A1)+,(A0)+
                dbra    D7,copy11
                bra.s   end_set_raster  ;(5)

del_raster:     clr.w   -(A0)           ;Raster löschen (5)

end_set_raster: movem.l (SP)+,D0-A6
                unlk    A6
                rts


;Fadet eine bereits bestehende Rasterpalette herunter. (6)
;C-Definition: void fade_raster(rasterzeile,anzahl)
mcode72:        link    A6,#0
                movem.l D0-A5,-(SP)

                lea     hbl_system,A0

                move.w  8(A6),D6        ;Rasterzeile
                beq.s   vbl_setzen2

;Ist derselbe Raster schon vorhanden?
                move.w  #anz_raster-2,D7
nicht_der2:     lea     34(A0),A0       ;Nächsten Eintrag nehmen
                cmp.w   (A0),D6         ;Ist dieser Raster an dieser Position?
                beq.s   vbl_setzen2     ;ja, also Palette setzen
                dbra    D7,nicht_der2

                bra.s   rausraus

vbl_setzen2:    addq.l  #2,A0
                movea.l A0,A1           ;merken
                move.w  10(A6),D6
                bra.s   fade_dbra
schleife11:     movea.l A1,A0
                moveq   #1,D7           ;Zuerst "B"-Wert behandeln
                move.w  #%0000000000000111,D5 ;AND-Maske
                bsr     sub_pal         ;runterschieben
                movea.l A1,A0
                moveq   #16,D7          ;Dann "G"-Wert behandeln
                move.w  #%0000000001110000,D5 ;AND-Maske
                bsr     sub_pal
                movea.l A1,A0
                move.w  #256,D7         ;Dann "R"-Wert behandeln
                move.w  #%0000011100000000,D5 ;AND-Maske
                bsr     sub_pal
fade_dbra:      dbra    D6,schleife11

rausraus:       movem.l (SP)+,D0-A5
                unlk    A6
                rts


;Berechnet die Offsets innerhalb des HBL-System2 neu (CHS):
mcode55:        movem.l D1-A6,-(SP)

                clr.w   first_pal       ;es wird jetzt die erste Raster berechnet

                lea     rasters2,A2
                movem.l hbl_system+2,D0-D7 ;VBL-Palette holen
                movem.l D0-D7,2(A2)     ;und VBL-Palette eintragen
                lea     34(A2),A2       ;und zum 1. Raster gehen

                moveq   #1,D7           ;Y-Wert
such_nxt_raster:move.w  #1000,D6        ;Max-Wert
                move.w  #anz_raster-1,D5 ;Anzahl der zu untersuchenden Raster
                lea     hbl_system,A0   ;Tabelle der Rasters
such_min:       move.w  (A0),D0         ;Y-Position dieses Rasters
                beq.s   nxt_raster      ;unbenutzt->nächsten
                cmp.w   D6,D0           ;Dieser Raster weiter oben?
                bge.s   nxt_raster      ;Nein->nächsten
                cmp.w   D7,D0           ;weiter unten als letzter Raster?
                ble.s   nxt_raster      ;Nein->nächsten
                lea     2(A0),A1        ;Position der Palette merken
                move.w  D0,D6           ;als neuen Kleinsten nehmen
nxt_raster:     lea     34(A0),A0       ;nächsten Raster nehmen
                dbra    D5,such_min

                cmpi.w  #1000,D6        ;Alle Raster gesetzt?
                beq.s   fertig

                move.w  D6,D1           ;Position der aktuellen Rasters
                sub.w   D7,D1           ;-letzte Position=Offset
                move.w  D1,-34(A2)      ;Beim letzten Raster als Offset eintragen
                tst.w   first_pal       ;Ist es der 1. Raster?
                bne.s   weiter237       ;Nein, eintragen
                addq.w  #1,D1           ;Sonst eins tiefer
                move.w  #-1,first_pal   ;Jetzt nichtmehr die 1. Raster

weiter237:      addq.w  #2,A2           ;und diesen Offset überspringen
                moveq   #15,D1
copy2367:       move.w  (A1)+,(A2)+     ;Farbpalette übertragen
                dbra    D1,copy2367

                move.w  D6,D7           ;Position des aktiven Rasters->letzter
                bra.s   such_nxt_raster

fertig:         clr.w   -34(A2)         ;Letzter Offset=0

                movem.l (SP)+,D1-A6
                rts


;Kopiert Rasters1 -> Rasters2 (CHS):
rst12rst2:      movem.l D0-A6,-(SP)
                lea     rasters1,A0     ;Raster1->Rasters2
                lea     rasters2,A1
                bra.s   copy_them

;Kopiert Rasters2 -> Rasters1 (CHS):
rst22rst1:      movem.l D0-A6,-(SP)
                lea     rasters2,A0     ;Rasters2->Rasters1
                lea     rasters1,A1
copy_them:      move.w  #anz_raster-1,D6 ;max .Anzahl der Raster
copy_raster:    moveq   #16,D7          ;17 Worte übertragen
copy_pal:       move.w  (A0)+,(A1)+
                dbra    D7,copy_pal
                dbra    D6,copy_raster
                movem.l (SP)+,D0-A6
                rts


;Rechnet die Offsets neu aus, und zeigt die Raster dann an
mcode56:        movem.l D0-A6,-(SP)

                bsr     mcode55         ;und neu ausrechnen
                move.l  #rasters2,rasters ;und neue anzeigen
                bsr.s   vsync
                bsr.s   rst22rst1       ;und zurück kopieren
                move.l  #rasters1,rasters ;und 1. System wieder anzeigen
                bsr.s   vsync

                movem.l (SP)+,D0-A6
                rts


;Diese Funktion schaltet die Raster aus
;C-Definition: clear_interrupts()
mcode24:        movem.l D7-A0,-(SP)

                lea     hbl_system,A0
                move.w  #anz_raster-1,D7
clear_lp:       clr.w   (A0)
                lea     34(A0),A0
                dbra    D7,clear_lp

                lea     hbl_system+2,A0
                moveq   #7,D7           ;16 Worte
clear_lp2:      clr.l   (A0)+
                dbra    D7,clear_lp2    ;VBL-Palette schwarz

                movem.l (SP)+,D7-A0
                rts

;Diese Funktion wartet einen Vsync ab:
mcode62:
vsync:          move.l  D0,-(SP)

                move.l  vbl_ct,D0
warte_sync:     cmp.l   vbl_ct,D0
                beq.s   warte_sync

                move.l  (SP)+,D0
                rts

;Routine blendet das Bild langsam aus
;C-Definition: fade_out()
mcode25:        movem.l D0-A6,-(SP)

                moveq   #6,D6           ;7 mal subtrahieren
schleife1:      moveq   #1,D7           ;Zuerst "B"-Wert behandeln
                move.w  #%0000000000000111,D5 ;AND-Maske
                bsr.s   subtract        ;runterschieben
                moveq   #16,D7          ;Dann "G"-Wert behandeln
                move.w  #%0000000001110000,D5 ;AND-Maske
                bsr.s   subtract
                move.w  #256,D7         ;Dann "R"-Wert behandeln
                move.w  #%0000011100000000,D5 ;AND-Maske
                bsr.s   subtract

                bsr.s   vsync
                bsr.s   vsync
                bsr.s   vsync
                dbra    D6,schleife1    ;alle 8 mal

                movem.l (SP)+,D0-A6
                rts

subtract:       lea     rasters1+2,A0
pal_loop:       bsr.s   sub_pal
                addq.l  #2,A0
                tst.w   -36(A0)         ;Kommt noch ein Raster?
                bne.s   pal_loop
                rts

sub_pal:        moveq   #15,D3          ;16 Farben pro Palette
col_loop:       bsr.s   sub_col         ;Farbe runterziehen
                dbra    D3,col_loop     ;alle 16 Farben
                rts

sub_col:        move.w  (A0)+,D0        ;Farbe holen
                move.w  D0,D1           ;kopieren
                and.w   D5,D1           ;ausmaskieren
                beq.s   raus56          ;schon auf Null, nichts mehr machen

                sub.w   D7,D0           ;Farbwert eins runter
                move.w  D0,-2(A0)       ;Farbe zurückschreiben
raus56:         rts


;Diese Routine blendet das Bild wieder weich ein
;C-Definition: fade_in();
mcode26:        movem.l D0-A6,-(SP)

                bsr     mcode55         ;Offsets berechnen

                lea     rasters1,A0
                move.w  #anz_raster-1,D7
cler:           moveq   #16,D6
cler2:          clr.w   (A0)+           ;Farbpaletten auf schwarz
                dbra    D6,cler2
                dbra    D7,cler

                lea     rasters2,A0
                lea     rasters1,A1
                move.w  #anz_raster-1,D7
copy88:         move.w  (A0),(A1)       ;Offset übertragen
                lea     34(A0),A0
                lea     34(A1),A1
                dbra    D7,copy88

                moveq   #6,D6           ;7 mal addieren
schleife3:      moveq   #1,D7           ;Zuerst "B"-Wert behandeln
                move.w  #%0000000000000111,D5 ;AND-Maske
                bsr.s   addiere         ;runterschieben
                moveq   #16,D7          ;Dann "G"-Wert behandeln
                move.w  #%0000000001110000,D5 ;AND-Maske
                bsr.s   addiere
                move.w  #256,D7         ;Dann "R"-Wert behandeln
                move.w  #%0000011100000000,D5 ;AND-Maske
                bsr.s   addiere

                bsr     vsync
                bsr     vsync
                bsr     vsync
                dbra    D6,schleife3    ;alle 8 mal
                movem.l (SP)+,D0-A6
                rts

addiere:        lea     rasters1+2,A0   ;Wird verändert
                lea     rasters2+2,A1   ;Endwert
pal_loop2:      bsr.s   add_pal
                addq.l  #2,A0
                addq.l  #2,A1           ;Offsets überspringen
                tst.w   -36(A0)         ;Kommt noch ein Raster
                bne.s   pal_loop2
                rts

add_pal:        move.w  #15,D3          ;16 Farben pro Palette
col_loop2:
                move.w  (A0)+,D0        ;Farbe holen
                move.w  (A1)+,D2        ;Endwert holen
                move.w  D0,D1           ;kopieren
                and.w   D5,D1           ;ausmaskieren
                and.w   D5,D2           ;ausmaskieren
                cmp.w   D2,D1           ;Endwert erreicht?
                beq.s   raus24

                add.w   D7,D0           ;Farbwert eins hoch
                move.w  D0,-2(A0)       ;Farbe speichern
raus24:
                dbra    D3,col_loop2    ;alle 16 Farben
                rts


; Fehlerbehandlung:
toomany:        moveq   #-1,D0          ; zu viele Tasks
                rts


;Diese Funktion initialisiert den Scroller
;C-Definition:  initscroller(scr,y,text,font,scroll_buff,copy)
;long scr;
;int y;
;char text[];
;char font[];
;char scroll_buff[]
;FLAG copy;
mcode32:        link    A6,#0
                movem.l D0-A6,-(SP)

                move.l  8(A6),scrollbase ;Basis des Bildschirms
                move.w  12(A6),yscroller
                move.l  14(A6),scrollbegin
                move.l  18(A6),font_start
                move.l  22(A6),scroll_buff
                move.w  26(A6),copyscroller ;Flag, ob bei jedem VBL reinkopieren?
                bsr.s   initscroller

                clr.w   steuerzeichen   ;(1) Kein Steuerzeichen bisher

                clr.w   scroll_akt      ;in Buffer 0 starten
                movea.l 22(A6),A0       ;Scrollbuffer
                move.w  #6719,D7
clerr:          clr.l   (A0)+
                clr.l   (A0)+           ;Buffer löschen
                dbra    D7,clerr

                movem.l (SP)+,D0-A6
                unlk    A6
                rts

initscroller:   move.l  scrollbegin,scrollpos
                subq.l  #1,scrollpos    ;ein zeichen zurück
                move.w  #-200,delta_x
                rts

;Diese Funktion scrollt die Laufschrift um eins weiter
;C-Definition: void doscroller(void)
mcode33:        movem.l D0-A6,-(SP)

scroll:         cmpi.w  #-2,delta_x     ;Noch Spalten da?
                bgt     char_weiter     ;Ja, Buchstabe fertig machen

                addq.l  #1,scrollpos    ;nächstes Zeichen
                movea.l scrollpos,A0
                moveq   #0,D0
                move.b  (A0),D0         ;Nächstes Zeichen holen
                bne.s   verarbeite      ;Nicht das letzte, weiter

                bsr.s   initscroller    ;Wieder von vorne
                bra.s   scroll          ;und starten

verarbeite:     move.w  D0,D1           ;Zeichen merken
                lea     sonder(PC),A0
nxt_element:    tst.b   (A0)            ;Tabelle zuende?
                beq.s   kein_sonder
                cmp.b   (A0)+,D0        ;in Tabelle enthalten?
                bne.s   nxt_element

                suba.l  #sonder+1,A0
                move.w  A0,D0
                addi.w  #26*2,D0        ;Hinter den Groß und Kleinbuchstaben
                bra.s   sonderzeichen

kein_sonder:    cmpi.w  #'Z',D0
                ble.s   nehmen
                subi.w  #'a'-'Z'-1,D0   ;Kleinbuchstaben
nehmen:         subi.w  #'A',D0         ;in internen Code wandeln
                bpl.s   sonderzeichen   ;(1)
                move.w  D1,steuerzeichen ;(1) und merken
                bra.s   scroll          ;(1)

sonderzeichen:  movea.l font_start,A0   ;Basis der Bibliothek
                cmp.w   (A0)+,D0        ;Objekt noch in Bibliothek?
                bgt.s   scroll          ;Nein->nächster Buchstabe

                mulu    #20,D0          ;Adresse des Ob_Headers berechnen

                adda.l  D0,A0
                move.l  A0,char_basis

                movea.l font_start,A1   ;Basis der Objekte
                addq.l  #2,A1           ;Anzahl überspringen
                adda.l  (A0),A1         ;=Basis des Objekts
                move.l  A1,adrspalte    ;Datenstart
                move.w  4(A0),D0        ;Breite
                addq.w  #1,D0
                bclr    #0,D0           ;Gerade machen
                move.w  D0,delta_x
                move.w  D0,4(A0)

char_weiter:    movea.l scroll_buff,A0
                addq.l  #8,A0
                move.w  scroll_akt,D0   ;Aktueller Scroller
                mulu    #160*42,D0      ;Adresse berechnen
                adda.l  D0,A0

                moveq   #69,D7
vorschieben:    movem.l (A0)+,D0-D6/A2-A6
                movem.l D0-D6/A2-A6,-56(A0)
                movem.l (A0)+,D0-D6/A2-A6 ;um 8 Bytes nach vorne schieben
                movem.l D0-D6/A2-A6,-56(A0)
                dbra    D7,vorschieben

                tst.w   delta_x
                bgt.s   kein_space

                lea     zeros(PC),A3
                suba.l  A2,A2
                moveq   #0,D7
                bra.s   scroll_it

kein_space:     movea.l char_basis,A3
                movea.w 10(A3),A2       ;Breite in Bytes
                move.w  6(A3),D3        ;Höhe in Scanlines
                move.w  4(A3),D0        ;Breite des Objekts
                sub.w   delta_x,D0
                move.w  D0,D7
                lsr.w   #4,D0
                lsl.w   #3,D0
                movea.l adrspalte,A3
                adda.w  D0,A3           ;Adresse des BB
                andi.w  #15,D7          ;Skew
                neg.w   D7
                addi.w  #14,D7

scroll_it:      subq.w  #2,delta_x      ;eines weiter

                lea     -160*42-8+152(A0),A0 ;Start des Buffers, Zeilenende

                lea     -160*42(A0),A1  ;vorheriger Scroller
                movea.l scroll_buff,A6
                lea     152(A6),A6
                cmpa.l  A6,A1           ;vor dem 1. Scroller?
                bge.s   positi
                movea.l scroll_buff,A1
                adda.l  #152+7*160*42,A1 ;Scroller #7
positi:

                moveq   #41,D5          ;42 Scanlines hoch
all_planes:     moveq   #3,D6           ;4 Planes
scanline:       move.w  (A3)+,D0        ;Scroller holen
                lsr.w   D7,D0           ;Bit hinschieben
                andi.w  #3,D0           ;und ausmaskieren

                move.w  (A1)+,D4        ;Bildschirm holen
                lsl.l   #2,D4
                or.w    D0,D4           ;einodern
                move.w  D4,(A0)+        ;und zurück auf Screen
                dbra    D6,scanline

                subq.l  #8,A3           ;zurück zum Start des BB
                adda.l  A2,A3           ;und auf nächste Zeile

                subq.w  #1,D3           ;Eine Scanline weniger
                bgt.s   noch_was_da
                suba.l  A2,A2
                lea     zeros(PC),A3    ;Zeichen ist zu ende
                moveq   #0,D7

noch_was_da:
                lea     152(A1),A1      ;eine Zeile tiefer
                lea     152(A0),A0
                dbra    D5,all_planes

                movea.l scrollbase,A1   ;Basis des Bildschirm
                move.w  yscroller,D5
                mulu    #160,D5         ;160 Bytes pro Scanline
                adda.l  D5,A1
                lea     160(A1),A1

                lea     -42*160-152(A0),A0 ;zum Start des Buffers
                moveq   #69,D7
vorschieben2:   movem.l (A0)+,D0-D6/A2-A6
                movem.l D0-D6/A2-A6,(A1)
                movem.l (A0)+,D0-D6/A2-A6 ;auf Screen kopieren
                movem.l D0-D6/A2-A6,48(A1)
                lea     96(A1),A1
                dbra    D7,vorschieben2

                addq.w  #1,scroll_akt   ;nächster Scrollbuffer
                cmpi.w  #8,scroll_akt
                blt.s   okokok
                clr.w   scroll_akt
okokok:

                tst.w   copyscroller    ;Soll kopiert werden?
                beq.s   no_copy         ;Nein, nicht kopieren

                move.w  #18,-(SP)       ;18 Zeilen kopieren
                move.l  scr1,-(SP)      ;Ziel
                move.l  scrollbase,-(SP)
                bsr     mcode15         ;copy_zeilen
                move.l  scr2,4(SP)
                bsr     mcode15
                lea     10(SP),SP

no_copy:        movem.l (SP)+,D0-A6
                rts

zeros:          DC.L 0,0

sonder:         DC.B ".,:' üäö?ß" ;(3)
                DC.B '"$',0     ;(3)

                EVEN

;void hippel_sfx(int nr);
;Schaltet einen Hippelsoundeffekt an
mcode38:        link    A6,#0
                movem.l D0-A6,-(SP)

                moveq   #0,D0
                move.w  8(A6),D0
                movea.l music_routine(PC),A0
                jsr     4(A0)

                movem.l (SP)+,D0-A6
                unlk    A6
                rts

;Zeichnet ein Objekt aus einer Objekt-Bibliothek auf den Bildschirm.
;Sollte das Objekt mit Rastern abgespeichert worden sein, so werden
;diese auch gesetzt.
;void draw_obj(int ob_nr,void *ob_basis,int modus,void *scr_basis,int x,int y)
;22(a6): y
;20(a6): x
;16(a6): scr_basis
;14(a6): modus (0=Replace, 1=Transparent, 2=Only Palette)
;10(a6): ob_basis
; 8(a6): ob_nr
mcode47:        link    A6,#0

                move.l  A0,-(SP)        ;a0 retten

                movea.l 10(A6),A0       ;Basis der Bibliothek
                move.w  8(A6),D0        ;Nummer des Objekts
                cmp.w   (A0)+,D0        ;Objekt noch in Bibliothek?
                bgt.s   zu_gross1       ;Nein->nichts zeichnen

                mulu    #20,D0          ;Adresse des Ob_Headers berechnen
                adda.l  D0,A0

                move.l  16(A0),D0       ;reserved
                lsr.w   #8,D0
                beq.s   ohne_raster

                movea.l 10(A6),A1       ;Basis der Objekte
                addq.l  #2,A1           ;Anzahl überspringen
                adda.l  (A0),A1         ;=Basis des Objekts
                move.w  6(A0),D1        ;Höhe des Objekts
                mulu    10(A0),D1       ;*Breite=Anzahl Bytes
                adda.l  D1,A1           ;=Start der Raster
                bra.s   set_dbra
set_loop:       move.w  (A1)+,D1        ;Offset des Rasters holen
                add.w   22(A6),D1       ;+Y auf Screen
                clr.w   -(SP)           ;Low-Res
                move.w  D1,-(SP)        ;Position des Rasters
                move.l  A1,-(SP)
                bsr     mcode23         ;set_raster
                addq.l  #8,SP
                lea     32(A1),A1       ;Palette überspringen
set_dbra:       dbra    D0,set_loop

ohne_raster:
                cmpi.w  #2,14(A6)       ;Modus=Nur Palette? (4)
                beq.s   zu_gross1       ;(4)

                move.l  20(A6),-(SP)    ;x,y
                move.l  16(A6),-(SP)    ;scr_basis
                move.w  14(A6),-(SP)    ;modus
                move.w  6(A0),D0        ;Höhe des Objekts
                subq.w  #1,D0
                move.w  D0,-(SP)        ;(Höhe-1)->y2
                move.w  4(A0),D0        ;Breite des Objekts
                subq.w  #1,D0
                move.w  D0,-(SP)        ;(Breite-1)->x2
                clr.l   -(SP)           ;x1,y1
                move.l  10(A6),-(SP)    ;Ob_Basis
                move.w  8(A6),-(SP)     ;Ob_Nr
                bsr.s   mcode48         ;Draw_Obj_Part

zu_gross1:      movea.l (SP)+,A0        ;a0 restaurieren

                unlk    A6
                rts

;Zeichnet einen Teil eines Objekt aus einer Objekt-Bibliothek auf den Bildschirm
;void draw_obj_part(int ob_nr,void *ob_basis,int x1,y1,x2,y2,
;                     int modus,void *scr_basis,int x,int y);
;30(a6): y              (Screenkoordinaten)
;28(a6): x              (Screenkoordinaten)
;24(a6): scr_basis
;22(a6): modus          (0=Replace, 1=Transparent)
;20(a6): y2             (rechtes, unteres Eck innerhalb Objekt)
;18(a6): x2             (-'-)
;16(a6): y1             (Linkes, oberes Eck innerhalb Objekt)
;14(a6): x1             (-'-)
;10(a6): ob_basis
; 8(a6): ob_nr
mcode48:        link    A6,#0
                movem.l D0-A5,-(SP)

                movea.l 10(A6),A5       ;Basis der Bibliothek
                move.w  8(A6),D0        ;Nummer des Objekts
                cmp.w   (A5)+,D0        ;Objekt noch in Bibliothek?
                bgt.s   zu_gross2       ;Nein->nichts zeichnen

                mulu    #20,D0          ;Adresse des Ob_Headers berechnen
                adda.l  D0,A5

                movea.l 10(A6),A0       ;Basis der Objekte
                addq.l  #2,A0           ;Anzahl überspringen
                adda.l  (A5),A0         ;Adresse der Objekt-Daten

                move.w  #320,-(SP)      ;Breite des Bildschirms in Pixel
                move.l  24(A6),-(SP)    ;Screenbasis
                move.w  30(A6),-(SP)    ;y
                move.w  28(A6),-(SP)    ;x
                move.w  10(A5),D0       ;Breite des Objekts in Bytes
                add.w   D0,D0           ;breite in Pixel
                move.w  D0,-(SP)        ;Breite des Objekts in Pixel merken
                pea     (A0)            ;Adresse des Objektes
                move.w  20(A6),-(SP)    ;y2
                move.w  18(A6),-(SP)    ;x2
                move.w  16(A6),-(SP)    ;y1
                move.w  14(A6),-(SP)    ;x1
                tst.w   22(A6)          ;modus
                beq.s   replace
                bsr     mcode50         ;raster_transparent
                bra.s   end_draw_obj
replace:        bsr.s   mcode49         ;raster_replace
end_draw_obj:   lea     24(SP),SP

zu_gross2:      movem.l (SP)+,D0-A5
                unlk    A6
                rts

;Diese Routine kopiert einen rechteckigen Bildschirmbereich von einem Platz
;zu einem anderen. Dabei werden Überlappungen NICHT berücksichtigt.
;Der neue Bereich ersetzt den alten vollständig (replace).
;Die Routine clippt den Zielbereich. Als Bildschirmbreite wird die
;übergebene genommen, als Bildschirmhöhe wird 200 angenommen.
;Diese Routine braucht ungefähr 52% der Rechenzeit von Line_A #7 ohne Blitter,
;d.h. sie ist ungefähr doppelt so schnell.
;Line_A #7 mit Blitter ist ca. 12 mal schneller als diese Routine (also ca. 24
;mal schneller als Line_A #7 ohne Blitter!).
;Ist keine Verschiebung notwendig, ist diese Routine ca. 5 mal schneller als
;Line_A #7 ohne Blitter.
;Diese Routine benötigt ca. 65% der Rechenzeit von raster_transp.
;-> 30(a6): Breite einer Scanline in Pixel (Destination)
;  26(a6): Adresse des Bildschirms (Destination)
;  24(a6): y3 (Destination)
;  22(a6): x3 (Destination)
;  20(a6): Breite einer Scanline in Pixel (Source)
;  16(a6): Adresse des Bildschirms (Source)
;  14(a6): y2 (Source)
;  12(a6): x2 (Source)
;  10(a6): y1 (Source)
;   8(a6): x1 (Source)
mcode49:        link    A6,#-12

                movem.l D0-A5,-(SP)     ;Register retten

                move.w  8(A6),D0        ;Source X_Min
                move.w  10(A6),D1       ;Source Y_Min
                move.w  12(A6),D2       ;Source X_Max
                move.w  14(A6),D3       ;Source Y_Max
                move.w  22(A6),D4       ;Dest X_Min
                bpl.s   koord_ok1
                sub.w   D4,D0           ;neg->zu Source X_Min addieren
                move.w  D0,8(A6)        ;zurück in Parameter
                moveq   #0,D4           ;Clippen
                move.w  D4,22(A6)
koord_ok1:      move.w  24(A6),D5       ;Dest Y_Min
                bpl.s   koord_ok2
                sub.w   D5,D1           ;neg->zu Source Y_Min addieren
                move.w  D1,10(A6)       ;zurück in Parameter
                moveq   #0,D5
                move.w  D5,24(A6)
koord_ok2:      move.w  D2,D7           ;Source X_Max
                move.w  D4,D6           ;Dest X_Min
                sub.w   D0,D7           ;Source X_Max-Source X_min=Breite-1
                add.w   D7,D6           ;=Dest X_Max
                move.w  30(A6),D7       ;Breite in Pixel (Dest)
                subq.w  #1,D7           ;bis einschliesslich hier
                sub.w   D6,D7           ;von 320 abziehen
                bpl.s   koord_ok3
                add.w   D7,D6           ;von Dest X_Max abziehen
                add.w   D7,D2           ;und von Source X_Max
                move.w  D2,12(A6)       ;zurück in Parameter
koord_ok3:      sub.w   D1,D3           ;Höhe-1
                add.w   D3,D5           ;=Dest Y_Max
                move.w  #199,D3         ;Unterer Rand
                sub.w   D5,D3
                bpl.s   koord_ok4
                add.w   D3,D5           ;von Dest Y_Max abziehen
                add.w   D3,14(A6)       ;von Source Y_Max abziehen
koord_ok4:      move.w  D6,-2(A6)       ;Dest X_Max merken
                move.w  D5,-4(A6)       ;Dest Y_Max merken

                move.w  12(A6),D5       ;Source X_Max
                sub.w   8(A6),D5        ;-Source X_Min
                bmi     nichts_da7
                move.w  14(A6),D5       ;Source Y_Max
                sub.w   10(A6),D5       ;-Source Y_Min
                bmi     nichts_da7

                moveq   #$0F,D5
                move.w  D0,D1           ;Source X_Min
                and.w   D5,D1
                move.w  D4,D3           ;Dest X_Min
                and.w   D5,D3
                lsr.w   #4,D0           ;# des BBs (Source X_Min)
                lsr.w   #4,D4           ;# des BBs (Dest X_Min)
                lsr.w   #4,D2           ;# des BBs (Source X_Max)
                lsr.w   #4,D6           ;# des BBs (Dest X_Max)
                sub.w   D0,D2           ;X_Max-X_Min=Breite in BBs-1 (Source)
                sub.w   D4,D6           ;-'- (Dest)
                move.w  D2,D5           ;Breite des Sourceblocks
                sub.w   D6,D5           ;-Breite des Zielblocks
                andi.w  #1,D5           ;NFSR?
                lsl.w   #2,D5           ;an richtige Position schieben
                sub.w   D3,D1           ;Skew
                move.w  D6,D0           ;Breite des Zielblocks in BBs
                subq.w  #1,D0
                move.w  D0,-6(A6)       ;und merken
                move.w  D2,D7           ;Breite des Sourceblocks in BBs-1
                move.w  D1,D4           ;Skew
                move.w  D1,D2           ;Skew
                tst.w   D4              ;Skew rechts oder links?
                bgt.s   links_rum       ;ja
                beq.s   weiter1
                neg.w   D4              ;Skew positiv machen
                addq.w  #1,D5           ;und merken
links_rum:      cmpi.w  #8,D4           ;Skew weniger als 8?
                blt.s   weiter1
                addq.w  #2,D5           ;und merken, besser andersrum shiften(!)
                neg.w   D4              ;Skew umdrehen, also
                addi.w  #16,D4          ;von 16 abziehen
weiter1:        move.w  12(A6),D0       ;Source X_Max
                move.w  14(A6),D1       ;Source Y_Max
                bsr     calc_quelle     ;von hinten reinkommen
                lsl.w   #3,D7
                sub.w   D3,D7           ;Breite einer Zeile in Bytes abziehen(?)
                move.w  D7,-8(A6)       ;Offsets am Zeilenende merken
                move.w  -2(A6),D0       ;Dest X_Max
                move.w  -4(A6),D1       ;Dest Y_Max
                bsr     calc_ziel       ;auch von hinten rein
                lsl.w   #3,D6           ;Breite in BBs->Breite in Bytes
                sub.w   D3,D6           ;Breite einer Zeile in Bytes abziehen(?)
                move.w  D6,-10(A6)      ;Offset am Zeilenende merken
                bsr     get_masken      ;Masken->D6
                swap    D6              ;Masken vertauschen
                asl.w   #3,D5           ;je nach Konstellation richtige Routine
                movea.l L0009(PC,D5.w),A3
                movea.l L0008(PC,D5.w),A4
                tst.w   -6(A6)          ;Breite des Zielblocks in BBs>=2?
                bgt     start_copy      ;ja
                blt.s   klein
                bclr    #3,D5           ;Skew nach rechts?
                beq     start_copy      ;Nein, dann okay
                lsr.w   #2,D5           ;nur noch mal 4
                movea.l rechts_klein(PC,D5.w),A4 ;Spezialroutine
                bra.s   start_copy

;Tabelle für Objekte, die genau 2 BBs breit sind, und nach rechts geschoben
;werden, da es für die nachfolgenden Routine (L0037) nicht möglich war an
;bestimmte Informationen heranzukommen.
rechts_klein:   DC.L L0023b
                DC.L L0025b
                DC.L L0024b
                DC.L L0026b

;Routinen für nur einen BB breite Objekte:
L0007:          DC.L L0021
                DC.L L0022
                DC.L L0022
                DC.L L0021

;Routinen für mindestens 2 BB breite Objekte
L0008:          DC.L L0026      ;4 . = 0
L0009:          DC.L L0030
                DC.L L0023      ;5 . = 1 -
                DC.L L0033
                DC.L L0024      ;6 . = 2
                DC.L L002F
                DC.L L0025      ;7 . = 3 -
                DC.L L0034
                DC.L L0025      ;12  = 4
                DC.L L0030
                DC.L L0024      ;13  = 5 -
                DC.L L0033
                DC.L L0023      ;14  = 6
                DC.L L002F
                DC.L L0026      ;15  = 7 -
                DC.L L0034

klein:          move.l  D6,D0           ;Maske
                swap    D0
                and.w   D0,D6           ;linker und rechter Rand verknüpfen
                lea     nxt_scanline(PC),A3
                btst    #5,D5           ;NFSR?
                bne.s   start_copy      ;Ja->Normal arbeiten
                lsr.w   #1,D5           ;nur noch mal 4
                andi.w  #%0000000000001100,D5
                movea.l L0007(PC,D5.w),A4 ;Sonst andere Routine nehmen
start_copy:     move.w  14(A6),D5       ;y2
                sub.w   10(A6),D5       ;-y1=Höhe in Pixeln-1
                addq.w  #1,D5           ;=Höhe in Pixeln
                swap    D5
                move.w  -6(A6),D5       ;Breite des Zielblocks in BBs-2
                addq.l  #8,A0           ;Ans Ende des BB gehen
                addq.l  #8,A1           ;-'-
                tst.w   D4              ;Skew=0?
                beq.s   no_shift
                jmp     (A4)            ;und Plane bearbeiten

get_masken:     move.w  -2(A6),D0       ;Dest X_Max
                andi.w  #$000F,D0
                add.w   D0,D0
                move.w  L0015(PC,D0.w),D6 ;Maske holen
                swap    D6
                move.w  22(A6),D0       ;Dest X_Min
                andi.w  #$000F,D0
                add.w   D0,D0
                move.w  L0014(PC,D0.w),D6 ;Maske holen
                not.w   D6              ;und inverieren
                rts

L0014:          DC.W %0000000000000000
L0015:          DC.W %1000000000000000
                DC.W %1100000000000000
                DC.W %1110000000000000
                DC.W %1111000000000000
                DC.W %1111100000000000
                DC.W %1111110000000000
                DC.W %1111111000000000
                DC.W %1111111100000000
                DC.W %1111111110000000
                DC.W %1111111111000000
                DC.W %1111111111100000
                DC.W %1111111111110000
                DC.W %1111111111111000
                DC.W %1111111111111100
                DC.W %1111111111111110
                DC.W %1111111111111111

;Routine berechnet die Startadresse des Quellrasters relativ zu:
;-> D0: X
;  D1: Y
;<- A0: Quellrasteradresse
calc_quelle:    movea.l 16(A6),A0       ;Anfangsadresse Quellraster
                move.w  20(A6),D3       ;Breite Quelle in Pixel
                lsr.w   #1,D3           ;Breite in Bytes
                lsr.w   #4,D0           ;# des BBs berechnen
                lsl.w   #3,D0
                muls    D3,D1           ;Y Offset ausrechnen
                adda.l  D1,A0
                adda.w  D0,A0           ;und dazuaddieren
                rts

;Routine berechnet die Startadresse des Zielrasters relativ zu:
;-> D0: X
;  D1: Y
;<- A1: Zielrasteradresse
calc_ziel:      movea.l 26(A6),A1       ;Anfangsadresse Zielraster
                move.w  30(A6),D3       ;Breite Ziel in Pixel
                lsr.w   #1,D3           ;Breite in Bytes berechnen
                lsr.w   #4,D0           ;# des BBs berechnen
                lsl.w   #3,D0
                muls    D3,D1           ;Y Offset berechnen
                adda.l  D1,A1
                adda.w  D0,A1           ;und dazuaddieren
                rts

;Spezialroutine falls Skew=0:
no_shift:       lea     no_shift1(PC),A4 ;Bei nächster Scanline diese Routine
                move.l  D6,D0           ;Maske retten
                swap    D6
                move.w  D6,D7           ;obere Maske->D7
                swap    D7
                move.w  D6,D7           ;->D7
                move.w  D0,D6           ;untere Maske->D6

no_shift1:      move.l  -(A0),D0        ;Plane 2 und 3 holen (12)
                move.l  -(A0),D1        ;Plane 0 und 1 holen (12)
                move.l  -(A1),D2        ;Screen holen   (12)
                move.l  -(A1),D3
                eor.l   D2,D0
                and.l   D6,D0
                eor.l   D2,D0
                eor.l   D3,D1
                and.l   D6,D1
                eor.l   D3,D1
                move.l  D0,4(A1)
                move.l  D1,(A1)
                tst.w   D5              ;nur ein BB?
                bmi     nxt_scan2       ;ja, -> Weiter
                bra.s   bb_dbra2

bb_mitte2:      move.l  -(A0),-(A1)     ;Plane 2 und 3 holen (12)
                move.l  -(A0),-(A1)     ;Plane 0 und 1 holen (12)
bb_dbra2:       dbra    D5,bb_mitte2    ;zählt bis auf 0 runter

                move.l  -(A0),D0        ;Plane 2 und 3 holen (12)
                move.l  -(A0),D1        ;Plane 0 und 1 holen (12)
                move.l  -(A1),D2        ;Screen holen   (12)
                move.l  -(A1),D3
                eor.l   D2,D0
                and.l   D7,D0
                eor.l   D2,D0
                eor.l   D3,D1
                and.l   D7,D1
                eor.l   D3,D1
                move.l  D0,4(A1)
                move.l  D1,(A1)
                bra     nxt_scan2

L0021:          move.w  -(A0),D0
                rol.w   D4,D0
                move.w  -(A1),D7
                eor.w   D7,D0
                and.w   D6,D0
                eor.w   D7,D0
                move.w  D0,(A1)

                move.w  -(A0),D0
                rol.w   D4,D0
                move.w  -(A1),D3
                eor.w   D3,D0
                and.w   D6,D0
                eor.w   D3,D0
                move.w  D0,(A1)

                move.w  -(A0),D0
                rol.w   D4,D0
                move.w  -(A1),D2
                eor.w   D2,D0
                and.w   D6,D0
                eor.w   D2,D0
                move.w  D0,(A1)

                move.w  -(A0),D0
                rol.w   D4,D0
                move.w  -(A1),D1
                eor.w   D1,D0
                and.w   D6,D0
                eor.w   D1,D0
                move.w  D0,(A1)
                jmp     (A3)

L0022:          move.w  -(A0),D0
                ror.w   D4,D0
                move.w  -(A1),D7
                eor.w   D7,D0
                and.w   D6,D0
                eor.w   D7,D0
                move.w  D0,(A1)

                move.w  -(A0),D0
                ror.w   D4,D0
                move.w  -(A1),D3
                eor.w   D3,D0
                and.w   D6,D0
                eor.w   D3,D0
                move.w  D0,(A1)

                move.w  -(A0),D0
                ror.w   D4,D0
                move.w  -(A1),D2
                eor.w   D2,D0
                and.w   D6,D0
                eor.w   D2,D0
                move.w  D0,(A1)

                move.w  -(A0),D0
                ror.w   D4,D0
                move.w  -(A1),D1
                eor.w   D1,D0
                and.w   D6,D0
                eor.w   D1,D0
                move.w  D0,(A1)
                jmp     (A3)

L0023:          move.w  -(A0),D7        ;Plane aus Objekt holen
                move.w  -(A0),D3
                move.w  -(A0),D2
                move.w  -(A0),D1
                swap    D7
                swap    D3
                swap    D2
                swap    D1
L0024:          move.w  -(A0),D7        ;nächste Plane aus Objekt
                move.w  -(A0),D3
                move.w  -(A0),D2
                move.w  -(A0),D1
                swap    D7
                swap    D3
                swap    D2
                swap    D1

                move.l  D7,D0           ;ins Schieberegister
                ror.l   D4,D0           ;runterschieben
                move.w  -(A1),D7        ;Screen holen
                eor.w   D7,D0
                and.w   D6,D0
                eor.w   D7,D0
                move.w  D0,(A1)         ;und zurück auf Screen

                move.l  D3,D0           ;ins Schieberegister
                ror.l   D4,D0           ;runterschieben
                move.w  -(A1),D3        ;Screen holen
                eor.w   D3,D0
                and.w   D6,D0
                eor.w   D3,D0
                move.w  D0,(A1)         ;und zurück auf Screen

                move.l  D2,D0           ;ins Schieberegister
                ror.l   D4,D0           ;runterschieben
                move.w  -(A1),D2        ;Screen holen
                eor.w   D2,D0
                and.w   D6,D0
                eor.w   D2,D0
                move.w  D0,(A1)         ;und zurück auf Screen

                move.l  D1,D0           ;ins Schieberegister
                ror.l   D4,D0           ;runterschieben
                move.w  -(A1),D1        ;Screen holen
                eor.w   D1,D0
                and.w   D6,D0
                eor.w   D1,D0
                move.w  D0,(A1)         ;und zurück auf Screen

                jmp     (A3)

L0025:          move.w  -(A0),D7        ;Plane aus Objekt holen
                move.w  -(A0),D3
                move.w  -(A0),D2
                move.w  -(A0),D1
                swap    D7
                swap    D3
                swap    D2
                swap    D1
L0026:          move.w  -(A0),D7
                move.w  -(A0),D3
                move.w  -(A0),D2
                move.w  -(A0),D1
                move.l  D7,D0           ;ins Schieberegister
                swap    D7
                rol.l   D4,D0           ;schieben
                move.w  -(A1),D7        ;Screen holen
                eor.w   D7,D0
                and.w   D6,D0
                eor.w   D7,D0
                move.w  D0,(A1)

                move.l  D3,D0           ;ins Schieberegister
                swap    D3
                rol.l   D4,D0           ;schieben
                move.w  -(A1),D3        ;Screen holen
                eor.w   D3,D0
                and.w   D6,D0
                eor.w   D3,D0
                move.w  D0,(A1)

                move.l  D2,D0           ;ins Schieberegister
                swap    D2
                rol.l   D4,D0           ;schieben
                move.w  -(A1),D2        ;Screen holen
                eor.w   D2,D0
                and.w   D6,D0
                eor.w   D2,D0
                move.w  D0,(A1)

                move.l  D1,D0           ;ins Schieberegister
                swap    D1
                rol.l   D4,D0           ;schieben
                move.w  -(A1),D1        ;Screen holen
                eor.w   D1,D0
                and.w   D6,D0
                eor.w   D1,D0
                move.w  D0,(A1)
                jmp     (A3)

L0023b:         move.w  -(A0),D7
                move.w  -(A0),D3
                move.w  -(A0),D2
                move.w  -(A0),D1
                swap    D7
                swap    D3
                swap    D2
                swap    D1
L0024b:         move.w  -(A0),D7
                move.w  -(A0),D3
                move.w  -(A0),D2
                move.w  -(A0),D1
                swap    D7
                swap    D3
                swap    D2
                swap    D1
                ror.l   D4,D7
                ror.l   D4,D3
                ror.l   D4,D2
                ror.l   D4,D1

                move.w  -(A1),D0
                eor.w   D0,D7
                and.w   D6,D7
                eor.w   D0,D7
                move.w  D7,(A1)

                move.w  -(A1),D0
                eor.w   D0,D3
                and.w   D6,D3
                eor.w   D0,D3
                move.w  D3,(A1)

                move.w  -(A1),D0
                eor.w   D0,D2
                and.w   D6,D2
                eor.w   D0,D2
                move.w  D2,(A1)

                move.w  -(A1),D0
                eor.w   D0,D1
                and.w   D6,D1
                eor.w   D0,D1
                move.w  D1,(A1)
                bra     L0037           ;und dort geht's weiter

L0025b:         move.w  -(A0),D7
                move.w  -(A0),D3
                move.w  -(A0),D2
                move.w  -(A0),D1
                swap    D7
                swap    D3
                swap    D2
                swap    D1
L0026b:         move.w  -(A0),D7
                move.w  -(A0),D3
                move.w  -(A0),D2
                move.w  -(A0),D1
                rol.l   D4,D7
                rol.l   D4,D3
                rol.l   D4,D2
                rol.l   D4,D1

                move.w  -(A1),D0
                eor.w   D0,D7
                and.w   D6,D7
                eor.w   D0,D7
                move.w  D7,(A1)

                move.w  -(A1),D0
                eor.w   D0,D3
                and.w   D6,D3
                eor.w   D0,D3
                move.w  D3,(A1)

                move.w  -(A1),D0
                eor.w   D0,D2
                and.w   D6,D2
                eor.w   D0,D2
                move.w  D2,(A1)

                move.w  -(A1),D0
                eor.w   D0,D1
                and.w   D6,D1
                eor.w   D0,D1
                move.w  D1,(A1)
                bra     L0037           ;dort geht's weiter

L002B:          move.w  -(A0),D7
                move.w  -(A0),D3
                move.w  -(A0),D2
                move.w  -(A0),D1
                swap    D7
                swap    D3
                swap    D2
                swap    D1

                move.l  D7,D0
                ror.l   D4,D0
                move.w  D0,-(A1)

                move.l  D3,D0
                ror.l   D4,D0
                move.w  D0,-(A1)

                move.l  D2,D0
                ror.l   D4,D0
                move.w  D0,-(A1)

                move.l  D1,D0
                ror.l   D4,D0
                move.w  D0,-(A1)

                jmp     (A3)

L002C:          move.w  -(A0),D7
                move.w  -(A0),D3
                move.w  -(A0),D2
                move.w  -(A0),D1

                move.l  D7,D0
                swap    D7
                rol.l   D4,D0
                move.w  D0,-(A1)

                move.l  D3,D0
                swap    D3
                rol.l   D4,D0
                move.w  D0,-(A1)

                move.l  D2,D0
                swap    D2
                rol.l   D4,D0
                move.w  D0,-(A1)

                move.l  D1,D0
                swap    D1
                rol.l   D4,D0
                move.w  D0,-(A1)
                jmp     (A3)

L002F:          dbra    D5,L002B

                move.w  -(A0),D7
                move.w  -(A0),D3
                move.w  -(A0),D2
                move.w  -(A0),D1
                ror.l   D4,D7
                ror.l   D4,D3
                ror.l   D4,D2
                ror.l   D4,D1
                bra.s   L0037

L0030:          dbra    D5,L002C

                move.w  -(A0),D7
                move.w  -(A0),D3
                move.w  -(A0),D2
                move.w  -(A0),D1
                rol.l   D4,D7
                rol.l   D4,D3
                rol.l   D4,D2
                rol.l   D4,D1
                bra.s   L0038

L0033:          cmpi.w  #1,D5           ;Nur bis 0 runterzählen
                beq.s   spezial         ;bei eins Spezialroutine
                dbra    D5,L002B
                bra.s   L0037

spezial:        move.w  -(A0),D7
                move.w  -(A0),D3
                move.w  -(A0),D2
                move.w  -(A0),D1
                swap    D7
                swap    D3
                swap    D2
                swap    D1
                ror.l   D4,D7
                ror.l   D4,D3
                ror.l   D4,D2
                ror.l   D4,D1
                move.w  D7,-(A1)
                move.w  D3,-(A1)
                move.w  D2,-(A1)
                move.w  D1,-(A1)
                bra.s   L0037

L0034:          cmpi.w  #1,D5
                beq.s   spezial2
                dbra    D5,L002C
                bra.s   L0037

spezial2:       move.w  -(A0),D7
                move.w  -(A0),D3
                move.w  -(A0),D2
                move.w  -(A0),D1
                rol.l   D4,D7
                rol.l   D4,D3
                rol.l   D4,D2
                rol.l   D4,D1
                move.w  D7,-(A1)
                move.w  D3,-(A1)
                move.w  D2,-(A1)
                move.w  D1,-(A1)

L0037:          swap    D7
                swap    D3
                swap    D2
                swap    D1
L0038:          swap    D6
                move.w  -(A1),D0        ;Screen holen
                eor.w   D0,D7
                and.w   D6,D7
                eor.w   D0,D7
                move.w  D7,(A1)         ;und zurück auf Screen

                move.w  -(A1),D0        ;Screen holen
                eor.w   D0,D3
                and.w   D6,D3
                eor.w   D0,D3
                move.w  D3,(A1)         ;und zurück auf Screen

                move.w  -(A1),D0        ;Screen holen
                eor.w   D0,D2
                and.w   D6,D2
                eor.w   D0,D2
                move.w  D2,(A1)         ;und zurück auf Screen

                move.w  -(A1),D0        ;Screen holen
                eor.w   D0,D1
                and.w   D6,D1
                eor.w   D0,D1
                move.w  D1,(A1)         ;und zurück auf Screen

L0039:          swap    D6
nxt_scan2:      move.w  -6(A6),D5       ;Breite in BBs holen
nxt_scanline:   swap    D5
                subq.w  #1,D5           ;Eine Scanline weniger
                beq.s   nichts_da7
                swap    D5              ;Wieder den BB-Zähler
                adda.w  -8(A6),A0       ;zur nächsten Scanline
                adda.w  -10(A6),A1      ;-'-
                addq.l  #8,A0
                addq.l  #8,A1
                jmp     (A4)            ;und von vorne starten
nichts_da7:     movem.l (SP)+,D0-A5
                unlk    A6
                rts                     ;und schultz

;Diese Routine kopiert einen rechteckigen Bildschirmbereich von einem Platz
;zu einem anderen. Dabei werden Überlappungen NICHT berücksichtigt.
;Der neue Bereich wird in den alten eingeknüpft, d.h. dort wo der neue
;Bereich Hintergrundfarbe hat, scheint der alte Bereich durch. (transparent)
;Diese Routine clippt den Zielbereich. Als Breite werden die übergebene, als
;Höhe 200 angenommen.
;Diese Routine ist ungefähr 6.8 mal schneller als Line_A #7 ohne Blitter
;Line_A #7 mit Blitter braucht ungefähr 85% der Rechenzeit dieser Routine.
;Ist keine Verschiebung notwendig, ist diese Routine ca. 13 mal schneller als
;Line_A #7 ohne Blitter!
;-> 30(a6): Breite einer Scanline in Pixel (Destination)
;  26(a6): Adresse des Bildschirms (Destination)
;  24(a6): y3 (Destination)
;  22(a6): x3 (Destination)
;  20(a6): Breite einer Scanline in Pixel (Source)
;  16(a6): Adresse des Bildschirms (Source)
;  14(a6): y2 (Source)
;  12(a6): x2 (Source)
;  10(a6): y1 (Source)
;   8(a6): x1 (Source)
mcode50:        link    A6,#-12

                movem.l D0-A5,-(SP)     ;Register retten

                move.w  8(A6),D0        ;Source X_Min
                move.w  10(A6),D1       ;Source Y_Min
                move.w  12(A6),D2       ;Source X_Max
                move.w  14(A6),D3       ;Source Y_Max
                move.w  22(A6),D4       ;Dest X_Min
                bpl.s   koord_ok11
                sub.w   D4,D0           ;neg->zu Source X_Min addieren
                move.w  D0,8(A6)        ;zurück in Parameter
                moveq   #0,D4           ;Clippen
                move.w  D4,22(A6)
koord_ok11:     move.w  24(A6),D5       ;Dest Y_Min
                bpl.s   koord_ok22
                sub.w   D5,D1           ;neg->zu Source Y_Min addieren
                move.w  D1,10(A6)       ;zurück in Parameter
                moveq   #0,D5
                move.w  D5,24(A6)
koord_ok22:     move.w  D2,D7           ;Source X_Max
                move.w  D4,D6           ;Dest X_Min
                sub.w   D0,D7           ;Source X_Max-Source X_min=Breite-1
                add.w   D7,D6           ;=Dest X_Max
                move.w  30(A6),D7       ;Breite in Pixel (Dest)
                subq.w  #1,D7           ;bis einschliesslich hier
                sub.w   D6,D7           ;von 320 abziehen
                bpl.s   koord_ok33
                add.w   D7,D6           ;von Dest X_Max abziehen
                add.w   D7,D2           ;und von Source X_Max
                move.w  D2,12(A6)       ;zurück in Parameter
koord_ok33:     sub.w   D1,D3           ;Höhe-1
                add.w   D3,D5           ;=Dest Y_Max
                move.w  #199,D3         ;Unterer Rand
                sub.w   D5,D3
                bpl.s   koord_ok44
                add.w   D3,D5           ;von Dest Y_Max abziehen
                add.w   D3,14(A6)       ;von Source Y_Max abziehen
koord_ok44:     move.w  D6,-2(A6)       ;Dest X_Max merken
                move.w  D5,-4(A6)       ;Dest Y_Max merken

                move.w  12(A6),D5       ;Source X_Max
                sub.w   8(A6),D5        ;-Source X_Min
                bmi     nichts_da7
                move.w  14(A6),D5       ;Source Y_Max
                sub.w   10(A6),D5       ;-Source Y_Min
                bmi     nichts_da7

                moveq   #$0F,D5
                move.w  D0,D1           ;Source X_Min
                and.w   D5,D1
                move.w  D4,D3           ;Dest X_Min
                and.w   D5,D3
                lsr.w   #4,D0           ;# des BBs (Source X_Min)
                lsr.w   #4,D4           ;# des BBs (Dest X_Min)
                lsr.w   #4,D2           ;# des BBs (Source X_Max)
                lsr.w   #4,D6           ;# des BBs (Dest X_Max)
                sub.w   D0,D2           ;X_Max-X_Min=Breite in BBs-1 (Source)
                sub.w   D4,D6           ;-'- (Dest)
                move.w  D2,D5           ;Breite des Sourceblocks
                sub.w   D6,D5           ;-Breite des Zielblocks
                andi.w  #1,D5           ;NFSR?
                lsl.w   #2,D5           ;an richtige Position schieben
                sub.w   D3,D1           ;Skew
                move.w  D6,D0           ;Breite des Zielblocks in BBs
                subq.w  #1,D0
                move.w  D0,-6(A6)       ;und merken
                move.w  D2,D7           ;Breite des Sourceblocks in BBs-1
                move.w  D1,D4           ;Skew
                move.w  D1,D2           ;Skew
                tst.w   D4              ;Skew rechts oder links?
                bgt.s   links_rum2      ;ja
                beq.s   weiter2
                neg.w   D4              ;Skew positiv machen
                addq.w  #1,D5           ;und merken
links_rum2:     cmpi.w  #8,D4           ;Skew weniger als 8?
                blt.s   weiter2
                addq.w  #2,D5           ;und merken, besser andersrum shiften(!)
                neg.w   D4              ;Skew umdrehen, also
                addi.w  #16,D4          ;von 16 abziehen
weiter2:        move.w  12(A6),D0       ;Source X_Max
                move.w  14(A6),D1       ;Source Y_Max
                bsr     calc_quelle     ;von hinten reinkommen
                lsl.w   #3,D7
                sub.w   D3,D7           ;Breite einer Zeile in Bytes abziehen(?)
                move.w  D7,-8(A6)       ;Offsets am Zeilenende merken
                move.w  -2(A6),D0       ;Dest X_Max
                move.w  -4(A6),D1       ;Dest Y_Max
                bsr     calc_ziel       ;auch von hinten rein
                lsl.w   #3,D6           ;Breite in BBs->Breite in Bytes
                sub.w   D3,D6           ;Breite einer Zeile in Bytes abziehen(?)
                move.w  D6,-10(A6)      ;Offset am Zeilenende merken
                bsr     get_masken      ;Masken->D6
                swap    D6              ;Masken vertauschen
                asl.w   #3,D5           ;je nach Konstellation richtige Routine
                movea.l L0009t(PC,D5.w),A3
                movea.l L0008t(PC,D5.w),A4
                tst.w   -6(A6)          ;Breite des Zielblocks in BBs>=2?
                bgt     start_trans     ;ja
                blt.s   klein2
                bclr    #3,D5           ;Skew nach rechts?
                beq     start_trans     ;Nein, dann okay
                lsr.w   #2,D5           ;nur noch mal 4
                movea.l rechts_kleint(PC,D5.w),A4 ;Spezialroutine
                bra.s   start_trans

;Tabelle für Objekte, die genau 2 BBs breit sind, und nach rechts geschoben
;werden, da es für die nachfolgenden Routine (L0037) nicht möglich war an
;bestimmte Informationen heranzukommen.
rechts_kleint:  DC.L L0023bt
                DC.L L0025bt
                DC.L L0024bt
                DC.L L0026bt

;Routinen für nur einen BB breite Objekte:
L0007t:         DC.L L0021t
                DC.L L0022t
                DC.L L0022t
                DC.L L0021t

;Routinen für mindestens 2 BB breite Objekte
L0008t:         DC.L L0026t     ;4 . = 0
L0009t:         DC.L L0030t
                DC.L L0023t     ;5 . = 1 -
                DC.L L0033t
                DC.L L0024t     ;6 . = 2
                DC.L L002Ft
                DC.L L0025t     ;7 . = 3 -
                DC.L L0034t
                DC.L L0025t     ;12  = 4
                DC.L L0030t
                DC.L L0024t     ;13  = 5 -
                DC.L L0033t
                DC.L L0023t     ;14  = 6
                DC.L L002Ft
                DC.L L0026t     ;15  = 7 -
                DC.L L0034t

klein2:         move.l  D6,D0           ;Maske
                swap    D0
                and.w   D0,D6           ;linker und rechter Rand verknüpfen
                lea     nxt_scanline(PC),A3
                btst    #5,D5           ;NFSR?
                bne.s   start_trans     ;Ja->Normal arbeiten
                lsr.w   #1,D5           ;nur noch mal 4
                andi.w  #%0000000000001100,D5
                movea.l L0007t(PC,D5.w),A4 ;Sonst andere Routine nehmen
start_trans:    move.w  14(A6),D5       ;y2
                sub.w   10(A6),D5       ;-y1=Höhe in Pixeln-1
                addq.w  #1,D5           ;=Höhe in Pixeln
                swap    D5
                move.w  -6(A6),D5       ;Breite des Zielblocks in BBs-2
                addq.l  #8,A0           ;Ans Ende des BB gehen
                addq.l  #8,A1           ;-'-
                tst.w   D4              ;Skew=0?
                beq.s   no_shift2       ;ja, dann Spezial-Routine nehmen
                jmp     (A4)            ;und Plane bearbeiten

;Spezialroutine falls Skew=0:
no_shift2:      lea     no_shift3(PC),A4 ;Bei nächster Scanline diese Routine
                move.l  D6,D0           ;Maske retten
                swap    D6
                move.w  D6,D7           ;obere Maske->D7
                swap    D7
                move.w  D6,D7           ;->D7
                move.w  D0,D6           ;untere Maske->D6

no_shift3:      move.l  -(A0),D0        ;Plane 2 und 3 holen (12)
                move.l  -(A0),D1        ;Plane 0 und 1 holen (12)
                move.l  D0,D4           ;für Maske      ( 4)
                or.l    D1,D4           ;               ( 8)
                swap    D0              ;               ( 4)
                swap    D1
                or.l    D0,D4           ;Maske in d4    ( 8)
                or.l    D1,D4
                swap    D0
                swap    D1
                and.l   D6,D4           ;maske_links
                move.l  -(A1),D2        ;Screen holen   (12)
                move.l  -(A1),D3
                eor.l   D2,D0
                and.l   D4,D0
                eor.l   D2,D0
                eor.l   D3,D1
                and.l   D4,D1
                eor.l   D3,D1
                move.l  D0,4(A1)
                move.l  D1,(A1)
                tst.w   D5              ;nur ein BB?
                bmi     nxt_scan2       ;ja, -> Weiter
                bra.s   bb_dbra

bb_mitte:       move.l  -(A0),D0        ;Plane 2 und 3 holen (12)
                move.l  -(A0),D1        ;Plane 0 und 1 holen (12)
                move.l  D0,D4           ;für Maske      ( 4)
                or.l    D1,D4           ;               ( 8)
                swap    D0              ;               ( 4)
                swap    D1
                or.l    D0,D4           ;Maske in d4    ( 8)
                or.l    D1,D4
                swap    D0
                swap    D1
                not.l   D4              ;Maske invertieren
                beq.s   klopfen2
                move.l  -4(A1),D2       ;Screen holen   (12)
                move.l  -8(A1),D3
                and.l   D4,D2           ;               ( 8)
                or.l    D2,D0           ;               ( 8)
                and.l   D4,D3           ;               ( 8)
                or.l    D3,D1           ;               ( 8)
klopfen2:       move.l  D0,-(A1)
                move.l  D1,-(A1)
bb_dbra:        dbra    D5,bb_mitte     ;zählt bis auf 0 runter

bb_rechts:      move.l  -(A0),D0        ;Plane 2 und 3 holen (12)
                move.l  -(A0),D1        ;Plane 0 und 1 holen (12)
                move.l  D0,D4           ;für Maske      ( 4)
                or.l    D1,D4           ;               ( 8)
                swap    D0              ;               ( 4)
                swap    D1
                or.l    D0,D4           ;Maske in d4    ( 8)
                or.l    D1,D4
                swap    D0
                swap    D1
                and.l   D7,D4           ;maske_rechts
                move.l  -(A1),D2        ;Screen holen   (12)
                move.l  -(A1),D3
                eor.l   D2,D0
                and.l   D4,D0
                eor.l   D2,D0
                eor.l   D3,D1
                and.l   D4,D1
                eor.l   D3,D1
                move.l  D0,4(A1)
                move.l  D1,(A1)
                bra     nxt_scan2

L0021t:         move.w  -(A0),D7
                move.w  -(A0),D3
                move.w  -(A0),D2
                move.w  -(A0),D1
                rol.w   D4,D7
                rol.w   D4,D3
                rol.w   D4,D2
                rol.w   D4,D1

                movea.l D1,A5           ;d1 retten
                or.w    D2,D1
                or.w    D3,D1           ;Maske berechnen
                or.w    D7,D1

                move.w  -(A1),D0
                eor.w   D0,D7
                and.w   D6,D7
                and.w   D1,D7
                eor.w   D0,D7
                move.w  D7,(A1)

                move.w  -(A1),D0
                eor.w   D0,D3
                and.w   D6,D3
                and.w   D1,D3
                eor.w   D0,D3
                move.w  D3,(A1)

                move.w  -(A1),D0
                eor.w   D0,D2
                and.w   D6,D2
                and.w   D1,D2
                eor.w   D0,D2
                move.w  D2,(A1)

                move.w  A5,D7           ;d1 holen

                move.w  -(A1),D0
                eor.w   D0,D7
                and.w   D6,D7
                and.w   D1,D7
                eor.w   D0,D7
                move.w  D7,(A1)
                jmp     (A3)

L0022t:         move.w  -(A0),D7
                move.w  -(A0),D3
                move.w  -(A0),D2
                move.w  -(A0),D1
                ror.w   D4,D7
                ror.w   D4,D3
                ror.w   D4,D2
                ror.w   D4,D1

                movea.l D1,A5           ;d1 retten
                or.w    D2,D1
                or.w    D3,D1
                or.w    D7,D1

                move.w  -(A1),D0
                eor.w   D0,D7
                and.w   D6,D7
                and.w   D1,D7
                eor.w   D0,D7
                move.w  D7,(A1)

                move.w  -(A1),D0
                eor.w   D0,D3
                and.w   D6,D3
                and.w   D1,D3
                eor.w   D0,D3
                move.w  D3,(A1)

                move.w  -(A1),D0
                eor.w   D0,D2
                and.w   D6,D2
                and.w   D1,D2
                eor.w   D0,D2
                move.w  D2,(A1)

                move.w  A5,D7           ;d1 holen
                move.w  -(A1),D0
                eor.w   D0,D7
                and.w   D6,D7
                and.w   D1,D7
                eor.w   D0,D7
                move.w  D7,(A1)
                jmp     (A3)

L0023t:         move.w  -(A0),D7        ;Plane aus Objekt holen
                move.w  -(A0),D3
                move.w  -(A0),D2
                move.w  -(A0),D1
                swap    D7
                swap    D3
                swap    D2
                swap    D1
L0024t:         move.w  -(A0),D7        ;nächste Plane aus Objekt
                move.w  -(A0),D3
                move.w  -(A0),D2
                move.w  -(A0),D1
                swap    D7
                swap    D3
                swap    D2
                swap    D1

                movea.l D1,A5           ;d1 retten
                or.l    D2,D1
                or.l    D3,D1           ;Maske berechnen
                or.l    D7,D1

                move.l  D7,D0           ;ins Schieberegister
                ror.l   D4,D0           ;runterschieben
                ror.l   D4,D1           ;Maske schieben
;                not.w   D1              ;Maske invertieren
                move.w  -(A1),D7        ;Screen holen
                eor.w   D7,D0
                and.w   D6,D0           ;ausblenden
                and.w   D1,D0           ;Maskieren
                eor.w   D7,D0
                move.w  D0,(A1)         ;und zurück auf Screen

                move.l  D3,D0           ;ins Schieberegister
                ror.l   D4,D0           ;runterschieben
                move.w  -(A1),D3        ;Screen holen
                eor.w   D3,D0
                and.w   D6,D0
                and.w   D1,D0           ;Maskieren
                eor.w   D3,D0
                move.w  D0,(A1)         ;und zurück auf Screen

                move.l  D2,D0           ;ins Schieberegister
                ror.l   D4,D0           ;runterschieben
                move.w  -(A1),D2        ;Screen holen
                eor.w   D2,D0
                and.w   D6,D0
                and.w   D1,D0
                eor.w   D2,D0
                move.w  D0,(A1)         ;und zurück auf Screen

                move.l  A5,D0           ;ins Schieberegister
                ror.l   D4,D0           ;runterschieben
                move.w  -(A1),D2        ;Screen holen
                eor.w   D2,D0
                and.w   D6,D0
                and.w   D1,D0
                eor.w   D2,D0
                move.w  D0,(A1)         ;und zurück auf Screen

                move.l  A5,D1           ;und richtiges d1 rausholen

                jmp     (A3)

L0025t:         move.w  -(A0),D7        ;Plane aus Objekt holen
                move.w  -(A0),D3
                move.w  -(A0),D2
                move.w  -(A0),D1
                swap    D7
                swap    D3
                swap    D2
                swap    D1
L0026t:         move.w  -(A0),D7
                move.w  -(A0),D3
                move.w  -(A0),D2
                move.w  -(A0),D1

                movea.l D1,A5           ;d1 retten
                or.l    D2,D1
                or.l    D3,D1           ;Maske berechnen
                or.l    D7,D1

                move.l  D7,D0           ;ins Schieberegister
                swap    D7
                rol.l   D4,D0           ;schieben
                rol.l   D4,D1           ;Maske hinschieben
;                not.w   D1              ;Maske invertieren
                move.w  -(A1),D7        ;Screen holen
                eor.w   D7,D0
                and.w   D6,D0
                and.w   D1,D0
                eor.w   D7,D0
                move.w  D0,(A1)

                move.l  D3,D0           ;ins Schieberegister
                swap    D3
                rol.l   D4,D0           ;schieben
                move.w  -(A1),D3        ;Screen holen
                eor.w   D3,D0
                and.w   D6,D0
                and.w   D1,D0
                eor.w   D3,D0
                move.w  D0,(A1)

                move.l  D2,D0           ;ins Schieberegister
                swap    D2
                rol.l   D4,D0           ;schieben
                move.w  -(A1),D2        ;Screen holen
                eor.w   D2,D0
                and.w   D6,D0
                and.w   D1,D0
                eor.w   D2,D0
                move.w  D0,(A1)

                move.l  A5,D0           ;ins Schieberegister
                rol.l   D4,D0           ;schieben
                move.w  -(A1),D2        ;Screen holen
                eor.w   D2,D0
                and.w   D6,D0
                and.w   D1,D0
                eor.w   D2,D0
                move.w  D0,(A1)

                move.l  A5,D1           ;d1 rausholen
                swap    D1              ;und auch noch umdrehen
                jmp     (A3)

L0023bt:        move.w  -(A0),D7
                move.w  -(A0),D3
                move.w  -(A0),D2
                move.w  -(A0),D1
                swap    D7
                swap    D3
                swap    D2
                swap    D1
L0024bt:        move.w  -(A0),D7
                move.w  -(A0),D3
                move.w  -(A0),D2

                move.w  -(A0),D1
                swap    D7
                swap    D3
                swap    D2
                swap    D1
                ror.l   D4,D7
                ror.l   D4,D3
                ror.l   D4,D2
                ror.l   D4,D1

                movea.l D1,A5           ;d1 retten
                or.w    D2,D1
                or.w    D3,D1           ;Maske berechnen
                or.w    D7,D1
;               not.w   D1              ;und invertieren

                move.w  -(A1),D0
                eor.w   D0,D7
                and.w   D6,D7
                and.w   D1,D7
                eor.w   D0,D7
                move.w  D7,(A1)

                move.w  -(A1),D0
                eor.w   D0,D3
                and.w   D6,D3
                and.w   D1,D3
                eor.w   D0,D3
                move.w  D3,(A1)

                move.w  -(A1),D0
                eor.w   D0,D2
                and.w   D6,D2
                and.w   D1,D2
                eor.w   D0,D2
                move.w  D2,(A1)

                move.w  A5,D2           ;altes d1 holen

                move.w  -(A1),D0
                eor.w   D0,D2
                and.w   D6,D2
                and.w   D1,D2
                eor.w   D0,D2
                move.w  D2,(A1)

                move.l  A5,D1           ;und nochmal her damit

                bra     L0037t          ;und dort geht's weiter

L0025bt:        move.w  -(A0),D7
                move.w  -(A0),D3
                move.w  -(A0),D2
                move.w  -(A0),D1
                swap    D7
                swap    D3
                swap    D2
                swap    D1
L0026bt:        move.w  -(A0),D7
                move.w  -(A0),D3
                move.w  -(A0),D2
                move.w  -(A0),D1
                rol.l   D4,D7
                rol.l   D4,D3
                rol.l   D4,D2
                rol.l   D4,D1

                movea.l D1,A5           ;d1 retten
                or.w    D2,D1
                or.w    D3,D1           ;Maske berechnen
                or.w    D7,D1
;              not.w   D1              ;und invertieren

                move.w  -(A1),D0
                eor.w   D0,D7
                and.w   D6,D7
                and.w   D1,D7
                eor.w   D0,D7
                move.w  D7,(A1)

                move.w  -(A1),D0
                eor.w   D0,D3
                and.w   D6,D3
                and.w   D1,D3
                eor.w   D0,D3
                move.w  D3,(A1)

                move.w  -(A1),D0
                eor.w   D0,D2
                and.w   D6,D2
                and.w   D1,D2
                eor.w   D0,D2
                move.w  D2,(A1)

                move.w  A5,D2
                move.w  -(A1),D0
                eor.w   D0,D2
                and.w   D6,D2
                and.w   D1,D2
                eor.w   D0,D2
                move.w  D2,(A1)

                bra     L0037t          ;dort geht's weiter

L002Bt:         move.w  -(A0),D7
                move.w  -(A0),D3
                move.w  -(A0),D2
                move.w  -(A0),D1
                swap    D7
                swap    D3
                swap    D2
                swap    D1

                movea.l D1,A5           ;d1 retten
                or.l    D2,D1
                or.l    D3,D1
                or.l    D7,D1

                move.l  D7,D0
                ror.l   D4,D0
                ror.l   D4,D1           ;Maske auch schieben
                not.w   D1              ;Maske invertieren
                and.w   D1,-(A1)
                or.w    D0,(A1)

                move.l  D3,D0
                ror.l   D4,D0
                and.w   D1,-(A1)
                or.w    D0,(A1)

                move.l  D2,D0
                ror.l   D4,D0
                and.w   D1,-(A1)
                or.w    D0,(A1)

                move.l  A5,D0
                ror.l   D4,D0
                and.w   D1,-(A1)
                or.w    D0,(A1)

                move.l  A5,D1           ;und altes d1 rausholen
                jmp     (A3)

L002Ct:         move.w  -(A0),D7
                move.w  -(A0),D3
                move.w  -(A0),D2
                move.w  -(A0),D1

                movea.l D1,A5           ;d1 retten
                or.l    D2,D1
                or.l    D3,D1           ;Maske berechnen
                or.l    D7,D1

                move.l  D7,D0           ;ins Schieberegister
                swap    D7
                rol.l   D4,D0
                rol.l   D4,D1           ;Maske auch schieben
                not.w   D1
                and.w   D1,-(A1)        ;Maskieren
                or.w    D0,(A1)         ;und einknüpfen

                move.l  D3,D0           ;ins Schieberegister
                swap    D3
                rol.l   D4,D0
                and.w   D1,-(A1)
                or.w    D0,(A1)

                move.l  D2,D0           ;ins Schieberegister
                swap    D2
                rol.l   D4,D0
                and.w   D1,-(A1)
                or.w    D0,(A1)

                move.l  A5,D0           ;ins Schieberegister
                rol.l   D4,D0
                and.w   D1,-(A1)
                or.w    D0,(A1)

                move.l  A5,D1           ;altes d1 holen
                swap    D1              ;und auch umdrehen
                jmp     (A3)

L002Ft:         dbra    D5,L002Bt

                move.w  -(A0),D7
                move.w  -(A0),D3
                move.w  -(A0),D2
                move.w  -(A0),D1
                ror.l   D4,D7
                ror.l   D4,D3
                ror.l   D4,D2
                ror.l   D4,D1
                bra     L0037t

L0030t:         dbra    D5,L002Ct

                move.w  -(A0),D7
                move.w  -(A0),D3
                move.w  -(A0),D2
                move.w  -(A0),D1
                rol.l   D4,D7
                rol.l   D4,D3
                rol.l   D4,D2
                rol.l   D4,D1
                bra     L0038t

L0033t:         cmpi.w  #1,D5           ;Nur bis 0 runterzählen
                beq.s   spezialt        ;bei eins Spezialroutine
                dbra    D5,L002Bt
                bra.s   L0037t

spezialt:       move.w  -(A0),D7
                move.w  -(A0),D3
                move.w  -(A0),D2
                move.w  -(A0),D1
                swap    D7
                swap    D3
                swap    D2
                swap    D1
                ror.l   D4,D7
                ror.l   D4,D3
                ror.l   D4,D2
                ror.l   D4,D1

                movea.l D1,A5           ;d1 merken
                or.w    D2,D1
                or.w    D3,D1           ;Maske bilden
                or.w    D7,D1
                not.w   D1

                and.w   D1,-(A1)
                or.w    D7,(A1)
                and.w   D1,-(A1)
                or.w    D3,(A1)
                and.w   D1,-(A1)
                or.w    D2,(A1)
                and.w   D1,-(A1)
                move.l  A5,D1           ;Altes D1 holen
                or.w    D1,(A1)

                bra.s   L0037t

L0034t:         cmpi.w  #1,D5
                beq.s   spezial2t
                dbra    D5,L002Ct
                bra.s   L0037t

spezial2t:
                move.w  -(A0),D7
                move.w  -(A0),D3
                move.w  -(A0),D2
                move.w  -(A0),D1
                rol.l   D4,D7
                rol.l   D4,D3
                rol.l   D4,D2
                rol.l   D4,D1

                movea.l D1,A5           ;d1 retten
                or.w    D2,D1
                or.w    D3,D1           ;Maske bilden
                or.w    D7,D1
                not.w   D1

                and.w   D1,-(A1)
                or.w    D7,(A1)
                and.w   D1,-(A1)
                or.w    D3,(A1)
                and.w   D1,-(A1)
                or.w    D2,(A1)
                and.w   D1,-(A1)
                move.l  A5,D1
                or.w    D1,(A1)

L0037t:         swap    D7
                swap    D3
                swap    D2
                swap    D1
L0038t:         swap    D6

                movea.l D1,A5           ;d1 retten
                or.w    D2,D1
                or.w    D3,D1
                or.w    D7,D1           ;Maske berechnen
;                not.w   D1

                move.w  -(A1),D0        ;Screen holen
                eor.w   D0,D7
                and.w   D6,D7
                and.w   D1,D7
                eor.w   D0,D7
                move.w  D7,(A1)         ;und zurück auf Screen

                move.w  -(A1),D0        ;Screen holen
                eor.w   D0,D3
                and.w   D6,D3
                and.w   D1,D3
                eor.w   D0,D3
                move.w  D3,(A1)         ;und zurück auf Screen

                move.w  -(A1),D0        ;Screen holen
                eor.w   D0,D2
                and.w   D6,D2
                and.w   D1,D2
                eor.w   D0,D2
                move.w  D2,(A1)         ;und zurück auf Screen

                move.w  A5,D2           ;und altes d1 holen

                move.w  -(A1),D0        ;Screen holen
                eor.w   D0,D2
                and.w   D6,D2
                and.w   D1,D2           ;Maske
                eor.w   D0,D2
                move.w  D2,(A1)         ;und zurück auf Screen
                bra     L0039           ;und nächste Scanline


;***************************************************************************
; Unpacking source for Pack-Ice Version 2.31
;-> 12(A6).L: Zeiger auf den Zieldatenbereich
;->  8(A6).L: Zeiger auf die gepackten Daten
mcode52:        movem.l A0-A1,-(SP)

                movea.l 12(SP),A0       ;Adresse der gepackten Daten
                movea.l 16(SP),A1       ;Zieladresse
                cmpa.l  A0,A1
                beq.s   ice_decrunch_2  ;Ja->Andere Routine nehmen

                bsr.s   decrunch
                bra.s   raus_hier
ice_decrunch_2: bsr     decrunch2
raus_hier:      movem.l (SP)+,A0-A1
                rts

decrunch:       movem.l D0-A6,-(SP)
                bsr.s   ice_unpa4
                cmpi.l  #$49434521,D0
                bne.s   ice_unpa3
                bsr.s   ice_unpa4
                lea     -8(A0,D0.l),A5
                bsr.s   ice_unpa4
                move.l  D0,(SP)
                movea.l A1,A4
                movea.l A1,A6
                adda.l  D0,A6
                movea.l A6,A3
                move.b  -(A5),D7
                bsr.s   ice_unpa6
                bsr.s   ice_unpa10
                bcc.s   ice_unpa3
                move.w  #$0F9F,D7
ice_unpa:       moveq   #3,D6
ice_unpa1:      move.w  -(A3),D4
                moveq   #3,D5
ice_unpa2:      add.w   D4,D4
                addx.w  D0,D0
                add.w   D4,D4
                addx.w  D1,D1
                add.w   D4,D4
                addx.w  D2,D2
                add.w   D4,D4
                addx.w  D3,D3
                dbra    D5,ice_unpa2
                dbra    D6,ice_unpa1
                movem.w D0-D3,(A3)
                dbra    D7,ice_unpa
ice_unpa3:      movem.l (SP)+,D0-A6
                rts
ice_unpa4:      moveq   #3,D1
ice_unpa5:      lsl.l   #8,D0
                move.b  (A0)+,D0
                dbra    D1,ice_unpa5
                rts
ice_unpa6:      bsr.s   ice_unpa10
                bcc.s   ice_unpa9
                moveq   #0,D1
                bsr.s   ice_unpa10
                bcc.s   ice_unpa8
                lea     ice_unpa23(PC),A1
                moveq   #4,D3
ice_unpa7:      move.l  -(A1),D0
                bsr.s   ice_unpa12
                swap    D0
                cmp.w   D0,D1
                dbne    D3,ice_unpa7
                add.l   20(A1),D1
ice_unpa8:      move.b  -(A5),-(A6)
                dbra    D1,ice_unpa8
ice_unpa9:      cmpa.l  A4,A6
                bgt.s   ice_unpa15
                rts
ice_unpa10:     add.b   D7,D7
                bne.s   ice_unpa11
                move.b  -(A5),D7
                addx.b  D7,D7
ice_unpa11:     rts
ice_unpa12:     moveq   #0,D1
ice_unpa13:     add.b   D7,D7
                bne.s   ice_unpa14
                move.b  -(A5),D7
                addx.b  D7,D7
ice_unpa14:     addx.w  D1,D1
                dbra    D0,ice_unpa13
                rts
ice_unpa15:     lea     ice_unpa24(PC),A1
                moveq   #3,D2
ice_unpa16:     bsr.s   ice_unpa10
                dbcc    D2,ice_unpa16
                moveq   #0,D4
                moveq   #0,D1
                move.b  1(A1,D2.w),D0
                ext.w   D0
                bmi.s   ice_unpa17
                bsr.s   ice_unpa12
ice_unpa17:     move.b  6(A1,D2.w),D4
                add.w   D1,D4
                beq.s   ice_unpa19
                lea     ice_unpa25(PC),A1
                moveq   #1,D2
ice_unpa18:     bsr.s   ice_unpa10
                dbcc    D2,ice_unpa18
                moveq   #0,D1
                move.b  1(A1,D2.w),D0
                ext.w   D0
                bsr.s   ice_unpa12
                add.w   D2,D2
                add.w   6(A1,D2.w),D1
                bpl.s   ice_unpa21
                sub.w   D4,D1
                bra.s   ice_unpa21
ice_unpa19:     moveq   #0,D1
                moveq   #5,D0
                moveq   #-1,D2
                bsr.s   ice_unpa10
                bcc.s   ice_unpa20
                moveq   #8,D0
                moveq   #$3F,D2
ice_unpa20:     bsr.s   ice_unpa12
                add.w   D2,D1
ice_unpa21:     lea     2(A6,D4.w),A1
                adda.w  D1,A1
                move.b  -(A1),-(A6)
ice_unpa22:     move.b  -(A1),-(A6)
                dbra    D4,ice_unpa22
                bra     ice_unpa6
                DC.B $7F,$FF,$00,$0E,$00,$FF,$00,$07
                DC.B $00,$07,$00,$02,$00,$03,$00,$01
                DC.B $00,$03,$00,$01
ice_unpa23:     DC.B $00,$00,$01,$0D,$00,$00,$00,$0E
                DC.B $00,$00,$00,$07,$00,$00,$00,$04
                DC.B $00,$00,$00,$01
ice_unpa24:     DC.B $09,$01,$00,$FF,$FF,$08,$04,$02
                DC.B $01,$00
ice_unpa25:     DC.B $0B,$04,$07,$00,$01,$1F,$FF,$FF
                DC.B $00,$1F
ice_unpa26:


;Entpackt auf sich selbst:
decrunch2:      link    A3,#-$0078
                movem.l D0-A6,-(SP)
                lea     120(A0),A4
                movea.l A4,A6
                bsr.s   decrunch2_9
                cmpi.l  #$49434521,D0
                bne.s   decrunch2_8
                bsr.s   decrunch2_9
                lea     -8(A0,D0.l),A5
                bsr.s   decrunch2_9
                move.l  D0,(SP)
                adda.l  D0,A6
                movea.l A6,A1
                moveq   #$77,D0
decrunch2_1:    move.b  -(A1),-(A3)
                dbra    D0,decrunch2_1
                movea.l A6,A3
                move.b  -(A5),D7
                bsr.s   decrunch2_11
                movea.l A3,A5
                bsr.s   decrunch2_15
                bcc.s   decrunch2_5
                move.w  #$0F9F,D7
decrunch2_2:    moveq   #3,D6
decrunch2_3:    move.w  -(A3),D4
                moveq   #3,D5
decrunch2_4:    add.w   D4,D4
                addx.w  D0,D0
                add.w   D4,D4
                addx.w  D1,D1
                add.w   D4,D4
                addx.w  D2,D2
                add.w   D4,D4
                addx.w  D3,D3
                dbra    D5,decrunch2_4
                dbra    D6,decrunch2_3
                movem.w D0-D3,(A3)
                dbra    D7,decrunch2_2
decrunch2_5:    movem.l (SP),D0-A3
decrunch2_6:    move.b  (A4)+,(A0)+
                subq.l  #1,D0
                bne.s   decrunch2_6
                moveq   #$77,D0
decrunch2_7:    move.b  -(A3),-(A5)
                dbra    D0,decrunch2_7
decrunch2_8:    movem.l (SP)+,D0-A6
                unlk    A3
                rts
decrunch2_9:    moveq   #3,D1
decrunch2_10:   lsl.l   #8,D0
                move.b  (A0)+,D0
                dbra    D1,decrunch2_10
                rts
decrunch2_11:   bsr.s   decrunch2_15
                bcc.s   decrunch2_14
                moveq   #0,D1
                bsr.s   decrunch2_15
                bcc.s   decrunch2_13
                lea     decrunch2_28(PC),A1
                moveq   #4,D3
decrunch2_12:   move.l  -(A1),D0
                bsr.s   decrunch2_17
                swap    D0
                cmp.w   D0,D1
                dbne    D3,decrunch2_12
                add.l   20(A1),D1
decrunch2_13:   move.b  -(A5),-(A6)
                dbra    D1,decrunch2_13
decrunch2_14:   cmpa.l  A4,A6
                bgt.s   decrunch2_20
                rts
decrunch2_15:   add.b   D7,D7
                bne.s   decrunch2_16
                move.b  -(A5),D7
                addx.b  D7,D7
decrunch2_16:   rts
decrunch2_17:   moveq   #0,D1
decrunch2_18:   add.b   D7,D7
                bne.s   decrunch2_19
                move.b  -(A5),D7
                addx.b  D7,D7
decrunch2_19:   addx.w  D1,D1
                dbra    D0,decrunch2_18
                rts
decrunch2_20:   lea     decrunch2_29(PC),A1
                moveq   #3,D2
decrunch2_21:   bsr.s   decrunch2_15
                dbcc    D2,decrunch2_21
                moveq   #0,D4
                moveq   #0,D1
                move.b  1(A1,D2.w),D0
                ext.w   D0
                bmi.s   decrunch2_22
                bsr.s   decrunch2_17
decrunch2_22:   move.b  6(A1,D2.w),D4
                add.w   D1,D4
                beq.s   decrunch2_24
                lea     decrunch2_30(PC),A1
                moveq   #1,D2
decrunch2_23:   bsr.s   decrunch2_15
                dbcc    D2,decrunch2_23
                moveq   #0,D1
                move.b  1(A1,D2.w),D0
                ext.w   D0
                bsr.s   decrunch2_17
                add.w   D2,D2
                add.w   6(A1,D2.w),D1
                bpl.s   decrunch2_26
                sub.w   D4,D1
                bra.s   decrunch2_26
decrunch2_24:   moveq   #0,D1
                moveq   #5,D0
                moveq   #-1,D2
                bsr.s   decrunch2_15
                bcc.s   decrunch2_25
                moveq   #8,D0
                moveq   #$3F,D2
decrunch2_25:   bsr.s   decrunch2_17
                add.w   D2,D1
decrunch2_26:   lea     2(A6,D4.w),A1
                adda.w  D1,A1
                move.b  -(A1),-(A6)
decrunch2_27:   move.b  -(A1),-(A6)
                dbra    D4,decrunch2_27
                bra     decrunch2_11
                DC.B $7F,$FF,$00,$0E,$00,$FF,$00,$07
                DC.B $00,$07,$00,$02,$00,$03,$00,$01
                DC.B $00,$03,$00,$01
decrunch2_28:   DC.B $00,$00,$01,$0D,$00,$00,$00,$0E
                DC.B $00,$00,$00,$07,$00,$00,$00,$04
                DC.B $00,$00,$00,$01
decrunch2_29:   DC.B $09,$01,$00,$FF,$FF,$08,$04,$02
                DC.B $01,$00
decrunch2_30:   DC.B $0B,$04,$07,$00,$01,$1F,$FF,$FF
                DC.B $00,$1F
decrunch2_31:

;Bestimmt die Farbe eines Pixels auf logbase
;->  8(a6): x
;   10(a6): y
;int get_pixel(x,y);
mcode59:        link    A6,#0
                movem.l D1-D3/A0,-(SP)

                movea.l logbase,A0
                move.w  10(A6),D0
                mulu    #160,D0
                adda.l  D0,A0
                move.w  8(A6),D0        ;x
                lsr.w   #4,D0
                lsl.w   #3,D0           ;BB berechnen
                adda.w  D0,A0
                move.w  8(A6),D0
                andi.w  #%0000000000001111,D0
                moveq   #15,D1
                sub.w   D0,D1
                moveq   #0,D0           ;Hier Farbe aufbauen
                moveq   #3,D3           ;4 Planes
plane_loop7:    ror.b   #1,D0
                move.w  (A0)+,D2
                btst    D1,D2           ;Farbe gesetzt?
                beq.s   naechste_plane
                addq.w  #1,D0
naechste_plane: dbra    D3,plane_loop7
                ror.b   #5,D0           ;an richtige Position schieben
                movem.l (SP)+,D1-D3/A0
                unlk    A6
                rts


;Diese Funktion initialisiert die Musik, als Parameter wird ihr die
;VBL-Routine der Musik übergeben. Diese wird mit 50 Hz angesprungen.
;void init_music((void *)(musik)(),int song_nr); (14)
mcode63:        link    A6,#0
                movem.l D0-A6,-(SP)

                move    #$2700,SR       ;IRQ's sperren

                move.l  8(A6),music_routine

                tst.b   first_music     ;1. Aufruf?
                bne.s   re_init

                st      first_music

                move.l  $00000110.w,old_music
                move.l  #music_irq,$00000110

                bclr    #4,$FFFFFA09    ;Timer D IRQ sperren
                andi.b  #%11110000,$FFFFFA1D ;Timer D Stop1
                move.b  #246,$FFFFFA25  ;Counterwert eintragen
                bset    #4,$FFFFFA15    ;Timer D nicht maskieren
                move.b  #%11101111,$FFFFFA0D ;Pending Bit löschen
                ori.b   #%00000111,$FFFFFA1D ;Vorteiler teilt durch 200
                bset    #4,$FFFFFA09    ;Timer D erlauben

                bclr    #4,$FFFFFA11    ;In-Service löschen

re_init:        move.w  12(A6),D0
                movea.l music_routine(PC),A0
                jsr     (A0)            ;Und Song einstellen (14)

                move    #$2300,SR       ;IRQ's ermöglichen

                movem.l (SP)+,D0-A6
                unlk    A6
                rts

music_irq:      move    #$2500,SR       ;HBL zulassen

                movem.l D0-A6,-(SP)     ;Register retten

                movea.l music_routine(PC),A0
                jsr     8(A0)           ;In VBL-Routine springen

                movem.l (SP)+,D0-A6
keine_musik:    bclr    #4,$FFFFFA11    ;In-Service Bit löschen
                rte

old_music:      DC.L 0
music_routine:  DC.L 0          ;Adresse der Musikroutine
musik_an:       DC.W -1         ;Musik ist an!

music_off:      move.l  old_music,$00000110.w
                bclr    #4,$FFFFFA09    ;Timer D IRQ sperren
                andi.b  #%11110000,$FFFFFA1D ;Timer D Stop1

                move.l  #$08080000,$FFFF8800.w
                move.l  #$09090000,$FFFF8800.w ;Lautstärken aus
                move.l  #$0A0A0000,$FFFF8800.w
                rts

fnt_buf:
;
;  NEOchrome V2.16 cut buffer contents (left justified):
;  by Chaos, Inc. of the Delta Force (member of The Union)
;
;    pixels/scanline    = $0140 (bytes/scanline: $0028)
;  # scanlines (height) = $0005
;
;  Monochrome mask (1 plane; background=0/non-background=1)
;
                DC.W $0255,$5452,$2250,$0001,$2166,$4727,$2200,$1046
                DC.W $7263,$6773,$5215,$4562,$6263,$7555,$5572,$4400
                DC.W $0008,$0000,$0000,$0000,$0250,$2122,$4122,$0001
                DC.W $5311,$5441,$5522,$2721,$7554,$5444,$5215,$4755
                DC.W $5554,$2555,$5515,$2844,$0814,$0000,$0000,$0000
                DC.W $0205,$5250,$4177,$2702,$5122,$7662,$2300,$4012
                DC.W $7764,$5665,$7216,$4555,$6562,$2555,$2222,$5428
                DC.W $142A,$0000,$0000,$0000,$0005,$5470,$4122,$2004
                DC.W $5141,$1152,$5122,$2720,$7554,$5445,$5255,$4555
                DC.W $4551,$2557,$5240,$2810,$2214,$0000,$0000,$0000
                DC.W $0202,$2150,$2250,$4024,$2176,$1622,$2604,$1042
                DC.W $7563,$6743,$5225,$7552,$4356,$2725,$5270,$1000
                DC.W $0022,$0000,$0000,$0000

music_nr:       DC.W 0
music_adr:      DC.L load_music,game_music

load_music:     IBYTES 'ROM2_LD.IMG'
game_music:     IBYTES 'ROM_II_G.IMG'
                EVEN

picture_tab:    DC.L pic1
                DC.L pic2
                DC.L pic3
                DC.L pic4
                DC.L pic5
                DC.L pic7
                DC.L pic8
                DC.L pic9
                DC.L pic10
                DC.L -1

pic1:           IBYTES 'MEDUSA.PIJ'
pic2:           IBYTES 'MEDUSA.PIA'
pic3:           IBYTES 'MEDUSA.PIB'
pic4:           IBYTES 'MEDUSA.PIG'
pic5:           IBYTES 'MEDUSA.PIH'
pic7:           IBYTES 'MEDUSA.PIC'
pic8:           IBYTES 'MEDUSA.PID'
pic9:           IBYTES 'MEDUSA.PIE'
pic10:          IBYTES 'MEDUSA.PII'

font:           IBYTES 'METALL.OBJ',,42

;Palette of METALL_1.IFF: Saved by NEOchrome V2.25 by Chaos, Inc.
font_pal:       DC.W $0000,$0002,$0013,$0024,$0035,$0046,$0057,$0067
                DC.W $0377,$0577,$0677,$0777,$0000,$0000,$0000,$0700

;Bytefolge zur Initialisierung der absoluten Maus:
abs_maus_an:    DC.B 9,1,63,0,199
mouse_key:
                DC.B 7,3,$10
abs_ms:
set_ms:         DC.B $0E,$00,0,160,0,155
abs_maus_end:

scrolltext:     DC.B '                               '
                DC.B 'The Delta Force proudly present: '
                DC.B 'Return of Medusa (Rings of Medusa Part II)...'
                DC.B "      It's a game with lots of large (!) dungeons,"
                DC.B " with many enemies, hidden features, treasures,"
                DC.B " keys, and many doors to open..."
                DC.B "     It's published through Starbyte and should"
                DC.B " be available at your local software dealer in any"
                DC.B " part of the world (Even in australia). "
                DC.B " Apart from the dungeons is the upper part of the"
                DC.B " world, which is called 'Morenor'. Here you can"
                DC.B " earn money in lotta different ways: You can"
                DC.B " trade with goods, buy ships and sail to"
                DC.B " lost islands, get treasures, buy an army and"
                DC.B " fight against others, play roulette inside the"
                DC.B " casino....."
                DC.B "     But what's the player's quest?"
                DC.B "    You have to open several doors, each well"
                DC.B " protected. Behind the last door is Medusa your"
                DC.B " opponent. You have to fight against her and to"
                DC.B " win the battle (you might remember the ancient"
                DC.B " greece saga of Medusa? Well, maybe it's better"
                DC.B " to be well protected...)."
                DC.B "      But to open these doors is a very difficult"
                DC.B " work, because of many dangers in which the player"
                DC.B " could come...."
                DC.B "      First you have to free a few prisoners, which"
                DC.B " are held by local gangsters. These prisoners will"
                DC.B " help you to fight against Medusa and to find"
                DC.B " the solution how to open the doors...."
                DC.B "      But remember to look at your flashlight; the"
                DC.B " batteries may be empty, eat and drink enough, other"
                DC.B "wise, you will starve..."
                DC.B "       There are a lot of things to discover..."
                DC.B " go and get your personal 'Return of Medusa', it's"
                DC.B " worth the price..."
                DC.B "       This excellent game is sold in"
                DC.B " german, english, french and maybe spain."
                DC.B "         Go for the ultimate quest!"
                DC.B "             signed Chaos, Inc. of Delte Force"
                DC.B "          Wrap Wrap Warp...",0


write_err:      DC.B "WRITE ERROR ON DISK!",0

abs_maus:       DC.B 0          ;0=Relative Maus, 1=Absolute Maus
maus_abfrag:    DC.B -1         ;Maus soll nicht abgefragt werden
track_display:  DC.B 0          ;FALSE
first_music:    DC.B 0          ;TRUE

                EVEN
keypress:       DC.L 0          ;Momentan gedrückte Taste
tastatur_read:  DC.L tastatur_buffer ;Lesepointer im Tastaturbuffer
tastatur_buffer:DS.B 50
old_118:        DS.L 1
old_timer_b:    DS.L 1
overrun:        DC.B 1          ;Bisher kein Overrun
                EVEN
rasters:        DC.L rasters1
first_pal:      DC.W 0          ;Für Calc-Offsets
line_modus:     DC.W 0          ;Typ der Linie 0=Replace, 1=EOR, 2=Transparent

ende_auswert:   DS.L 1          ;Tastaturroutine

digi_works:     DS.W 1          ;Flag, ob Digisound läuft
anzahl:         DS.W 1

logbase:        DS.L 1          ;log. Bildschirmadresse

fuellmuster:    DS.L 1          ;Adresse des Füllmusters

copyscroller:   DS.W 1
scrollbase:     DS.L 1
scrollbegin:    DS.L 1          ; Für Laufschrift
yscroller:      DS.W 1
delta_x:        DS.W 1
scrollpos:      DS.L 1
scroll_akt:     DS.W 1
scroll_buff:    DS.L 1
font_start:     DS.L 1
char_basis:     DS.L 1

counter:        DS.W 1          ;Für Scrollroutine
adrspalte:      DS.L 1          ;Pointer auf reinzuscrollendes Bild
oldbase:        DS.L 1
neuerscreen:    DS.L 1
timercvec:      DS.L 1
basepage:       DS.L 1
helpram:        DS.L 16
savereg:        DS.L 4          ; Schneller Zwischenspeicher
quicksave:      DS.L 6
steuerzeichen:  DS.W 1          ;Zum Steuern der Bilder des Scrollers (1)

vbl_ct:         DC.L 50000      ;(11) Schon etwas vor setzen

mouseform:      DS.W 1          ;Form der Mouse
mdeltax:        DS.W 1          ;(10)
mdeltay:        DS.W 1          ;(10)

space:          DS.L 3

endsource:      DS.L 1
pic_length:     DS.L 1          ; Bildlänge in Bytes

tos_da:         DC.W 0          ;Flag, ob TOS aktiv oder nicht
disk_drin:      DC.W -10        ;Im Moment ist keine Diskette im Laufwerk

msminy:         DC.W 0

                BSS

old_taste:      DS.W 1          ;ALte Taste

scr1:           DS.L 1
scr2:           DS.L 1
screen:         DS.B 32256+60*160 ;Screen+Border

mousek:         DS.W 1
mousex:         DS.W 1
mousey:         DS.W 1

dregister:      DS.L 15         ;zum Retten der Register
retry:          DS.W 1          ;Retrycounter bei Schreibfehler
disk_status:    DS.W 1          ;FDC-Status nach wait_fertig
sek_per_track:  DS.W 1          ;Wieviel Sektoren pro Track hat diese Disk?
first_sek:      DS.W 1          ;Der erste Sektor des Archivs (logisch)
datei_laenge:   DS.L 1          ;Die Länge des Archivs in Bytes
dend:           DS.L 1          ;DMA-Endadresse
dflag:          DS.B 1          ;DMA-Transfer?
flop_aktiv:     DS.B 1          ;Flag, ob Floppy aktiv ist
                EVEN

errno:          DS.W 1          ;Globale Fehlermeldung für TCFLT.LIB
mem_strt:       DS.L 1          ;Startadresse des Benutzerspeichers
mem_len:        DS.L 1          ;Größe des Benutzerspeichers

_BasPag:        DS.L 1
_PgmSize:       DS.L 1
overlay:        DS.W 1          ;Wird mit Overlay-Technik gearbeitet?

vbl_queue:      DS.L 10         ;Platz für 10 VBL-Einträge

hbl_system:     DS.W anz_raster*(1+16) ;Y-Position+Paletten
rasters1:       DS.W anz_raster*(1+16) ;dargestellte Rasterpalette
rasters2:       DS.W anz_raster*(1+16) ;Aufbaurastertabelle

buff:           DS.B 54000      ;Scrollbuffer

                END
