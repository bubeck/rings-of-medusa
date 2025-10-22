    /*

		                     Return of Medusa (Rings of Medusa II)

Programm: Till Bubeck, Ziegeleistr. 28, 7056 Weinstadt (Wochenende und Semesterferien)
									Tel: 07151-66437
					bzw. Till Bubeck, Schloßbergstr. 9, 7400 Tübingen (während des Semsters)
									Tel: 07071-49612
Grafik:	  Torsten Zimmermann, Steinackerstr. 12, 7520 Bruchsal
									Tel: 07251-84170
Musik:		Jochen Hippel, Flörsheimerstr. 27, 6719 Bischheim
									Tel: 06352-8602

Konvertierungen:

Amiga und ST: Till Bubeck, s.o.
PC-Version: 	C: Martin Kreutz, Taubenstr. 61, 4300 Essen 17, Tel: 0201/570036
							Mcode: Volker Zinke, Weuenstraße 36, 4300 Essen, Tel: 0201/623465
C-64:					Steve Kups, Föhrenstr. 13, 8800 Ansbach, Tel: 0981-13771

Sonstige:

Co-Programmierer: Christian Weber, Bruggerweg 2, CH-8037 Zürich, Tel: 0041-12726197
Story: Matthias Krenzel, Brüderstr. 61, 5630 Remscheid 1, Tel.: 02191-75944
Bunkerdesign:	Peter Richter, Tel: 0201-743432
Vertrieb: Starbyte Software, Nordring 71, 4630 Bochum 1, Tel: 0234/680460			
Federal Express: 07150-30070, Kun-Nr.: 14301012

Promotion: Tronicverlag GmbH, ASM-Redaktion, z.Hd. Eva Hoogh, Postfach, 3440 Eschwege,
					 Tel.: 05651-30011							
					 Amiga Joker, z.Hd. Herr Labiner, Untere Parkstr. 67, 8013 Haar b. München
					 Markt & Technik Verlag, Red. Powerplay, z.Hd. Herrn Weitz,
											 Hans-Pinsel-Str. 2 8013 Haar b. München

																							*/
																							
																							
#include "version.c"								/* Versionsnummer */

/* Je nach Sprache die entsprechenden Strings einbinden: */
#if (SPRACHE==_DEUTSCH)
  #include "DEUTSCH.C"
#elif (SPRACHE==_ENGLISCH)
  #include "ENGLISCH.C"
#elif (SPRACHE==_FRANZ)
  #include "FRANZ.C"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <tos.h>										/* Turbo C 2.0 */
#include <setjmp.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "filename.c"
#include "const.c"              /* Alle Konstanten holen */
#include "grund.c"              /* Konstantendefintion der Untergründe */
#include "typedef.c"            /* Typendefinitionen */
#include "global.c"             /* Variablendeklarationen */
#include "prototyp.c"           /* Alle Funktionsprototypen */
#include "mcode.c"							/* Schnittstelle zum Assembler */

unsigned long seed;                     /* Startwert der Zufallszahlen */
FLAG peilsound=FALSE;

/* Start: */

void main()
{
  int *daypoi;
  long *moneypoi,*heutepoi,*gesamtpoi;

  save_version=0x005;       						/* Version des Savefiles */

  init_medusa();             /* Ramdisk, Speicher, Files etc. */
  setjmp(restart);          /* Hier Einsprungpunkt für erneutes Starten */

  re_initialize();          /* Den Rest voll initliaisieren */

	dungeon(21);							/* Startbunker */
		
  for(;;) {
    hol_maus();                  /* Mauskoordinaten holen */
    check_time();               /* Schaut, ob schon ein Tag vorbei ist */

    if (mk == 0) {
      if (!auf_schiff && ground_num==LICHTER_WALD) sn[0]=0;  /* Sprite an */
      }
		else if (my>169) button_leiste();

    if (button==SEARCH) such_rohstoffe();
    if (button==DIG) mach_mine();
    if (button==PEILUNG) peilung();
    if (button==EDITOR) {
      editor();
      land_an();
      }

    if (map==SEA) sea_move();                 /* SPieler bewegt sich auf dem Meer */

    if (mk == 1) {           /* Knopf gedrückt */
	    if ((mx != sx[0]) || (my != sy[0])) {  		/* Maus woanders ? */
        update_coordinates();     					/* Grenze angelaufen */
        get_ground();            					/* Holt die Untergründe der Sprites */
        test_ground();  				        		/* testet den Untergrund auf Wasser etc. */
        if(sx[0] != sxalt[0] || sy[0] != syalt[0]) {   /* Figur bewegt? */
          show_ground();               				/* Untergrund, Zeit, etc. zeigen */
          }

        test_mine();        /* Ermittelt, ob Spieler in Mine will */
        test_city();        /* Ermittelt ob city angelaufen -> citynum */
				test_bunker();			/* -'- Bunker */
        }
      else {             /* Figur nicht bewegt */
        if (!auf_schiff && ground_num==LICHTER_WALD) sn[0]=0;  /* Sprite an */
        }
      }

    if (citycheat) {
      citynum=0;
      citycheat=FALSE;
      }
    if(citynum>=0) {
#ifdef COPYPROT
			char *pw;									
			
			pw=password-100;
      if (*(pw+100)!=0) city();			/* Passworteingabe vorhanden? */
#else
			city();
#endif
      prep_date();
      }

    simulation();           /* Gegnerische Armeen bewegen, Vulkane etc. */
    test_fight();           /* prüfen, ob Spieler und Gegner zusammentreffen */
    check_cheat();          /* Testet, ob Cheateingabe gemacht wurde */
    button=NOTHING;
    test_untergrund();      /* Prüft ob Spieler im Sumpf oder verhexten Wald */
    draw_mobs();            /* Sprites auf neuem Screen zeichnen */
    draw_peilgeraete();     /* Peilgeraete rotieren */
    swap_screens();         /* Screens umschalten */
    delete_peilgeraete();   /* Peilgeräte weglöschen */
    delete_mobs();          /* Alte Sprites löschen */
    }
}

void draw_peilgeraete()
{
  /* Rotiert die Strahlen des Funkpeilgerätes, falls diese eingeschaltet
     sind.. */
  int i,j;
 	int old_line_mode;
	void *old_logbase;
  KOORD *ziel;                  /* Zielpunkt des Strahls */

	old_logbase=logbase;
	logbase=scr2;									/* Auf Scr2 zeichnen */

  for(i=0;i<2;i++)
    if (peilgeraet[i].on) {         /* Gerät eingeschaltet? */
      if (scr2==scrn_1) ziel=&peilgeraet[i].dest_1;
      else ziel=&peilgeraet[i].dest_2;

			old_line_mode=line_mode(1);			/* EOR */

      peilgeraet[i].ziel.x+=1*peilgeraet[i].dx;
      peilgeraet[i].ziel.y+=1*peilgeraet[i].dy;

      if (peilgeraet[i].ziel.x==peilgeraet[i].x &&
             peilgeraet[i].ziel.y==leiste_y) {
        peilgeraet[i].umdrehungen++;
        if (peilgeraet[i].umdrehungen==2) {
          peilgeraet[i].on=FALSE;               /* abschalten */
					if (!peilgeraet[0].on && !peilgeraet[1].on) {		/* Beide aus -> Sound weg */
						hippel_sfx(FX_AUS);
						peilsound=FALSE;					/* Kein Peilsound mehr */
						}
					}
        }

      if (peilgeraet[i].ziel.x>319) {
        peilgeraet[i].ziel.x=319;
        peilgeraet[i].dx=0;
        peilgeraet[i].dy=1;
        }
      if (peilgeraet[i].ziel.y>167) {
        peilgeraet[i].ziel.y=167;
        peilgeraet[i].dx=-1;
        peilgeraet[i].dy=0;
        }
      if (peilgeraet[i].ziel.x<0) {
        peilgeraet[i].ziel.x=0;
        peilgeraet[i].dx=0;
        peilgeraet[i].dy=-1;
        }
      if (peilgeraet[i].ziel.y<leiste_y) {
        peilgeraet[i].ziel.y=leiste_y;
        peilgeraet[i].dx=1;
        peilgeraet[i].dy=0;
        }
      ziel->x=peilgeraet[i].ziel.x;
      ziel->y=peilgeraet[i].ziel.y;
      line(15,peilgeraet[i].x,peilgeraet[i].y,ziel->x,ziel->y);

      if (peilgeraet[i].schatz.x==ziel->x &&
             peilgeraet[i].schatz.y==ziel->y) {
        copy_screen(scr2,pack_buf);         /* Screen retten */
        swap_screens();                            /* Auf Screen2 schalten */
        line_mode(0);															/* Replace */
				hippel_sfx(ECHOLOT);
				 
        for(j=0;j<10;j++) {
          line(2,peilgeraet[i].x,peilgeraet[i].y,ziel->x,ziel->y);
          wait_once(1);
          line(4,peilgeraet[i].x,peilgeraet[i].y,ziel->x,ziel->y);
          wait_once(1);
          }
        swap_screens();                            /* zurückschalten */
        copy_screen(pack_buf,scr2);         /* restaurieren */
				logbase=scr2;
				hippel_sfx(PEILSTRAHL);
        }

			line_mode(old_line_mode);				/* Alten Modus wieder setzen */
      }
	logbase=old_logbase;
}

void delete_peilgeraete()
{
  /* Löscht die Strahlen des Funkpeilgerätes, falls diese eingeschaltet
     sind.. */
  int i;
	int old_line_mode;						/* Alter Modus der Line-Funktion */
	void *old_logbase;
  KOORD *ziel;                  /* Zielpunkt des Strahls */

	old_logbase=logbase;
	logbase=scr2;														/* Auf Screen2 zeichnen */

  for(i=1;i>=0;i--) {
    if (scr2==scrn_1) ziel=&peilgeraet[i].dest_1;
    else ziel=&peilgeraet[i].dest_2;

    if (ziel->x>=0) {                   /* Strahl weglöschen */
			old_line_mode=line_mode(1);			/* EOR-MODUS */
      line(15,peilgeraet[i].x,peilgeraet[i].y,ziel->x,ziel->y);
      ziel->x=-1;                       /* Strahl ungültig */
      line_mode(old_line_mode);				/* Alten Modus wieder setzen */
      }
    }
	logbase=old_logbase;
}

void sea_move()
{
  /* Spieler ist jetzt auf dem offenen Meer */
	int old_sync;
	
	old_sync=sync;	
	sync=1;													/* 1 VBL pro Bild */
	
  redraw_buttons(EDITOR|PAUSE);
  copy_buttons();

  for(;;) {                     /* Solange, bis break kommt */
    hol_maus();                  /* Mauskoordinaten holen */
    check_time();               /* Schaut, ob schon ein Tag vorbei ist */

    if (button==EDITOR) {
      editor();
      land_an();
      redraw_buttons(EDITOR|PAUSE);
      copy_buttons();
      }

    if (map!=SEA) break;                 /* Spieler bewegt sich nicht mehr auf dem Meer */

		if (my>169) button_leiste();

    if (my<169 && (mx != sx[0]) || (my != sy[0])) {  /* Maus woanders ? */
      update_coordinates();     /* Grenze angelaufen */
      test_ground();            /* testet den Untergrund auf Wasser etc. */
      show_pos();               /* zeigt die Position des Spielers an */
      }

    button=NOTHING;
		draw_shapes();						/* Alles Sprites zeichnen */
    }

	sync=old_sync;								/* Alte Geschwindigkeit einstellen */
	
  redraw_buttons(EDITOR|PEILUNG|PAUSE);
  copy_buttons();
}

int rnd_winkel()
{
  /* Berechnet eine neue 'Zufallszahl' aus der alten, dabei sind aber
     keine wirklichen Zufallselemente im Spiel, damit immer die selben
     Werte errechnet werden. */

  float random;

  seed=(5L*seed+7227L) % 2345L;
  random=seed/2345.0;
  random*=90.0;
  return((int)random);
}

void peilung()
{
  /* Hiermit kann der Spieler ein Peilgerät setzen */
  int nr,i;                   /* Nummer des Peilgerätes */
  register int x,y;
  int schatz_x,schatz_y;                /* Pixelkoordinaten des Schatzes */
  double winkel,radius;
	int bunker_zahl,bunker_anzahl;
	int bun_nr1,bun_nr2;

	if (ycargo_menge[11]<=0) alert(romstr249);
	else if (bunker_nr[map][0]<128 || bunker_nr[map][1]<128) {		/* wenigstens 1 Bunker? */
		if (bunker_nr[map][0]<128 && bunker_nr[map][1]<128) {		/* Zwei Eingänge -> auswählen */
			bun_nr1=bunker_nr[map][0];								/* Nummer des 1. Bunkers */
			bun_nr2=bunker_nr[map][1];								/* Nummer des 2. Bunkers */
			if (!bunker_bekannt[bun_nr1] && !bunker_bekannt[bun_nr2])		/* Beide Bunker unbekannt */
				bunker_zahl=zufall(2);								/* Einen von beiden wählen */
			else {
				if (!bunker_bekannt[bun_nr1]) bunker_zahl=0;			/* Bunker 1 unbekannt */
				else bunker_zahl=1;								/* Sonst Bunker 2 nehmen */
				}
			}
		else {
			if (bunker_nr[map][0]<128) bunker_zahl=0;			/* Bunker 1 betretbar? */
			else bunker_zahl=1;								/* sonst ist es Bunker 2 */
			}
					
	  for(x=0;x<80;x++)
	    for(y=0;y<36;y++)
	      if (get_raster(x,y)==BUNKER_1+bunker_zahl) {
	        schatz_x=x*4+2;                           /* Koordinaten des Bunkers in */
	 	      schatz_y=y*4+25+2;                        /* Pixeln */
	   	    goto raus;
	     	  }
	  return;                                         /* Kein Schatz mehr da */
	
	raus:
	
	  nr=-1;
	  for(i=1;i>=0;i--)
	    if (!peilgeraet[i].on) nr=i;        /* Peilgerät an ? */
	
	  if (nr==-1) return;                   /* Beide Peilgeräte sind an */

		ycargo_menge[11]--;							/* Einen Quarz weniger */
			
	  seed=map*233.43;                      /* Startwert für Zufallszahl */
	  for(i=0;i<10;i++) rnd_winkel();       /* ein bißchen zufallen... */
	
	  winkel=(rnd_winkel()+120.0*i)*(2.0*PI/360.0);               /* 0..90° */
	
	  radius=10.0+rnd_winkel()/6.428;                       /* 10..24 */
	  schatz_x+=(int)(radius*cos(winkel));
	  schatz_y+=(int)(radius*sin(winkel));
	  if (schatz_x<0)   schatz_x=0;
	  if (schatz_x>318) schatz_x=318;
	  if (schatz_y<leiste_y)  schatz_y=leiste_y;
	  if (schatz_y>166) schatz_y=166;
	
	  peilgeraet[nr].x=sx[0];               /* Standort des Peilgerätes */
	  peilgeraet[nr].y=sy[0];
	  peilgeraet[nr].ziel.x=sx[0];
	  peilgeraet[nr].ziel.y=leiste_y;		/* Senkrecht starten */
	  peilgeraet[nr].dx=1;
	  peilgeraet[nr].dy=0;
	  peilgeraet[nr].on=TRUE;
	  peilgeraet[nr].dest_1.x=-1;           /* Keine Strahlen da */
	  peilgeraet[nr].dest_2.x=-1;
	  peilgeraet[nr].umdrehungen=0;         /* Noch keine Umdrehung */
	
	  if (schatz_x==sx[0]) {                /* Spieler auf gleicher X Koordinate */
	    peilgeraet[nr].schatz.x=sx[0];
	    if (schatz_y>sy[0]) peilgeraet[nr].schatz.y=167;        /* oberer/unterer? */
	    if (schatz_y<sy[0]) peilgeraet[nr].schatz.y=leiste_y;
	    }
	  if (schatz_y==sy[0]) {                    /* gleiche Y-Koordinate */
	    peilgeraet[nr].schatz.y=sy[0];
	    if (schatz_x<sx[0]) peilgeraet[nr].schatz.x=0;
	    if (schatz_x>sx[0]) peilgeraet[nr].schatz.x=319;
	    /* Beides gleich -> keine Anzeige: */
	    if (schatz_x==sx[0]) peilgeraet[nr].schatz.x=peilgeraet[nr].schatz.y=-1;
	    }
	
	  if (schatz_x<sx[0]) {                   /* Schatz links von Spieler */
	    peilgeraet[nr].schatz.x=0;
	    /* Schnittpunkt mit x=0 berechnen: (siehe Lineare Algebra...) */
	    peilgeraet[nr].schatz.y=sy[0]+sx[0]/(double)(sx[0]-schatz_x)*
	              (double)(schatz_y-sy[0]);
	    }
	  if (schatz_x>sx[0]) {                   /* Schatz rechts von Spieler */
	    peilgeraet[nr].schatz.x=319;
	    /* Schnittpunkt mit x=319 berechnen: */
	    peilgeraet[nr].schatz.y=sy[0]+(sx[0]-319)/(double)(sx[0]-schatz_x)*
	              (double)(schatz_y-sy[0]);
	    }
	
	  /* Clippen... */
	  if (schatz_x!=sx[0]) {
	    if (peilgeraet[nr].schatz.y<=leiste_y) {                /* Am oberen Rand */
	      peilgeraet[nr].schatz.x=sx[0]+(sy[0]-leiste_y)/(double)(sy[0]-schatz_y)*
	              (double)(schatz_x-sx[0]);
	      peilgeraet[nr].schatz.y=leiste_y;
	      }
	    if (peilgeraet[nr].schatz.y>167) {                /* Unterer Rand */
	      peilgeraet[nr].schatz.x=sx[0]+(sy[0]-167)/(double)(sy[0]-schatz_y)*
	              (double)(schatz_x-sx[0]);
	      peilgeraet[nr].schatz.y=167;
	      }
	    }
		hippel_sfx(PEILSTRAHL);
		peilsound=TRUE;									/* Peilsound ist zu hören */
		}
	else alert(romstr250);
}

void peilgeraet_aus()
{
	/* Schaltet die Peilgeräte aus */
	
  peilgeraet[0].on=peilgeraet[1].on=FALSE;            /* Peilgeräte aus */
  peilgeraet[0].dest_1.x=peilgeraet[0].dest_2.x=-1;   /* nicht neu gezeichnet */
  peilgeraet[1].dest_1.x=peilgeraet[1].dest_2.x=-1;

	if (peilsound) {							/* Ist Peilsound an? */
		hippel_sfx(FX_AUS);				/* dann abschalten */
		peilsound=FALSE;
		}
}

void such_rohstoffe()
{
  /* Spieler schickt seine Pioneere aus, damit sie Rohstoffe finden */
  int i;
  long pio_zahl;                /* Anzahl der Pioneere */
	void *oldlogbase;
	int oldmode;
	int x1,y1,x2,y2;
	int zeichnen;
	FLAG rahmen_da=FALSE;
	int radius;
	
  clear_buttons(TRUE);          /* nur Veränderung */
  copy_buttons();               /* Leiste auf 2. Screen */

  alert(romstr251);

  pio_zahl=armeeteil[0][3];
  armeeteil[0][3]=0L;           /* Alle Pioniere sind weg */
  armeegesamt[0]-=pio_zahl;     /* abziehen, wegen evtl. Kampf */
  writexy_anzeige();            /* Anzeigen */

	radius=pio_zahl/25;
	if (radius>20) radius=20;
	
	x1=sx[0]-radius;
	y1=sy[0]-radius;
	x2=x1+radius*2;
	y2=y1+radius*2;
	if (x1<0) x1=0;
	if (y1<25) y1=25;
	if (x2>319) x2=319;
	if (y2>167) y2=167;
	
	oldmode=line_mode(0);							/* Replace */
	copy_screen(hlpbuf,pack_buf+64000L);
	
  for(i=0;i<400;i++) {
    if (i%38==0) {
      clear_buttons(TRUE);
      copy_buttons();
      }
    if (i%38==19) {
      redraw_buttons(SEARCH);
      copy_buttons();
      }
		if (i%20==0) {
			zeichnen=2;									/* Noch 2 mal zeichnen */
			rahmen_da=!rahmen_da;
			}
			
    hol_maus();
    check_time();               /* Schaut, ob schon ein Tag vorbei ist */

    if (mk == 0) {
      if (!auf_schiff && ground_num==LICHTER_WALD) sn[0]=0;
      }
		else
			if (my>169) button_leiste();

    simulation();           /* Gegnerische Armeen bewegen, Vulkane etc. */
    test_fight();           /* prüfen, ob Spieler und Gegner zusammentreffen */

		if (--zeichnen>=0) {
			if (rahmen_da) {
				oldlogbase=logbase;
				logbase=scr2;
		
				rahmen(0,x1,y1,x2,y2);
				logbase=oldlogbase;
				}
			else cpy_raster(pack_buf+64000L,scr2,x1,y1,x2,y2,x1,y1);
			cpy_raster(scr2,hlpbuf,x1,y1,x2,y2,x1,y1);
			}
					
    draw_mobs();            /* Sprites auf neuem Screen zeichnen */
    draw_peilgeraete();     /* Peilgeraete rotieren */
    swap_screens();         /* Screens umschalten */
    delete_peilgeraete();   /* Peilgeräte weglöschen */
    delete_mobs();          /* Alte Sprites löschen */
    }

	line_mode(oldmode);

  such_roh(pio_zahl);       /* Schauen, ob Rohstoffe da sind */

  armeegesamt[0]+=pio_zahl;     /* Pioniere sind wieder bei der Armee */
  armeeteil[0][3]=pio_zahl;
  writexy_anzeige();              /* ggf. anzeigen */

  if (auf_schiff) redraw_buttons(PAUSE|EDITOR|PEILUNG);   /* Wird hier was geändert, dann */
  else redraw_buttons(PAUSE|EDITOR|SEARCH|DIG|PEILUNG);   /* auch in sea_move()! */
                                    /* und in re_initialize() !!! */
  copy_buttons();                   /* und auf 2. Screen */
  button=NOTHING;
}

void such_roh(pio_zahl)
long pio_zahl;
{
  /* Versucht Rohstoffe in der Umgebung des Spielers zu suchen */
  FLAG found1,found2,found3,found4;	/* Wurde was gefunden? */
  int radius;           				/* Radius in dem gefunden wird */

  radius=pio_zahl/100L;         /* 100 Pios für ein Rasterkästchen */
  if (radius>5) radius=5;       /* max. 5 Einheiten */
  found1=such_roh2(radius,EISEN,GOLD);       /* Eisen .. Gold suchen */

  radius=pio_zahl/300L;         /* 200 Pios für ein Rasterkästchen */
  if (radius>2) radius=2;
  found2=such_roh2(radius,SCHATZ,SCHATZ);      /* Schatz suchen */
  found3=such_roh2(radius,GIMMIG,GIMMIG);      /* Gimmicks suchen */

	radius=pio_zahl/150L;							/* 300 Pios für ein Rasterkästchen */
	if (radius>3) radius=3;
	found4=such_roh2(radius,BUNKER_1,BUNKER_2);				/* Bunker suchen */
	
  if (!found1 && !found2 && !found3 && !found4)
    alert(romstr252);
}


FLAG such_roh2(radius,start_wert,end_wert)
int radius;                 /* Radius in dem gesucht wird */
int start_wert,end_wert;    /* Sachen, die gefunden werden */
{
  int x,y;
  int start_x,start_y,end_x,end_y;
  int rohstoff;
  FLAG found=FALSE;
	int bun_nr;
	
  x=sx[0]/4;            /* In rasterkoordinaten wandeln */
  y=(sy[0]-25)/4;
  start_x=x-radius;
  start_y=y-radius;
  end_x=x+radius;
  end_y=y+radius;
  if (start_x<2) start_x=2;
  if (start_y<0) start_y=0;
  if (end_x>76) end_x=76;
  if (end_y>35) end_y=35;
  for(x=start_x;x<=end_x;x++) {
    for(y=start_y;y<=end_y;y++) {
      rohstoff=get_raster(x,y);
      if (rohstoff>=start_wert && rohstoff<=end_wert)
        if (!is_mine(map,x,y)) {
					if (start_wert==EISEN && end_wert==GOLD) {			/* Suche nach Eisen..Gold */
						if (rohstoff==GIMMIG) continue;					/* Dann Gimmick überspringen */
						}
	        if (rohstoff==SCHATZ || rohstoff==GIMMIG) {
						found=TRUE;
            alert(build(romstr253,calc_pos(pos_welt,x*4+2,y*4+25+2)));
						}
          else if (rohstoff>=BUNKER_1 && rohstoff<=BUNKER_2) {
						bun_nr=bunker_nr[map][rohstoff-BUNKER_1];
						if (bun_nr<128) {
							found=TRUE;
							alert(build(romstr254,calc_pos(pos_welt,x*4+2,y*4+25+2)));
							bunker_bekannt[bun_nr]=TRUE;
							}
						}				
          else {
						alert(build(romstr255,metal_name[rohstoff],calc_pos(pos_welt,x*4+2,y*4+25+2)));
						found=TRUE;
						}
          }
      }
    }
  return(found);
}

void mach_mine()
{
  /* Spieler will hier eine Mine errichten */
	int land;
	
  if (money>30000L) {                   		/* Mine kostet 30000 */
    if (yes_no(build(romstr256,str(0,money)))) {
      money-=30000L;      	     	         /* Kosten */
      if ((land=get_untergrund(sx[0],sy[0]))==SCHATZ || land==GIMMIG) schatz_found();
      else {
        if (minenzahl>MINE_MAX) {
          alert(romstr257);
          money+=30000L;                /* Geld zurückgeben */
          }
        else neue_mine();
        }
      }
    }
  else alert(romstr258);

  button=NOTHING;
}

void schatz_found()
{
  /* Der Spieler hat einen Schatz gefunden ! */
  long wert;

	wert=zufall_long(100000L)+20000L;
  alert(build(romstr259,str(0,wert)));
  money+=wert;

  ground_change(EBENE,map,sx[0],sy[0]);
  put_untergrund(EBENE,sx[0],sy[0]);          	/* Schatz ist weg */
}

void neue_mine()
{
  /* Spieler will hier eine neue Mine errichten */

  mine[minenzahl].rohstoff=get_untergrund(sx[0],sy[0]);
  mine[minenzahl].datum=heute;
  mine[minenzahl].ges_menge=0L;
  mine[minenzahl].turm_zahl=0;
  mine[minenzahl].filter_zahl=0;
	mine[minenzahl].silo_zahl=0;
  mine[minenzahl].im_lager=0L;
  mine[minenzahl].zuletzt=heute;
  mine[minenzahl].land=map;                 /* Dieses Land */
  mine[minenzahl].x=(sx[0]/4)*4+2;          /* In die Mitte dieses Rasters */
  mine[minenzahl].y=((sy[0]-25)/4)*4+27;
  mine[minenzahl].vorkommen=zufall(1500)+1500L;

  minenzahl++;
  besuch_mine(minenzahl-1);          /* Jetzt Besuch in der Mine */

  such_platz();                 /* Sucht einen neuen Platz für den Spieler */
}

void besuch_mine(nr)
int nr;
{
  /* Spieler besucht Mine nr */
  long old_buttons;
  FLAG newmoney,newmine;
  int i;
  long verschieb,platz;
	int oel_pos;
	int zeile;
	long laenge;
	
	peilgeraet_aus();

	load_digisound(KLONG_SEQ,pack_buf);			/* Digisound laden */
	
  oel_pos=zeichne_mine(nr);                 /* Mine anzeigen */
  old_buttons=bleiste;

  redraw_buttons(EXIT_BTN|PAUSE);

  newmine=FALSE;
  newmoney=TRUE;

  do {
		animier_mine(oel_pos);

    if (newmoney) {
      newmoney=FALSE;
      Hm();
      writexy_money(168,126);
      balken(168,144,frei_waggon(),sum_waren()+sum_rohstoffe());
      Sm();
      }
    if (newmine) {
      newmine=FALSE;
      oel_pos=draw_mine(scr1,nr);
      }

    wait_sync_klick(15);
		
		if (zufall(10)==1 && effects) {
			zeile=zufall(3);
			play_digi(pack_buf,FALSE,zeile,zeile);			/* Maschinenklongen */
			}
			
    if (mk==1)
      if (mx>30 && mx<150) {
        if (my<88) {                    /* Beladen */
          Krec(29,71,156,87);
          platz=frei_waggon()-sum_waren()-sum_rohstoffe();
          if (platz>mine[nr].im_lager) verschieb=mine[nr].im_lager;
          else verschieb=platz;

          if (verschieb!=0L) {
            if (mine[nr].rohstoff>=GOLD) {              /* MUD'N Dirt */
              alert(build(romstr260,str(0,verschieb/KAP_KARREN+1)));
              ystable_menge[0]-=verschieb/KAP_KARREN+1;
              ystable_menge[1]-=verschieb/KAP_KARREN+1;
              }
            else {
              ymetal[mine[nr].rohstoff]+=verschieb;
              }
            mine[nr].im_lager-=verschieb;

            writexy(0,220,84,str(4,mine[nr].im_lager));
            if (mine[nr].rohstoff>=GOLD)              /* Sorry, hier hat's nichts */
              writexy(1,220+4*5,84,romstr261);
            else
              writexy(0,220+4*5,84,metal_name[mine[nr].rohstoff]);

            newmoney=TRUE;                    /* Balken neu zeichnen */
            }
          }
        if (my>88 && my<105) {          /* Neuer Förderturm */
          Krec(29,89,156,105);
          if (mine[nr].turm_zahl<3)
            if (money>5000L)
              if (ycargo_menge[13]!=0L) {   /* Maschine dabei? */
                ycargo_menge[13]--;         /* Jetzt nicht mehr */
                mine[nr].turm_zahl++;
                money-=5000L;
                newmoney=TRUE;
                newmine=TRUE;
                }
              else alert(romstr262);
            else money_alert();
          else alert(romstr263);
          }
        if (my>105 && my<123) {         		/* Neue Filteranlage */
          Krec(29,107,156,123);
          if (mine[nr].filter_zahl<3)
            if (money>1000L) 
  						if (ycargo_menge[14]>0) {
		            mine[nr].filter_zahl++;
  	            money-=1000L;
    	          newmoney=TRUE;
      	        newmine=TRUE;
								ycargo_menge[14]--;
								}
							else alert(romstr264);
            else money_alert();
          else alert(romstr265);
          }
        if (my>123 && my<142) {         		/* Neuer Öltank */
          Krec(29,125,156,141);
          if (mine[nr].silo_zahl<3)
            if (money>4000L) 
							if (ycargo_menge[15]>0) {
								ycargo_menge[15]--;
	              mine[nr].silo_zahl++;
  	            money-=4000L;
    	          newmoney=TRUE;
      	        newmine=TRUE;
								}
							else alert(romstr266);
            else money_alert();
          else alert(romstr267);
          }
        if (my>142) {                     /* Mine schließen */
          Krec(29,143,156,159);
          if (money>5000L) {
            if (yes_no(romstr268)) {
              money-=5000L;
              if (mine[nr].rohstoff<=GOLD) {          				/* Hier war Rohstoff */
                ground_change(EBENE,map,mine[nr].x,mine[nr].y);     	/* Rohstoff weg */
	              put_untergrund(EBENE,mine[nr].x,mine[nr].y);    		/* Mine löschen */
								}
							else {
                ground_change(mine[nr].rohstoff,map,mine[nr].x,mine[nr].y);     	/* Rohstoff weg */
	              put_untergrund(mine[nr].rohstoff,mine[nr].x,mine[nr].y);    		/* Mine löschen */
                }
							minenzahl--;
              for(i=nr;i<minenzahl;i++) {
                mine[i]=mine[i+1];           /* Anderen Minen aufrücken */
                }
              button=EXIT_BTN;            /* gleichzeitig raus */
              }
            }
          else money_alert();           /* Zu wenig Geld */
          }
        }
    } while(button!=EXIT_BTN);

	while (digi_works) ; 					/* Warten bis Digisound fertig ist */
	
	wait_once(1);									/* Störungen abfangen */
	digi_aus();										/* Digisound abschalten */
	
  redraw_buttons(old_buttons);          /* Alte Leiste wiederherstellen */
  land_an();
}

int zeichne_mine(nr)
int nr;                 /* Nummer der Mine */
{
  /* Zeichnet eine Mine auf den Bildschim und initialisiert alles */
  long tage,prod;                    /* Wieviel Tage nicht mehr abgeholt */
  char datum_text[30];
  char *menge,*menge2;                  /* Pointer auf string */
	int oel_pos;									/* Position des Ölstrahls */

  tage=heute-mine[nr].zuletzt;                          /* Anzahl an Tagen */
  prod=mine[nr].turm_zahl*tage*(zufall(4)+1);

  if (mine[nr].im_lager+prod>mine[nr].silo_zahl*150L) {   	/* Paßt das ins Lager? */
    prod=mine[nr].silo_zahl*150L-mine[nr].im_lager;        	/* Soviel kam dazu */
    }
  /* in Prod steht das, was maximal gelagert werden konnte, jetzt bestimmen,
     ob die Mine soviel noch hergab */

  if (prod>mine[nr].vorkommen) prod=mine[nr].vorkommen;

  mine[nr].vorkommen-=prod;
  mine[nr].im_lager+=prod;
  mine[nr].ges_menge+=prod;
  mine[nr].zuletzt=heute;               /* Heute war Spieler zuletzt da */

  loc=CITY;                     /* Wegen Alertbox */

	clear_raster();
  clear_screen(hlpbuf);
  formular(hlpbuf,63);
	oel_pos=draw_mine(hlpbuf,nr);					/* Zeichnet die Mine in den hlpbuf */

  leiste_y=msminy=64;

  init_oben();

  Hm();
  city_screen();
  leiste_oben(romstr269);

  writexy(0,32,74,romstr270
									romstr271
									romstr272
									romstr273);

  writexy(1,32,146,romstr274);

  writexy(0,172,84,	romstr275
										romstr276
										romstr277
										romstr278);

  dlstr(mine[nr].datum,datum_text);
  writexy(0,168,102,datum_text);

  menge=str(4,mine[nr].ges_menge);
  writexy(0,168,114,menge);
  menge2=str(4,mine[nr].im_lager);
  writexy(0,220,84,menge2);

  if (mine[nr].rohstoff>=GOLD) {             /* Sorry, hier hat's nichts */
    writexy(1,220+4*5,84,romstr279);
    writexy(0,168+4*5,114,romstr280);
    }
  else {
    writexy(0,220+4*5,84,metal_name[mine[nr].rohstoff]);
    writexy(0,168+4*5,114,metal_name[mine[nr].rohstoff]);
    }

  Sm();

	return(oel_pos);									/* Hier kommt's Öl raus */
}

int draw_mine(screen,nr)
void *screen;
int nr;														/* Nummer der Mine */
{
  /* Zeichnet die Objekte auf hlpbuf */
  int i,x;
	int xmax;

	Hm();
  load_objekte(EBENE_OBJ, scr2); 		     	/* Minenbilder auf Screen2 laden */
	draw_obj(0,scr2,0,screen,0,0);				/* Hintergrund zeichnen */

	if (mine[nr].vorkommen>0) load_objekte(MINE_OK_OBJ,scr2);
	else load_objekte(MINE_PUT_OBJ,scr2);

	xmax=160;													/* In der Mitte des Screens */
	x=24;
	for(i=0;i<mine[nr].turm_zahl;i++) {
		draw_obj(0,scr2,ODER,screen,x,10);
		xmax=x+31;
		x+=27;
		}
	for(i=0;i<mine[nr].filter_zahl;i++) {
		if (i==0) x+=2;
		draw_obj(1,scr2,ODER,screen,x,10);
		xmax=x+38;
		x+=36;
		}
	for(i=0;i<mine[nr].silo_zahl;i++) {
		if (i==0) x+=7-22+mine[nr].silo_zahl*22;
		draw_obj(2,scr2,ODER,screen,x,11);
		xmax=-1;
		x-=22;
		}
	cpy_raster(screen,scr2,0,0,319,44,0,100);		/* Auf den  hlpbuf retten */

	Sm();

	return(xmax);												/* Hier kommt's Öl raus */
}

void animier_mine(oel_pos)
int oel_pos;
{
	/* Animiert den Ölstrahl der Mine */
	static int oel_nr=3;

	if (oel_pos>=0) {
		cpy_raster(scr2,scr1,oel_pos,100,oel_pos+30,144,oel_pos,0);		/* Hintergrund hin */
		if (oel_pos==160) draw_obj(oel_nr,scr2,ODER,scr1,oel_pos,3);
		else draw_obj_part(oel_nr,scr2,0,5,objekt_breite(oel_nr,scr2)-1,20,ODER,scr1,oel_pos,0);
		oel_nr++;
		if (oel_nr>5) oel_nr=3;
		}
}

void check_cheat()
{
  /* Zeigt die Helpseite an und gibt dann TRUE zurück */
	long taste;
	
  if (cheat_on)                /* Nur wenn Cheat-Check an ist */
    if ((taste=get_key())!=-1L)              /* Taste da? */
      if (taste==0x00620000L) {              /* Help */
				cheat_auswert(FALSE);
				land_an();
				}
}

void cheat_auswert(bunker)
FLAG bunker;												/* Wurde im Bunker HELP gedrückt? */
{
  /* Zeigt die Helpseite an und wertet Tastendrücke aus */
  long oldleiste;
	long taste;
	void *oldlogbase;
	int x,y;
	long dif;
	char bun_nr[10];
	int nr;
	int i;
	register ITEMS *itemp;
	
  Hm();

	loc=CITY;
  oldleiste=bleiste;                /* Alte Buttons sichern */
  redraw_buttons(EXIT_BTN|PAUSE);

	load_objekte(TILL_OBJ,window_back);
	formular(scr1,63);
	draw_obj(0,window_back,MOVE,scr1,270,80);
	show_raster();
	
  center(0,70,romstr281);
	line(0,105,76,210,76);
	Sm();
	
	do {	
		Hm();
		cheat(20,80, romstr282,rohstoff_cheat);
		cheat(20,86, romstr283,unlim_money_cheat);
		cheat(20,92, romstr284,produce_cheat);
		cheat(20,98, romstr285,strength_cheat);
		cheat(20,104,romstr286,pio_cheat);
		cheat(20,110,romstr287,treasure_cheat);
		cheat(20,116,romstr288,kaserne_cheat);
		cheat(20,122,romstr289,(speed!=2));
		cheat(20,128,romstr290,(money!=0));
		cheat(20,134,romstr291,(bunker) ? FALSE : TRUE);
		cheat(20,140,romstr292,(bunker) ? FALSE : TRUE);
		cheat(20,146,romstr293,(bunker) ? FALSE : TRUE);
		cheat(20,152,romstr294,door_cheat);
		
		cheat(150,80,romstr295,zeit_cheat);
		cheat(150,86,romstr296,TRUE);
		cheat(150,92,romstr297,(bunker) ? FALSE : TRUE);
		cheat(150,98,romstr298,automapping);
		cheat(150,104,romstr299,(bunker) ? TRUE : FALSE);
		cheat(150,110,romstr300,treffer_cheat);
		
		Sm();

    do {
			button=NOTHING;
			hol_maus();
			if (my>167) button_leiste();
			} while(button==NOTHING && !is_key());
		
		if (is_key()) {
			taste=wait_key();
		
      switch (toupper((int)taste)) {
				case 'E':
					for(itemp=item,i=0;i<ITEMMAX;i++,itemp++)
						if (itemp->typ==MIRROR) party[0].tasche[0]=i;
					break;
				case 'L': 
					if (bunker) {
						ycargo_menge[10]=100;
						helligkeit=6;
						set_bunker_pal();
						}
					break;
				case 'U': treffer_cheat=!treffer_cheat; break;
				case 'A': automapping=!automapping; break;
				case 'R': rohstoff_cheat=!rohstoff_cheat; break;
        case 'C':
          if (!bunker) {
						citycheat=TRUE;
						button=EXIT_BTN;
						}
          break;
        case '+':
          speed=0;                /* volle Geschwindigkeit */
          break;
        case '*':
          unlim_money_cheat=!unlim_money_cheat;
          break;
				case 'X':
					door_cheat=!door_cheat;
					break;
        case '0':
          money=0;
          break;
        case 'S':
          strength_cheat=!strength_cheat;
          break;
        case 'B':
          if (!bunker) {
						if (schiffbesitz==0) {
							schiffbesitz=1;
							ship_own[3]=1;
							flotte[0].typ=3;
							flotte[0].aboard=ship_crew[3];
							flotte[0].salary=999;
							flotte[0].ort=-1;
							}
						Seeschlacht(1);
						button=EXIT_BTN;
						}
          break;
        case 'F':
          if (!bunker) {
						fight(1);
						button=EXIT_BTN;
						}
          break;
        case 'O':
          pio_cheat=!pio_cheat;
          break;
        case 'K':
          kaserne_cheat=!kaserne_cheat;
          break;
        case 'T':
          treasure_cheat=!treasure_cheat;
          break;
        case 'W':
          produce_cheat=!produce_cheat;
          break;
        case 'Z':
          zeit_cheat=!zeit_cheat;
          break;
        case 'M':
          dif=30-day;
          heute+=dif;
          day=30;
					check_time();
          break;
        case 'D':
					if (!bunker) {
	          Hm();
						do {
							show_window(ns);
							center(0,97,romstr301);
							center(0,105,romstr302);
							center(0,115,romstr303);
					    input(136,123,3,bun_nr);
							nr=atoi(bun_nr);
							for(i=0;i<CITIES;i++) if (personen_bunker[i]==nr) break;
							if (i==CITIES)
								for(i=0;i<LAENDER;i++) 
									if (bunker_nr[i][0]==nr || bunker_nr[i][1]==nr) break; 
							} while(i==LAENDER);
							Sm();
							dungeon(nr);
						button=EXIT_BTN;
						}
				case 13:										/* Return */
          break;
        default:
          autsch();
          break;
        }
      }
    } while((int)taste!=13 && button!=EXIT_BTN);

  redraw_buttons(oldleiste);
	set_raster(0,63,NULL);							/* Formularraster löschen */
	set_raster(0,80,NULL);							/* T.Bubeck Raster löschen */
	show_raster();
	button=NOTHING;
}

void cheat(x,y,text,cheat_var)
int x,y;
char text[];
FLAG cheat_var;
{
	writexy((cheat_var) ? 2 : 1,x,y,text);
}

void simulation()
{
  /* Es werden hier die Bewegungen der gegnerischen Armeen erzeugt */

  vulkane();                    /* Läßt Vulkane rauchen */
  move_armies();                /* Bewegt die gegnerischen Armeensprites */
  switch_sprites();             /* Sprites an- oder ausschalten */
}

void vulkane()
{
  /* Sorgt dafür, daß ab und zu Vulkane rauchen */
  if (vulkan_aktiv) {           /* Ist ein Vulkan schon aktiv? */
    ani_vulkan();               /* Ja, Dann animieren */
    }
  else
    if (map==9) {     				/* Nur hier hat's Vulkane */
      if (zufall(20)==2) {     	/* Nur manchmal */
        such_vulkan();          	/* Vulkan starten */
        }
      }
}

void ani_vulkan()
{
  /* Animiert den Vulkan */

  if (--vulkan_counter!=0) return;
  vulkan_counter=5;

  sn[vulkan_n]++;          /* Nächstes Bild */
  if (sn[vulkan_n]==63) {          /* Ende der Animation? */
    sn[vulkan_n]=59;
    vulkan_stufe++;                     /* Nächste Stufe */
    switch (vulkan_stufe) {
      case 1:
        sx[vulkan_n]=vulkanx-2;
        sy[vulkan_n]=vulkany-1;
        break;
      case 2:
        sx[vulkan_n]=vulkanx+1;
        sy[vulkan_n]=vulkany-2;
        break;
      case 3:
        sx[vulkan_n]=vulkanx;
        sy[vulkan_n]=vulkany;
        break;
      case 4:                           /* Vulkan fertig */
        vulkan_aktiv=FALSE;               /* Kein Vulkan aktiv */
        sx[vulkan_n]=-1;                  /* Sprite anireigeben */
        break;
      }
    }

  if (vulkan_aktiv)
    if (!auf_schiff)                   /* Landarmee */
      if (sx[0]>sx[vulkan_n]-8 && sx[0]<sx[vulkan_n]+8 &&
           sy[0]<=sy[vulkan_n] && sy[0]>=sy[vulkan_n]-7) {
        armeegesamt[0]-=armeeteil[0][4];
        armeeteil[0][4]=0;                      /* DRR tot */
        gehalt[4]=0;                            /* Kein Geld mehr */
        calc_gehalt();                            /* Gesamtgehalt ermitteln */
        writexy_anzeige();                        /* ggf. Anzeigen */
        }
}

void such_vulkan()
{
  /* Es beginnt ein Vulkan zu rauchen */
  register int y,x;
  int anzahl;
  register int startx,starty,endx,endy;

  switch (map) {
    case 9: anzahl=4;
            startx=24;
            starty=12;
            endx=29;
            endy=26;
            break;
    }

  anzahl=zufall(anzahl);                    /* Einen Vulkan auswählen */

  for(x=startx;x<endx;x++)
    for(y=starty;y<endy;y++)                       /* enspr. Vulkan suchen */
      if (get_raster(x,y)==VULKANE) {
        if (anzahl==0) {
          vulkanx=x;
          vulkany=y;                        /* Koordinaten merken */
          goto raus;                        /* und raus aus der Schleife */
          }
        else anzahl--;
        }

  internal(romstr304);

raus:

  vulkanx=vulkanx*4+7;
  vulkany=vulkany*4+27;
  vulkan_n=such_sprite();                   /* Nr des Vulkansprites */
  sx[vulkan_n]=vulkanx;
  sy[vulkan_n]=vulkany;
  sn[vulkan_n]=59;                     /* Anfang des Vulkans */
  vulkan_aktiv=TRUE;
  vulkan_stufe=0;                           /* 0. Animationstufe */
  vulkan_counter=5;                         /* Counter setzen */
}

void cdecl int_err(error)
char *error;
{
  /* ErrorRoutine, selbe wie internal, wird aus mcode.o heraus aufgerufen */

  internal(error);
}

void internal(error)
char *error;
{
  /* Interner Fehler anzeigen */

  alert(build(romstr305,error));
	longjmp(restart,1);										/* Medusa neu starten */
}

void daily_events()
{
  schiff_test();                    /* Ist Irgendwo ein Schiff fertig? */
}

void monthly_events()
{
  register int k,j,i;
  int zuwachs[EINHEITEN];
  FLAG aus_und_schluss;

  leiste_oben(romstr306);

#ifdef DEMO
  if (month & 1) alert(romstr307,romstr308,ns,ns,ns);
#endif

  money-=gesamt_gehalt;
  if (money<0) aus_und_schluss=TRUE;
  else aus_und_schluss=FALSE;

  writexy_anzeige();                          /* ggf. anzeigen */

  if (aus_und_schluss) game_over(romstr309,romstr310);

  /* Gegner werden mehr: */
  for(i=0;i<GEGNER;i++) {
    zuwachs[0]=zufall(3);               /* Inf */
    zuwachs[1]=zufall(2);                /* Cav */
    zuwachs[2]=zufall(3);                /* art */
    zuwachs[3]=zufall(3);                /* Scout */
    zuwachs[4]=zufall(2);                 /* Drr */
    zuwachs[5]=zufall(2);                 /* wiz */
    zuwachs[6]=zufall(3);                /* Arscher */
    for(j=0;j<EINHEITEN;j++) {
      for(k=0;k<EIGENSCHAFTEN;k++) {
        training[i+1][j][k]+=0.3333333333;              /* Armeetraing ++ */
        if (training[i+1][j][k]>100.0) training[i+1][j][k]=100.0;
        }
      armeeteil[i+1][j]+=zuwachs[j];
      armeegesamt[i+1]+=zuwachs[j];
      }
    }

  show_ground();               /* Untergrund in Leiste */

  if (money<gesamt_gehalt) alert(romstr311);
}

void editor()
{
  int i;
  long taste;
  int farbe=0;
  char string[2];
  char cursor[2];
	int tast_repeat;
	
	peilgeraet_aus();
	
  leiste_oben(romstr312);
  loc = CITY;
  leiste_y=64;
  msminy=63;                     /* höher darf Maus nicht */
  c_pic(STORE_OBJ);
  leiste_oben(romstr313);

  cursor[0]='@';
  cursor[1]=0;

  Hm();
  redraw_buttons(EXIT_BTN|PAUSE);      /* nur Exit */

  for(i=0;i<15;i++) {               /* ALten Editormemo ausgeben */
    writexy(0,12,72+i*6,memo[i]);
    }

  home();

  string[1]=0;                 /* EOS */

	tast_repeat=4;
	
  do {
    writexy(0,spalte*4,zeile*6,cursor);

    Sm();

    do {
			taste=get_key();
			if (taste!=-1) {
				tast_repeat=15;											/* Startverzögerung */
				}
			else 
				if (keypress!=0) 							/* Taste noch gedrückt? */
					if (--tast_repeat<=0) {
						tast_repeat=3;
						taste=keypress;
						}
					else wait_once(1);
				else {												/* Taste nicht gedrückt */
					button=NOTHING;
  		    hol_maus();
	    	  if (my>167 && mk!=0) button_leiste();
      		if (button==EXIT_BTN) return;
					}
			} while(taste==-1);
				
    Hm();

    string[0]=memo[zeile-12][spalte-3];
    writexy(0,spalte*4,zeile*6,string);

    if (taste==0x00470037L) {
      cls();
      taste=0;
      }
    if (isprint((int)taste)) {
      if (islower((int)taste)) toupper(taste);
      string[0]=taste;
      string[1]=0;
      writexy(farbe,spalte*4,zeile*6,string);
      memo[zeile-12][spalte-3]=taste;           /* abspeichern */
      taste=0x004d0000L;                         /* Cursor right */
      }
    if (taste==0x000e0008L) {                    /* Backspace */
      if (spalte>3) {
        for(i=spalte-4;i<73;i++) memo[zeile-12][i]=memo[zeile-12][i+1];
        memo[zeile-12][73]=' ';           /* von hinten Spaces rein */
        writexy(farbe,12,zeile*6,memo[zeile-12]);
        taste=0x004b0000L;
        }
      }
    if (taste==0x00520000L) {
      for(i=73;i>(spalte-3);i--) memo[zeile-12][i]=memo[zeile-12][i-1];
      memo[zeile-12][spalte-3]=' ';
      writexy(0,12,zeile*6,memo[zeile-12]);
      }

    if (taste==0x00470000L) {
      home();
      }
    if (taste==0x004d0000L) {
      spalte++;
      if (spalte>76) crlf();
      }
    if (taste==0x004b0000L) {
      spalte--;
      if (spalte<=2) {
        spalte=76;
        }
      }
    if (taste==0x00480000L) {
      zeile--;
      if (zeile<12) zeile=26;
      }
    if (taste==0x00500000L) {
      zeile++;
      if (zeile==27) zeile=12;
      }

    if (taste==0x001e0000L) farbe=0;
    if (taste==0x00300000L) farbe=1;
    if (taste==0x002e0000L) farbe=2;
    if (taste==0x00200000L) farbe=3;
    if (taste==0x00120000L) farbe=4;
    if (taste==0x00210000L) farbe=5;
    if (taste==0x00220000L) farbe=6;
    if (taste==0x00230000L) farbe=7;
    if (taste==0x00170000L) farbe=8;
    if (taste==0x00240000L) farbe=9;
    if (taste==0x00250000L) farbe=10;
    if (taste==0x00260000L) farbe=11;
    if (taste==0x00320000L) farbe=12;
    if (taste==0x00310000L) farbe=13;
    if (taste==0x00180000L) farbe=14;
    if (taste==0x00190000L) farbe=15;

    if (taste==0x001c000dL) crlf();
    } while(TRUE);
}

long minimum(w1,w2)
long w1,w2;
{
  /* gibt den kleineren Wert zurück */
  if (w1<w2) return(w1);
  else return(w2);
}

void balken(x,y,max,real)
int x,y;
long max,real;
{
  /* Zeigt den Beladungszustand als Balkendiagramm an */
  int laenge;
  FLAG ueberladen,nicht_da;

  Hm();
  x=x-x%4;                      /* Auf 4er Position bringen */

  writexy(0,x-12,y,romstr314);
  writexy(0,x+48,y,romstr315);

  if (max==0) nicht_da=TRUE;
  else nicht_da=FALSE;

  laenge=(max==0) ? 44 : real*44/max;            /* Division/0 vermeiden */
  if (real>max) {
    laenge=44;
    ueberladen=TRUE;
    }
  else ueberladen=FALSE;

  fill(1,x,y,x+laenge,y+4);
  if (laenge<44) fill(2,x+laenge+1,y,x+44,y+4);

  if (ueberladen) writexy(0,x+4,y,romstr316);
  if (nicht_da) writexy(0,x+4,y,  romstr317);
  Sm();
}

void game_over(txt1,txt2)
char txt1[],txt2[];                 /* 2 Zeilen Begründung für Game over */
{
  status(txt1,txt2);                /* Status anzeigen */

  longjmp(restart,1);               /* Und Spiel neustarten */
}

void make_adresses()
{
  /* Dies ist wohl die verrückteste Funktion von Medusa:
     Sie schreibt die Adresse und Größe der Variablen ab poi, die gesavt oder
     geladen werden beim Spielstand */

  register long *poi;

  poi=(long *)hlpbuf;               /* Nach hlpbuf schreiben */

#include romstr318

  *poi=-1L;                         /* Ende marker */
}

void blitz(color)
int color;
{
	int old_color;

	old_color=hbl_system[0][1];						/* Alte Farbe merken */
	hbl_system[0][1]=color;							/* color als neue VBL-Hintergrundfarbe */
	show_raster();											/* und anzeigen */
	hbl_system[0][1]=old_color;
	show_raster();											/* Alte Farbe wieder rein */
}

void autsch()
{
  show_window(romstr319);
  wait_once(20);
  hide_window();
}


