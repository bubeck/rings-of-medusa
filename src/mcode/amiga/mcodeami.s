
;Maschinensprachemodul fÅr Rings of Medusa II fÅr Commodore Amiga
;entwickelt unter TurboAss auf einem Atari ST
;(C) 1991 by Till Bubeck, Ziegeleistr. 28, D-7056 Weinstadt
;Tel: 07151-66437 (Wochenende und Semesterferien)
;bzw: 07071-49612 (innerhalb der Woche wÑhrend des Semesters)

;SÑmtliche énderungen sind in den Kommentaren durchnummeriert (1)-(x)
;folgenden énderungen wurden durchgefÅhrt:
;(1) Scroller liefert, sofern er ein unbekanntes Zeichen im Scrolltext
;    findet dieses in steuerzeichen ans Hauptprogramm (zur Synchronisation
;    mit Bildern). (14.5.1991)
;(2) Bei der Alertmeldung zum Einlegen von Diskette 'x' kam es vor,
;    daû noch kein Raster auf dem Bildschirm stand, und alle VBL-Farben
;    schwarz waren->man sah die Alertbox nicht.
;    Jetzt wird in diesem Fall die VBL-Palette mit einer entsprechenden
;    Palette geladen. (15.5.1991)
;(3) Scroller um 2 Zeichen (AnfÅhrunszeichen an/aus) erweitert
;(4) Neue Funktion bei draw_objekt (nur Palette setzen) (28.5.1991)
;(5) Neue Funktion del_raster() bei set_raster() (28.5.1991)
;(6) Neue Funktion fade_raster() (31.5.1991)
;(7) set_mousepos(x,y)=mcode73. Setzt Maus auf bestimmte Position (20.6.1991)
;(8) mcode31() ersatzlos entfernt (25.6.1991)
;(9) an/abschaltbares Trackdisplay, nur zum Debuggen (6.7.1991)
;(10) rel. Mauskoordinaten in mdeltax/y (9.7.1991)
;(11) vbl_ct wird mit Anfangswert Åber Null initialisiert (9.7.1991)
;(12) mcode64(), mcode65() speichern jetzt 9 Tracks (9.7.1991)
;(13) mcode74(), mcode75() sind hinzugekommen (28.7.1991)

          .PATH "E:\MEDUSA\MEDUSA_2.COL\MCODE\AMIGA\"

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
          .XDEF mcode38       ;Hippel_SFX
          .XDEF mcode39       ;convert
          .XDEF mcode47       ;draw_obj
          .XDEF mcode48       ;draw_obj_part
          .XDEF mcode49       ;raster_replace
          .XDEF mcode50       ;raster_transp(arent)
          .XDEF mcode51       ;mirror
          .XDEF mcode52       ;ICE! unpack
          .XDEF mcode53       ;line
          .XDEF mcode54       ;Fill
          .XDEF mcode55       ;Calc_Offsets (fÅr Raster)
          .XDEF mcode56       ;Neue_Raster
          .XDEF mcode57       ;Get_Key
          .XDEF mcode58       ;Line_mode
          .XDEF mcode59       ;get_pixel
          .XDEF mcode60       ;line_pattern
          .XDEF mcode61       ;loaddisk
          .XDEF mcode62       ;Vsync
          .XDEF mcode63       ;play_music
          .XDEF mcode64       ;Save_Data
          .XDEF mcode65       ;Load_Data
          .XDEF mcode66       ;random
          .XDEF mcode67       ;is_key
          .XDEF mcode68       ;floppy_read
          .XDEF mcode69       ;Insert_Disk
          .XDEF mcode70       ;floppy_write
          .XDEF mcode71       ;convert_objekt
          .XDEF mcode72       ;fade-Raster
          .XDEF mcode73       ;Set-Mouse
          .XDEF mcode74       ;Mouse_Off
          .XDEF mcode75       ;Mouse_On

          .XDEF exit

;Variablenexporte:
          .XDEF amiga_raw     ;Byte: Tastaturrawcode Amiga
          .XDEF protect       ;Track 41 Kopierschutz
          .XDEF game_music    ;Beginn der Musik
          .XDEF chcrypt       ;Hier kann Medusa ÅberprÅfen ob gecrypt

          .XDEF vbl_ct
          .XDEF keypress      ;Zur Zeit gedrÅckte Taste (Scancode+ASCII)
          .XDEF shift         ;Status der Shift-Tasten
          .XDEF fnt_buf       ;Zeiger auf Zeichensatz
          .XDEF digi_works    ;Flag, ob Digisound gerade gespielt wird
          .XDEF zeit_che      ;Flag, ob Zeit-Cheat angeschaltet
          .XDEF logbase       ;log. Bildschirmbasis
          .XDEF hbl_system    ;Das Rastersystem
          .XDEF mem_len       ;LÑnge des User Speichers
          .XDEF mem_strt      ;Startadresse des Usermemory
          .XDEF fast_mem      ;Startadresse des Fast-Mem
          .XDEF fast_len      ;LÑnge des Fast-Mem
          .XDEF errno         ;Globale Fehlermeldung (fÅr Bibliothek)
          .XDEF tos_da        ;Flag, ob TOS aktiv ist
          .XDEF musik_an      ;Flag, ob Musik spielen soll
          .XDEF mdeltax       ;(10)
          .XDEF track_display ;(9)
          .XDEF steuerzeichen

;Variablenimporte:
          .XREF scr1          ;Basisadresse des 1. Screens
          .XREF scr2          ;Basisadresse des 2. Screens
          .XREF save_1        ;Hier wird Maushintergrund fÅr 1. Screen
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
          .XREF _StkSize      ;Grîûe des Stacks
          .XREF insert_disk
          .XREF insert_game

;Funktionsimporte:
          .XREF alert         ;Stellt Alertbox dar
          .XREF _fpuinit      ;Init-Routine fÅr Coprozessor
          .XREF main          ;Hauptroutine C




;Offsets der Costumregister:

aud0lc    equ $000000a0       ;Adresse des Digisounds
aud0len   equ $000000a4       ;LÑnge in Worten
aud0per   equ $000000a6       ;Sample-Frequemz
aud0vol   equ $000000a8       ;LautstÑrke

ciaapra   equ $00bfe001       ;Portregister A
potgor    equ $16   ;FÅr rechts Maustaste

spr0pth   equ $00000120

bltapth   equ $00000050
bltbpth   equ $0000004c
bltcpth   equ $00000048
bltdpth   equ $00000054


HSIZEBITS           equ 6
VSIZEBITS           equ 16-HSIZEBITS
HSIZEMASK           equ $3f   ;2^6 -- 1
VSIZEMASK           equ $3ff  ;2^10 - 1

; all agnii support horizontal blit of at least 1024 bits (128 bytes) wide
; some agnii support horizontal blit of up to 32768 bits (4096 bytes) wide

          .IFD NO_BIG_BLITS
MAXBYTESPERROW      equ 128
          .ENDC 

          .IFND NO_BIG_BLITS
MINBYTESPERROW      equ 128
MAXBYTESPERROW      equ 4096
          .ENDC 

; definitions for blitter control register 0

ABC       equ $80
ABNC      equ $40
ANBC      equ $20
ANBNC     equ $10
NABC      equ $8
NABNC     equ $4
NANBC     equ $2
NANBNC    equ $1

BC0B_DEST           equ 8
BC0B_SRCC           equ 9
BC0B_SRCB           equ 10
BC0B_SRCA           equ 11
BC0F_DEST           equ $100
BC0F_SRCC           equ $200
BC0F_SRCB           equ $400
BC0F_SRCA           equ $800

BC1F_DESC           equ 2

DEST      equ $100
SRCC      equ $200
SRCB      equ $400
SRCA      equ $800

ASHIFTSHIFT         equ 12    ;bits to right align ashift value
BSHIFTSHIFT         equ 12    ;bits to right align bshift value

; definations for blitter control register 1
LINEMODE  equ $1
FILL_OR   equ $8
FILL_XOR  equ $10
FILL_CARRYIN        equ $4
ONEDOT    equ $2
OVFLAG    equ $20
SIGNFLAG  equ $40
BLITREVERSE         equ $2

SUD       equ $10
SUL       equ $8
AUL       equ $4

OCTANT8   equ 24
OCTANT7   equ 4
OCTANT6   equ 12
OCTANT5   equ 28
OCTANT4   equ 20
OCTANT3   equ 8
OCTANT2   equ 0
OCTANT1   equ 16

; custom chip register definitions

custom    equ $00dff000
bltddat   equ $00000000
dmaconr   equ $00000002
vposr     equ $00000004
vhposr    equ $00000006
dskdatr   equ $00000008
joy0dat   equ $0000000a
joy1dat   equ $0000000c
clxdat    equ $0000000e

adkconr   equ $00000010
pot0dat   equ $00000012
pot1dat   equ $00000014
potinp    equ $00000016
serdatr   equ $00000018
dskbytr   equ $0000001a
intenar   equ $0000001c
intreqr   equ $0000001e

dskpt     equ $00000020
dsklen    equ $00000024
dskdat    equ $00000026
refptr    equ $00000028
vposw     equ $0000002a
vhposw    equ $0000002c
copcon    equ $0000002e
serdat    equ $00000030
serper    equ $00000032
potgo     equ $00000034
joytest   equ $00000036
strequ    equ $00000038
strvbl    equ $0000003a
strhor    equ $0000003c
strlong   equ $0000003e

bltcon0   equ $00000040
bltcon1   equ $00000042
bltafwm   equ $00000044
bltalwm   equ $00000046
bltcpt    equ $00000048
bltbpt    equ $0000004c
bltapt    equ $00000050
bltdpt    equ $00000054
bltsize   equ $00000058
bltcon0l  equ $0000005b       ; note: byte access only
bltsizv   equ $0000005c
bltsizh   equ $0000005e

bltcmod   equ $00000060
bltbmod   equ $00000062
bltamod   equ $00000064
bltdmod   equ $00000066

bltcdat   equ $00000070
bltbdat   equ $00000072
bltadat   equ $00000074

deniseid  equ $0000007c
dsksync   equ $0000007e

cop1lc    equ $00000080
cop2lc    equ $00000084
copjmp1   equ $00000088
copjmp2   equ $0000008a
copins    equ $0000008c
diwstrt   equ $0000008e
diwstop   equ $00000090
ddfstrt   equ $00000092
ddfstop   equ $00000094
dmacon    equ $00000096
clxcon    equ $00000098
intena    equ $0000009a
intreq    equ $0000009c
adkcon    equ $0000009e

aud       equ $000000a0
aud0      equ $000000a0
aud1      equ $000000b0
aud2      equ $000000c0
aud3      equ $000000d0

; AudChannel
ac_ptr    equ $00000000       ; ptr to start of waveform data
ac_len    equ $00000004       ; length of waveform in words
ac_per    equ $00000006       ; sample period
ac_vol    equ $00000008       ; volume
ac_dat    equ $0000000a       ; sample pair
ac_SIZEOF           equ $10

bplpt     equ $000000e0

bplcon0   equ $100
bplcon1   equ $102
bplcon2   equ $104
bplcon3   equ $106
bpl1mod   equ $108
bpl2mod   equ $10a
bplhmod   equ $10c

bpldat    equ $110

sprpt     equ $120

spr       equ $140

; SpriteDef
sd_pos    equ $00000000
sd_ctl    equ $00000002
sd_dataa  equ $00000004
sd_dataB  equ $00000006
sd_SIZEOF           equ $00000008

color     equ $180

htotal    equ $1c0
hsstop    equ $1c2
hbstrt    equ $1c4
hbstop    equ $1c6
vtotal    equ $1c8
vsstop    equ $1ca
vbstrt    equ $1cc
vbstop    equ $1ce
sprhstrt  equ $1d0
sprhstop  equ $1d2
bplhstrt  equ $1d4
bplhstop  equ $1d6
hhposw    equ $1d8
hhposr    equ $1da
beamcom0  equ $1dc
hsstrt    equ $1de
vsstrt    equ $1e0
hcenter   equ $1e2
diwhigh   equ $1e4

anz_raster          equ 50    ;Soviele Raster sind gleichzeitig da...

Start0:   bra.s     amigastart
chcrypt:  
amigatool:          .DC.w -1  ;Wird vom Amigatool auf 0 gesetzt

amigastart:         move.w    #$2000,sr ;Alle IRQs ermîglichen
          move.w    #%0000000111101111,$dff096    ;Alle DMA aus, bis auf Disk-DMA

          lea.l     Start0,a3
          lea.l     -256(a3),a3         ;Adresse der Basepage

          movea.l   $18(a3),a0          ;Start des BSS
          move.l    $1c(a3),d7          ;Grîûe des BSS
          lsr.l     #4,d7     ;/8
          subq.l    #1,d7
          moveq     #0,d0
clear_bss:          move.l    d0,(a0)+
          move.l    d0,(a0)+
          move.l    d0,(a0)+  ;BSS lîschen
          move.l    d0,(a0)+
          dbra      d7,clear_bss

          move.l    a3,_BasPag          ;Pointer merken
          movea.l   $c(a3),a0 ;Grîûe Text
          adda.l    $14(a3),a0          ;+Grîûe DATA
          adda.l    $1c(a3),a0          ;+Grîûe BSS (including Stack)
          adda.w    #256,a0   ;+Grîûe Basepage

          move.l    a3,d0     ;Basepagestart
          add.l     a0,d0     ;+ProgrammlÑnge
          andi.b    #$fc,d0   ;gerade machen
          movea.l   d0,sp     ;Stack setzen
          move.l    d0,mem_strt         ;Hier beginnt der freie Speicher

          move.w    #512,d1
          mulu.w    #1024,d1  ;=Speicher in Bytes
          sub.l     d0,d1     ;=Freie Bytes
          move.l    d1,mem_len

          clr.w     tos_da    ;Ja, TOS ist nicht da!
          clr.w     _fpumode  ;Kein Floatingpoint da!

          tst.w     amigatool
          beq.s     kein_speicher2      ;ja->Kein Fast_Mem

          move.l    #hier_nicht,8
          move.l    8,12      ;Bus- und Adresserror setzen

          lea       $200000,a0          ;Ist hier Speicher?
          bsr       test_mem
          tst.w     d0
          beq.s     speicher_da

          lea       $80000,a0 ;1 Mega Chip-Mem?
          bsr       test_mem
          tst.w     d0
          beq.s     speicher_da

          lea       $c00000,a0
          bsr       test_mem
          tst.w     d0
          bne.s     kein_speicher2

speicher_da:        move.l    a0,fast_mem
          move.l    #512*1024,fast_len
          bra.s     speicher_ready

kein_speicher2:     clr.l     fast_mem
          clr.l     fast_len

speicher_ready:     movea.l   mem_strt,sp         ;Stackpointer setzen

to_main:  jsr       main

exit:     .DC.w $4afc         ;Illegal

meldung:  .DC.b "Please do NOT crack this program, because it's "
          .DC.b "written by The Delta Force/THE UNION. We're one "
          .DC.b "of the oldest groups on the ST (Maybe you'll know the "
          .DC.b "UNION DEMO?)..."
          .EVEN 

reset_fest:         .DC.w 0,0
ausgleich:          .DC.l 0
          .DC.l reset_routine

reset_routine:      addq.w    #1,d0
          move.w    d0,$dff180
          bra.s     reset_routine

;öberprÅft, ob an angegebener Stelle 512 Kbyte Speicher ist
;-> A0.L: Speicheradresse
test_mem: moveq     #0,d0
          move.l    #$55555555,d5
          move.l    #$aaaaaaaa,d6
          move.w    d0,0
          move.w    d5,(a0)
          cmp.w     (a0),d5
          bne.s     kein_speicher
          move.w    d6,(a0)
          cmp.w     (a0),d6
          bne.s     kein_speicher
          tst.l     0         ;Wurde nicht ins Chip-Ram gespiegelt?
          bne.s     kein_speicher       ;Doch->Kein Speicher

          movea.l   a0,a1
          move.w    #32767,d7 ;512 Kbyte testen
test_512: move.l    (a1),d4   ;Inhalt merken
          move.l    d5,(a1)
          cmp.l     (a1),d5
          bne.s     kein_speicher
          move.l    d6,(a1)
          cmp.l     (a1),d6
          bne.s     kein_speicher
          move.l    d4,(a1)+  ;Inhalt wieder schreiben
          dbra      d7,test_512

          moveq     #0,d0
          rts       
kein_speicher:      moveq     #-1,d0
          rts       

hier_nicht:         moveq     #-1,d0
          rte       

;Einsprung ins Amigatool
trap_1:   movem.l   d1-d7/a0-a6,-(sp)   ;Register retten
          trap      #1
          movem.l   (sp)+,d1-d7/a0-a6
          rts       

;Schreibt Zeichen in Low-Res auf den Bildschirm an Position X/Y, in der Farbe 15
;C-Definition: char_fast(x1,y1,zeichen)
;int x,y;
;char zeichen;
mcode1:   movem.l   d0-d7/a0-a6,-(sp)   ; Register retten

          andi.w    #$fffc,64(sp)       ;auf gerade 4'er X
          lea       fnt_buf,a2          ;Adresse des Zeichensatz

          movea.l   logbase,a0          ;Basis des Bildschirms
          move.w    66(sp),d0 ;Bildschirmzeile
          mulu.w    #160,d0   ;160 Bytes/Bildschirmzeile
          adda.l    d0,a0
          move.w    64(sp),d0
          move.w    d0,d1     ;x sichern
          lsr.w     #4,d0     ;/16
          lsl.w     #1,d0
          adda.w    d0,a0     ;Adresse der 4 Words
          andi.w    #$000c,d1 ;0/4/8/12
          cmpi.w    #4,d1
          beq       po4
          cmpi.w    #8,d1
          beq       po8
          cmpi.w    #12,d1
          beq       po12

oben:     moveq     #0,d0
          move.b    69(sp),d0 ;Zeichen holen
          bsr       wandel    ;in internen Zeichensatz
          moveq     #4,d6     ;5 Zeilen
          moveq     #15,d5    ;zum Aus-anden
zeil_loop:          move.w    d0,d1
          lsr.w     #1,d1
          move.b    0(a2,d1.w),d3
          btst      #0,d0     ;gerades Zeichen?
          beq       gerade    ;ja!
          lsl.b     #4,d3     ;nur die untersten 4 Bit
          bra       zeil_it
gerade:   andi.b    #$f0,d3   ;in die obersten 4 Bit
zeil_it:  moveq     #0,d7     ;Bei Plane 0 starten
plane_loop:         and.b     d5,(a0)   ;Hintergrund lîschen
          or.b      d3,(a0)   ;Zeichen einodern
          lea       40(a0),a0 ;eine Plane weiter
          and.b     d5,(a0)   ;Hintergrund lîschen
          or.b      d3,(a0)   ;Zeichen einodern
          lea       40(a0),a0 ;eine Plane weiter
          and.b     d5,(a0)   ;Hintergrund lîschen
          or.b      d3,(a0)   ;Zeichen einodern
          lea       40(a0),a0 ;Eine Plane weiter
          and.b     d5,(a0)   ;Hintergrund lîschen
          or.b      d3,(a0)   ;Zeichen einodern

          lea       40(a0),a0 ;Eine Zeile tiefer
          lea       40(a2),a2 ;Zeile tiefer
          dbra      d6,zeil_loop
          bra       ende

po8:      addq.l    #1,a0     ;8 Pixel weiter
          bra       oben

po4:      moveq     #0,d0
          move.b    69(sp),d0 ;Zeichen holen
          bsr       wandel    ;in internen Zeichensatz
          moveq     #4,d6     ;5 Zeilen
          move.l    #$000000f0,d5       ;zum Aus-Anden
zeil2:    move.w    d0,d1
          lsr.w     #1,d1
          move.b    0(a2,d1.w),d3
          btst      #0,d0     ;gerades Zeichen?
          beq       gerade2   ;ja!
          andi.b    #$0f,d3   ;nur die untersten 4 Bit
          bra       zeil_it2
gerade2:  lsr.b     #4,d3     ;in die obersten 4 Bit
zeil_it2: moveq     #0,d7     ;Bei Plane 0 starten
plane2:   and.b     d5,(a0)
          or.b      d3,(a0)   ;Zeichen einodern
          lea       40(a0),a0 ;eine Plane weiter
          and.b     d5,(a0)
          or.b      d3,(a0)   ;Zeichen einodern
          lea       40(a0),a0 ;eine Plane weiter
          and.b     d5,(a0)
          or.b      d3,(a0)   ;Zeichen einodern
          lea       40(a0),a0 ;eine Plane weiter
          and.b     d5,(a0)
          or.b      d3,(a0)   ;Zeichen einodern

          lea       40(a0),a0 ;Zeile tiefer
          lea       40(a2),a2 ;Zeile tiefer
          dbra      d6,zeil2
          bra       ende

po12:     addq.l    #1,a0     ;8 Pixel weiter
          bra       po4

wandel:   cmpi.w    #$009a,d0 ;'ö'
          beq       ueh
          cmpi.w    #$008e,d0 ;'é'
          beq       aeh
          cmpi.w    #$0099,d0 ;'ô'
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
          lea       fnt_buf,a2          ;Adresse des Zeichensatzes

          movea.l   logbase,a0          ;Basis des Bildschirms
          move.w    70(sp),d0 ;Bildschirmzeile
          mulu.w    #160,d0   ;160 Bytes/Bildschirmzeile
          adda.l    d0,a0
          move.w    68(sp),d0 ;x holen
          move.w    d0,d1     ;x sichern
          lsr.w     #4,d0     ;/16
          lsl.w     #1,d0
          adda.w    d0,a0     ;Adresse der 4 Words
          andi.w    #$000c,d1 ;0/4/8/12
          cmpi.w    #4,d1
          beq       po42
          cmpi.w    #8,d1
          beq       po82
          cmpi.w    #12,d1
          beq       po122

oben2:    moveq     #0,d0
          move.b    73(sp),d0 ;Zeichen holen
          bsr       wandel    ;in internen Zeichensatz
          moveq     #4,d6     ;5 Zeilen
          moveq     #15,d5    ;zum Aus-anden
          move.l    #$000000f0,d4       ;zum Einodern des Hintergrundes
zeil_loop3:         move.w    d0,d1
          lsr.w     #1,d1
          move.b    0(a2,d1.w),d3
          btst      #0,d0     ;gerades Zeichen?
          beq       gerade3   ;ja!
          lsl.b     #4,d3     ;nur die untersten 4 Bit
          bra       zeil_it3
gerade3:  andi.b    #$f0,d3   ;in die obersten 4 Bit
zeil_it3: moveq     #0,d7     ;Bei Plane 0 starten
plane_loop3:        btst      d7,67(sp) ;Plane im Hintergrund gesetzt?
          beq       loesch_back3
          or.b      d4,(a0)   ;Hintergrund setzen
          btst      d7,65(sp) ;Schriftfarbe an?
          bne       fertig3   ;auch an, nichts machen
          not.b     d3        ;invertieren
          and.b     d3,(a0)   ;Schrift wieder lîschen
          not.b     d3        ;zurÅck invertieren
          bra       fertig3
loesch_back3:       and.b     d5,(a0)   ;Hintergrund lîschen
          btst      d7,65(sp) ;Schriftplane an?
          beq       fertig3   ;auch nicht, fertig
          or.b      d3,(a0)   ;Zeichen einodern

fertig3:  lea       40(a0),a0 ;eine Plane weiter
          addq.w    #1,d7
          cmpi.w    #4,d7
          bne       plane_loop3

          adda.w    #40,a2    ;Zeile tiefer
          dbra      d6,zeil_loop3
          bra       ende2

po82:     addq.l    #1,a0     ;8 Pixel weiter
          bra       oben2

po42:     moveq     #0,d0
          move.b    73(sp),d0 ;Zeichen holen
          bsr       wandel    ;in internen Zeichensatz
          moveq     #4,d6     ;5 Zeilen
          move.l    #$000000f0,d5       ;zum Aus-Anden
          moveq     #15,d4    ;zum Setzen des Hintergrund
zeil4:    move.w    d0,d1
          lsr.w     #1,d1
          move.b    0(a2,d1.w),d3
          btst      #0,d0     ;gerades Zeichen?
          beq       gerade4   ;ja!
          andi.b    #$0f,d3   ;nur die untersten 4 Bit
          bra       zeil_it4
gerade4:  lsr.b     #4,d3     ;in die obersten 4 Bit
zeil_it4: moveq     #0,d7     ;Bei Plane 0 starten
plane4:   btst      d7,67(sp) ;Plane im Hintergrund gesetzt?
          beq       loesch_back4
          or.b      d4,(a0)   ;Hintergrund setzen
          btst      d7,65(sp) ;Schriftfarbe an?
          bne       fertig4   ;auch an, nichts machen
          not.b     d3        ;invertieren
          and.b     d3,(a0)   ;Schrift wieder lîschen
          not.b     d3        ;zurÅck invertieren
          bra       fertig4
loesch_back4:       and.b     d5,(a0)   ;Hintergrund lîschen
          btst      d7,65(sp) ;Schriftplane an?
          beq       fertig4   ;auch nicht, fertig
          or.b      d3,(a0)   ;Zeichen einodern

fertig4:  lea       40(a0),a0 ;eine Plane weiter
          addq.w    #1,d7
          cmpi.w    #4,d7
          bne       plane4

          lea       40(a2),a2 ;Zeile tiefer
          dbra      d6,zeil4
          bra       ende2

po122:    addq.l    #1,a0     ;8 Pixel weiter
          bra       po42

ende2:    movem.l   (sp)+,d0-d7/a0-a6
          rts       

;Initialisiert die AMIGA-Hardware etc.
;C-Definition: init_hardware()

mcode3:   movem.l   d0-d7/a0-a6,-(sp)

;Als nÑchstes werden die Exception-Vektoren gesetzt,
;damit ich weiû, was passiert ist....

          tst.w     amigatool
          beq.s     no_set

          move.l    #bus_error,$8
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
;Usermode ausgefÅhrt wurde. Da beim 68000'er der Befehl 'Move SR,<EA>' kein
;priviligierter Befehl ist, fÅhrt er auf diesem Prozessor zu keiner
;Privilegverletzung. Auf Atari TT, der ja einen 68030 enthÑlt, fÅhrt dies
;aber zu einer Privilegverletzung. Diese Routine fÅhrt den 'Move SR,<EA>' dann
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
          nop                 ;Move Sr,<Ea> wurde Åbersetzt in:
          nop                 ;Move.w status_reg,<EA>
          nop       
          move.l    pc_counter,-(sp)
          move.w    status_reg,-(sp)
          rte                 ;und fortsetzen

pc_counter:         .DC.l 0
status_reg:         .DC.w $0300
register_rett:      .DS.l 3   ;Platz fÅr 3 Register

privileg_error:     move.w    #4,error_nr

show_error:         move.w    #$2700,sr ;Keine Stîrung bitte

          movem.l   d0-d7/a0-a7,$204    ;Register retten
          move.l    #'RoM2',$200

          move.w    #$f,$dff180         ;Hintergrund rot

          move.l    scr1,d0
          move.l    d0,logbase
          movea.l   d0,a0

          move.w    #479,d7   ;die obersten 2 Zeilen lîschen
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

flacker:  bra.s     flacker

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
          lea       fnt_buf,a2          ;Adresse des Zeichensatzes

          movea.l   logbase,a0          ;Basis des Bildschirms
          move.w    12(a6),d0 ;Bildschirmzeile
          mulu.w    #160,d0   ;160 Bytes/Bildschirmzeile
          adda.l    d0,a0
          move.w    10(a6),d0 ;x holen
          move.w    d0,d1     ;x sichern
          lsr.w     #4,d0     ;/16
          lsl.w     #1,d0
          adda.w    d0,a0     ;Adresse der 4 Words
          andi.w    #$000c,d1 ;0/4/8/12
          cmpi.w    #4,d1
          beq       po43
          cmpi.w    #8,d1
          beq       po83
          cmpi.w    #12,d1
          beq       po123

oben3:    moveq     #0,d0
          move.b    15(a6),d0 ;Zeichen holen
          bsr       wandel    ;in internen Zeichensatz
          moveq     #4,d6     ;5 Zeilen
          moveq     #15,d5    ;zum Aus-anden
          move.l    #$000000f0,d4       ;zum Einodern des Hintergrundes
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
          moveq     #0,d7     ;Bei Plane 0 starten
plane_loop33:       btst      d7,9(a6)  ;Schriftfarbe an?
          beq       ausis1    ;auch nicht, fertig
          or.b      d3,(a0)   ;Zeichen einodern
          bra.s     fertig33
ausis1:   and.b     d1,(a0)   ;und weglîschen
fertig33: lea       40(a0),a0 ;eine Plane weiter
          addq.w    #1,d7
          cmpi.w    #4,d7
          bne       plane_loop33

          lea       40(a2),a2
          dbra      d6,zeil_loop33
          bra       ende23

po83:     addq.l    #1,a0     ;8 Pixel weiter
          bra       oben3

po43:     moveq     #0,d0
          move.b    15(a6),d0 ;Zeichen holen
          bsr       wandel    ;in internen Zeichensatz
          moveq     #4,d6     ;5 Zeilen
          move.l    #$000000f0,d5       ;zum Aus-Anden
          moveq     #15,d4    ;zum Setzen des Hintergrund
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
          moveq     #0,d7     ;Bei Plane 0 starten
plane43:  btst      d7,9(a6)  ;Schriftplane an?
          beq       ausis2    ;auch nicht, fertig
          or.b      d3,(a0)   ;Zeichen einodern
          bra.s     fertig43
ausis2:   and.b     d1,(a0)   ;und weglîschen
fertig43: lea       40(a0),a0 ;eine Plane weiter
          addq.w    #1,d7
          cmpi.w    #4,d7
          bne       plane43

          lea       40(a2),a2
          dbra      d6,zeil43
          bra       ende23

po123:    addq.l    #1,a0     ;8 Pixel weiter
          bra       po43

ende23:   movem.l   (sp)+,d0-d7/a0-a5
          unlk      a6
          rts       

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

;Scrollt ein Bild, welches in scr2 gespeichert ist von links in den
;Screen rein
mcode8:   movem.l   d0-d7/a0-a6,-(sp)

          move.w    #200,-(sp)
          move.l    hlpbuf,-(sp)
          move.l    scr2,-(sp)
          bsr       mcode15   ;scr2->hlpbuf kopieren
          lea       10(sp),sp

          move.w    #200,-(sp)
          move.l    scr2,-(sp)
          move.l    scr1,-(sp)
          bsr       mcode15   ;scr1->scr2 kopieren
          lea       10(sp),sp

          movea.l   hlpbuf,a0
          lea       17*160(a0),a0

          jsr       wait_blitter        ;Warten bis Blitter fertig ist

          lea       $dff000,a5          ;Custom-Chips

          move.w    #39,d6    ;40 mal schieben
schieb_links:       
          movea.l   scr1,a2
          lea       17*160+2(a2),a2     ;Quelle
          movea.l   scr2,a1   ;Arbeitsbildschirm
          lea       17*160(a1),a1       ;Ziel
          movea.l   a1,a6     ;merken

          lea       -1(a2),a3
          lea       bitfeld,a4          ;hier hin retten
          move.w    #150,d4   ;151 Zeilen retten
rette1:   move.b    (a3),(a4)+
          move.b    40(a3),(a4)+
          move.b    80(a3),(a4)+
          move.b    120(a3),(a4)+
          lea       160(a3),a3
          dbra      d4,rette1

          moveq     #3,d5     ;4 Planes
all2:     move.l    a2,bltapth(a5)      ;Quelle A
          move.l    a1,bltdpth(a5)      ;Ziel D
          move.w    #120,bltamod(a5)
          move.w    #120,bltdmod(a5)
          move.w    #-1,bltafwm(a5)
          move.w    #-1,bltalwm(a5)     ;Keine Maskierung
          move.w    #%1000100111110000,bltcon0(a5)          ;D=A
          clr.w     bltcon1(a5)         ;Ascending Mode

          move.w    #151*64+20,d0       ;und schieben
          jsr       start_blit

          lea       40(a1),a1 ;NÑchste Plane
          lea       40(a2),a2
          dbra      d5,all2

          lea       bitfeld,a4
          move.w    #150,d4   ;150 Scanlines hin
setz1:    move.b    (a4)+,(a6)
          move.b    (a4)+,40(a6)
          move.b    (a4)+,80(a6)        ;und linken Rand wieder hinsetzen
          move.b    (a4)+,120(a6)
          lea       160(a6),a6
          dbra      d4,setz1

          movea.l   scr2,a1
          adda.w    #17*160+39,a1
          move.w    #150,d4
setz2:    move.b    (a0),(a1)
          move.b    40(a0),40(a1)
          move.b    80(a0),80(a1)
          move.b    120(a0),120(a1)
          lea       160(a1),a1
          lea       160(a0),a0
          dbra      d4,setz2

          lea       -151*160+1(a0),a0

          jsr       switchscreens
          dbra      d6,schieb_links

          movem.l   (sp)+,d0-d7/a0-a6
          rts       


;Scrollt ein Bild, welches in hlpbuf gespeichert ist von rechts in den
;Screen rein
mcode9:   movem.l   d0-d7/a0-a6,-(sp)

          move.w    #200,-(sp)
          move.l    hlpbuf,-(sp)
          move.l    scr2,-(sp)
          bsr       mcode15   ;scr2->hlpbuf kopieren
          lea       10(sp),sp

          move.w    #200,-(sp)
          move.l    scr2,-(sp)
          move.l    scr1,-(sp)
          bsr       mcode15   ;scr1->scr2 kopieren
          lea       10(sp),sp

          movea.l   hlpbuf,a0
          lea       17*160+40-1(a0),a0  ;Ende der 1. Zeile

          jsr       wait_blitter        ;Warten bis Blitter fertig ist

          lea       $dff000,a5          ;Custom-Chips

          moveq     #39,d6    ;40 mal schieben
schieb_rechts:      
          movea.l   scr1,a2
          lea       17*160(a2),a2       ;Quelle
          movea.l   scr2,a1   ;Arbeitsbildschirm
          lea       17*160(a1),a1       ;Ziel

          moveq     #3,d5     ;4 Planes
all_planes1:        move.l    a2,bltapth(a5)      ;Quelle A
          move.l    a1,bltdpth(a5)      ;Ziel D
          move.w    #120,bltamod(a5)
          move.w    #120,bltdmod(a5)
          move.w    #-1,bltafwm(a5)
          move.w    #-1,bltalwm(a5)     ;Keine Maskierung
          move.w    #%1000100111110000,bltcon0(a5)          ;D=A>>8
          clr.w     bltcon1(a5)         ;Ascending Mode

          move.w    #151*64+20,d0       ;und schieben
          jsr       start_blit
          lea       40(a1),a1 ;nÑchste Plane
          lea       40(a2),a2
          dbra      d5,all_planes1

          movea.l   scr2,a1
          lea       17*160(a1),a1       ;linker Rand der Zielbildes

          move.w    #150,d5   ;151 Zeilen einfÅgen
alle_1:   move.b    (a0),(a1) ;Am linken Rand reinkommen
          move.b    40(a0),40(a1)
          move.b    80(a0),80(a1)       ;Alle 4 Planes
          move.b    120(a0),120(a1)
          lea       160(a0),a0
          lea       160(a1),a1
          dbra      d5,alle_1

          lea       -151*160-1(a0),a0   ;ZurÅck zum Start+1

          jsr       switchscreens
          dbra      d6,schieb_rechts

          movem.l   (sp)+,d0-d7/a0-a6
          rts       



;Scrollt ein Bild rein:
;-> 8(a6): Adresse des neuen Screens
mcode10:  link      a6,#0
          movem.l   d0-d7/a0-a5,-(sp)

          movea.l   8(a6),a0  ;Adresse des Quellbildes
          lea       17*160(a0),a0       ;Obere Leiste Åberspringen

          jsr       wait_blitter        ;Warten bis Blitter fertig ist

          lea       $dff000,a5          ;Custom-Chips

          move.w    #36,d6    ;37 mal schieben
schieb_hoch:        movea.l   scr2,a1   ;Arbeitsbildschirm
          lea       164*160-2(a1),a1    ;Ziel
          movea.l   scr1,a2
          lea       168*160-2(a2),a2    ;Quelle

          move.l    a2,bltapth(a5)      ;Quelle A
          move.l    a1,bltdpth(a5)      ;Ziel D
          clr.w     bltamod(a5)
          clr.w     bltdmod(a5)         ;Modulo=0
          move.w    #-1,bltafwm(a5)
          move.w    #-1,bltalwm(a5)     ;Keine Maskierung
          move.w    #%0000100111110000,bltcon0(a5)          ;D=A
          move.w    #%0000000000000010,bltcon1(a5)          ;Descending Mode

          move.w    #196*64+60,d0       ;und hoch schieben
          jsr       start_blit

          movea.l   scr2,a1
          lea       164*160(a1),a1      ;in die unterste Zeile reinkopieren
          moveq     #79,d7
copy_zeile3:        move.l    (a0)+,(a1)+
          move.l    (a0)+,(a1)+
          dbra      d7,copy_zeile3

          jsr       switchscreens
          dbra      d6,schieb_hoch

          movem.l   (sp)+,d0-d7/a0-a5
          unlk      a6
          rts       


;Scrollt Bild nach unten rein
mcode11:  link      a6,#0
          movem.l   d0-d7/a0-a5,-(sp)

          movea.l   8(a6),a0  ;Adresse des Quellbildes
          lea       168*160(a0),a0      ;Bild Åberspringen

          jsr       wait_blitter        ;Warten bis Blitter fertig ist

          lea       $dff000,a5          ;Custom-Chips

          move.w    #36,d6    ;37 mal schieben
schieb_runter:      movea.l   scr2,a1   ;Arbeitsbildschirm
          lea       21*160(a1),a1       ;Ziel
          movea.l   scr1,a2
          lea       17*160(a2),a2       ;Quelle

          move.l    a2,bltapth(a5)      ;Quelle A
          move.l    a1,bltdpth(a5)      ;Ziel D
          clr.w     bltamod(a5)
          clr.w     bltdmod(a5)         ;Modulo=0
          move.w    #-1,bltafwm(a5)
          move.w    #-1,bltalwm(a5)     ;Keine Maskierung
          move.w    #%0000100111110000,bltcon0(a5)          ;D=A
          clr.w     bltcon1(a5)         ;Ascending Mode

          move.w    #196*64+60,d0       ;und hoch schieben
          jsr       start_blit

          movea.l   scr2,a1
          lea       21*160(a1),a1       ;in die unterste Zeile reinkopieren
          moveq     #79,d7
copy_zeile4:        move.l    -(a0),-(a1)
          move.l    -(a0),-(a1)
          dbra      d7,copy_zeile4

          jsr       switchscreens
          dbra      d6,schieb_runter

          movem.l   (sp)+,d0-d7/a0-a5
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

;Zeigt Screen an
;C-Definition: showscreen(adr)
;long adr;          /* ADresse des neuen Screens */
mcode12:  link      a6,#0
          movem.l   d0-d7/a0-a6,-(sp)

          move.l    8(a6),d0  ;Adresse holen
          move.l    d0,logbase          ;auch als Logbase setzen

          movea.l   copperliste,a2      ;Aktive Copperliste
          moveq     #3,d7     ;4 Planes
          move.w    #$00e0,d1 ;Register der BB
          movea.l   d0,a0     ;Adresse des Screens
make_cl:  bsr       set_adr
          lea       40(a0),a0
          dbra      d7,make_cl

          tst.b     zeit_che  ;Zeit cheat an?
          beq       vsync24
          move.w    #$0fff,$00dff180

vsync24:  bsr       vsync

          movem.l   (sp)+,d0-d7/a0-a6
          unlk      a6
          rts       

;Holt die Synchronisatzionfrequenz und setzt sie je nachdem.
mcode14:  move.w    sync,d0
          bra.s     set_sync  ;und sicherheitshalber auch setzen

;Setzt die Synchronisationsfrequenz
mcode13:  move.w    4(sp),d0
          move.w    d0,sync
set_sync: cmpi.w    #50,d0
          beq.s     set_50
          move.w    #$0000,$dff1dc
          rts       
set_50:   move.w    #$20,$dff1dc
          rts       



;Funktion kopiert anzahl Bildschirmzeilen von quelle nach ziel
mcode15:  link      a6,#0
          movem.l   d0-d7/a0-a5,-(sp)

          movea.l   8(a6),a0  ;QUelle
          movea.l   12(a6),a1 ;Ziel
          move.w    16(a6),d7 ;Anzahl
          subq.w    #1,d7     ;wegen DBRA
          moveq     #40,d6    ;jeweils 40 Bytes
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


;unDraw_Mouse
mcode16:  rts       

;draw_mouse:
mcode17:  rts       


;Routine trÑgt andere Routine in VBL-Queue ein
;Prototyp: void *mcode18(void *adr)
mcode18:  link      a6,#0
          movem.l   d1-d7/a0-a5,-(sp)

          lea       vbl_queue,a0
weiter34: tst.l     (a0)+     ;Eintrag frei?
          bne.s     weiter34
          move.l    8(a6),-(a0)
          move.l    a0,d0     ;Adresse zurÅckgeben

          movem.l   (sp)+,d1-d7/a0-a5
          unlk      a6
          rts       

;draw_shift(base,save,sprite_mem,sprite_nr,shape,x,y)
;long base;              /* Bildschirmbasis */
;long save;              /* Basis des Hintergrundpointerbereichs */
;long sprite_mem;        /* basis fÅr die Spritedaten */
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
;verÑnderte Register: keine
          mulu.w    #160,d2   ;160 Bytes pro Scanline
          move.w    d1,d3     ;X-retten
          lsr.w     #4,d3     ;/16
          lsl.w     #1,d3     ;*2

          add.w     d3,d2     ;Adresse des Beginns von 4 Planes
          adda.w    d2,a0     ;Offset auf Bildschirmbasis
          movea.l   hlpbuf,a1
          adda.w    d2,a1     ;Offset fÅr Hintergrund (Quelle)
          move.l    a1,(a2)+
          move.l    a0,(a2)   ;Offset in Buffer

          andi.w    #15,d1    ;Verschiebungsfaktor berechnen
          move.w    d1,d5

          movea.l   16(a6),a1 ;Basisadresse der Sprites
          lsl.w     #6,d0     ;*64
          adda.w    d0,a1     ;Basisadresse der Spritedaten

          moveq     #7,d7     ;Anzahl der Scanlines (Hîhe des Sprites)

spr_draw: moveq     #0,d0
          moveq     #0,d1
          moveq     #0,d2
          moveq     #0,d3
          moveq     #0,d4     ;Maske
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
          or.w      d0,(a0)
          and.w     d4,40(a0)
          or.w      d1,40(a0)
          and.w     d4,80(a0)
          or.w      d2,80(a0)
          and.w     d4,120(a0)
          or.w      d3,120(a0)

          swap.w    d0
          swap.w    d1
          swap.w    d2
          swap.w    d3
          swap.w    d4

          and.w     d4,2(a0)  ;Maske ausanden
          or.w      d0,2(a0)
          and.w     d4,42(a0)
          or.w      d1,42(a0)
          and.w     d4,82(a0)
          or.w      d2,82(a0)
          and.w     d4,122(a0)
          or.w      d3,122(a0)
          lea       160(a0),a0          ;Eine Zeile tiefer

          dbra      d7,spr_draw

          movem.l   (sp)+,d0-d7/a0-a6
          unlk      a6
          rts       


;undraw_sprite(save,sprite_nr)
;long save;
;int sprite_nr;
;  /* Lîscht ein Sprite vom Bildschirm */
;  /* Die Basis wird aus dem Saveblock gewonnen */
;  /* Die Hintergrundinformation aus bild_buf */
mcode20:  link      a6,#0
          movem.l   d0-d7/a0-a6,-(sp)   ; Register retten

          move.w    12(a6),d0 ;Sprite_nr
          movea.l   8(a6),a0  ; Savebasis

; Lîscht Sprite vom Bildschirm, Hintergrund wird von bild_buf geholt
; Parameter:
; D0: Spritenummer
; A0: Basisadresse des Savebereichs
; verÑnderte Register: ?
clr_mob:  lsl.w     #3,d0     ;x8
          adda.w    d0,a0     ;dazuaddieren zu Savebasis

          movea.l   (a0),a2   ;Offset holen
          cmpa.l    #0,a2     ;Buffer ungÅltig?
          beq.s     raus      ; ja, raus

          clr.l     (a0)+     ;Buffer ist jetzt ungÅltig
          movea.l   (a0)+,a1  ;Zieladresse

          moveq     #7,d7     ;8 Scanlines restaurieren
loop89:   move.l    (a2),(a1)
          move.l    40(a2),40(a1)
          move.l    80(a2),80(a1)
          move.l    120(a2),120(a1)
          lea       160(a2),a2
          lea       160(a1),a1
          dbra      d7,loop89

raus:     movem.l   (sp)+,d0-d7/a0-a6
          unlk      a6
          rts       

;Funktion zum Setzen einer Palette in einer bestimmten Rasterzeile
;C-Definition: set_palette(pal_adr,zeile,res)

mcode23:  link      a6,#0
          movem.l   d0-d7/a0-a6,-(sp)

          lea       hbl_system,a0

          move.w    12(a6),d6
          beq.s     vbl_setzen

;Ist derselbe Raster schon vorhanden?
          move.w    #anz_raster-2,d7
nicht_der:          lea       34(a0),a0 ;NÑchsten Eintrag nehmen
          cmp.w     (a0),d6   ;Ist dieser Raster an dieser Position?
          beq.s     vbl_setzen          ;ja, also Palette setzen
          dbra      d7,nicht_der

;Nein->Neuen Rastereintrag nehmen:
          lea       hbl_system,a0
besetzt:  lea       34(a0),a0 ;Einen Eintrag weitergehen
          tst.w     (a0)      ;Raster frei?
          bne.s     besetzt

vbl_setzen:         move.w    12(a6),(a0)+        ;Freier Eintrag->Position eintragen
          movea.l   8(a6),a1  ;Farbpalette
          cmpa.l    #0,a1     ;Keine Rasterpalette angegeben? (5)
          beq.s     del_raster          ;ja->Raster lîschen (5)
          moveq.l   #15,d7    ;16 Worte Åbertragen
copy11:   move.w    (a1)+,(a0)+
          dbra      d7,copy11
          bra.s     end_set_raster      ;(5)

del_raster:         clr.w     -(a0)     ;Raster lîschen (5)

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

;Ist der Raster vorhanden?
          move.w    #anz_raster-2,d7
nicht_der2:         lea.l     34(a0),a0 ;NÑchsten Eintrag nehmen
          cmp.w     (a0),d6   ;Ist dieser Raster an dieser Position?
          beq.s     vbl_setzen2         ;ja, also Palette setzen
          dbra      d7,nicht_der2

          bra.s     rausraus

vbl_setzen2:        addq.l    #2,a0
          movea.l   a0,a1     ;merken
          move.w    10(a6),d6 ;Anzahl der Faderschritte
          bra.s     fade_dbra
schleife11:         movea.l   a1,a0
          moveq.l   #1,d7     ;Zuerst "B"-Wert behandeln
          move.w    #%111,d5  ;AND-Maske
          bsr       sub_pal_atari       ;runterschieben
          movea.l   a1,a0
          moveq.l   #16,d7    ;Dann "G"-Wert behandeln
          move.w    #%1110000,d5        ;AND-Maske
          bsr       sub_pal_atari
          movea.l   a1,a0
          move.w    #256,d7   ;Dann "R"-Wert behandeln
          move.w    #%11100000000,d5    ;AND-Maske
          bsr       sub_pal_atari
fade_dbra:          dbra      d6,schleife11

rausraus: movem.l   (sp)+,d0-d7/a0-a5
          unlk      a6
          rts       

;Fadet eine Palette, aber nicht in einer Copperliste
sub_pal_atari:      moveq.l   #15,d3    ;16 Farben pro Palette
col_loop22:         bsr       sub_col_atari       ;Farbe runterziehen
          dbra      d3,col_loop22       ;alle 16 Farben
          rts       

sub_col_atari:      move.w    (a0)+,d0  ;Farbe holen
          move.w    d0,d1     ;kopieren
          and.w     d5,d1     ;ausmaskieren
          beq       raus566   ;schon auf Null, nichts mehr machen

          sub.w     d7,d0     ;Farbwert eins runter
          move.w    d0,-2(a0) ;Farbe zurÅckschreiben
raus566:  rts       

;Diese Funktion schaltet die Raster aus
;C-Definition: clear_interrupts()
mcode24:  movem.l   d7/a0,-(sp)

          lea       hbl_system,a0
          move.w    #anz_raster-1,d7
clear_lp: clr.w     (a0)
          lea       34(a0),a0
          dbra      d7,clear_lp

          lea       hbl_system+2,a0
          moveq     #7,d7     ;8 Langworte
clear_lp2:          clr.l     (a0)+
          dbra      d7,clear_lp2        ;VBL-Palette schwarz

          movem.l   (sp)+,d7/a0
          rts       

;Routine blendet das Bild langsam aus
;C-Definition: fade_out()
mcode25:  movem.l   d0-d7/a0-a6,-(sp)

          moveq     #14,d6    ;15 mal subtrahieren
schleife1:          moveq     #1,d7     ;Zuerst "B"-Wert behandeln
          move.w    #%0000000000001111,d5         ;AND-Maske
          bsr       subtract  ;runterschieben
          moveq     #16,d7    ;Dann "G"-Wert behandeln
          move.w    #%0000000011110000,d5         ;AND-Maske
          bsr       subtract
          move.w    #256,d7   ;Dann "R"-Wert behandeln
          move.w    #%0000111100000000,d5         ;AND-Maske
          bsr       subtract

          bsr       vsync
          dbra      d6,schleife1        ;alle 8 mal

          movem.l   (sp)+,d0-d7/a0-a6
          rts       

subtract: lea       rasters1,a0
pal_loop: cmpi.l    #$fffffffe,(a0)     ;Ende erreicht?
          beq.s     no_more_sub
          cmpi.w    #$0180,(a0)         ;Raster gefunden?
          beq.s     abfaden
          addq.l    #4,a0
          bra.s     pal_loop
abfaden:  bsr       sub_pal
          bra.s     pal_loop
no_more_sub:        rts       

sub_pal:  moveq     #15,d3    ;16 Farben pro Palette
col_loop: bsr       sub_col   ;Farbe runterziehen
          dbra      d3,col_loop         ;alle 16 Farben
          rts       

sub_col:  addq.l    #2,a0     ;Farbregister Åberspringen
          move.w    (a0)+,d0  ;Farbe holen
          move.w    d0,d1     ;kopieren
          and.w     d5,d1     ;ausmaskieren
          beq       raus56    ;schon auf Null, nichts mehr machen

          sub.w     d7,d0     ;Farbwert eins runter
          move.w    d0,-2(a0) ;Farbe zurÅckschreiben
raus56:   rts       


;Diese Routine blendet das Bild wieder weich ein
;C-Definition: fade_in();
mcode26:  movem.l   d0-d7/a0-a6,-(sp)

          lea       $00dff000,a5
          bsr       mcode55   ;Offsets berechnen

          lea       bitfeld,a2          ;Hier Copperliste aufbauen
          lea       rasters2,a0
trans_copper:       move.w    (a0)+,(a2)+         ;Copperliste Åbertragen
          cmpa.l    raster_start,a0     ;aber nur bis Raster
          blt.s     trans_copper
          lea       zeros,a0
          bsr       make_copper         ;VBL-Palette schwarz
          move.l    #$fffffffe,(a2)     ;Ende der Copperlist

          move.l    #bitfeld,cop1lc(a5) ;und anzeigen
          move.l    #bitfeld,copperliste

          bsr       vsync
          bsr       rst22rst1 ;nach Raster1 kopieren

          lea       rasters1,a0
trans1:   cmpi.l    #$fffffffe,(a0)     ;Ende der Copperlist?
          beq.s     copperend
          cmpi.w    #$0180,(a0)         ;Beginn eines Rasters?
          bne.s     skip
          moveq     #15,d7    ;16 Farbregister
farbe_schwarz:      addq.l    #2,a0     ;Farbregister skipen
          clr.w     (a0)+     ;Farbe auf schwarz
          dbra      d7,farbe_schwarz
          bra.s     trans1
skip:     addq.l    #4,a0     ;Befehl Åberspringen
          bra.s     trans1

copperend:          
          move.l    #rasters1,cop1lc(a5)
          move.l    #rasters1,copperliste

          moveq     #14,d6    ;15 mal addieren
schleife3:          moveq     #1,d7     ;Zuerst "B"-Wert behandeln
          move.w    #%0000000000001111,d5         ;AND-Maske
          bsr       addiere   ;runterschieben
          moveq     #16,d7    ;Dann "G"-Wert behandeln
          move.w    #%0000000011110000,d5         ;AND-Maske
          bsr       addiere
          move.w    #256,d7   ;Dann "R"-Wert behandeln
          move.w    #%0000111100000000,d5         ;AND-Maske
          bsr       addiere

          bsr       vsync
          dbra      d6,schleife3        ;alle 8 mal

          movem.l   (sp)+,d0-d7/a0-a6
          rts       

addiere:  lea       rasters1,a0         ;Wird verÑndert
          lea       rasters2,a1         ;Endwert
such_raster:        cmpi.l    #$fffffffe,(a0)     ;Ende erreicht?
          beq.s     no_more_add
          cmpi.w    #$0180,(a0)         ;Raster gefunden?
          beq.s     auffaden
          addq.l    #4,a0     ;zum nÑchsten Befehl
          addq.l    #4,a1
          bra.s     such_raster

auffaden: bsr       add_pal
          bra.s     such_raster
no_more_add:        rts       

add_pal:  moveq     #15,d3    ;16 Farben pro Palette
col_loop2:          addq.l    #2,a0     ;Farbregister Åberspringen
          addq.l    #2,a1     ;-"-
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

;          "C O M P R E S S O R"
; ( fÅr Nightrider Software )
; komprimiert farbige Bilder sehr platzsparend.
; Algorithmus besser als Degas Elite!

; (c) Stefan Schreiber  1987/88
; V 2.0

;packt ein Bild platzsparend
;C-Definition: compress(zeilen,quelle,ziel);

mcode27:  link      a6,#0
          movem.l   d1-d7/a0-a6,-(sp)

          lea       bitfeld,a0
          move.w    #1999,d0
schleife2:          
          clr.w     (a0)+
          dbra      d0,schleife2

          movem.l   d3-d7,quicksave
          move.w    8(a6),d0  ; LÑnge des Bilds in Zeilen ( 1-200 )
          movea.l   10(a6),a0 ; Quelladdresse
          movea.l   14(a6),a1 ; Zieladdr., in die kompr. File geschrieben wird

          move.w    d0,(a1)+  ; ZeilenlÑnge: 1. Wort nach dem Vorspann
          movea.l   a0,a2
          mulu.w    #160,d0   ; Zeilenzahl * 160 = Anzahl der Bytes
          move.w    d0,pic_length
          ext.l     d0
          adda.l    d0,a2
          move.l    a2,endsource

          clr.w     d0        ; Testwert
          clr.w     d1        ; Pos
          moveq     #1,d2     ; Counter fÅr Bytewiederholungen
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
          lea       0(a0,d5.w),a2
          cmpa.l    endsource,a2
          bcc       exit28
          cmp.b     (a2),d0   ; = Testwert?
          bne       exit28
          addq.w    #1,d2     ; Counter erhîhen
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
          moveq     #1,d2     ; !!!
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
          suba.l    a0,a1     ; Pufferende-Pufferanfang = FilelÑnge
          move.l    a1,d0     ; RÅckgabewert fÅr Hauptprogramm
          movem.l   quicksave,d3-d7

          movem.l   (sp)+,d1-d7/a0-a6
          unlk      a6
          rts       

; d0 enthÑlt FilelÑnge

restloop: 
          bsr       belegung
          bne       cont2
          move.b    0(a0,d1.w),(a1)+    ; Byte in Puffer
cont2:    
          addq.w    #1,d1
          cmp.w     pic_length,d1
          bcs       restloop
          rts                 ; Es fehlt die PufferlÑnge!!!

belegung: 
          lea       bitfeld,a2
          move.w    d1,d3
          lsr.w     #3,d3     ; d3 \= 8       ( C-Notation )
          btst      d1,0(a2,d3.w)
          rts       

set_flags:          
          move.w    d1,d3     ; cpos
          lsr.w     #3,d3
          lea       bitfeld,a2
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

          lea       bitfeld,a0          ; Flags lîschen
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
          lea       bitfeld,a2
          move.w    (a0)+,d0  ; BildlÑnge in Zeilen
          mulu.w    #160,d0   ; *160
          move.w    d0,pic_length

          clr.w     d1
          clr.w     d2
          clr.w     d3
          clr.w     d4
          clr.w     d5
          clr.w     d6

decomp:   
          move.b    (a0)+,d4  ; Offset zu nÑchsten Block
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
          addi.w    #160,d5   ; nÑchste Bytewiederholung
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

;Set_Mouse_Form: Setzt die Form der Maus
;-> 8(a6).W: Mausnummer
mcode29:  link      a6,#0
          movem.l   d0-d7/a0-a6,-(sp)

          move.w    8(a6),d0  ;Mausform holen
          move.w    d0,mouseform        ;und merken

          lea       maus_daten,a0
          mulu.w    #36,d0    ;36 Bytes pro Maus
          adda.l    d0,a0
          lea       maus_sprite,a1
          addq.l    #4,a1     ;Koordinaten skipen
          move.w    (a0)+,hot_spotx
          move.w    (a0)+,hot_spoty
          moveq     #15,d7    ;16 Zeilen hoch
copy_maus:          move.w    (a0)+,d0
          not.w     d0
          move.w    d0,(a1)+
          move.w    d0,(a1)+
          dbra      d7,copy_maus

          movem.l   (sp)+,d0-d7/a0-a6
          unlk      a6
          rts       


;TrÑgt in die Spriteliste der Maus ihre Koordinaten ein...
set_mousexy:        movem.l   d0-d7/a0-a6,-(sp)

          move.w    mousex,d0
          move.w    mousey,d1
          sub.w     hot_spotx,d0
          sub.w     hot_spoty,d1
          moveq     #16,d2    ;Hîhe der Maus
          lea       maus_sprite,a0

          bsr       setz_koordinaten

          movem.l   (sp)+,d0-d7/a0-a6
          rts       

;Setzt die Koordinaten eines Sprites in dessen Sprite-Liste
;-> D0.W: X
;   D1.W: Y
;   D2.W: Hîhe des Sprites
;   A0.L: Adresse der Spriteliste
setz_koordinaten:   
          addi.w    #128,d0   ;Bildschirmstart
          add.w     wistarty,d1         ;-"-

          move.w    d0,d3
          andi.w    #1,d3     ;H0-Bit
          lsr.w     #1,d0     ;H1-H8 auf Position
          move.w    d0,d4
          andi.w    #$00ff,d4 ;E0-E7 lîschen
          move.w    d1,d0
          add.w     d2,d0
          asl.w     #8,d1
          bcc.s     noE8
          bset      #2,d3
noE8:     or.w      d1,d4
          asl.w     #8,d0
          bcc.s     noL8
          bset      #1,d3
noL8:     or.w      d0,d3
          move.w    d4,(a0)+
          move.w    d3,(a0)
          rts       

;Setzt die Maus auf einen bestimmten Punkt (7)
;C-Defintion: void set_mousepos(int x,int y);
mcode73:  move.l    4(sp),mousex        ;X/Y setzen
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

          moveq     #0,d3
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
          andi.w    #%0000000000000111,d3
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

traf_err: moveq     #0,d3     ;Auûerhalb der Karte=LEER
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

          clr.w     steuerzeichen       ;(1)

          clr.w     scroll_akt          ;in Buffer 0 starten
          movea.l   22(a6),a0 ;Scrollbuffer
          move.w    #6719,d7
clerr:    clr.l     (a0)+
          clr.l     (a0)+     ;Buffer lîschen
          dbra      d7,clerr

          movem.l   (sp)+,d0-d7/a0-a6
          unlk      a6
          rts       

initscroller:       move.l    scrollbegin,scrollpos
          subq.l    #1,scrollpos        ;ein zeichen zurÅck
          move.w    #-200,delta_x
          rts       

;Diese Funktion scrollt die Laufschrift um eins weiter
;C-Definition: void doscroller(void)
mcode33:  movem.l   d0-d7/a0-a6,-(sp)

scroll:   cmpi.w    #-2,delta_x         ;Noch Spalten da?
          bgt       char_weiter         ;Ja, Buchstabe fertig machen

          addq.l    #1,scrollpos        ;nÑchstes Zeichen
          movea.l   scrollpos,a0
          moveq     #0,d0
          move.b    (a0),d0   ;NÑchstes Zeichen holen
          bne.s     verarbeite          ;Nicht das letzte, weiter

          bsr       initscroller        ;Wieder von vorne
          bra.s     scroll    ;und starten

verarbeite:         move.w    d0,d1
          lea       sonder,a0
nxt_element:        tst.b     (a0)      ;Tabelle zuende?
          beq.s     kein_sonder
          cmp.b     (a0)+,d0  ;in Tabelle enthalten?
          bne.s     nxt_element

          suba.l    #sonder+1,a0
          move.w    a0,d0
          addi.w    #26*2,d0  ;Hinter den Groû und Kleinbuchstaben
          bra.s     sonderzeichen

kein_sonder:        cmpi.w    #'Z',d0
          ble.s     nehmen
          subi.w    #'a'-'Z'-1,d0       ;Kleinbuchstaben
nehmen:   subi.w    #'A',d0   ;in internen Code wandeln
          bpl.s     sonderzeichen       ;(1)
          move.w    d1,steuerzeichen    ;(1)
          bra.s     scroll    ;(1)

sonderzeichen:      movea.l   font_start,a0       ;Basis der Bibliothek
          cmp.w     (a0)+,d0  ;Objekt noch in Bibliothek?
          bgt.s     scroll    ;Nein->nÑchster Buchstabe

          move.w    d0,buchstabe        ;Aktuelle Ob_Nr des Buchstabens

          mulu.w    #20,d0    ;Adresse des Ob_Headers berechnen
          adda.l    d0,a0
          move.l    a0,char_basis

          movea.l   font_start,a1       ;Basis der Objekte
          addq.l    #2,a1     ;Anzahl Åberspringen
          adda.l    (a0),a1   ;=Basis des Objekts
          move.l    a1,adrspalte        ;Datenstart
          move.w    4(a0),d0  ;Breite
          addq.w    #1,d0
          bclr      #0,d0     ;Gerade machen
          move.w    d0,delta_x
          move.w    d0,4(a0)

char_weiter:        

          move.w    #336,-(sp)
          move.l    scroll_buff,-(sp)
          clr.l     -(sp)
          move.w    #336,-(sp)
          move.l    scroll_buff,-(sp)
          move.w    #41,-(sp)
          move.w    #319,-(sp)
          clr.w     -(sp)
          move.w    #2,-(sp)
          bsr       mcode49   ;Buffer um zwei Pixel nach links schieben
          lea       24(sp),sp

          tst.w     delta_x
          bgt.s     kein_space

          movea.l   font_start,a0       ;Basis der Bibliothek
          addq.l    #2,a0     ;Anzahl Åberspringen

          moveq     #56,d0    ;Space
          mulu.w    #20,d0    ;Adresse des Ob_Headers berechnen
          adda.l    d0,a0
          move.l    a0,char_basis

          movea.l   font_start,a1       ;Basis der Objekte
          addq.l    #2,a1     ;Anzahl Åberspringen
          adda.l    (a0),a1   ;=Basis des Objekts
          move.l    a1,adrspalte        ;Datenstart

          moveq     #0,d0
          movea.l   char_basis,a3
          bra.s     scroll_it

kein_space:         movea.l   char_basis,a3
          move.w    4(a3),d0  ;Breite des Objekts
          sub.w     delta_x,d0

scroll_it:          move.w    #336,-(sp)          ;Breite des Bildschirms in Pixel
          move.l    scroll_buff,-(sp)   ;Screenbasis
          move.w    #0,-(sp)  ;y
          move.w    #318,-(sp)          ;x
          move.w    10(a3),d1 ;Breite des Objekts in Bytes
          add.w     d1,d1     ;breite in Pixel
          move.w    d1,-(sp)  ;Breite des Objekts in Pixel merken
          move.l    adrspalte,-(sp)     ;Adresse des Objektes
          move.w    6(a3),-(sp)         ;y2
          subq.w    #1,(sp)
          move.w    d0,-(sp)  ;x2
          addq.w    #1,(sp)
          clr.w     -(sp)     ;y1
          move.w    d0,-(sp)  ;x1
          bsr       mcode49   ;raster_replace
          lea       24(sp),sp

          subq.w    #2,delta_x          ;eines weiter

          move.w    #320,-(sp)
          move.l    scrollbase,-(sp)
          move.w    yscroller,-(sp)     ;yscroller
          clr.w     -(sp)
          move.w    #336,-(sp)
          move.l    scroll_buff,-(sp)
          move.w    #41,-(sp)
          move.w    #319,-(sp)
          clr.w     -(sp)
          clr.w     -(sp)
          bsr       mcode49   ;Scroller auf Screen kopieren
          lea       24(sp),sp

scroller_fertig:    movem.l   (sp)+,d0-d7/a0-a6
          rts       

sonder:   .DC.b ".,:' ÅÑî?û"  ;(3)
          .DC.b '"$',0        ;(3)

          .EVEN 

;Diese Funktion initialisiert das Rastersystem
mcode34:  movem.l   d0-d7/a0-a6,-(sp)

          move.w    #0,-(sp)  ;Mauskreuz bitte
          bsr       mcode29
          addq.l    #2,sp

          bsr       set_mousexy         ;TrÑgt die Koordinaten ein

          lea       $00dff000,a5
          move.w    #%0000001111111111,dmacon(a5) ;DMA aus

          bsr       mcode24   ;Clear Raster
          bsr       mcode55   ;Calc-Offsets
          bsr       rst22rst1 ;und zurÅckkopieren

          move.l    $0000006c,old_i_3   ;Alten VBL-Vektor merken
          move.l    #new_i_3,$0000006c  ;Neuen Vektor setzen
          move.l    $00000068,old_i_2
          move.l    #new_i_2,$00000068

          lea       $00dff000,a5        ;Basis der Custom-Chips
          move.b    #%10001000,$00bfed01          ;Tastatur Irq an

          move.l    #rasters1,cop1lc(a5)
          move.l    #rasters1,copperliste         ;Adresse der Copperliste
          clr.w     copjmp1(a5)         ;Copper starten
          move.w    wistarty,d0
          lsl.w     #8,d0
          move.b    #$81,d0
          move.w    d0,diwstrt(a5)
          move.w    wistarty,d0
          addi.w    #200,d0
          lsl.w     #8,d0
          move.b    #$c1,d0
          move.w    d0,diwstop(a5)
          move.w    #$0038,ddfstrt(a5)
          move.w    #$00d0,ddfstop(a5)
          move.w    #%0100001000000000,bplcon0(a5)
          clr.w     bplcon1(a5)
          move.w    #8,bplcon2(a5)
          move.w    #120,bpl1mod(a5)
          move.w    #120,bpl2mod(a5)

          move.w    #%1000011111100000,dmacon(a5) ;DMA-Quellen einschalten

          lea       $00dff1a0,a0
          moveq     #15,d7    ;16 Farben
set_maus_farbe:     move.w    #$0f00,(a0)+        ;Mauszeiger rot
          dbra      d7,set_maus_farbe

          move.w    #%1100000000101000,intena(a5) ;IRQs anschalten

          movem.l   (sp)+,d0-d7/a0-a6
          rts       

;Level 3 IRQ-Routine (Blitter/VBL/Copper):
new_i_3:  movem.l   d0-d7/a0-a6,-(sp)

          lea       $00dff000,a5
          move.w    intenar(a5),d0
          btst      #14,d0    ;IRQs erlaubt?
          beq.s     raus_i_3
          and.w     intreqr(a5),d0      ;erlaubte und aufgetretene..
          btst      #5,d0     ;VBL?
          beq.s     raus_i_3

          addq.l    #1,vbl_ct ;VBL-Counter erhîhen
          bsr       check_rand          ;ÅberprÅfen, ob Rand bewegt werden soll
          bsr       maus_auswert
          bsr       set_mousexy         ;und neue X/Y eintragen

          lea       vbl_queue,a0
          moveq     #9,d7
test_queue:         tst.l     (a0)+     ;Eintrag vorhanden?
          beq.s     next_queue
          movea.l   -4(a0),a1 ;Adresse holen
          movem.l   d0-d7/a0-a6,-(sp)
          jsr       (a1)
          movem.l   (sp)+,d0-d7/a0-a6
next_queue:         dbra      d7,test_queue

raus_i_3: move.w    #%0000000001110000,$00dff09c  ;Request frei
          movem.l   (sp)+,d0-d7/a0-a6
          rte       

maus_auswert:       moveq     #0,d0
          btst      #6,ciaapra          ;Links gedrÅckt?
          bne.s     kein_links
          moveq     #1,d0
kein_links:         move.w    potgor(a5),d1
          btst      #10,d1    ;rechts gedrÅckt?
          bne.s     kein_rechts
          addq.w    #2,d0
kein_rechts:        move.w    d0,mousek

          lea       oldmouse,a1
          move.w    joy0dat(a5),d0      ;MauszÑhler lesen
          move.w    d0,d1
          lsr.w     #8,d1
          sub.b     (a1),d1
          ext.w     d1
          tst.w     rand_bewegen        ;Wird gerade Screen verschoben?
          bne.s     maus_bewegen

          add.w     d1,wistarty         ;auf Screenstart
          cmpi.w    #3,wistarty
          bge       ok77
          move.w    #3,wistarty
ok77:     cmpi.w    #250,wistarty
          ble       ok78
          move.w    #250,wistarty
ok78:     move.w    wistarty,d1
          lsl.w     #8,d1
          move.b    #$81,d1
          move.w    d1,diwstrt(a5)
          move.w    wistarty,d1
          addi.w    #200,d1
          lsl.w     #8,d1
          move.b    #$c1,d1
          move.w    d1,diwstop(a5)
          bra       wtt

maus_bewegen:       add.w     d1,mousey
          add.w     d1,mdeltay          ;(10)
          sub.b     1(a1),d0
          ext.w     d0
          add.w     d0,mousex
          add.w     d0,mdeltax          ;(10)
wtt:      move.w    joy0dat(a5),(a1)    ;und alten Status merken

          tst.w     mousex
          bpl.s     ok1
          clr.w     mousex
ok1:      tst.w     mousey
          bpl.s     ok2
          clr.w     mousey
ok2:      cmpi.w    #320,mousex
          blt.s     ok3
          move.w    #319,mousex
ok3:      cmpi.w    #199,mousey
          ble.s     ok4
          move.w    #199,mousey
ok4:      rts       

;öberprÅft, ob Spieler in der obersten Zeile mit links geklickt hat
;dann wird der Bildschirm verschoben
check_rand:         tst.w     rand_bewegen        ;Wird Screen schon verschoben?
          beq.s     screen_move

          tst.w     mousey    ;Spieler in Zeile 0?
          bne.s     kein_rand
          cmpi.w    #3,mousek ;Rechts+Links gedrÅckt?
          bne.s     kein_rand

          clr.w     rand_bewegen        ;ab jetzt wird Screen bewegt!
          rts       

screen_move:        cmpi.w    #3,mousek ;Maustaste immer noch gedrÅckt?
          beq.s     rts78
kein_rand:          move.w    #-1,rand_bewegen    ;Screen nicht mehr bewegen
rts78:    rts       

rand_bewegen:       .DC.w -1  ;Kein Screen bewegen
wistarty: .DC.w 41  ;Scanline, in der Bild startet

;IRQ-Routine fÅr Level 2 (CIA A/Expansion Port):
new_i_2:  movem.l   d0-d7/a0-a6,-(sp)

          moveq     #0,d0
          move.b    $00bfed01,d0        ;IRQ-Kontrollregister
          move.w    d0,-(sp)  ;Kontrollregister retten
          btst      #7,d0     ;IRQ von CIA-A?
          beq       falsch
i_2_auswert:        move.w    (sp),d0   ;Kontrollregister holen
          btst      #1,d0
          bne       music_bitte
          btst      #3,d0     ;Tastaturbyte?
          beq       falsch

          bclr      #3,1(sp)  ;Tastaturirq ausgewertet

          move.b    $00bfec01,d0        ;Tastaturbyte abholen

          ori.b     #$40,$00bfee01      ;fÅr Handshake

          andi.w    #$00ff,d0
          not.b     d0        ;invertierem
          ror.b     #1,d0

          cmpi.b    #$f9,d0
          beq       tastatur_fehler
          cmpi.b    #$fa,d0
          beq       tastatur_fehler
          cmpi.b    #$fc,d0
          beq       tastatur_fehler
          cmpi.b    #$fd,d0
          beq       tastatur_fehler
          cmpi.w    #$00fe,d0
          beq       tastatur_fehler
          cmpi.w    #$0060,d0 ;Linke Shift?
          beq       shift_pressed
          cmpi.w    #$0061,d0 ;Rechte Shift?
          beq       shift_pressed
          cmpi.w    #$0060+$0080,d0
          beq       shift_released
          cmpi.w    #$0061+$0080,d0
          beq       shift_released
          cmpi.w    #$0064,d0 ;Alternate
          beq       alt_pressed
          cmpi.w    #$0066,d0 ;Amiga links?
          beq       alt_pressed
          cmpi.w    #$0067,d0 ;Amiga rechts?
          beq       alt_pressed
          cmpi.w    #$0063,d0 ;Control?
          beq       ctrl_pressed
          cmpi.w    #$0064+$0080,d0     ;Alt los?
          beq       alt_released
          cmpi.w    #$0066+$0080,d0     ;Amiga links los?
          beq       alt_released
          cmpi.w    #$0067+$0080,d0     ;Amiga rechts los?
          beq       alt_released
          cmpi.w    #$0063+$0080,d0     ;CTRL los?
          beq       ctrl_released
          cmpi.w    #$0046,d0 ;Delete?
          beq       delete_pressed
normale_taste:      
          tst.b     d0
          bmi.s     losgelassen

          move.b    d0,amiga_raw

          lea       tast_ami_st,a3
          move.b    0(a3,d0.w),d0       ;in ST Scancode wandeln
          lea       scancode,a3         ;Adresse der Scancode-Tabelle
          moveq     #0,d2
          move.b    0(a3,d0.w),d2       ;In ASCII wandeln
          swap.w    d0        ;Scancode in oberes Byte
          move.w    d2,d0     ;ASCII unten rein
          move.l    d0,keypress         ;merken
eintragen:          move.w    tail,d1
          addq.w    #4,d1     ;Ein Byte weiter
          cmpi.w    #80,d1    ;Bufferende erreicht?
          bcs.s     nein
          moveq     #0,d1
nein:     cmp.w     head,d1   ;Buffer voll?
          beq.s     buffer_voll
          lea       key_buffer,a2
          move.l    d0,0(a2,d1.w)
          move.w    d1,tail
tastatur_fehler:    
buffer_voll:        

          move.w    #136,d0   ;Verzîgerung fÅr Amiga 2000-A
warten:   tst.w     $dff004   ;26 cc pro Schleife
          dbra      d0,warten ;ergibt 500 Mikrosekunden

          andi.b    #$bf,$00bfee01      ;Handshake zurÅck

          bra       i_2_auswert         ;Vielleicht noch ein Music-Irq?

falsch:   move.w    (sp)+,d0  ;Kontrollregister entfernen

          movem.l   (sp)+,d0-d7/a0-a6
          move.w    #%0000000000001000,$00dff09c  ;Request frei
          rte       

losgelassen:        clr.l     keypress  ;Keine Taste mehr gedrÅckt
          bra.s     buffer_voll

shift_pressed:      bset      #0,shift
          bra.s     buffer_voll
shift_released:     bclr      #0,shift
          bra.s     buffer_voll
alt_pressed:        bset      #1,shift
          bra.s     buffer_voll
alt_released:       bclr      #1,shift
          bra.s     buffer_voll
ctrl_pressed:       bset      #2,shift
          bra.s     buffer_voll
ctrl_released:      bclr      #2,shift
          bra.s     buffer_voll

delete_pressed:     cmpi.b    #%00000110,shift    ;CTRL+ALT?
          bne       normale_taste
          reset               ;ROM einblenden
          movea.l   4,a0
          jmp       (a0)      ;Reset auslîsen

;Diese Tabelle wandelt die Rawcodes der Amiga-Tastatur in
;ScanCodes des ST um:
tast_ami_st:        .DC.b $39
          .DC.b $02,$03,$04,$05,$06,$07,$08,$09,$0a,$0b     ;1..0
          .DC.b $0c,$0d,$2b   ;bis "|"=$0d
          .DC.b $39,$70
          .DC.b $10,$11,$12,$13,$14,$15,$16,$17   ;Qwert
          .DC.b $18,$19,$1a,$1b         ;opÅ*
          .DC.b $39 ;=$1c
          .DC.b $4b,$50,$4d   ;1,2,3 (Num)
          .DC.b $1e,$1f,$20,$21,$22,$23,$24,$25   ;asdfgh
          .DC.b $26,$27,$28   ;LîÑ#
          .DC.b $39,$39       ;$2b,$2c
          .DC.b $52,$48,$47   ;4,5,6 (Num)
          .DC.b $60,$2c,$2d,$2e,$2f,$30,$31       ;<yxcv
          .DC.b $32,$33,$34,$35         ;,.-
          .DC.b $39,$39       ;$3b,$3c
          .DC.b $67,$68,$69   ;7,8,9 (Num)
          .DC.b $39 ;$40
          .DC.b $0e ;Backspace
          .DC.b $0f ;Tab
          .DC.b $1c ;Enter (Num)
          .DC.b $1c ;Return
          .DC.b $01 ;Escape
          .DC.b $53 ;Delete
          .DC.b $39,$39,$39   ;47,48,49
          .DC.b $4a ;$4a
          .DC.b $39 ;$4b
          .DC.b $48,$50,$4d,$4b         ;Cursor
          .DC.b $3b,$3c,$3d,$3e,$3f,$40,$41       ;F1...
          .DC.b $42,$43,$44   ;...F10
          .DC.b $63,$64,$65,$66,$4e     ;$5a-$5e ()/* ZiffBlock
          .DC.b $62 ;Help
          .DS.b 32  ;,$00000039 ;$60-$7f
          .EVEN 

music_bitte:        bclr      #1,1(sp)  ;Musik-IRQ ausgewertet....
          tst.w     musik_an  ;Soll Musik dudeln?
          beq.s     keine_musik

          tst.b     halts_maul
          beq.s     keine_musik

          movea.l   music_routine,a0
          jsr       4(a0)

keine_musik:        bra       i_2_auswert         ;Vielleicht noch ein Tastaturbyte?


;Diese Funktion meldet das Rastersystem ab:
mcode35:  movem.l   d0-d7/a0-a6,-(sp)

          move.l    old_i_3,$0000006c   ;Alten VBL setzen
          move.l    old_i_2,$00000068   ;Alte Tastaturroutine

          lea       $00dff000,a5
          move.w    #$0080,dmacon(a5)   ;Copper abschalten
          move.l    copperliste,cop1lc(a5)        ;Alte Copperlist setzen
          move.w    #$8080,dmacon(a5)   ;Copper anschalten

          move.w    #%0000001111111111,dmacon(a5) ;DMA aus
          move.w    old_dma,dmacon(a5)  ;und alte

          movem.l   (sp)+,d0-d7/a0-a6
          rts       

;Diese Funktion spielt einen Digisound ab, dabei werden saemtliche benîtigte
;IRQ's installiert, und wenn Digi fertig ist, werden die Routinen wieder
;destalliert. Wird als Startadresse 0L ÅÅbergeben wird evtl. vorhandene
;Abspielroutine destalliert (abschalten).
;C-Definition: play_digi(startadr,roundabout,startzeile,endzeile)
;long startadr;                 /* Startadresse */
;FLAG roundabout;               /* True bedeutet, wieder von vorne anfangen wenn fertig */
;int startzeile,endzeile;       /* Im Editor */
mcode36:  link      a6,#0
          movem.l   d0-d7/a0-a5,-(sp)

          move.l    #neuer_sample,$00000070       ;Sample Nachladeroutine

          lea       $00dff000,a5        ;Adresse der Custom-Chips
          move.w    #%0000000000001111,dmacon(a5) ;Audio DMA aus

          clr.w     digi_works          ;Digisound ist aus
          st        halts_maul          ;Hippel darf Spielen

          tst.l     8(a6)     ;Start=0?
          beq       raus_digi ;Ja, nur abschalten

          movea.l   8(a6),a0  ;Start der Digidaten
          cmpi.l    #'SAMP',(a0)+       ;Start einer Sequenz?
          bne       raus_digi ;Nein, also raus

          clr.b     halts_maul          ;Hippel Aus

          move.w    (a0)+,anzahl_seq    ;Anzahl der Sequenzen
          move.w    (a0)+,d7  ;Anzahl der Zeilen im Editor
          move.w    16(a6),d0 ;Endzeile
          sub.w     14(a6),d0 ;-startzeile=Anzahl Zeilen-1
          addq.w    #1,d0     ;Anzahl Zeilen
;          tst.w     12(a6)    ;Roundabout?
;          bne.s     round_round         ;Ja!
;                addq.w  #1,D0                ;Nein, eines mehr
round_round:        move.w    d0,anzahl_zeilen
          subq.w    #1,d0
          move.w    d0,anz_start        ;fÅr nachher merken
          move.l    a0,seq_tabelle      ;Beginn der Sequenztabelle
          move.w    anzahl_seq,d0
          mulu.w    #24,d0
          adda.l    d0,a0
          movea.l   a0,a1     ;Beginn der Editorzeilen
          move.w    14(a6),d0 ;startzeile
          lsl.w     #2,d0     ;x4
          adda.w    d0,a1
          move.l    a1,zeilen_tabelle   ;Beginn der Editorzeilen
          move.l    a1,naechste_zeile   ;Adresse der nÑchsten Editorzeile
          lsl.w     #2,d7
          adda.w    d7,a0
          move.l    a0,start_digi       ;Beginn der Digisounds

          move.w    12(a6),roundabout   ;Flag merken

          move.w    #$000f,dmacon(a5)   ;Audio-DMA aus
          move.w    #$00ff,adkcon(a5)   ;Keine Modulation
          move.w    #64,aud0vol(a5)     ;volle LautstÑrke

          move.w    #1,seq_counter      ;Sofort nachladen
          bsr.s     nxt_zeile ;Alle Parameter setzen

          move.w    #%0000000010000000,intreq(a5) ;Request lîschen
          move.w    #$8201,dmacon(a5)   ;Audio-DMA ein
          move.w    #%1100000010000000,intena(a5) ;Kann IRQ auslîsen

          move.w    #1,digi_works       ;Digisound wird gerade gespielt

raus_digi:          movem.l   (sp)+,d0-d7/a0-a5
          unlk      a6
return:   rts       

;IRQ-Routine zum Nachladen der Samples
neuer_sample:       bsr.s     nxt_zeile ;Neuer Sample nachladen
          move.w    #%0000000010000000,intreq+$00dff000     ;IRQ-Freigeben
          rte       

;LÑdt das nÑchste Sample nach:
nxt_zeile:          movem.l   d0-d7/a0-a6,-(sp)

          lea       $00dff000,a5        ;Adresse DMA

          move.w    sample_freq,aud0per(a5)

          subq.w    #1,seq_counter      ;Einmal abspielen ist fertig
          bgt       nxt_ende  ;Diese Sequenz nochmal spielen

          movea.l   naechste_zeile,a0
          subq.w    #1,anzahl_zeilen    ;Eine Zeile weniger
          bpl.s     nicht_von_vorne     ;weitermachen mit dieser Zeile
          tst.w     roundabout          ;wieder von vorne anfangen?
          bne.s     start_vorne         ;Ja,
          cmpi.w    #-1,anzahl_zeilen   ;Letzten Sample voll laufen lassen
          blt       digi_zuende         ;Stop!
          move.w    #1,seq_counter      ;Beim nÑchsten Mal stoppen!
          bra       nxt_ende  ;und spielen lassen

start_vorne:        movea.l   zeilen_tabelle,a0   ;von vorne anfangen
          move.w    anz_start,anzahl_zeilen       ;Startwert wieder eintragen

nicht_von_vorne:    moveq     #0,d5
          move.b    (a0)+,d5  ;Anzahl der Wiederholungen
          move.w    d5,seq_counter
          move.b    (a0)+,d5  ;nÑchste Sequenz
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
          moveq     #0,d5
          move.b    (a0)+,d5  ;Flag fÅr Richtung

          move.b    (a0)+,d5  ;Frequenz
          move.l    a0,naechste_zeile

          subq.w    #1,d5     ;mindestens 1 Khz
          add.w     d5,d5
          lea       period_tab,a0
          move.w    0(a0,d5.w),sample_freq        ;Periodenwert merken
          move.l    digi_pos,aud0lc(a5) ;Adresse setzen
          move.l    digi_end,d0
          sub.l     digi_pos,d0         ;LÑnge berechnen
          lsr.l     #1,d0     ;Anzahl an Worten
          move.w    d0,aud0len(a5)

nxt_ende: movem.l   (sp)+,d0-d7/a0-a6
          rts       

digi_zuende:        move.w    #$0001,dmacon(a5)   ;DMA Audio aus
          move.w    #%0000000010000000,intena(a5) ;IRQ sperren

          clr.w     digi_works          ;Digisound ist aus
          st        halts_maul          ;Hippel wieder an
          bra.s     nxt_ende

;Tabelle der verschiedenen Periodenwerte fÅr die Samplefrequenzen:
period_tab:         .DC.w 3580          ;1 Khz
          .DC.w 1790          ;2 Khz
          .DC.w 1193          ;3 Khz
          .DC.w 895 ;4 Khz
          .DC.w 716 ;5 Khz
          .DC.w 597 ;6 Khz
          .DC.w 511 ;7 Khz
          .DC.w 447 ;8 Khz
          .DC.w 398 ;9 Khz
          .DC.w 356 ;10 Khz
          .DC.w 325 ;11 Khz
          .DC.w 298 ;12 Khz
          .DC.w 275 ;13 Khz
          .DC.w 256 ;14 Khz
          .DC.w 239 ;15 Khz
          .DC.w 224 ;16 Khz
          .DC.w 211 ;17 Khz
          .DC.w 199 ;18 Khz
          .DC.w 188 ;19 Khz
          .DC.w 179 ;20 Khz

sample_freq:        .DS.w 1


;Zeichnet ein Objekt aus einer Objekt-Bibliothek auf den Bildschirm.
;Sollte das Objekt mit Rastern abgespeichert worden sein, so werden
;diese auch gesetzt.
;void draw_obj(int ob_nr,void *ob_basis,int modus,void *scr_basis,int x,int y)
;22(a6): y
;20(a6): x
;16(a6): scr_basis
;14(a6): modus (0=Replace, 1=Transparent)
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
          addq.l    #2,a1     ;Anzahl Åberspringen
          adda.l    (a0),a1   ;=Basis des Objekts
          move.w    6(a0),d1  ;Hîhe des Objekts
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
          lea       32(a1),a1 ;Palette Åberspringen
set_dbra: dbra      d0,set_loop

ohne_raster:        
          cmpi.w    #2,14(a6) ;Modus=Nur Palette? (4)
          beq.s     zu_gross1 ;(4)

          move.l    20(a6),-(sp)        ;x,y
          move.l    16(a6),-(sp)        ;scr_basis
          move.w    14(a6),-(sp)        ;modus
          move.w    6(a0),d0  ;Hîhe des Objekts
          subq.w    #1,d0
          move.w    d0,-(sp)  ;(Hîhe-1)->y2
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
          addq.l    #2,a0     ;Anzahl Åberspringen
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
end_draw_obj:       lea       24(sp),sp

zu_gross2:          movem.l   (sp)+,d0-d7/a0-a5
          unlk      a6
          rts       


;Diese Routine kopiert einen rechteckigen Bildschirmbereich von einem Platz
;zu einem anderen. Dabei werden öberlappungen NICHT beruecksichtigt.
;Der neue Bereich ersetzt den alten vollstaendig (replace).
;Als Bildschirmbreite wird die uebergebene genommen
;
;  30(a6).W: Breite einer Scanline in Pixel (Destination)
;  26(a6).L: Adresse des Bildschirms (Destination)
;  24(a6).W: y3 (Destination)
;  22(a6).W: x3 (Destination)
;  20(a6).W: Breite einer Scanline in Pixel (Source)
;  16(a6).L: Adresse des Bildschirms (Source)
;  14(a6).W: y2 (Source)
;  12(a6).W: x2 (Source)
;  10(a6).W: y1 (Source)
;   8(a6).W: x1 (Source)

mcode49:  link      a6,#-12
          movem.l   d0-d7/a0-a5,-(sp)

          move.w    8(a6),d0  ;Source X_Min
          move.w    10(a6),d1 ;Source Y_Min
          move.w    12(a6),d2 ;Source X_Max
          move.w    14(a6),d3 ;Source Y_Max
          move.w    22(a6),d4 ;Dest X_Min
          bpl.s     koord_ok5
          sub.w     d4,d0     ;neg->zu Source X_Min addieren
          move.w    d0,8(a6)  ;zurueck in Parameter
          moveq     #0,d4     ;Clippen
          move.w    d4,22(a6)
koord_ok5:          move.w    24(a6),d5 ;Dest Y_Min
          bpl.s     koord_ok6
          sub.w     d5,d1     ;neg->zu Source Y_Min addieren
          move.w    d1,10(a6) ;zurueck in Parameter
          moveq     #0,d5
          move.w    d5,24(a6)
koord_ok6:          move.w    d2,d7     ;Source X_Max
          move.w    d4,d6     ;Dest X_Min
          sub.w     d0,d7     ;Source X_Max-Source X_min=Breite-1
          add.w     d7,d6     ;=Dest X_Max
          move.w    30(a6),d7 ;Breite in Pixel (Dest)
          subq.w    #1,d7     ;bis einschliesslich hier
          sub.w     d6,d7     ;von 320 abziehen
          bpl.s     koord_ok7
          add.w     d7,d6     ;von Dest X_Max abziehen
          add.w     d7,d2     ;und von Source X_Max
          move.w    d2,12(a6) ;zurueck in Parameter
koord_ok7:          sub.w     d1,d3     ;Hoehe-1
          add.w     d3,d5     ;=Dest Y_Max
          move.w    #199,d3   ;Unterer Rand
          sub.w     d5,d3
          bpl.s     koord_ok8
          add.w     d3,d5     ;von Dest Y_Max abziehen
          add.w     d3,14(a6) ;von Source Y_Max abziehen
koord_ok8:          

          move.w    12(a6),d5 ;Source X_Max
          sub.w     8(a6),d5  ;-Source X_Min
          bmi       nichts_da8
          move.w    14(a6),d5 ;Source Y_Max
          sub.w     10(a6),d5 ;-Source Y_Min
          bmi       nichts_da8

          lea       custom,a5 ; Zeiger auf custom chips

;Maske aufbauen
          lea       blit_maske,a0
          clr.l     -4(a0)

          move.w    22(a6),d0 ;X1 Dest
          move.w    d0,d1
          lsr.w     #4,d0     ;/16
          move.w    d0,d2     ;Position der linken Maske

          lea       L0001(pc),a1
          andi.w    #15,d1
          add.w     d1,d1
          move.w    -2(a1,d1.w),d0      ;Maske eintragen
          not.w     d0
          move.w    d0,(a0)+

          move.w    12(a6),d0 ;X2
          sub.w     8(a6),d0
          add.w     22(a6),d0 ;X2 Dest
          move.w    d0,d3
          andi.w    #15,d3
          add.w     d3,d3
          lsr.w     #4,d0
          andi.w    #15,d1
          cmp.w     d0,d2     ;linke und rechte Maske gleich?
          bne.s     groesser_1

          move.w    0(a1,d3.w),d0       ;Maske holen
          and.w     d0,-2(a0) ;und nur eine Maske
          bra.s     fill_maske          ;und restliche Nullen hin

groesser_1:         sub.w     d2,d0
          subq.w    #1,d0     ;und noch eines weniger, wegen der linken Maske
          bra.s     maske_hin
set1:     move.w    #-1,(a0)+
maske_hin:          dbra      d0,set1

          move.w    0(a1,d3.w),d0
          move.w    d0,(a0)+

fill_maske:         clr.l     (a0)+

fertig_maske:       move.w    8(a6),d0  ; X1 (Source)
          lsr.w     #4,d0     ; /8
          lsl.w     #1,d0     ; WORD-align
          move.w    d0,x1_bb

          move.w    12(a6),d0 ;X2 Source
          lsr.w     #4,d0
          lsl.w     #1,d0
          move.w    d0,x2_bb

          move.w    22(a6),d0 ;X3 Dest
          lsr.w     #4,d0
          lsl.w     #1,d0
          move.w    d0,x3_bb

          move.w    12(a6),d0
          sub.w     8(a6),d0  ;Breite-1
          add.w     22(a6),d0 ;=X4 Dest
          lsr.w     #4,d0
          lsl.w     #1,d0
          move.w    d0,x4_bb

          move.w    x2_bb,d0
          sub.w     x1_bb,d0
          addq.w    #2,d0
          move.w    d0,quelle_breite    ;Breite der Quelle in BBs berechnen

          move.w    x4_bb,d0
          sub.w     x3_bb,d0
          addq.w    #2,d0
          move.w    d0,ziel_breite      ;Breite des Ziels in BBs berechnen

; Quell-Offset berechnen (D0.W)

          move.w    x1_bb,d0
          move.w    20(a6),d1 ; Breite einer line in Pixel (Source)
          lsr.w     #3,d1     ; D1: Source-Breite in Bytes
          move.w    d1,d2
          mulu.w    10(a6),d2 ; Mal Y1 (Source)
          lsl.w     #2,d2     ; * 4 Planes
          add.w     d2,d0     ; D0: Source-Offset

; Ziel-Offset berechnen (D1.W)

          move.w    x3_bb,d6
          move.w    30(a6),d1 ; Destination-Breite
          lsr.w     #3,d1     ; in Bytes
          mulu.w    24(a6),d1 ; Mal Y3 (Destination)
          lsl.w     #2,d1     ; * 4 Planes
          add.w     d6,d1     ; D1: Destination-Offset

; Source- und Zieladressen berechnen

          movea.l   16(a6),a0 ; Source-Adresse
          adda.w    d0,a0     ; Plus Source-Offset

          movea.l   26(a6),a1 ; Destination-Adresse
          adda.w    d1,a1     ; Plus Destination-Offset

          lea       blit_maske,a3       ;Ab hier steht die Maske

; Skew berechnen (D7.W)

          move.w    22(a6),d7 ;Dest X3
          andi.w    #15,d7
          move.w    8(a6),d6  ;Source X1
          andi.w    #15,d6
          sub.w     d6,d7     ;Skew

; Auswertung der Parameter:

          tst.w     d7        ;in welche Richtung schieben?
          bmi.s     nach_links

          move.w    quelle_breite,d0    ;Sind die Breiten gleich?
          cmp.w     ziel_breite,d0
          beq.s     starten
          blt.s     fall_2    ;Quellbreite < Zielbreite

;Zielbreite < Quellbreite:
          subq.l    #2,a3     ;Maske eins frÅher starten
          subq.l    #2,a1     ;Ziel eins frÅher starten
          move.w    quelle_breite,ziel_breite
          bra.s     starten

;Quellbreite < Zielbreite:
fall_2:   move.w    ziel_breite,quelle_breite
          bra.s     starten


nach_links:         move.w    quelle_breite,d0
          cmp.w     ziel_breite,d0
          beq.s     fall_5    ;Normal nach links schieben
          blt.s     fall_4

;Zielbreite < Quellbreite:
          subq.w    #2,a1
          subq.w    #2,a3
          move.w    quelle_breite,ziel_breite
          bra.s     starten

;Quellbreite < Zielbreite:
fall_4:   subq.w    #2,a1
          subq.w    #2,a3
          move.w    ziel_breite,quelle_breite
          bra.s     starten

;Quellbreite = Zielbreite:
fall_5:   addq.w    #2,quelle_breite    ;Fenster breiter machen
          addq.w    #2,ziel_breite
          subq.w    #2,a1     ;Aber ein Wort frÅher starten
          subq.w    #2,a3

starten:  

; Modulos berechnen (D4.W=Source, D5.W=Destination)

          move.w    20(a6),d4 ;Breite Source in Pixel
          lsr.w     #3,d4     ;Breite Source in Bytes/Plane
          sub.w     quelle_breite,d4

          move.w    30(a6),d5 ;Breite Destination in Pixel
          lsr.w     #3,d5     ;Breite Destination in Bytes/Plane
          sub.w     ziel_breite,d5


; BLTSIZE berechnen (D3.W)

          move.w    14(a6),d3 ; Y2 (Source)
          sub.w     10(a6),d3 ; minus Y1 (Source)
          addq.w    #1,d3     ; gibt Hîhe in Zeilen
          lsl.w     #(2+HSIZEBITS),d3   ; * 4 Planes, in HSIZEBITS schieben
          move.w    quelle_breite,d0
          lsr.w     #1,d0
          or.w      d0,d3     ; Breite in WORDs dazu


; Blitter-Register initialisieren

          bsr       WaitBlit

          move.w    ziel_breite,d0
          neg.w     d0
          move.w    d0,bltamod(a5)      ;Immer zurÅck auf dieselbe Maske
          move.w    d4,bltbmod(a5)      ;Source-Modulo
          move.w    d5,bltcmod(a5)      ;Destination-Modulo
          move.w    d5,bltdmod(a5)      ;Destination-Modulo

          move.l    #-1,bltafwm(a5)     ;Kein Maske bitte

ausdruck  equ BC0F_SRCA|BC0F_SRCB|BC0F_SRCC|BC0F_DEST

          move.w    #ausdruck|ABC|ABNC|NABC|NANBC,bltcon0(a5)

          andi.w    #15,d7    ;Skew eingrenzen
          ror.w     #16-BSHIFTSHIFT,d7  ; Source Shift Value (bits 15-12)
          move.w    d7,bltcon1(a5)

          move.l    a3,bltapt(a5)       ; Masken-Adresse
          move.l    a0,bltbpt(a5)       ; Source-Adresse
          move.l    a1,bltcpt(a5)       ; Destination-Adresse
          move.l    a1,bltdpt(a5)       ; Destination-Adresse

          move.w    d3,bltsize(a5)      ; Blit starten

          bsr       WaitBlit

nichts_da8:         movem.l   (sp)+,d0-d7/a0-a5
          unlk      a6
          rts       



; Auf Blitter warten

WaitBlit: btst      #6,dmaconr(a5)      ; DON'T CHANGE THIS ROUTINE!!!!!!
wait2:    btst      #6,dmaconr(a5)
          bne.s     wait2
          rts       


; First Word Masken

fwm:      .DC.w $ffff,$7fff,$3fff,$1fff,$0fff,$07ff,$03ff,$01ff
          .DC.w $00ff,$007f,$003f,$001f,$000f,$0007,$0003,$0001


; Last Word Masken

lwm:      .DC.w $0000,$8000,$c000,$e000,$f000,$f800,$fc00,$fe00
          .DC.w $ff00,$ff80,$ffc0,$ffe0,$fff0,$fff8,$fffc,$fffe


L0000:    .DC.w %0000000000000000
L0001:    .DC.w %1000000000000000,%1100000000000000,%1110000000000000,%1111000000000000
          .DC.w %1111100000000000,%1111110000000000,%1111111000000000,%1111111100000000
          .DC.w %1111111110000000,%1111111111000000,%1111111111100000,%1111111111110000
          .DC.w %1111111111111000,%1111111111111100,%1111111111111110,%1111111111111111

;Diese Routine kopiert einen rechteckigen Bildschirmbereich von einem Platz
;zu einem anderen. Dabei werden ‹berlappungen NICHT beruecksichtigt.
;Der neue Bereich ersetzt den alten vollstaendig (replace).
;Die Routine clippt den Zielbereich. Als Bildschirmbreite wird die
;uebergebene genommen, als Bildschirmhoehe wird 200 angenommen.


old_mcode49:        link      a6,#-12

          movem.l   d0-d7/a0-a5,-(sp)   ;Register retten

mcde49:   bsr       set_offsets         ;Die Offsets eintragen

          move.w    8(a6),d0  ;Source X_Min
          move.w    10(a6),d1 ;Source Y_Min
          move.w    12(a6),d2 ;Source X_Max
          move.w    14(a6),d3 ;Source Y_Max
          move.w    22(a6),d4 ;Dest X_Min
          bpl.s     koord_ok1
          sub.w     d4,d0     ;neg->zu Source X_Min addieren
          move.w    d0,8(a6)  ;zurueck in Parameter
          moveq     #0,d4     ;Clippen
          move.w    d4,22(a6)
koord_ok1:          move.w    24(a6),d5 ;Dest Y_Min
          bpl.s     koord_ok2
          sub.w     d5,d1     ;neg->zu Source Y_Min addieren
          move.w    d1,10(a6) ;zurueck in Parameter
          moveq     #0,d5
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
          move.w    d2,12(a6) ;zurueck in Parameter
koord_ok3:          sub.w     d1,d3     ;Hoehe-1
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

          moveq     #$000f,d5
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
          lsl.w     #1,d7     ;Breite einer Scanline in Byte
          sub.w     d3,d7     ;Breite einer Zeile in Bytes abziehen(?)
          move.w    d7,-8(a6) ;Offsets am Zeilenende merken
          move.w    -2(a6),d0 ;Dest X_Max
          move.w    -4(a6),d1 ;Dest Y_Max
          bsr       calc_ziel ;auch von hinten rein
          lsl.w     #1,d6     ;Breite in BBs->Breite in Bytes
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

;Tabelle fuer Objekte, die genau 2 BBs breit sind, und nach rechts geschoben
;werden, da es fuer die nachfolgenden Routine (L0037) nicht moeglich war an
;bestimmte Informationen heranzukommen.
rechts_klein:       .DC.l L0023b
          .DC.l L0025b
          .DC.l L0024b
          .DC.l L0026b

;Routinen fuer nur einen BB breite Objekte:
L0007:    .DC.l L0021
          .DC.l L0022
          .DC.l L0022
          .DC.l L0021

;Routinen fuer mindestens 2 BB breite Objekte
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
          and.w     d0,d6     ;linker und rechter Rand verknuepfen
          lea       nxt_scanline,a3
          btst      #5,d5     ;NFSR?
          bne       start_copy          ;Ja->Normal arbeiten
          lsr.w     #1,d5     ;nur noch mal 4
          andi.w    #%0000000000001100,d5
          movea.l   L0007(pc,d5.w),a4   ;Sonst andere Routine nehmen
start_copy:         move.w    14(a6),d5 ;y2
          sub.w     10(a6),d5 ;-y1=Hoehe in Pixeln-1
          addq.w    #1,d5     ;=Hoehe in Pixeln
          swap.w    d5
          move.w    -6(a6),d5 ;Breite des Zielblocks in BBs-2
          addq.l    #2,a0     ;Amiga, ans Ende des BB
          addq.l    #2,a1     ;-"-
          tst.w     d4        ;Skew=0?
;          beq       no_shift
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
;-> D0: X
;   D1: Y
;   A0: Quellrasteradresse
calc_quelle:        movea.l   16(a6),a0 ;Anfangsadresse Quellraster
          move.w    20(a6),d3 ;Breite Quelle in Pixel
          lsr.w     #1,d3     ;Breite in Bytes
          lsr.w     #4,d0     ;# des BBs berechnen
          lsl.w     #1,d0
          muls.w    d3,d1     ;Y Offset ausrechnen
          adda.l    d1,a0
          adda.w    d0,a0     ;und dazuaddieren
          rts       

;Routine berechnet die Startadresse des Zielrasters relativ zu:
;-> D0: X
;   D1: Y
;   A1: Zielrasteradresse
calc_ziel:          movea.l   26(a6),a1 ;Anfangsadresse Zielraster
          move.w    30(a6),d3 ;Breite Ziel in Pixel
          lsr.w     #1,d3     ;Breite in Bytes berechnen
          lsr.w     #4,d0     ;# des BBs berechnen
          lsl.w     #1,d0
          muls.w    d3,d1     ;Y Offset berechnen
          adda.l    d1,a1
          adda.w    d0,a1     ;und dazuaddieren
          rts       

;Diese Routine setzt die Offsets zu den BitPlanes in den Routinen
;neu. Sie wird von mcode49/50 verwendet.
set_offsets:        movem.l   d0-d7/a0-a6,-(sp)

          lea       offsets,a0
          move.w    20(a6),d0 ;Breite (Source) in Pixel
          lsr.w     #3,d0     ;Breite in Bytes/4
          moveq     #2,d7     ;3 Offsets
          moveq     #0,d1
off1:     add.w     d0,d1
          move.w    d1,(a0)+
          dbra      d7,off1

          move.w    30(a6),d0 ;Breite (Dest) in Pixel
          lsr.w     #3,d0     ;Breite in Bytes/4
          moveq     #2,d7     ;3 Offsets
          moveq     #0,d1
off2:     add.w     d0,d1
          move.w    d1,(a0)+
          dbra      d7,off2

          moveq     #5,d7     ;6 Offsettabellen
          lea       offsets,a0
          lea       off_tab,a1
          lea       L0021,a3
off_loop2:          move.w    (a0)+,d0  ;Offsetwert
off_loop: movea.w   (a1)+,a2
          cmpa.l    #0,a2     ;Ende der Tabelle?
          beq.s     ende_tab
          adda.l    a3,a2     ;relozieren
          move.w    d0,2(a2)  ;Offset eintragen
          bra.s     off_loop
ende_tab: dbra      d7,off_loop2

          movem.l   (sp)+,d0-d7/a0-a6
          rts       


;Tabelle zum EinfÅgen der Offsets direkt in den Programmcode
;Dabei ist folgende Reihenfolge einzuhalten:
;Source_Offset 2. Plane, 3. Plane, 4. Plane
;Dest_Offset   2. Plane, 3. Plane, 4. Plane
off_tab:  .DC.w os401-L0021
          .DC.w os402-L0021
          .DC.w os403-L0021
          .DC.w os404-L0021
          .DC.w os405-L0021
          .DC.w os406-L0021
          .DC.w os407-L0021
          .DC.w os408-L0021
          .DC.w os409-L0021
          .DC.w os4010-L0021
          .DC.w os4011-L0021
          .DC.w os4012-L0021
          .DC.w os4013-L0021
          .DC.w os4014-L0021
          .DC.w os4015-L0021
          .DC.w os4016-L0021
          .DC.w os4017-L0021
          .DC.w os4018-L0021
          .DC.w os4019-L0021
          .DC.w os4020-L0021
          .DC.w os4021-L0021
          .DC.w os4022-L0021
          .DC.w os4023-L0021
          .DC.w os4024-L0021
          .DC.w os4025-L0021
          .DC.w os4026-L0021
          .DC.w os4027-L0021
          .DC.w os4028-L0021
;               dc.w os4029-L0021       ;Ist ‹bersprungen worden
          .DC.w os4030-L0021
          .DC.w os4031-L0021
          .DC.w os4032-L0021
          .DC.w os4033-L0021
          .DC.w 0
          .DC.w os801-L0021
          .DC.w os802-L0021
          .DC.w os803-L0021
          .DC.w os804-L0021
          .DC.w os805-L0021
          .DC.w os806-L0021
          .DC.w os807-L0021
          .DC.w os808-L0021
          .DC.w os809-L0021
          .DC.w os8010-L0021
          .DC.w os8011-L0021
          .DC.w os8012-L0021
          .DC.w os8013-L0021
          .DC.w os8014-L0021
          .DC.w os8015-L0021
          .DC.w os8016-L0021
          .DC.w os8017-L0021
          .DC.w os8018-L0021
          .DC.w os8019-L0021
          .DC.w os8020-L0021
          .DC.w os8021-L0021
          .DC.w os8022-L0021
          .DC.w os8023-L0021
          .DC.w os8024-L0021
          .DC.w os8025-L0021
          .DC.w os8026-L0021
          .DC.w os8027-L0021
          .DC.w os8028-L0021
;               dc.w os8029-L0021       ;Ist ‹bersprungen worden
          .DC.w os8030-L0021
          .DC.w os8031-L0021
          .DC.w os8032-L0021
          .DC.w os8033-L0021
          .DC.w 0
          .DC.w os1201-L0021
          .DC.w os1202-L0021
          .DC.w os1203-L0021
          .DC.w os1204-L0021
          .DC.w os1205-L0021
          .DC.w os1206-L0021
          .DC.w os1207-L0021
          .DC.w os1208-L0021
          .DC.w os1209-L0021
          .DC.w os12010-L0021
          .DC.w os12011-L0021
          .DC.w os12012-L0021
          .DC.w os12013-L0021
          .DC.w os12014-L0021
          .DC.w os12015-L0021
          .DC.w os12016-L0021
          .DC.w os12017-L0021
          .DC.w os12018-L0021
          .DC.w os12019-L0021
          .DC.w os12020-L0021
          .DC.w os12021-L0021
          .DC.w os12022-L0021
          .DC.w os12023-L0021
          .DC.w os12024-L0021
          .DC.w os12025-L0021
          .DC.w os12026-L0021
          .DC.w os12027-L0021
          .DC.w os12028-L0021
;               dc.w os12029-L0021      ;Ist ‹bersprungen worden
          .DC.w os12030-L0021
          .DC.w os12031-L0021
          .DC.w os12032-L0021
          .DC.w os12033-L0021
          .DC.w 0
          .DC.w od401-L0021
          .DC.w od402-L0021
          .DC.w od403-L0021
          .DC.w od404-L0021
          .DC.w od405-L0021
          .DC.w od406-L0021
          .DC.w od407-L0021
          .DC.w od408-L0021
          .DC.w od409-L0021
          .DC.w od4010-L0021
          .DC.w od4011-L0021
          .DC.w od4012-L0021
          .DC.w od4013-L0021
          .DC.w od4014-L0021
          .DC.w od4015-L0021
          .DC.w od4016-L0021
          .DC.w od4017-L0021
          .DC.w od4018-L0021
          .DC.w od4019-L0021
          .DC.w od4020-L0021
          .DC.w od4021-L0021
          .DC.w od4022-L0021
          .DC.w od4023-L0021
          .DC.w od4024-L0021
          .DC.w od4025-L0021
          .DC.w od4026-L0021
          .DC.w od4027-L0021
          .DC.w od4028-L0021
          .DC.w od4029-L0021
          .DC.w od4030-L0021
          .DC.w od4031-L0021
          .DC.w od4032-L0021
          .DC.w od4033-L0021
          .DC.w od4034-L0021
          .DC.w od4035-L0021
          .DC.w od4036-L0021
          .DC.w od4037-L0021
          .DC.w od4038-L0021
          .DC.w od4039-L0021
          .DC.w od4040-L0021
          .DC.w 0
          .DC.w od801-L0021
          .DC.w od802-L0021
          .DC.w od803-L0021
          .DC.w od804-L0021
          .DC.w od805-L0021
          .DC.w od806-L0021
          .DC.w od807-L0021
          .DC.w od808-L0021
          .DC.w od809-L0021
          .DC.w od8010-L0021
          .DC.w od8011-L0021
          .DC.w od8012-L0021
          .DC.w od8013-L0021
          .DC.w od8014-L0021
          .DC.w od8015-L0021
          .DC.w od8016-L0021
          .DC.w od8017-L0021
          .DC.w od8018-L0021
          .DC.w od8019-L0021
          .DC.w od8020-L0021
          .DC.w od8021-L0021
          .DC.w od8022-L0021
          .DC.w od8023-L0021
          .DC.w od8024-L0021
          .DC.w od8025-L0021
          .DC.w od8026-L0021
          .DC.w od8027-L0021
          .DC.w od8028-L0021
          .DC.w od8029-L0021
          .DC.w od8030-L0021
          .DC.w od8031-L0021
          .DC.w od8032-L0021
          .DC.w od8033-L0021
          .DC.w od8034-L0021
          .DC.w od8035-L0021
          .DC.w od8036-L0021
          .DC.w od8037-L0021
          .DC.w od8038-L0021
          .DC.w od8039-L0021
          .DC.w od8040-L0021
          .DC.w 0
          .DC.w od1201-L0021
          .DC.w od1202-L0021
          .DC.w od1203-L0021
          .DC.w od1204-L0021
          .DC.w od1205-L0021
          .DC.w od1206-L0021
          .DC.w od1207-L0021
          .DC.w od1208-L0021
          .DC.w od1209-L0021
          .DC.w od12010-L0021
          .DC.w od12011-L0021
          .DC.w od12012-L0021
          .DC.w od12013-L0021
          .DC.w od12014-L0021
          .DC.w od12015-L0021
          .DC.w od12016-L0021
          .DC.w od12017-L0021
          .DC.w od12018-L0021
          .DC.w od12019-L0021
          .DC.w od12020-L0021
          .DC.w od12021-L0021
          .DC.w od12022-L0021
          .DC.w od12023-L0021
          .DC.w od12024-L0021
          .DC.w od12025-L0021
          .DC.w od12026-L0021
          .DC.w od12027-L0021
          .DC.w od12028-L0021
          .DC.w od12029-L0021
          .DC.w od12030-L0021
          .DC.w od12031-L0021
          .DC.w od12032-L0021
          .DC.w od12033-L0021
          .DC.w od12034-L0021
          .DC.w od12035-L0021
          .DC.w od12036-L0021
          .DC.w od12037-L0021
          .DC.w od12038-L0021
          .DC.w od12039-L0021
          .DC.w od12040-L0021
          .DC.w 0


;Spezialroutine falls Skew=0:
no_shift: lea       no_shift1,a4        ;Bei naechster Scanline diese Routine
          move.l    d6,d0     ;Maske retten
          swap.w    d6
          move.w    d6,d7     ;obere Maske->D7
          swap.w    d7
          move.w    d6,d7     ;->D7
          move.w    d0,d6     ;untere Maske->D6

no_shift1:          



          move.l    -(a0),d0  ;Plane 2 und 3 holen (12)
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
bb_dbra2: dbra      d5,bb_mitte2        ;zaehlt bis auf 0 runter

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

os401:    move.w    40(a0),d0 ;so wie oben
          rol.w     d4,d0
od401:    move.w    40(a1),d3
          eor.w     d3,d0
          and.w     d6,d0
          eor.w     d3,d0
od402:    move.w    d0,40(a1)

os801:    move.w    80(a0),d0
          rol.w     d4,d0
od801:    move.w    80(a1),d2
          eor.w     d2,d0
          and.w     d6,d0
          eor.w     d2,d0
od802:    move.w    d0,80(a1)

os1201:   move.w    120(a0),d0
          rol.w     d4,d0
od1201:   move.w    120(a1),d1
          eor.w     d1,d0
          and.w     d6,d0
          eor.w     d1,d0
od1202:   move.w    d0,120(a1)
          jmp       (a3)

L0022:    move.w    -(a0),d0
          ror.w     d4,d0
          move.w    -(a1),d7
          eor.w     d7,d0
          and.w     d6,d0
          eor.w     d7,d0
          move.w    d0,(a1)

os402:    move.w    40(a0),d0 ;so wie oben
          ror.w     d4,d0
od403:    move.w    40(a1),d3
          eor.w     d3,d0
          and.w     d6,d0
          eor.w     d3,d0
od404:    move.w    d0,40(a1)

os802:    move.w    80(a0),d0
          ror.w     d4,d0
od803:    move.w    80(a1),d2
          eor.w     d2,d0
          and.w     d6,d0
          eor.w     d2,d0
od804:    move.w    d0,80(a1)

os1202:   move.w    120(a0),d0
          ror.w     d4,d0
od1203:   move.w    120(a1),d1
          eor.w     d1,d0
          and.w     d6,d0
          eor.w     d1,d0
od1204:   move.w    d0,120(a1)
          jmp       (a3)

L0023:    move.w    -(a0),d7  ;Plane aus Objekt holen
os403:    move.w    40(a0),d3
os803:    move.w    80(a0),d2 ;Jeweils -(a0)
os1203:   move.w    120(a0),d1
          swap.w    d7
          swap.w    d3
          swap.w    d2
          swap.w    d1
L0024:    move.w    -(a0),d7  ;naechste Plane aus Objekt
os404:    move.w    40(a0),d3
os804:    move.w    80(a0),d2 ;Jeweils -(a0)
os1204:   move.w    120(a0),d1
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
          move.w    d0,(a1)   ;und zurueck auf Screen

          move.l    d3,d0     ;ins Schieberegister
          ror.l     d4,d0     ;runterschieben
od405:    move.w    40(a1),d3 ;Screen holen (wie oben)
          eor.w     d3,d0
          and.w     d6,d0
          eor.w     d3,d0
od406:    move.w    d0,40(a1) ;und zurueck auf Screen

          move.l    d2,d0     ;ins Schieberegister
          ror.l     d4,d0     ;runterschieben
od805:    move.w    80(a1),d2 ;Screen holen
          eor.w     d2,d0
          and.w     d6,d0
          eor.w     d2,d0
od806:    move.w    d0,80(a1) ;und zurueck auf Screen

          move.l    d1,d0     ;ins Schieberegister
          ror.l     d4,d0     ;runterschieben
od1205:   move.w    120(a1),d1          ;Screen holen
          eor.w     d1,d0
          and.w     d6,d0
          eor.w     d1,d0
od1206:   move.w    d0,120(a1)          ;und zurueck auf Screen

          jmp       (a3)

L0025:    move.w    -(a0),d7  ;Plane aus Objekt holen
os405:    move.w    40(a0),d3
os805:    move.w    80(a0),d2 ;Jeweils -(a0)
os1205:   move.w    120(a0),d1
          swap.w    d7
          swap.w    d3
          swap.w    d2
          swap.w    d1
L0026:    move.w    -(a0),d7
os406:    move.w    40(a0),d3 ;Jeweils -(a0)
os806:    move.w    80(a0),d2
os1206:   move.w    120(a0),d1
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
od407:    move.w    40(a1),d3 ;Screen holen (wie oben)
          eor.w     d3,d0
          and.w     d6,d0
          eor.w     d3,d0
od408:    move.w    d0,40(a1)

          move.l    d2,d0     ;ins Schieberegister
          swap.w    d2
          rol.l     d4,d0     ;schieben
od807:    move.w    80(a1),d2 ;Screen holen
          eor.w     d2,d0
          and.w     d6,d0
          eor.w     d2,d0
od808:    move.w    d0,80(a1)

          move.l    d1,d0     ;ins Schieberegister
          swap.w    d1
          rol.l     d4,d0     ;schieben
od1207:   move.w    120(a1),d1          ;Screen holen
          eor.w     d1,d0
          and.w     d6,d0
          eor.w     d1,d0
od1208:   move.w    d0,120(a1)
          jmp       (a3)

L0023b:   move.w    -(a0),d7
os407:    move.w    40(a0),d3
os807:    move.w    80(a0),d2 ;Jeweils -(a0)
os1207:   move.w    120(a0),d1
          swap.w    d7
          swap.w    d3
          swap.w    d2
          swap.w    d1
L0024b:   move.w    -(a0),d7
os408:    move.w    40(a0),d3
os808:    move.w    80(a0),d2 ;Jeweils -(a0)
os1208:   move.w    120(a0),d1
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

od409:    move.w    40(a1),d0 ;wie oben
          eor.w     d0,d3
          and.w     d6,d3
          eor.w     d0,d3
od4010:   move.w    d3,40(a1)

od809:    move.w    80(a1),d0
          eor.w     d0,d2
          and.w     d6,d2
          eor.w     d0,d2
od8010:   move.w    d2,80(a1)

od1209:   move.w    120(a1),d0
          eor.w     d0,d1
          and.w     d6,d1
          eor.w     d0,d1
od12010:  move.w    d1,120(a1)
          bra       L0037     ;und dort geht's weiter

L0025b:   move.w    -(a0),d7
os409:    move.w    40(a0),d3 ;Jeweils -(a0)
os809:    move.w    80(a0),d2
os1209:   move.w    120(a0),d1
          swap.w    d7
          swap.w    d3
          swap.w    d2
          swap.w    d1
L0026b:   move.w    -(a0),d7
os4010:   move.w    40(a0),d3
os8010:   move.w    80(a0),d2 ;Jeweils -(a0)
os12010:  move.w    120(a0),d1
          rol.l     d4,d7
          rol.l     d4,d3
          rol.l     d4,d2
          rol.l     d4,d1

          move.w    -(a1),d0
          eor.w     d0,d7
          and.w     d6,d7
          eor.w     d0,d7
          move.w    d7,(a1)

od4011:   move.w    40(a1),d0 ;wie oben
          eor.w     d0,d3
          and.w     d6,d3
          eor.w     d0,d3
od4012:   move.w    d3,40(a1)

od8011:   move.w    80(a1),d0
          eor.w     d0,d2
          and.w     d6,d2
          eor.w     d0,d2
od8012:   move.w    d2,80(a1)

od12011:  move.w    120(a1),d0
          eor.w     d0,d1
          and.w     d6,d1
          eor.w     d0,d1
od12012:  move.w    d1,120(a1)
          bra       L0037     ;dort geht's weiter

L002B:    move.w    -(a0),d7
os4011:   move.w    40(a0),d3
os8011:   move.w    80(a0),d2 ;Jeweils -(a0)
os12011:  move.w    120(a0),d1
          swap.w    d7
          swap.w    d3
          swap.w    d2
          swap.w    d1

          move.l    d7,d0
          ror.l     d4,d0
          move.w    d0,-(a1)

          move.l    d3,d0
          ror.l     d4,d0
od4013:   move.w    d0,40(a1) ;wie oben

          move.l    d2,d0
          ror.l     d4,d0
od8013:   move.w    d0,80(a1)

          move.l    d1,d0
          ror.l     d4,d0
od12013:  move.w    d0,120(a1)

          jmp       (a3)

L002C:    move.w    -(a0),d7
os4012:   move.w    40(a0),d3 ;Jeweils -(a0)
os8012:   move.w    80(a0),d2
os12012:  move.w    120(a0),d1

          move.l    d7,d0
          swap.w    d7
          rol.l     d4,d0
          move.w    d0,-(a1)

          move.l    d3,d0
          swap.w    d3
          rol.l     d4,d0
od4014:   move.w    d0,40(a1) ;wie oben

          move.l    d2,d0
          swap.w    d2
          rol.l     d4,d0
od8014:   move.w    d0,80(a1)

          move.l    d1,d0
          swap.w    d1
          rol.l     d4,d0
od12014:  move.w    d0,120(a1)
          jmp       (a3)

L002F:    dbra      d5,L002B

          move.w    -(a0),d7
os4013:   move.w    40(a0),d3 ;Jeweils -(a0)
os8013:   move.w    80(a0),d2
os12013:  move.w    120(a0),d1
          ror.l     d4,d7
          ror.l     d4,d3
          ror.l     d4,d2
          ror.l     d4,d1
          bra       L0037

L0030:    dbra      d5,L002C

          move.w    -(a0),d7
os4014:   move.w    40(a0),d3 ;Jeweils -(a0)
os8014:   move.w    80(a0),d2
os12014:  move.w    120(a0),d1
          rol.l     d4,d7
          rol.l     d4,d3
          rol.l     d4,d2
          rol.l     d4,d1
          bra       L0038

L0033:    cmpi.w    #1,d5     ;Nur bis 0 runterzaehlen
          beq.s     spezial   ;bei eins Spezialroutine
          dbra      d5,L002B
          bra       L0037

spezial:  move.w    -(a0),d7
os4015:   move.w    40(a0),d3 ;Jeweils -(a0)
os8015:   move.w    80(a0),d2
os12015:  move.w    120(a0),d1
          swap.w    d7
          swap.w    d3
          swap.w    d2
          swap.w    d1
          ror.l     d4,d7
          ror.l     d4,d3
          ror.l     d4,d2
          ror.l     d4,d1
          move.w    d7,-(a1)
od4015:   move.w    d3,40(a1) ;Jeweils -(a1)
od8015:   move.w    d2,80(a1)
od12015:  move.w    d1,120(a1)
          bra.s     L0037

L0034:    cmpi.w    #1,d5
          beq.s     spezial2
          dbra      d5,L002C
          bra.s     L0037

spezial2: move.w    -(a0),d7
os4016:   move.w    40(a0),d3 ;Jeweils -(a0)
os8016:   move.w    80(a0),d2
os12016:  move.w    120(a0),d1
          rol.l     d4,d7
          rol.l     d4,d3
          rol.l     d4,d2
          rol.l     d4,d1
          move.w    d7,-(a1)
od4016:   move.w    d3,40(a1) ;Jeweils -(a1)
od8016:   move.w    d2,80(a1)
od12016:  move.w    d1,120(a1)

L0037:    swap.w    d7
          swap.w    d3
          swap.w    d2
          swap.w    d1
L0038:    swap.w    d6
          move.w    -(a1),d0  ;Screen holen
          eor.w     d0,d7
          and.w     d6,d7
          eor.w     d0,d7
          move.w    d7,(a1)   ;und zurueck auf Screen

od4017:   move.w    40(a1),d0 ;Screen holen (wie oben)
          eor.w     d0,d3
          and.w     d6,d3
          eor.w     d0,d3
od4018:   move.w    d3,40(a1) ;und zurueck auf Screen

od8017:   move.w    80(a1),d0 ;Screen holen
          eor.w     d0,d2
          and.w     d6,d2
          eor.w     d0,d2
od8018:   move.w    d2,80(a1) ;und zurueck auf Screen

od12017:  move.w    120(a1),d0          ;Screen holen
          eor.w     d0,d1
          and.w     d6,d1
          eor.w     d0,d1
od12018:  move.w    d1,120(a1)          ;und zurueck auf Screen

L0039:    swap.w    d6
nxt_scan2:          move.w    -6(a6),d5 ;Breite in BBs holen
nxt_scanline:       swap.w    d5
          subq.w    #1,d5     ;Eine Scanline weniger
          beq.s     nichts_da7
          swap.w    d5        ;Wieder den BB-Zaehler
          adda.w    -8(a6),a0 ;zur naechsten Scanline
          adda.w    -10(a6),a1          ;-'-
          addq.l    #2,a0     ;Amiga
          addq.l    #2,a1
          jmp       (a4)      ;und von vorne starten
nichts_da7:         movem.l   (sp)+,d0-d7/a0-a5
          unlk      a6
          rts                 ;und schultz

;Diese Routine kopiert einen rechteckigen Bildschirmbereich von einem Platz
;zu einem anderen. Dabei werden ‹berlappungen NICHT beruecksichtigt.
;Der neue Bereich wird in den alten eingeknuepft, d.h. dort wo der neue
;Bereich Hintergrundfarbe hat, scheint der alte Bereich durch. (transparent)
;Diese Routine clippt den Zielbereich. Als Breite werden die uebergebene, als
;Hoehe 200 angenommen.
;Diese Routine ist ungefaehr 6.8 mal schneller als Line_A #7 ohne Blitter
;Line_A #7 mit Blitter braucht ungefaehr 85% der Rechenzeit dieser Routine.
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

          bsr       set_offsets

          move.w    8(a6),d0  ;Source X_Min
          move.w    10(a6),d1 ;Source Y_Min
          move.w    12(a6),d2 ;Source X_Max
          move.w    14(a6),d3 ;Source Y_Max
          move.w    22(a6),d4 ;Dest X_Min
          bpl.s     koord_ok11
          sub.w     d4,d0     ;neg->zu Source X_Min addieren
          move.w    d0,8(a6)  ;zurueck in Parameter
          moveq     #0,d4     ;Clippen
          move.w    d4,22(a6)
koord_ok11:         move.w    24(a6),d5 ;Dest Y_Min
          bpl.s     koord_ok22
          sub.w     d5,d1     ;neg->zu Source Y_Min addieren
          move.w    d1,10(a6) ;zurueck in Parameter
          moveq     #0,d5
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
          move.w    d2,12(a6) ;zurueck in Parameter
koord_ok33:         sub.w     d1,d3     ;Hoehe-1
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

          moveq     #$000f,d5
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
          lsl.w     #1,d7
          sub.w     d3,d7     ;Breite einer Zeile in Bytes abziehen(?)
          move.w    d7,-8(a6) ;Offsets am Zeilenende merken
          move.w    -2(a6),d0 ;Dest X_Max
          move.w    -4(a6),d1 ;Dest Y_Max
          bsr       calc_ziel ;auch von hinten rein
          lsl.w     #1,d6     ;Breite in BBs->Breite in Bytes
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

;Tabelle fuer Objekte, die genau 2 BBs breit sind, und nach rechts geschoben
;werden, da es fuer die nachfolgenden Routine (L0037) nicht moeglich war an
;bestimmte Informationen heranzukommen.
rechts_kleint:      .DC.l L0023bt
          .DC.l L0025bt
          .DC.l L0024bt
          .DC.l L0026bt

;Routinen fuer nur einen BB breite Objekte:
L0007t:   .DC.l L0021t
          .DC.l L0022t
          .DC.l L0022t
          .DC.l L0021t

;Routinen fuer mindestens 2 BB breite Objekte
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
          and.w     d0,d6     ;linker und rechter Rand verknuepfen
          lea       nxt_scanline,a3
          btst      #5,d5     ;NFSR?
          bne       start_trans         ;Ja->Normal arbeiten
          lsr.w     #1,d5     ;nur noch mal 4
          andi.w    #%0000000000001100,d5
          movea.l   L0007t(pc,d5.w),a4  ;Sonst andere Routine nehmen
start_trans:        move.w    14(a6),d5 ;y2
          sub.w     10(a6),d5 ;-y1=Hoehe in Pixeln-1
          addq.w    #1,d5     ;=Hoehe in Pixeln
          swap.w    d5
          move.w    -6(a6),d5 ;Breite des Zielblocks in BBs-2
;               addq.l    #8,a0     ;Ans Ende des BB gehen
;               addq.l    #8,a1     ;-'-
          addq.l    #2,a0     ;Amiga
          addq.l    #2,a1
;               tst.w     d4        ;Skew=0?
;               beq       no_shift2 ;ja, dann Spezial-Routine nehmen
          jmp       (a4)      ;und Plane bearbeiten

;Spezialroutine falls Skew=0:
no_shift2:          lea       no_shift3,a4        ;Bei naechster Scanline diese Routine
          move.l    d6,d0     ;Maske retten
          swap.w    d6
          move.w    d6,d7     ;obere Maske->D7
          swap.w    d7
          move.w    d6,d7     ;->D7
          move.w    d0,d6     ;untere Maske->D6

no_shift3:          move.l    -(a0),d0  ;Plane 2 und 3 holen (12)
          move.l    -(a0),d1  ;Plane 0 und 1 holen (12)
          move.l    d0,d4     ;fuer Maske      ( 4)
          or.l      d1,d4     ;              ( 8)
          swap.w    d0        ;              ( 4)
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
          move.l    d0,d4     ;fuer Maske      ( 4)
          or.l      d1,d4     ;              ( 8)
          swap.w    d0        ;              ( 4)
          swap.w    d1
          or.l      d0,d4     ;Maske in d4    ( 8)
          or.l      d1,d4
          swap.w    d0
          swap.w    d1
          not.l     d4        ;Maske invertieren
          beq.s     klopfen2
          move.l    -4(a1),d2 ;Screen holen   (12)
          move.l    -8(a1),d3
          and.l     d4,d2     ;              ( 8)
          or.l      d2,d0     ;              ( 8)
          and.l     d4,d3     ;              ( 8)
          or.l      d3,d1     ;              ( 8)
klopfen2: move.l    d0,-(a1)
          move.l    d1,-(a1)
bb_dbra:  dbra      d5,bb_mitte         ;zaehlt bis auf 0 runter

bb_rechts:          move.l    -(a0),d0  ;Plane 2 und 3 holen (12)
          move.l    -(a0),d1  ;Plane 0 und 1 holen (12)
          move.l    d0,d4     ;fuer Maske      ( 4)
          or.l      d1,d4     ;              ( 8)
          swap.w    d0        ;              ( 4)
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
os4017:   move.w    40(a0),d3
os8017:   move.w    80(a0),d2 ;jeweils -(a0)
os12017:  move.w    120(a0),d1
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

od4019:   move.w    40(a1),d0 ;wie oben
          eor.w     d0,d3
          and.w     d6,d3
          and.w     d1,d3
          eor.w     d0,d3
od4020:   move.w    d3,40(a1)

od8019:   move.w    80(a1),d0
          eor.w     d0,d2
          and.w     d6,d2
          and.w     d1,d2
          eor.w     d0,d2
od8020:   move.w    d2,80(a1)

          move.w    a5,d7     ;d1 holen

od12019:  move.w    120(a1),d0
          eor.w     d0,d7
          and.w     d6,d7
          and.w     d1,d7
          eor.w     d0,d7
od12020:  move.w    d7,120(a1)
          jmp       (a3)

L0022t:   move.w    -(a0),d7
os4018:   move.w    40(a0),d3 ;Jeweils -(a0)
os8018:   move.w    80(a0),d2
os12018:  move.w    120(a0),d1
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

od4021:   move.w    40(a1),d0 ;wie oben
          eor.w     d0,d3
          and.w     d6,d3
          and.w     d1,d3
          eor.w     d0,d3
od4022:   move.w    d3,40(a1)

od8021:   move.w    80(a1),d0
          eor.w     d0,d2
          and.w     d6,d2
          and.w     d1,d2
          eor.w     d0,d2
od8022:   move.w    d2,80(a1)

          move.w    a5,d7     ;d1 holen
od12021:  move.w    120(a1),d0
          eor.w     d0,d7
          and.w     d6,d7
          and.w     d1,d7
          eor.w     d0,d7
od12022:  move.w    d7,120(a1)
          jmp       (a3)

L0023t:   move.w    -(a0),d7  ;Plane aus Objekt holen
os4019:   move.w    40(a0),d3
os8019:   move.w    80(a0),d2 ;Jeweils -(a0)
os12019:  move.w    120(a0),d1
          swap.w    d7
          swap.w    d3
          swap.w    d2
          swap.w    d1
L0024t:   move.w    -(a0),d7  ;naechste Plane aus Objekt
os4020:   move.w    40(a0),d3
os8020:   move.w    80(a0),d2 ;Jeweils -(a0)
os12020:  move.w    120(a0),d1
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
;                     not.w   D1                    ;Maske invertieren
          move.w    -(a1),d7  ;Screen holen
          eor.w     d7,d0
          and.w     d6,d0     ;ausblenden
          and.w     d1,d0     ;Maskieren
          eor.w     d7,d0
          move.w    d0,(a1)   ;und zurueck auf Screen

          move.l    d3,d0     ;ins Schieberegister
          ror.l     d4,d0     ;runterschieben
od4023:   move.w    40(a1),d3 ;Screen holen (wie oben)
          eor.w     d3,d0
          and.w     d6,d0
          and.w     d1,d0     ;Maskieren
          eor.w     d3,d0
od4024:   move.w    d0,40(a1) ;und zurueck auf Screen

          move.l    d2,d0     ;ins Schieberegister
          ror.l     d4,d0     ;runterschieben
od8023:   move.w    80(a1),d2 ;Screen holen
          eor.w     d2,d0
          and.w     d6,d0
          and.w     d1,d0
          eor.w     d2,d0
od8024:   move.w    d0,80(a1) ;und zurueck auf Screen

          move.l    a5,d0     ;ins Schieberegister
          ror.l     d4,d0     ;runterschieben
od12023:  move.w    120(a1),d2          ;Screen holen
          eor.w     d2,d0
          and.w     d6,d0
          and.w     d1,d0
          eor.w     d2,d0
od12024:  move.w    d0,120(a1)          ;und zurueck auf Screen

          move.l    a5,d1     ;und richtiges d1 rausholen

          jmp       (a3)

L0025t:   move.w    -(a0),d7  ;Plane aus Objekt holen
os4021:   move.w    40(a0),d3
os8021:   move.w    80(a0),d2 ;Jeweils -(a0)
os12021:  move.w    120(a0),d1
          swap.w    d7
          swap.w    d3
          swap.w    d2
          swap.w    d1
L0026t:   move.w    -(a0),d7
os4022:   move.w    40(a0),d3
os8022:   move.w    80(a0),d2 ;Jeweils -(a0)
os12022:  move.w    120(a0),d1

          movea.l   d1,a5     ;d1 retten
          or.l      d2,d1
          or.l      d3,d1     ;Maske berechnen
          or.l      d7,d1

          move.l    d7,d0     ;ins Schieberegister
          swap.w    d7
          rol.l     d4,d0     ;schieben
          rol.l     d4,d1     ;Maske hinschieben
;                     not.w   D1                    ;Maske invertieren
          move.w    -(a1),d7  ;Screen holen
          eor.w     d7,d0
          and.w     d6,d0
          and.w     d1,d0
          eor.w     d7,d0
          move.w    d0,(a1)

          move.l    d3,d0     ;ins Schieberegister
          swap.w    d3
          rol.l     d4,d0     ;schieben
od4025:   move.w    40(a1),d3 ;Screen holen (wie oben)
          eor.w     d3,d0
          and.w     d6,d0
          and.w     d1,d0
          eor.w     d3,d0
od4026:   move.w    d0,40(a1)

          move.l    d2,d0     ;ins Schieberegister
          swap.w    d2
          rol.l     d4,d0     ;schieben
od8025:   move.w    80(a1),d2 ;Screen holen
          eor.w     d2,d0
          and.w     d6,d0
          and.w     d1,d0
          eor.w     d2,d0
od8026:   move.w    d0,80(a1)

          move.l    a5,d0     ;ins Schieberegister
          rol.l     d4,d0     ;schieben
od12025:  move.w    120(a1),d2          ;Screen holen
          eor.w     d2,d0
          and.w     d6,d0
          and.w     d1,d0
          eor.w     d2,d0
od12026:  move.w    d0,120(a1)

          move.l    a5,d1     ;d1 rausholen
          swap.w    d1        ;und auch noch umdrehen
          jmp       (a3)

L0023bt:  move.w    -(a0),d7
os4023:   move.w    40(a0),d3 ;Jeweils -(a0)
os8023:   move.w    80(a0),d2
os12023:  move.w    120(a0),d1
          swap.w    d7
          swap.w    d3
          swap.w    d2
          swap.w    d1
L0024bt:  move.w    -(a0),d7
os4024:   move.w    40(a0),d3 ;Jeweils -(a0)
os8024:   move.w    80(a0),d2
os12024:  move.w    120(a0),d1
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
;                    not.w   D1             ;und invertieren

          move.w    -(a1),d0
          eor.w     d0,d7
          and.w     d6,d7
          and.w     d1,d7
          eor.w     d0,d7
          move.w    d7,(a1)

od4027:   move.w    40(a1),d0 ;Wie oben
          eor.w     d0,d3
          and.w     d6,d3
          and.w     d1,d3
          eor.w     d0,d3
od4028:   move.w    d3,40(a1)

od8027:   move.w    80(a1),d0
          eor.w     d0,d2
          and.w     d6,d2
          and.w     d1,d2
          eor.w     d0,d2
od8028:   move.w    d2,80(a1)

          move.w    a5,d2     ;altes d1 holen

od12027:  move.w    120(a1),d0
          eor.w     d0,d2
          and.w     d6,d2
          and.w     d1,d2
          eor.w     d0,d2
od12028:  move.w    d2,120(a1)

          move.l    a5,d1     ;und nochmal her damit

          bra       L0037t    ;und dort geht's weiter

L0025bt:  move.w    -(a0),d7
os4025:   move.w    40(a0),d3 ;Jeweils -(a0)
os8025:   move.w    80(a0),d2
os12025:  move.w    120(a0),d1
          swap.w    d7
          swap.w    d3
          swap.w    d2
          swap.w    d1
L0026bt:  move.w    -(a0),d7
os4026:   move.w    40(a0),d3 ;Jeweils -(a0)
os8026:   move.w    80(a0),d2
os12026:  move.w    120(a0),d1
          rol.l     d4,d7
          rol.l     d4,d3
          rol.l     d4,d2
          rol.l     d4,d1

          movea.l   d1,a5     ;d1 retten
          or.w      d2,d1
          or.w      d3,d1     ;Maske berechnen
          or.w      d7,d1
;                   not.w   D1              ;und invertieren

          move.w    -(a1),d0
          eor.w     d0,d7
          and.w     d6,d7
          and.w     d1,d7
          eor.w     d0,d7
          move.w    d7,(a1)

od4029:   move.w    40(a1),d0 ;wie oben
          eor.w     d0,d3
          and.w     d6,d3
          and.w     d1,d3
          eor.w     d0,d3
od4030:   move.w    d3,40(a1)

od8029:   move.w    80(a1),d0
          eor.w     d0,d2
          and.w     d6,d2
          and.w     d1,d2
          eor.w     d0,d2
od8030:   move.w    d2,80(a1)

          move.w    a5,d2
od12029:  move.w    120(a1),d0
          eor.w     d0,d2
          and.w     d6,d2
          and.w     d1,d2
          eor.w     d0,d2
od12030:  move.w    d2,120(a1)

          bra       L0037t    ;dort geht's weiter

L002Bt:   move.w    -(a0),d7
os4027:   move.w    40(a0),d3 ;Jeweils -(a0)
os8027:   move.w    80(a0),d2
os12027:  move.w    120(a0),d1
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
od4031:   and.w     d1,40(a1) ;wie oben
od4032:   or.w      d0,40(a1)

          move.l    d2,d0
          ror.l     d4,d0
od8031:   and.w     d1,80(a1)
od8032:   or.w      d0,80(a1)

          move.l    a5,d0
          ror.l     d4,d0
od12031:  and.w     d1,120(a1)
od12032:  or.w      d0,120(a1)

          move.l    a5,d1     ;und altes d1 rausholen
          jmp       (a3)

L002Ct:   move.w    -(a0),d7
os4028:   move.w    40(a0),d3 ;Jeweils -(a0)
os8028:   move.w    80(a0),d2
os12028:  move.w    120(a0),d1

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
          or.w      d0,(a1)   ;und einknuepfen

          move.l    d3,d0     ;ins Schieberegister
          swap.w    d3
          rol.l     d4,d0
od4033:   and.w     d1,40(a1) ;wie oben
od4034:   or.w      d0,40(a1)

          move.l    d2,d0     ;ins Schieberegister
          swap.w    d2
          rol.l     d4,d0
od8033:   and.w     d1,80(a1)
od8034:   or.w      d0,80(a1)

          move.l    a5,d0     ;ins Schieberegister
          rol.l     d4,d0
od12033:  and.w     d1,120(a1)
od12034:  or.w      d0,120(a1)

          move.l    a5,d1     ;altes d1 holen
          swap.w    d1        ;und auch umdrehen
          jmp       (a3)

L002Ft:   dbra      d5,L002Bt

          move.w    -(a0),d7
os4030:   move.w    40(a0),d3 ;Jeweils -(a0)
os8030:   move.w    80(a0),d2
os12030:  move.w    120(a0),d1
          ror.l     d4,d7
          ror.l     d4,d3
          ror.l     d4,d2
          ror.l     d4,d1
          bra       L0037t

L0030t:   dbra      d5,L002Ct

          move.w    -(a0),d7
os4031:   move.w    40(a0),d3 ;Jeweils -(a0)
os8031:   move.w    80(a0),d2
os12031:  move.w    120(a0),d1
          rol.l     d4,d7
          rol.l     d4,d3
          rol.l     d4,d2
          rol.l     d4,d1
          bra       L0038t

L0033t:   cmpi.w    #1,d5     ;Nur bis 0 runterzaehlen
          beq.s     spezialt  ;bei eins Spezialroutine
          dbra      d5,L002Bt
          bra       L0037t

spezialt: move.w    -(a0),d7
os4032:   move.w    40(a0),d3 ;Jeweils -(a0)
os8032:   move.w    80(a0),d2
os12032:  move.w    120(a0),d1
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
od4035:   and.w     d1,40(a1) ;wie oben
od4036:   or.w      d3,40(a1)
od8035:   and.w     d1,80(a1)
od8036:   or.w      d2,80(a1)
od12035:  and.w     d1,120(a1)
          move.l    a5,d1     ;Altes D1 holen
od12036:  or.w      d1,120(a1)

          bra.s     L0037t

L0034t:   cmpi.w    #1,d5
          beq.s     spezial2t
          dbra      d5,L002Ct
          bra.s     L0037t

spezial2t:          
          move.w    -(a0),d7
os4033:   move.w    40(a0),d3 ;Jeweils -(a0)
os8033:   move.w    80(a0),d2
os12033:  move.w    120(a0),d1
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
od4037:   and.w     d1,40(a1) ;wie oben
od4038:   or.w      d3,40(a1)
od8037:   and.w     d1,80(a1)
od8038:   or.w      d2,80(a1)
od12037:  and.w     d1,120(a1)
          move.l    a5,d1
od12038:  or.w      d1,120(a1)

L0037t:   swap.w    d7
          swap.w    d3
          swap.w    d2
          swap.w    d1
L0038t:   swap.w    d6

          movea.l   d1,a5     ;d1 retten
          or.w      d2,d1
          or.w      d3,d1
          or.w      d7,d1     ;Maske berechnen
;                     not.w   D1

          move.w    -(a1),d0  ;Screen holen
          eor.w     d0,d7
          and.w     d6,d7
          and.w     d1,d7
          eor.w     d0,d7
          move.w    d7,(a1)   ;und zurueck auf Screen

od4039:   move.w    40(a1),d0 ;Screen holen (wie oben)
          eor.w     d0,d3
          and.w     d6,d3
          and.w     d1,d3
          eor.w     d0,d3
od4040:   move.w    d3,40(a1) ;und zurueck auf Screen

od8039:   move.w    80(a1),d0 ;Screen holen
          eor.w     d0,d2
          and.w     d6,d2
          and.w     d1,d2
          eor.w     d0,d2
od8040:   move.w    d2,80(a1) ;und zurueck auf Screen

          move.w    a5,d2     ;und altes d1 holen

od12039:  move.w    120(a1),d0          ;Screen holen
          eor.w     d0,d2
          and.w     d6,d2
          and.w     d1,d2     ;Maske
          eor.w     d0,d2
od12040:  move.w    d2,120(a1)          ;und zurueck auf Screen
          bra       L0039     ;und naechste Scanline


;Diese Routine kann ganze Bildschirmteile spiegeln. Diese Bildschirmbereiche
;mÅssen aber auf BBs liegen, d.h. sie clippt die RÑnder nicht!
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
          lsl.w     #1,d0
          adda.w    d0,a0
          move.w    16(a6),d0 ;x2
          lsr.w     #4,d0
          lsl.w     #1,d0
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
          lsl.w     #1,d0     ;Breite in Bytes 3
          movea.w   #160,a3
          suba.w    d0,a3     ;Offset fÅr linken Rand
          movea.w   #160,a4   ;Offset fÅr rechten Rand
          adda.w    d0,a4
          tst.b     ungerade2
          bne.s     gerade22
          subq.l    #2,a3
          subq.l    #2,a4
gerade22: move.w    18(a6),d6
          sub.w     14(a6),d6 ;Hîhe in Scanlines
          lea.l     mirror_tab,a2

          moveq.l   #0,d0
          moveq.l   #0,d1

mirror_loop2:       move.w    a5,d7     ;Breite in BBs-1

mirror_loop1:       move.w    (a1),d2   ;Rechten Rand merken
          move.w    40(a1),d3
          move.w    80(a1),d4
          move.w    120(a1),d5

          move.b    (a0),d0
          move.b    1(a0),d1
          move.b    0(a2,d0.w),d0       ;Byte spiegeln
          move.b    0(a2,d1.w),d1       ;-'-
          move.b    d1,(a1)
          move.b    d0,1(a1)

          move.b    40(a0),d0
          move.b    41(a0),d1
          move.b    0(a2,d0.w),d0       ;Byte spiegeln
          move.b    0(a2,d1.w),d1       ;-'-
          move.b    d1,40(a1)
          move.b    d0,41(a1)

          move.b    80(a0),d0
          move.b    81(a0),d1
          move.b    0(a2,d0.w),d0       ;Byte spiegeln
          move.b    0(a2,d1.w),d1       ;-'-
          move.b    d1,80(a1)
          move.b    d0,81(a1)

          move.b    120(a0),d0
          move.b    121(a0),d1
          move.b    0(a2,d0.w),d0       ;Byte spiegeln
          move.b    0(a2,d1.w),d1       ;-'-
          move.b    d1,120(a1)
          move.b    d0,121(a1)

          subq.l    #2,a1     ;An Anfang des nÑchsten BB rechts

          move.b    d2,d0
          move.b    0(a2,d0.w),d0
          move.b    d0,(a0)
          lsr.w     #8,d2     ;2. Byte holen
          move.b    0(a2,d2.w),d2
          move.b    d2,1(a0)

          move.b    d3,d0
          move.b    0(a2,d0.w),d0
          move.b    d0,40(a0)
          lsr.w     #8,d3     ;2. Byte holen
          move.b    0(a2,d3.w),d3
          move.b    d3,41(a0)

          move.b    d4,d0
          move.b    0(a2,d0.w),d0
          move.b    d0,80(a0)
          lsr.w     #8,d4     ;2. Byte holen
          move.b    0(a2,d4.w),d4
          move.b    d4,81(a0)

          move.b    d5,d0
          move.b    0(a2,d0.w),d0
          move.b    d0,120(a0)
          lsr.w     #8,d5     ;2. Byte holen
          move.b    0(a2,d5.w),d5
          move.b    d5,121(a0)

          addq.l    #2,a0     ;nÑchster BB
          dbra      d7,mirror_loop1

          tst.b     ungerade2
          bne.s     wt62

          move.b    (a0),d0
          move.b    1(a0),d1
          move.b    0(a2,d0.w),d0       ;Byte spiegeln
          move.b    0(a2,d1.w),d1       ;-'-
          move.b    d1,(a1)
          move.b    d0,1(a1)

          move.b    40(a0),d0
          move.b    41(a0),d1
          move.b    0(a2,d0.w),d0       ;Byte spiegeln
          move.b    0(a2,d1.w),d1       ;-'-
          move.b    d1,40(a1)
          move.b    d0,41(a1)

          move.b    80(a0),d0
          move.b    81(a0),d1
          move.b    0(a2,d0.w),d0       ;Byte spiegeln
          move.b    0(a2,d1.w),d1       ;-'-
          move.b    d1,80(a1)
          move.b    d0,81(a1)

          move.b    120(a0),d0
          move.b    121(a0),d1
          move.b    0(a2,d0.w),d0       ;Byte spiegeln
          move.b    0(a2,d1.w),d1       ;-'-
          move.b    d1,120(a1)
          move.b    d0,121(a1)

          addq.l    #2,a0
          addq.l    #2,a1

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


;Entpackt auf sich selber
decrunch2:          link      a3,#-$00000078
          movem.l   d0-d7/a0-a6,-(sp)
          lea       120(a0),a4
          movea.l   a4,a6
          bsr.s     ice_unp28
          cmpi.l    #$49434521,d0
          bne.s     ice_unp27
          bsr.s     ice_unp28
          lea       -8(a0,d0.l),a5
          bsr.s     ice_unp28
          move.l    d0,(sp)
          adda.l    d0,a6
          movea.l   a6,a1
          moveq     #$77,d0
ice_unp2: move.b    -(a1),-(a3)
          dbra      d0,ice_unp2
          movea.l   a6,a3
          move.b    -(a5),d7
          bsr.s     ice_unp210
          movea.l   a3,a5
          bsr.s     ice_unp214
          bcc.s     ice_unp24
          move.w    #$0f9f,d7
ice_unp21:          moveq     #3,d6
ice_unp22:          move.w    -(a3),d4
          moveq     #3,d5
ice_unp23:          add.w     d4,d4
          addx.w    d0,d0
          add.w     d4,d4
          addx.w    d1,d1
          add.w     d4,d4
          addx.w    d2,d2
          add.w     d4,d4
          addx.w    d3,d3
          dbra      d5,ice_unp23
          dbra      d6,ice_unp22
          movem.w   d0-d3,(a3)
          dbra      d7,ice_unp21
ice_unp24:          movem.l   (sp),d0-d7/a0-a3
ice_unp25:          move.b    (a4)+,(a0)+
          subq.l    #1,d0
          bne.s     ice_unp25
          moveq     #$77,d0
ice_unp26:          move.b    -(a3),-(a5)
          dbra      d0,ice_unp26
ice_unp27:          movem.l   (sp)+,d0-d7/a0-a6
          unlk      a3
          rts       
ice_unp28:          moveq     #3,d1
ice_unp29:          lsl.l     #8,d0
          move.b    (a0)+,d0
          dbra      d1,ice_unp29
          rts       
ice_unp210:         bsr.s     ice_unp214
          bcc.s     ice_unp213
          moveq     #0,d1
          bsr.s     ice_unp214
          bcc.s     ice_unp212
          lea       ice_unp227(pc),a1
          moveq     #4,d3
ice_unp211:         move.l    -(a1),d0
          bsr.s     ice_unp216
          swap.w    d0
          cmp.w     d0,d1
          dbne      d3,ice_unp211
          add.l     20(a1),d1
ice_unp212:         move.b    -(a5),-(a6)
          dbra      d1,ice_unp212
ice_unp213:         cmpa.l    a4,a6
          bgt.s     ice_unp219
          rts       
ice_unp214:         add.b     d7,d7
          bne.s     ice_unp215
          move.b    -(a5),d7
          addx.b    d7,d7
ice_unp215:         rts       
ice_unp216:         moveq     #0,d1
ice_unp217:         add.b     d7,d7
          bne.s     ice_unp218
          move.b    -(a5),d7
          addx.b    d7,d7
ice_unp218:         addx.w    d1,d1
          dbra      d0,ice_unp217
          rts       
ice_unp219:         lea       ice_unp228(pc),a1
          moveq     #3,d2
ice_unp220:         bsr.s     ice_unp214
          dbcc      d2,ice_unp220
          moveq     #0,d4
          moveq     #0,d1
          move.b    1(a1,d2.w),d0
          ext.w     d0
          bmi.s     ice_unp221
          bsr.s     ice_unp216
ice_unp221:         move.b    6(a1,d2.w),d4
          add.w     d1,d4
          beq.s     ice_unp223
          lea       ice_unp229(pc),a1
          moveq     #1,d2
ice_unp222:         bsr.s     ice_unp214
          dbcc      d2,ice_unp222
          moveq     #0,d1
          move.b    1(a1,d2.w),d0
          ext.w     d0
          bsr.s     ice_unp216
          add.w     d2,d2
          add.w     6(a1,d2.w),d1
          bpl.s     ice_unp225
          sub.w     d4,d1
          bra.s     ice_unp225
ice_unp223:         moveq     #0,d1
          moveq     #5,d0
          moveq     #-1,d2
          bsr.s     ice_unp214
          bcc.s     ice_unp224
          moveq     #8,d0
          moveq     #$3f,d2
ice_unp224:         bsr.s     ice_unp216
          add.w     d2,d1
ice_unp225:         lea       2(a6,d4.w),a1
          adda.w    d1,a1
          move.b    -(a1),-(a6)
ice_unp226:         move.b    -(a1),-(a6)
          dbra      d4,ice_unp226
          bra       ice_unp210
          .DC.b $7f,$ff,$00,$0e,$00,$ff,$00,$07
          .DC.b $00,$07,$00,$02,$00,$03,$00,$01
          .DC.b $00,$03,$00,$01
ice_unp227:         .DC.b $00,$00,$01,$0d,$00,$00,$00,$0e
          .DC.b $00,$00,$00,$07,$00,$00,$00,$04
          .DC.b $00,$00,$00,$01
ice_unp228:         .DC.b $09,$01,$00,$ff,$ff,$08,$04,$02
          .DC.b $01,$00
ice_unp229:         .DC.b $0b,$04,$07,$00,$01,$1f,$ff,$ff
          .DC.b $00,$1f
ice_unp230:         
          .EVEN 

;Zeichnet eine Linie auf dem Screen, der durch logbase bestimmt wird. Zum
;Zeichnen der Linie kann auch eine Farbe angegeben werden.
;-> 16(a6).W: y2
;   14(a6).W: x2
;   12(a6).W: y1
;   10(a6).W: x1
;    8(a6).W: Color
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
          movea.l   d1,a5     ;Y-ZeilenzÑhler (Startzeile)
          mulu.w    #160,d1
          movea.l   logbase,a0
          move.w    d0,d4     ;Start_x
          andi.w    #$fff0,d4
          lsr.w     #3,d4
          add.w     d4,d1
          adda.w    d1,a0     ;-> bildschirmadresse
          not.w     d0
          andi.w    #15,d0
          moveq     #0,d4
          bset      d0,d4     ;-> bitmuster
          tst.w     d2        ;in welche Richtung starten?
          bmi.s     linksrum
          cmp.w     d2,d3     ;Delta_y grîûer Delta_x?
          bgt.s     dygrdx1   ;ja, diese Routine
;flache gerade
          move.w    d2,d7
;d0=zaehler d2/d3 -> delta's
          move.w    d2,d0
          lsr.w     #1,d0
          moveq     #0,d5
gloop1:   
;punkt setzen
          or.w      d4,d5
          ror.w     #1,d4
          bcc.s     nichtneueswort1
          bsr       setze_farben
          addq.l    #2,a0     ;Eine Plane weiter
          moveq     #0,d5
nichtneueswort1:    sub.w     d3,d0     ;dx-dy
          bcc.s     keinsprung1         ;war vorher bhi
          add.w     d2,d0
          bsr       setze_farben
          moveq     #0,d5
          lea       160(a0),a0          ;eine Zeile tiefer
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
          move.w    d3,d0     ;Hîhe des Blocks
          lsr.w     #1,d0     ;halbieren
          move.w    d4,d5     ;Bitmuster
gloop2:   
;punkt setzen
          bsr       setze_farben
          lea       160(a0),a0
          addq.w    #1,a5     ;Eine Zeile tiefer
          sub.w     d2,d0     ;dy-dx
          bcc.s     keinsprung2         ;war vorher bhi konnte dann aber kein 2 Pixel hohe Linie zeichnen
          add.w     d3,d0
          ror.w     #1,d5
          bcc.s     nichtneueswort2
          addq.l    #2,a0
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
          moveq     #0,d5
gloop3:   
;punkt setzen
          or.w      d4,d5
          rol.w     #1,d4
          bcc.s     nichtneueswort3
          bsr       setze_farben
          subq.l    #2,a0
          moveq     #0,d5
nichtneueswort3:    
          sub.w     d3,d0     ;dx-dy
          bhi.s     keinsprung3
          add.w     d2,d0
          bsr.s     setze_farben
          moveq     #0,d5
          lea       160(a0),a0
          addq.l    #1,a5     ;Eine Zeile tiefer
keinsprung3:        
          dbra      d7,gloop3
          bsr.s     setze_farben
          bra.s     line_end

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
          lea       160(a0),a0
          addq.l    #1,a5     ;Eine Zeile tiefer
          sub.w     d2,d0     ;dy-dx
          bhi.s     keinsprung4
          add.w     d3,d0
          rol.w     #1,d5
          bcc.s     nichtneueswort4
          subq.l    #2,a0
nichtneueswort4:    
keinsprung4:        
          dbra      d7,gloop4
          bra       line_end

;Selbst modfiziert sich, je nach Farbe, Linientyp und FÅllmuster
;-> D6: Farbe
self_modify:        move.w    line_modus,d0
          lsl.w     #4,d0     ;16 Bytes pro Opcode
          lea       opcodes,a0
          adda.w    d0,a0
          lea       donne_le_moi,a1     ;Hier aufbauen

          moveq     #0,d1     ;Offset zu den Planes
          moveq     #3,d7     ;4 Planes
put_word: moveq     #8,d0     ;weglîschen als Default
          lsr.w     #1,d6     ;Farbe schieben
          bcc.s     set_plane
          moveq     #0,d0     ;setzen
set_plane:          move.w    0(a0,d0.w),(a1)+    ;Opcode eintragen
          move.w    d1,(a1)+  ;Offset zur Plane
          tst.l     4(a0,d0.w)
          beq.s     nxt_plane
          move.w    4(a0,d0.w),(a1)+    ;2. Opcode eintragen
          move.w    d1,(a1)+
nxt_plane:          addi.w    #40,d1    ;nÑchste Plane
          dbra      d7,put_word
          move.l    schluss,(a1)        ;RTS etc.

          movea.l   fuellmuster,a2      ;Adresse des FÅllmusters
          rts       

;Setzt die Farbe im Bildschirmspeicher
; D5: Bitmuster
;  A0: Bildschirmadresse
setze_farben:       movea.l   d7,a3     ;d7 retten

          move.w    d5,d6     ;Linientyp
          not.w     d6        ;Maske berechnen
          move.w    a5,d7     ;Y-Counter holen
          andi.w    #%0000000000001111,d7         ;nur 16'er
          add.w     d7,d7
          and.w     0(a2,d7.w),d5       ;Mit FÅllmuster verknÅpfen
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

;Dies ist eine Tabelle fÅr die verschiedenen FÅllmodi. Dabei
;steht zuerst der Opcode fÅr Replace, dann Eor und schliesslich Transparent.
;Es sind jeweils 2 Befehle zum Setzen von Punkten und 2 zum Lîschen.
opcodes:  and.w     d6,0(a0)  ;Replace, Maskieren
          or.w      d5,0(a0)  ;und FÅllmuster rein
          and.w     d6,0(a0)  ;weglîschen, Maskieren
          and.w     d7,0(a0)  ;und Punkte wieder weg

          eor.w     d5,0(a0)  ;Eor setzen
          .DC.l 0
          eor.w     d5,0(a0)  ;Eor lîschen
          .DC.l 0

          or.w      d5,0(a0)  ;Transparent setzen
          .DC.l 0
          and.w     d7,0(a0)  ;Transparent lîschen
          .DC.l 0

;FÅllt eine FlÑche auf dem Bildschirm unter Verwendung der Line-Funktion
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
          exg.l     d2,d4     ;immer von oben nach unten fÅllen
fuellen:  move.w    d2,-(sp)
          move.w    d3,-(sp)
          move.w    d2,-(sp)
          move.w    d1,-(sp)
          move.w    d0,-(sp)
          bsr       mcode53
          lea       10(sp),sp

          addq.w    #1,d2
          cmp.w     d4,d2
          ble.s     fuellen

          movem.l   (sp)+,d0-d4
          unlk      a6
          rts       

;Berechnet die Offsets innerhalb von Rasters2 neu
;d.h. auf dem Amiga wird die copperliste 2 gebaut, aber weiterhin
;copperliste 1 angezeigt.
mcode55:  movem.l   d1-d7/a0-a6,-(sp)

          lea       rasters2,a2         ;Hier Copperliste aufbauen

          moveq     #3,d7     ;4 Planes
          move.w    #$00e0,d1 ;Register der BB
          movea.l   scr1,a0   ;Adresse des Screens
make_cl2: bsr       set_adr
          lea       40(a0),a0
          dbra      d7,make_cl2

          move.w    #spr0pth,d1
          lea       maus_sprite,a0
          bsr       set_adr

          moveq     #6,d7     ;noch weitere 7 Sprites
          lea       spr_off,a0
set_sprites:        bsr       set_adr
          dbra      d7,set_sprites

          move.l    a2,raster_start     ;ab hier nur noch Raster

          lea       hbl_system+2,a0     ;VBL-Raster ab hier
          bsr       make_copper         ;und alle Farben umschalten

          moveq     #0,d4     ;noch unterhalb 256

          moveq     #1,d7     ;Y-Wert
such_nxt_raster:    move.w    #1000,d6  ;Max-Wert
          move.w    #anz_raster-1,d5    ;Anzahl der zu untersuchenden Raster
          lea       hbl_system,a0       ;Tabelle der Rasters
such_min: move.w    (a0),d0   ;Y-Position dieses Rasters
          beq.s     nxt_raster          ;unbenutzt->nÑchsten
          cmp.w     d6,d0     ;Dieser Raster weiter oben?
          bge.s     nxt_raster          ;Nein->nÑchsten
          cmp.w     d7,d0     ;weiter unten als letzter Raster?
          ble.s     nxt_raster          ;Nein->nÑchsten
          lea       2(a0),a1  ;Position der Palette merken
          move.w    d0,d6     ;als neuen Kleinsten nehmen
nxt_raster:         lea       34(a0),a0 ;nÑchsten Raster nehmen
          dbra      d5,such_min

          cmpi.w    #1000,d6  ;Alle Raster gesetzt?
          beq.s     fertig

          move.w    d6,d7     ;Position des aktiven Rasters->letzter

          add.w     wistarty,d6         ;tatsaechliche Rasterzeile auf TV
          cmpi.w    #255,d6
          ble.s     normal_setzen
          tst.w     d4        ;Erster Raster oberhalb 256?
          bne.s     normal_setzen       ;Nein->setzen
          moveq     #-1,d4
          move.l    #$ffe1fffe,(a2)+    ;auf Zeile 255 warten
normal_setzen:      move.b    d6,(a2)+  ;Y-Position eintragen
          move.b    #1,(a2)+  ;X-Position eintragen
          move.w    #$fffe,(a2)+        ;WAIT-Befehl

          movea.l   a1,a0     ;Adresse der Palette
          bsr       make_copper         ;und Farbumschaltung

          bra.s     such_nxt_raster

fertig:   move.l    #$fffffffe,(a2)+    ;Ende der Copperlist

          movem.l   (sp)+,d1-d7/a0-a6
          rts       

;Setzt in der Copperliste die Adresse eines DMA-AdresszÑhlers
;-> D1.W: Register, welches zu setzen ist (wird erhîht)
;   A0.L: zu schreibende Adresse
;   A2.L: Adresse der Copperliste (wird erhîht)
set_adr:  movem.l   d0/d2-d7/a0/a1/a3-a6,-(sp)

          move.w    d1,(a2)+
          move.l    a0,d0
          swap.w    d0
          move.w    d0,(a2)+
          swap.w    d0
          addq.w    #2,d1     ;dann High-Pointer
          move.w    d1,(a2)+
          move.w    d0,(a2)+
          addq.w    #2,d1     ;auf nÑchstes Register gehen

          movem.l   (sp)+,d0/d2-d7/a0/a1/a3-a6
          rts       

;Diese Funktion erstellt ein Copper-Programm, welches eine gesamte
;Farbpalette umschaltet..
;-> A0.L: Adresse der neuen Farbpalette (wird erhîht)
;   A2.L: Adresse des Copperprogramms (wird erhîht)
make_copper:        movem.l   d0-d7/a1/a3-a6,-(sp)

          move.w    #$0180,d0 ;Offset zur Palette
          moveq     #15,d7    ;16 Farbregister
make_2:   move.w    d0,(a2)+  ;Register eintragen
          addq.w    #2,d0     ;nÑchstes Register
          move.w    (a0)+,d1  ;ST-Farbe holen
          lsl.w     #1,d1     ;in Amiga wandeln
          ori.w     #%0000000100010001,d1
;          andi.w    #%0000111011101110,d1         ;Unterstes lîschen
;          move.w    d1,d2
;          andi.w    #%0000100010001000,d2
;          lsr.l     #3,d2
;          or.w      d2,d1
          move.w    d1,(a2)+  ;und speichern
          dbra      d7,make_2

          movem.l   (sp)+,d0-d7/a1/a3-a6
          rts       

;Kopiert Rasters1 -> Rasters2 (CHS):
rst12rst2:          movem.l   d0-d7/a0-a6,-(sp)
          lea       rasters1,a0         ;Raster1->Rasters2
          lea       rasters2,a1
          bra.s     copy_them

;Kopiert Rasters2 -> Rasters1 (CHS):
rst22rst1:          movem.l   d0-d7/a0-a6,-(sp)
          lea       rasters2,a0         ;Rasters2->Rasters1
          lea       rasters1,a1
copy_them:          move.w    #(end_rasters-rasters2)/4-1,d6          ;max .Anzahl der Raster
copy_raster:        move.l    (a0)+,(a1)+
          dbra      d6,copy_raster
          movem.l   (sp)+,d0-d7/a0-a6
          rts       

;Rechnet die Offsets neu aus, und zeigt die Raster dann an
;show_raster()
mcode56:  movem.l   d0-d7/a0-a6,-(sp)

          bsr       mcode55   ;und neu ausrechnen

          lea       $00dff000,a5
          move.l    #rasters2,$00dff080 ;und neue anzeigen
          move.l    #rasters2,copperliste

          bsr       vsync
          bsr       rst22rst1 ;und zurÅck kopieren

          move.l    #rasters1,$00dff080 ;und 1. System wieder anzeigen
          move.l    #rasters1,copperliste

          movem.l   (sp)+,d0-d7/a0-a6
          rts       

;Holt ein Zeichen aus dem Tastaturbuffer
;Ist kein Zeichen vorhanden, bringt die Routine -1 zurÅck
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
          moveq     #0,d1
buffer_ende:        lea       key_buffer,a0
          moveq     #0,d0
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
shift:    .DC.b 0   ;Nichts gedrÅckt
          .EVEN 
scancode: .IBYTES "SCANTAB.IMG"

key_buffer:         .DS.b 84
keypress: .DS.l 1

;Mit Hilfe dieser Funktion kînnen die Parameter fÅr die Line-Funktion
;eingestellt werden. Da die Fill Funktion die Line-Funktion benÅtzt, gelten
;die Einstellungen auch fÅr die Fill-Funktion:
;Die Funktion gibt den alten Linientyp zurÅck, bei einer öbergabe von -1 wird
;der Linientyp nicht verÑndert.
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
          lsl.w     #1,d0     ;BB berechnen
          adda.w    d0,a0
          move.w    8(a6),d0
          andi.w    #%0000000000001111,d0         ;Skew
          moveq     #15,d1
          sub.w     d0,d1
          moveq     #0,d0     ;Hier Farbe aufbauen
          moveq     #3,d3     ;4 Planes
plane_loop7:        ror.b     #1,d0
          move.w    (a0),d2
          lea       40(a0),a0 ;nÑchste Plane
          btst      d1,d2     ;Farbe gesetzt?
          beq.s     naechste_plane
          addq.w    #1,d0
naechste_plane:     dbra      d3,plane_loop7
          ror.b     #5,d0     ;an richtige Position schieben
          movem.l   (sp)+,d1-d3/a0
          unlk      a6
          rts       


;Bestimmt das Muster, mit dem gefÅllt werden soll. Diese Einstellung gilt fÅr
;Linienziehen, wie fÅr fÅllen.
;Es wird hierbei ein Pointer auf 16 Integers Åbergeben, in denen das FÅllmuster
;zeilenweise abgespeichert wird. Es wird also, je nach y-Zeile % 16 die
;entsprechende Zeile aus dem Muster geholt, und damit die Linie verknÅpft.
;Ist das gesamte Integerarray mit -1 gefÅllt, also alle Bits gesetzt, wird
;ganz normal gefÅllt.
;Wird als Integeradresse -1L angegeben, so wird die Adresse des letzten
;FÅllmusters zurÅckgegeben.

;int *line_pattern(int *pattern);
mcode60:  move.l    fuellmuster,d0      ;Adresse des alten FÅllmuster
          tst.l     4(sp)
          bmi.s     gib_zurueck2
          move.l    4(sp),fuellmuster   ;Adresse
gib_zurueck2:       rts       


;Dies ist die Diskettenfunktion. Ist der Modus 0, wird das TOS zum Laden
;benutzt, sonst eine eigene Diskettenroutine. Diese Routine prÅft vor dem
;Laden, ob ein entsprechendes Archiv vorhanden ist, bzw. fordert zur Einlage
;der jeweiligen Diskette auf. Gibt die tatsÑchliche LÑnge der gelesenen Daten
;zurÅck.
;long loaddisk(archiv,offset,lÑnge,adr)
;-> 18(a6): Ladeadresse
;   14(a6): LÑnge der zu ladenden Daten
;   10(a6): Offset innerhalb des Archivs
;    8(a6): archiv [0..n]
mcode61:  link      a6,#0
          movem.l   d1-d7/a0-a5,-(sp)

          tst.w     amigatool ;Vom ST holen?
          beq       use_amigatool       ;ja

          move.w    8(a6),-(sp)         ;Nummer der Diskette
          bsr       mcode69   ;User soll ggf. Diskette wechseln
          addq.l    #2,sp

once_again:         move.l    10(a6),d0 ;Offset innerhalb Archiv
          add.l     14(a6),d0 ;Erstes freies Byte
          cmp.l     datei_laenge,d0     ;Kleiner als DateilÑnge?
          ble.s     laenge_okay
          move.l    datei_laenge,d0
          sub.l     10(a6),d0 ;-Offset
          move.l    d0,14(a6) ;Als neue LÑnge merken
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
          move.l    14(a6),d0 ;LÑnge der Daten zurÅckgeben

ende_load:          movem.l   (sp)+,d1-d7/a0-a5
          unlk      a6
          rts       

use_amigatool:      move.w    8(a6),d0  ;Archivnummer holen
          addi.w    #'0',d0
          move.b    d0,rom_nummer

try_again1:         moveq     #0,d1     ;Modus
          lea       rom_grfx(pc),a1
          moveq     #$3d,d0   ;Datei îffnen
          bsr       trap_1
          move.w    d0,fhandle
          bpl.s     datei_offen

hoch:     moveq     #$000f,d0
          bsr       flash_amiga
          bra.s     hoch

datei_offen:        move.l    10(a6),d1 ;Offset
          move.w    fhandle(pc),d2
          lea       rom_grfx(pc),a1
          moveq     #$42,d0   ;Lseek
          bsr       trap_1

          movea.l   18(a6),a0 ;Ladeadresse
          move.l    14(a6),d1 ;LÑnge
          move.w    fhandle(pc),d2
          lea       rom_grfx(pc),a1
          moveq     #$3f,d0   ;Fread
          bsr       trap_1
          move.l    d0,d7     ;LÑnge merken

          move.w    fhandle(pc),d1
          lea       rom_grfx(pc),a1
          moveq     #$3e,d0
          bsr       trap_1    ;Fclose

          move.l    d7,d0     ;und LÑnge zurÅckgeben
          bra.s     ende_load

rom_grfx: .DC.b 'D:\MEDUSA.DAT\MEDUSA_2.COL\ROM_GRFX.'
rom_nummer:         .DC.b 'x',0
          .EVEN 
fhandle:  .DC.w 0


;Diese Funktion wartet einen Vsync ab:
mcode62:  
vsync:    move.l    d0,-(sp)

          move.l    vbl_ct,d0
warte_sync:         cmp.l     vbl_ct,d0
          beq.s     warte_sync

          move.l    (sp)+,d0
          rts       

;Diese Routine spielt einen Hippelsound ab
;void play_music(void *routine,int soundnr); (14)
mcode63:  link      a6,#0

          movem.l   d0-d7/a0-a6,-(sp)

          move.l    8(a6),music_routine

          tst.b     first_init
          bne.s     re_init

          st        first_init

          move.w    #$2700,sr

          move.b    #240,$00bfe601      ;TBLO
          move.b    #55,$00bfe701       ;TBHI

          move.b    $00bfef01,d0        ;CrB
          andi.b    #%10010101,d0       ;Inmode=00 -> ZÑhlt Taktzyklen
          ori.b     #%00000001,d0       ;Start setzen
          move.b    d0,$00bfef01        ;Und Timer starten

          move.b    #%10000010,$00bfed01          ;ICR, Timer B kann IRQ auslîsen

          move.w    #$2000,sr

re_init:  move.w    12(a6),d0 ;Songnummer
          movea.l   music_routine,a0
          movea.l   a0,a1
          lea       $f90(a0),a0
          jsr       (a1)

          moveq     #0,d0     ;volume setzen          (000 = laut)
          movea.l   music_routine,a0
          jsr       8(a0)     ;volle lautstÑrke       (100 = leise)

          movea.l   music_routine,a0
          jsr       12(a0)    ;sfx initialisieren

          move.w    #%1100000000001000,$00dff09a  ;CIA-A IRQ erlauben

          movem.l   (sp)+,d0-d7/a0-a6
          unlk      a6
          rts       



;Spielt einen Hippelsoundeffekt ab
;hippel_sfx(int sound)
mcode38:  link      a6,#0
          movem.l   d0-d7/a0-a6,-(sp)

          move.w    8(a6),d0
          movea.l   music_routine,a0
          jsr       16(a0)

          movem.l   (sp)+,d0-d7/a0-a6
          unlk      a6
          rts       

game_music:         .IBYTES 'ROM_II_G.IMG'
;          .DS.b 5000
          .EVEN 

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
          move.w    d0,-(sp)  ;Endtrack
          clr.w     -(sp)     ;Startoffset
          move.w    #1,-(sp)
          subq.w    #8,d0     ;(12) Startrack
          move.w    d0,-(sp)
          clr.w     -(sp)     ;A:
          move.l    10(a6),-(sp)
          bsr       mcode70   ;Daten schreiben
          lea       18(sp),sp

zurueck2: movem.l   (sp)+,d1-d7/a0-a5
          unlk      a6
          rts       

;Diese Routine lÑdt einen Spielstand von Diskette und zwar immer genau
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


;Random - liefert 16-Bit Zufallszahl
mcode66:  move.w    $00dff006,d0        ;Bildschirmadresse
          rts       


;Diese Funktion ÅberprÅft, ob ein Zeichen vorhanden ist.
;FLAG is_key(void);
mcode67:  move.l    d1,-(sp)

          moveq     #0,d0     ;Kein Zeichen da

          move.w    head,d1
          cmp.w     tail,d1
          beq.s     nichts_da3

          moveq     #-1,d0    ;Ja, es ist was da!

nichts_da3:         move.l    (sp)+,d1
          rts       


;void floppy_read(void *adr,int laufwerk,int strack,int ssektor,int soffset,
;                          int etrack,int esektor,int eoffset);
;-> 24(a6): eoffset
;   22(a6): esektor [1..11]
;   20(a6): etrack
;   18(a6): soffset
;   16(a6): ssektor [1..11]
;   14(a6): strack
;   12(a6): Laufwerk (Bit 0=Laufwerk, Bit 1=Seite)
;    8(a6): adr

mcode68:  link      a6,#0
          movem.l   d0-d7/a0-a5,-(sp)

          bsr       select    ;DF0: selektieren

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

mehr_als_einer:     bsr       trans_start         ;Ab Startoffset Åbertragen

          addq.w    #1,16(a6) ;nÑchsten Sektor geht's weiter
          move.w    sek_per_track,d0
          cmp.w     16(a6),d0 ;Startsektor 10?
          bge.s     no_step   ;Nein, also kein Step

vorne_anfangen:     move.w    #1,16(a6) ;Wieder bei Sektor #1 starten
          addq.w    #1,14(a6) ;Auf nÑchsten Track gehen
          move.w    14(a6),d6
          bsr       seek_track          ;Track anfahren

no_step:  move.w    14(a6),d4 ;Starttrack
          cmp.w     20(a6),d4 ;Endtrack erreicht?
          beq.s     letzter_track

read_again2:        move.w    sek_per_track,d0
          addq.w    #1,d0
          sub.w     16(a6),d0 ;Anzahl der zu lesenden Sektoren
          movea.l   a2,a0     ;Ab Ladeadresse laden
          bsr       read_sektoren
          adda.w    d5,a2     ;Bytes Åberspringen
          bra.s     vorne_anfangen

letzter_track:      move.w    22(a6),d0 ;Endsektor
          sub.w     16(a6),d0 ;-Startsektor
          beq.s     nur_ein_sektor
          move.w    d0,-(sp)  ;Anzahl der Sektoren retten
          movea.l   a2,a0     ;Ladeadresse
          bsr       read_sektoren
          adda.w    d5,a2     ;öberspringen
          move.w    (sp)+,d0  ;Anzahl der gelesenen Sektoren
          add.w     d0,16(a6) ;=NÑchster Sektor

nur_ein_sektor:     moveq     #1,d0
          lea       bitfeld,a0
          bsr       read_sektoren       ;Letzten Sektor lesen
          lea       bitfeld,a0
          move.w    24(a6),d7 ;Endoffset
trans3:   move.b    (a0)+,(a2)+         ;Rest Åbertragen
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

aus_is:   bsr       deselect  ;Laufwerk wieder aus

          movem.l   (sp)+,d0-d7/a0-a5
          unlk      a6
          rts       


;öbertrÑgt 1. Sektor ab Startoffset bis Sektorende
trans_start:        move.w    18(a6),d0 ;Startoffset
          lea       bitfeld,a0
          adda.w    d0,a0     ;Bytes Åberspringen
          move.w    #512,d1   ;Soviel Bytes kommen
          sub.w     d0,d1     ;Offset abziehen=Anzahl an Bytes
          bra.s     trans18
trans2:   move.b    (a0)+,(a2)+
trans18:  dbra      d1,trans2
          rts       

;Liest ab Startsektor Sektoren ein. Gibt in D5 512*n zurÅck.
;-> D0: Anzahl der Sektoren
;   A0: Ladeadresse
read_sektoren:      movem.l   d0-d4/d6/d7/a0-a6,-(sp)

          move.w    d0,-(sp)  ;Anzahl retten
read_error:         bsr       read_track          ;Track lesen
          move.w    (sp),d0   ;Anzahl holen
          move.w    16(a6),d7 ;Aktueller Sektor
          subq.w    #1,d7     ;Beim Amiga startet die ZÑhlung bei 0
          bra.s     ss_dbra
sektor_trans:       tst.l     (a1)      ;Wurde Åberhaupt was geladen?
          beq.s     lesefehler
          move.w    track,d1
          cmp.b     1(a1),d1  ;Richtiger Track?
          bne.s     lesefehler
          cmp.b     2(a1),d7  ;Sektor gefunden?
          bne.s     nxt_sektor3
          addq.l    #4,a1     ;Miniheader Åberspringen
          moveq     #127,d6
ttz:      move.l    (a1)+,(a0)+         ;eintragen
          dbra      d6,ttz
          addq.w    #1,d7     ;nÑchster Sektor
ss_dbra:  lea       track_buffer,a1
          dbra      d0,sektor_trans

          move.w    (sp)+,d0
          move.w    d0,d5
          mulu.w    #512,d5   ;Soviel Bytes wurden gelesen
          movem.l   (sp)+,d0-d4/d6/d7/a0-a6
          rts       

nxt_sektor3:        lea       512+4(a1),a1
          bra.s     sektor_trans

lesefehler:         move.w    track,-(sp)         ;gewÅnschter Track
          moveq     #0,d6
          bsr       seek_track          ;Restore ausfÅhren
          move.w    (sp)+,d6
          bsr       seek_track          ;und nochmal anfahren
          bra       read_error


;Liest einen Track von Diskette ein und dekodiert ihn.
;Falls der Track sich bereits im Buffer befindet, wird er nicht erneut
;eingelesen.
read_track:         movem.l   d0-d7/a0-a6,-(sp)

          move.w    track,d0  ;Aktueller Track
          cmp.w     track_at_buffer,d0
;          beq.s     read_ready          ;Buffer im Track?

          move.w    track,track_at_buffer         ;Jetzt ist dieser Track da

read_again:         lea       track_buffer,a0     ;hier hin lesen

          lea       $dff000,a5
          move.w    #%1000001000010000,$96(a5)    ;Disk DMA an
          move.w    #%111111100000000,$9e(a5)     ;alle Bits aus
          move.w    #%1001010100000000,$9e(a5)    ;MFM+Sync
          move.w    #$4489,$7e(a5)      ;Sync-Wert
          move.w    #$4489,(a0)+        ;1. Sync eintragen
          move.w    #$4000,$24(a5)      ;Disk-DMA aus
          move.l    a0,$20(a5)          ;DMA-Adresse
          bsr       wait_ready
          move.w    #12980,d0 ;Anzahl Bytes
          lsr.w     #1,d0
          ori.w     #$8000,d0 ;DMA lesen
          move.w    d0,$24(a5)
          move.w    d0,$24(a5)
          bsr       wait_dma
          move.w    d0,timeout

          subq.l    #2,a0     ;Bufferadresse
          bsr.s     decode_track

          moveq     #11,d7
          bra.s     sek_dbra1
sek_loop: tst.l     (a0)      ;Buffer zuende?
          beq.s     read_again
          cmp.b     2(a0),d7
          beq.s     sek_dbra1 ;Jawohl, gefunden
          lea       512+4(a0),a0
          bra.s     sek_loop
sek_dbra1:          lea       track_buffer,a0
          dbra      d7,sek_loop

read_ready:         movem.l   (sp)+,d0-d7/a0-a6
          rts       


track_at_buffer:    .DC.w -1  ;Kein Track im Buffer

;Diese Routine decodiert einen Track.
;-> A0.L: Adresse des Tracks
decode_track:       movem.l   d0-d7/a0-a6,-(sp)

          movea.l   a0,a1     ;hier wieder hin

          lea       12800(a0),a3        ;Ende des Buffers
          move.l    #$55555555,d2       ;zum MFM dekodieren

such_sektor:        cmpi.w    #$4489,(a0)         ;Sync?
          bne.s     no_sync
weitere_sync:       cmpi.w    #$4489,2(a0)        ;2. Sync?
          bne.s     keine_zweite
          addq.l    #2,a0     ;Eine Sync weitergehen
          bra.s     weitere_sync
keine_zweite:       cmpi.b    #$55,2(a0)          ;$FF (Format-Mark)?
          bne.s     no_sync
          cmpi.b    #$55,6(a0)          ;2. Mark?
          bne.s     no_sync

          move.l    2(a0),d0
          move.l    6(a0),d1
          and.l     d2,d0
          and.l     d2,d1
          lsl.l     #1,d0
          or.l      d1,d0
          move.l    d0,(a1)   ;Buffer Mark merken

          lea       2(a0),a2  ;Sync nicht CRC
          moveq     #40,d0    ;40 Bytes
          bsr.s     calc_crc

          move.l    42(a0),d1
          move.l    46(a0),d3
          and.l     d2,d1
          and.l     d2,d3
          lsl.l     #1,d1
          or.l      d3,d1
          cmp.l     d1,d0     ;CRC korrekt?
          bne.s     crc_error

          lea       58(a0),a2 ;Datenbuffer
          move.w    #1024,d0
          bsr.s     calc_crc

          move.l    50(a0),d1
          move.l    54(a0),d3
          and.l     d2,d1
          and.l     d2,d3
          lsl.l     #1,d1
          or.l      d3,d1
          cmp.l     d1,d0     ;CRC korrekt?
          bne.s     crc_error

          lea       58(a0),a2 ;Datenbuffer
          addq.l    #4,a1     ;Header Ok
          bsr.s     decode_sektor

crc_error:          
no_sync:  addq.l    #2,a0     ;nÑchstes Wort
          cmpa.l    a3,a0
          blt       such_sektor

          clr.l     (a1)      ;Ende der Sektoren

          movem.l   (sp)+,d0-d7/a0-a6
          rts       

;Diese Routine berechnet eine PrÅfsumme Åber den angegebenen Bereich
;-> A2.L: Adresse des Bereichs (wird NICHT erhîht)
;   D0.L: LÑnge in Bytes
calc_crc: movem.l   d1-d7/a0-a6,-(sp)

          move.w    d0,d1
          lsr.w     #2,d1     ;Langworte
          subq.w    #1,d1
          moveq     #0,d0
crc_loop: move.l    (a2)+,d2
          eor.l     d2,d0
          dbra      d1,crc_loop

          andi.l    #$55555555,d0       ;Taktbits raus

          movem.l   (sp)+,d1-d7/a0-a6
          rts       

;Diese Routine dekodiert einen Sektor von A0 nach A1.
;-> A2.L: Quelle
;   A1.L: Ziel (wird erhîht)
decode_sektor:      movem.l   d0-d7/a0/a2-a6,-(sp)

          moveq     #127,d7   ;256 Langworte
          move.l    #$55555555,d2
dat_loop: move.l    (a2)+,d0
          move.l    508(a2),d1
          and.l     d2,d1
          and.l     d2,d0
          lsl.l     #1,d0
          or.l      d1,d0
          move.l    d0,(a1)+
          dbra      d7,dat_loop

          movem.l   (sp)+,d0-d7/a0/a2-a6
          rts       


;User soll eine bestimmte Diskette einlegen.
;void insert_disk(int nr);
;-> 8(A6): Diskettennummer [0..n] -3..-99=Datendiskette
mcode69:  link      a6,#0
          movem.l   d0-d7/a0-a5,-(sp)

          move.w    8(a6),d0  ;gewÅnschte Disk
          cmp.w     disk_drin,d0        ;Ist die etwa noch drin?
          beq       nichts

insert_again:       
          move.w    #-99,track_at_buffer          ;Bufferung abschalten
          move.w    #11,sek_per_track
          move.w    #4,first_sek

          move.w    #511,-(sp)          ;End_Offset
          move.w    #3,-(sp)  ;End_Sektor
          move.w    #0,-(sp)  ;End_Track
          clr.w     -(sp)     ;Start_Offset
          move.w    #3,-(sp)  ;Start_Sektor
          move.w    #0,-(sp)  ;Start_Track
          clr.w     -(sp)     ;A:
          pea       bitfeld+1024+512
          bsr       mcode68   ;Directory lesen
          lea       18(sp),sp

          move.w    8(a6),d0
          bpl.s     archiv_disk
          moveq     #0,d0     ;Gesucht wird nur 'ROM_GRFX.'
          bra.s     donne
archiv_disk:        addi.w    #'0',d0
donne:    move.b    d0,rom+8  ;Nummer des Archivs eintragen

          lea       bitfeld+1024+512,a0
          lea       rom,a1
          moveq     #15,d6    ;16 EintrÑge pro Directory
nxt2:     moveq     #0,d7
kein_ende:          move.b    0(a0,d7.w),d0       ;Zeichen von Diskette lesen
          cmp.b     0(a1,d7.w),d0       ;ROM_GRFX.x?
          bne.s     nxt_entry
          addq.w    #1,d7
          tst.b     0(a1,d7.w)
          bne.s     kein_ende
          move.b    27(a0),d0
          lsl.w     #8,d0
          move.b    26(a0),d0 ;Startcluster der Datei holen
          move.w    d0,first_sek        ;1. Sektor der Daten

          move.b    31(a0),d0
          lsl.w     #8,d0
          move.b    30(a0),d0
          lsl.l     #8,d0
          move.b    29(a0),d0
          lsl.l     #8,d0
          move.b    28(a0),d0
          move.l    d0,datei_laenge     ;LÑnge der Datei merken

          tst.w     8(a6)     ;Sollte Datendisk eingelegt werden?
          bmi.s     falsche_disk

nichts:   move.w    8(a6),disk_drin

          movem.l   (sp)+,d0-d7/a0-a5
          unlk      a6
          rts       

nxt_entry:          lea       32(a0),a0 ;Einen Eintrag weiter
          dbra      d6,nxt2

          tst.w     8(a6)     ;Sollte Datendisk eingelegt werden?
          bmi.s     nichts    ;Ja, also okay

falsche_disk:       bsr       deselect  ;Alle Lichter aus
          move.w    8(a6),d0  ;Archivnummer
          bmi.s     datendiskette
          addi.w    #'A',d0
;          move.b    d0,please+17

          movem.l   d0-d7/a0-a6,-(sp)
          lea       hbl_system+2,a0     ;Sind Raster gesetzt? (2) ab hier..
          moveq     #15,d7
check_vbl:          tst.w     (a0)+     ;Alle VBL-Farben auf Schwarz?
          bne.s     anzeigen  ;Nein->Okay
          dbra      d7,check_vbl

          clr.w     -(sp)     ;Auflîsung
          clr.w     -(sp)     ;Zeile
          pea       alert_pal
          jsr       mcode23   ;Raster setzen
          addq.l    #8,sp

          bsr       mcode56   ;Show_Raster


anzeigen: lea.l     insert_disk,a0      ;(2) bis hier....
          jsr       alert     ;und Alert ausgeben
          movem.l   (sp)+,d0-d7/a0-a6

          bra       insert_again

datendiskette:      lea       insert_game,a0
          movem.l   d0-d7/a0-a6,-(sp)
          jsr       alert
          movem.l   (sp)+,d0-d7/a0-a6
          bra       insert_again

;Palette of MAPA.IFF: Saved by NEOchrome V2.24 by Chaos, Inc. (2)
alert_pal:          .DC.w $0000,$0300,$0520,$0642,$0753,$0030,$0242,$0bdb
          .DC.w $0464,$0057,$0077,$0333,$0444,$0555,$0666,$0fff

rom:      .DC.b 'ROM_GRFXx',0

          .EVEN 


;Diese Funktion schreibt eine Reihe von Sektoren auf die Diskette.
;Achtung! Dies geschieht unter Mithilfe von hlpbuf, d.h. hlpbuf wird zerstîrt!
;Desweiteren wird ein Verify durchgefÅhrt.

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
mcode70:  link      a6,#0
          movem.l   d0-d7/a0-a5,-(sp)

          moveq     #1,d7     ;2 Versuche
retry:    move.w    24(a6),-(sp)        ;End-Offset auf Stack
          move.w    22(a6),-(sp)        ;End-Sektor
          move.w    20(a6),-(sp)        ;End-Track
          move.w    18(a6),-(sp)        ;Start-Offset
          move.w    16(a6),-(sp)        ;Start-Sektor
          move.w    14(a6),-(sp)        ;Start-Track
          move.w    12(a6),-(sp)        ;Laufwerk
          move.l    8(a6),-(sp)         ;Adresse
          bsr       mcode70_2 ;Schreiben
          lea       18(sp),sp
          tst.w     disk_error          ;Gab es einen Diskfehler?
          beq.s     okay_hiller

          move.w    #512,-(sp)          ;End-Offset
          move.w    #6,-(sp)  ;End-Sektor
          move.w    #2,-(sp)  ;End-Track
          move.w    #0,-(sp)  ;Start-Offset
          move.w    #6,-(sp)  ;Start-Sektor
          move.w    #2,-(sp)  ;Start-Track
          move.w    12(a6),-(sp)        ;Laufwerk
          move.l    8(a6),-(sp)
          bsr       mcode70_2
          lea       18(sp),sp

          move.w    #512,-(sp)          ;End-Offset
          move.w    #6,-(sp)  ;End-Sektor
          move.w    #0,-(sp)  ;End-Track
          move.w    #0,-(sp)  ;Start-Offset
          move.w    #6,-(sp)  ;Start-Sektor
          move.w    #0,-(sp)  ;Start-Track
          move.w    12(a6),-(sp)        ;Laufwerk
          move.l    8(a6),-(sp)
          bsr       mcode70_2
          lea       18(sp),sp

          subq.w    #1,d7     ;ein Versuch weniger
          bpl.s     retry

okay_hiller:        movem.l   (sp)+,d0-d7/a0-a5
          unlk      a6
          move.w    disk_error,d0       ;Fehlermeldung zurÅckgeben
          rts       

;Eigentliche Schreiberoutine
;Parameter: Siehe Mcode70()
mcode70_2:          link      a6,#0
          movem.l   d0-d7/a0-a5,-(sp)


          clr.w     disk_error          ;kein Fehler bisher

          bsr       select    ;DF0: selektieren

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
          adda.w    d0,a0     ;Bytes Åberspringen
          move.w    #512,d1   ;Soviel Bytes kommen
          sub.w     d0,d1     ;Offset abziehen=Anzahl an Bytes
          bra.s     trans33
trans44:  move.b    (a2)+,(a0)+         ;In Buffer eintragen
trans33:  dbra      d1,trans44

          moveq     #1,d0
          lea       bitfeld,a0          ;hier steht der neue Sektor
          bsr       write_sektoren      ;und Sektor wieder zurÅckschreiben

          addq.w    #1,16(a6) ;nÑchsten Sektor geht's weiter
          move.w    sek_per_track,d0
          cmp.w     16(a6),d0 ;Startsektor 10?
          bge.s     no_step2  ;Nein, also kein Step

vorne_anfangen2:    move.w    #1,16(a6) ;Wieder bei Sektor #1 starten
          addq.w    #1,14(a6) ;Auf nÑchsten Track gehen
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
          adda.w    d5,a2     ;Bytes Åberspringen
          bra.s     vorne_anfangen2

letzter_track2:     move.w    22(a6),d0 ;Endsektor
          sub.w     16(a6),d0 ;-Startsektor
          beq.s     nur_ein_sektor2
          move.w    d0,-(sp)  ;Anzahl der Sektoren retten
          movea.l   a2,a0     ;Ladeadresse
          bsr.s     write_sektoren
          adda.w    d5,a2     ;öberspringen
          move.w    (sp)+,d0  ;Anzahl der gelesenen Sektoren
          add.w     d0,16(a6) ;=NÑchster Sektor

nur_ein_sektor2:    moveq     #1,d0
          lea       bitfeld,a0
          bsr       read_sektoren       ;Letzten Sektor lesen
          lea       bitfeld,a0
          move.w    24(a6),d7 ;Endoffset
trans6:   move.b    (a2)+,(a0)+         ;Rest Åbertragen
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

          moveq     #1,d0     ;1 Sektor schreiben
          lea       bitfeld,a0
          bsr       write_sektoren

aus_is2:  bsr       deselect

          movem.l   (sp)+,d0-d7/a0-a5
          unlk      a6

          move.w    disk_error,d0       ;Fehlercode zurÅckgeben
          rts       


;Schreibt ab Startsektor Sektoren. Gibt in D5 512*n zurÅck.
;-> D0: Anzahl der Sektoren
;   A0: Ladeadresse
write_sektoren:     movem.l   d0-d7/a0-a6,-(sp)

          move.w    #2,retries          ;3 Versuche

          tst.w     d0        ;wenigstens 1 Sektor?
          ble       schreib_ende

          cmp.w     sek_per_track,d0    ;Alle Sektoren der Disk?
          bne.s     single

          lea       track_buffer,a1
          move.w    track,d1
          moveq     #0,d7
bau_track:          move.b    #$ff,(a1)+
          move.b    d1,(a1)+  ;TrackgerÅst aufbauen
          move.b    d7,(a1)+
          lea       513(a1),a1
          addq.w    #1,d7
          cmp.w     sek_per_track,d7
          blt.s     bau_track
          clr.l     (a1)      ;Ende der Liste

          move.w    d0,-(sp)  ;Anzahl auf Stack
          bra.s     einfuellen

single:   move.w    d0,-(sp)
rewrite:  bsr       read_track          ;Track einlesen+decode
          move.w    (sp),d0   ;Anzahl holen

einfuellen:         move.w    16(a6),d7 ;Startsektor
          subq.w    #1,d7     ;beim Amiga startet die Sektor-ZÑhlung bei 0
          bra.s     sektor_dbra

sektor_loop:        tst.l     (a1)      ;Track zuende?
          beq.s     schreibfehler       ;Sektor nicht gefunden

          cmp.b     2(a1),d7  ;aktueller Sektor?
          bne.s     nxt_sektor

          addq.l    #4,a1     ;Miniheader Åberspringen

sektor_copy:        moveq     #127,d6
trans17:  move.l    (a0)+,(a1)+         ;Sektor eintragen
          dbra      d6,trans17
          addq.w    #1,d7     ;nÑchster Sektor
sektor_dbra:        lea       track_buffer,a1     ;von vorne suchen
          dbra      d0,sektor_loop
          bra.s     schreib_track

nxt_sektor:         lea       512+4(a1),a1
          bra.s     sektor_loop

;In den gelesenen Daten befindet sich nicht der Sektor der geschrieben
;werden soll.
schreibfehler:      move.w    #$f0,d0
          bsr       flash_amiga
          move.w    track,-(sp)
          moveq     #0,d6
          bsr       seek_track
          move.w    (sp)+,d6
          bsr       seek_track

          subq.w    #1,retries          ;1 Versuch weniger
          bpl.s     rewrite   ;nochmal

          move.w    #-1,disk_error      ;Fehler!
          tst.w     (sp)+     ;D0 runterholen
          bra       schreib_ende

schreib_track:      tst.w     (sp)+     ;Anzahl runterholen

          move.w    #1,retries          ;2 Versuche

write_again:        lea       track_buffer,a0
          movea.l   hlpbuf,a1
          bsr       code_track          ;Track codieren
          movea.l   hlpbuf,a5
          moveq     #0,d1
          bsr       write_track         ;und wegschreiben

          lea       track_buffer,a0
          movea.l   hlpbuf,a1
          move.w    sek_per_track,d7
          mulu.w    #(4+512)/4,d7
          bra.s     fdj_dbra
fdj:      move.l    (a0)+,(a1)+         ;Track merken
fdj_dbra: dbra      d7,fdj

          bsr       read_track          ;Track wieder einlesen
          moveq     #-1,d7
          bsr       display_sektor

          moveq     #11,d7    ;Sektorcounter=11
          bra.s     sek_dbra
verify_sek:         tst.l     (a1)
          beq.s     verify_error1
          cmp.b     2(a1),d7  ;Sektor gefunden?
          bne.s     nxt_sek

          lea       track_buffer,a0
disk_loop:          tst.l     (a0)
          beq       verify_error2       ;Sektor nicht gefunden
          cmp.b     2(a0),d7
          bne.s     nxt_ss

          bsr       display_sektor

          addq.l    #4,a0
          addq.l    #4,a1     ;Miniheader Åberspringen

          moveq     #127,d5
verify_loop2:       cmpm.l    (a0)+,(a1)+
          bne.s     verify_error3
          dbra      d5,verify_loop2

sek_dbra: movea.l   hlpbuf,a1
          dbra      d7,verify_sek

          bra.s     schreib_ende        ;Alles Okay

nxt_ss:   lea       512+4(a0),a0
          bra.s     disk_loop

nxt_sek:  lea       512+4(a1),a1
          bra.s     verify_sek

schreib_ende:       movem.l   (sp)+,d0-d7/a0-a6
          move.w    d0,d5
          mulu.w    #512,d5   ;Anzahl zurÅckgeben
          rts       

verify_error1:      move.w    #-2,disk_error
          move.w    #$f0,d0
          bra       verify_error
verify_error2:      move.w    #-3,disk_error
          move.w    #$0ff0,d0
          bra       verify_error
verify_error3:      move.w    #-4,disk_error
          move.w    #$0f00,d0

verify_error:       tst.b     track_display
          beq.s     kein_display3

          bsr       flash_amiga

kein_display3:      lea       track_buffer,a1
          movea.l   hlpbuf,a0
          move.w    sek_per_track,d7
          mulu.w    #(4+512)/4,d7
          bra.s     fdj2_dbra
fdj2:     move.l    (a0)+,(a1)+         ;Track zurÅckschreiben
fdj2_dbra:          dbra      d7,fdj2

          subq.w    #1,retries
          bpl       write_again
          bra       schreib_ende

;Schreibt Track auf Diskette (Routine aus Amigacopy)
;-> D0.W: Anzahl an Bytes zu schreiben
;   D1.W: Synchronisiert mit Index oder nicht (<>0=Synchronisiert)
;   A5.L: Adresse
write_track:        movem.l   d2/d3,-(sp)
          move.w    d1,d2
          move.w    d0,d3
          bsr       test_change         ;Diskettenwechsel feststellen
          tst.l     d0
          bmi       write_error
          move.b    $bfe001,d0
          btst      #3,d0     ;Write Protect?
          bne       L00B8     ;->Fehler->RTS
          bra       write_error
L00B8:    lsr.w     #1,d3
          move.w    #2,$dff09c          ;Flags rÅcksetzen
          move.l    a5,$dff020          ;Adresse setzen
          move.w    #$8210,$dff096
          move.w    #$7f00,$dff09e
          move.w    #$8100,$dff09e
          cmpi.w    #$50,track          ;Track 80?
          bcs       L00B9
          move.w    #$a000,$dff09e      ;Sonst Precomp
L00B9:    move.w    #$4000,$dff024      ;DMA aus
          cmpi.w    #0,d2
          beq       L00BA     ;synchronisiertes Schreiben?
          bsr       wait_index
          tst.l     d0
          bmi       write_error
L00BA:    ori.w     #$c000,d3
          move.w    d3,$dff024          ;und schreiben
          move.w    d3,$dff024
          clr.l     d0
          move.l    #$00020000,d1
L00BB:    move.w    $dff01e,d2
          btst      #1,d2
          bne.s     L00BD
          subq.l    #1,d1
          bne.s     L00BB
write_error:        move.l    #-1,d0
L00BD:    move.w    #$4000,$dff024
          movem.l   (sp)+,d2/d3
          rts       


;Testet einen Diskettenwechsel:
test_change:        clr.l     d0
          move.b    $bfe001,d0
          btst      #2,d0
          bne       L0081
          move.l    #-1,d0
L0081:    rts       

;Wartet 6 Umdrehungen
wait_6:   movem.l   d0-d7/a0-a6,-(sp)

          moveq     #5,d7
index_loop:         bsr       wait_index
          dbra      d7,index_loop

          movem.l   (sp)+,d0-d7/a0-a6
          rts       

;Wartet auf den Index-Impuls:
wait_index:         move.l    d1,-(sp)
          clr.l     d0
          move.l    #$00030000,d1
          move.b    $bfdd00,d0
L0088:    move.b    $bfdd00,d0
          btst      #4,d0     ;Index Impuls da?
          bne.s     L0089     ;ja->zurÅck
          subi.l    #1,d1
          bne.s     L0088     ;weiter warten
          move.l    #-1,d0
L0089:    move.l    (sp)+,d1
          rts       


;Diese Routine codiert einen Track ins MFM-Format:
;-> A0.L: Adresse des unverschlÅsselten Tracks
;   A1.L: Adresse des MFM-Tracks
;<- D0.L: LÑnge des Tracks in Bytes
code_track:         movem.l   d1-d7/a0-a6,-(sp)

          move.w    #1000/4,d0
track_gap:          move.l    #$aaaaaaaa,(a1)+    ;LÅcke vor den Track machen
          dbra      d0,track_gap

          movea.l   a0,a2     ;Trackadresse merken
          moveq     #0,d7     ;Startsektor
sek_loop3:          movea.l   a2,a0     ;Trackadresse holen
sek_loop2:          tst.l     (a0)      ;Trackende erreicht?
          beq.s     code_end
          cmp.b     2(a0),d7  ;richtiger Sektor?
          bne.s     nxt_sektor2

          moveq     #11,d0
          sub.w     d7,d0     ;Anzahl an Sektoren bis zum Trackende
          move.b    d0,3(a0)  ;eintragen

          bsr.s     code_sektor         ;Diesen Sektor codieren
          addq.w    #1,d7     ;nÑchster Sektor
          cmpi.w    #12,d7
          blt.s     sek_loop3
          bra.s     code_end

nxt_sektor2:        lea       512+4(a0),a0
          bra.s     sek_loop2

code_end: move.l    a1,d0     ;Trackende
          addq.l    #2,d0     ;+2
          movem.l   (sp)+,d1-d7/a0-a6
          sub.l     a1,d0     ;-Trackstart=LÑnge
          rts       

;Diese Routine codiert einen Sektor ins MFM-Format
;-> A0.L: Adresse des uncodierten Sektors (wird erhîht)
;   A1.L: Adresse des MFM-Buffers (wird erhîht)
code_sektor:        movem.l   d0-d7/a2-a6,-(sp)

          movea.l   a1,a5     ;Adresse merken

          moveq     #7,d7
          moveq     #0,d0
code_0:   bsr       code_long ;Header lîschen
          dbra      d7,code_0

          movea.l   a5,a1     ;zurÅck auf Header
          moveq     #0,d0
          bsr       code_long ;$aaaaaaaa eintragen
          move.l    #$44894489,4(a5)    ;Sync eintragen

          move.l    (a0)+,d0  ;Sektorheader
          lea       8(a5),a1  ;Position des Headers
          bsr       code_long

          lea       8(a5),a2  ;Start des Headers
          moveq     #40,d0    ;40 Bytes
          bsr       calc_crc
          lea       48(a5),a1
          bsr       code_long ;CRC eintragen

          move.w    #512,d0   ;SektorlÑnge
          lea       64(a5),a1 ;Zeiger hinter Header
          bsr.s     code_block

          lea       64(a5),a2
          move.w    #1024,d0  ;LÑnge in MFM Format
          bsr       calc_crc

          lea       56(a5),a1
          bsr       code_long ;CRC eintragen

          lea       512(a0),a0          ;Sektor Åberspringen
          lea       64+1024(a5),a1      ;Header+MFM Åberspringen

          movem.l   (sp)+,d0-d7/a2-a6
          rts       

;Kodiert einen 512 Bytes groûen Datenblock ins MFM-Format
;-> A0.L: Adresse des unverschlÅsselten Sektors
;   A1.L: Adresse des MFM-Formats
;   D0.W: LÑnge des unverschlÅsselten in Bytes
code_block:         movem.l   d0-d7/a0-a6,-(sp)

          movea.l   a0,a2
          movea.l   a1,a3
          moveq     #0,d2
          move.w    d0,d2     ;BlocklÑnge
          move.w    d0,d1     ;BlocklÑnge
          lsl.w     #2,d1
          ori.w     #8,d1     ;Wert fÅr Blitterstart
          bsr       wait_blitter        ;warten bis Blitter fertig
          bsr.s     blitter_code
          movea.l   a3,a0     ;Zeiger auf codierten Block
          bsr       randsetzen
          adda.l    d2,a0     ;Zeiger auf 1. LÅcke
          bsr       randsetzen
          adda.l    d2,a0     ;Zeiger auf Blockende
          bsr       randsetzen

          movem.l   (sp)+,d0-d7/a0-a6
          rts       

;Diese Routine fÅhrt einen Blittertransfer durch (Blockkodierung)
;-> A0.L: Quelle
;   A1.L: Ziel
;   D0.W: LÑnge des Quelle
;   D1.W: Blitterstartwert
blitter_code:       movem.l   d2/d3/a2-a4,-(sp)

          moveq     #0,d2
          move.w    d0,d2
          move.w    d1,d3

          movea.l   a0,a2
          movea.l   a1,a3
          bsr       blitmask

          lea       $dff000,a4
          move.l    a2,$4c(a4)          ;Zeiger auf Source B
          move.l    a2,$50(a4)          ;-"- A
          move.l    a3,$54(a4)          ;Zeiger auf Destination
          move.w    #$1db1,$40(a4)      ;Blittercontrol 0
          move.w    #0,$42(a4)          ;Blittercontrol 1
          move.w    d3,d0
          bsr       start_blit
          move.l    a3,$4c(a4)          ;Zeiger auf Source B
          move.l    a2,$50(a4)          ;Zeiger auf Source A
          move.l    a3,$54(a4)          ;Zeiger auf Destination
          move.w    #$2d8c,$40(a4)      ;Blittercontrol 0
          move.w    d3,d0
          bsr.s     start_blit

          movea.l   a2,a0     ;Zeiger auf Quelle
          movea.l   a3,a1     ;Zeiger auf Ziel
          adda.l    d2,a0     ;Zeiger auf Ende der Quelle
          subq.l    #2,a0     ;Zeiger auf letztes Wort
          adda.l    d2,a1
          adda.l    d2,a1     ;Zeiger auf Ende des Ziel
          subq.l    #2,a1
          move.l    a0,$4c(a4)          ;Source B
          move.l    a0,$50(a4)          ;Source A
          move.l    a1,$54(a4)          ;Destination
          move.w    #$0db1,$40(a4)      ;Blittercontrol 0
          move.w    #$1002,$42(a4)      ;Blittercontrol 1
          move.w    d3,d0
          bsr.s     start_blit

          movea.l   a2,a0
          movea.l   a3,a1
          adda.l    d2,a1
          move.l    a1,$4c(a4)          ;Source B
          move.l    a0,$50(a4)          ;Source A
          move.l    a1,$54(a4)          ;Destination
          move.w    #$1d8c,$40(a4)      ;Blittercontrol 0
          move.w    #0,$42(a4)          ;Blittercontrol 1
          move.w    d3,d0
          bsr.s     start_blit

          movem.l   (sp)+,d2/d3/a2-a4
          rts       

;Maskenregister fÅr Blitter setzen
blitmask: lea       $dff000,a0
          moveq     #0,d0
          lea       $44(a0),a1          ;Zeiger auf 1. Maskword
          move.l    #-1,(a1)
          lea       $62(a0),a1          ;Zeiger auf Modulo
          move.l    d0,(a1)+
          move.w    d0,(a1)+  ;Blittermodulo setzen
          addq.l    #8,a1
          move.w    #$5555,(a1)         ;Blittersource C
          rts       

;Blitter starten und auf dessen Ende warten
start_blit:         move.w    d0,$dff058          ;GO!

wait_blitter:       btst      #6,$dff002
          bne.s     wait_blitter
          rts       


;Codiert ein Langwort ins MFM-Format:
;-> D0.L: Langwort
;   A1.L: Schreibadresse (wird erhîht)
code_long:          movem.l   d0-d7,-(sp)

          move.l    d0,d3
          lsr.l     #1,d0
          bsr.s     code_1
          move.l    d3,d0
          bsr.s     code_1
          bsr.s     randsetzen

          movem.l   (sp)+,d0-d7
          rts       

code_1:   andi.l    #$55555555,d0
          move.l    d0,d2
          eori.l    #$55555555,d2
          move.l    d2,d1
          lsl.l     #1,d2
          lsr.l     #1,d1
          bset      #31,d1
          and.l     d2,d1
          or.l      d1,d0
          btst      #0,-1(a1)
          beq.s     no_set87
          bclr      #31,d0
no_set87: move.l    d0,(a1)+
          rts       

randsetzen:         move.b    (a1),d0
          btst      #0,-1(a1)
          bne.s     kein_4
          btst      #6,d0
          bne.s     kein_6
          bset      #7,d0
          bra.s     kein_5
kein_4:   bclr      #7,d0
kein_5:   move.b    d0,(a1)
kein_6:   rts       

;Routine selektiert Laufwerk df0:.
select:   move.b    #%1111111,$bfd100   ;DF0: selektieren
          nop       
          nop       
          move.b    #%1110111,$bfd100   ;Motor an, DF0:
          nop       
          nop       
          bra       wait_ready          ;Auf Laufwerk warten

;Diese Routine deselektiert Laufwerk DF0:
deselect: move.b    #%11111111,$bfd100  ;DF0: deselektieren
          nop       
          nop       
          move.b    #%11110111,$bfd100  ;Motor aus
          rts       


;Routine fÑhrt einen bestimmten Track an und selektiert entspr. Seite
;-> D6.W: gewÅnschter Track
seek_track:         tst.w     track     ;Ist dies der 1. Aufruf?
          bpl.s     seek      ;Nein, direkt anfahren

          move.l    d6,-(sp)  ;gewÅnschter Track merken
          moveq     #0,d6
          bsr       seek      ;zuerst Restore
          move.l    (sp)+,d6


;FÑhrt Track an
;-> D6.W: Track
seek:     tst.b     track_display       ;(9) ab hier...
          beq       kein_display        ;(9) kein Display

          movem.l   d0-d7/a0-a6,-(sp)

          lea       track_nr,a0
          ext.l     d6
          divu.w    #100,d6
          addi.w    #'0',d6
          move.b    d6,(a0)+
          swap.w    d6
          ext.l     d6
          divu.w    #10,d6
          addi.w    #'0',d6
          move.b    d6,(a0)+
          swap.w    d6
          addi.w    #'0',d6
          move.b    d6,(a0)+

          lea       track_str,a0
          move.w    #280,d0
string_loop:        moveq     #0,d2
          move.b    (a0)+,d2  ;Zeichen holen
          beq.s     loop_ende

          move.w    d2,-(sp)  ;Zeichen
          clr.w     -(sp)     ;Y
          move.w    d0,-(sp)  ;X
          jsr       mcode1
          addq.l    #6,sp

          addq.w    #4,d0     ;eine Position weiter
          bra.s     string_loop

loop_ende:          movem.l   (sp)+,d0-d7/a0-a6   ;(9) ... bis hier

kein_display:       movem.l   d0-d7/a0-a6,-(sp)

          move.w    track(pc),d7        ;aktueller Track
          lsr.w     #1,d7     ;tatsÑchliche Tracknummer
          lsr.w     #1,d6     ;gewÅnschter Track
          bcs.s     untere_seite
          bset      #2,$bfd100          ;Obere Seite
          bra.s     seeken
untere_seite:       bclr      #2,$bfd100

seeken:   tst.w     d6        ;Track 0?
          beq.s     restore

seek_loop:          cmp.w     d7,d6     ;Sind wir schon auf Track?
          beq.s     seek_end
          bgt.s     nach_innen
          bsr.s     aussen
          subq.w    #1,d7
          bra.s     seek_loop
nach_innen:         bsr.s     innen
          addq.w    #1,d7
          bra.s     seek_loop

restore:  btst      #4,$bfe001          ;Kopf auf Spur 0?
          beq.s     seek_end
          bsr.s     aussen
          bra.s     restore

seek_end: movem.l   (sp)+,d0-d7/a0-a6
          move.w    d6,track  ;angekommen
          rts       

;Bewegt Kopf auf Track 80 zu:
innen:    movem.l   d0-d7/a0-a6,-(sp)
          bsr       wait_ready
          bclr      #1,$bfd100
          bclr      #0,$bfd100
          bset      #0,$bfd100
          bsr.s     minipause
          bsr       wait_ready
          movem.l   (sp)+,d0-d7/a0-a6
          rts       

;Bewegt Kopf auf Track 0 zu:
aussen:   movem.l   d0-d7/a0-a6,-(sp)
          bsr       wait_ready
          bset      #1,$bfd100
          bclr      #0,$bfd100
          bset      #0,$bfd100
          bsr.s     minipause
          bsr       wait_ready
          movem.l   (sp)+,d0-d7/a0-a6
          rts       

track_str:          .DC.b "TRACK: "     ;(9)
track_nr: .DC.b "xxx",0
sektor_str:         .DC.b "SEKTOR: "
sektor_nr:          .DC.b "xx",0

          .EVEN 

pause:    move.w    #$49d8,d7
loop2:    tst.w     $dff004   ;Waitstates einlegen
          dbra      d7,loop2
          rts       

minipause:          move.w    #$c4e,d7
mini2:    tst.w     $dff004   ;Waitstates einlegen
          dbra      d7,mini2
          rts       

wait_a_little:      move.l    d0,-(sp)
          move.w    #$0180,d0
wait77:   nop       
          tst.w     $dff004   ;Waitstates einlegen
          dbra      d0,wait77
          move.l    (sp)+,d0
          rts       

;Diese Routine wartet auf das Ende einer DMA-Floppyaktion
;<- D0.W: 0=Kein Timeout, -1=Timeout
wait_dma: movem.l   d1-d7/a0-a6,-(sp)

          move.l    #$00020000,d0
          move.w    #$0002,$dff09c
wait_loop:          move.w    $dff01e,d1
          btst      #1,d1
          bne.s     fertig56
          subq.l    #1,d0
          bne.s     wait_loop

fertig56: move.w    #$0000,$dff024      ;DMA ausschalten
          movem.l   (sp)+,d1-d7/a0-a6
          rts       

;Wartet auf die AusfÅhrung eines normalen Diskbefehls:
wait_ready:         btst      #5,$bfe001
          bne.s     wait_ready
          rts       

flash_amiga:        movem.l   d0-d7/a0-a6,-(sp)

          moveq     #6,d6
loop5:    move.w    d0,$dff180
          move.w    #0,$dff180
          dbra      d7,loop5
          dbra      d6,loop5

          move.w    #0,$dff180          ;Hintergrundfarbe schwarz

          movem.l   (sp)+,d0-d7/a0-a6
          rts       


disk_drin:          .DC.w -10 ;Welche Disk ist jetzt im Laufwerk? (Noch keine)
track:    .DC.w -1  ;Trackposition unbestimmt

;Zeigt den aktuellen Sektor an
display_sektor:     tst.b     track_display       ;(9) ab hier...
          beq       kein_display2       ;(9) kein Display

          movem.l   d0-d7/a0-a6,-(sp)

          lea       sektor_nr,a0
          move.w    d7,d6     ;Sektornummer
          ext.l     d6
          divu.w    #10,d6
          addi.w    #'0',d6
          move.b    d6,(a0)+
          swap.w    d6
          addi.w    #'0',d6
          move.b    d6,(a0)+

          lea       sektor_str,a0
          move.w    #280,d0
string_loop2:       moveq     #0,d2
          move.b    (a0)+,d2  ;Zeichen holen
          beq.s     loop_ende2

          move.w    d2,-(sp)  ;Zeichen
          move.w    #6,-(sp)  ;Y
          move.w    d0,-(sp)  ;X
          jsr       mcode1
          addq.l    #6,sp

          addq.w    #4,d0     ;eine Position weiter
          bra.s     string_loop2

loop_ende2:         movem.l   (sp)+,d0-d7/a0-a6   ;(9) ... bis hier
kein_display2:      rts       


;Konvertiert ein Grafikobjekt in ein gerÑtespezifisches Format:
;C-Defintition: convert_objekt(int ob_nr,OBJEKTE *obj);
mcode71:  link      a6,#0
          movem.l   d0-d7/a0-a5,-(sp)

          movea.l   10(a6),a0 ;Basis der Bibliothek
          move.w    8(a6),d0  ;Nummer des Objekts
          cmp.w     (a0)+,d0  ;Objekt noch in Bibliothek?
          bgt.s     zu_gross27          ;Nein->nichts zeichnen

          mulu.w    #20,d0    ;Adresse des Ob_Headers berechnen
          adda.l    d0,a0

          movea.l   10(a6),a1
          addq.l    #2,a1     ;Anzahl Åberspringen
          adda.l    (a0),a1   ;Beginn des Objektdaten

          move.w    10(a0),d6 ;Breite in Bytes
          lsr.w     #2,d6     ;/4
          move.w    d6,eins+2
          move.w    d6,d7
          add.w     d7,d7
          move.w    d7,zwei+2
          add.w     d6,d7
          move.w    d7,drei+2

          move.w    6(a0),d7  ;Hîhe des Objekts
          bra.s     neue_zeile
neue_loop:          move.w    10(a0),d6 ;Breite des Objekts in Bytes
          lea       bitfeld,a2          ;hier aufbauen
          bra.s     copy_dbra
copy_first:         move.b    (a1)+,(a2)+         ;Zeile kopieren
copy_dbra:          dbra      d6,copy_first
          movea.l   a1,a3     ;Ende merken

          move.w    10(a0),d6 ;Breite in Bytes
          suba.w    d6,a1
          suba.w    d6,a2     ;zurÅck zum Start
          lsr.w     #3,d6     ;Anzahl in BBs
          bra.s     kai_dbra
copy_kai: move.w    (a2)+,d0
          move.w    (a2)+,d1
          move.w    (a2)+,d2
          move.w    (a2)+,d3
eins:     move.w    d1,40(a1) ;Offsets werden reingepatcht
zwei:     move.w    d2,80(a1)
drei:     move.w    d3,120(a1)
          move.w    d0,(a1)+  ;und Planes umsortieren, gell Kai?
kai_dbra: dbra      d6,copy_kai
          movea.l   a3,a1     ;hier geht's weiter

neue_zeile:         dbra      d7,neue_loop

zu_gross27:         movem.l   (sp)+,d0-d7/a0-a5
          unlk      a6
          rts       

;Mouse_Off
mcode74:  

;Mouse_On()
mcode75:  rts       

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

fast_mem: .DS.l 1   ;Startadresse des Fast-Ram
fast_len: .DS.l 1   ;LÑnge des Fast-Ram

sync:     .DC.w 50  ;Voreingestellt 50 Hz

music_routine:      .DC.l 0
musik_an: .DC.w 0   ;Musik ist aus
first_init:         .DC.b 0
halts_maul:         .DC.b -1  ;Hippel darf spielen

          .EVEN 
track_display:      .DC.w 0
steuerzeichen:      .DS.w 1   ;(1) Zum Steuern der Bildes des Scrollers
vbl_ct:   .DC.l 50000         ;(11) VBL-Counter

          .BSS 
track_buffer:       .DS.b 13000         ;Zum Einlesen des Tracks
timeout:  .DS.w 1
disk_error:         .DS.w 1   ;Jeweiliger Diskfehler
retries:  .DS.w 1   ;Anzahl der Schreibversuche

tos_da:   .DS.w 1   ;Ist TOS aktiv?
_BasPag:  .DS.l 1   ;Adresse der Basepage
mem_strt: .DS.l 1   ;Adresse des freien Speichers
mem_len:  .DS.l 1   ;Grîûe des freien Speichers
errno:    .DS.w 1   ;Globale Fehlernummer (fÅr Library)

first_sek:          .DS.w 1
sek_per_track:      .DS.w 1
datei_laenge:       .DS.l 1   ;Die LÑnge der Datei in Bytes

digi_works:         .DS.w 1
anzahl_seq:         .DS.w 1
anz_start:          .DS.w 1
anzahl_zeilen:      .DS.w 1
seq_tabelle:        .DS.l 1
zeilen_tabelle:     .DS.l 1
naechste_zeile:     .DS.l 1
start_digi:         .DS.l 1
roundabout:         .DS.w 1
seq_counter:        .DS.w 1
digi_pos: .DS.l 1
digi_end: .DS.l 1
start_sequenz:      .DS.l 1

          .EVEN 

line_modus:         .DS.w 1
fuellmuster:        .DS.l 1   ;Adresse des Fuellmusters

fuell:    .DS.w 16  ;,-1
logbase:  .DS.l 1

oldmouse: .DS.l 1
old_mx:   .DS.b 1
old_my:   .DS.b 1
          .EVEN 
mousex:   .DS.w 1
mousey:   .DS.w 1
mdeltax:  .DS.w 1
mdeltay:  .DS.w 1

mouseform:          .DS.w 1
hot_spotx:          .DS.w 1
hot_spoty:          .DS.w 1

maus_sprite:        .DS.w 34  ;fÅr den Maussprite
spr_off:  .DS.w 2   ;Ende des Sprites

adrspalte:          .DS.l 1
scrollbase:         .DS.l 1
yscroller:          .DS.w 1
scrollbegin:        .DS.l 1
scrollpos:          .DS.l 1
delta_x:  .DS.w 1
font_start:         .DS.l 1
offset_start:       .DS.l 1
copyscroller:       .DS.w 1
scroll_buff:        .DS.l 1
scroll_akt:         .DS.w 1
char_basis:         .DS.l 1
buchstabe:          .DS.w 1

endsource:          .DS.l 1
pic_length:         .DS.w 1
quicksave:          .DS.l 6
bitfeld:  .DS.b 4000

vbl_queue:          .DS.l 10  ;10 Routinen in VBL

zeros:    .DS.w 16

gfxbase:  .DS.l 1   ;Basis der Grafics-Lib
oldcopper:          .DS.l 1   ;Adresse der alten Copperiste
old_i_2:  .DS.l 1
old_dma:  .DS.w 1   ;Alter DMA Einstellung

raster_start:       .DS.l 1   ;Beginn der Raster in rasters2

copperliste:        .DS.l 1   ;Adresse der aktiven Copperliste
hbl_system:         .DS.w anz_raster*(1+16)       ;Y-Position+Paletten
rasters1: .DS.w 50+(anz_raster*34)      ;dargestellte Rasterpalette
rasters2: .DS.w 50+(anz_raster*34)      ;Aufbaurastertabelle
end_rasters:                  ;Zum Berechnen der LÑnge

rasters:  .DS.l 1
old_i_3:  .DS.l 1   ;Alter IRQ-Level 3 Vektor

amiga_raw:          .DS.w 1

offsets:  .DS.w 6

;BSS fÅr mcode49 (Blitter_Copy):
x1_bb:    .DS.w 1
x2_bb:    .DS.w 1
x3_bb:    .DS.w 1
x4_bb:    .DS.w 1
quelle_breite:      .DS.w 1
ziel_breite:        .DS.w 1
          .EVEN 
          .DS.b 4
blit_maske:         .DS.b 50

          .EVEN 
          .END 
