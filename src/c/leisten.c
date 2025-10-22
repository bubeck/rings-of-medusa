  
/* Dieser Part regelt alles, was mit den Button-Leisten zu tun hat */

#include "includes.c"           /* Definiert alle Variablen als Extern,... */

int aufklapp[6][15]= {
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 3,  /* Parameter, zum Drehen */
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 3, 7,  /* der Optionsleiste */
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 1, 3, 7,10,
  -1,-1,-1,-1,-1,-1,-1,-1,-1, 1, 3, 6, 7,10,12,
  -1,-1,-1,-1,-1, 0, 1, 3, 4, 6, 7, 9,10,12,13,
   0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14
   };

FLAG option_changed;

void do_board()
{
  /* Abfrageroutine für das Board */

  switch (option_nr) {                  /* je nach Board verzweigen */
    case 1:                             /* Anzeige */
			check_scroller();
			break;
    case 0:                             /* Signs of Medusa */
    case 3:                             /* Autoren */
      wait_close();                     /* Nur auf Schliessfeld warten */
      break;
    case 2:                             /* Optionboard */
      options();
      break;
		case 4:
			bunker_steuerung();
			break;
    }
}

void bunker_steuerung()
{
	/* Bedient das Board, mit welchem im Bunker gesteuert werden kann. */

  do {
  	hol_maus();
  	if (maus_in(100,171,110,179)) {               /* Close */
      new_option();                               /* Nächstes Optionsboard */
      break;
      }
    if (is_key()) break;
 		} while(mk!=0 && mx>96 && mx<223 && my>169);					/* Solange noch im Optionboard */
	button=RAUS;																	/* Buttonabfrage beenden */
}

void check_scroller()
{
	/* Überprüft, ob das Schliessfeld geklickt oder in den Scrollbutton */
	int scrollpos,scrolldelta;
	void *oldlogbase;
	char zeichen[2];
	long laenge;
	
	do {
		hol_maus();
    if (maus_in(100,171,110,179)) {               /* Close */
      new_option();                               /* Nächstes Optionsboard */
      break;
      }
    if (maus_in(128,194,140,199)) {         		/* Scrollbutton */
			memset(window_back,0,sizeof(window_back));
#if defined(DEUTSCH)
			laenge=load_bibliothek(GAME_TXT,&window_back[5*160]);
#elif defined(ENGLISCH)
			laenge=load_bibliothek(GAME_GB_TXT,&window_back[5*160]);
#elif defined(FRANZ)
			laenge=load_bibliothek(GAME_TXT,&window_back[5*160]);
#endif
			decrypt(&window_back[5*160],laenge);
			wait_mouse(0);
			Hm();
			scrollpos=0;
			scrolldelta=6;
			oldlogbase=logbase;
			logbase=window_back;
			zeichen[1]=0;
			do {
				hol_maus();
				if (++scrolldelta>3) {
					scrolldelta=0;
					do {
						zeichen[0]=window_back[5*160+scrollpos++];
						if (zeichen[0]==0 || zeichen[0]==13 || zeichen[0]==10) scrollpos=0;
						else break;
						} while(TRUE);
					writexy_fast(0,0,zeichen);						/* Zeichen ausgeben */
					}
				cpy_raster(scr1,window_back,152,194,203,198,4,0);		/* Zeile in window_back */
				cpy_raster(window_back,scr1,5,0,56,4,152,194);			/* und scrollen */
				cpy_raster(window_back,scr1,scrolldelta,0,scrolldelta,4,203,194);		/* Zeichen rein */
				wait_once(2);
				} while(mk==0 && !is_key());
			
			wait_mouse(0);
			logbase=oldlogbase;
			fill(0,152,194,203,198);
    	Sm();
		  }
    } while(mk!=0 && mx>=96 && mx<=223 && my>=169);

}		

void wait_close()
{
  /* Optionboardroutine, die auf Klicken in Schliessfeld lauert */
  int mx2,my2;
  long counter;

  mx2=my2=0;

  counter=0;

  do {
    hol_maus();

    if (mx==mx2 && my==my2) {
      counter++;
      if (counter>=180000L*3) {
        pause_game();
        counter=0;
        }
      }
    else {
      mx2=mx;
      my2=my;
      counter=0;
      }

    if (maus_in(100,171,110,179)) {               /* Close */
      new_option();                               /* Nächstes Optionsboard */
      break;
      }
    if (mk==3) {													/* Beide Knöpfe gedrückt? */
			mk=1;																/* Wegen maus_in() */
			if (maus_in(149,173,149,176) && option_nr==3) {         /* Cheat an */
				cheat_on=!cheat_on;
				blitz(0x700);
				wait_mouse(0);										/* Maustaste wieder loslassen */
				}
      }
    } while(mk!=0 && mx>=96 && mx<=223 && my>=169);
}

void option_city()
{
  /* Spieler ist in Stadt oder Burg, und darf deshalb die Moneyanzeige
     nicht eingeschaltet haben */

  if (option_nr==1) {                   /* Er hat sie an */
    option_nr--;                        /* Eins zurück */
    option_changed=TRUE;
    }
  else option_changed=FALSE;
}

void option_city_ende()
{
  /* Spieler ist in Stadt oder Burg, und darf deshalb die Moneyanzeige
     nicht eingeschaltet haben */

  if (option_changed) {                   /* Er hatte sie an */
    option_nr=1;
		draw_options();												/* Zeichnet die aktuelle Options-Board neu */
		copy_buttons();
    }
}

void new_option()
{
  /* Dreht das nächste Optionsboard rein, kopiert es in die Leiste */

  option_nr++;
  if (option_nr==5) option_nr=0;

  if (loc!=LAND)
    if (option_nr==1) option_nr=2;      /* In der Stadt keine Moneyanzeige */

  flip(option_nr);                          /* Dreht aktuelles Board rein */
	print_richtung();								/* ggf. Richtung eintragen */
	
	copy_buttons();
}

void draw_options()
{
	/* Zeichnet das momentan aktive Board auf den Screen und trägt ggf. Werte ein: */
	
	draw_obj(4+option_nr,leisten,0,scr1,96,169);
	writexy_anzeige();
}

void writexy_anzeige()
{
  /* In der Anzeigetafel stehen die Werte für Money und Soldiers, hat sich
     irgendeiner dieser Werte geändert wird er mit dieser Funktion ausgegeben
     */
  void *oldlogbase;

	switch (option_nr) {
		case 1:
			oldlogbase=logbase;
			logbase=scr1;																		/* In die Leiste printen */
		  writexy(15,126,175,romstr610);
			logbase=oldlogbase;
		
		  writexy_fast(162,175,str(10,money));
		  writexy_fast(162,181,str(10,armeegesamt[0]));
		  writexy_fast(162,187,str(10,gesamt_gehalt));
			break;
		case 2:
			fill(13,124,172,132,197);					/* Alte Haken wegmachen */
			if (sync==60) draw_obj(3,leisten,0,scr1,127,173);
			if (music==MUSIK_AN) draw_obj(3,leisten,0,scr1,127,179);
			if (effects) draw_obj(3,leisten,0,scr1,127,185);
			if (tv_modus) draw_obj(3,leisten,MOVE,scr1,127,191);
			draw_obj_part(6,leisten,78,9,80,25,MOVE,scr1,174,178);	/* Level weg */
			draw_obj(9,leisten,MOVE,scr1,174,(int)(178+(2.0-level)*15.0));	/* und hin */
			break;
    }

	copy_buttons();
}

void clear_buttons(redraw)
FLAG redraw;                        /* TRUE: nur veränderte zeichnen */
                                    /* FALSE: komplett neu kopieren */
{
  init_buttons(redraw,0L);                 /* Alle aus */
}

void redraw_buttons(butt)
long butt;
{
  /* Zeichnet nur die Buttons neu, die sich geändert haben */

  init_buttons(TRUE,butt);
}

void new_buttons(butt)
long butt;
{
  /* zeichnet alle Buttons neu, und kopiert die Leiste rein */

  init_buttons(FALSE,butt);
}

void init_buttons(redraw,butt)
FLAG redraw;                        /* s.o. */
long butt;                          /* Bitwert der Leiste */
{
  bleiste_old=bleiste;              /* Alte Bleiste retten */
  bleiste=butt;                     /* Neue Bleiste sichern */
  show_buttons(redraw);             /* Neue Leiste anzeigen */
}

void show_buttons(redraw)
FLAG redraw;                        /* s.o. */
{
  /* Zeigt die aktuelle Leiste an, bzw. kopiert sie auf den Bildschirm */
  int i,j;
  int pxy[4];
  long element;
	int ob_nr,ob_x;
	int *old_pattern,old_mode;
	
  Hm();
  if (!redraw) {
  	draw_obj(1,leisten,MOVE,scr1,0,169);					/* Linker Teil des Boards */
  	draw_obj(2,leisten,MOVE,scr1,224,169);					/* rechter Teil */
  	draw_options();												/* und Mittelteil */
		}

	old_mode=line_mode(2);									/* Transparent */
	old_pattern=line_pattern(point_pat); 
			
  for(i=0;i<19;i++) {
    element=menu_nr[i];             /* Wert des Menupunktes */
    if ((bleiste&element)==0 &&
           ((bleiste_old&element)!=0 || !redraw)) {     /* löschen */
      for(j=0;j<4;j++)
        pxy[j]=menu_koord[i*4+j];   /* Koordinaten holen */
  		fill(5,menu_koord[i*4],menu_koord[i*4+1],menu_koord[i*4+2],menu_koord[i*4+3]);
      }
    if ((bleiste&element)!=0 && (bleiste_old&element)==0 && redraw) {   /* Setzen */
      for(j=0;j<4;j++) {
        pxy[j]=menu_koord[i*4+j];              /* lst_buf-Koordinaten holen */
        }
			/* Button zeichnen: */
			if (pxy[0]<160) {									/* In welchem Objekt ist der Button? */
				ob_nr=1;												/* Im Linken */
				ob_x=0;													/* das startet hier */
				}
			else {
				ob_nr=2;
				ob_x=224;
				}
			draw_obj_part(ob_nr,leisten,pxy[0]-ob_x,pxy[1]-169,pxy[2]-ob_x,pxy[3]-169,
											0,scr1,pxy[0],pxy[1]);
      }
    }
  Sm();

	line_mode(old_mode);
	line_pattern(old_pattern); 
}

void copy_buttons()
{
  /* Kopiert die Buttonleiste auf den 2. Bildschirm */
	void *oldlogbase;
	char *pw;

  Hm();
	
	pw=password-881;
	if (*(pw+881)==0) {
		oldlogbase=logbase;
		logbase=scr1;
		line(15,319,170,319,170);
		logbase=oldlogbase;
		}

  cpy_raster(scr1,scr2,0,169,319,199,0,169);
  Sm();
}

void options()
{
  /* Optionboard */
  long old_leiste;
  int i;
  long nr;
  FLAG flash_on;
  int flash_counter;
  int button;
  int x;

#ifdef FRANZ
  x=180;                        /* Koordinate für Sure? */
#else
  x=192;
#endif

  do {
    hol_maus();
    if (mk==1) {
	    if (maus_in(100,171,110,179)) {               /* Close */
        new_option();            	                   /* Nächstes Optionsboard */
        return;
        }
      Hm();
			if (maus_in(174,178,176,194)) {			/* Level */
				draw_obj_part(6,leisten,78,9,80,25,MOVE,scr1,174,178);
				move_objekt(9,leisten,MOVE,178,scr2,
											STOP_O|STOP_U|STOP_R|STOP_L,174,178,176,195,FALSE);		/* keine 2 Screens */
				if (my<178) my=178;
				if (my>193) my=193;
				level=2.0-(my-178)/15.0;
				draw_options();
				}
      if (mx>=118 && mx<=125) {             /* 1. Spalte */
        if (my>=174 && my<=177) {           /* Sync */
          wait_mouse(0);
          if (sync==50) sync=60;
          else sync=50;
          set_sync(sync);
          draw_options();
          }
        if (my>=180 && my<=191) {            /* Music/Effects */
          wait_mouse(0);
          music=music+1;
          if (music>MUSIK_AUS) {
						music=MUSIK_AN;
						effects=FALSE;				/* Effekte aus */
						}
					else {
						effects=TRUE;					/* Effekte an */
						}
					play_music(&game_music,music);
          draw_options();
          }
     		if (my>=192 && my<=195) {			/* TV-Modus */
					wait_mouse(0);
					tv_modus=!tv_modus;
					draw_options();
					if (akt_bunker>=0) set_bunker_pal();		/* gerade im Bunker->Palette neu setzen */
					}
		    }
      if (mx>=209 && mx<=217) {             /* 2. Spalte */
        if (my>=189 && my<=194) {           /* Quit Game */
          old_leiste=bleiste;
          redraw_buttons(YES|NO);
          Sm();
          flash_on=TRUE;
          flash_counter=0;
          button=NOTHING;
          do {
            flash_counter--;
            if (flash_counter<=0) {
              flash_counter=7000;
              Hm();
              if (flash_on) writexy(1,x,175,romstr611);
              else writexy(13,x,175,romstr612);
              Sm();
              flash_on=!flash_on;
              }

            hol_maus();
            if (mk==1) {
              for(i=0;i<68;i+=4) {
                nr=menu_nr[i/4];
                if (mx>=menu_koord[i] && mx<=menu_koord[i+2] &&
                  my>=menu_koord[i+1] && my<=menu_koord[i+3] &&
                  (nr==YES || nr==NO)) button=nr;
                }
              wait_mouse(0);
              }
            } while (button!=YES && button!=NO);
          Hm();
          writexy(13,x,175,romstr613);
          if (button==YES) {
            Sm();
            longjmp(restart,1);                 /* Crown neu starten */
            }
          redraw_buttons(old_leiste);
          }
        }
      Sm();
      }
    } while (mk!=0 && mx>96 && mx<223 && my>168);
}

void flip(option_nr)
int option_nr;
{
  /* dreht die untere Leiste, so daß die Options sichtbar werden */
  /* Kopiert dann die untere Leiste auf den Screen2 */
  int i;

  Hm();

	wait_sync(0);							/* Timer rücksetzen */
  for(i=0;i<6;i++) {
    stauch();               /* 6 mal stauchen->Board dreht weg */
    wait_sync(3);           /* bißchen warten */
    }

  dehn(option_nr);               /* neues Bild reindrehen */

	draw_options();									/* und Werte eintragen */
	
  /* Auf 2. Bildschirm übertragen: */
	copy_buttons();
  Sm();
}

void dehn(option_nr)
int option_nr;
{
  int i,j;

	wait_sync(0);										/* Timer rücksetzen */
	
  zeichne(option_nr,15,15);      /* mittlerste Zeile des neuen Bildes zeichnen */

  for(i=0;i<6;i++) {        /* 6 mal ausdehnen */
    for(j=0;j<15;j++) {     /* 15 Zeilen obere Hälfte und 15 untere */
      zeichne(option_nr,j,aufklapp[i][j]);       /* obere Hälfte zeichnen */
      if (aufklapp[i][14-j]!=-1)
        zeichne(option_nr,16+j,30-aufklapp[i][14-j]);      /* untere Hälfte */
      }
    wait_sync(3);
    }
}

void zeichne(option_nr,bildzeile,orgzeile)
int option_nr;
int bildzeile,orgzeile;
{
  /* Zeichnet auf dem Bildschirm auf Zeile romstr614 die Zeile romstr615
     aus dem Originalobjekt von romstr616+4 */

  if (orgzeile!=-1)                    /* Bei -1 garnicht zeichnen */
    draw_obj_part(4+option_nr,leisten,0,orgzeile,objekt_breite(4+option_nr,leisten)-1,orgzeile,
											0,scr1,96,169+bildzeile);
}	

void stauch()
{
  /* staucht einmal */
  int i;
	void *oldscreen;
	
	/* Die Zeilen kopieren -> stauchen: */
  for(i=0;i<40;i+=2) 
  	cpy_raster(scr1,scr1,96,169+copylist[i],223,169+copylist[i],96,169+copylist[i+1]);

	/* und oberen und unteren Rand weglöschen: */
	oldscreen=logbase;
	logbase=scr1;
	fill(0,96,169,223,173);
	fill(0,96,195,223,199);
	logbase=oldscreen;
}

void load_leisten()
{
  /* Lädt die Leisten und den Kopierschutz und startet ihn. */
	long laenge;
		
	option_nr=0;									/* 1. Board ist aktiv */
  laenge=load_objekte(LEISTEN_OBJ,leisten);        
#ifndef COPYPROT
	if (laenge<sizeof(leisten)) writexy(15,0,0,romstr617);
	if (laenge>sizeof(leisten)) {
		char fehler[100];
		strcpy(fehler,build(romstr618,str(0,(long)sizeof(leisten))));
		internal(fehler);
		}
#endif
}

