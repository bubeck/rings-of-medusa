
                       /* Diashow von Crown ... */

                /* Crown - The Ultimate Simulation */
     /* Code von Till Bubeck, Ziegeleistr. 28, 7056 Weinstadt */
                           /* 07151-66437 */

                        /* All rights reserved */



#define ATARI                       /* Rechnertyp */

#include <compiler.h>               /* Definiert den Compilertyp */
#include <stdio.h>
#include <osbind.h>
#include <gemdefs.h>

#include <geminit.h>
#include <ctype.h>
#include <math.h>
#include <line_a.h>
#include <linea_fn.h>

#include "FILENAME.C"
#include "strings.c"
#include "const.c"              /* Alle Konstanten holen */
#include "grund.c"              /* Konstantendefintion der Untergründe */
#include "typedef.c"            /* Typendefinitionen */
#include "global.c"             /* Variablendeklarationen */

int laufwerk;
int fh;

main()
{
  DTA *d;
  char filename[30];
  long ssp;
  long *poi;
  long i;

  strcpy(filename,"MEDUSA.PI?");

    printf("%cE     This is a Slideshow of:\n\n",27);
    printf("        Rings of Medusa\n\n");
    printf("   Copyright 1989 by Starbyte\n\n");
    for(i=0;i<3000000/2;i++) ;

  ssp=Super(0L);
  poi=(long *)0x4ce;
  if (*poi==0) {
    laufwerk='A';            /* Auto-Ordner */
    }
  else {
    Cconws("Bitte Laufwerk angeben: ");
    do {
      laufwerk=Cconin();
      if (laufwerk>='a' && laufwerk<='z') laufwerk=(laufwerk-'a')+'A';
      } while(!(laufwerk>='A' && laufwerk<'P'));
     }
  fh=Fopen("MUZAK.IMG",0);
  Fread(fh,8192L,0x7e000);
  Fclose(fh);

  asm {     move.l  #0x7e012,0x4d2
            moveq   #2,d0
            jsr     0x7e000
            }

  Super(ssp);

  init_atari();
  alloc_mem();
  rez = Getrez();               /* momentane Auflösungh holen */
  if (rez>0) lo_res();          /* Wenn nicht Low-Res, dann einschalten */
  hbl_init();

  do {
    d=(DTA *)Fgetdta();

    if (Fsfirst(filename,0)>=0)
    do {
      load_n_show(d->name);
      } while(Fsnext()>=0);
    } while(TRUE);
}

init_atari()
{
  /* Diese Funktion initialisiert das ST-Betriebsystem, sowie die
     Hardware */

  init_hardware();             /* Intialisiert die Hardware */
  init_gem();                  /* Öffnet GEM Arbeitsumgebung */

  blitter=check_blitter();     /* Schaut, ob Blitter vorhanden */
  if (blitter) {
    Blitmode(0);                  /* Blitter ab(!)schalten */
    }
}

init_hardware()
{
  /* Initialisiert die Hardware */

  sync=get_sync();               /* Get_Sync() */
  mcode3();                     /* Init_Hardware */
}

init_gem()
{
  /* Öffnet Gem-Workstation und stellt Default ein */

  open_work();

  vswr_mode(handle,1);              /* Replace Mode */
  graf_mouse(M_OFF,0);              /* Betriebsystemmaus weg */
  la_init();                        /* LINE_A initialisieren */
  Kbshift((int)Kbshift(-1) | 16);
  vsl_width(handle,1);              /* Dünne Linie */
}

alloc_mem()
{
  /* holt den benötigten Speicher */
  long i;
  char *poi;

  pack_buf=(char *)Malloc(32000L);           /* Ladebereich für gepackte Bilder */
  hlpbuf=(char *)Malloc(32000L);           /* Hilfspeicher */
  scr1 = oldbase=Logbase();                          /* 1. Bildschirm */
  scr2_mem = Malloc(32000L+256L);            /* 2. Bildschirm */
  scr2 = (scr2_mem/256+1)*256L;           /* auf Baseanfang bringen */
}

load_n_show(filename)
char filename[];
{
  int fh;
  long i;

  fh=Fopen(filename,0);
  if (fh>=0) {
    Fread(fh,20L,save_1);           /* HBL-Offsets */
    Fread(fh,320L,fpals);             /* 10 HBL Paletten */
    Fread(fh,32L,fvblpal);            /* Eine VBL-Palette */
    Fread(fh,32000L,pack_buf);
    entpack(pack_buf,scr2);
    Fclose(fh);
    fade_out();

    copy_zeilen(scr2,scr1,200);         /* Bild übertragen */
    for(i=9;i>=0;i--) offlist[i]=save_1[i];

    fade_in();

    for(i=0;i<25000;i++) {
      if (Cconis()) {
        if ((int)Crawcin()==27) programmende();
        break;
        }
      }
    }
}

lo_res()
{
  Setscreen(-1L,-1L,0);
}

programmende()
{
  long ssp;

  Mfree(hlpbuf);          /* Speicher wieder freigeben */
  Mfree(scr2_mem);
  Mfree(pack_buf);

  hbl_exit();               /* Rasterirq's wieder freigeben */
  exit_atari();             /* meldet Crown beim Atari ab */

  ssp=Super(0L);
  asm {
                clr.l   0x4d2
                move.l  #0x08080000,0xff8800
                move.l  #0x09090000,0xff8800
                move.l  #0x0a0a0000,0xff8800
                }
  Super(ssp);
  Pterm0();                 /* und zurück zum Desktop */
}

exit_atari()
{
  /* Diese Funktion meldet Crown beim Atari-OS ab, und gibt alle
     belegten IRQ's etc. wieder frei */

  graf_mouse(M_ON,0);               /* Betriebssystemmaus an */

  clear_interrupts();               /* keine HBL's mehr */
  Vsync();

  Setcolor(0, 0x777);
  Setcolor(1, 0x700);
  Setcolor(2, 0x070);               /* Farben setzen (unwichtig) */
  Setcolor(3, 0x000);
  Setcolor(15,0x000);

  if (rez>0) Setscreen(-1L,-1L,1);  /* Midres einschalten */
  Setscreen(oldbase,oldbase,-1);    /* alte Bildschirmbasis setzen */

  Kbshift((int)Kbshift(-1) & 0xffef);
  close_work();                     /* GEM-Workstation schliessen */
}

alert()
{ }
