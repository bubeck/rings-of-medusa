
;MASCHINENSPRACHETEIL von Rings of Medusa II für Atari ST
;(C) 1991 by Till Bubeck, Ziegeleistr. 28, D-7056 Weinstadt
;Tel: 07151-66437 (Wochenende und Semesterferien)
;bzw: 07071-49612 (innerhalb der Woche während des Semesters)

;Sämtliche Änderungen sind in den Kommentaren durchnummeriert (1)-(x)
;folgenden Änderungen wurden durchgeführt:
;(1) Scroller liefert, sofern er ein unbekanntes Zeichen im Scrolltext
;    findet dieses in steuerzeichen ans Hauptprogramm (zur Synchronisation
;    mit Bildern). (14.5.1991)
;(2) Bei der Alertmeldung zum Einlegen von Diskette 'x' kam es vor,
;    daß noch kein Raster auf dem Bildschirm stand, und alle VBL-Farben
;    schwarz waren->man sah die Alertbox nicht.
;    Jetzt wird in diesem Fall die VBL-Palette mit einer entsprechenden
;    Palette geladen. (15.5.1991)
;(3) Scroller um 2 Zeichen (Anführunszeichen an/aus) erweitert
;(4) Neue Funktion bei draw_objekt (nur Palette setzen) (28.5.1991)
;(5) Neue Funktion del_raster() bei set_raster() (28.5.1991)
;(6) Neue Funktion fade_raster() (31.5.1991)
;(7) set_mousepos(x,y)=mcode73. Setzt Maus auf bestimmte Position (20.6.1991)
;(8) mcode31() ersatzlos entfernt (25.6.1991)
;(9) an/abschaltbares Trackdisplay, nur zum Debuggen (6.7.1991)
;(10) rel. Mauskoordinaten in mdeltax/y (9.7.1991)
;(11) vbl_ct wird mit Anfangswert über Null initialisiert (9.7.1991)
;(12) mcode64(), mcode65() speichern jetzt 9 Tracks (9.7.1991)
;(13) mcode74(), mcode75() sind hinzugekommen (28.7.1991)
;(14) mcode63() hat neuen Parameter (29.7.1991)
;(15) mcode40-44 entfernt
;(16) Bei Verbesserung (2) wurde überprüft, ob im hbl_system[0][] die VBL-Palette
;     gesetzt war. Wenn nicht, wurde zum Anzeigen der Alertbox eine neue
;     VBL-Palette gesetzt. Doch leider kam es vor (finale()), daß die VBL-Palette
;     zwar bereits im hbl_system[0][] drin war, aber noch nicht mit show_rasters
;     angezeigt wurde. Deshalb überprüft (16) jetzt die tatsächlich dargestellte
;     VBL-Palette. (5.8.1991)

          .PATH "E:\MEDUSA\MEDUSA_2.COL\MCODE\ATARI\"

digisound           = 82      ;8 Bit Digisound (Tabelle 2)

;Funktionsexporte:
          .XDEF mcode1        ;Char Fast
          .XDEF mcode2        ;Char Out
          .XDEF mcode3        ;Init_hardware
          .XDEF mcode4        ;Char Transparent
          .XDEF mcode7        ;Exit VBL
          .XDEF mcode8        ;Scroll_links
          .XDEF mcode9        ;Scroll_rechts
          .XDEF mcode10       ;Scroll_hoch
          .XDEF mcode11       ;Scroll_runter
          .XDEF mcode12       ;Show_Screen
          .XDEF mcode13       ;Set_Sync
          .XDEF mcode14       ;Get_Sync
          .XDEF mcode15       ;copy_zeilen
          .XDEF mcode16       ;undraw_mouse
          .XDEF mcode17       ;draw_mouse
          .XDEF mcode18       ;init_vbl
          .XDEF mcode19       ;draw_shift
          .XDEF mcode20       ;undraw_sprite
          .XDEF mcode23       ;set_raster
          .XDEF mcode24       ;clear_raster()
          .XDEF mcode25       ;fade_out
          .XDEF mcode26       ;fade_in
          .XDEF mcode27       ;Crw!-Packer
          .XDEF mcode28       ;Crw!-Entpacker
          .XDEF mcode29       ;Set Mouse Form
          .XDEF mcode30       ;get_trafficxy
          .XDEF mcode32       ;Init_Scroller
          .XDEF mcode33       ;doscroller
          .XDEF mcode34       ;hbl_init
          .XDEF mcode35       ;hbl_exit
          .XDEF mcode36       ;Play Digisound
          .XDEF mcode38       ;hippel_sfx
          .XDEF mcode39       ;convert
          .XDEF mcode45       ;rest sperren
          .XDEF mcode46       ;Check protection
          .XDEF mcode47       ;draw_obj
          .XDEF mcode48       ;draw_obj_part
          .XDEF mcode49       ;raster_replace
          .XDEF mcode50       ;raster_transp(arent)
          .XDEF mcode51       ;mirror
          .XDEF mcode52       ;ICE! unpack
          .XDEF mcode53       ;line
          .XDEF mcode54       ;Fill
          .XDEF mcode55       ;Calc_Offsets (für Raster)
          .XDEF mcode56       ;Neue_Raster
          .XDEF mcode57       ;Get_Key
          .XDEF mcode58       ;Line_mode
          .XDEF mcode59       ;get_pixel
          .XDEF mcode60       ;line_pattern
          .XDEF mcode61       ;loaddisk
          .XDEF mcode62       ;Vsync
          .XDEF mcode63       ;init_music
          .XDEF mcode64       ;Save_Data
          .XDEF mcode65       ;Load_Data
          .XDEF mcode66       ;random
          .XDEF mcode67       ;is_key
          .XDEF mcode68       ;floppy_read
          .XDEF mcode69       ;Insert_Disk
          .XDEF mcode70       ;floppy_write
          .XDEF mcode71       ;Objekte ins gerätespezifische Format wandeln
          .XDEF mcode72       ;fade_raster
          .XDEF mcode73       ;set_mouse
          .XDEF mcode74       ;mouse_off
          .XDEF mcode75       ;mouse_on

          .XDEF exit

;Variablenexporte:
          .XDEF protect       ;Track 41 Kopierschutz
          .XDEF game_music    ;Beginn der Musik
          .XDEF chcrypt       ;Hier kann Medusa überprüfen ob gecrypt

          .XDEF vbl_ct        ;Darf *nur* von VBL erhöht werden
          .XDEF keypress      ;Zur Zeit gedrückte Taste (Scancode+ASCII)
          .XDEF shift         ;Status der Shift-Tasten
          .XDEF fnt_buf       ;Zeiger auf Zeichensatz
          .XDEF digi_works    ;Flag, ob Digisound gerade gespielt wird
          .XDEF zeit_che      ;Flag, ob Zeit-Cheat angeschaltet
          .XDEF logbase       ;log. Bildschirmbasis
          .XDEF hbl_system    ;Das Rastersystem
          .XDEF mem_len       ;Länge des User Speichers
          .XDEF mem_strt      ;Startadresse des Usermemory
          .XDEF errno         ;Globale Fehlermeldung (für Bibliothek)
          .XDEF tos_da        ;Flag, ob TOS aktiv ist
          .XDEF musik_an      ;Flag, ob Musik spielen soll
          .XDEF steuerzeichen ;(1) Steuerzeichen innerhalb des Scrollers
          .XDEF track_display ;(9)
          .XDEF mdeltax       ;(10) Mousedeltax
          .XDEF mdeltay       ;(10) Mousedeltay

;Variablenimporte:
          .XREF scr1          ;Basisadresse des 1. Screens
          .XREF scr2          ;Basisadresse des 2. Screens
          .XREF save_1        ;Hier wird Maushintergrund für 1. Screen
          .XREF save_2        ;-'-
          .XREF scrn_1        ;Vergleichsvariable zur Auswahl save_1/2
          .XREF mousex        ;Maus_x Koordinate
          .XREF mousey        ;Maus_y Koordinate
          .XREF mousek        ;Mausknopf
          .XREF msminy        ;Oberste Zeile der Maus
          .XREF hlpbuf        ;Hilfsspeicher
          .XREF pack_buf      ;Hier wird entpackt (Hilfspeicher)
          .XREF trf_buf       ;Hier sind die Traffic-Bufs gespeichert
          .XREF _fpumode      ;Ist Floating Coproz vorhanden?
          .XREF _StkSize      ;Größe des Stacks
          .XREF insert_disk   ;'X' an Position 30: BITTE LEGE DISK 'X' IN LAUFWERK A:
          .XREF insert_game   ;Mitteilung: BITTE LEGE SPIELSTAND...


;Funktionsimporte:
          .XREF alert         ;Stellt Alertbox dar
          .XREF _fpuinit      ;Init-Routine für Coprozessor
          .XREF main          ;Hauptroutine C

REITEN    = 2       ;Nummer des Hippelsound zum Reiten
anz_raster          = 50      ;max. Raster pro Bild
scanlines           equ 151   ;Anzahl der Scanlines beim Scrollen

tastatur_write      = $140    ;Hier wird der Schreibpointer gespeichert


Start0:   lea       Start0-256,a0       ;Zeiger auf Basepage
          movea.l   $18(a0),a1          ;Start des BSS
          move.l    $1c(a0),d7          ;Länge des BSS
          lsr.l     #4,d7
          moveq     #0,d0
clear_bss:          move.l    d0,(a1)+
          move.l    d0,(a1)+
          move.l    d0,(a1)+  ;BSS löschen
          move.l    d0,(a1)+
          dbra      d7,clear_bss

          lea.l     Start0,a0
          cmpa.l    #$5000,a0 ;Läuft Prg unten im Ram?
          ble.s     unten
          move.w    #-1,tos_da          ;Ja, TOS ist da
unten:    tst.w     tos_da
          beq       kein_realloc        ;Nein->Kein Speicher holen

          lea       Start0-256,a3       ;Adresse der Basepage
          move.l    a3,_BasPag          ;Pointer merken
          movea.l   $c(a3),a0 ;Größe Text
          adda.l    $14(a3),a0          ;+Größe DATA
          adda.l    $1c(a3),a0          ;+Größe BSS (including Stack)
          adda.w    #256,a0   ;+Größe Basepage

          move.l    a3,d0     ;Basepagestart
          add.l     a0,d0     ;+Programmlänge
          andi.b    #$fc,d0   ;gerade machen
          movea.l   d0,sp     ;Stack setzen

          move.l    a0,-(sp)  ;Programmlänge
          move.l    a3,-(sp)  ;Startadresse
          move.l    #$4a0000,-(sp)
          trap      #1        ;und zurückgeben
          lea.l     12(sp),sp

          move.l    #-1,-(sp)
          move.w    #$48,-(sp)          ;Wieviel ist noch frei?
          trap      #1
          addq.l    #6,sp
          move.l    d0,mem_len          ;Länge merken

          move.l    d0,-(sp)
          move.w    #$48,-(sp)
          trap      #1        ;Allen Speicher reservieren
          addq.l    #6,sp
          move.l    d0,mem_strt         ;Startadresse merken

          jsr       _fpuinit  ;Ist Floating Point da?

          clr.l     -(sp)
          move.w    #$20,-(sp)          ;in Supervisor wechseln
          trap      #1
          addq.l    #6,sp

to_main:  st        overlay   ;Keine Overlaytechnik
          cmpi.l    #300000,mem_len     ;nur 512 K da?
          bge.s     okay_genug
          sf        overlay   ;Overlaytechnik
okay_genug:         jsr       main

exit:     clr.w     -(sp)
          trap      #1        ;und Tschüß

;Initroutine für Lowmem Lage
kein_realloc:       clr.w     _fpumode  ;Kein Floatingpoint da!

          lea.l     Start0,a3
          lea.l     -256(a0),a3         ;Adresse der Basepage
          move.l    a3,_BasPag          ;Pointer merken
          movea.l   $c(a3),a0 ;Größe Text
          adda.l    $14(a3),a0          ;+Größe DATA
          adda.l    $1c(a3),a0          ;+Größe BSS (including Stack)
          adda.w    #256,a0   ;+Größe Basepage

          move.l    a3,d0     ;Basepagestart
          add.l     a0,d0     ;+Programmlänge
          andi.b    #$fc,d0   ;gerade machen
          movea.l   d0,sp     ;Stack setzen
          move.l    d0,mem_strt         ;Hier beginnt der freie Speicher

          move.l    $148,d1   ;Hier endet der Speicher
          sub.l     d0,d1     ;-Startadresse=Freie Bytes
          move.l    d1,mem_len

          bra       to_main

          .DC.b "Do NOT crack this program, because it's "
          .DC.b "written by Chaos, Inc. of Delta Force (member of the Union)."
          .DC.b " The man behind NEOchrome Master and Syntax-Terror-Demo..."
          .EVEN 

;Schreibt Zeichen in Low-Res auf den Bildschirm an Position X/Y, in der Farbe 15
;C-Definition: char_fast(x1,y1,zeichen)
;int x,y;
;char zeichen;
mcode1:   movem.l   d0-d7/a0-a6,-(sp)   ; Register retten

          andi.w    #$fffc,64(sp)       ;auf gerade 4'er X
          lea.l     fnt_buf(pc),a2      ;Adresse des Zeichensatz

          movea.l   logbase,a0          ;Basis des Bildschirms
          move.w    66(sp),d0 ;Bildschirmzeile
          mulu.w    #160,d0   ;160 Bytes/Bildschirmzeile
          adda.l    d0,a0
          move.w    64(sp),d0
          move.w    d0,d1     ;x sichern
          lsr.w     #4,d0     ;/16
          lsl.w     #3,d0
          adda.w    d0,a0     ;Adresse der 4 Words
          andi.w    #$000c,d1 ;0/4/8/12
          cmpi.w    #4,d1
          beq       po4
          cmpi.w    #8,d1
          beq       po8
          cmpi.w    #12,d1
          beq       po12

oben:     moveq.l   #0,d0
          move.b    69(sp),d0 ;Zeichen holen
          bsr       wandel    ;in internen Zeichensatz
          moveq.l   #4,d6     ;5 Zeilen
          moveq.l   #15,d5    ;zum Aus-anden
zeil_loop:          move.w    d0,d1
          lsr.w     #1,d1
          move.b    0(a2,d1.w),d3
          btst      #0,d0     ;gerades Zeichen?
          beq       gerade    ;ja!
          lsl.b     #4,d3     ;nur die untersten 4 Bit
          bra       zeil_it
gerade:   andi.b    #$f0,d3   ;in die obersten 4 Bit
zeil_it:  moveq.l   #0,d7     ;Bei Plane 0 starten
plane_loop:         and.b     d5,(a0)   ;Hintergrund löschen
          or.b      d3,(a0)   ;Zeichen einodern
          addq.l    #2,a0     ;eine Plane weiter
          and.b     d5,(a0)   ;Hintergrund löschen
          or.b      d3,(a0)   ;Zeichen einodern
          addq.l    #2,a0     ;eine Plane weiter
          and.b     d5,(a0)   ;Hintergrund löschen
          or.b      d3,(a0)   ;Zeichen einodern
          addq.l    #2,a0     ;eine Plane weiter
          and.b     d5,(a0)   ;Hintergrund löschen
          or.b      d3,(a0)   ;Zeichen einodern

          lea.l     154(a0),a0          ;Zeile tiefer
          lea.l     40(a2),a2 ;Zeile tiefer
          dbra      d6,zeil_loop
          bra       ende

po8:      addq.l    #1,a0     ;8 Pixel weiter
          bra       oben

po4:      moveq.l   #0,d0
          move.b    69(sp),d0 ;Zeichen holen
          bsr       wandel    ;in internen Zeichensatz
          moveq.l   #4,d6     ;5 Zeilen
          move.l    #$f0,d5   ;zum Aus-Anden
zeil2:    move.w    d0,d1
          lsr.w     #1,d1
          move.b    0(a2,d1.w),d3
          btst      #0,d0     ;gerades Zeichen?
          beq       gerade2   ;ja!
          andi.b    #$0f,d3   ;nur die untersten 4 Bit
          bra       zeil_it2
gerade2:  lsr.b     #4,d3     ;in die obersten 4 Bit
zeil_it2: moveq.l   #0,d7     ;Bei Plane 0 starten
plane2:   and.b     d5,(a0)
          or.b      d3,(a0)   ;Zeichen einodern
          addq.l    #2,a0     ;eine Plane weiter
          and.b     d5,(a0)
          or.b      d3,(a0)   ;Zeichen einodern
          addq.l    #2,a0     ;eine Plane weiter
          and.b     d5,(a0)
          or.b      d3,(a0)   ;Zeichen einodern
          addq.l    #2,a0     ;eine Plane weiter
          and.b     d5,(a0)
          or.b      d3,(a0)   ;Zeichen einodern

          lea.l     154(a0),a0          ;Zeile tiefer
          lea.l     40(a2),a2 ;Zeile tiefer
          dbra      d6,zeil2
          bra       ende


po12:     addq.l    #1,a0     ;8 Pixel weiter
          bra       po4

wandel:   cmpi.w    #$009a,d0 ;'Ü'
          beq       ueh
          cmpi.w    #$008e,d0 ;'Ä'
          beq       aeh
          cmpi.w    #$0099,d0 ;'Ö'
          beq       oeh
          subi.w    #' ',d0
          rts       

ueh:      move.w    #'#'-' ',d0
          rts       
oeh:      move.w    #'$'-' ',d0
          rts       
aeh:      move.w    #'&'-' ',d0
          rts       

ende:     movem.l   (sp)+,d0-d7/a0-a6
          rts       

;Schreibt Zeichen in Low-Res auf den Bildschirm an Position X/Y
;C-Definition: char_out(farbe,hintergrund,x,y,zeichen)
;int farbe,....
mcode2:   movem.l   d0-d7/a0-a6,-(sp)   ; Register retten

          andi.w    #$fffc,68(sp)       ;auf gerade 4'er X
          lea.l     fnt_buf(pc),a2      ;Adresse des Zeichensatzes

          movea.l   logbase,a0          ;Basis des Bildschirms
          move.w    70(sp),d0 ;Bildschirmzeile
          mulu.w    #160,d0   ;160 Bytes/Bildschirmzeile
          adda.l    d0,a0
          move.w    68(sp),d0 ;x holen
          move.w    d0,d1     ;x sichern
          lsr.w     #4,d0     ;/16
          lsl.w     #3,d0
          adda.w    d0,a0     ;Adresse der 4 Words
          andi.w    #$000c,d1 ;0/4/8/12
          cmpi.w    #4,d1
          beq       po42
          cmpi.w    #8,d1
          beq       po82
          cmpi.w    #12,d1
          beq       po122

oben2:    moveq.l   #0,d0
          move.b    73(sp),d0 ;Zeichen holen
          bsr       wandel    ;in internen Zeichensatz
          moveq.l   #4,d6     ;5 Zeilen
          moveq.l   #15,d5    ;zum Aus-anden
          move.l    #$f0,d4   ;zum Einodern des Hintergrundes
zeil_loop3:         move.w    d0,d1
          lsr.w     #1,d1
          move.b    0(a2,d1.w),d3
          btst      #0,d0     ;gerades Zeichen?
          beq       gerade3   ;ja!
          lsl.b     #4,d3     ;nur die untersten 4 Bit
          bra       zeil_it3
gerade3:  andi.b    #$f0,d3   ;in die obersten 4 Bit
zeil_it3: moveq.l   #0,d7     ;Bei Plane 0 starten
plane_loop3:        btst      d7,67(sp) ;Plane im Hintergrund gesetzt?
          beq       loesch_back3
          or.b      d4,(a0)   ;Hintergrund setzen
          btst      d7,65(sp) ;Schriftfarbe an?
          bne       fertig3   ;auch an, nichts machen
          not.b     d3        ;invertieren
          and.b     d3,(a0)   ;Schrift wieder löschen
          not.b     d3        ;zurück invertieren
          bra       fertig3
loesch_back3:       and.b     d5,(a0)   ;Hintergrund löschen
          btst      d7,65(sp) ;Schriftplane an?
          beq       fertig3   ;auch nicht, fertig
          or.b      d3,(a0)   ;Zeichen einodern

fertig3:  addq.l    #2,a0     ;eine Plane weiter
          addq.w    #1,d7
          cmpi.w    #4,d7
          bne       plane_loop3

          adda.w    #160-8,a0 ;Zeile tiefer
          adda.w    #40,a2    ;Zeile tiefer
          dbra      d6,zeil_loop3
          bra       ende2

po82:     addq.l    #1,a0     ;8 Pixel weiter
          bra       oben2

po42:     moveq.l   #0,d0
          move.b    73(sp),d0 ;Zeichen holen
          bsr       wandel    ;in internen Zeichensatz
          moveq.l   #4,d6     ;5 Zeilen
          move.l    #$f0,d5   ;zum Aus-Anden
          moveq.l   #15,d4    ;zum Setzen des Hintergrund
zeil4:    move.w    d0,d1
          lsr.w     #1,d1
          move.b    0(a2,d1.w),d3
          btst      #0,d0     ;gerades Zeichen?
          beq       gerade4   ;ja!
          andi.b    #$0f,d3   ;nur die untersten 4 Bit
          bra       zeil_it4
gerade4:  lsr.b     #4,d3     ;in die obersten 4 Bit
zeil_it4: moveq.l   #0,d7     ;Bei Plane 0 starten
plane4:   btst      d7,67(sp) ;Plane im Hintergrund gesetzt?
          beq       loesch_back4
          or.b      d4,(a0)   ;Hintergrund setzen
          btst      d7,65(sp) ;Schriftfarbe an?
          bne       fertig4   ;auch an, nichts machen
          not.b     d3        ;invertieren
          and.b     d3,(a0)   ;Schrift wieder löschen
          not.b     d3        ;zurück invertieren
          bra       fertig4
loesch_back4:       and.b     d5,(a0)   ;Hintergrund löschen
          btst      d7,65(sp) ;Schriftplane an?
          beq       fertig4   ;auch nicht, fertig
          or.b      d3,(a0)   ;Zeichen einodern

fertig4:  addq.l    #2,a0     ;eine Plane weiter
          addq.w    #1,d7
          cmpi.w    #4,d7
          bne       plane4

          lea.l     152(a0),a0          ;Zeile tiefer
          lea.l     40(a2),a2 ;Zeile tiefer
          dbra      d6,zeil4
          bra       ende2

po122:    addq.l    #1,a0     ;8 Pixel weiter
          bra       po42

ende2:    movem.l   (sp)+,d0-d7/a0-a6
          rts       


;Schreibt Zeichen auf den Bildschirm (logbase) an Position X/Y.
;Dabei scheint durch die Buchstaben der Hintergrund durch (Transparent)
;void char_trans(int farbe,int x,int y,char zeichen)
;-> 14(a6): zeichen
;   12(a6): y
;   10(a6): x
;    8(a6): Farbe
mcode4:   link      a6,#0
          movem.l   d0-d7/a0-a5,-(sp)   ; Register retten

          andi.w    #$fffc,10(a6)       ;auf gerade 4'er X
          lea.l     fnt_buf(pc),a2      ;Adresse des Zeichensatzes

          movea.l   logbase,a0          ;Basis des Bildschirms
          move.w    12(a6),d0 ;Bildschirmzeile
          mulu.w    #160,d0   ;160 Bytes/Bildschirmzeile
          adda.l    d0,a0
          move.w    10(a6),d0 ;x holen
          move.w    d0,d1     ;x sichern
          lsr.w     #4,d0     ;/16
          lsl.w     #3,d0
          adda.w    d0,a0     ;Adresse der 4 Words
          andi.w    #$000c,d1 ;0/4/8/12
          cmpi.w    #4,d1
          beq       po43
          cmpi.w    #8,d1
          beq       po83
          cmpi.w    #12,d1
          beq       po123

oben3:    moveq.l   #0,d0
          move.b    15(a6),d0 ;Zeichen holen
          bsr       wandel    ;in internen Zeichensatz
          moveq.l   #4,d6     ;5 Zeilen
          moveq.l   #15,d5    ;zum Aus-anden
          move.l    #$f0,d4   ;zum Einodern des Hintergrundes
zeil_loop33:        move.w    d0,d1
          lsr.w     #1,d1     ;halbieren
          move.b    0(a2,d1.w),d3       ;Matrix der Zeichens holen
          btst      #0,d0     ;gerades Zeichen?
          beq       gerade33  ;ja!
          lsl.b     #4,d3     ;nur die untersten 4 Bit
          bra       zeil_it33
gerade33: andi.b    #$f0,d3   ;in die obersten 4 Bit
zeil_it33:          move.w    d3,d1
          not.w     d1        ;invertiert
          moveq.l   #0,d7     ;Bei Plane 0 starten
plane_loop33:       btst      d7,9(a6)  ;Schriftfarbe an?
          beq       ausis1    ;auch nicht, fertig
          or.b      d3,(a0)   ;Zeichen einodern
          bra.s     fertig33
ausis1:   and.b     d1,(a0)   ;und weglöschen
fertig33: addq.l    #2,a0     ;eine Plane weiter
          addq.w    #1,d7
          cmpi.w    #4,d7
          bne       plane_loop33

          lea.l     152(a0),a0
          lea.l     40(a2),a2
          dbra      d6,zeil_loop33
          bra       ende23

po83:     addq.l    #1,a0     ;8 Pixel weiter
          bra       oben3

po43:     moveq.l   #0,d0
          move.b    15(a6),d0 ;Zeichen holen
          bsr       wandel    ;in internen Zeichensatz
          moveq.l   #4,d6     ;5 Zeilen
          move.l    #$f0,d5   ;zum Aus-Anden
          moveq.l   #15,d4    ;zum Setzen des Hintergrund
zeil43:   move.w    d0,d1
          lsr.w     #1,d1
          move.b    0(a2,d1.w),d3
          btst      #0,d0     ;gerades Zeichen?
          beq       gerade43  ;ja!
          andi.b    #$0f,d3   ;nur die untersten 4 Bit
          bra       zeil_it43
gerade43: lsr.b     #4,d3     ;in die obersten 4 Bit
zeil_it43:          move.w    d3,d1
          not.w     d1
          moveq.l   #0,d7     ;Bei Plane 0 starten
plane43:  btst      d7,9(a6)  ;Schriftplane an?
          beq       ausis2    ;auch nicht, fertig
          or.b      d3,(a0)   ;Zeichen einodern
          bra.s     fertig43
ausis2:   and.b     d1,(a0)   ;und weglöschen
fertig43: addq.l    #2,a0     ;eine Plane weiter
          addq.w    #1,d7
          cmpi.w    #4,d7
          bne       plane43

          lea.l     152(a0),a0
          lea.l     40(a2),a2
          dbra      d6,zeil43
          bra       ende23

po123:    addq.l    #1,a0     ;8 Pixel weiter
          bra       po43

ende23:   movem.l   (sp)+,d0-d7/a0-a5
          unlk      a6
          rts       


;Initialisiert die ATARI-Hardware etc.
;C-Definition: init_hardware()

mcode3:   movem.l   d0-d7/a0-a6,-(sp)

          move.l    #spurios,$60        ;NMI auf RTE setzen
          move.b    #%01001000,$fffffa17          ;Software-End-of-IRQ setzen

;Als nächstes werden die Exception-Vektoren gesetzt,
;damit ich weiß, was passiert ist....

          movea.l   $c,a0     ;Adresserror
          cmpi.w    #$e42d,-(a0)        ;Ist der Omikron-Assembler im System?
          bne.s     set_them
          cmpi.l    #'Soft',-(a0)
          beq.s     no_set
set_them: move.l    #bus_error,$8
          move.l    #adress_error,$c
          move.l    #illegal_code,$10
          move.l    #division,$14
          move.l    #privileg,$20

no_set:   movem.l   (sp)+,d0-d7/a0-a6
          rts       

bus_error:          clr.w     error_nr
          lea.l     8(sp),sp  ;Auf PC gehen
          bra       show_error
adress_error:       move.w    #1,error_nr
          lea.l     8(sp),sp  ;Auf PC gehen
          bra       show_error
illegal_code:       move.w    #2,error_nr
          bra       show_error
division: move.w    #3,error_nr
          bra       show_error

;Die folgende Funktion wird aufgerufen wenn ein priviligierter Befehl im
;Usermode ausgeführt wurde. Da beim 68000'er der Befehl 'Move SR,<EA>' kein
;priviligierter Befehl ist, führt er auf diesem Prozessor zu keiner
;Privilegverletzung. Auf Atari TT, der ja einen 68030 enthält, führt dies
;aber zu einer Privilegverletzung. Diese Routine führt den 'Move SR,<EA>' dann
;im Supervisormodus aus, wo es auch auf dem 68030 keine Privilegverletzung gibt.
;Dann wird das Programm fortgesetzt.
privileg: movem.l   d0/a0/a1,register_rett        ;Register retten
          movea.l   2(sp),a0  ;PC holen
          move.w    (a0),d0   ;Befehl holen
          andi.w    #%1111111111000000,d0
          cmpi.w    #%0100000011000000,d0         ;Move Sr,<EA>?
          bne       privileg_error      ;Nein, dann wirklich Fehler

          lea.l     do_error+4,a1
          move.w    (a0)+,d0  ;Befehl holen
          andi.w    #%111111,d0         ;Modus isolieren
          cmpi.w    #%101000,d0         ;d(An)..Abs.L?
          blt.s     auswert_fertig
          move.w    (a0)+,(a1)+         ;Zwei Worte langer Befehl
          cmpi.w    #%111001,d0         ;Abs.L?
          bne.s     auswert_fertig
          move.w    (a0)+,(a1)+         ;Drei Worte lang
auswert_fertig:     lsr.w     #1,d0
          bcc.s     noset1
          bset      #5,d0
noset1:   lsr.w     #1,d0
          bcc.s     noset2    ;Modus und Register vertauschen
          bset      #5,d0
noset2:   lsr.w     #1,d0
          bcc.s     noset3
          bset      #5,d0
noset3:   lsl.w     #6,d0     ;Modus an Zielstelle schieben

          ori.w     #%0011000000111010,d0         ;Wortmove PC-Relativ
          move.w    d0,do_error         ;und eintragen
          move.w    #$4e71,(a1)+
          move.w    #$4e71,(a1)+        ;NOPS ans Ende
          move.l    a0,pc_counter       ;PC merken

          movem.l   register_rett,d0/a0/a1        ;Register wieder her
          move.w    (sp)+,status_reg    ;Statusregister holen
          addq.l    #4,sp     ;PC wieder runter
do_error: move.w    status_reg(pc),d0
          nop       
          nop                 ;Move Sr,<Ea> wurde übersetzt in:
          nop                 ;Move.w status_reg,<EA>
          nop       
          move.l    pc_counter,-(sp)
          move.w    status_reg,-(sp)
          rte                 ;und fortsetzen

pc_counter:         .DC.l 0
status_reg:         .DC.w $0300
register_rett:      .DS.l 3   ;Platz für 3 Register

privileg_error:     move.w    #4,error_nr

show_error:         move.w    #$2700,sr ;Keine Störung bitte

          movem.l   d0-d7/a0-a7,$204    ;Register retten
          move.l    #'RoM2',$200

          move.w    #$7,$ffff8240
          clr.w     $ffff825e

          moveq.l   #0,d0
          lea.l     $ffff8200,a0
          movep.w   1(a0),d0  ;Videobasis holen
          lsl.l     #8,d0
          move.l    d0,logbase          ;als logische Basis setzen
          movea.l   d0,a0

          move.w    #479,d7   ;die obersten 2 Zeilen löschen
clr_loop: clr.l     (a0)+
          dbra      d7,clr_loop

          move.w    error_nr,d0
          lsl.w     #2,d0
          movea.l   error_text(pc,d0.w),a0
          moveq.l   #0,d0
          moveq.l   #0,d1     ;Koordinaten
          bsr       stringout

          move.l    2(sp),d7  ;PC holen
          bsr       hex_out

          lea.l     prg_base,a0
          moveq.l   #0,d0
          moveq.l   #6,d1
          bsr       stringout
          move.l    _BasPag,d7          ;Adresse der Basepage
          bsr       hex_out

flacker:  bra.s     flacker   ;10

error_text:         .DC.l bus_text,adress_text,illegal_text
          .DC.l division_text,privileg_text,sonstiges_text

bus_text: .DC.b 'BUSERROR AT: ',0
adress_text:        .DC.b 'ADRESSERROR AT: ',0
illegal_text:       .DC.b 'ILLEGAL AT: ',0
division_text:      .DC.b 'DIVISION BY ZERO AT: ',0
privileg_text:      .DC.b 'PRIVILEG VIOLATION AT: ',0
sonstiges_text:     .DC.b 'ANOTHER ERROR AT: ',0
prg_base: .DC.b 'ROM STARTS AT: ',0
hex_tab:  .DC.b '0123456789ABCDEF'
          .EVEN 

error_nr: .DC.w 5

hex_out:  lea.l     bitfeld,a0
          lea.l     hex_tab,a1

          moveq.l   #7,d6     ;8 Stellen
hex_loop: rol.l     #4,d7
          move.w    d7,d2
          andi.w    #$f,d2
          move.b    0(a1,d2.w),(a0)+    ;Hex-Digit holen
          dbra      d6,hex_loop
          clr.b     (a0)

          lea.l     bitfeld,a0

stringout:          moveq.l   #0,d2
          move.b    (a0)+,d2  ;Zeichen holen
          beq.s     stringend
          move.w    d2,-(sp)
          move.w    d1,-(sp)  ;y
          move.w    d0,-(sp)  ;x
          bsr       mcode1    ;Fast-out
          addq.l    #6,sp
          addq.w    #4,d0     ;x+=4
          bra.s     stringout
stringend:          rts       

write_protect:      lea       str1,a0
          jsr       alert
          rts       

spurios:  rte                 ;Tritt NMI auf, weitermachen

str1:     .DC.b "DISKETTE IST SCHREIBGESCHÜTZT.",0
          .EVEN 


;Entfernt eine VBL-Routine aus der VBL-Queue
;C-Definition: exit_vbl(adresse)
;long adresse;                        /* Adresse in der VBL-Queue */

mcode7:   link      a6,#0
          movem.l   d0-d7/a0-a6,-(sp)

          movea.l   8(a6),a0
          clr.l     (a0)

          movem.l   (sp)+,d0-d7/a0-a6
          unlk      a6
          rts       


;Routine scrollt ein Neochrombild in den Bildschirm
;C-Defintion: scrl_links(adr)
;long adr;          /* Adresse des neuen Bildes */

mcode8:   movem.l   d0-d7/a0-a6,-(sp)   ;Register retten

          move.l    hlpbuf,screenpos1   ; Pointer auf 2. Bild
          move.l    pack_buf,screenpos2 ; Pointer auf 2. Bild

          movea.l   scr2,a0
          movea.l   hlpbuf,a1
          bsr       orderpicture        ; richtet Bild 1 her

          movea.l   scr2,a0   ; Bild zwei
          bsr       preshift8 ; um 8 preshiften

          movea.l   scr2,a0   ; Source
          movea.l   pack_buf,a1         ; Destination
          bsr       orderpicture

          move.w    #200,-(sp)          ;200 Rasterzeilen kopieren
          move.l    scr2,-(sp)          ;Ziel
          move.l    scr1,-(sp)          ;Quelle
          bsr       mcode15   ;copy_zeilen
          lea.l     10(sp),sp

          movea.l   scr2,a0   ; Screen2 um 8 nach links
          bsr       preshift8 ; verschieben

          movea.l   scr2,a1
          lea.l     17*160+153(a1),a1   ; letzter BB (rechts)
          movea.l   screenpos1,a0       ;rechten Rand aufbauen
          move.w    #scanlines-1,d0
sll1:     move.b    (a0),(a1) ;Rechten Rand holen
          move.b    2(a0),2(a1)
          move.b    4(a0),4(a1)
          move.b    6(a0),6(a1)
          lea.l     160(a1),a1
          addq.l    #8,a0     ;nächste Scanline holen
          dbra      d0,sll1

          clr.w     readycount          ; Zähler auf 0

mainloop: jsr       switchscreens
          bsr       scroll_links
          cmpi.w    #39,readycount      ; schon fertig ?
          bne.s     mainloop
          jsr       switchscreens

          movem.l   (sp)+,d0-d7/a0-a6
          rts       

preshift8:          
; shiftet Bild um 8 Pixel nach links
; in a0=Pointer auf Bild

          lea.l     17*160+1(a0),a0     ; Pointer um 1 Byte weiter
          move.w    #scanlines-1,d0     ; 150 Zeilen
sh8screen1:         moveq.l   #19,d1    ; 20 Bitblocks
sh8screen2:         move.b    (a0),-1(a0)         ; erzeugt preshiftetes
          move.b    2(a0),1(a0)         ; Bild auf Newscreen
          move.b    4(a0),3(a0)
          move.b    6(a0),5(a0)
          move.b    7(a0),(a0)
          move.b    9(a0),2(a0)
          move.b    11(a0),4(a0)
          move.b    13(a0),6(a0)
          lea.l     8(a0),a0
          dbra      d1,sh8screen2
          dbra      d0,sh8screen1
          rts       

scroll_links:       movea.l   scr2,a0   ; schiebt Screen um 16
          lea.l     17*160+8(a0),a0     ; auf zweiten Bitblock
          move.w    #(scanlines/2)-1,d0
sl0:      movem.l   (a0)+,d1-d7/a1-a6
          movem.l   d1-d7/a1-a6,-60(a0)
          movem.l   (a0)+,d1-d7/a1-a6
          movem.l   d1-d7/a1-a6,-60(a0)
          movem.l   (a0)+,d1-d7/a1-a5
          movem.l   d1-d7/a1-a5,-56(a0)
          lea.l     8(a0),a0  ;Hier werden 150 Scanlines kopiert
          movem.l   (a0)+,d1-d7/a1-a6
          movem.l   d1-d7/a1-a6,-60(a0)
          movem.l   (a0)+,d1-d7/a1-a6
          movem.l   d1-d7/a1-a6,-60(a0)
          movem.l   (a0)+,d1-d7/a1-a5
          movem.l   d1-d7/a1-a5,-56(a0)
          lea.l     8(a0),a0
          dbra      d0,sl0

          movem.l   (a0)+,d1-d7/a1-a6
          movem.l   d1-d7/a1-a6,-60(a0)
          movem.l   (a0)+,d1-d7/a1-a6   ;Und hier noch eine
          movem.l   d1-d7/a1-a6,-60(a0)
          movem.l   (a0)+,d1-d7/a1-a5
          movem.l   d1-d7/a1-a5,-56(a0)

          addq.w    #1,readycount       ; Zähler +1

          movea.l   screenpos1,a0       ; Pointer auf 2. Bildschirm
          addi.l    #scanlines*8,screenpos1
          movea.l   scr2,a1
          lea.l     17*160+152(a1),a1   ; letzter Bitblock
          moveq.l   #(scanlines/6)-1,d0
aos16l0:  movem.l   (a0)+,d1-d7/a2-a6   ; 12 Longs holen
          move.l    d1,(a1)
          move.l    d2,4(a1)
          move.l    d3,160(a1)
          move.l    d4,164(a1)
          move.l    d5,320(a1)
          move.l    d6,324(a1)
          move.l    d7,480(a1)
          move.l    a2,484(a1)
          move.l    a3,640(a1)
          move.l    a4,644(a1)
          move.l    a5,800(a1)
          move.l    a6,804(a1)
          lea.l     960(a1),a1          ; 7 Zeilen tiefer
          dbra      d0,aos16l0

          move.l    (a0)+,(a1)+
          move.l    (a0)+,(a1)+         ;und die 151te Zeile

          move.l    screenpos1,d0       ; Pointer tauschen
          move.l    screenpos2,screenpos1
          move.l    d0,screenpos2
          rts       

orderpicture:       
; baut das Bild um in neue Daten
; Daten die untereinander standen, stehen jetzt nebeneinander
; in a0=Bild
; in a1=Buffer

          lea.l     17*160(a0),a0       ;Zu Beginn des Scrollbereichs gehen
          moveq.l   #19,d0    ; 20 Bitblocks
opl0:     move.w    #scanlines-1,d1
          movea.l   a0,a2     ; Pointer in a2
opl1:     move.l    (a2),(a1)+          ; 2 Longs ablegen (1 Bitblock)
          move.l    4(a2),(a1)+
          lea.l     160(a2),a2          ; eine Zeile tiefer
          dbra      d1,opl1
          lea.l     8(a0),a0  ; einen Bitblock weiter
          dbra      d0,opl0
          rts       

screenpos1:         .DC.l 0   ;Pointer auf 2. Bild
screenpos2:         .DC.l 0   ;-"-
readycount:         .DC.w 0   ;Zähler zum Bild scrollen

;Routine scrollt ein Neochrombild in den Bildschirm
;C-Defintion: scrl_rechts()
mcode9:   movem.l   d0-d7/a0-a6,-(sp)   ;Register retten

          move.l    hlpbuf,screenpos1   ; Pointer auf 2. Bild
          move.l    pack_buf,screenpos2 ; Pointer auf 2. Bild

          movea.l   scr2,a0
          movea.l   hlpbuf,a1
          bsr       orderpicr ; richtet Bild 1 her

          movea.l   scr2,a0   ; Bild zwei
          bsr       preshift8r          ; um 8 preshiften

          movea.l   scr2,a0   ; Source
          movea.l   pack_buf,a1         ; Destination
          bsr       orderpicr

          move.w    #200,-(sp)          ;200 Rasterzeilen kopieren
          move.l    scr2,-(sp)          ;Ziel
          move.l    scr1,-(sp)          ;Quelle
          bsr       mcode15   ;copy_zeilen
          lea.l     10(sp),sp

          movea.l   scr2,a0   ; Screen2 um 8 nach links
          bsr       preshift8r          ; verschieben

          movea.l   scr2,a1
          lea.l     17*160(a1),a1       ; letzter BB (rechts)
          movea.l   screenpos1,a0       ;rechten Rand aufbauen
          move.w    #scanlines-1,d0
slr1:     move.b    1(a0),(a1)          ;Rechten Rand holen
          move.b    3(a0),2(a1)
          move.b    5(a0),4(a1)
          move.b    7(a0),6(a1)
          lea.l     160(a1),a1
          addq.l    #8,a0     ;nächste Scanline holen
          dbra      d0,slr1

          clr.w     readycount          ; Zähler auf 0

mainloopr:          jsr       switchscreens
          bsr       scroll_rechts
          cmpi.w    #39,readycount      ; schon fertig ?
          bne.s     mainloopr
          jsr       switchscreens

          movem.l   (sp)+,d0-d7/a0-a6
          rts       

preshift8r:         
; shiftet Bild um 8 Pixel nach rechts
; in a0=Pointer auf Bild

          lea.l     167*160+152(a0),a0  ; Pointer um 1 Byte weiter
          move.w    #scanlines-1,d0     ; 150 Zeilen
sr8screen1:         moveq.l   #19,d1    ; 20 Bitblocks
sr8screen2:         move.b    (a0),1(a0)          ; erzeugt preshiftetes
          move.b    2(a0),3(a0)         ; Bild auf Newscreen
          move.b    4(a0),5(a0)
          move.b    6(a0),7(a0)
          move.b    -7(a0),(a0)
          move.b    -5(a0),2(a0)
          move.b    -3(a0),4(a0)
          move.b    -1(a0),6(a0)
          subq.l    #8,a0     ;Eine BB zurück
          dbra      d1,sr8screen2
          dbra      d0,sr8screen1
          rts       

scroll_rechts:      movea.l   scr2,a0   ; schiebt Screen um 16
          lea.l     167*160+160(a0),a0  ; auf zweiten Bitblock
          move.w    #(scanlines/2)-1,d0
sr0:      movem.l   -60(a0),d1-d7/a1-a6
          movem.l   d1-d7/a1-a6,-(a0)
          movem.l   -60(a0),d1-d7/a1-a6
          movem.l   d1-d7/a1-a6,-(a0)
          movem.l   -56(a0),d1-d7/a1-a5
          movem.l   d1-d7/a1-a5,-(a0)
          subq.l    #8,a0     ;Hier werden 150 Scanlines kopiert
          movem.l   -60(a0),d1-d7/a1-a6
          movem.l   d1-d7/a1-a6,-(a0)
          movem.l   -60(a0),d1-d7/a1-a6
          movem.l   d1-d7/a1-a6,-(a0)
          movem.l   -56(a0),d1-d7/a1-a5
          movem.l   d1-d7/a1-a5,-(a0)
          subq.l    #8,a0
          dbra      d0,sr0

          movem.l   -60(a0),d1-d7/a1-a6
          movem.l   d1-d7/a1-a6,-(a0)
          movem.l   -60(a0),d1-d7/a1-a6 ;Und hier noch eine
          movem.l   d1-d7/a1-a6,-(a0)
          movem.l   -56(a0),d1-d7/a1-a5
          movem.l   d1-d7/a1-a5,-(a0)

          addq.w    #1,readycount       ; Zähler +1

          movea.l   screenpos1,a0       ; Pointer auf 2. Bildschirm
          addi.l    #scanlines*8,screenpos1
          movea.l   scr2,a1
          lea.l     17*160(a1),a1       ; letzter Bitblock
          moveq.l   #(scanlines/6)-1,d0
aos16r0:  movem.l   (a0)+,d1-d7/a2-a6   ; 12 Longs holen
          move.l    d1,(a1)
          move.l    d2,4(a1)
          move.l    d3,160(a1)
          move.l    d4,164(a1)
          move.l    d5,320(a1)
          move.l    d6,324(a1)
          move.l    d7,480(a1)
          move.l    a2,484(a1)
          move.l    a3,640(a1)
          move.l    a4,644(a1)
          move.l    a5,800(a1)
          move.l    a6,804(a1)
          lea.l     960(a1),a1          ; 7 Zeilen tiefer
          dbra      d0,aos16r0

          move.l    (a0)+,(a1)+
          move.l    (a0)+,(a1)+         ;und die 151te Zeile

          move.l    screenpos1,d0       ; Pointer tauschen
          move.l    screenpos2,screenpos1
          move.l    d0,screenpos2
          rts       

orderpicr:          
; baut das Bild um in neue Daten
; Daten die untereinander standen, stehen jetzt nebeneinander
; in a0=Bild
; in a1=Buffer

          lea.l     17*160+152(a0),a0   ;Zu Beginn des Scrollbereichs gehen
          moveq.l   #19,d0    ; 20 Bitblocks
opr0:     move.w    #scanlines-1,d1
          movea.l   a0,a2     ; Pointer in a2
opr1:     move.l    (a2),(a1)+          ; 2 Longs ablegen (1 Bitblock)
          move.l    4(a2),(a1)+
          lea.l     160(a2),a2          ; eine Zeile tiefer
          dbra      d1,opr1
          subq.l    #8,a0     ; einen Bitblock weiter
          dbra      d0,opr0
          rts       


;scrollt Bildschirm nach oben
;C-Definition: scrl_ho(adr)
;long adr;
mcode10:  link      a6,#0
          movem.l   d0-d7/a0-a6,-(sp)

          movea.l   8(a6),a0
          lea.l     17*160(a0),a0       ;An Start des Bildes gehen
          move.l    a0,adrspalte

          bsr       vor_hoch  ;scr2 4 Zeilen hoch
          jsr       switchscreens

          move.w    #36,counter         ;37 mal hoch setzen
nochmal12h:         movea.l   scr2,a0
          lea.l     25*160(a0),a0

          moveq.l   #87,d0
z12h:     movem.l   (a0)+,d1-d7/a1-a6   ;52 Bytes holen
          movem.l   d1-d7/a1-a6,-1332(a0)         ;52 Bytes schreiben
          movem.l   (a0)+,d1-d7/a1-a6   ;52 Bytes holen
          movem.l   d1-d7/a1-a6,-1332(a0)         ;52 Bytes schreiben
          movem.l   (a0)+,d1-d7/a1-a6   ;52 Bytes holen
          movem.l   d1-d7/a1-a6,-1332(a0)         ;52 Bytes schreiben
          movem.l   (a0)+,d1-d7/a1-a6   ;52 Bytes holen
          movem.l   d1-d7/a1-a6,-1332(a0)         ;52 Bytes schreiben
          movem.l   (a0)+,d1-d7/a1-a6   ;52 Bytes holen
          movem.l   d1-d7/a1-a6,-1332(a0)         ;52 Bytes schreiben
          dbra      d0,z12h

          movea.l   adrspalte,a1
          lea.l     -1280(a0),a0
          moveq.l   #25,d0
z22h:     movem.l   (a1)+,d1-d7/a2-a6
          movem.l   d1-d7/a2-a6,(a0)
          lea.l     48(a0),a0
          dbra      d0,z22h

          movem.l   (a1)+,d1-d7/a2
          movem.l   d1-d7/a2,(a0)

          addi.l    #4*160,adrspalte

          jsr       switchscreens

          subq.w    #1,counter
          bpl       nochmal12h

          movem.l   (sp)+,d0-d7/a0-a6
          unlk      a6
          rts       

vor_hoch: movea.l   scr2,a0
          lea.l     21*160(a0),a0

          move.w    #225,d0
vorh:     movem.l   (a0)+,d1-d7/a1-a6   ;52 Bytes holen
          movem.l   d1-d7/a1-a6,-692(a0)          ;52 Bytes schreiben
          movem.l   (a0)+,d1-d7/a1-a6   ;52 Bytes holen
          movem.l   d1-d7/a1-a6,-692(a0)          ;52 Bytes schreiben
          dbra      d0,vorh

          movem.l   (a0)+,d1-d4
          movem.l   d1-d4,-656(a0)      ;ganzes Bild 4 Zeilen hoch

          movea.l   adrspalte,a1
          moveq.l   #12,d0
          lea.l     -640(a0),a0
z222h:    movem.l   (a1)+,d1-d7/a2-a6
          movem.l   d1-d7/a2-a6,(a0)
          lea.l     48(a0),a0
          dbra      d0,z222h

          movem.l   (a1)+,d1-d4
          movem.l   d1-d4,(a0)
          rts       

;scrollt Bildschirm nach unten
;C-Definition: scrl_ru(adr)
;long adr;
mcode11:  link      a6,#0
          movem.l   d0-d7/a0-a6,-(sp)

          movea.l   8(a6),a0
          lea.l     168*160(a0),a0      ;Ans Ende des Bildes gehen
          move.l    a0,adrspalte

          bsr       vor_runter          ;scr2 4 Zeilen runter
          jsr       switchscreens

          move.w    #36,counter         ;37 mal hoch setzen
nochmal12:          movea.l   scr2,a0
          lea.l     168*160(a0),a0

          moveq.l   #87,d0
z12:      movem.l   -1332(a0),d1-d7/a1-a6         ;52 Bytes holen
          movem.l   d1-d7/a1-a6,-(a0)   ;52 Bytes schreiben
          movem.l   -1332(a0),d1-d7/a1-a6         ;52 Bytes holen
          movem.l   d1-d7/a1-a6,-(a0)   ;52 Bytes schreiben
          movem.l   -1332(a0),d1-d7/a1-a6         ;52 Bytes holen
          movem.l   d1-d7/a1-a6,-(a0)   ;52 Bytes schreiben
          movem.l   -1332(a0),d1-d7/a1-a6         ;52 Bytes holen
          movem.l   d1-d7/a1-a6,-(a0)   ;52 Bytes schreiben
          movem.l   -1332(a0),d1-d7/a1-a6         ;52 Bytes holen
          movem.l   d1-d7/a1-a6,-(a0)   ;52 Bytes schreiben
          dbra      d0,z12

          movea.l   adrspalte,a1
          moveq.l   #25,d0
z22:      lea.l     -48(a1),a1
          movem.l   (a1),d1-d7/a2-a6
          movem.l   d1-d7/a2-a6,-(a0)
          dbra      d0,z22

          lea.l     -32(a1),a1
          movem.l   (a1),d1-d7/a2
          movem.l   d1-d7/a2,-(a0)

          subi.l    #4*160,adrspalte

          jsr       switchscreens

          subq.w    #1,counter
          bpl       nochmal12

          movem.l   (sp)+,d0-d7/a0-a6
          unlk      a6
          rts       

vor_runter:         movea.l   scr2,a0
          lea.l     168*160(a0),a0

          move.w    #225,d0
vorr:     movem.l   -692(a0),d1-d7/a1-a6          ;52 Bytes holen
          movem.l   d1-d7/a1-a6,-(a0)   ;52 Bytes schreiben
          movem.l   -692(a0),d1-d7/a1-a6          ;52 Bytes holen
          movem.l   d1-d7/a1-a6,-(a0)   ;52 Bytes schreiben
          dbra      d0,vorr

          movem.l   -656(a0),d1-d4
          movem.l   d1-d4,-(a0)         ;ganzes Bild 4 Zeilen hoch

          movea.l   adrspalte,a1
          moveq.l   #12,d0
z222:     lea.l     -48(a1),a1
          movem.l   (a1),d1-d7/a2-a6
          movem.l   d1-d7/a2-a6,-(a0)
          dbra      d0,z222

          lea.l     -16(a1),a1
          movem.l   (a1),d1-d4
          movem.l   d1-d4,-(a0)
          rts       

;Zeigt Screen an
;C-Definition: showscreen(adr)
;long adr;          /* ADresse des neuen Screens */

mcode12:  link      a6,#0
          movem.l   d0-d7/a0-a6,-(sp)

          move.l    8(a6),d0  ;Adresse holen
          move.l    d0,$44e   ;log. Adresse setzen
          move.l    d0,logbase          ;auch als Logbase setzen
          lsr.l     #8,d0
          movea.w   #$8201,a0 ;Adresse Shifter
          movep.w   d0,0(a0)

          tst.b     zeit_che  ;Zeit cheat an?
          beq       vsync24
          move.w    #$777,$ffff8240

vsync24:  bsr       vsync

          movem.l   (sp)+,d0-d7/a0-a6
          unlk      a6
          rts       



;Vertauscht die beiden Bildschirm, zeigt Screen1 an und macht Vsync
switchscreens:      move.l    scr2,d0
          move.l    scr1,scr2
          move.l    d0,scr1

          move.l    d0,-(sp)
          bsr       mcode12   ;Show_Screen
          addq.l    #4,sp
          rts       

;Funktion setzt die Synchronisationsfrequenz des Monitors
;C-DEFINITION: set_sync(50)    /* oder 60 */
mcode13:  link      a6,#0
          movem.l   d0-d7/a0-a5,-(sp)

mmcode13: cmpi.w    #50,8(a6) ;50Hz?
          beq.s     hertz_50  ;Ja, einschalten
          bclr      #1,$ffff820a        ;60 Hz einschalten
          bra.s     mcode13_raus
hertz_50: bset      #1,$ffff820a        ;50 Hz einschalten
mcode13_raus:       

          movem.l   d0-d7/a0-a5,-(sp)
          unlk      a6
          rts       


;Diese Funktion holt die momentane Synchrnisationsfrequenz
;C-Definition: sync=get_sync();         /* 50 oder 60 */

mcode14:  moveq.l   #50,d0
          btst      #1,$ffff820a
          bne.s     mcode14_raus
          moveq.l   #60,d0
mcode14_raus:       rts       


;Funktion kopiert anzahl Bildschirmzeilen von quelle nach ziel
mcode15:  link      a6,#0
          movem.l   d0-d7/a0-a5,-(sp)

          movea.l   8(a6),a0  ;QUelle
          movea.l   12(a6),a1 ;Ziel
          move.w    16(a6),d7 ;Anzahl
          subq.w    #1,d7     ;wegen DBRA
          moveq.l   #40,d6    ;jeweils 40 Bytes
copy_loop:          movem.l   (a0)+,d0-d5/a2-a5   ;40 Bytes holen
          movem.l   d0-d5/a2-a5,(a1)
          adda.l    d6,a1
          movem.l   (a0)+,d0-d5/a2-a5   ;40 Bytes holen
          movem.l   d0-d5/a2-a5,(a1)
          adda.l    d6,a1
          movem.l   (a0)+,d0-d5/a2-a5   ;40 Bytes holen
          movem.l   d0-d5/a2-a5,(a1)
          adda.l    d6,a1
          movem.l   (a0)+,d0-d5/a2-a5   ;40 Bytes holen
          movem.l   d0-d5/a2-a5,(a1)
          adda.l    d6,a1
          dbra      d7,copy_loop

          movem.l   (sp)+,d0-d7/a0-a5
          unlk      a6
          rts       


;Diese Funktion löscht den Mauscursor auf den Bildschirm unter
;Berücksichtigung der 2 Bildschirmtechnik.
; void delete_mouse(long screen);
;->  8(a6): screen
mcode16:  link      a6,#0
          movem.l   d0-d7/a0-a5,-(sp)

          lea.l     save_1,a2 ;Default: Savebuffer von Screen1
          move.l    scrn_1,d0
          cmp.l     8(a6),d0  ;Ist es Bildschirm 1?
          beq       restore   ;ja, restaurieren
          lea.l     save_2,a2 ;nein, aus Savebuffer 2 holen
restore:  movea.l   (a2)+,a1  ;Pointer auf Screen holen
          moveq.l   #0,d7
          move.b    1(a2),d7  ;Anzahl holen
          beq.s     ende1     ;Buffer ungültig, abbrechen
          clr.b     1(a2)     ;Buffer jetzt ungültig
          tst.w     (a2)+     ;nur ein Wort gerettet?
          bmi.s     nur_ein_wort        ;ja->entsprechend restaurieren

restore1: move.l    (a2)+,(a1)+
          move.l    (a2)+,(a1)+
          move.l    (a2)+,(a1)+
          move.l    (a2)+,(a1)+
          lea.l     144(a1),a1
          dbra      d7,restore1
          bra.s     ende1

nur_ein_wort:       move.l    (a2)+,(a1)+
          move.l    (a2)+,(a1)+
          lea.l     152(a1),a1
          dbra      d7,nur_ein_wort

ende1:    movem.l   (sp)+,d0-d7/a0-a5
          unlk      a6
          rts       

;Diese Funktion zeichnet den Mauscursor auf den Bildschirm unter
;Berücksichtigung der 2 Bildschirmtechnik.
; void draw_mouse(long screen,int x,int y);
;-> 14(a6): y
;   12(a6): x
;    8(a6): screen
mcode17:  link      a6,#0
          movem.l   d0-d7/a0-a5,-(sp)

          lea.l     save_1,a2 ;Default: Savebuffer von Screen1
          move.l    scrn_1,d0
          cmp.l     8(a6),d0  ;Ist es Bildschirm 1?
          beq.s     put       ;ja, Maus setzen
          lea.l     save_2,a2 ;nein, aus Savebuffer 2 holen
put:      move.w    mouseform,d0
          mulu.w    #36,d0
          lea.l     maus_daten,a1
          adda.l    d0,a1     ;Adresse des Mousedefinition Blocks
          move.w    (a1)+,d0  ;Hot-Spot x
          sub.w     d0,12(a6)
          move.w    (a1)+,d0  ;Hot-Spot y
          sub.w     d0,14(a6)

          moveq.l   #15,d7    ;Höhe des Sprites
          move.w    14(a6),d1 ;Y
          bpl.s     clip3
          add.w     d1,d7     ;von der Höhe abziehen
          bmi.s     nichts_mehr_da
          add.w     d1,d1     ;verdoppeln (Achtung, negativ!)
          suba.w    d1,a1     ;und erst ab dieser Scanline Daten holen
          moveq.l   #0,d1     ;bei y=0 starten zu zeichnen
          move.w    d1,14(a6) ;und merken
clip3:    move.w    #200-16,d2          ;unterer Rand des Screens
          sub.w     d1,d2
          bpl.s     clip4
          add.w     d2,d7     ;von Höhe abziehen
          bmi.s     nichts_mehr_da
clip4:    movea.l   8(a6),a0  ;Bildschirmbasis
          mulu.w    #160,d1
          adda.l    d1,a0     ;=Adresse der Scanline

          move.w    12(a6),d0 ;x-Koordinate
          move.w    d0,d1
          bpl.s     no_clipl
          cmpi.w    #-16,d0
          ble.s     nichts_mehr_da      ;zu weit links
          moveq.l   #0,d0
no_clipl: cmpi.w    #320,d0
          bge.s     nichts_mehr_da      ;zu weit rechts
          lsr.w     #4,d0
          lsl.w     #3,d0
          adda.w    d0,a0     ;Adresse des BBs berechnen
          andi.w    #%1111,d1 ;Shiftwert berechnen

          move.l    a0,(a2)+  ;Adresse auf Screen in Buffer retten
          move.w    d7,(a2)+  ;Anzahl der Scanlines merken

          move.w    12(a6),d0 ;x-Koordinate
          bmi.s     clip_links
          cmpi.w    #304,d0
          bgt.s     clip_rechts

maus_loop:          moveq.l   #-1,d0
          move.w    (a1)+,d0  ;Wert der Maus holen
          ror.l     d1,d0     ;shiften

          move.l    (a0),(a2)+          ;Bildschirm retten
          and.w     d0,(a0)+  ;linker Teil
          and.w     d0,(a0)+  ;Linker Teil
          move.l    (a0),(a2)+
          and.w     d0,(a0)+
          and.w     d0,(a0)+
          swap.w    d0
          move.l    (a0),(a2)+
          and.w     d0,(a0)+
          and.w     d0,(a0)+
          move.l    (a0),(a2)+
          and.w     d0,(a0)+
          and.w     d0,(a0)+

          lea.l     144(a0),a0          ;Eine Scanline tiefer
          dbra      d7,maus_loop

nichts_mehr_da:     movem.l   (sp)+,d0-d7/a0-a5
          unlk      a6
          rts       


clip_links:         bset      #7,-2(a2) ;Bit 7=Nur ein Wort retten

maus_loop2:         moveq.l   #-1,d0
          move.w    (a1)+,d0  ;Mauszeiger holen
          ror.l     d1,d0     ;shiften
          swap.w    d0        ;nur rechten Teil nehmen
          move.l    (a0),(a2)+          ;Hintergrund merken
          and.w     d0,(a0)+  ;und einknüpfen
          and.w     d0,(a0)+
          move.l    (a0),(a2)+
          and.w     d0,(a0)+
          and.w     d0,(a0)+
          lea.l     152(a0),a0          ;Eine Scanline tiefer
          dbra      d7,maus_loop2
          bra.s     nichts_mehr_da

clip_rechts:        bset      #7,-2(a2) ;Bit 7=Nur ein Wort retten

maus_loop3:         moveq.l   #-1,d0
          move.w    (a1)+,d0  ;Mauszeiger holen
          ror.l     d1,d0     ;shiften
          move.l    (a0),(a2)+          ;nur linken Teil verwenden
          and.w     d0,(a0)+  ;und einknüpfen
          and.w     d0,(a0)+
          move.l    (a0),(a2)+
          and.w     d0,(a0)+
          and.w     d0,(a0)+
          lea.l     152(a0),a0          ;Eine Scanline tiefer
          dbra      d7,maus_loop3
          bra.s     nichts_mehr_da


;Routine trägt andere Routine in VBL-Queue ein
;Prototyp: void *mcode18(void *adr)
mcode18:  link      a6,#0
          movem.l   d1-d7/a0-a5,-(sp)

          lea.l     vbl_queue,a0
weiter34: tst.l     (a0)+     ;Eintrag frei?
          bne.s     weiter34
          move.l    8(a6),-(a0)
          move.l    a0,d0     ;Adresse zurückgeben

          movem.l   (sp)+,d1-d7/a0-a5
          unlk      a6
          rts       


;draw_shift(base,save,sprite_mem,sprite_nr,shape,x,y)
;long base;              /* Bildschirmbasis */
;long save;              /* Basis des Hintergrundpointerbereichs */
;long sprite_mem;        /* basis für die Spritedaten */
;int sprite_nr;          /* Nummer des Sprites */
;int shape;              /* Form des Sprites */
;int x,y;                /* Position */
mcode19:  link      a6,#0
          movem.l   d0-d7/a0-a6,-(sp)   ;Register retten

          movea.l   8(a6),a0  ;base
          movea.l   12(a6),a2 ;save
          move.w    20(a6),d0 ;Nummer des Sprites
          lsl.w     #3,d0     ;x8
          adda.w    d0,a2     ;Adresse des jeweiligen Savebuffers
          move.w    22(a6),d0 ;Shape
          move.w    24(a6),d1 ;x
          move.w    26(a6),d2 ;y

;Zeichnet ein Sprite auf den Bildschirm
;Parameter:
;D0: Shapenummer
;D1: X-Koordinate
;D2: Y-Koordinate
;A0: Basisadresse des Screens
;A2: Adresse des Savebuffers
;veränderte Register: keine
          mulu.w    #160,d2   ;160 Bytes pro Scanline
          move.w    d1,d3     ;X-retten
          lsr.w     #4,d3     ;/16
          lsl.w     #3,d3     ;*8

          add.w     d3,d2     ;Adresse des Beginns von 4 Planes
          adda.w    d2,a0     ;Offset auf Bildschirmbasis
          movea.l   hlpbuf,a1
          adda.w    d2,a1     ;Offset für Hintergrund (Quelle)
          move.l    a1,(a2)+
          move.l    a0,(a2)   ;Offset in Buffer

          andi.w    #15,d1    ;Verschiebungsfaktor berechnen
          move.w    d1,d5

          movea.l   16(a6),a1 ;Basisadresse der Sprites
          lsl.w     #6,d0     ;*64
          adda.w    d0,a1     ;Basisadresse der Spritedaten

          moveq.l   #7,d7     ;Anzahl der Scanlines (Höhe des Sprites)

spr_draw: moveq.l   #0,d0
          moveq.l   #0,d1
          moveq.l   #0,d2
          moveq.l   #0,d3
          moveq.l   #0,d4     ;Maske
          move.w    (a1)+,d0
          move.w    (a1)+,d1  ;Sprite holen
          move.w    (a1)+,d2
          move.w    (a1)+,d3

          move.w    d0,d4
          or.w      d1,d4
          or.w      d2,d4
          or.w      d3,d4     ;Maske errechnen
          ror.l     d5,d0
          ror.l     d5,d1
          ror.l     d5,d2
          ror.l     d5,d3     ;Sprite shiften
          ror.l     d5,d4     ;Maske shiften
          not.l     d4        ;Maske invertieren
          and.w     d4,(a0)   ;Maske ausanden
          or.w      d0,(a0)+
          and.w     d4,(a0)
          or.w      d1,(a0)+
          and.w     d4,(a0)
          or.w      d2,(a0)+
          and.w     d4,(a0)
          or.w      d3,(a0)+

          swap.w    d0
          swap.w    d1
          swap.w    d2
          swap.w    d3
          swap.w    d4

          and.w     d4,(a0)   ;Maske ausanden
          or.w      d0,(a0)+
          and.w     d4,(a0)
          or.w      d1,(a0)+
          and.w     d4,(a0)
          or.w      d2,(a0)+
          and.w     d4,(a0)
          or.w      d3,(a0)+
          lea.l     144(a0),a0          ;Eine Zeile tiefer

          dbra      d7,spr_draw

          movem.l   (sp)+,d0-d7/a0-a6
          unlk      a6
          rts       


;undraw_sprite(save,sprite_nr)
;long save;
;int sprite_nr;
;  /* Löscht ein Sprite vom Bildschirm */
;  /* Die Basis wird aus dem Saveblock gewonnen */
;  /* Die Hintergrundinformation aus bild_buf */
mcode20:  link      a6,#0
          movem.l   d0-d7/a0-a6,-(sp)   ; Register retten

          move.w    12(a6),d0 ;Sprite_nr
          movea.l   8(a6),a0  ; Savebasis

; Löscht Sprite vom Bildschirm, Hintergrund wird von bild_buf geholt
; Parameter:
; D0: Spritenummer
; A0: Basisadresse des Savebereichs
; veränderte Register: ?
clr_mob:  lsl.w     #3,d0     ;x8
          adda.w    d0,a0     ;dazuaddieren zu Savebasis

          movea.l   (a0),a2   ;Offset holen
          cmpa.l    #0,a2     ;Buffer ungültig?
          beq.s     raus      ; ja, raus

          clr.l     (a0)+     ;Buffer ist jetzt ungültig
          movea.l   (a0)+,a1  ;Zieladresse

          move.l    #144,d5   ;Offset für A1
          moveq.l   #7,d7     ;8 Scanlines restaurieren

loop89:   move.l    (a2)+,(a1)+         ; Spritehintergrund
          move.l    (a2)+,(a1)+         ; Hintergrund restaurieren
          move.l    (a2)+,(a1)+
          move.l    (a2)+,(a1)+
          adda.l    d5,a2
          adda.l    d5,a1     ; um 160 Weiter
          dbf       d7,loop89

raus:     movem.l   (sp)+,d0-d7/a0-a6
          unlk      a6
          rts       



; Mfp-Definitions:

iera      = $fffa07 ; Interrupt Enable RegisterA
isra      = $fffa0f ; Interrupt In-Service Register
imra      = $fffa13 ; Interrupt Mask Register
imrb      = imra+2
timerbcr  = $fffa1b ; TimerB-Kontrollregister
timerbdr  = $fffa21 ; TimerB-Datenregister

get_scancodes:      tst.w     tos_da    ;Ist das TOS vorhanden?
          beq.s     nichts_da74         ;Nein
          moveq.l   #-1,d0
          move.l    d0,-(sp)
          move.l    d0,-(sp)
          move.l    d0,-(sp)
          move.w    #16,-(sp)
          trap      #14       ;Adresse der Tastaturscancodes holen
          lea.l     14(sp),sp
          movea.l   d0,a0
          move.l    8(a0),scancode
nichts_da74:        rts       

;Diese Funktion schaltet das HBL-System ein (einmal zu Beginn des Programms)
mcode34:  movem.l   d0-d7/a0-a6,-(sp)

          move.b    #-1,flop_aktiv      ;Floppy nicht aktiv

          bsr       get_scancodes       ;Adresse der Scancode Tabelle holen

          move.l    $118,old_118
          move.l    #dummy_tastatur,$118

          tst.b     abs_maus  ;Absolute Maus?
          beq.s     relative_maus

          moveq.l   #(abs_maus_end-abs_maus_an)-1,d1
          lea.l     abs_maus_an,a0
          bsr       send_ikbd_str
          bra.s     joy_aus

relative_maus:      
          moveq.l   #8,d0     ;Relative Maus an
          bsr       send_ikbd

joy_aus:  moveq.l   #$1a,d0   ;Joysticks aus
          bsr       send_ikbd

          bsr       mcode24   ;clear_raster
          bsr       mcode55   ;und neu ausrechnen
          bsr       rst22rst1 ;und zurück kopieren
          move.l    #rasters1,rasters   ;und 1. System wieder anzeigen

          move.w    #6000,d7  ;Counter
          clr.b     keypress
warten_ende2:       tst.b     keypress  ;Byte angekommen?
          beq.s     nichts_da29
          move.w    #6000,d7
          clr.b     keypress
nichts_da29:        tst.w     d7
          dbmi      d7,warten_ende2

          move.l    #tastatur_buffer,tastatur_write

          move.l    #neue_tastatur,$118
          move.l    $120,old_timer_b
          move.l    $70,old_vbl

          andi.b    #$fe,$fffffa07      ;Timer B IRQ sperren
          andi.b    #%11011111,$fffffa09          ;200Hz sperren

          clr.b     $fffffa1b ;Timer B STop!

          move.l    #sync_hbl,$00000120
          move.l    #vbl,$00000070

          ori.b     #1,$fffffa07        ;Timer B zulassen
          ori.b     #1,$fffffa13        ;nicht maskieren
          bclr      #3,$fffffa03        ;Timer B am Ende der Rasterzeile bitte

          bsr       vsync
          clr.b     $ffff8260 ;LOW-Rez einschalten

          movem.l   (sp)+,d0-d7/a0-a6
          rts       


;Diese Funktion schaltet die Rasterroutinen wieder aus
mcode35:  movem.l   d0-d7/a0-a6,-(sp)

          bclr      #0,iera   ;Timer B kein IRQ
          clr.b     timerbcr  ;TimerB stoppen

          move.l    old_vbl,$70

          move.w    sr,-(sp)

wartebisleer2:      move.w    #$2500,sr ;MFP ermöglichen
wartebisleer:       cmpi.l    #tastatur_buffer,tastatur_write         ;Buffer leer?
          bne.s     wartebisleer

          move.w    #$2700,sr
          cmpi.l    #tastatur_buffer,tastatur_write         ;Immer noch leer?
          bne.s     wartebisleer2       ;Nein->Scheisse

          move.l    old_118,$118        ;abmelden
          move.l    old_timer_b,$120

          move.w    (sp)+,sr

          moveq.l   #8,d0     ;Relative Maus an
          bsr       send_ikbd

          bset      #5,$fffffa09        ;200Hz IRQ wieder an

          movem.l   (sp)+,d0-d7/a0-a6
          rts       


;Diese IRQ-Routine wird angesprungen, falls von der Tastatur ein Zeichen
;abgeholt werden soll.
neue_tastatur:      move.w    #$2500,sr ;HBL ermöglichen

          movem.l   d0-d7/a0-a6,-(sp)   ;Alle Register retten

          btst      #0,$fffffc00        ;Byte angekommen?
          beq.s     start_send          ;Nein, weitermachen

hol_byte: move.w    sr,-(sp)
          move.w    #$2700,sr ;HBL sperren
          move.b    $fffffc00,d0        ;ACIA-Status holen
          btst      #0,d0     ;Receiver full, also Byte da?
          beq.s     no_byte_da          ;Nein->Scheiß HBL hat geklaut
          movea.l   tastatur_write,a0
          move.b    $fffffc02,(a0)+     ;Wert merken
          move.l    a0,tastatur_write   ;Neue Schreibposition merken
no_byte_da:         move.w    (sp)+,sr
          btst      #5,d0     ;Overrun?
          beq.s     start_send

          clr.b     overrun   ;merken
          move.b    $fffffc02,d0        ;Daten nochmal lesen, Status löschen

start_send:         tst.b     overrun   ;Ist Overrun aufgetreten?
          beq       overrun_auswert     ;ja, neu synchronisieren
          movea.l   tastatur_read,a0    ;Leseposition
          moveq.l   #0,d0
          moveq.l   #0,d1

weitersuchen:       cmpa.l    tastatur_write,a0   ;Schreibposition erreicht?
          bge       start_senden        ;ja, Ende interpret
          move.b    (a0)+,d0  ;Zeichen holen
          cmpi.b    #$f6,d0   ;Paket?
          bcc.s     ein_paket
          bsr       taste_auswert       ;Tastendruck auswerten
          bra.s     weitersuchen
ein_paket:          move.b    d0,d1
          subi.w    #$00f6,d1
          lea.l     paket_tabelle,a1
          move.b    0(a1,d1.w),d1       ;Länge des Pakets-1 holen
          lea.l     -1(a0,d1.w),a1      ;letztes Byte des Pakets
          cmpa.l    tastatur_write,a1   ;Ist das Paket komplett?
          blt       paket_komplett      ;Ja, auswerten
          subq.l    #1,a0     ;Um den Header zurückgehen

start_senden:       move.l    a0,ende_auswert     ;Bis hier wurde ausgewertet

          btst      #0,$fffffc00        ;Byte angekommen?
          beq.s     weiter4   ;Nein, weitermachen

          move.w    sr,-(sp)
          move.w    #$2700,sr ;HBL sperren
          move.b    $fffffc00,d0        ;ACIA-Status holen
          btst      #0,d0     ;Receiver full, also Byte da?
          beq.s     no_byte_da2         ;Nein->Scheiß HBL hat geklaut
          movea.l   tastatur_write,a3
          move.b    $fffffc02,(a3)+     ;Wert merken
          move.l    a3,tastatur_write   ;Neue Schreibposition merken
no_byte_da2:        move.w    (sp)+,sr
          btst      #5,d0     ;Overrun?
          beq.s     weiter4
          clr.b     overrun   ;merken
          move.b    $fffffc02,d0        ;Byte nochmal lesen,Status löschen

weiter4:  tst.b     overrun   ;Ist Overrun aufgetreten?
          beq       overrun_auswert     ;ja, neu synchronisieren

          lea.l     tastatur_buffer,a0
          movea.l   ende_auswert,a1

          move.w    sr,-(sp)
          move.w    #$2700,sr
          move.l    a0,tastatur_read    ;vorne im Buffer anfangen zu lesen
          bra.s     copy_if
copy_buffer:        move.b    (a1)+,(a0)+
copy_if:  cmpa.l    tastatur_write,a1
          blt.s     copy_buffer
          move.l    a0,tastatur_write
          move.w    (sp)+,sr

          btst      #0,$fffffc00        ;noch ein Zeichen angekommen?
          bne       hol_byte  ;ja, abholen

ende_tastatur:      movem.l   (sp)+,d0-d7/a0-a6   ;Register wieder holen
tast_end: bclr      #6,$fffffa11        ;In-Service löschen
          rte       

paket_komplett:     bsr       paket_auswert
          lea.l     1(a1),a0  ;nächstes Byte hinter Paket
          bra       weitersuchen


;Es ist ein Overrun aufgetreten. Deshalb wird der IKBD auf absolute Maus
;umgeschaltet, und gewartet, bis keine Daten mehr kommen. Dann wird wieder
;auf relative Maus zurückgeschaltet. Damit bekommt der Computer den Anfang
;eines Datenpakets wieder mit.
overrun_auswert:    move.w    #$2300,sr ;VBL zulassen

          moveq.l   #(abs_ms-abs_maus_an)-1,d1
          lea.l     abs_maus_an,a0
          bsr       send_ikbd_str       ;Auf absolute Maus gehen

          move.w    #3000,d7  ;Counter
warten_ende:        btst      #0,$fffffc00        ;Byte angekommen?
          beq.s     nichts_da
          move.w    #3000,d7
          move.b    $fffffc02,d0        ;abholen
nichts_da:          tst.w     d7
          dbmi      d7,warten_ende

          lea.l     tastatur_buffer,a0
          move.w    sr,-(sp)
          move.w    #$2700,sr ;Tastaturbuffer löschen
          move.b    #1,overrun          ;Jetzt kein Overrun mehr!
          move.l    a0,tastatur_read
          move.l    a0,tastatur_write
          move.w    (sp)+,sr

          tst.b     abs_maus  ;Absolute Maus?
          bne.s     ende_tastatur

          moveq.l   #8,d0     ;Relative Maus wieder an
          bsr       send_ikbd

          bra.s     ende_tastatur


;Von der Tastatur kam ein einzelner Tastendruck
;Beachte Xbios(16) zum Transformieren
; D0.B: Scancode der Taste
taste_auswert:      andi.w    #$ff,d0
          cmpi.w    #$2a,d0   ;Linke Shift?
          beq.s     shift_pressed
          cmpi.w    #$36,d0   ;Rechte Shift?
          beq.s     shift_pressed
          cmpi.w    #$2a+$80,d0
          beq.s     shift_released
          cmpi.w    #$36+$80,d0
          beq.s     shift_released
          cmpi.w    #$38,d0   ;Alternate
          beq.s     alt_pressed
          cmpi.w    #$3a,d0   ;Caps Lock?
          beq.s     losgelassen
          cmpi.w    #$1d,d0   ;Control?
          beq       ctrl_pressed
          cmpi.w    #$38+$80,d0         ;Alt los?
          beq.s     alt_released
          cmpi.w    #$1d+$80,d0         ;CTRL los?
          beq.s     ctrl_released
          cmpi.w    #$53,d0   ;Delete?
          beq       delete_pressed
normale_taste:      movea.l   scancode,a3         ;Adresse der Scancode-Tabelle
          tst.b     d0
          bmi.s     losgelassen
          moveq.l   #0,d2
          move.b    0(a3,d0.w),d2       ;In ASCII wandeln
          swap.w    d0        ;Scancode in oberes Byte
          move.w    d2,d0     ;ASCII unten rein
          move.l    d0,keypress         ;merken
          move.w    tail,d1
          addq.w    #4,d1     ;Ein Byte weiter
          cmpi.w    #80,d1    ;Bufferende erreicht?
          bcs.s     nein
          moveq.l   #0,d1
nein:     cmp.w     head,d1   ;Buffer voll?
          beq.s     buffer_voll
          lea.l     key_buffer,a2
          move.l    d0,0(a2,d1.w)
          move.w    d1,tail
buffer_voll:        rts       

losgelassen:        clr.l     keypress  ;Keine Taste mehr gedrückt
          bra.s     buffer_voll

shift_pressed:      bset      #0,shift
          rts       
shift_released:     bclr      #0,shift
          rts       
alt_pressed:        bset      #1,shift
          rts       
alt_released:       bclr      #1,shift
          rts       
ctrl_pressed:       bset      #2,shift
          rts       
ctrl_released:      bclr      #2,shift
          rts       

delete_pressed:     cmpi.b    #%110,shift         ;CTRL+ALT?
          bne       normale_taste
          movea.l   4,a0
          jmp       (a0)      ;Reset auslösen

;Es wurde von der Tastatur ein komplettes Paket empfangen
;Dieses Paket muß jetzt ausgewertet werden
; A0.L: Zeiger auf Paket
;  D0.B: Paketheader
paket_auswert:      andi.w    #$ff,d0   ;nur unteres Byte beachten
          cmpi.w    #$f8,d0   ;relative Mausangabe?
          blt       absolute_maus       ;nein, muß absolut sein!
          cmpi.w    #$fb,d0   ;relative Maus?
          bgt       no_maus   ;Nein, ganz raus
          andi.b    #3,d0     ;Maustasten isolieren
          lsr.b     #1,d0     ;-> 1
          bcc       no_set2   ;war rechter Knopf gedrückt?
          bset      #1,d0     ;ja, Bit wieder setzen
no_set2:  ext.w     d0
          move.w    d0,mousek ;und Maustasten sichern
          move.b    (a0)+,d0  ;X Offset
          ext.w     d0
          add.w     d0,mdeltax          ;(10)
          add.w     d0,mousex ;zu MausX addieren
          move.b    (a0)+,d0  ;Y Offset
          ext.w     d0
          add.w     d0,mdeltay          ;(10)
          add.w     d0,mousey ;zu Mousey addieren
rand_test:          cmpi.w    #319,mousex         ;rechter Rand erreicht?
          ble       o1
          move.w    #319,mousex
o1:       tst.w     mousex
          bge       o2
          clr.w     mousex
o2:       cmpi.w    #199,mousey
          ble       oben_test
          move.w    #199,mousey
oben_test:          move.w    mousey,d0
          cmp.w     msminy,d0 ;maximal Wert nach oben, Leiste erreicht?
          bge       no_maus
          move.w    msminy,mousey
no_maus:  rts       

absolute_maus:      move.b    (a0)+,d0  ;Maustasten holen
          move.w    mousek,d1 ;Maustaste aufbauen
          btst      #0,d0     ;rechter Knopf gedrückt?
          beq.s     no1       ;nein
          bset      #1,d1     ;rechter Knopf wurde gedrückt!
no1:      btst      #2,d0     ;linker Knopf gedrückt?
          beq.s     no2       ;nein
          bset      #0,d1     ;gedrückt!
no2:      btst      #1,d0     ;rechter Knopf losgelassen?
          beq.s     no3       ;nein
          bclr      #1,d1     ;losgelassen!
no3:      btst      #3,d0     ;linker Knopf losgelassen?
          beq.s     no4       ;nein
          bclr      #0,d1     ;losgelassen!
no4:      move.w    d1,mousek

          moveq.l   #0,d0
          move.b    (a0)+,d0  ;abs_x
          lsl.w     #8,d0
          move.b    (a0)+,d0  ;=Word
          move.w    d0,mousex ;speichern
          move.b    (a0)+,d0  ;abs_y
          lsl.w     #8,d0
          move.b    (a0)+,d0
          move.w    d0,mousey
          bra       rand_test ;Testen auf oberen Rand


paket_tabelle:      
; hier steht die Länge der Tastaturpakete in Bytes
          .DC.b 7   ; $f6 Status-Meldung
          .DC.b 5   ; $f7 (absolute Mausposition)
          .DC.b 2   ; $f8 - $fb (relative Mausposition)
          .DC.b 2   ; $f9
          .DC.b 2   ; $fa
          .DC.b 2   ; $fb
          .DC.b 6   ; $fc (Uhrzeit abfragen)
          .DC.b 2   ; $fd (?)
          .DC.b 1   ; $fe-$ff (Joystick)
          .DC.b 1   ; $ff


;Diese Routine holt Bytes vom Tastaturprozessor und wirft sie fort.
dummy_tastatur:     btst      #0,$fffffc00        ;Byte angekommen?
          beq.s     nichts99
          tst.b     $fffffc02 ;Daten holen und wegwerfen
          st        keypress  ;Es kam was an
nichts99: bclr      #6,$fffffa11
          rte       

;Übermittelt einen String an den IKBD (reentrant)
; A0.L: Stringadresse
;  D1.W: Länge-1
send_ikbd_str:      move.b    (a0)+,d0
          bsr.s     send_ikbd
          dbra      d1,send_ikbd_str
          rts       

;Übermittelt ein Byte an die Tastatur (reentrant)
; D0.B: Wert, der gesendet werden soll
send_ikbd:          btst      #1,$fffffc00        ;.w  ;ACIA bereit?
          beq.s     send_ikbd ;Nein, warten
          move.b    d0,$fffffc02        ;.w  ;und abschicken
          rts       

;Setzt die Maus auf einen bestimmten Punkt (7)
;C-Defintion: void set_mousepos(int x,int y);
mcode73:  link      a6,#0
          movem.l   d0-d7/a0-a6,-(sp)

          move.l    8(a6),set_ms+2      ;X/Y setzen
          move.l    8(a6),mousex        ;X/Y setzen

          tst.b     abs_maus  ;Absolute Maus?
          beq.s     keine_absolute

          st        maus_abfrag         ;Keine VBL Mausabfrage

          moveq.l   #(set_ms-abs_maus_an)-1,d1
          lea.l     set_ms,a0
          bsr       send_ikbd_str       ;und senden

          sf        maus_abfrag         ;und wieder Position holen

keine_absolute:     movem.l   (sp)+,d0-d7/a0-a6
          unlk      a6
          rts       


;(13) Schaltet aus Zeitgründen die Mausabfrage ab
;void mouse_off(void)
mcode74:  st        maus_abfrag         ;Keine VBL Mausabfrage

          movem.l   d0-d7/a0-a6,-(sp)
          moveq     #(mouse_key-abs_maus_an)-1,d1
          lea       abs_maus_an,a0
          bsr       send_ikbd_str       ;Absolute Maus anschalten
          movem.l   (sp)+,d0-d7/a0-a6
          rts       


;(13) Schaltet die Mausabfrage wieder an
;void mouse_on()
mcode75:  tst.b     abs_maus
          bne.s     absolute3

          moveq     #$8,d0    ;Relative Maus an
          bra       send_ikbd

absolute3:          sf        maus_abfrag         ;und wieder Position holen
          rts       


;Holt ein Zeichen aus dem Tastaturbuffer
;Ist kein Zeichen vorhanden, bringt die Routine -1 zurück
mcode57:  movem.l   d1-d7/a0-a6,-(sp)

          move.l    #-1,zeichen

          move.w    head,d1
          cmp.w     tail,d1
          beq.s     nichts_da2

          move.w    sr,-(sp)
          move.w    #$2700,sr
          move.w    head,d1
          addq.w    #4,d1
          cmpi.w    #80,d1
          bcs.s     buffer_ende
          moveq.l   #0,d1
buffer_ende:        lea.l     key_buffer,a0
          moveq.l   #0,d0
          move.l    0(a0,d1.w),d0       ;Zeichen holen
          move.l    d0,zeichen
          move.w    d1,head
          move.w    (sp)+,sr

nichts_da2:         move.l    zeichen,d0
          movem.l   (sp)+,d1-d7/a0-a6
          rts       

zeichen:  .DC.l 0
head:     .DC.w 0
tail:     .DC.w 0
;Bit 0=Shift, 1=Alt, 2=CTRL:
shift:    .DC.b 0   ;Nichts gedrückt
          .EVEN 
scancode: .DC.l $00000200     ;Zeiger auf Scancode-Tabelle (Vom Loader)
key_buffer:         .DS.b 84


;Dies ist die VBL-Routine aus NEOchrome
vbl:      addq.l    #1,vbl_ct ;Ein VBL mehr
          tst.w     tos_da
          beq.s     kein_tos  ;Nein, Weitermachen
          addq.l    #1,$4ba   ;200Hz erhöhen
kein_tos: move.w    #$2700,sr ;Erstmal Raster initialisieren
          movem.l   d0-d7/a0-a6,-(sp)

          movea.l   rasters,a1          ;von hier die Raster holen
          movem.l   2(a1),d0-d7         ;VBL-Palette holen
          movem.l   d0-d7,$ffff8240     ;und setzen

          lea.l     34(a1),a1 ;ersten Raster nehmen
          move.l    a1,raster_pos       ;Wieder von oben anfangen

          move.w    -34(a1),d0          ;Rasteroffset holen

          clr.b     $fffffa1b ;Timer B Stop!
          move.b    d0,$fffffa21        ;Hier beginnt der 1. Raster
          move.b    #8,$fffffa1b        ;Timer B Go!

vbl_end:  move.w    #$2300,sr ;Neuen VBL wieder zulassen

          tst.b     overrun   ;Gab es einen Overrun von der Tastatur?
          beq.s     keine_maus          ;Ja, keine abs. Maus anfordern
          tst.b     abs_maus  ;Absolute Maus?
          beq.s     keine_maus          ;Nein->nichts abfragen
          tst.b     maus_abfrag         ;Soll Maus abgefragt werden?
          bne.s     keine_maus
          moveq.l   #$0000000d,d0       ;Interrogate Mouse position
          bsr       send_ikbd
keine_maus:         lea.l     vbl_queue,a0
          moveq.l   #9,d7
weitersuchen8:      tst.l     (a0)      ;VBL-Eintrag leer?
          beq.s     leer
          movea.l   (a0),a1
          jsr       (a1)      ;Sonst starten
leer:     addq.l    #4,a0
          dbra      d7,weitersuchen8

          movem.l   (sp)+,d0-d7/a0-a6
          rte       

old_vbl:  .DC.l 0



;Dies ist die neue HBL-Routine, die nicht mit Blitter arbeitet, dafür aber
;absolut flackerfrei arbeitet
sync_hbl: movem.l   d0-d7/a0/a1,register

          movea.l   raster_pos(pc),a0   ;Position des nächsten Rasters
naechster_raster:   clr.b     $fffffa1b ;Timer B Stop!
          move.w    (a0)+,d0  ;Position des nächsten Rasters
          cmpi.b    #1,d0     ;Abstand eins?
          beq.s     kurzer_raster       ;ja, dann im IRQ bleiben
          move.b    d0,$fffffa21        ;Nächsten Raster setzen
          move.b    #8,$fffffa1b        ;und wieder starten

          movem.l   (a0)+,d1-d7/a1      ;Farben holen
          move.l    a0,raster_pos       ;und merken

cpsync_loop:        cmp.b     $fffffa21,d0        ;synchonisieren
          beq.s     cpsync_loop

          nop       
          nop       
          nop       
          nop                 ;Flackern verhindern
          nop       
          nop       

          movem.l   d1-d7/a1,$ffff8240  ;Farben setzen

          movem.l   register,d0-d7/a0/a1
          bclr      #0,$fffffa0f        ;In-Service-Bit löschen
          rte       

raster_pos:         .DC.l 0   ;Position des Rastersystems

kurzer_raster:      clr.b     $fffffa21 ;Timer setzen
          move.b    #8,$fffffa1b        ;und wieder starten

          move.b    $fffffc00,d1        ;ACIA-Status holen
          btst      #0,d1     ;Receiver full, also Byte da?
branch_kurz:        beq.s     get_colors          ;Nein->nichts machen
          btst      #5,d1     ;Overrun?
          beq.s     hols_dir
          clr.b     overrun
hols_dir: movea.l   tastatur_write,a1
          move.b    $fffffc02,(a1)+     ;Wert merken
          move.l    a1,tastatur_write   ;Neue Schreibposition merken

get_colors:         movem.l   (a0)+,d1-d7/a1      ;Farben holen

cpsync_loop56:      tst.b     $fffffa21 ;synchonisieren
          beq.s     cpsync_loop56

          nop       
          nop       
          nop                 ;Flackern verhindern
          nop       
          nop       
          nop       

          movem.l   d1-d7/a1,$ffff8240  ;Farben setzen
          bra       naechster_raster    ;Nein, also normalen Raster setzen


;Platz zum Retten der Register bei Sync_HBL
register: .DS.l 10


;Funktion zum Setzen einer Palette in einer bestimmten Rasterzeile
;C-Definition: set_raster(pal_adr,zeile,res)
mcode23:  link      a6,#0
          movem.l   d0-d7/a0-a6,-(sp)

          lea.l     hbl_system,a0

          move.w    12(a6),d6
          beq.s     vbl_setzen

;Ist derselbe Raster schon vorhanden?
          move.w    #anz_raster-2,d7
nicht_der:          lea.l     34(a0),a0 ;Nächsten Eintrag nehmen
          cmp.w     (a0),d6   ;Ist dieser Raster an dieser Position?
          beq.s     vbl_setzen          ;ja, also Palette setzen
          dbra      d7,nicht_der

;Nein->Neuen Rastereintrag nehmen:
          lea.l     hbl_system,a0
besetzt:  lea.l     34(a0),a0 ;Einen Eintrag weitergehen
          tst.w     (a0)      ;Raster frei?
          bne.s     besetzt

vbl_setzen:         move.w    12(a6),(a0)+        ;Freier Eintrag->Position eintragen
          movea.l   8(a6),a1  ;Farbpalette
          cmpa.l    #0,a1     ;Keine Rasterpalette angegeben? (5)
          beq.s     del_raster          ;ja->Raster löschen (5)
          moveq.l   #15,d7    ;16 Worte übertragen
copy11:   move.w    (a1)+,(a0)+
          dbra      d7,copy11
          bra.s     end_set_raster      ;(5)

del_raster:         clr.w     -(a0)     ;Raster löschen (5)

end_set_raster:     movem.l   (sp)+,d0-d7/a0-a6
          unlk      a6
          rts       


;Fadet eine bereits bestehende Rasterpalette herunter. (6)
;C-Definition: void fade_raster(rasterzeile,anzahl)
mcode72:  link      a6,#0
          movem.l   d0-d7/a0-a5,-(sp)

          lea.l     hbl_system,a0

          move.w    8(a6),d6  ;Rasterzeile
          beq.s     vbl_setzen2

;Ist derselbe Raster schon vorhanden?
          move.w    #anz_raster-2,d7
nicht_der2:         lea.l     34(a0),a0 ;Nächsten Eintrag nehmen
          cmp.w     (a0),d6   ;Ist dieser Raster an dieser Position?
          beq.s     vbl_setzen2         ;ja, also Palette setzen
          dbra      d7,nicht_der2

          bra.s     rausraus

vbl_setzen2:        addq.l    #2,a0
          movea.l   a0,a1     ;merken
          move.w    10(a6),d6
          bra.s     fade_dbra
schleife11:         movea.l   a1,a0
          moveq.l   #1,d7     ;Zuerst "B"-Wert behandeln
          move.w    #%111,d5  ;AND-Maske
          bsr       sub_pal   ;runterschieben
          movea.l   a1,a0
          moveq.l   #16,d7    ;Dann "G"-Wert behandeln
          move.w    #%1110000,d5        ;AND-Maske
          bsr       sub_pal
          movea.l   a1,a0
          move.w    #256,d7   ;Dann "R"-Wert behandeln
          move.w    #%11100000000,d5    ;AND-Maske
          bsr       sub_pal
fade_dbra:          dbra      d6,schleife11

rausraus: movem.l   (sp)+,d0-d7/a0-a5
          unlk      a6
          rts       


;Berechnet die Offsets innerhalb des HBL-System2 neu (CHS):
mcode55:  movem.l   d1-d7/a0-a6,-(sp)

          clr.w     first_pal ;es wird jetzt die erste Raster berechnet

          lea.l     rasters2,a2
          movem.l   hbl_system+2,d0-d7  ;VBL-Palette holen
          movem.l   d0-d7,2(a2)         ;und VBL-Palette eintragen
          lea.l     34(a2),a2 ;und zum 1. Raster gehen

          moveq.l   #1,d7     ;Y-Wert
such_nxt_raster:    move.w    #1000,d6  ;Max-Wert
          move.w    #anz_raster-1,d5    ;Anzahl der zu untersuchenden Raster
          lea.l     hbl_system,a0       ;Tabelle der Rasters
such_min: move.w    (a0),d0   ;Y-Position dieses Rasters
          beq.s     nxt_raster          ;unbenutzt->nächsten
          cmp.w     d6,d0     ;Dieser Raster weiter oben?
          bge.s     nxt_raster          ;Nein->nächsten
          cmp.w     d7,d0     ;weiter unten als letzter Raster?
          ble.s     nxt_raster          ;Nein->nächsten
          lea.l     2(a0),a1  ;Position der Palette merken
          move.w    d0,d6     ;als neuen Kleinsten nehmen
nxt_raster:         lea.l     34(a0),a0 ;nächsten Raster nehmen
          dbra      d5,such_min

          cmpi.w    #1000,d6  ;Alle Raster gesetzt?
          beq.s     fertig

          move.w    d6,d1     ;Position der aktuellen Rasters
          sub.w     d7,d1     ;-letzte Position=Offset
          move.w    d1,-34(a2)          ;Beim letzten Raster als Offset eintragen
          tst.w     first_pal ;Ist es der 1. Raster?
          bne.s     weiter237 ;Nein, eintragen
          addq.w    #1,d1     ;Sonst eins tiefer
          move.w    #-1,first_pal       ;Jetzt nichtmehr die 1. Raster

weiter237:          addq.w    #2,a2     ;und diesen Offset überspringen
          moveq.l   #15,d1
copy2367: move.w    (a1)+,(a2)+         ;Farbpalette übertragen
          dbra      d1,copy2367

          move.w    d6,d7     ;Position des aktiven Rasters->letzter
          bra.s     such_nxt_raster

fertig:   clr.w     -34(a2)   ;Letzter Offset=0

          movem.l   (sp)+,d1-d7/a0-a6
          rts       


;Kopiert Rasters1 -> Rasters2 (CHS):
rst12rst2:          movem.l   d0-d7/a0-a6,-(sp)
          lea.l     rasters1,a0         ;Raster1->Rasters2
          lea.l     rasters2,a1
          bra.s     copy_them

;Kopiert Rasters2 -> Rasters1 (CHS):
rst22rst1:          movem.l   d0-d7/a0-a6,-(sp)
          lea.l     rasters2,a0         ;Rasters2->Rasters1
          lea.l     rasters1,a1
copy_them:          move.w    #anz_raster-1,d6    ;max .Anzahl der Raster
copy_raster:        moveq.l   #16,d7    ;17 Worte übertragen
copy_pal: move.w    (a0)+,(a1)+
          dbra      d7,copy_pal
          dbra      d6,copy_raster
          movem.l   (sp)+,d0-d7/a0-a6
          rts       


;Rechnet die Offsets neu aus, und zeigt die Raster dann an
mcode56:  movem.l   d0-d7/a0-a6,-(sp)

          bsr       mcode55   ;und neu ausrechnen
          move.l    #rasters2,rasters   ;und neue anzeigen
          bsr       vsync
          bsr       rst22rst1 ;und zurück kopieren
          move.l    #rasters1,rasters   ;und 1. System wieder anzeigen
          bsr       vsync

          movem.l   (sp)+,d0-d7/a0-a6
          rts       


;Diese Funktion schaltet die Raster aus
;C-Definition: clear_interrupts()
mcode24:  movem.l   d7/a0,-(sp)

          lea.l     hbl_system,a0
          move.w    #anz_raster-1,d7
clear_lp: clr.w     (a0)
          lea.l     34(a0),a0
          dbra      d7,clear_lp

          lea.l     hbl_system+2,a0
          moveq.l   #7,d7     ;16 Worte
clear_lp2:          clr.l     (a0)+
          dbra      d7,clear_lp2        ;VBL-Palette schwarz

          movem.l   (sp)+,d7/a0
          rts       

;Diese Funktion wartet einen Vsync ab:
mcode62:  
vsync:    move.l    d0,-(sp)

          move.l    vbl_ct,d0
warte_sync:         cmp.l     vbl_ct,d0
          beq.s     warte_sync

          move.l    (sp)+,d0
          rts       

;Routine blendet das Bild langsam aus
;C-Definition: fade_out()
mcode25:  movem.l   d0-d7/a0-a6,-(sp)

          moveq.l   #6,d6     ;7 mal subtrahieren
schleife1:          moveq.l   #1,d7     ;Zuerst "B"-Wert behandeln
          move.w    #%111,d5  ;AND-Maske
          bsr       subtract  ;runterschieben
          moveq.l   #16,d7    ;Dann "G"-Wert behandeln
          move.w    #%1110000,d5        ;AND-Maske
          bsr       subtract
          move.w    #256,d7   ;Dann "R"-Wert behandeln
          move.w    #%11100000000,d5    ;AND-Maske
          bsr       subtract

          bsr       vsync
          bsr       vsync
          bsr       vsync
          dbra      d6,schleife1        ;alle 8 mal

          movem.l   (sp)+,d0-d7/a0-a6
          rts       

subtract: lea.l     rasters1+2,a0
pal_loop: bsr       sub_pal
          addq.l    #2,a0
          tst.w     -36(a0)   ;Kommt noch ein Raster?
          bne.s     pal_loop
          rts       

sub_pal:  moveq.l   #15,d3    ;16 Farben pro Palette
col_loop: bsr       sub_col   ;Farbe runterziehen
          dbra      d3,col_loop         ;alle 16 Farben
          rts       

sub_col:  move.w    (a0)+,d0  ;Farbe holen
          move.w    d0,d1     ;kopieren
          and.w     d5,d1     ;ausmaskieren
          beq       raus56    ;schon auf Null, nichts mehr machen

          sub.w     d7,d0     ;Farbwert eins runter
          move.w    d0,-2(a0) ;Farbe zurückschreiben
raus56:   rts       


;Diese Routine blendet das Bild wieder weich ein
;C-Definition: fade_in();
mcode26:  movem.l   d0-d7/a0-a6,-(sp)

          bsr       mcode55   ;Offsets berechnen

          lea.l     rasters1,a0
          move.w    #anz_raster-1,d7
cler:     moveq.l   #16,d6
cler2:    clr.w     (a0)+     ;Farbpaletten auf schwarz
          dbra      d6,cler2
          dbra      d7,cler

          lea.l     rasters2,a0
          lea.l     rasters1,a1
          move.w    #anz_raster-1,d7
copy88:   move.w    (a0),(a1) ;Offset übertragen
          lea.l     34(a0),a0
          lea.l     34(a1),a1
          dbra      d7,copy88

          moveq.l   #6,d6     ;7 mal addieren
schleife3:          moveq.l   #1,d7     ;Zuerst "B"-Wert behandeln
          move.w    #%111,d5  ;AND-Maske
          bsr       addiere   ;runterschieben
          moveq.l   #16,d7    ;Dann "G"-Wert behandeln
          move.w    #%1110000,d5        ;AND-Maske
          bsr       addiere
          move.w    #256,d7   ;Dann "R"-Wert behandeln
          move.w    #%11100000000,d5    ;AND-Maske
          bsr       addiere

          bsr       vsync
          bsr       vsync
          bsr       vsync
          dbra      d6,schleife3        ;alle 8 mal
          movem.l   (sp)+,d0-d7/a0-a6
          rts       

addiere:  lea.l     rasters1+2,a0       ;Wird verändert
          lea.l     rasters2+2,a1       ;Endwert
pal_loop2:          bsr       add_pal
          addq.l    #2,a0
          addq.l    #2,a1     ;Offsets überspringen
          tst.w     -36(a0)   ;Kommt noch ein Raster
          bne.s     pal_loop2
          rts       

add_pal:  move.w    #15,d3    ;16 Farben pro Palette
col_loop2:          
          move.w    (a0)+,d0  ;Farbe holen
          move.w    (a1)+,d2  ;Endwert holen
          move.w    d0,d1     ;kopieren
          and.w     d5,d1     ;ausmaskieren
          and.w     d5,d2     ;ausmaskieren
          cmp.w     d2,d1     ;Endwert erreicht?
          beq       raus24

          add.w     d7,d0     ;Farbwert eins hoch
          move.w    d0,-2(a0) ;Farbe speichern
raus24:   
          dbra      d3,col_loop2        ;alle 16 Farben
          rts       


; Fehlerbehandlung:
toomany:  moveq.l   #-1,d0    ; zu viele Tasks
          rts       



;          "C O M P R E S S O R"
; ( für Nightrider Software )
; komprimiert farbige Bilder sehr platzsparend.
; Algorithmus besser als Degas Elite!

; (c) Stefan Schreiber  1987/88
; V 2.0

;packt ein Bild platzsparend
;C-Definition: compress(zeilen,quelle,ziel);

mcode27:  link      a6,#0
          movem.l   d1-d7/a0-a6,-(sp)

          lea.l     bitfeld,a0
          move.w    #1999,d0
schleife2:          
          clr.w     (a0)+
          dbra      d0,schleife2

          movem.l   d3-d7,quicksave
          move.w    8(a6),d0  ; Länge des Bilds in Zeilen ( 1-200 )
          movea.l   10(a6),a0 ; Quelladdresse
          movea.l   14(a6),a1 ; Zieladdr., in die kompr. File geschrieben wird

          move.w    d0,(a1)+  ; Zeilenlänge: 1. Wort nach dem Vorspann
          movea.l   a0,a2
          mulu.w    #160,d0   ; Zeilenzahl * 160 = Anzahl der Bytes
          move.w    d0,pic_length
          ext.l     d0
          adda.l    d0,a2
          move.l    a2,endsource

          clr.w     d0        ; Testwert
          clr.w     d1        ; Pos
          moveq.l   #1,d2     ; Counter für Bytewiederholungen
          clr.w     d3        ; HPos
          clr.w     d4
          clr.w     d5        ; Offset
          clr.w     d6        ; Lastpos

getbyte:  
          bsr       belegung
          bne       prepare_test
          move.b    0(a0,d1.w),d0       ; Test%
          move.w    d1,d5     ; hpos: Startwert = Pos

loop28:   
          addi.w    #160,d5
          lea.l     0(a0,d5.w),a2
          cmpa.l    endsource,a2
          bcc       exit28
          cmp.b     (a2),d0   ; = Testwert?
          bne       exit28
          addq.w    #1,d2     ; Counter erhöhen
          cmpi.w    #255,d2   ; >=255?
          bcs       loop28

exit28:   
          cmpi.w    #3,d2
          bls       prepare_test
          move.b    d6,(a1)+  ; current offset
          move.b    d2,(a1)+  ; Anz. Wiederholungen
          move.b    d0,(a1)+  ; Testwert!
          bsr       set_flags
          clr.w     d6

prepare_test:       
          moveq.l   #1,d2     ; !!!
          addq.w    #1,d6     ; c_offset
          addq.w    #1,d1     ; inc pos
          cmpi.w    #255,d6
          bcs       cont

          move.b    d6,(a1)+  ; #255-Offset
          clr.w     d6

cont:     
          cmp.w     pic_length,d1
          bcs       getbyte

          move.b    #0,(a1)+  ; Doppelnull als Vorspannende
          move.b    #0,(a1)+

          clr.w     d1        ; Pos = 0!
          bsr       restloop
          movea.l   14(a6),a0 ; Start des Zielpuffers
          suba.l    a0,a1     ; Pufferende-Pufferanfang = Filelänge
          move.l    a1,d0     ; Rückgabewert für Hauptprogramm
          movem.l   quicksave,d3-d7

          movem.l   (sp)+,d1-d7/a0-a6
          unlk      a6
          rts       

; d0 enthält Filelänge

restloop: 
          bsr       belegung
          bne       cont2
          move.b    0(a0,d1.w),(a1)+    ; Byte in Puffer
cont2:    
          addq.w    #1,d1
          cmp.w     pic_length,d1
          bcs       restloop
          rts                 ; Es fehlt die Pufferlänge!!!

belegung: 
          lea.l     bitfeld,a2
          move.w    d1,d3
          lsr.w     #3,d3     ; d3 \= 8       ( C-Notation )
          btst      d1,0(a2,d3.w)
          rts       

set_flags:          
          move.w    d1,d3     ; cpos
          lsr.w     #3,d3
          lea.l     bitfeld,a2
          subq.w    #1,d2     ; Wiederholungen-1
loop29:   
          bset      d1,0(a2,d3.w)       ; d3 ist noch aus Proc. Belegung gesetzt!
          addi.w    #20,d3    ; 160/8
          dbra      d2,loop29
          rts       

; *************************************************

;Entpackt ein Grafikbild
;C-Definition: entpack(quelle,ziel)

mcode28:  movem.l   d0-d7/a0-a6,-(sp)

          lea.l     bitfeld,a0          ; Flags löschen
          move.w    #249,d0
loop30:   
          clr.l     (a0)+
          clr.l     (a0)+
          clr.l     (a0)+
          clr.l     (a0)+
          dbra      d0,loop30

          movea.l   64(sp),a0 ; Puffer ( Quelladdresse )
          movea.l   68(sp),a1 ; Zieladdresse: meistens Bildschirmspeicher

          movem.l   d3-d7,quicksave
          lea.l     bitfeld,a2
          move.w    (a0)+,d0  ; Bildlänge in Zeilen
          mulu.w    #160,d0   ; *160
          move.w    d0,pic_length

          clr.w     d1
          clr.w     d2
          clr.w     d3
          clr.w     d4
          clr.w     d5
          clr.w     d6

decomp:   
          move.b    (a0)+,d4  ; Offset zu nächsten Block
          beq       nulltest
cycle:    
          add.w     d4,d1     ; Pos

          cmpi.b    #255,d4
          beq       decomp

          move.b    (a0)+,d2  ; Counter
          move.w    d2,d6     ; Zwischenspeicher
          subq.b    #1,d2
          move.b    (a0)+,d0  ; Wert
          move.w    d1,d5     ; d5 : HPos

loop31:   
          move.b    d0,0(a1,d5.w)
          addi.w    #160,d5   ; nächste Bytewiederholung
          dbra      d2,loop31

          move.w    d6,d2
          bsr       set_flags ; Belegung
          clr.w     d2        ; !!!
          bra       decomp    ; neuer Zyklus

nulltest: 
          tst.b     (a0)      ; 2*0-Byte ?
          bne       cycle     ; Zyklus weitermachen

exit3:    
          addq.l    #1,a0
          clr.w     d1
          bsr       restloop2
          movem.l   quicksave,d3-d7

          movem.l   (sp)+,d0-d7/a0-a6
          rts       

restloop2:          
          bsr       belegung
          bne       cont6
          move.b    (a0)+,0(a1,d1.w)    ; Byte in Puffer
cont6:    
          addq.w    #1,d1
          cmp.w     pic_length,d1
          bcs       restloop2
          rts       


;Funktion setzt Form des Mauszeigers
;C-Definition: setmouse_form(nr)
mcode29:  move.w    4(sp),mouseform
          rts       

;Funktion holt sich einen Wert aus der Trafficmap
;C-Definition: get_trafficxy(land,x,y)
mcode30:  link      a6,#0
          movem.l   d1-d4/d7/a0,-(sp)

          move.w    10(a6),d0 ;x
          move.w    12(a6),d1 ;y
          tst.w     d0
          bmi       traf_err
          cmpi.w    #319,d0
          bgt       traf_err
          cmpi.w    #21,d1
          blt       traf_err
          cmpi.w    #168,d1
          bgt       traf_err

          moveq.l   #0,d3
          cmpi.w    #25,d1
          blt       schultz_jetzt

          subi.w    #25,d1
          lsr.w     #2,d1
          mulu.w    #50,d1

          lsr.w     #2,d0
          move.w    d0,d2
          lsl.w     #2,d0     ;mal 4
          add.w     d2,d0     ;+1 = 5mal

          move.w    d0,d3
          lsr.w     #3,d0     ;/8=Byte
          add.w     d1,d0     ;+zeile
          andi.w    #%111,d3
          move.w    #7,d2
          sub.w     d3,d2     ;in d2 steht Bit

          movea.l   trf_buf,a0
          move.w    8(a6),d7  ;land
          mulu.w    #1800,d7
          adda.l    d7,a0     ;+offset

          move.b    0(a0,d0.w),d3       ;op1
          move.b    1(a0,d0.w),d4       ;op2

          subi.w    #4,d2     ;bit-4->d2
          bmi       negativ

          lsr.w     d2,d3
          bra       schultz_jetzt

negativ:  lsl.w     #8,d3     ;op1 ins obere Byte
          move.b    d4,d3     ;op2 ins untere Byte

          move.w    #8,d0
          add.w     d2,d0     ;Achtung! d2 ist negativ!
          lsr.w     d0,d3     ;runterschieben
schultz_jetzt:      andi.w    #31,d3
          move.w    d3,d0

          movem.l   (sp)+,d1-d4/d7/a0
          unlk      a6
          rts       

traf_err: moveq.l   #0,d3     ;Außerhalb der Karte=LEER
          bra.s     schultz_jetzt

;Diese Funktion initialisiert den Scroller
;C-Definition:  initscroller(scr,y,text,font,scroll_buff,copy)
;long scr;
;int y;
;char text[];
;char font[];
;char scroll_buff[]
;FLAG copy;
mcode32:  link      a6,#0
          movem.l   d0-d7/a0-a6,-(sp)

          move.l    8(a6),scrollbase    ;Basis des Bildschirms
          move.w    12(a6),yscroller
          move.l    14(a6),scrollbegin
          move.l    18(a6),font_start
          move.l    22(a6),scroll_buff
          move.w    26(a6),copyscroller ;Flag, ob bei jedem VBL reinkopieren?
          bsr       initscroller

          clr.w     steuerzeichen       ;(1) Kein Steuerzeichen bisher

          clr.w     scroll_akt          ;in Buffer 0 starten
          movea.l   22(a6),a0 ;Scrollbuffer
          move.w    #6719,d7
clerr:    clr.l     (a0)+
          clr.l     (a0)+     ;Buffer löschen
          dbra      d7,clerr

          movem.l   (sp)+,d0-d7/a0-a6
          unlk      a6
          rts       

initscroller:       move.l    scrollbegin,scrollpos
          subq.l    #1,scrollpos        ;ein zeichen zurück
          move.w    #-200,delta_x
          rts       

;Diese Funktion scrollt die Laufschrift um eins weiter
;C-Definition: void doscroller(void)
mcode33:  movem.l   d0-d7/a0-a6,-(sp)

scroll:   cmpi.w    #-2,delta_x         ;Noch Spalten da?
          bgt       char_weiter         ;Ja, Buchstabe fertig machen

          addq.l    #1,scrollpos        ;nächstes Zeichen
          movea.l   scrollpos,a0
          moveq     #0,d0
          move.b    (a0),d0   ;Nächstes Zeichen holen
          bne.s     verarbeite          ;Nicht das letzte, weiter

          bsr       initscroller        ;Wieder von vorne
          bra.s     scroll    ;und starten

verarbeite:         move.w    d0,d1     ;Zeichen merken
          lea       sonder,a0
nxt_element:        tst.b     (a0)      ;Tabelle zuende?
          beq.s     kein_sonder
          cmp.b     (a0)+,d0  ;in Tabelle enthalten?
          bne.s     nxt_element

          suba.l    #sonder+1,a0
          move.w    a0,d0
          addi.w    #26*2,d0  ;Hinter den Groß und Kleinbuchstaben
          bra.s     sonderzeichen

kein_sonder:        cmpi.w    #'Z',d0
          ble.s     nehmen
          subi.w    #'a'-'Z'-1,d0       ;Kleinbuchstaben
nehmen:   subi.w    #'A',d0   ;in internen Code wandeln
          bpl.s     sonderzeichen       ;(1)
          move.w    d1,steuerzeichen    ;(1) und merken
          bra.s     scroll    ;(1)

sonderzeichen:      movea.l   font_start,a0       ;Basis der Bibliothek
          cmp.w     (a0)+,d0  ;Objekt noch in Bibliothek?
          bgt.s     scroll    ;Nein->nächster Buchstabe

          mulu.w    #20,d0    ;Adresse des Ob_Headers berechnen
          adda.l    d0,a0
          move.l    a0,char_basis

          movea.l   font_start,a1       ;Basis der Objekte
          addq.l    #2,a1     ;Anzahl überspringen
          adda.l    (a0),a1   ;=Basis des Objekts
          move.l    a1,adrspalte        ;Datenstart
          move.w    4(a0),d0  ;Breite
          addq.w    #1,d0
          bclr      #0,d0     ;Gerade machen
          move.w    d0,delta_x
          move.w    d0,4(a0)

char_weiter:        movea.l   scroll_buff,a0
          addq.l    #8,a0
          move.w    scroll_akt,d0       ;Aktueller Scroller
          mulu.w    #160*42,d0          ;Adresse berechnen
          adda.l    d0,a0

          moveq     #69,d7
vorschieben:        movem.l   (a0)+,d0-d6/a2-a6
          movem.l   d0-d6/a2-a6,-56(a0)
          movem.l   (a0)+,d0-d6/a2-a6   ;um 8 Bytes nach vorne schieben
          movem.l   d0-d6/a2-a6,-56(a0)
          dbra      d7,vorschieben

          tst.w     delta_x
          bgt.s     kein_space

          lea       zeros,a3
          suba.l    a2,a2
          moveq     #0,d7
          bra.s     scroll_it

kein_space:         movea.l   char_basis,a3
          movea.w   10(a3),a2 ;Breite in Bytes
          move.w    6(a3),d3  ;Höhe in Scanlines
          move.w    4(a3),d0  ;Breite des Objekts
          sub.w     delta_x,d0
          move.w    d0,d7
          lsr.w     #4,d0
          lsl.w     #3,d0
          movea.l   adrspalte,a3
          adda.w    d0,a3     ;Adresse des BB
          andi.w    #15,d7    ;Skew
          neg.w     d7
          addi.w    #14,d7

scroll_it:          subq.w    #2,delta_x          ;eines weiter

          lea       -160*42-8+152(a0),a0          ;Start des Buffers, Zeilenende

          lea       -160*42(a0),a1      ;vorheriger Scroller
          movea.l   scroll_buff,a6
          lea       152(a6),a6
          cmpa.l    a6,a1     ;vor dem 1. Scroller?
          bge.s     positi
          movea.l   scroll_buff,a1
          adda.l    #152+7*160*42,a1    ;Scroller #7
positi:   

          moveq     #41,d5    ;42 Scanlines hoch
all_planes:         moveq     #3,d6     ;4 Planes
scanline: move.w    (a3)+,d0  ;Scroller holen
          lsr.w     d7,d0     ;Bit hinschieben
          andi.w    #3,d0     ;und ausmaskieren

          move.w    (a1)+,d4  ;Bildschirm holen
          lsl.l     #2,d4
          or.w      d0,d4     ;einodern
          move.w    d4,(a0)+  ;und zurück auf Screen
          dbra      d6,scanline

          subq.l    #8,a3     ;zurück zum Start des BB
          adda.l    a2,a3     ;und auf nächste Zeile

          subq.w    #1,d3     ;Eine Scanline weniger
          bgt.s     noch_was_da
          suba.l    a2,a2
          lea       zeros,a3  ;Zeichen ist zu ende
          moveq     #0,d7

noch_was_da:        
          lea       152(a1),a1          ;eine Zeile tiefer
          lea       152(a0),a0
          dbra      d5,all_planes

          movea.l   scrollbase,a1       ;Basis des Bildschirm
          move.w    yscroller,d5
          mulu.w    #160,d5   ;160 Bytes pro Scanline
          adda.l    d5,a1
          adda.w    #160,a1

          lea       -42*160-152(a0),a0  ;zum Start des Buffers
          moveq     #69,d7
vorschieben2:       movem.l   (a0)+,d0-d6/a2-a6
          movem.l   d0-d6/a2-a6,(a1)
          movem.l   (a0)+,d0-d6/a2-a6   ;auf Screen kopieren
          movem.l   d0-d6/a2-a6,48(a1)
          lea       96(a1),a1
          dbra      d7,vorschieben2

          addq.w    #1,scroll_akt       ;nächster Scrollbuffer
          cmpi.w    #8,scroll_akt
          blt.s     okokok
          clr.w     scroll_akt
okokok:   

          tst.w     copyscroller        ;Soll kopiert werden?
          beq.s     no_copy   ;Nein, nicht kopieren

          move.w    #18,-(sp) ;18 Zeilen kopieren
          move.l    scr1,-(sp)          ;Ziel
          move.l    scrollbase,-(sp)
          bsr       mcode15   ;copy_zeilen
          move.l    scr2,4(sp)
          bsr       mcode15
          lea       10(sp),sp

no_copy:  movem.l   (sp)+,d0-d7/a0-a6
          rts       

zeros:    .DC.l 0,0

sonder:   .DC.b ".,:' üäö?ß"  ;(3)
          .DC.b '"$',0        ;(3)

          .EVEN 

;void hippel_sfx(int nr);
;Schaltet einen Hippelsoundeffekt an
mcode38:  link      a6,#0
          movem.l   d0-d7/a0-a6,-(sp)

          moveq.l   #0,d0
          move.w    8(a6),d0
          movea.l   music_routine,a0
          jsr       4(a0)

          movem.l   (sp)+,d0-d7/a0-a6
          unlk      a6
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
mcode47:  link      a6,#0

          move.l    a0,-(sp)  ;a0 retten

          movea.l   10(a6),a0 ;Basis der Bibliothek
          move.w    8(a6),d0  ;Nummer des Objekts
          cmp.w     (a0)+,d0  ;Objekt noch in Bibliothek?
          bgt.s     zu_gross1 ;Nein->nichts zeichnen

          mulu.w    #20,d0    ;Adresse des Ob_Headers berechnen
          adda.l    d0,a0

          move.l    16(a0),d0 ;reserved
          lsr.w     #8,d0
          beq.s     ohne_raster

          movea.l   10(a6),a1 ;Basis der Objekte
          addq.l    #2,a1     ;Anzahl überspringen
          adda.l    (a0),a1   ;=Basis des Objekts
          move.w    6(a0),d1  ;Höhe des Objekts
          mulu.w    10(a0),d1 ;*Breite=Anzahl Bytes
          adda.l    d1,a1     ;=Start der Raster
          bra.s     set_dbra
set_loop: move.w    (a1)+,d1  ;Offset des Rasters holen
          add.w     22(a6),d1 ;+Y auf Screen
          clr.w     -(sp)     ;Low-Res
          move.w    d1,-(sp)  ;Position des Rasters
          move.l    a1,-(sp)
          bsr       mcode23   ;set_raster
          addq.l    #8,sp
          lea.l     32(a1),a1 ;Palette überspringen
set_dbra: dbra      d0,set_loop

ohne_raster:        
          cmpi.w    #2,14(a6) ;Modus=Nur Palette? (4)
          beq.s     zu_gross1 ;(4)

          move.l    20(a6),-(sp)        ;x,y
          move.l    16(a6),-(sp)        ;scr_basis
          move.w    14(a6),-(sp)        ;modus
          move.w    6(a0),d0  ;Höhe des Objekts
          subq.w    #1,d0
          move.w    d0,-(sp)  ;(Höhe-1)->y2
          move.w    4(a0),d0  ;Breite des Objekts
          subq.w    #1,d0
          move.w    d0,-(sp)  ;(Breite-1)->x2
          clr.l     -(sp)     ;x1,y1
          move.l    10(a6),-(sp)        ;Ob_Basis
          move.w    8(a6),-(sp)         ;Ob_Nr
          bsr.s     mcode48   ;Draw_Obj_Part

zu_gross1:          movea.l   (sp)+,a0  ;a0 restaurieren

          unlk      a6
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
mcode48:  link      a6,#0
          movem.l   d0-d7/a0-a5,-(sp)

          movea.l   10(a6),a5 ;Basis der Bibliothek
          move.w    8(a6),d0  ;Nummer des Objekts
          cmp.w     (a5)+,d0  ;Objekt noch in Bibliothek?
          bgt       zu_gross2 ;Nein->nichts zeichnen

          mulu.w    #20,d0    ;Adresse des Ob_Headers berechnen
          adda.l    d0,a5

          movea.l   10(a6),a0 ;Basis der Objekte
          addq.l    #2,a0     ;Anzahl überspringen
          adda.l    (a5),a0   ;Adresse der Objekt-Daten

          move.w    #320,-(sp)          ;Breite des Bildschirms in Pixel
          move.l    24(a6),-(sp)        ;Screenbasis
          move.w    30(a6),-(sp)        ;y
          move.w    28(a6),-(sp)        ;x
          move.w    10(a5),d0 ;Breite des Objekts in Bytes
          add.w     d0,d0     ;breite in Pixel
          move.w    d0,-(sp)  ;Breite des Objekts in Pixel merken
          pea       (a0)      ;Adresse des Objektes
          move.w    20(a6),-(sp)        ;y2
          move.w    18(a6),-(sp)        ;x2
          move.w    16(a6),-(sp)        ;y1
          move.w    14(a6),-(sp)        ;x1
          tst.w     22(a6)    ;modus
          beq.s     replace
          bsr       mcode50   ;raster_transparent
          bra.s     end_draw_obj
replace:  bsr       mcode49   ;raster_replace
end_draw_obj:       lea.l     24(sp),sp

zu_gross2:          movem.l   (sp)+,d0-d7/a0-a5
          unlk      a6
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
; 30(a6): Breite einer Scanline in Pixel (Destination)
;  26(a6): Adresse des Bildschirms (Destination)
;  24(a6): y3 (Destination)
;  22(a6): x3 (Destination)
;  20(a6): Breite einer Scanline in Pixel (Source)
;  16(a6): Adresse des Bildschirms (Source)
;  14(a6): y2 (Source)
;  12(a6): x2 (Source)
;  10(a6): y1 (Source)
;   8(a6): x1 (Source)
mcode49:  link      a6,#-12

          movem.l   d0-d7/a0-a5,-(sp)   ;Register retten

          move.w    8(a6),d0  ;Source X_Min
          move.w    10(a6),d1 ;Source Y_Min
          move.w    12(a6),d2 ;Source X_Max
          move.w    14(a6),d3 ;Source Y_Max
          move.w    22(a6),d4 ;Dest X_Min
          bpl.s     koord_ok1
          sub.w     d4,d0     ;neg->zu Source X_Min addieren
          move.w    d0,8(a6)  ;zurück in Parameter
          moveq.l   #0,d4     ;Clippen
          move.w    d4,22(a6)
koord_ok1:          move.w    24(a6),d5 ;Dest Y_Min
          bpl.s     koord_ok2
          sub.w     d5,d1     ;neg->zu Source Y_Min addieren
          move.w    d1,10(a6) ;zurück in Parameter
          moveq.l   #0,d5
          move.w    d5,24(a6)
koord_ok2:          move.w    d2,d7     ;Source X_Max
          move.w    d4,d6     ;Dest X_Min
          sub.w     d0,d7     ;Source X_Max-Source X_min=Breite-1
          add.w     d7,d6     ;=Dest X_Max
          move.w    30(a6),d7 ;Breite in Pixel (Dest)
          subq.w    #1,d7     ;bis einschliesslich hier
          sub.w     d6,d7     ;von 320 abziehen
          bpl.s     koord_ok3
          add.w     d7,d6     ;von Dest X_Max abziehen
          add.w     d7,d2     ;und von Source X_Max
          move.w    d2,12(a6) ;zurück in Parameter
koord_ok3:          sub.w     d1,d3     ;Höhe-1
          add.w     d3,d5     ;=Dest Y_Max
          move.w    #199,d3   ;Unterer Rand
          sub.w     d5,d3
          bpl.s     koord_ok4
          add.w     d3,d5     ;von Dest Y_Max abziehen
          add.w     d3,14(a6) ;von Source Y_Max abziehen
koord_ok4:          move.w    d6,-2(a6) ;Dest X_Max merken
          move.w    d5,-4(a6) ;Dest Y_Max merken

          move.w    12(a6),d5 ;Source X_Max
          sub.w     8(a6),d5  ;-Source X_Min
          bmi       nichts_da7
          move.w    14(a6),d5 ;Source Y_Max
          sub.w     10(a6),d5 ;-Source Y_Min
          bmi       nichts_da7

          moveq.l   #$0000000f,d5
          move.w    d0,d1     ;Source X_Min
          and.w     d5,d1
          move.w    d4,d3     ;Dest X_Min
          and.w     d5,d3
          lsr.w     #4,d0     ;# des BBs (Source X_Min)
          lsr.w     #4,d4     ;# des BBs (Dest X_Min)
          lsr.w     #4,d2     ;# des BBs (Source X_Max)
          lsr.w     #4,d6     ;# des BBs (Dest X_Max)
          sub.w     d0,d2     ;X_Max-X_Min=Breite in BBs-1 (Source)
          sub.w     d4,d6     ;-'- (Dest)
          move.w    d2,d5     ;Breite des Sourceblocks
          sub.w     d6,d5     ;-Breite des Zielblocks
          andi.w    #1,d5     ;NFSR?
          lsl.w     #2,d5     ;an richtige Position schieben
          sub.w     d3,d1     ;Skew
          move.w    d6,d0     ;Breite des Zielblocks in BBs
          subq.w    #1,d0
          move.w    d0,-6(a6) ;und merken
          move.w    d2,d7     ;Breite des Sourceblocks in BBs-1
          move.w    d1,d4     ;Skew
          move.w    d1,d2     ;Skew
          tst.w     d4        ;Skew rechts oder links?
          bgt.s     links_rum ;ja
          beq.s     weiter1
          neg.w     d4        ;Skew positiv machen
          addq.w    #1,d5     ;und merken
links_rum:          cmpi.w    #8,d4     ;Skew weniger als 8?
          blt.s     weiter1
          addq.w    #2,d5     ;und merken, besser andersrum shiften(!)
          neg.w     d4        ;Skew umdrehen, also
          addi.w    #16,d4    ;von 16 abziehen
weiter1:  move.w    12(a6),d0 ;Source X_Max
          move.w    14(a6),d1 ;Source Y_Max
          bsr       calc_quelle         ;von hinten reinkommen
          lsl.w     #3,d7
          sub.w     d3,d7     ;Breite einer Zeile in Bytes abziehen(?)
          move.w    d7,-8(a6) ;Offsets am Zeilenende merken
          move.w    -2(a6),d0 ;Dest X_Max
          move.w    -4(a6),d1 ;Dest Y_Max
          bsr       calc_ziel ;auch von hinten rein
          lsl.w     #3,d6     ;Breite in BBs->Breite in Bytes
          sub.w     d3,d6     ;Breite einer Zeile in Bytes abziehen(?)
          move.w    d6,-10(a6)          ;Offset am Zeilenende merken
          bsr       get_masken          ;Masken->D6
          swap.w    d6        ;Masken vertauschen
          asl.w     #3,d5     ;je nach Konstellation richtige Routine
          movea.l   L0009(pc,d5.w),a3
          movea.l   L0008(pc,d5.w),a4
          tst.w     -6(a6)    ;Breite des Zielblocks in BBs>=2?
          bgt       start_copy          ;ja
          blt.s     klein
          bclr      #3,d5     ;Skew nach rechts?
          beq       start_copy          ;Nein, dann okay
          lsr.w     #2,d5     ;nur noch mal 4
          movea.l   rechts_klein(pc,d5.w),a4      ;Spezialroutine
          bra       start_copy

;Tabelle für Objekte, die genau 2 BBs breit sind, und nach rechts geschoben
;werden, da es für die nachfolgenden Routine (L0037) nicht möglich war an
;bestimmte Informationen heranzukommen.
rechts_klein:       .DC.l L0023b
          .DC.l L0025b
          .DC.l L0024b
          .DC.l L0026b

;Routinen für nur einen BB breite Objekte:
L0007:    .DC.l L0021
          .DC.l L0022
          .DC.l L0022
          .DC.l L0021

;Routinen für mindestens 2 BB breite Objekte
L0008:    .DC.l L0026         ;4  = 0
L0009:    .DC.l L0030
          .DC.l L0023         ;5  = 1 -
          .DC.l L0033
          .DC.l L0024         ;6  = 2
          .DC.l L002F
          .DC.l L0025         ;7  = 3 -
          .DC.l L0034
          .DC.l L0025         ;12  = 4
          .DC.l L0030
          .DC.l L0024         ;13  = 5 -
          .DC.l L0033
          .DC.l L0023         ;14  = 6
          .DC.l L002F
          .DC.l L0026         ;15  = 7 -
          .DC.l L0034

klein:    move.l    d6,d0     ;Maske
          swap.w    d0
          and.w     d0,d6     ;linker und rechter Rand verknüpfen
          lea.l     nxt_scanline,a3
          btst      #5,d5     ;NFSR?
          bne       start_copy          ;Ja->Normal arbeiten
          lsr.w     #1,d5     ;nur noch mal 4
          andi.w    #%0000000000001100,d5
          movea.l   L0007(pc,d5.w),a4   ;Sonst andere Routine nehmen
start_copy:         move.w    14(a6),d5 ;y2
          sub.w     10(a6),d5 ;-y1=Höhe in Pixeln-1
          addq.w    #1,d5     ;=Höhe in Pixeln
          swap.w    d5
          move.w    -6(a6),d5 ;Breite des Zielblocks in BBs-2
          addq.l    #8,a0     ;Ans Ende des BB gehen
          addq.l    #8,a1     ;-'-
          tst.w     d4        ;Skew=0?
          beq       no_shift
          jmp       (a4)      ;und Plane bearbeiten

get_masken:         move.w    -2(a6),d0 ;Dest X_Max
          andi.w    #$000f,d0
          add.w     d0,d0
          move.w    L0015(pc,d0.w),d6   ;Maske holen
          swap.w    d6
          move.w    22(a6),d0 ;Dest X_Min
          andi.w    #$000f,d0
          add.w     d0,d0
          move.w    L0014(pc,d0.w),d6   ;Maske holen
          not.w     d6        ;und inverieren
          rts       

L0014:    .DC.w %0000000000000000
L0015:    .DC.w %1000000000000000
          .DC.w %1100000000000000
          .DC.w %1110000000000000
          .DC.w %1111000000000000
          .DC.w %1111100000000000
          .DC.w %1111110000000000
          .DC.w %1111111000000000
          .DC.w %1111111100000000
          .DC.w %1111111110000000
          .DC.w %1111111111000000
          .DC.w %1111111111100000
          .DC.w %1111111111110000
          .DC.w %1111111111111000
          .DC.w %1111111111111100
          .DC.w %1111111111111110
          .DC.w %1111111111111111

;Routine berechnet die Startadresse des Quellrasters relativ zu:
; D0: X
;  D1: Y
; A0: Quellrasteradresse
calc_quelle:        movea.l   16(a6),a0 ;Anfangsadresse Quellraster
          move.w    20(a6),d3 ;Breite Quelle in Pixel
          lsr.w     #1,d3     ;Breite in Bytes
          lsr.w     #4,d0     ;# des BBs berechnen
          lsl.w     #3,d0
          muls.w    d3,d1     ;Y Offset ausrechnen
          adda.l    d1,a0
          adda.w    d0,a0     ;und dazuaddieren
          rts       

;Routine berechnet die Startadresse des Zielrasters relativ zu:
; D0: X
;  D1: Y
; A1: Zielrasteradresse
calc_ziel:          movea.l   26(a6),a1 ;Anfangsadresse Zielraster
          move.w    30(a6),d3 ;Breite Ziel in Pixel
          lsr.w     #1,d3     ;Breite in Bytes berechnen
          lsr.w     #4,d0     ;# des BBs berechnen
          lsl.w     #3,d0
          muls.w    d3,d1     ;Y Offset berechnen
          adda.l    d1,a1
          adda.w    d0,a1     ;und dazuaddieren
          rts       

;Spezialroutine falls Skew=0:
no_shift: lea.l     no_shift1(pc),a4    ;Bei nächster Scanline diese Routine
          move.l    d6,d0     ;Maske retten
          swap.w    d6
          move.w    d6,d7     ;obere Maske->D7
          swap.w    d7
          move.w    d6,d7     ;->D7
          move.w    d0,d6     ;untere Maske->D6

no_shift1:          move.l    -(a0),d0  ;Plane 2 und 3 holen (12)
          move.l    -(a0),d1  ;Plane 0 und 1 holen (12)
          move.l    -(a1),d2  ;Screen holen   (12)
          move.l    -(a1),d3
          eor.l     d2,d0
          and.l     d6,d0
          eor.l     d2,d0
          eor.l     d3,d1
          and.l     d6,d1
          eor.l     d3,d1
          move.l    d0,4(a1)
          move.l    d1,(a1)
          tst.w     d5        ;nur ein BB?
          bmi       nxt_scan2 ;ja, -> Weiter
          bra       bb_dbra2

bb_mitte2:          move.l    -(a0),-(a1)         ;Plane 2 und 3 holen (12)
          move.l    -(a0),-(a1)         ;Plane 0 und 1 holen (12)
bb_dbra2: dbra      d5,bb_mitte2        ;zählt bis auf 0 runter

          move.l    -(a0),d0  ;Plane 2 und 3 holen (12)
          move.l    -(a0),d1  ;Plane 0 und 1 holen (12)
          move.l    -(a1),d2  ;Screen holen   (12)
          move.l    -(a1),d3
          eor.l     d2,d0
          and.l     d7,d0
          eor.l     d2,d0
          eor.l     d3,d1
          and.l     d7,d1
          eor.l     d3,d1
          move.l    d0,4(a1)
          move.l    d1,(a1)
          bra       nxt_scan2

L0021:    move.w    -(a0),d0
          rol.w     d4,d0
          move.w    -(a1),d7
          eor.w     d7,d0
          and.w     d6,d0
          eor.w     d7,d0
          move.w    d0,(a1)

          move.w    -(a0),d0
          rol.w     d4,d0
          move.w    -(a1),d3
          eor.w     d3,d0
          and.w     d6,d0
          eor.w     d3,d0
          move.w    d0,(a1)

          move.w    -(a0),d0
          rol.w     d4,d0
          move.w    -(a1),d2
          eor.w     d2,d0
          and.w     d6,d0
          eor.w     d2,d0
          move.w    d0,(a1)

          move.w    -(a0),d0
          rol.w     d4,d0
          move.w    -(a1),d1
          eor.w     d1,d0
          and.w     d6,d0
          eor.w     d1,d0
          move.w    d0,(a1)
          jmp       (a3)

L0022:    move.w    -(a0),d0
          ror.w     d4,d0
          move.w    -(a1),d7
          eor.w     d7,d0
          and.w     d6,d0
          eor.w     d7,d0
          move.w    d0,(a1)

          move.w    -(a0),d0
          ror.w     d4,d0
          move.w    -(a1),d3
          eor.w     d3,d0
          and.w     d6,d0
          eor.w     d3,d0
          move.w    d0,(a1)

          move.w    -(a0),d0
          ror.w     d4,d0
          move.w    -(a1),d2
          eor.w     d2,d0
          and.w     d6,d0
          eor.w     d2,d0
          move.w    d0,(a1)

          move.w    -(a0),d0
          ror.w     d4,d0
          move.w    -(a1),d1
          eor.w     d1,d0
          and.w     d6,d0
          eor.w     d1,d0
          move.w    d0,(a1)
          jmp       (a3)

L0023:    move.w    -(a0),d7  ;Plane aus Objekt holen
          move.w    -(a0),d3
          move.w    -(a0),d2
          move.w    -(a0),d1
          swap.w    d7
          swap.w    d3
          swap.w    d2
          swap.w    d1
L0024:    move.w    -(a0),d7  ;nächste Plane aus Objekt
          move.w    -(a0),d3
          move.w    -(a0),d2
          move.w    -(a0),d1
          swap.w    d7
          swap.w    d3
          swap.w    d2
          swap.w    d1

          move.l    d7,d0     ;ins Schieberegister
          ror.l     d4,d0     ;runterschieben
          move.w    -(a1),d7  ;Screen holen
          eor.w     d7,d0
          and.w     d6,d0
          eor.w     d7,d0
          move.w    d0,(a1)   ;und zurück auf Screen

          move.l    d3,d0     ;ins Schieberegister
          ror.l     d4,d0     ;runterschieben
          move.w    -(a1),d3  ;Screen holen
          eor.w     d3,d0
          and.w     d6,d0
          eor.w     d3,d0
          move.w    d0,(a1)   ;und zurück auf Screen

          move.l    d2,d0     ;ins Schieberegister
          ror.l     d4,d0     ;runterschieben
          move.w    -(a1),d2  ;Screen holen
          eor.w     d2,d0
          and.w     d6,d0
          eor.w     d2,d0
          move.w    d0,(a1)   ;und zurück auf Screen

          move.l    d1,d0     ;ins Schieberegister
          ror.l     d4,d0     ;runterschieben
          move.w    -(a1),d1  ;Screen holen
          eor.w     d1,d0
          and.w     d6,d0
          eor.w     d1,d0
          move.w    d0,(a1)   ;und zurück auf Screen

          jmp       (a3)

L0025:    move.w    -(a0),d7  ;Plane aus Objekt holen
          move.w    -(a0),d3
          move.w    -(a0),d2
          move.w    -(a0),d1
          swap.w    d7
          swap.w    d3
          swap.w    d2
          swap.w    d1
L0026:    move.w    -(a0),d7
          move.w    -(a0),d3
          move.w    -(a0),d2
          move.w    -(a0),d1
          move.l    d7,d0     ;ins Schieberegister
          swap.w    d7
          rol.l     d4,d0     ;schieben
          move.w    -(a1),d7  ;Screen holen
          eor.w     d7,d0
          and.w     d6,d0
          eor.w     d7,d0
          move.w    d0,(a1)

          move.l    d3,d0     ;ins Schieberegister
          swap.w    d3
          rol.l     d4,d0     ;schieben
          move.w    -(a1),d3  ;Screen holen
          eor.w     d3,d0
          and.w     d6,d0
          eor.w     d3,d0
          move.w    d0,(a1)

          move.l    d2,d0     ;ins Schieberegister
          swap.w    d2
          rol.l     d4,d0     ;schieben
          move.w    -(a1),d2  ;Screen holen
          eor.w     d2,d0
          and.w     d6,d0
          eor.w     d2,d0
          move.w    d0,(a1)

          move.l    d1,d0     ;ins Schieberegister
          swap.w    d1
          rol.l     d4,d0     ;schieben
          move.w    -(a1),d1  ;Screen holen
          eor.w     d1,d0
          and.w     d6,d0
          eor.w     d1,d0
          move.w    d0,(a1)
          jmp       (a3)

L0023b:   move.w    -(a0),d7
          move.w    -(a0),d3
          move.w    -(a0),d2
          move.w    -(a0),d1
          swap.w    d7
          swap.w    d3
          swap.w    d2
          swap.w    d1
L0024b:   move.w    -(a0),d7
          move.w    -(a0),d3
          move.w    -(a0),d2
          move.w    -(a0),d1
          swap.w    d7
          swap.w    d3
          swap.w    d2
          swap.w    d1
          ror.l     d4,d7
          ror.l     d4,d3
          ror.l     d4,d2
          ror.l     d4,d1

          move.w    -(a1),d0
          eor.w     d0,d7
          and.w     d6,d7
          eor.w     d0,d7
          move.w    d7,(a1)

          move.w    -(a1),d0
          eor.w     d0,d3
          and.w     d6,d3
          eor.w     d0,d3
          move.w    d3,(a1)

          move.w    -(a1),d0
          eor.w     d0,d2
          and.w     d6,d2
          eor.w     d0,d2
          move.w    d2,(a1)

          move.w    -(a1),d0
          eor.w     d0,d1
          and.w     d6,d1
          eor.w     d0,d1
          move.w    d1,(a1)
          bra       L0037     ;und dort geht's weiter

L0025b:   move.w    -(a0),d7
          move.w    -(a0),d3
          move.w    -(a0),d2
          move.w    -(a0),d1
          swap.w    d7
          swap.w    d3
          swap.w    d2
          swap.w    d1
L0026b:   move.w    -(a0),d7
          move.w    -(a0),d3
          move.w    -(a0),d2
          move.w    -(a0),d1
          rol.l     d4,d7
          rol.l     d4,d3
          rol.l     d4,d2
          rol.l     d4,d1

          move.w    -(a1),d0
          eor.w     d0,d7
          and.w     d6,d7
          eor.w     d0,d7
          move.w    d7,(a1)

          move.w    -(a1),d0
          eor.w     d0,d3
          and.w     d6,d3
          eor.w     d0,d3
          move.w    d3,(a1)

          move.w    -(a1),d0
          eor.w     d0,d2
          and.w     d6,d2
          eor.w     d0,d2
          move.w    d2,(a1)

          move.w    -(a1),d0
          eor.w     d0,d1
          and.w     d6,d1
          eor.w     d0,d1
          move.w    d1,(a1)
          bra       L0037     ;dort geht's weiter

L002B:    move.w    -(a0),d7
          move.w    -(a0),d3
          move.w    -(a0),d2
          move.w    -(a0),d1
          swap.w    d7
          swap.w    d3
          swap.w    d2
          swap.w    d1

          move.l    d7,d0
          ror.l     d4,d0
          move.w    d0,-(a1)

          move.l    d3,d0
          ror.l     d4,d0
          move.w    d0,-(a1)

          move.l    d2,d0
          ror.l     d4,d0
          move.w    d0,-(a1)

          move.l    d1,d0
          ror.l     d4,d0
          move.w    d0,-(a1)

          jmp       (a3)

L002C:    move.w    -(a0),d7
          move.w    -(a0),d3
          move.w    -(a0),d2
          move.w    -(a0),d1

          move.l    d7,d0
          swap.w    d7
          rol.l     d4,d0
          move.w    d0,-(a1)

          move.l    d3,d0
          swap.w    d3
          rol.l     d4,d0
          move.w    d0,-(a1)

          move.l    d2,d0
          swap.w    d2
          rol.l     d4,d0
          move.w    d0,-(a1)

          move.l    d1,d0
          swap.w    d1
          rol.l     d4,d0
          move.w    d0,-(a1)
          jmp       (a3)

L002F:    dbra      d5,L002B

          move.w    -(a0),d7
          move.w    -(a0),d3
          move.w    -(a0),d2
          move.w    -(a0),d1
          ror.l     d4,d7
          ror.l     d4,d3
          ror.l     d4,d2
          ror.l     d4,d1
          bra       L0037

L0030:    dbra      d5,L002C

          move.w    -(a0),d7
          move.w    -(a0),d3
          move.w    -(a0),d2
          move.w    -(a0),d1
          rol.l     d4,d7
          rol.l     d4,d3
          rol.l     d4,d2
          rol.l     d4,d1
          bra       L0038

L0033:    cmpi.w    #1,d5     ;Nur bis 0 runterzählen
          beq.s     spezial   ;bei eins Spezialroutine
          dbra      d5,L002B
          bra       L0037

spezial:  move.w    -(a0),d7
          move.w    -(a0),d3
          move.w    -(a0),d2
          move.w    -(a0),d1
          swap.w    d7
          swap.w    d3
          swap.w    d2
          swap.w    d1
          ror.l     d4,d7
          ror.l     d4,d3
          ror.l     d4,d2
          ror.l     d4,d1
          move.w    d7,-(a1)
          move.w    d3,-(a1)
          move.w    d2,-(a1)
          move.w    d1,-(a1)
          bra.s     L0037

L0034:    cmpi.w    #1,d5
          beq.s     spezial2
          dbra      d5,L002C
          bra.s     L0037

spezial2: move.w    -(a0),d7
          move.w    -(a0),d3
          move.w    -(a0),d2
          move.w    -(a0),d1
          rol.l     d4,d7
          rol.l     d4,d3
          rol.l     d4,d2
          rol.l     d4,d1
          move.w    d7,-(a1)
          move.w    d3,-(a1)
          move.w    d2,-(a1)
          move.w    d1,-(a1)

L0037:    swap.w    d7
          swap.w    d3
          swap.w    d2
          swap.w    d1
L0038:    swap.w    d6
          move.w    -(a1),d0  ;Screen holen
          eor.w     d0,d7
          and.w     d6,d7
          eor.w     d0,d7
          move.w    d7,(a1)   ;und zurück auf Screen

          move.w    -(a1),d0  ;Screen holen
          eor.w     d0,d3
          and.w     d6,d3
          eor.w     d0,d3
          move.w    d3,(a1)   ;und zurück auf Screen

          move.w    -(a1),d0  ;Screen holen
          eor.w     d0,d2
          and.w     d6,d2
          eor.w     d0,d2
          move.w    d2,(a1)   ;und zurück auf Screen

          move.w    -(a1),d0  ;Screen holen
          eor.w     d0,d1
          and.w     d6,d1
          eor.w     d0,d1
          move.w    d1,(a1)   ;und zurück auf Screen

L0039:    swap.w    d6
nxt_scan2:          move.w    -6(a6),d5 ;Breite in BBs holen
nxt_scanline:       swap.w    d5
          subq.w    #1,d5     ;Eine Scanline weniger
          beq.s     nichts_da7
          swap.w    d5        ;Wieder den BB-Zähler
          adda.w    -8(a6),a0 ;zur nächsten Scanline
          adda.w    -10(a6),a1          ;-'-
          addq.l    #8,a0
          addq.l    #8,a1
          jmp       (a4)      ;und von vorne starten
nichts_da7:         movem.l   (sp)+,d0-d7/a0-a5
          unlk      a6
          rts                 ;und schultz

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
; 30(a6): Breite einer Scanline in Pixel (Destination)
;  26(a6): Adresse des Bildschirms (Destination)
;  24(a6): y3 (Destination)
;  22(a6): x3 (Destination)
;  20(a6): Breite einer Scanline in Pixel (Source)
;  16(a6): Adresse des Bildschirms (Source)
;  14(a6): y2 (Source)
;  12(a6): x2 (Source)
;  10(a6): y1 (Source)
;   8(a6): x1 (Source)
mcode50:  link      a6,#-12

          movem.l   d0-d7/a0-a5,-(sp)   ;Register retten

          move.w    8(a6),d0  ;Source X_Min
          move.w    10(a6),d1 ;Source Y_Min
          move.w    12(a6),d2 ;Source X_Max
          move.w    14(a6),d3 ;Source Y_Max
          move.w    22(a6),d4 ;Dest X_Min
          bpl.s     koord_ok11
          sub.w     d4,d0     ;neg->zu Source X_Min addieren
          move.w    d0,8(a6)  ;zurück in Parameter
          moveq.l   #0,d4     ;Clippen
          move.w    d4,22(a6)
koord_ok11:         move.w    24(a6),d5 ;Dest Y_Min
          bpl.s     koord_ok22
          sub.w     d5,d1     ;neg->zu Source Y_Min addieren
          move.w    d1,10(a6) ;zurück in Parameter
          moveq.l   #0,d5
          move.w    d5,24(a6)
koord_ok22:         move.w    d2,d7     ;Source X_Max
          move.w    d4,d6     ;Dest X_Min
          sub.w     d0,d7     ;Source X_Max-Source X_min=Breite-1
          add.w     d7,d6     ;=Dest X_Max
          move.w    30(a6),d7 ;Breite in Pixel (Dest)
          subq.w    #1,d7     ;bis einschliesslich hier
          sub.w     d6,d7     ;von 320 abziehen
          bpl.s     koord_ok33
          add.w     d7,d6     ;von Dest X_Max abziehen
          add.w     d7,d2     ;und von Source X_Max
          move.w    d2,12(a6) ;zurück in Parameter
koord_ok33:         sub.w     d1,d3     ;Höhe-1
          add.w     d3,d5     ;=Dest Y_Max
          move.w    #199,d3   ;Unterer Rand
          sub.w     d5,d3
          bpl.s     koord_ok44
          add.w     d3,d5     ;von Dest Y_Max abziehen
          add.w     d3,14(a6) ;von Source Y_Max abziehen
koord_ok44:         move.w    d6,-2(a6) ;Dest X_Max merken
          move.w    d5,-4(a6) ;Dest Y_Max merken

          move.w    12(a6),d5 ;Source X_Max
          sub.w     8(a6),d5  ;-Source X_Min
          bmi       nichts_da7
          move.w    14(a6),d5 ;Source Y_Max
          sub.w     10(a6),d5 ;-Source Y_Min
          bmi       nichts_da7

          moveq.l   #$0000000f,d5
          move.w    d0,d1     ;Source X_Min
          and.w     d5,d1
          move.w    d4,d3     ;Dest X_Min
          and.w     d5,d3
          lsr.w     #4,d0     ;# des BBs (Source X_Min)
          lsr.w     #4,d4     ;# des BBs (Dest X_Min)
          lsr.w     #4,d2     ;# des BBs (Source X_Max)
          lsr.w     #4,d6     ;# des BBs (Dest X_Max)
          sub.w     d0,d2     ;X_Max-X_Min=Breite in BBs-1 (Source)
          sub.w     d4,d6     ;-'- (Dest)
          move.w    d2,d5     ;Breite des Sourceblocks
          sub.w     d6,d5     ;-Breite des Zielblocks
          andi.w    #1,d5     ;NFSR?
          lsl.w     #2,d5     ;an richtige Position schieben
          sub.w     d3,d1     ;Skew
          move.w    d6,d0     ;Breite des Zielblocks in BBs
          subq.w    #1,d0
          move.w    d0,-6(a6) ;und merken
          move.w    d2,d7     ;Breite des Sourceblocks in BBs-1
          move.w    d1,d4     ;Skew
          move.w    d1,d2     ;Skew
          tst.w     d4        ;Skew rechts oder links?
          bgt.s     links_rum2          ;ja
          beq       weiter2
          neg.w     d4        ;Skew positiv machen
          addq.w    #1,d5     ;und merken
links_rum2:         cmpi.w    #8,d4     ;Skew weniger als 8?
          blt.s     weiter2
          addq.w    #2,d5     ;und merken, besser andersrum shiften(!)
          neg.w     d4        ;Skew umdrehen, also
          addi.w    #16,d4    ;von 16 abziehen
weiter2:  move.w    12(a6),d0 ;Source X_Max
          move.w    14(a6),d1 ;Source Y_Max
          bsr       calc_quelle         ;von hinten reinkommen
          lsl.w     #3,d7
          sub.w     d3,d7     ;Breite einer Zeile in Bytes abziehen(?)
          move.w    d7,-8(a6) ;Offsets am Zeilenende merken
          move.w    -2(a6),d0 ;Dest X_Max
          move.w    -4(a6),d1 ;Dest Y_Max
          bsr       calc_ziel ;auch von hinten rein
          lsl.w     #3,d6     ;Breite in BBs->Breite in Bytes
          sub.w     d3,d6     ;Breite einer Zeile in Bytes abziehen(?)
          move.w    d6,-10(a6)          ;Offset am Zeilenende merken
          bsr       get_masken          ;Masken->D6
          swap.w    d6        ;Masken vertauschen
          asl.w     #3,d5     ;je nach Konstellation richtige Routine
          movea.l   L0009t(pc,d5.w),a3
          movea.l   L0008t(pc,d5.w),a4
          tst.w     -6(a6)    ;Breite des Zielblocks in BBs>=2?
          bgt       start_trans         ;ja
          blt.s     klein2
          bclr      #3,d5     ;Skew nach rechts?
          beq       start_trans         ;Nein, dann okay
          lsr.w     #2,d5     ;nur noch mal 4
          movea.l   rechts_kleint(pc,d5.w),a4     ;Spezialroutine
          bra       start_trans

;Tabelle für Objekte, die genau 2 BBs breit sind, und nach rechts geschoben
;werden, da es für die nachfolgenden Routine (L0037) nicht möglich war an
;bestimmte Informationen heranzukommen.
rechts_kleint:      .DC.l L0023bt
          .DC.l L0025bt
          .DC.l L0024bt
          .DC.l L0026bt

;Routinen für nur einen BB breite Objekte:
L0007t:   .DC.l L0021t
          .DC.l L0022t
          .DC.l L0022t
          .DC.l L0021t

;Routinen für mindestens 2 BB breite Objekte
L0008t:   .DC.l L0026t        ;4  = 0
L0009t:   .DC.l L0030t
          .DC.l L0023t        ;5  = 1 -
          .DC.l L0033t
          .DC.l L0024t        ;6  = 2
          .DC.l L002Ft
          .DC.l L0025t        ;7  = 3 -
          .DC.l L0034t
          .DC.l L0025t        ;12  = 4
          .DC.l L0030t
          .DC.l L0024t        ;13  = 5 -
          .DC.l L0033t
          .DC.l L0023t        ;14  = 6
          .DC.l L002Ft
          .DC.l L0026t        ;15  = 7 -
          .DC.l L0034t

klein2:   move.l    d6,d0     ;Maske
          swap.w    d0
          and.w     d0,d6     ;linker und rechter Rand verknüpfen
          lea.l     nxt_scanline,a3
          btst      #5,d5     ;NFSR?
          bne       start_trans         ;Ja->Normal arbeiten
          lsr.w     #1,d5     ;nur noch mal 4
          andi.w    #%0000000000001100,d5
          movea.l   L0007t(pc,d5.w),a4  ;Sonst andere Routine nehmen
start_trans:        move.w    14(a6),d5 ;y2
          sub.w     10(a6),d5 ;-y1=Höhe in Pixeln-1
          addq.w    #1,d5     ;=Höhe in Pixeln
          swap.w    d5
          move.w    -6(a6),d5 ;Breite des Zielblocks in BBs-2
          addq.l    #8,a0     ;Ans Ende des BB gehen
          addq.l    #8,a1     ;-'-
          tst.w     d4        ;Skew=0?
          beq       no_shift2 ;ja, dann Spezial-Routine nehmen
          jmp       (a4)      ;und Plane bearbeiten

;Spezialroutine falls Skew=0:
no_shift2:          lea.l     no_shift3(pc),a4    ;Bei nächster Scanline diese Routine
          move.l    d6,d0     ;Maske retten
          swap.w    d6
          move.w    d6,d7     ;obere Maske->D7
          swap.w    d7
          move.w    d6,d7     ;->D7
          move.w    d0,d6     ;untere Maske->D6

no_shift3:          move.l    -(a0),d0  ;Plane 2 und 3 holen (12)
          move.l    -(a0),d1  ;Plane 0 und 1 holen (12)
          move.l    d0,d4     ;für Maske      ( 4)
          or.l      d1,d4     ;               ( 8)
          swap.w    d0        ;               ( 4)
          swap.w    d1
          or.l      d0,d4     ;Maske in d4    ( 8)
          or.l      d1,d4
          swap.w    d0
          swap.w    d1
          and.l     d6,d4     ;maske_links
          move.l    -(a1),d2  ;Screen holen   (12)
          move.l    -(a1),d3
          eor.l     d2,d0
          and.l     d4,d0
          eor.l     d2,d0
          eor.l     d3,d1
          and.l     d4,d1
          eor.l     d3,d1
          move.l    d0,4(a1)
          move.l    d1,(a1)
          tst.w     d5        ;nur ein BB?
          bmi       nxt_scan2 ;ja, -> Weiter
          bra       bb_dbra

bb_mitte: move.l    -(a0),d0  ;Plane 2 und 3 holen (12)
          move.l    -(a0),d1  ;Plane 0 und 1 holen (12)
          move.l    d0,d4     ;für Maske      ( 4)
          or.l      d1,d4     ;               ( 8)
          swap.w    d0        ;               ( 4)
          swap.w    d1
          or.l      d0,d4     ;Maske in d4    ( 8)
          or.l      d1,d4
          swap.w    d0
          swap.w    d1
          not.l     d4        ;Maske invertieren
          beq.s     klopfen2
          move.l    -4(a1),d2 ;Screen holen   (12)
          move.l    -8(a1),d3
          and.l     d4,d2     ;               ( 8)
          or.l      d2,d0     ;               ( 8)
          and.l     d4,d3     ;               ( 8)
          or.l      d3,d1     ;               ( 8)
klopfen2: move.l    d0,-(a1)
          move.l    d1,-(a1)
bb_dbra:  dbra      d5,bb_mitte         ;zählt bis auf 0 runter

bb_rechts:          move.l    -(a0),d0  ;Plane 2 und 3 holen (12)
          move.l    -(a0),d1  ;Plane 0 und 1 holen (12)
          move.l    d0,d4     ;für Maske      ( 4)
          or.l      d1,d4     ;               ( 8)
          swap.w    d0        ;               ( 4)
          swap.w    d1
          or.l      d0,d4     ;Maske in d4    ( 8)
          or.l      d1,d4
          swap.w    d0
          swap.w    d1
          and.l     d7,d4     ;maske_rechts
          move.l    -(a1),d2  ;Screen holen   (12)
          move.l    -(a1),d3
          eor.l     d2,d0
          and.l     d4,d0
          eor.l     d2,d0
          eor.l     d3,d1
          and.l     d4,d1
          eor.l     d3,d1
          move.l    d0,4(a1)
          move.l    d1,(a1)
          bra       nxt_scan2

L0021t:   move.w    -(a0),d7
          move.w    -(a0),d3
          move.w    -(a0),d2
          move.w    -(a0),d1
          rol.w     d4,d7
          rol.w     d4,d3
          rol.w     d4,d2
          rol.w     d4,d1

          movea.l   d1,a5     ;d1 retten
          or.w      d2,d1
          or.w      d3,d1     ;Maske berechnen
          or.w      d7,d1

          move.w    -(a1),d0
          eor.w     d0,d7
          and.w     d6,d7
          and.w     d1,d7
          eor.w     d0,d7
          move.w    d7,(a1)

          move.w    -(a1),d0
          eor.w     d0,d3
          and.w     d6,d3
          and.w     d1,d3
          eor.w     d0,d3
          move.w    d3,(a1)

          move.w    -(a1),d0
          eor.w     d0,d2
          and.w     d6,d2
          and.w     d1,d2
          eor.w     d0,d2
          move.w    d2,(a1)

          move.w    a5,d7     ;d1 holen

          move.w    -(a1),d0
          eor.w     d0,d7
          and.w     d6,d7
          and.w     d1,d7
          eor.w     d0,d7
          move.w    d7,(a1)
          jmp       (a3)

L0022t:   move.w    -(a0),d7
          move.w    -(a0),d3
          move.w    -(a0),d2
          move.w    -(a0),d1
          ror.w     d4,d7
          ror.w     d4,d3
          ror.w     d4,d2
          ror.w     d4,d1

          movea.l   d1,a5     ;d1 retten
          or.w      d2,d1
          or.w      d3,d1
          or.w      d7,d1

          move.w    -(a1),d0
          eor.w     d0,d7
          and.w     d6,d7
          and.w     d1,d7
          eor.w     d0,d7
          move.w    d7,(a1)

          move.w    -(a1),d0
          eor.w     d0,d3
          and.w     d6,d3
          and.w     d1,d3
          eor.w     d0,d3
          move.w    d3,(a1)

          move.w    -(a1),d0
          eor.w     d0,d2
          and.w     d6,d2
          and.w     d1,d2
          eor.w     d0,d2
          move.w    d2,(a1)

          move.w    a5,d7     ;d1 holen
          move.w    -(a1),d0
          eor.w     d0,d7
          and.w     d6,d7
          and.w     d1,d7
          eor.w     d0,d7
          move.w    d7,(a1)
          jmp       (a3)

L0023t:   move.w    -(a0),d7  ;Plane aus Objekt holen
          move.w    -(a0),d3
          move.w    -(a0),d2
          move.w    -(a0),d1
          swap.w    d7
          swap.w    d3
          swap.w    d2
          swap.w    d1
L0024t:   move.w    -(a0),d7  ;nächste Plane aus Objekt
          move.w    -(a0),d3
          move.w    -(a0),d2
          move.w    -(a0),d1
          swap.w    d7
          swap.w    d3
          swap.w    d2
          swap.w    d1

          movea.l   d1,a5     ;d1 retten
          or.l      d2,d1
          or.l      d3,d1     ;Maske berechnen
          or.l      d7,d1

          move.l    d7,d0     ;ins Schieberegister
          ror.l     d4,d0     ;runterschieben
          ror.l     d4,d1     ;Maske schieben
;                not.w   D1              ;Maske invertieren
          move.w    -(a1),d7  ;Screen holen
          eor.w     d7,d0
          and.w     d6,d0     ;ausblenden
          and.w     d1,d0     ;Maskieren
          eor.w     d7,d0
          move.w    d0,(a1)   ;und zurück auf Screen

          move.l    d3,d0     ;ins Schieberegister
          ror.l     d4,d0     ;runterschieben
          move.w    -(a1),d3  ;Screen holen
          eor.w     d3,d0
          and.w     d6,d0
          and.w     d1,d0     ;Maskieren
          eor.w     d3,d0
          move.w    d0,(a1)   ;und zurück auf Screen

          move.l    d2,d0     ;ins Schieberegister
          ror.l     d4,d0     ;runterschieben
          move.w    -(a1),d2  ;Screen holen
          eor.w     d2,d0
          and.w     d6,d0
          and.w     d1,d0
          eor.w     d2,d0
          move.w    d0,(a1)   ;und zurück auf Screen

          move.l    a5,d0     ;ins Schieberegister
          ror.l     d4,d0     ;runterschieben
          move.w    -(a1),d2  ;Screen holen
          eor.w     d2,d0
          and.w     d6,d0
          and.w     d1,d0
          eor.w     d2,d0
          move.w    d0,(a1)   ;und zurück auf Screen

          move.l    a5,d1     ;und richtiges d1 rausholen

          jmp       (a3)

L0025t:   move.w    -(a0),d7  ;Plane aus Objekt holen
          move.w    -(a0),d3
          move.w    -(a0),d2
          move.w    -(a0),d1
          swap.w    d7
          swap.w    d3
          swap.w    d2
          swap.w    d1
L0026t:   move.w    -(a0),d7
          move.w    -(a0),d3
          move.w    -(a0),d2
          move.w    -(a0),d1

          movea.l   d1,a5     ;d1 retten
          or.l      d2,d1
          or.l      d3,d1     ;Maske berechnen
          or.l      d7,d1

          move.l    d7,d0     ;ins Schieberegister
          swap.w    d7
          rol.l     d4,d0     ;schieben
          rol.l     d4,d1     ;Maske hinschieben
;                not.w   D1              ;Maske invertieren
          move.w    -(a1),d7  ;Screen holen
          eor.w     d7,d0
          and.w     d6,d0
          and.w     d1,d0
          eor.w     d7,d0
          move.w    d0,(a1)

          move.l    d3,d0     ;ins Schieberegister
          swap.w    d3
          rol.l     d4,d0     ;schieben
          move.w    -(a1),d3  ;Screen holen
          eor.w     d3,d0
          and.w     d6,d0
          and.w     d1,d0
          eor.w     d3,d0
          move.w    d0,(a1)

          move.l    d2,d0     ;ins Schieberegister
          swap.w    d2
          rol.l     d4,d0     ;schieben
          move.w    -(a1),d2  ;Screen holen
          eor.w     d2,d0
          and.w     d6,d0
          and.w     d1,d0
          eor.w     d2,d0
          move.w    d0,(a1)

          move.l    a5,d0     ;ins Schieberegister
          rol.l     d4,d0     ;schieben
          move.w    -(a1),d2  ;Screen holen
          eor.w     d2,d0
          and.w     d6,d0
          and.w     d1,d0
          eor.w     d2,d0
          move.w    d0,(a1)

          move.l    a5,d1     ;d1 rausholen
          swap.w    d1        ;und auch noch umdrehen
          jmp       (a3)

L0023bt:  move.w    -(a0),d7
          move.w    -(a0),d3
          move.w    -(a0),d2
          move.w    -(a0),d1
          swap.w    d7
          swap.w    d3
          swap.w    d2
          swap.w    d1
L0024bt:  move.w    -(a0),d7
          move.w    -(a0),d3
          move.w    -(a0),d2
          move.w    -(a0),d1
          swap.w    d7
          swap.w    d3
          swap.w    d2
          swap.w    d1
          ror.l     d4,d7
          ror.l     d4,d3
          ror.l     d4,d2
          ror.l     d4,d1

          movea.l   d1,a5     ;d1 retten
          or.w      d2,d1
          or.w      d3,d1     ;Maske berechnen
          or.w      d7,d1
;               not.w   D1              ;und invertieren

          move.w    -(a1),d0
          eor.w     d0,d7
          and.w     d6,d7
          and.w     d1,d7
          eor.w     d0,d7
          move.w    d7,(a1)

          move.w    -(a1),d0
          eor.w     d0,d3
          and.w     d6,d3
          and.w     d1,d3
          eor.w     d0,d3
          move.w    d3,(a1)

          move.w    -(a1),d0
          eor.w     d0,d2
          and.w     d6,d2
          and.w     d1,d2
          eor.w     d0,d2
          move.w    d2,(a1)

          move.w    a5,d2     ;altes d1 holen

          move.w    -(a1),d0
          eor.w     d0,d2
          and.w     d6,d2
          and.w     d1,d2
          eor.w     d0,d2
          move.w    d2,(a1)

          move.l    a5,d1     ;und nochmal her damit

          bra       L0037t    ;und dort geht's weiter

L0025bt:  move.w    -(a0),d7
          move.w    -(a0),d3
          move.w    -(a0),d2
          move.w    -(a0),d1
          swap.w    d7
          swap.w    d3
          swap.w    d2
          swap.w    d1
L0026bt:  move.w    -(a0),d7
          move.w    -(a0),d3
          move.w    -(a0),d2
          move.w    -(a0),d1
          rol.l     d4,d7
          rol.l     d4,d3
          rol.l     d4,d2
          rol.l     d4,d1

          movea.l   d1,a5     ;d1 retten
          or.w      d2,d1
          or.w      d3,d1     ;Maske berechnen
          or.w      d7,d1
;              not.w   D1              ;und invertieren

          move.w    -(a1),d0
          eor.w     d0,d7
          and.w     d6,d7
          and.w     d1,d7
          eor.w     d0,d7
          move.w    d7,(a1)

          move.w    -(a1),d0
          eor.w     d0,d3
          and.w     d6,d3
          and.w     d1,d3
          eor.w     d0,d3
          move.w    d3,(a1)

          move.w    -(a1),d0
          eor.w     d0,d2
          and.w     d6,d2
          and.w     d1,d2
          eor.w     d0,d2
          move.w    d2,(a1)

          move.w    a5,d2
          move.w    -(a1),d0
          eor.w     d0,d2
          and.w     d6,d2
          and.w     d1,d2
          eor.w     d0,d2
          move.w    d2,(a1)

          bra       L0037t    ;dort geht's weiter

L002Bt:   move.w    -(a0),d7
          move.w    -(a0),d3
          move.w    -(a0),d2
          move.w    -(a0),d1
          swap.w    d7
          swap.w    d3
          swap.w    d2
          swap.w    d1

          movea.l   d1,a5     ;d1 retten
          or.l      d2,d1
          or.l      d3,d1
          or.l      d7,d1

          move.l    d7,d0
          ror.l     d4,d0
          ror.l     d4,d1     ;Maske auch schieben
          not.w     d1        ;Maske invertieren
          and.w     d1,-(a1)
          or.w      d0,(a1)

          move.l    d3,d0
          ror.l     d4,d0
          and.w     d1,-(a1)
          or.w      d0,(a1)

          move.l    d2,d0
          ror.l     d4,d0
          and.w     d1,-(a1)
          or.w      d0,(a1)

          move.l    a5,d0
          ror.l     d4,d0
          and.w     d1,-(a1)
          or.w      d0,(a1)

          move.l    a5,d1     ;und altes d1 rausholen
          jmp       (a3)

L002Ct:   move.w    -(a0),d7
          move.w    -(a0),d3
          move.w    -(a0),d2
          move.w    -(a0),d1

          movea.l   d1,a5     ;d1 retten
          or.l      d2,d1
          or.l      d3,d1     ;Maske berechnen
          or.l      d7,d1

          move.l    d7,d0     ;ins Schieberegister
          swap.w    d7
          rol.l     d4,d0
          rol.l     d4,d1     ;Maske auch schieben
          not.w     d1
          and.w     d1,-(a1)  ;Maskieren
          or.w      d0,(a1)   ;und einknüpfen

          move.l    d3,d0     ;ins Schieberegister
          swap.w    d3
          rol.l     d4,d0
          and.w     d1,-(a1)
          or.w      d0,(a1)

          move.l    d2,d0     ;ins Schieberegister
          swap.w    d2
          rol.l     d4,d0
          and.w     d1,-(a1)
          or.w      d0,(a1)

          move.l    a5,d0     ;ins Schieberegister
          rol.l     d4,d0
          and.w     d1,-(a1)
          or.w      d0,(a1)

          move.l    a5,d1     ;altes d1 holen
          swap.w    d1        ;und auch umdrehen
          jmp       (a3)

L002Ft:   dbra      d5,L002Bt

          move.w    -(a0),d7
          move.w    -(a0),d3
          move.w    -(a0),d2
          move.w    -(a0),d1
          ror.l     d4,d7
          ror.l     d4,d3
          ror.l     d4,d2
          ror.l     d4,d1
          bra       L0037t

L0030t:   dbra      d5,L002Ct

          move.w    -(a0),d7
          move.w    -(a0),d3
          move.w    -(a0),d2
          move.w    -(a0),d1
          rol.l     d4,d7
          rol.l     d4,d3
          rol.l     d4,d2
          rol.l     d4,d1
          bra       L0038t

L0033t:   cmpi.w    #1,d5     ;Nur bis 0 runterzählen
          beq.s     spezialt  ;bei eins Spezialroutine
          dbra      d5,L002Bt
          bra       L0037t

spezialt: move.w    -(a0),d7
          move.w    -(a0),d3
          move.w    -(a0),d2
          move.w    -(a0),d1
          swap.w    d7
          swap.w    d3
          swap.w    d2
          swap.w    d1
          ror.l     d4,d7
          ror.l     d4,d3
          ror.l     d4,d2
          ror.l     d4,d1

          movea.l   d1,a5     ;d1 merken
          or.w      d2,d1
          or.w      d3,d1     ;Maske bilden
          or.w      d7,d1
          not.w     d1

          and.w     d1,-(a1)
          or.w      d7,(a1)
          and.w     d1,-(a1)
          or.w      d3,(a1)
          and.w     d1,-(a1)
          or.w      d2,(a1)
          and.w     d1,-(a1)
          move.l    a5,d1     ;Altes D1 holen
          or.w      d1,(a1)

          bra.s     L0037t

L0034t:   cmpi.w    #1,d5
          beq.s     spezial2t
          dbra      d5,L002Ct
          bra.s     L0037t

spezial2t:          
          move.w    -(a0),d7
          move.w    -(a0),d3
          move.w    -(a0),d2
          move.w    -(a0),d1
          rol.l     d4,d7
          rol.l     d4,d3
          rol.l     d4,d2
          rol.l     d4,d1

          movea.l   d1,a5     ;d1 retten
          or.w      d2,d1
          or.w      d3,d1     ;Maske bilden
          or.w      d7,d1
          not.w     d1

          and.w     d1,-(a1)
          or.w      d7,(a1)
          and.w     d1,-(a1)
          or.w      d3,(a1)
          and.w     d1,-(a1)
          or.w      d2,(a1)
          and.w     d1,-(a1)
          move.l    a5,d1
          or.w      d1,(a1)

L0037t:   swap.w    d7
          swap.w    d3
          swap.w    d2
          swap.w    d1
L0038t:   swap.w    d6

          movea.l   d1,a5     ;d1 retten
          or.w      d2,d1
          or.w      d3,d1
          or.w      d7,d1     ;Maske berechnen
;                not.w   D1

          move.w    -(a1),d0  ;Screen holen
          eor.w     d0,d7
          and.w     d6,d7
          and.w     d1,d7
          eor.w     d0,d7
          move.w    d7,(a1)   ;und zurück auf Screen

          move.w    -(a1),d0  ;Screen holen
          eor.w     d0,d3
          and.w     d6,d3
          and.w     d1,d3
          eor.w     d0,d3
          move.w    d3,(a1)   ;und zurück auf Screen

          move.w    -(a1),d0  ;Screen holen
          eor.w     d0,d2
          and.w     d6,d2
          and.w     d1,d2
          eor.w     d0,d2
          move.w    d2,(a1)   ;und zurück auf Screen

          move.w    a5,d2     ;und altes d1 holen

          move.w    -(a1),d0  ;Screen holen
          eor.w     d0,d2
          and.w     d6,d2
          and.w     d1,d2     ;Maske
          eor.w     d0,d2
          move.w    d2,(a1)   ;und zurück auf Screen
          bra       L0039     ;und nächste Scanline



;Diese Routine kann ganze Bildschirmteile spiegeln. Diese Bildschirmbereiche
;müssen aber auf BBs liegen, d.h. sie clippt die Ränder nicht!
; 18(a6): y2
;  16(a6): x2
;  14(a6): y1
;  12(a6): x1
;   8(a6): Screenadresse
mcode51:  link      a6,#0

          movem.l   d0-d7/a0-a5,-(sp)
          movea.l   8(a6),a0  ;Screenadresse
          move.w    14(a6),d0 ;y1
          mulu.w    #160,d0
          adda.l    d0,a0
          movea.l   a0,a1     ;rechter Rand
          move.w    12(a6),d0 ;x1
          lsr.w     #4,d0
          lsl.w     #3,d0
          adda.w    d0,a0
          move.w    16(a6),d0 ;x2
          lsr.w     #4,d0
          lsl.w     #3,d0
          adda.w    d0,a1     ;x1=rechter Rand

          st        ungerade2
          move.w    16(a6),d7 ;x2
          sub.w     12(a6),d7 ;-x1=Breite in Pixeln
          lsr.w     #5,d7     ;=Breite in BBs-1/2
          bcs.s     gerade88
          sf        ungerade2
          subq.w    #1,d7     ;ein BB weniger
gerade88: movea.w   d7,a5     ;Breite in BBs-1 merken
          move.w    d7,d0
          addq.w    #1,d0
          lsl.w     #3,d0     ;Breite in Bytes
          movea.w   #160,a3
          suba.w    d0,a3     ;Offset für linken Rand
          movea.w   #160,a4   ;Offset für rechten Rand
          adda.w    d0,a4
          tst.b     ungerade2
          bne.s     gerade22
          subq.l    #8,a3
          subq.l    #8,a4
gerade22: move.w    18(a6),d6
          sub.w     14(a6),d6 ;Höhe in Scanlines
          lea.l     mirror_tab,a2

          moveq.l   #0,d0
          moveq.l   #0,d1

mirror_loop2:       move.w    a5,d7     ;Breite in BBs-1

mirror_loop1:       movem.w   (a1),d2-d5          ;rechten Rand merken

          move.b    (a0)+,d0
          move.b    (a0)+,d1
          move.b    0(a2,d0.w),d0       ;Byte spiegeln
          move.b    0(a2,d1.w),d1       ;-'-
          move.b    d1,(a1)+
          move.b    d0,(a1)+

          move.b    (a0)+,d0
          move.b    (a0)+,d1
          move.b    0(a2,d0.w),d0       ;Byte spiegeln
          move.b    0(a2,d1.w),d1       ;-'-
          move.b    d1,(a1)+
          move.b    d0,(a1)+

          move.b    (a0)+,d0
          move.b    (a0)+,d1
          move.b    0(a2,d0.w),d0       ;Byte spiegeln
          move.b    0(a2,d1.w),d1       ;-'-
          move.b    d1,(a1)+
          move.b    d0,(a1)+

          move.b    (a0)+,d0
          move.b    (a0)+,d1
          move.b    0(a2,d0.w),d0       ;Byte spiegeln
          move.b    0(a2,d1.w),d1       ;-'-
          move.b    d1,(a1)+
          move.b    d0,(a1)+

          subq.l    #8,a0     ;zurück an Anfang des linken BBs
          lea.l     -16(a1),a1          ;An Anfang des nächsten BBs rechts

          move.b    d2,d0
          move.b    0(a2,d0.w),d0
          move.b    d0,(a0)+
          lsr.w     #8,d2     ;2. Byte holen
          move.b    0(a2,d2.w),d2
          move.b    d2,(a0)+

          move.b    d3,d0
          move.b    0(a2,d0.w),d0
          move.b    d0,(a0)+
          lsr.w     #8,d3     ;2. Byte holen
          move.b    0(a2,d3.w),d3
          move.b    d3,(a0)+

          move.b    d4,d0
          move.b    0(a2,d0.w),d0
          move.b    d0,(a0)+
          lsr.w     #8,d4     ;2. Byte holen
          move.b    0(a2,d4.w),d4
          move.b    d4,(a0)+

          move.b    d5,d0
          move.b    0(a2,d0.w),d0
          move.b    d0,(a0)+
          lsr.w     #8,d5     ;2. Byte holen
          move.b    0(a2,d5.w),d5
          move.b    d5,(a0)+
          dbra      d7,mirror_loop1

          tst.b     ungerade2
          bne.s     wt62

          move.b    (a0)+,d0
          move.b    (a0)+,d1
          move.b    0(a2,d0.w),d0       ;Byte spiegeln
          move.b    0(a2,d1.w),d1       ;-'-
          move.b    d1,(a1)+
          move.b    d0,(a1)+

          move.b    (a0)+,d0
          move.b    (a0)+,d1
          move.b    0(a2,d0.w),d0       ;Byte spiegeln
          move.b    0(a2,d1.w),d1       ;-'-
          move.b    d1,(a1)+
          move.b    d0,(a1)+

          move.b    (a0)+,d0
          move.b    (a0)+,d1
          move.b    0(a2,d0.w),d0       ;Byte spiegeln
          move.b    0(a2,d1.w),d1       ;-'-
          move.b    d1,(a1)+
          move.b    d0,(a1)+

          move.b    (a0)+,d0
          move.b    (a0)+,d1
          move.b    0(a2,d0.w),d0       ;Byte spiegeln
          move.b    0(a2,d1.w),d1       ;-'-
          move.b    d1,(a1)+
          move.b    d0,(a1)+

wt62:     adda.l    a3,a0
          adda.l    a4,a1
          dbra      d6,mirror_loop2

          movem.l   (sp)+,d0-d7/a0-a5
          unlk      a6
          rts       

mirror_tab:         .DC.b $00,$80,$40,$c0,$20,$a0,$60,$e0
          .DC.b $10,$90,$50,$d0,$30,$b0,$70,$f0
          .DC.b $08,$88,$48,$c8,$28,$a8,$68,$e8
          .DC.b $18,$98,$58,$d8,$38,$b8,$78,$f8
          .DC.b $04,$84,$44,$c4,$24,$a4,$64,$e4
          .DC.b $14,$94,$54,$d4,$34,$b4,$74,$f4
          .DC.b $0c,$8c,$4c,$cc,$2c,$ac,$6c,$ec
          .DC.b $1c,$9c,$5c,$dc,$3c,$bc,$7c,$fc
          .DC.b $02,$82,$42,$c2,$22,$a2,$62,$e2
          .DC.b $12,$92,$52,$d2,$32,$b2,$72,$f2
          .DC.b $0a,$8a,$4a,$ca,$2a,$aa,$6a,$ea
          .DC.b $1a,$9a,$5a,$da,$3a,$ba,$7a,$fa
          .DC.b $06,$86,$46,$c6,$26,$a6,$66,$e6
          .DC.b $16,$96,$56,$d6,$36,$b6,$76,$f6
          .DC.b $0e,$8e,$4e,$ce,$2e,$ae,$6e,$ee
          .DC.b $1e,$9e,$5e,$de,$3e,$be,$7e,$fe
          .DC.b $01,$81,$41,$c1,$21,$a1,$61,$e1
          .DC.b $11,$91,$51,$d1,$31,$b1,$71,$f1
          .DC.b $09,$89,$49,$c9,$29,$a9,$69,$e9
          .DC.b $19,$99,$59,$d9,$39,$b9,$79,$f9
          .DC.b $05,$85,$45,$c5,$25,$a5,$65,$e5
          .DC.b $15,$95,$55,$d5,$35,$b5,$75,$f5
          .DC.b $0d,$8d,$4d,$cd,$2d,$ad,$6d,$ed
          .DC.b $1d,$9d,$5d,$dd,$3d,$bd,$7d,$fd
          .DC.b $03,$83,$43,$c3,$23,$a3,$63,$e3
          .DC.b $13,$93,$53,$d3,$33,$b3,$73,$f3
          .DC.b $0b,$8b,$4b,$cb,$2b,$ab,$6b,$eb
          .DC.b $1b,$9b,$5b,$db,$3b,$bb,$7b,$fb
          .DC.b $07,$87,$47,$c7,$27,$a7,$67,$e7
          .DC.b $17,$97,$57,$d7,$37,$b7,$77,$f7
          .DC.b $0f,$8f,$4f,$cf,$2f,$af,$6f,$ef
          .DC.b $1f,$9f,$5f,$df,$3f,$bf,$7f,$ff

ungerade2:          .DC.b 0
          .EVEN 

;***************************************************************************
; Unpacking source for Pack-Ice Version 2.31
; 12(A6).L: Zeiger auf den Zieldatenbereich
;  8(A6).L: Zeiger auf die gepackten Daten
mcode52:  movem.l   a0/a1,-(sp)

          movea.l   12(sp),a0 ;Adresse der gepackten Daten
          movea.l   16(sp),a1 ;Zieladresse
          cmpa.l    a0,a1
          beq       ice_decrunch_2      ;Ja->Andere Routine nehmen

          bsr       decrunch
          bra.s     raus_hier
ice_decrunch_2:     bsr       decrunch2
raus_hier:          movem.l   (sp)+,a0/a1
          rts       

decrunch: movem.l   d0-d7/a0-a6,-(sp)
          bsr.s     ice_unpa4
          cmpi.l    #$49434521,d0
          bne.s     ice_unpa3
          bsr.s     ice_unpa4
          lea       -8(a0,d0.l),a5
          bsr.s     ice_unpa4
          move.l    d0,(sp)
          movea.l   a1,a4
          movea.l   a1,a6
          adda.l    d0,a6
          movea.l   a6,a3
          move.b    -(a5),d7
          bsr.s     ice_unpa6
          bsr.s     ice_unpa10
          bcc.s     ice_unpa3
          move.w    #$0f9f,d7
ice_unpa: moveq     #3,d6
ice_unpa1:          move.w    -(a3),d4
          moveq     #3,d5
ice_unpa2:          add.w     d4,d4
          addx.w    d0,d0
          add.w     d4,d4
          addx.w    d1,d1
          add.w     d4,d4
          addx.w    d2,d2
          add.w     d4,d4
          addx.w    d3,d3
          dbra      d5,ice_unpa2
          dbra      d6,ice_unpa1
          movem.w   d0-d3,(a3)
          dbra      d7,ice_unpa
ice_unpa3:          movem.l   (sp)+,d0-d7/a0-a6
          rts       
ice_unpa4:          moveq     #3,d1
ice_unpa5:          lsl.l     #8,d0
          move.b    (a0)+,d0
          dbra      d1,ice_unpa5
          rts       
ice_unpa6:          bsr.s     ice_unpa10
          bcc.s     ice_unpa9
          moveq     #0,d1
          bsr.s     ice_unpa10
          bcc.s     ice_unpa8
          lea       ice_unpa23(pc),a1
          moveq     #4,d3
ice_unpa7:          move.l    -(a1),d0
          bsr.s     ice_unpa12
          swap.w    d0
          cmp.w     d0,d1
          dbne      d3,ice_unpa7
          add.l     20(a1),d1
ice_unpa8:          move.b    -(a5),-(a6)
          dbra      d1,ice_unpa8
ice_unpa9:          cmpa.l    a4,a6
          bgt.s     ice_unpa15
          rts       
ice_unpa10:         add.b     d7,d7
          bne.s     ice_unpa11
          move.b    -(a5),d7
          addx.b    d7,d7
ice_unpa11:         rts       
ice_unpa12:         moveq     #0,d1
ice_unpa13:         add.b     d7,d7
          bne.s     ice_unpa14
          move.b    -(a5),d7
          addx.b    d7,d7
ice_unpa14:         addx.w    d1,d1
          dbra      d0,ice_unpa13
          rts       
ice_unpa15:         lea       ice_unpa24(pc),a1
          moveq     #3,d2
ice_unpa16:         bsr.s     ice_unpa10
          dbcc      d2,ice_unpa16
          moveq     #0,d4
          moveq     #0,d1
          move.b    1(a1,d2.w),d0
          ext.w     d0
          bmi.s     ice_unpa17
          bsr.s     ice_unpa12
ice_unpa17:         move.b    6(a1,d2.w),d4
          add.w     d1,d4
          beq.s     ice_unpa19
          lea       ice_unpa25(pc),a1
          moveq     #1,d2
ice_unpa18:         bsr.s     ice_unpa10
          dbcc      d2,ice_unpa18
          moveq     #0,d1
          move.b    1(a1,d2.w),d0
          ext.w     d0
          bsr.s     ice_unpa12
          add.w     d2,d2
          add.w     6(a1,d2.w),d1
          bpl.s     ice_unpa21
          sub.w     d4,d1
          bra.s     ice_unpa21
ice_unpa19:         moveq     #0,d1
          moveq     #5,d0
          moveq     #-1,d2
          bsr.s     ice_unpa10
          bcc.s     ice_unpa20
          moveq     #8,d0
          moveq     #$3f,d2
ice_unpa20:         bsr.s     ice_unpa12
          add.w     d2,d1
ice_unpa21:         lea       2(a6,d4.w),a1
          adda.w    d1,a1
          move.b    -(a1),-(a6)
ice_unpa22:         move.b    -(a1),-(a6)
          dbra      d4,ice_unpa22
          bra       ice_unpa6
          .DC.b $7f,$ff,$00,$0e,$00,$ff,$00,$07
          .DC.b $00,$07,$00,$02,$00,$03,$00,$01
          .DC.b $00,$03,$00,$01
ice_unpa23:         .DC.b $00,$00,$01,$0d,$00,$00,$00,$0e
          .DC.b $00,$00,$00,$07,$00,$00,$00,$04
          .DC.b $00,$00,$00,$01
ice_unpa24:         .DC.b $09,$01,$00,$ff,$ff,$08,$04,$02
          .DC.b $01,$00
ice_unpa25:         .DC.b $0b,$04,$07,$00,$01,$1f,$ff,$ff
          .DC.b $00,$1f
ice_unpa26:         


;Entpackt auf sich selbst:
decrunch2:          link      a3,#-$00000078
          movem.l   d0-d7/a0-a6,-(sp)
          lea       120(a0),a4
          movea.l   a4,a6
          bsr.s     decrunch2_9
          cmpi.l    #$49434521,d0
          bne       decrunch2_8
          bsr.s     decrunch2_9
          lea       -8(a0,d0.l),a5
          bsr.s     decrunch2_9
          move.l    d0,(sp)
          adda.l    d0,a6
          movea.l   a6,a1
          moveq     #$77,d0
decrunch2_1:        move.b    -(a1),-(a3)
          dbra      d0,decrunch2_1
          movea.l   a6,a3
          move.b    -(a5),d7
          bsr.s     decrunch2_11
          movea.l   a3,a5
          bsr.s     decrunch2_15
          bcc.s     decrunch2_5
          move.w    #$0f9f,d7
decrunch2_2:        moveq     #3,d6
decrunch2_3:        move.w    -(a3),d4
          moveq     #3,d5
decrunch2_4:        add.w     d4,d4
          addx.w    d0,d0
          add.w     d4,d4
          addx.w    d1,d1
          add.w     d4,d4
          addx.w    d2,d2
          add.w     d4,d4
          addx.w    d3,d3
          dbra      d5,decrunch2_4
          dbra      d6,decrunch2_3
          movem.w   d0-d3,(a3)
          dbra      d7,decrunch2_2
decrunch2_5:        movem.l   (sp),d0-d7/a0-a3
decrunch2_6:        move.b    (a4)+,(a0)+
          subq.l    #1,d0
          bne.s     decrunch2_6
          moveq     #$77,d0
decrunch2_7:        move.b    -(a3),-(a5)
          dbra      d0,decrunch2_7
decrunch2_8:        movem.l   (sp)+,d0-d7/a0-a6
          unlk      a3
          rts       
decrunch2_9:        moveq     #3,d1
decrunch2_10:       lsl.l     #8,d0
          move.b    (a0)+,d0
          dbra      d1,decrunch2_10
          rts       
decrunch2_11:       bsr.s     decrunch2_15
          bcc.s     decrunch2_14
          moveq     #0,d1
          bsr.s     decrunch2_15
          bcc.s     decrunch2_13
          lea       decrunch2_28(pc),a1
          moveq     #4,d3
decrunch2_12:       move.l    -(a1),d0
          bsr.s     decrunch2_17
          swap.w    d0
          cmp.w     d0,d1
          dbne      d3,decrunch2_12
          add.l     20(a1),d1
decrunch2_13:       move.b    -(a5),-(a6)
          dbra      d1,decrunch2_13
decrunch2_14:       cmpa.l    a4,a6
          bgt.s     decrunch2_20
          rts       
decrunch2_15:       add.b     d7,d7
          bne.s     decrunch2_16
          move.b    -(a5),d7
          addx.b    d7,d7
decrunch2_16:       rts       
decrunch2_17:       moveq     #0,d1
decrunch2_18:       add.b     d7,d7
          bne.s     decrunch2_19
          move.b    -(a5),d7
          addx.b    d7,d7
decrunch2_19:       addx.w    d1,d1
          dbra      d0,decrunch2_18
          rts       
decrunch2_20:       lea       decrunch2_29(pc),a1
          moveq     #3,d2
decrunch2_21:       bsr.s     decrunch2_15
          dbcc      d2,decrunch2_21
          moveq     #0,d4
          moveq     #0,d1
          move.b    1(a1,d2.w),d0
          ext.w     d0
          bmi.s     decrunch2_22
          bsr.s     decrunch2_17
decrunch2_22:       move.b    6(a1,d2.w),d4
          add.w     d1,d4
          beq.s     decrunch2_24
          lea       decrunch2_30(pc),a1
          moveq     #1,d2
decrunch2_23:       bsr.s     decrunch2_15
          dbcc      d2,decrunch2_23
          moveq     #0,d1
          move.b    1(a1,d2.w),d0
          ext.w     d0
          bsr.s     decrunch2_17
          add.w     d2,d2
          add.w     6(a1,d2.w),d1
          bpl.s     decrunch2_26
          sub.w     d4,d1
          bra.s     decrunch2_26
decrunch2_24:       moveq     #0,d1
          moveq     #5,d0
          moveq     #-1,d2
          bsr.s     decrunch2_15
          bcc.s     decrunch2_25
          moveq     #8,d0
          moveq     #$3f,d2
decrunch2_25:       bsr.s     decrunch2_17
          add.w     d2,d1
decrunch2_26:       lea       2(a6,d4.w),a1
          adda.w    d1,a1
          move.b    -(a1),-(a6)
decrunch2_27:       move.b    -(a1),-(a6)
          dbra      d4,decrunch2_27
          bra       decrunch2_11
          .DC.b $7f,$ff,$00,$0e,$00,$ff,$00,$07
          .DC.b $00,$07,$00,$02,$00,$03,$00,$01
          .DC.b $00,$03,$00,$01
decrunch2_28:       .DC.b $00,$00,$01,$0d,$00,$00,$00,$0e
          .DC.b $00,$00,$00,$07,$00,$00,$00,$04
          .DC.b $00,$00,$00,$01
decrunch2_29:       .DC.b $09,$01,$00,$ff,$ff,$08,$04,$02
          .DC.b $01,$00
decrunch2_30:       .DC.b $0b,$04,$07,$00,$01,$1f,$ff,$ff
          .DC.b $00,$1f
decrunch2_31:       


;Füllt eine Fläche auf dem Bildschirm unter Verwendung der Line-Funktion
; 16(a6).W: y2
;  14(a6).W: x2
;  12(a6).W: y1
;  10(a6).W: x1
;   8(a6).W: Color
mcode54:  link      a6,#0
          movem.l   d0-d4,-(sp)

          movem.w   8(a6),d0-d4
          cmp.w     d2,d4
          bge.s     fuellen
          exg.l     d2,d4     ;immer von oben nach unten füllen
fuellen:  move.w    d2,-(sp)
          move.w    d3,-(sp)
          move.w    d2,-(sp)
          move.w    d1,-(sp)
          move.w    d0,-(sp)
          bsr       mcode53
          lea.l     10(sp),sp

          addq.w    #1,d2
          cmp.w     d4,d2
          ble.s     fuellen

          movem.l   (sp)+,d0-d4
          unlk      a6
          rts       


;Zeichnet eine Linie auf dem Screen, der durch logbase bestimmt wird. Zum
;Zeichnen der Linie kann auch eine Farbe angegeben werden.
; 16(a6).W: y2
;  14(a6).W: x2
;  12(a6).W: y1
;  10(a6).W: x1
;   8(a6).W: Color
mcode53:  link      a6,#0

          movem.l   d0-d7/a0-a5,-(sp)
          move.w    8(a6),d6  ;Farbe
          bsr       self_modify         ;entsprechenden Code erzeugen
          movem.w   10(a6),d0-d3        ;Parameter vom Stack holen

          cmp.w     d1,d3     ;y1 kleiner y2?
          bge.s     richtungok          ;war vorher bgt
          exg.l     d0,d2
          exg.l     d1,d3
richtungok:         sub.w     d1,d3     ;delta-y
          sub.w     d0,d2     ;delta-x
          movea.l   d1,a5     ;Y-Zeilenzähler (Startzeile)
          mulu.w    #160,d1
          movea.l   logbase,a0
          move.w    d0,d4     ;Start_x
          andi.w    #$fff0,d4
          lsr.w     #1,d4
          add.w     d4,d1
          adda.w    d1,a0     ;-> bildschirmadresse
          not.w     d0
          andi.w    #15,d0
          moveq.l   #0,d4
          bset      d0,d4     ;-> bitmuster
          tst.w     d2        ;in welche Richtung starten?
          bmi.s     linksrum
          cmp.w     d2,d3     ;Delta_y größer Delta_x?
          bgt.s     dygrdx1   ;ja, diese Routine
;flache gerade
          move.w    d2,d7
;d0=zaehler d2/d3 -> delta's
          move.w    d2,d0
          lsr.w     #1,d0
          moveq.l   #0,d5
gloop1:   
;punkt setzen
          or.w      d4,d5
          ror.w     #1,d4
          bcc.s     nichtneueswort1
          bsr       setze_farben
          addq.l    #8,a0     ;Eine Plane weiter
          moveq.l   #0,d5
nichtneueswort1:    sub.w     d3,d0     ;dx-dy
          bcc.s     keinsprung1         ;war vorher bhi
          add.w     d2,d0
          bsr       setze_farben
          moveq.l   #0,d5
          lea.l     160(a0),a0          ;eine Zeile tiefer
          addq.l    #1,a5     ;Y-Counter++
keinsprung1:        dbra      d7,gloop1
          bsr       setze_farben

line_end: movem.l   (sp)+,d0-d7/a0-a5
          unlk      a6
          rts       


dygrdx1:  
;steile gerade
          move.w    d3,d7     ;Zeilencounter
;d0=zaehler d2/d3 -> delta's
          move.w    d3,d0     ;Höhe des Blocks
          lsr.w     #1,d0     ;halbieren
          move.w    d4,d5     ;Bitmuster
gloop2:   
;punkt setzen
          bsr       setze_farben
          lea.l     160(a0),a0
          addq.w    #1,a5     ;Eine Zeile tiefer
          sub.w     d2,d0     ;dy-dx
          bcc.s     keinsprung2         ;war vorher bhi konnte dann aber kein 2 Pixel hohe Linie zeichnen
          add.w     d3,d0
          ror.w     #1,d5
          bcc.s     nichtneueswort2
          addq.l    #8,a0
nichtneueswort2:    
keinsprung2:        
          dbra      d7,gloop2
          bra.s     line_end

linksrum: 
          neg.w     d2
          cmp.w     d2,d3
          bgt.s     dygrdx2
;flache gerade
          move.w    d2,d7
;d0=zaehler d2/d3 -> delta's
          move.w    d2,d0
          lsr.w     #1,d0
          moveq.l   #0,d5
gloop3:   
;punkt setzen
          or.w      d4,d5
          rol.w     #1,d4
          bcc.s     nichtneueswort3
          bsr       setze_farben
          subq.l    #8,a0
          moveq.l   #0,d5
nichtneueswort3:    
          sub.w     d3,d0     ;dx-dy
          bhi.s     keinsprung3
          add.w     d2,d0
          bsr.s     setze_farben
          moveq.l   #0,d5
          lea.l     160(a0),a0
          addq.l    #1,a5     ;Eine Zeile tiefer
keinsprung3:        
          dbra      d7,gloop3
          bsr.s     setze_farben
          bra       line_end

dygrdx2:  
;steile gerade
          move.w    d3,d7
;d0=zaehler d2/d3 -> delta's
          move.w    d3,d0
          lsr.w     #1,d0
          move.w    d4,d5
gloop4:   
;punkt setzen
          bsr.s     setze_farben
          lea.l     160(a0),a0
          addq.l    #1,a5     ;Eine Zeile tiefer
          sub.w     d2,d0     ;dy-dx
          bhi.s     keinsprung4
          add.w     d3,d0
          rol.w     #1,d5
          bcc.s     nichtneueswort4
          subq.l    #8,a0
nichtneueswort4:    
keinsprung4:        
          dbra      d7,gloop4
          bra       line_end

;Selbst modfiziert sich, je nach Farbe, Linientyp und Füllmuster
; D6: Farbe
self_modify:        move.w    line_modus,d0
          lsl.w     #4,d0     ;16 Bytes pro Opcode
          lea.l     opcodes,a0
          adda.w    d0,a0
          lea.l     donne_le_moi,a1     ;Hier aufbauen

          moveq.l   #0,d1
          moveq.l   #3,d7     ;4 Planes
put_word: moveq.l   #8,d0     ;weglöschen als Default
          lsr.w     #1,d6     ;Farbe schieben
          bcc.s     set_plane
          moveq.l   #0,d0     ;setzen
set_plane:          tst.l     0(a0,d0.w)          ;opcode vorhanden?
          beq.s     nxt_plane
          move.w    0(a0,d0.w),(a1)+
          move.w    d1,(a1)+  ;Wertigkeit in der Plane
          tst.l     4(a0,d0.w)
          beq.s     nxt_plane
          move.w    4(a0,d0.w),(a1)+
          move.w    d1,(a1)+
nxt_plane:          addq.w    #2,d1
          dbra      d7,put_word
          move.l    schluss,(a1)        ;RTS etc.

          movea.l   fuellmuster,a2      ;Adresse des Füllmusters
          rts       

;Setzt die Farbe im Bildschirmspeicher
; D5: Bitmuster
;  A0: Bildschirmadresse
setze_farben:       movea.l   d7,a3     ;d7 retten

          move.w    d5,d6     ;Linientyp
          not.w     d6        ;Maske berechnen
          move.w    a5,d7     ;Y-Counter holen
          andi.w    #%1111,d7 ;nur 16'er
          add.w     d7,d7
          and.w     0(a2,d7.w),d5       ;Mit Füllmuster verknüpfen
          move.w    d5,d7
          not.w     d7        ;und negierte Linie berechnen

donne_le_moi:       and.w     d6,0(a0)
          or.w      d5,0(a0)
          and.w     d6,0(a0)
          or.w      d5,2(a0)
          and.w     d6,0(a0)
          or.w      d5,4(a0)
          and.w     d6,0(a0)
          or.w      d6,6(a0)
schluss:  move.l    a3,d7     ;d7 restaurieren
          rts       


;Dies ist eine Tabelle für die verschiedenen Füllmodi. Dabei
;steht zuerst der Opcode für Replace, dann Eor und schliesslich Transparent.
;Es sind jeweils 2 Befehle zum Setzen von Punkten und 2 zum Löschen.
opcodes:  and.w     d6,0(a0)  ;Replace, Maskieren
          or.w      d5,0(a0)  ;und Füllmuster rein
          and.w     d6,0(a0)  ;weglöschen, Maskieren
          and.w     d7,0(a0)  ;und Punkte wieder weg

          eor.w     d5,0(a0)  ;Eor setzen
          .DC.l 0
          .DC.l 0
;          eor.w     d5,0(a0)  ;Eor löschen
          .DC.l 0

          or.w      d5,0(a0)  ;Transparent setzen
          .DC.l 0
          and.w     d7,0(a0)  ;Transparent löschen
          .DC.l 0

;Mit Hilfe dieser Funktion können die Parameter für die Line-Funktion
;eingestellt werden. Da die Fill Funktion die Line-Funktion benützt, gelten
;die Einstellungen auch für die Fill-Funktion:
;Die Funktion gibt den alten Linientyp zurück, bei einer Übergabe von -1 wird
;der Linientyp nicht verändert.
;0=Replace, 1=Eor, 2=Transparent
;int line_modus(int modus);
mcode58:  move.w    line_modus,d0
          tst.w     4(sp)     ;Negativ?
          bmi.s     gib_zurueck
          move.w    4(sp),line_modus
gib_zurueck:        rts       

;Bestimmt die Farbe eines Pixels auf logbase
;->  8(a6): x
;   10(a6): y
;int get_pixel(x,y);
mcode59:  link      a6,#0
          movem.l   d1-d3/a0,-(sp)

          movea.l   logbase,a0
          move.w    10(a6),d0
          mulu.w    #160,d0
          adda.l    d0,a0
          move.w    8(a6),d0  ;x
          lsr.w     #4,d0
          lsl.w     #3,d0     ;BB berechnen
          adda.w    d0,a0
          move.w    8(a6),d0
          andi.w    #%1111,d0
          moveq.l   #15,d1
          sub.w     d0,d1
          moveq.l   #0,d0     ;Hier Farbe aufbauen
          moveq.l   #3,d3     ;4 Planes
plane_loop7:        ror.b     #1,d0
          move.w    (a0)+,d2
          btst      d1,d2     ;Farbe gesetzt?
          beq.s     naechste_plane
          addq.w    #1,d0
naechste_plane:     dbra      d3,plane_loop7
          ror.b     #5,d0     ;an richtige Position schieben
          movem.l   (sp)+,d1-d3/a0
          unlk      a6
          rts       


;Bestimmt das Muster, mit dem gefüllt werden soll. Diese Einstellung gilt für
;Linienziehen, wie für füllen.
;Es wird hierbei ein Pointer auf 16 Integers übergeben, in denen das Füllmuster
;zeilenweise abgespeichert wird. Es wird also, je nach y-Zeile % 16 die
;entsprechende Zeile aus dem Muster geholt, und damit die Linie verknüpft.
;Ist das gesamte Integerarray mit -1 gefüllt, also alle Bits gesetzt, wird
;ganz normal gefüllt.
;Wird als Integeradresse -1L angegeben, so wird die Adresse des letzten
;Füllmusters zurückgegeben.

;int *line_pattern(int *pattern);
mcode60:  move.l    fuellmuster,d0      ;Adresse des alten Füllmuster
          tst.l     4(sp)
          bmi.s     gib_zurueck2
          move.l    4(sp),fuellmuster   ;Adresse
gib_zurueck2:       rts       

;Dies ist die Diskettenfunktion. Ist der Modus 0, wird das TOS zum Laden
;benutzt, sonst eine eigene Diskettenroutine. Diese Routine prüft vor dem
;Laden, ob ein entsprechendes Archiv vorhanden ist, bzw. fordert zur Einlage
;der jeweiligen Diskette auf. Gibt die tatsächliche Länge der gelesenen Daten
;zurück.
;long loaddisk(archiv,offset,länge,adr)
;-> 18(a6): Ladeadresse
;   14(a6): Länge der zu ladenden Daten
;   10(a6): Offset innerhalb des Archivs
;    8(a6): archiv [-2,0..n] -2=MEDUSA.PRG (für Overlay)
mcode61:  link      a6,#0
          movem.l   d1-d7/a0-a5,-(sp)

          tst.w     tos_da    ;TOS benutzen?
          bne       use_tos   ;ja

          move.w    8(a6),-(sp)         ;Nummer der Diskette
          bsr       mcode69   ;User soll ggf. Diskette wechseln
          addq.l    #2,sp

once_again:         move.l    10(a6),d0 ;Offset innerhalb Archiv
          add.l     14(a6),d0 ;Erstes freies Byte
          cmp.l     datei_laenge,d0     ;Kleiner als Dateilänge?
          ble.s     laenge_okay
          move.l    datei_laenge,d0
          sub.l     10(a6),d0 ;-Offset
          move.l    d0,14(a6) ;Als neue Länge merken
          move.l    datei_laenge,d0     ;bis zum Ende lesen
laenge_okay:        subq.l    #1,d0     ;Letztes benutztes Byte
          divu.w    #512,d0   ;Anzahl an Sektoren vom Start
          swap.w    d0
          move.w    d0,-(sp)  ;End_Offset
          swap.w    d0
          add.w     first_sek,d0        ;Archiv startet hier
          ext.l     d0
          divu.w    sek_per_track,d0    ;10 Sektoren pro Track
          swap.w    d0        ;Sektornummer
          addq.w    #1,d0
          move.w    d0,-(sp)  ;End_Sektor
          swap.w    d0
          move.w    d0,-(sp)  ;End_Track
          move.l    10(a6),d0 ;Startoffset
          divu.w    #512,d0
          swap.w    d0
          move.w    d0,-(sp)  ;Startoffset
          swap.w    d0
          add.w     first_sek,d0        ;Archiv startet hier
          ext.l     d0
          divu.w    sek_per_track,d0    ;10 Sektoren pro Track
          swap.w    d0        ;Sektornummer
          addq.w    #1,d0
          move.w    d0,-(sp)  ;Startsektor
          swap.w    d0
          move.w    d0,-(sp)  ;Starttrack
          clr.w     -(sp)     ;A:
          move.l    18(a6),-(sp)        ;Ladeadresse
          bsr       mcode68   ;Floppy_Read
          lea       18(sp),sp
          move.l    14(a6),d0 ;Länge der Daten zurückgeben

ende_load:          movem.l   (sp)+,d1-d7/a0-a5
          unlk      a6
          rts       

use_tos:  move.w    8(a6),d0  ;Archivnummer holen
          lea       rom_grfx,a0
          move.l    #'ROM_',(a0)+
          move.l    #'GRFX',(a0)+
          move.b    #'.',(a0)+
          addi.w    #'0',d0
          move.b    d0,(a0)+
          clr.b     (a0)

try_again1:         clr.w     -(sp)
          pea       rom_grfx
          move.w    #$3d,-(sp)          ;Datei öffnen
          trap      #1
          addq.l    #8,sp
          move.w    d0,fhandle
          bpl.s     datei_offen

          bsr       please_insert       ;Diskette einlegen
          bra       try_again1

datei_offen:        clr.w     -(sp)     ;Ab Dateistart
          move.w    fhandle,-(sp)
          move.l    10(a6),-(sp)        ;Offset
          move.w    #$42,-(sp)
          trap      #1
          lea.l     10(sp),sp

          move.l    18(a6),-(sp)        ;Ladeadresse
          move.l    14(a6),-(sp)        ;Länge
          move.w    fhandle,-(sp)
          move.w    #$3f,-(sp)          ;Fread
          trap      #1
          lea.l     12(sp),sp
          move.l    d0,d7     ;Länge merken

          move.w    fhandle,-(sp)
          move.w    #$3e,-(sp)
          trap      #1        ;Fclose
          addq.l    #4,sp

          move.l    d7,d0     ;und Länge zurückgeben
          bra       ende_load

please_insert:      bsr       deselect  ;Alle Lichter aus
          move.w    8(a6),d0  ;Archivnummer
          addi.w    #'A',d0
          move.b    d0,insert_disk+30

          lea.l     insert_disk,a0
          jsr       alert     ;und Alert ausgeben

          move.w    #0,-(sp)  ;A:
          bsr       media_chg ;und Media-Change simulieren
          addq.l    #2,sp
          rts       

rom_grfx: .DC.b 'ROM_GRFX.x',0
          .EVEN 
fhandle:  .DC.w 0

;Erzeugt auf entsprechendem Laufwerk einen Media-Wechsel
;C-DEFINITON: media(int laufwerk)

;***********************
; Force Media Change
;
; zwingt GEMDOS, einen Medienwechsel für
; ein bestimmtes Laufwerk anzuerkennen.
; nach den Empfehlungen in der Doku zu
; TOS 1.4.
;
; USAGE:
;     void mediach(devno);
;     int devno;
;
;***********************

media_chg:          move.w    4(sp),d0  ; Laufwerksnummer
          lea.l     mydev(pc),a0
          move.w    d0,(a0)   ; merken
          addi.b    #'A',d0
          lea.l     fspec(pc),a0
          move.b    d0,(a0)   ; Laufwerkskennung setzen

loop:     
          lea.l     o_bpb2(pc),a0
          move.l    $472,(a0) ; alten BPB-Vektor retten
          move.l    $47e,4(a0)          ; alten MEDIA-Vektor retten
          move.l    $476,8(a0)          ; alten RWABS-Vektor retten

          move.l    #my_bpb2,$472       ; neue Vektoren setzen
          move.l    #my_media2,$47e
          move.l    #my_rwabs2,$476

          pea       dta2(pc)  ; DTA-Adresse
          move.w    #$1a,-(sp)          ; Fsetdta
          trap      #1
          addq.l    #6,sp
          clr.w     -(sp)     ; Suchattribut 0
          pea       fspec(pc) ; -> Suchmuster
          move.w    #$4e,-(sp)          ; Fsfirst
          trap      #1
          addq.l    #8,sp

          move.l    o_bpb2(pc),$472     ; alte Vektoren wieder installieren
          move.l    o_media2(pc),$47e
          move.l    o_rwabs2(pc),$476

done:     rts       

; eigene BPB-Routine
my_bpb2:  
          move.w    mydev(pc),d0
          move.w    4(sp),d1
          cmp.w     d1,d0     ; ist es das gewünschte Laufwerk?
          bne.s     dooldg    ; nein
          moveq.l   #0,d0
          rts       
dooldg:   
          movea.l   o_bpb2(pc),a0       ; alten BPB-Vektor holen
          jmp       (a0)      ; zur alten Routine


; eigene MEDIA-Routine
; liefert für das gewünschte Laufwerk generell 2
; (auf jeden Fall gewechselt)
my_media2:          
          move.w    mydev(pc),d0
          move.w    4(sp),d1
          cmp.w     4(sp),d0  ; paßt die Laufwerksnummer?
          bne.s     dooldm    ; nein, alten Vektor anspringen
          moveq.l   #2,d0     ; 2 = garantiert gewechselt
          rts       
dooldm:   
          movea.l   o_media2(pc),a0     ; alten Vektor holen
          jmp       (a0)

; eigene RWABS-Routine
; liefert für das gewünschte Laufwerk -14 (E_CHG)

my_rwabs2:          
          move.w    mydev(pc),d0
          move.w    14(sp),d1
          cmp.w     14(sp),d0 ; das passende Laufwerk?
          bne.s     dooldr    ; nein
          moveq.l   #-14,d0   ; E_CHG liefern
          rts       

dooldr:   
          movea.l   o_rwabs2(pc),a0     ; alten Vektor anspringen
          jmp       (a0)

fspec:    
          .DC.b "X:\\*.*",0   ; Suchmuster

          .EVEN 
dta2:     
          .DC.l 0,0,0,0,0,0,0,0,0,0,0
mydev:    
          .DC.w 0
o_bpb2:   
          .DC.l 0
o_media2: 
          .DC.l 0
o_rwabs2: 
          .DC.l 0


;Diese Funktion initialisiert die Musik, als Parameter wird ihr die
;VBL-Routine der Musik übergeben. Diese wird mit 50 Hz angesprungen.
;void init_music((void *)(musik)(),int song_nr); (14)
mcode63:  link      a6,#0
          movem.l   d0-d7/a0-a6,-(sp)

          move.w    #$2700,sr ;IRQ's sperren

          move.l    8(a6),music_routine

          tst.b     first_music         ;1. Aufruf?
          bne.s     re_init

          st        first_music

          move.l    #music_irq,$110

          bclr      #4,$fffffa09        ;Timer D IRQ sperren
          andi.b    #%11110000,$fffffa1d          ;Timer D Stop1
          move.b    #246,$fffffa25      ;Counterwert eintragen
          bset      #4,$fffffa15        ;Timer D nicht maskieren
          move.b    #%11101111,$fffffa0d          ;Pending Bit löschen
          ori.b     #%111,$fffffa1d     ;Vorteiler teilt durch 200
          bset      #4,$fffffa09        ;Timer D erlauben

          bclr      #4,$fffffa11        ;In-Service löschen

re_init:  move.w    12(a6),d0
          movea.l   music_routine,a0
          jsr       (a0)      ;Und Song einstellen (14)

          move.w    #$2300,sr ;IRQ's ermöglichen

          movem.l   (sp)+,d0-d7/a0-a6
          unlk      a6
          rts       

music_irq:          move.w    #$2500,sr ;HBL zulassen
          tst.w     musik_an  ;Soll Musik dudeln?
          beq       keine_musik

          movem.l   d0-d7/a0-a6,-(sp)   ;Register retten

          movea.l   music_routine,a0
          jsr       8(a0)     ;In VBL-Routine springen

          movem.l   (sp)+,d0-d7/a0-a6
keine_musik:        bclr      #4,$fffffa11        ;In-Service Bit löschen
          rte       

music_routine:      .DC.l 0   ;Adresse der Musikroutine
musik_an: .DC.w -1  ;Musik ist an!


;Diese Routine speichert einen Spielstand auf Diskette und zwar immer genau
;41472 Byte. (9 Tracks)
;int save_data(nr,adr)
mcode64:  link      a6,#0
          movem.l   d1-d7/a0-a5,-(sp)

          move.w    #-1,-(sp)
          bsr       mcode69   ;Bitte Datendisk einlegen
          addq.l    #2,sp

          move.w    #511,-(sp)
          move.w    #9,-(sp)
          move.w    8(a6),d0
          mulu.w    #9,d0     ;(12)
          addi.w    #8+3,d0   ;(12)
          move.w    d0,-(sp)
          clr.w     -(sp)
          move.w    #1,-(sp)
          subq.w    #8,d0     ;(12)
          move.w    d0,-(sp)
          clr.w     -(sp)     ;A:
          move.l    10(a6),-(sp)
          bsr       mcode70   ;Daten schreiben
          lea       18(sp),sp

zurueck2: movem.l   (sp)+,d1-d7/a0-a5
          unlk      a6
          rts       

;Diese Routine lädt einen Spielstand von Diskette und zwar immer genau
;41472 Bytes. (9 Tracks)
;int load_data(nr,adr)
mcode65:  link      a6,#0
          movem.l   d1-d7/a0-a5,-(sp)

          move.w    #-1,-(sp)
          bsr       mcode69   ;Bitte Datendisk einlegen
          addq.l    #2,sp

          move.w    #512,-(sp)
          move.w    #9,-(sp)
          move.w    8(a6),d0
          mulu.w    #9,d0     ;(12)
          addi.w    #8+3,d0   ;(12)
          move.w    d0,-(sp)
          clr.w     -(sp)
          move.w    #1,-(sp)
          subq.w    #8,d0     ;(12)
          move.w    d0,-(sp)
          clr.w     -(sp)     ;A:
          move.l    10(a6),-(sp)
          bsr       mcode68
          lea       18(sp),sp

zurueck1: movem.l   (sp)+,d1-d7/a0-a5
          unlk      a6
          rts       

load_from_tos:      
          move.w    8(a6),d0
          addi.w    #'0',d0
          move.b    d0,number
          clr.w     -(sp)
          pea       fn
          move.w    #$3d,-(sp)
          trap      #1
          addq.l    #8,sp
          tst.w     d0
          bmi.s     zurueck1

          move.w    d0,-(sp)

          move.l    10(a6),-(sp)
          move.l    #32000,-(sp)
          move.w    d0,-(sp)
          move.w    #$3f,-(sp)
          trap      #1
          lea       12(sp),sp

          move.w    #$3e,-(sp)
          trap      #1
          addq.l    #4,sp
          bra.s     zurueck1

save_to_tos:        
          move.w    8(a6),d0
          addi.w    #'0',d0
          move.b    d0,number
          clr.w     -(sp)
          pea       fn
          move.w    #$3c,-(sp)
          trap      #1
          addq.l    #8,sp
          tst.w     d0
          bmi       zurueck2

          move.w    d0,-(sp)

          move.l    10(a6),-(sp)
          move.l    #32000,-(sp)
          move.w    d0,-(sp)
          move.w    #$40,-(sp)
          trap      #1
          lea       12(sp),sp

          move.w    #$3e,-(sp)
          trap      #1
          addq.l    #4,sp
          bra       zurueck2

fn:       .DC.b 'ROM_SAVE.'
number:   .DC.b '0',0
          .EVEN 

;Diese Funktion liefert eine 16-Bit 'Zufallszahl'.
mcode66:  move.b    $ffff8209,d0
          lsl.w     #8,d0
          move.b    $ffff8207,d0
          rts       


;Diese Funktion überprüft, ob ein Zeichen vorhanden ist.
;FLAG is_key(void);
mcode67:  move.l    d1,-(sp)

          moveq.l   #0,d0     ;Kein Zeichen da

          move.w    head,d1
          cmp.w     tail,d1
          beq.s     nichts_da3

          moveq.l   #-1,d0    ;Ja, es ist was da!

nichts_da3:         move.l    (sp)+,d1
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

mcode68:  link      a6,#0
          movem.l   d0-d7/a0-a5,-(sp)

          move.w    12(a6),d0 ;Laufwerk holen
          bsr       select    ;selektieren

          lea       $ffff8604,a3        ;FDC
          lea       2(a3),a4  ;DMA
          lea       $fffffa01,a5        ;MFP
          movea.l   8(a6),a2  ;Ladeadresse

          move.w    14(a6),d6 ;Starttrack
          bsr       seek_track

          moveq     #1,d0     ;1 Sektor lesen
          lea       bitfeld,a0          ;hierhin
          bsr       read_sektoren

          move.w    14(a6),d4 ;Starttrack
          move.w    20(a6),d5 ;Endtrack
          cmp.w     d4,d5     ;nur auf einem Track lesen?
          beq.s     nur_ein_track

mehr_als_einer:     bsr       trans_start         ;Ab Startoffset übertragen

          addq.w    #1,16(a6) ;nächsten Sektor geht's weiter
          move.w    sek_per_track,d0
          cmp.w     16(a6),d0 ;Startsektor 10?
          bge.s     no_step   ;Nein, also kein Step

vorne_anfangen:     move.w    #1,16(a6) ;Wieder bei Sektor #1 starten
          addq.w    #1,14(a6) ;Auf nächsten Track gehen
          move.w    14(a6),d6
          bsr       seek_track          ;Track anfahren

no_step:  move.w    14(a6),d4 ;Starttrack
          cmp.w     20(a6),d4 ;Endtrack erreicht?
          beq.s     letzter_track

read_again2:        move.w    sek_per_track,d0
          addq.w    #1,d0
          sub.w     16(a6),d0 ;Anzahl der zu lesenden Sektoren
          movea.l   a2,a0     ;Ab Ladeadresse laden
          bsr.s     read_sektoren
          adda.w    d5,a2     ;Bytes überspringen
          bra.s     vorne_anfangen

letzter_track:      move.w    22(a6),d0 ;Endsektor
          sub.w     16(a6),d0 ;-Startsektor
          beq.s     nur_ein_sektor
          move.w    d0,-(sp)  ;Anzahl der Sektoren retten
          movea.l   a2,a0     ;Ladeadresse
          bsr.s     read_sektoren
          adda.w    d5,a2     ;Überspringen
          move.w    (sp)+,d0  ;Anzahl der gelesenen Sektoren
          add.w     d0,16(a6) ;=Nächster Sektor

nur_ein_sektor:     moveq     #1,d0
          lea       bitfeld,a0
          bsr.s     read_sektoren       ;Letzten Sektor lesen
          lea       bitfeld,a0
          move.w    24(a6),d7 ;Endoffset
trans3:   move.b    (a0)+,(a2)+         ;Rest übertragen
          dbra      d7,trans3
          bra.s     aus_is

nur_ein_track:      move.w    22(a6),d0 ;Endsektor
          sub.w     16(a6),d0 ;-Startsektor
          bne       mehr_als_einer

          lea       bitfeld,a0
          adda.w    18(a6),a0 ;+Startoffset
          move.w    24(a6),d7 ;Endoffset
          sub.w     18(a6),d7 ;-Startoffset
trans4:   move.b    (a0)+,(a2)+
          dbra      d7,trans4

aus_is:   move.b    #-1,flop_aktiv      ;Transfer beendet
          movem.l   (sp)+,d0-d7/a0-a5
          unlk      a6
          rts       


;Liest ab Startsektor Sektoren ein. Gibt in D5 512*n zurück.
;-> D0: Anzahl der Sektoren
;   A0: Ladeadresse
read_sektoren:      move.w    d0,-(sp)  ;Anzahl retten
read_error:         move.w    (sp),d0   ;Anzahl holen
          mulu.w    #512,d0
          move.w    d0,d5     ;Länge merken (für nachher)
          bsr       setdma
          bsr       rdtoggle
          move.w    (sp),d7   ;Sektorcounter
          bsr       fdcout
          move.w    #$0084,(a4)         ;Sektorregister
          move.w    16(a6),d7 ;Startsektor holen
          bsr       fdcout
          move.w    #$0080,(a4)         ;Commandoregister
          move.w    #$0090,d7 ;Read multiple
          bsr       fdcout
          bsr       waitfertig
          tst.w     d0        ;Alles Okay?
          bne.s     read_error
          tst.w     (sp)+     ;D0 runternehmen
          rts       

;Überträgt 1. Sektor ab Startoffset bis Sektorende
trans_start:        move.w    18(a6),d0 ;Startoffset
          lea       bitfeld,a0
          adda.w    d0,a0     ;Bytes überspringen
          move.w    #512,d1   ;Soviel Bytes kommen
          sub.w     d0,d1     ;Offset abziehen=Anzahl an Bytes
          bra.s     trans1
trans2:   move.b    (a0)+,(a2)+
trans1:   dbra      d1,trans2
          rts       

restore_fdc:        move.w    #$0080,(a4)         ;Kommandoregister
          move.w    #$0003,d7 ;Restore
          clr.b     dflag     ;Kein DMA-Transfer
          bsr       fdcout
          bsr       waitfertig
          cmpi.w    #4,d0     ;Spur 0 erreicht?
          bne.s     restore_fdc

;Fährt Track D6 an, macht ggf. Restore
seek_track:         tst.b     track_display       ;(9) ab hier...
          beq       kein_display        ;(9) kein Display

          movem.l   d0-d7/a0-a6,-(sp)

          lea       track_nr,a0
          ext.l     d6
          divu.w    #10,d6
          addi.w    #'0',d6
          move.b    d6,(a0)+
          swap.w    d6
          addi.w    #'0',d6
          move.b    d6,(a0)+

          lea       track_str,a0
          move.w    #284,d0
string_loop:        moveq     #0,d2
          move.b    (a0)+,d2  ;Zeichen holen
          beq.s     loop_ende

          move.w    d2,-(sp)  ;Zeichen
          clr.w     -(sp)     ;Y
          move.w    d0,-(sp)  ;X
          bsr       mcode1
          addq.l    #6,sp

          addq.w    #4,d0     ;eine Position weiter
          bra.s     string_loop

loop_ende:          movem.l   (sp)+,d0-d7/a0-a6   ;(9) ... bis hier

kein_display:       move.w    #$0086,(a4)         ;Datareg.
          move.w    d6,d7     ;Starttrack setzen
          bsr.s     fdcout
          move.w    #$0080,(a4)         ;Commandoreg.
          move.w    #$0017,d7 ;Seek mit Verify
          clr.b     dflag     ;Kein DMA-Transfer
          bsr.s     fdcout
          bsr.s     waitfertig          ;auf Ausführung warten
          andi.w    #%0000000000011000,d0         ;nur CRC/RNF
          bne       restore_fdc         ;nicht gefunden, Restore first
          rts       

track_str:          .DC.b "TRACK: "
track_nr: .DC.b "xx",0
          .EVEN 

;Setzt die DMA-Adresse
setdma:   pea       (a0)      ;Hier hinlesen
          move.b    3(sp),7(a4)         ;Adresse in DMA
          move.b    2(sp),5(a4)
          move.b    1(sp),3(a4)
          addq.l    #4,sp
          move.l    a0,dend
          ext.l     d0
          add.l     d0,dend   ;End-Adresse berechnen
          move.b    #1,dflag  ;DMA-Transfer findet statt
          rts       

rdtoggle: move.w    #$0090,(a4)
          move.w    #$0190,(a4)
          move.w    #$0090,(a4)
          rts       

fdcout:   bsr.s     waitfdc
          move.w    d7,(a3)   ;Wert schreiben

waitfdc:  move.w    sr,-(sp)
          move.l    d0,-(sp)
          moveq     #$30,d0
waitloop: dbra      d0,waitloop
          move.l    (sp)+,d0
          move.w    (sp)+,sr
          rts       

; wartet auf vollständige Ausführung
; veränderte Register: ?
waitfertig:         move.l    #$00060000,d7       ;Timeout Zähler
poll:     btst      #5,(a5)   ;IRQ ausgelöst?
          beq.s     fix_und_fertig
          subq.l    #1,d7
          beq.s     timeout   ;Normalerweise Timeout
          tst.b     dflag     ;DMA-Transfer?
          beq.s     poll      ;Nein, weiterwarten

          moveq     #0,d0
          move.b    $ffff8609,d0
          lsl.w     #8,d0
          move.b    $ffff860b,d0        ;DMA-Adresse holen
          lsl.l     #8,d0
          move.b    $ffff860d,d0
          cmp.l     dend,d0
          blt.s     poll

          clr.b     dflag
fix_und_fertig:     move.w    (a3),d0   ;FDC-Status lesen
          move.w    d0,disk_status
fertig2:  andi.w    #%0000000000011100,d0         ;CRC-Lost etc. isolieren
          move.w    #$0080,(a4)         ;Commandoregister
          move.w    #$00d0,d7
          bsr.s     fdcout
          move.w    #250,d1
as_time_goes:       dbra      d1,as_time_goes
          rts       

timeout:  movem.l   d0/d1,-(sp)

          moveq     #5,d1
flsh:     addq.w    #1,$ffff8240        ;.w
          dbra      d0,flsh
          dbra      d1,flsh

          movem.l   (sp)+,d0/d1
          moveq     #-1,d0    ;Fehler!
          bra.s     fertig2

;Selektiert ein Laufwerk. In D0:
;Bit 0: Laufwerk
;Bit 1: Seite
select:   clr.b     flop_aktiv          ;Jetzt ist Floppy aktiv

          move.w    sr,-(sp)
          ori.w     #$0700,sr
          move.w    d0,d1
          lsr.w     #1,d0     ;Seite runterschieben
          andi.w    #%0000000000000001,d0         ;Seite isolieren
          andi.w    #%0000000000000001,d1         ;Laufwerk isolieren
          addq.w    #1,d1
          add.w     d1,d1
          or.w      d0,d1     ;Seite einodern
          eori.w    #7,d1
          move.b    #14,$ffff8800
          move.b    $ffff8802,d0
          andi.w    #$00f8,d0
          or.w      d1,d0
          move.b    d0,$ffff8802

          move.w    (sp)+,sr
          rts       

;Deselektiert alle angeschlossenen Laufwerke
deselect: move.w    sr,-(sp)
          ori.w     #$0700,sr
          move.b    #14,$ffff8800
          move.b    $ffff8802,d1
          andi.w    #$00f8,d1
          ori.w     #7,d1     ;Floppy deselektieren
          move.b    d1,$ffff8802
          move.w    (sp)+,sr
          rts       

;User soll eine bestimmte Diskette einlegen.
;void insert_disk(int nr);
;-> 8(A6): Diskettennummer [0..n] -2=Medusa.prg, -3...-99 = Datendiskette
mcode69:  link      a6,#0
          movem.l   d0-d7/a0-a5,-(sp)

          move.w    8(a6),d5  ;gewünschte Disk merken
          cmpi.w    #-2,d5    ;Medusa.prg?
          bne.s     kein_medusa
          clr.w     8(a6)     ;Dann auf Disk #0 suchen
          move.w    #-100,disk_drin     ;Auf jeden Fall suchen

kein_medusa:        move.w    8(a6),d0  ;gewünschte Disk
          cmp.w     disk_drin,d0        ;Ist die etwa noch drin?
          beq       nichts

insert_again:       move.w    #511,-(sp)          ;End_Offset
          move.w    #1,-(sp)  ;End_Sektor
          clr.w     -(sp)     ;End_Track
          clr.w     -(sp)     ;Start_Offset
          move.w    #1,-(sp)  ;Start_Sektor
          clr.w     -(sp)     ;Start_Track
          clr.w     -(sp)     ;A:
          pea       bitfeld+512
          bsr       mcode68   ;Bootsektor lesen
          lea.l     18(sp),sp
          lea.l     bitfeld+512,a0
          move.b    $19(a0),d0
          lsl.w     #8,d0
          move.b    $18(a0),d0
          move.w    d0,sek_per_track    ;Sektoren pro Track
          cmpi.w    #9,d0     ;9 Sektoren pro Track?
          beq.s     disk_ok
          cmpi.w    #10,d0
          beq.s     disk_ok

          movem.l   d0-d7/a0-a6,-(sp)
          lea       standard_disk,a0
          jsr       alert
          movem.l   (sp)+,d0-d7/a0-a6
          bra       insert_again        ;und nochmal einlegen

disk_ok:  move.b    $12(a0),d0
          lsl.w     #8,d0
          move.b    $11(a0),d0          ;Verzeichnisgröße
          ext.l     d0
          divu.w    #16,d0
          move.w    d0,d1     ;Anzahl an Sektoren fürs Directory
          swap.w    d0
          tst.w     d0
          beq.s     geht_auf
          addq.w    #1,d1     ;+1 für den Rest
geht_auf: move.b    $17(a0),d0
          lsl.w     #8,d0
          move.b    $16(a0),d0          ;Größe der FAT
          add.w     d0,d0     ;da 2 FATs
          addq.w    #1,d0     ;für Bootsektor
          move.w    d0,d2
          add.w     d1,d2     ;+Größe des Directory=erster Datensektor
          move.w    d2,first_sek
          ext.l     d0
          divu.w    sek_per_track,d0    ;10 Sektoren/Track

          move.w    #511,-(sp)          ;End_Offset
          swap.w    d0
          addq.w    #1,d0     ;Sektoren starten bei Eins
          move.w    d0,-(sp)  ;End_Sektor
          swap.w    d0
          move.w    d0,-(sp)  ;End_Track
          clr.w     -(sp)     ;Start_Offset
          swap.w    d0
          move.w    d0,-(sp)  ;Start_Sektor
          swap.w    d0
          move.w    d0,-(sp)  ;Start_Track
          clr.w     -(sp)     ;A:
          pea       bitfeld+1024+512
          bsr       mcode68   ;Directory lesen
          lea.l     18(sp),sp

          move.w    8(a6),d0
          bpl.s     archiv_disk
          moveq     #0,d0
          bra.s     donne
archiv_disk:        addi.w    #'0',d0
donne:    move.b    d0,rom+8  ;Nummer des Archivs eintragen

          lea.l     bitfeld+1024+512,a0
          lea.l     rom,a1
          cmpi.w    #-2,d5    ;Medusa.prg?
          bne.s     kein_med
          lea       medusa,a1
kein_med: moveq     #15,d6    ;16 Einträge pro Directory
nxt2:     moveq.l   #0,d7
kein_ende:          move.b    0(a0,d7.w),d0       ;Zeichen von Diskette lesen
          cmp.b     0(a1,d7.w),d0       ;ROM_GRFX.x?
          bne.s     nxt_entry
          addq.w    #1,d7
          tst.b     0(a1,d7.w)
          bne.s     kein_ende
          move.b    27(a0),d0
          lsl.w     #8,d0
          move.b    26(a0),d0 ;Startcluster der Datei holen
          subq.w    #2,d0
          add.w     d0,d0     ;ein Cluster hat 2 Sektoren
          add.w     d0,first_sek        ;1. Sektor der Daten

          move.b    31(a0),d0
          lsl.w     #8,d0
          move.b    30(a0),d0
          lsl.l     #8,d0
          move.b    29(a0),d0
          lsl.l     #8,d0
          move.b    28(a0),d0
          move.l    d0,datei_laenge     ;Länge der Datei merken

          tst.w     8(a6)     ;Sollte Datendisk eingelegt werden?
          bmi.s     falsche_disk

nichts:   move.w    8(a6),disk_drin

          movem.l   (sp)+,d0-d7/a0-a5
          unlk      a6
          rts       

nxt_entry:          lea.l     32(a0),a0 ;Einen Eintrag weiter
          dbra      d6,nxt2

          tst.w     8(a6)     ;Sollte Datendisk eingelegt werden?
          bmi.s     nichts    ;Ja, also okay

falsche_disk:       bsr       deselect  ;Alle Lichter aus
          move.w    8(a6),d0  ;Archivnummer
          bmi.s     datendiskette
          addi.w    #'A',d0
          move.b    d0,insert_disk+30

          movem.l   d0-d7/a0-a6,-(sp)
          movea.l   rasters,a0          ;(16) vorher (2): dargestellte VBL-Palette
          addq.l    #2,a0     ;(16) Offset überspringen
          moveq     #15,d7    ;(2) ab hier
check_vbl:          tst.w     (a0)+     ;Alle VBL-Farben auf Schwarz?
          bne.s     anzeigen  ;Nein->Okay
          dbra      d7,check_vbl

          moveq     #anz_raster-1,d7
          movea.l   rasters,a0          ;(16)
          movem.l   alert_pal,d0-d6/a1  ;(16)
set_pal:  movem.l   d0-d6/a1,2(a0)      ;(16) Und als Palette setzen
          lea       34(a0),a0 ;(16)
          dbra      d7,set_pal          ;(16)

anzeigen: lea.l     insert_disk,a0      ;(2) bis hier....
          jsr       alert     ;und Alert ausgeben
          movem.l   (sp)+,d0-d7/a0-a6

          bra       insert_again

datendiskette:      lea.l     insert_game,a0
          movem.l   d0-d7/a0-a6,-(sp)
          jsr       alert
          movem.l   (sp)+,d0-d7/a0-a6
          bra       insert_again

;Palette of MAPA.IFF: Saved by NEOchrome V2.24 by Chaos, Inc. (2)
alert_pal:          .DC.w $0000,$0300,$0520,$0642,$0753,$0030,$0242,$0bdb
          .DC.w $0464,$0057,$0077,$0333,$0444,$0555,$0666,$0fff

rom:      .DC.b 'ROM_GRFXx',0
medusa:   .DC.b 'MEDUSA  PRG',0

          .EVEN 


;int floppy_write(void *adr,int strack,int ssektor,int soffset,
;                          int etrack,int esektor,int eoffset);
;-> 24(a6): eoffset
;   22(a6): esektor
;   20(a6): etrack
;   18(a6): soffset
;   16(a6): ssektor
;   14(a6): strack
;   12(a6): Laufwerk (Bit 0=Laufwerk, Bit 1=Seite)
;    8(a6): adr
;<- D0.W: 0=Okay, -1=Fehler
mcode70:  link      a6,#0
          movem.l   d1-d7/a0-a7,dregister

          move.w    12(a6),d0 ;Laufwerk holen
          bsr       select    ;selektieren

          lea       $ffff8604,a3        ;FDC
          lea       2(a3),a4  ;DMA
          lea       $fffffa01,a5        ;MFP
          movea.l   8(a6),a2  ;Speicheradresse

          move.w    14(a6),d6 ;Starttrack
          bsr       seek_track

          moveq     #1,d0     ;1. Sektor lesen
          lea       bitfeld,a0
          bsr       read_sektoren

          move.w    14(a6),d4 ;Starttrack
          move.w    20(a6),d5 ;Endtrack
          cmp.w     d4,d5     ;nur auf einem Track lesen?
          beq       nur_ein_track2

mehr_als_einer2:    
          move.w    18(a6),d0 ;Startoffset
          lea       bitfeld,a0
          adda.w    d0,a0     ;Bytes überspringen
          move.w    #512,d1   ;Soviel Bytes kommen
          sub.w     d0,d1     ;Offset abziehen=Anzahl an Bytes
          bra.s     trans33
trans44:  move.b    (a2)+,(a0)+         ;In Buffer eintragen
trans33:  dbra      d1,trans44

          moveq     #1,d0
          lea       bitfeld,a0          ;hier steht der neue Sektor
          bsr       write_sektoren      ;und Sektor wieder zurückschreiben

          addq.w    #1,16(a6) ;nächsten Sektor geht's weiter
          move.w    sek_per_track,d0
          cmp.w     16(a6),d0 ;Startsektor 10?
          bge.s     no_step2  ;Nein, also kein Step

vorne_anfangen2:    move.w    #1,16(a6) ;Wieder bei Sektor #1 starten
          addq.w    #1,14(a6) ;Auf nächsten Track gehen
          move.w    14(a6),d6
          bsr       seek_track          ;Track anfahren

no_step2: move.w    14(a6),d4 ;Starttrack
          cmp.w     20(a6),d4 ;Endtrack erreicht?
          beq.s     letzter_track2

          move.w    sek_per_track,d0
          addq.w    #1,d0
          sub.w     16(a6),d0 ;Anzahl der zu schreibenden Sektoren
          movea.l   a2,a0     ;Ab Speicheradresse speichern
          bsr       write_sektoren
          adda.w    d5,a2     ;Bytes überspringen
          bra.s     vorne_anfangen2

letzter_track2:     move.w    22(a6),d0 ;Endsektor
          sub.w     16(a6),d0 ;-Startsektor
          beq.s     nur_ein_sektor2
          move.w    d0,-(sp)  ;Anzahl der Sektoren retten
          movea.l   a2,a0     ;Ladeadresse
          bsr.s     write_sektoren
          adda.w    d5,a2     ;Überspringen
          move.w    (sp)+,d0  ;Anzahl der gelesenen Sektoren
          add.w     d0,16(a6) ;=Nächster Sektor

nur_ein_sektor2:    moveq     #1,d0
          lea       bitfeld,a0
          bsr       read_sektoren       ;Letzten Sektor lesen
          lea       bitfeld,a0
          move.w    24(a6),d7 ;Endoffset
trans6:   move.b    (a2)+,(a0)+         ;Rest übertragen
          dbra      d7,trans6

          moveq     #1,d0
          lea       bitfeld,a0
          bsr       write_sektoren
          bra.s     aus_is2

nur_ein_track2:     move.w    22(a6),d0 ;Endsektor
          sub.w     16(a6),d0 ;-Startsektor
          bne       mehr_als_einer2

          lea       bitfeld,a0
          adda.w    18(a6),a0 ;+Startoffset
          move.w    24(a6),d7 ;Endoffset
          sub.w     18(a6),d7 ;-Startoffset
trans5:   move.b    (a2)+,(a0)+
          dbra      d7,trans5

          moveq     #1,d0
          lea       bitfeld,a0
          bsr       write_sektoren

aus_is2:  moveq     #0,d0     ;Okay

aus_is3:  move.b    #-1,flop_aktiv      ;Transfer beendet
          movem.l   dregister,d1-d7/a0-a7
          unlk      a6
          rts       


;Schreibt ab Startsektor Sektoren. Gibt in D5 512*n zurück.
;-> D0: Anzahl der Sektoren
;   A0: Ladeadresse
write_sektoren:     movem.l   d0/a0,-(sp)
          move.w    d0,d5
          mulu.w    #512,d5   ;Soviel Bytes werden geschrieben

          move.w    16(a6),d6 ;Startsektor
schreib_loop:       
          movem.l   d0-d7/a0-a6,-(sp)
          bsr       write_sektor
          movem.l   (sp)+,d0-d7/a0-a6
          addq.w    #1,d6     ;nächster Setor
          lea       512(a0),a0
          subq.w    #1,d0
          bgt.s     schreib_loop

          movem.l   (sp)+,d0/a0
          rts       

write_error:        subq.w    #1,retry
          bpl.s     write_again

          lea       write_err,a0
          jsr       alert

          moveq     #-1,d0    ;Fehler!
          bra       aus_is3   ;Abbruch

write_prt:          movem.l   d0-d7/a0-a6,-(sp)

          bsr       write_protect

          movem.l   (sp)+,d0-d7/a0-a6


;Schreibt einen Sektor
;-> D6: Sektornummer
;   A0: Speicheradresse
write_sektor:       move.w    #3,retry
write_again:        bsr       setdma
          clr.b     dflag     ;Kein DMA-Transfer
          bsr       wrtoggle
          moveq     #1,d7     ;eine Sektor
          bsr       fdcout
          move.w    #$0184,(a4)         ;Sektorregister
          move.w    d6,d7     ;Startsektor holen
          bsr       fdcout
          move.w    #$0180,(a4)         ;Commandoregister
          move.w    #$00a0,d7 ;Write Sektor
          bsr       fdcout
          bsr       waitfertig
          tst.w     d0        ;Alles Okay?
          bne.s     write_error
          move.w    disk_status,d0
          btst      #6,d0     ;Write Protect?
          bne.s     write_prt
          rts       

wrtoggle: move.w    #$0190,(a4)
          move.w    #$0090,(a4)
          move.w    #$0190,(a4)
          rts       


;Diese Routine wird nach dem Laden von Objektbibliotheken angesprungen
;und soll diese in ein gerätespezifisches Format umwandeln.
mcode71:  rts       

;***************************************************************************


;Diese Funktion spielt einen Digisound ab, dabei werden sämtliche benötigte
;IRQ's installiert, und wenn Digi fertig ist, werden die Routinen wieder
;destalliert. Wird als Startadresse 0L übergeben wird evtl. vorhandene
;Abspielroutine destalliert (abschalten).
;C-Definition: play_digi(startadr,roundabout,startzeile,endzeile)
;long startadr;            /* Startadresse */
;FLAG roundabout;    /* True bedeutet, wieder von vorne anfangen wenn fertig */
;int startzeile,endzeile;     /* Im Editor */
mcode36:: link      a6,#0
          movem.l   d0-d7/a0-a5,-(sp)

          bclr      #5,$fffffa07        ;IRQ-disablen

          move.b    #7,$ffff8800
          move.b    $ffff8800,d0        ;Control holen
          ori.w     #%111111,d0         ;Alle Kanäle aus
          move.b    d0,$ffff8802

          move.l    #$08080000,$ffff8800
          move.l    #$09090000,$ffff8800          ;Alle Lautstärken aus
          move.l    #$0a0a0000,$ffff8800

          clr.w     digi_works          ;Digisound ist aus

          tst.l     8(a6)     ;Start=0?
          beq       raus_digi ;Ja, nur abschalten

          clr.w     musik_an  ;Musik aus

          movea.l   8(a6),a0  ;Start der Digidaten
          cmpi.l    #'SAMP',(a0)+       ;Start einer Sequenz?
          bne.s     raus_digi ;Nein, also raus
          move.w    (a0)+,anzahl_seq    ;Anzahl der Sequenzen
          move.w    (a0)+,d7  ;Anzahl der Zeilen im Editor
          move.w    16(a6),d0 ;Endzeile
          sub.w     14(a6),d0 ;-startzeile=Anzahl Zeilen-1
          move.w    d0,anz_start        ;für Wiederholung
          addq.w    #1,d0     ;=Anzahl Zeilen
          move.w    d0,anzahl_zeilen
          move.l    a0,seq_tabelle      ;Beginn der Sequenztabelle
          move.w    anzahl_seq,d0
          mulu.w    #24,d0
          adda.l    d0,a0
          movea.l   a0,a1     ;Beginn der Editorzeilen
          move.w    14(a6),d0 ;startzeile
          lsl.w     #2,d0     ;x4
          adda.w    d0,a1
          move.l    a1,zeilen_tabelle   ;Beginn der Editorzeilen
          move.l    a1,naechste_zeile   ;Adresse der nächsten Editorzeile
          lsl.w     #2,d7
          adda.w    d7,a0
          move.l    a0,start_digi       ;Beginn der Digisounds

          move.w    12(a6),roundabout   ;Flag merken

          move.l    #play_digi,$00000134          ;Vektor für Timer A setzen

          move.w    #1,seq_counter      ;keine Wiederholung der aktuellen Seq
          bsr.s     nxt_zeile ;nächste Zeile für Digisound einstellen
          move.w    #1,digi_works       ;Digisound wird gerade gespielt
          bclr      #5,$fffffa0f        ;In-Service Bit löschen

raus_digi:          movem.l   (sp)+,d0-d7/a0-a5
          unlk      a6
          rts       

nxt_zeile:          movem.l   d0-d7/a0-a6,-(sp)

          subq.w    #1,seq_counter      ;Einmal abspielen ist fertig
          bne       nochmal_seq         ;Diese Sequenz nochmal spielen

          movea.l   naechste_zeile,a0
          subq.w    #1,anzahl_zeilen    ;Eine Zeile weniger
          bpl       nicht_von_vorne     ;weitermachen mit dieser Zeile
          tst.w     roundabout          ;wieder von vorne anfangen?
          beq       digi_zuende         ;nein, abstoppen
          movea.l   zeilen_tabelle,a0   ;von vorne anfangen
          move.w    anz_start,anzahl_zeilen       ;Startwert wieder eintragen

nicht_von_vorne:    moveq.l   #0,d5
          move.b    (a0)+,d5  ;Anzahl der Wiederholungen
          move.w    d5,seq_counter
          move.b    (a0)+,d5  ;nächste Sequenz
          movea.l   seq_tabelle,a1
          mulu.w    #24,d5    ;Werte der Sequenz
          adda.l    d5,a1
          move.l    (a1)+,d0  ;Start der Sequenz
          add.l     start_digi,d0
          move.l    d0,digi_pos         ;abspeichern
          move.l    d0,start_sequenz
          move.l    (a1)+,d0
          subq.l    #2,d0
          add.l     start_digi,d0       ;Ende der Sequenz
          move.l    d0,digi_end
          moveq.l   #0,d5
          move.b    (a0)+,d5  ;Flag, vorwärts, rückwärts

          move.b    (a0)+,d5  ;Frequenz
          move.l    a0,naechste_zeile

          move.l    #61440,d0 ;Vorteiler teilt durch 4
          mulu.w    #100,d5
          divu.w    d5,d0     ;Errechnet Counterwert

          bclr      #5,$fffffa07        ;Timer A IRQ verbieten
          move.b    #0,$fffffa19        ;Timer A Stop!
          move.b    d0,$fffffa1f        ;Counterwert eintragen
          bset      #5,$fffffa13        ;Timer A nicht maskieren
          move.b    #%11011111,$fffffa0b          ;Pending Bit löschen
          move.b    #1,$fffffa19        ;Vorteiler teilt durch 4
          bset      #5,$fffffa07        ;Timer A IRQ erlauben

nxt_ende: movem.l   (sp)+,d0-d7/a0-a6
          rts       

nochmal_seq:        move.l    start_sequenz,digi_pos        ;Nochmal von vorne
          bra.s     nxt_ende

digi_zuende:        bclr      #5,$fffffa07        ;Timer A verbieten
          clr.w     digi_works          ;Digisound ist aus
          st        musik_an  ;Musik wieder an
          bra.s     nxt_ende

play_digi:          move.w    #$2500,sr ;Rasterirq erlauben

          movem.l   d5/d6/a0,-(sp)      ;Register retten

          movea.l   digi_pos,a0
          moveq.l   #0,d5
          move.b    (a0)+,d5  ;Datenwert holen
          move.l    a0,digi_pos         ;nächster Datenwert
          cmpa.l    digi_end,a0         ;Ende erreicht?
          blt.s     digi_raus ;nein, weiter spielen

          bsr       nxt_zeile ;Ja, nächste Zeile?
          bra       nxt_ton

digi_raus:          .IF digisound=7
          add.w     d5,d5     ;7 Bit Digisound
          add.w     d5,d5
          move.w    d5,d6
          add.w     d5,d5
          add.w     d6,d5
          movem.l   smp_tabelle(pc,d5.w),d5/d6/a0
          movem.l   d5/d6/a0,$ffff8800
          .ENDC 

          .IF digisound=6
          andi.w    #$fc,d5   ;6 Bit Digisound
          move.w    d5,d6
          add.w     d5,d5
          add.w     d6,d5
          movem.l   smp_tabelle(pc,d5.w),d5/d6/a0
          movem.l   d5/d6/a0,$ffff8800
          .ENDC 

          .IF digisound=8
          lsl.w     #4,d5
          movem.l   smp_tabelle(pc,d5.w),d5/d6/a0
          movem.l   d5/d6/a0,$ffff8800
          .ENDC 

          .IF digisound=82
          add.w     d5,d5
          add.w     d5,d5
          move.w    d5,d6
          add.w     d5,d5
          add.w     d6,d5
          movem.l   smp_tabelle(pc,d5.w),d5/d6/a0
          movem.l   d5/d6/a0,$ffff8800
          .ENDC 

nxt_ton:  movem.l   (sp)+,d5/d6/a0
          bclr      #5,$fffffa0f        ;In-Service-Bit löschen
          rte       



roundabout          = $9c     ;Flag, ob am Ende des Sounds wieder von Vorne los geht
anzahl_seq          = $98     ;Anzahl der verschiedenen Sequenzen
anzahl_zeilen       = $94     ;Anzahl der Zeilen im Editor
anz_start           = $90     ;Anzahl der Zeilen im Editor für Wiederholung speichern
seq_tabelle         = $8c     ;Adresse des Beginns der Seq-Tabelle
zeilen_tabelle      = $80     ;Adresse der Editorzeilen
naechste_zeile      = $20     ;Adresse der nächsten Zeile im Editor
start_sequenz       = $1c     ;Adresse im Digisound der aktuellen Sequenz, (repeat)
start_digi          = $18     ;Adresse des Beginns der Digidaten
seq_counter         = $a0     ;Counter für Wiederholung der einzelnen Sequenzen
digi_pos  = $8      ;Adresse des nächsten Wertes, der gespielt wird
digi_end  = $c      ;Ende des aktuellen Digisounds

;Für den Sampler die Soundtabelle
smp_tabelle:        .IF digisound=6
          .IBYTES "6BIT.IMG"
          .ENDC 
          .IF digisound=7
          .IBYTES "7BIT.IMG"
          .ENDC 
          .IF digisound=8
          .IBYTES "8BIT.IMG"
          .ENDC 
          .IF digisound=82
          .IBYTES "8BIT2.IMG"
          .ENDC 

;Diese Routine konvertiert ein Farbbild in der Atari-320x200-Auflösung
;in hohe Atari 640x400 Auflösung unter Benutzung von Graustufen....
;C-Definition: void convert(screen,breite,höhe,palette)
mcode39:  link      a6,#0

          movem.l   d1-d7/a0-a5,-(sp)   ;Register retten

          move.w    12(a6),d0 ;Breite in Pixeln
          lsr.w     #4,d0     ;Breite in BBs errechnen
          subq.w    #1,d0     ;-1
          move.w    d0,12(a6) ;und wieder merken

          movea.l   16(a6),a0 ;Palette des Farbbildes
          lea.l     palette_mono,a1     ;Hier wird pro Farbe Muster gespeichert
          lea.l     convert_muster,a2   ;hier stehen die unterschiedlichen Muster
          moveq.l   #15,d7    ;16 Farbregister
pal_loop3:          move.w    (a0)+,d0  ;Farbwert holen
          move.w    d0,d1
          andi.w    #7,d1
          move.w    d0,d2
          lsr.w     #4,d2     ;RGB-Werte aufaddieren
          andi.w    #7,d2     ;=Helligkeit
          add.w     d2,d1
          lsr.w     #8,d0
          andi.w    #7,d0
          add.w     d0,d1
          muls.w    #3,d1     ;mal 3
          lsr.w     #2,d1     ;durch 4->0..15
          andi.w    #%1111,d1 ;Bits ausblenden
          moveq.l   #15,d0
          sub.w     d1,d0     ;von 15 abziehen
          lsl.w     #3,d0     ;mal 8
          move.l    0(a2,d0.w),(a1)+    ;und Muster holen
          move.l    4(a2,d0.w),(a1)+
          dbra      d7,pal_loop3        ;alle 15 Farben

          movea.l   8(a6),a0  ;zu bearbeitendes Bild
          movea.l   a0,a3     ;hier wird nachher wieder hinkopiert
          movea.l   a0,a4
          move.w    12(a6),d0 ;Breite in BBs
          addq.w    #1,d0     ;+1
          lsl.w     #2,d0
          move.w    d0,8(a6)  ;und merken in Screenadresse!
          adda.w    d0,a4

          lea.l     palette_mono,a2     ;und entsprechende Muster

          movea.l   #%11000000000000001100000000000000,a5   ;Zum Ausmaskieren

          move.w    14(a6),ycounter     ;Anzahl an Scanlines
nxt_y:    move.w    12(a6),xcounter     ;20 BBs
          lea.l     bitfeld,a1          ;hier zwischenspeichern
nxt_x:    movem.w   (a0)+,d0-d3         ;4 Planes holen
          move.l    a5,d7     ;Maske
          moveq.l   #7,d6     ;Bitcounter
          moveq.l   #0,d5     ;Hier Raster aufbauen
bit_loop2:          moveq.l   #0,d4     ;Hier Farbwert aufbauen
          addx.w    d3,d3
          addx.w    d4,d4
          addx.w    d2,d2
          addx.w    d4,d4     ;Farbwert des Pixels holen
          addx.w    d1,d1
          addx.w    d4,d4
          addx.w    d0,d0
          addx.w    d4,d4     ;Farbwert vom Bildschirm holen
          lsl.w     #3,d4     ;mal 8
          move.l    0(a2,d4.w),d4       ;Muster für diese Farbe
          and.l     d7,d4     ;Maskieren
          or.l      d4,d5     ;und einknüpfen
          lsr.l     #2,d7     ;und Maske weiterschieben
          dbra      d6,bit_loop2
          move.l    d5,(a1)+  ;Und Raster merken

          move.l    a5,d7     ;Maske wieder holen
          moveq.l   #7,d6     ;Bitcounter
          moveq.l   #0,d5     ;Hier Raster aufbauen
bit_loop1:          moveq.l   #0,d4     ;Hier Farbwert aufbauen
          addx.w    d3,d3
          addx.w    d4,d4
          addx.w    d2,d2
          addx.w    d4,d4
          addx.w    d1,d1     ;Getpixel
          addx.w    d4,d4
          addx.w    d0,d0
          addx.w    d4,d4     ;Farbwert vom Bildschirm holen
          lsl.w     #3,d4     ;mal 8
          move.l    0(a2,d4.w),d4       ;Muster für diese Farbe
          and.l     d7,d4     ;Ausmaskieren
          or.l      d4,d5     ;und einknüpfen
          lsr.l     #2,d7     ;und Maske runterschieben
          dbra      d6,bit_loop1
          move.l    d5,(a1)+  ;Muster merken

          subq.w    #1,xcounter         ;Alle BBs
          bpl.s     nxt_x

          move.w    12(a6),d7 ;Breite in BBs-1
          lea.l     bitfeld,a1
chg:      move.w    (a1)+,(a4)+         ;und in Reihenfolge umkopieren
          move.w    (a1)+,(a3)+         ;zurück auf Bildschirm
          move.w    (a1)+,(a4)+
          move.w    (a1)+,(a3)+
          dbra      d7,chg
          adda.w    8(a6),a3
          adda.w    8(a6),a4

          btst      #0,ycounter+1       ;Y-Zeile gerade?
          beq       gerade78  ;ja
          subq.l    #4,a2     ;Alte Musterwerte nehmen
          bra.s     weiter22
gerade78: addq.l    #4,a2     ;Neue Musterwerte benutzen
weiter22: subq.w    #1,ycounter         ;Alle Scanlines bearbeiten
          bne       nxt_y

          movem.l   (sp)+,d0-d7/a0-a5
          unlk      a6
          rts       

;Füllmuster für Low->Mono Wandeln
convert_muster:     .DC.w $0000,$0000,$0000,$0000
          .DC.w $8888,$0000,$8888,$0000
          .DC.w $8888,$0000,$2222,$0000
          .DC.w $8888,$0000,$aaaa,$0000
          .DC.w $aaaa,$0000,$aaaa,$0000
          .DC.w $aaaa,$4444,$aaaa,$4444
          .DC.w $aaaa,$4444,$aaaa,$1111
          .DC.w $aaaa,$4444,$aaaa,$5555
          .DC.w $aaaa,$5555,$aaaa,$5555
          .DC.w $5555,$bbbb,$5555,$eeee
          .DC.w $5555,$bbbb,$5555,$ffff
          .DC.w $5555,$ffff,$5555,$ffff
          .DC.w $7777,$ffff,$5555,$ffff
          .DC.w $7777,$ffff,$dddd,$ffff
          .DC.w $7777,$ffff,$ffff,$ffff
          .DC.w $ffff,$ffff,$ffff,$ffff

xcounter: .DS.w 1
ycounter: .DS.w 1
palette_mono:       .DS.w 64

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
          .DC.w $0255,$5452,$2250,$0001,$2166,$4727,$2200,$1046
          .DC.w $7263,$6773,$5215,$4562,$6263,$7555,$5572,$4400
          .DC.w $0008,$0000,$0000,$0000,$0250,$2122,$4122,$0001
          .DC.w $5311,$5441,$5522,$2721,$7554,$5444,$5215,$4755
          .DC.w $5554,$2555,$5515,$2844,$0814,$0000,$0000,$0000
          .DC.w $0205,$5250,$4177,$2702,$5122,$7662,$2300,$4012
          .DC.w $7764,$5665,$7216,$4555,$6562,$2555,$2222,$5428
          .DC.w $142a,$0000,$0000,$0000,$0005,$5470,$4122,$2004
          .DC.w $5141,$1152,$5122,$2720,$7554,$5445,$5255,$4555
          .DC.w $4551,$2557,$5240,$2810,$2214,$0000,$0000,$0000
          .DC.w $0202,$2150,$2250,$4024,$2176,$1622,$2604,$1042
          .DC.w $7563,$6743,$5225,$7552,$4356,$2725,$5270,$1000
          .DC.w $0022,$0000,$0000,$0000

;Routine überprüft den Kopierschutz auf Track 41 (wird nach hlpbuf) gelesen!
;C-Definition: protect()

protect:  movem.l   d1-d7/a0-a6,-(sp)

          bsr       crypt
          bsr       check_it
          bsr       crypt

          move.w    anzahl,d0 ;Flag zurückgeben
          movem.l   (sp)+,d1-d7/a0-a6   ;,-(a7)
          rts       

          .DC.b "TB"          ;Zeichen zum Finden von Crypt

crypt:    lea.l     kopier_anfang(pc),a0
          lea.l     kopier_ende(pc),a1
crypt_loop:         not.b     (a0)+
          cmpa.l    a1,a0     ;Ende erreicht?
          blt       crypt_loop
          rts       

mem:      .DC.l 0   ;Hier wird hlpbuf gespeichert

check_it: move.l    hlpbuf,mem          ;hier Track hinlesen

kopier_anfang:      movea.l   mem(pc),a5          ;hlpbuf
          movea.w   #$229,a0
          adda.w    #$215,a0  ;->$43e
          st        (a0)      ;flock setzen

          moveq.l   #0,d0     ; Dann Drive A:
          addq.w    #1,d0
          lsl.w     #1,d0
          eori.w    #%111,d0

          move.w    word_41202(pc),d1   ; $4120->D0
          lsl.w     #1,d1     ; $8240->D0
          movea.w   d1,a6     ; $FF8240->a6

          move.w    sr,-(sp)  ; sr sichern
          ori.w     #$500,sr  ; IRQs sperren
          ori.w     #$200,sr  ; = ORI #$700,sr
          move.b    #$e,$5c0(a6)        ; Port A
          move.b    $5c0(a6),d1         ; holen
          andi.b    #%11111000,d1       ; Bits löschen
          or.b      d0,d1     ; Floppy X:, Seite 0
          move.b    d1,$5c2(a6)         ; schreiben
          move.w    (sp)+,sr  ; STatus wieder holen
          moveq.l   #0,d4

          move.w    #$82,$3c6(a6)       ;Alten Trackwert holen
          bsr       wait_fdc
          lea.l     old_track(pc),a1
          move.w    $3c4(a6),(a1)       ;Speichern
          bsr       wait_fdc

kopierschutz2:      move.w    #$43,d0
          lsl.w     #1,d0     ; $86->d0
          move.w    d0,$3c6(a6)         ; DATA Register
ch_1:     move.w    #41,d7    ; Track 41 setzen
chcrypt   = ch_1+2
          bsr       fdc_out
          moveq.l   #$20,d0
          lsl.w     #2,d0     ; $80->D0
          move.w    d0,$3c6(a6)         ; Commando Register
          move.w    #%0000000000010111,d7         ; Seek mit Verify
          bsr       fdc_out
          bsr       wait_fertig
          btst      #4,d0     ; Track gefunden?
          bne       fehler    ; kann Track nicht anfahren

          move.w    #$79,d0
          addi.w    #9,d0     ; $82->d0
          move.w    d0,$3c6(a6)         ; Track-Register
          bsr       wait_fdc
          move.w    $3c4(a6),d0         ; holen
          cmpi.b    #41,d0    ; auf Track 41?
          bne       fehler

          move.l    a5,d0     ; ab hier Track lesen

          move.b    d0,$3cd(a6)         ; Low
          lsr.l     #8,d0
          move.b    d0,$3cb(a6)         ; Mid
          lsr.w     #8,d0
          move.b    d0,$3c9(a6)         ; High

          move.w    #$48,d0   ; Hälfte von $90
          lsl.w     #1,d0     ; mal 2->$90
          move.w    d0,$3c6(a6)
          bset      #16,d0    ; -> $190
          move.w    d0,$3c6(a6)
          bclr      #16,d0    ; -> $90
          move.w    d0,$3c6(a6)

          move.w    #$1f,d7   ; Sektor Count
          bsr       fdc_out   ; schreiben

          move.w    #$100,d0
          lsr.w     #1,d0     ; $80->D0
          move.w    d0,$3c6(a6)         ; Commando Register
          move.w    #$100,d7  ; Read Track
          subi.w    #$20,d7   ; $e0->D7
          bsr       fdc_out
          bsr       wait_fertig

          movea.l   a5,a0     ; ab hier steht Track
          lea.l     copyright(pc),a1    ; Copyright Original
          moveq.l   #0,d7     ; 0 Bytes geprüft
          moveq.l   #0,d1     ; von vorne prüfen
such_loop:          move.b    0(a0,d1.w),d0       ; Byte aus Track holen
          cmp.b     0(a1,d1.w),d0       ; gleich Copyright?
          bne       nicht_gleich        ; nein, weitersuchen
          addq.w    #1,d1
          tst.b     0(a1,d1.w)          ; Original Copyright zuende?
          bne       such_loop ; ja, Original
; Copyright gefunden

          bsr       disk_aus  ; Disk aus, zurück

          clr.w     d7        ;Flag löschen
          bra       test_ende

nicht_gleich:       moveq.l   #0,d1     ; vorne wieder anfangen
          addq.l    #1,a0     ; nächstes Byte
          addq.l    #1,d7     ; eins weiter
          cmpi.l    #97,d7    ; ganzen Trackanfang durchsuchen
          bne       such_loop
; Kopie:
fehler:   addq.w    #1,d4     ; noch ein Versuch
          cmpi.w    #4,d4     ; insgesamt 4
          bne       kopierschutz2       ; wieder anfangen
; Es wurde 10mal gelesen, auswerten

toete_dugger:       bsr       disk_aus
          move.w    #-1,d7    ;Flag für Fehler
          bra       test_ende

fdc_out:  bsr       wait_fdc
          move.w    d7,$3c4(a6)         ; Commando schreiben

wait_fdc: move.w    sr,-(sp)
          move.w    #$30,d0
wait_loop:          dbra      d0,wait_loop
          move.w    (sp)+,sr
          rts       

; wartet auf vollständige Ausführung
; veränderte Register: ?
wait_fertig:        move.l    #$5fffe,d7          ; Timeout Zähler
wait_again:         subq.l    #1,d7     ; vermindern
          beq       status    ; Null, ende
          btst      #5,$77c1(a6)        ; IRQ ausgelöst?
          bne       wait_again          ; nein, weiter warten

status:   move.w    #$30,d0
          addi.w    #$50,d0   ; $80->D0
          move.w    d0,$3c6(a6)         ; Status Register
          bsr       wait_fdc
          move.w    $3c4(a6),d0         ; Status holen
          rts       

disk_aus: move.w    #$86,$3c6(a6)       ;Data
          move.w    old_track(pc),d7
          bsr       fdc_out

          move.w    #$80,$3c6(a6)       ;Commando
          move.w    #$13,d7   ;Seek ohne Verify
          bsr       fdc_out
          bsr       wait_fertig

          move.w    #$70,d0
          addi.w    #$10,d0   ; $80->D0
          move.w    d0,$3c6(a6)         ; Status
          bsr       wait_fdc
wait_aus: move.w    $3c4(a6),d0         ; holen
          btst      #7,d0     ; Motor an?
          bne       wait_aus  ; ja, warten

          move.w    sr,-(sp)  ; sr sichern
          ori.w     #$700,sr  ; IRQs sperren
          move.b    #$e,$5c0(a6)        ; Port A
          move.b    $5c0(a6),d1         ; holen
          andi.b    #%11111000,d1       ; Bits löschen
          ori.b     #%111,d1  ; Floppy X:, Seite 0
          move.b    d1,$5c2(a6)         ; schreiben
          move.w    (sp)+,sr  ; STatus wieder holen

          movea.w   #$229,a0
          adda.w    #$215,a0  ;->$43e
          sf        (a0)      ;flock löschen
          rts       

word_41202:         .DC.w $4120
copyright:          .DC.b $0b,$df,$86,$73,$f7,$15,$10,$68,0
kopier_ende:                  ; Ende der Routine

          .EVEN 
test_ende:          move.w    d7,anzahl
          rts       

old_track:          .DC.l 0   ;Alten Track hier speichern

maus_daten:         .DC.w 7,7 ;Hotspot

          .DC.w %1111111111111111
          .DC.w %1111111111111111
          .DC.w %1111111111111111
          .DC.w %1111111011111111
          .DC.w %1111111011111111
          .DC.w %1111111011111111
          .DC.w %1111111111111111
          .DC.w %1110001110001111
          .DC.w %1111111111111111
          .DC.w %1111111011111111
          .DC.w %1111111011111111
          .DC.w %1111111011111111
          .DC.w %1111111111111111
          .DC.w %1111111111111111
          .DC.w %1111111111111111
          .DC.w %1111111111111111

          .DC.w 7,7 ; Offsets zum Hot Spot */

          .DC.w %1111111111111111
          .DC.w %1111111111111111
          .DC.w %1111111011111111
          .DC.w %1111110001111111
          .DC.w %1111111011111111
          .DC.w %1111111011111111
          .DC.w %1110111111101111
          .DC.w %1100001110000111
          .DC.w %1110111111101111
          .DC.w %1111111011111111
          .DC.w %1111111011111111
          .DC.w %1111110001111111
          .DC.w %1111111011111111
          .DC.w %1111111111111111
          .DC.w %1111111111111111
          .DC.w %1111111111111111

game_music:         .IBYTES "ROM_II_G.IMG"
          .EVEN 

;Bytefolge zur Initialisierung der absoluten Maus:
abs_maus_an:        .DC.b 9,1,63,0,199
mouse_key:          
          .DC.b 7,3,$10
abs_ms:   
set_ms:   .DC.b $0e,$0,0,160,0,155
abs_maus_end:       

write_err:          .DC.b "WRITE ERROR ON DISK!",0

abs_maus: .DC.b 0   ;0=Relative Maus, 1=Absolute Maus
maus_abfrag:        .DC.b -1  ;Maus soll nicht abgefragt werden
track_display:      .DC.b 0   ;FALSE
first_music:        .DC.b 0   ;TRUE

          .EVEN 
keypress: .DC.l 0   ;Momentan gedrückte Taste
tastatur_read:      .DC.l tastatur_buffer         ;Lesepointer im Tastaturbuffer
tastatur_buffer:    .DS.b 50
old_118:  .DS.l 1
old_timer_b:        .DS.l 1
overrun:  .DC.b 1   ;Bisher kein Overrun
          .EVEN 
rasters:  .DC.l rasters1
first_pal:          .DC.w 0   ;Für Calc-Offsets
line_modus:         .DC.w 0   ;Typ der Linie 0=Replace, 1=EOR, 2=Transparent

ende_auswert:       .DS.l 1   ;Tastaturroutine

digi_works:         .DS.w 1   ;Flag, ob Digisound läuft
anzahl:   .DS.w 1

logbase:  .DS.l 1   ;log. Bildschirmadresse

fuellmuster:        .DS.l 1   ;Adresse des Füllmusters

copyscroller:       .DS.w 1
scrollbase:         .DS.l 1
scrollbegin:        .DS.l 1   ; Für Laufschrift
yscroller:          .DS.w 1
delta_x:  .DS.w 1
scrollpos:          .DS.l 1
scroll_akt:         .DS.w 1
scroll_buff:        .DS.l 1
font_start:         .DS.l 1
char_basis:         .DS.l 1

counter:  .DS.w 1   ;Für Scrollroutine
adrspalte:          .DS.l 1   ;Pointer auf reinzuscrollendes Bild
oldbase:  .DS.l 1
neuerscreen:        .DS.l 1
timercvec:          .DS.l 1
basepage: .DS.l 1
helpram:  .DS.l 16
savereg:  .DS.l 4   ; Schneller Zwischenspeicher
quicksave:          .DS.l 6
steuerzeichen:      .DS.w 1   ;Zum Steuern der Bilder des Scrollers (1)

vbl_ct:   .DC.l 50000         ;(11) Schon etwas vor setzen

mouseform:          .DS.w 1   ;Form der Mouse
mdeltax:  .DS.w 1   ;(10)
mdeltay:  .DS.w 1   ;(10)

space:    .DS.l 3

endsource:          .DS.l 1
pic_length:         .DS.l 1   ; Bildlänge in Bytes

tos_da:   .DC.w 0   ;Flag, ob TOS aktiv oder nicht
disk_drin:          .DC.w -10 ;Im Moment ist keine Diskette im Laufwerk

          .BSS 

dregister:          .DS.l 15  ;zum Retten der Register
retry:    .DS.w 1   ;Retrycounter bei Schreibfehler
disk_status:        .DS.w 1   ;FDC-Status nach wait_fertig
sek_per_track:      .DS.w 1   ;Wieviel Sektoren pro Track hat diese Disk?
first_sek:          .DS.w 1   ;Der erste Sektor des Archivs (logisch)
datei_laenge:       .DS.l 1   ;Die Länge des Archivs in Bytes
dend:     .DS.l 1   ;DMA-Endadresse
dflag:    .DS.b 1   ;DMA-Transfer?
flop_aktiv:         .DS.b 1   ;Flag, ob Floppy aktiv ist
          .EVEN 

errno:    .DS.w 1   ;Globale Fehlermeldung für TCFLT.LIB
mem_strt: .DS.l 1   ;Startadresse des Benutzerspeichers
mem_len:  .DS.l 1   ;Größe des Benutzerspeichers

_BasPag:  .DS.l 1
_PgmSize: .DS.l 1
overlay:  .DS.w 1   ;Wird mit Overlay-Technik gearbeitet?

vbl_queue:          .DS.l 10  ;Platz für 10 VBL-Einträge

bitfeld:  .DS.b 4000

hbl_system:         .DS.w anz_raster*(1+16)       ;Y-Position+Paletten
rasters1: .DS.w anz_raster*(1+16)       ;dargestellte Rasterpalette
rasters2: .DS.w anz_raster*(1+16)       ;Aufbaurastertabelle

          .END 

