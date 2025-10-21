 
int armee_aktiv;

/* Dieser Part beinhaltet alle Funktionen, die benîtigt werden, damit der
   Spieler sein Sprite auf der Landkarte bewegen kann... */
/* Achtung, alle Stellen, die mit !!! markiert sind, sind zu Debug-zwecken geÑndert worden! */

#include "includes.c"           /* Definiert alle Variablen als Extern,... */

long timecounter;								/* Counter zum ZÑhlen der Tage */

/* Start: */

FLAG land_map(nr,scrollen,rich,x,y)
short nr;             /* Zeigt Land mit nr an */
FLAG scrollen;
int rich;           /* ScrollRichtung  0=hoch, 1=rechts, 2=runter, 3=links */
int x,y;            /* X/Y des Spielers, damit nicht ins Wasser gezogen wird */
{
  int grund;
  FLAG gesperrt;
	void *oldlogbase;
	
  if (vulkan_aktiv) {               /* Vulkan aktiv? */
    vulkan_aktiv=FALSE;             /* abschalten */
    sx[vulkan_n]=-1;                /* Sprite freigeben */
    }

  load_map(nr);     /* Landschaft laden */
  Hm();

  gesperrt=FALSE;

  grund=get_untergrund(x,y);        /* Untergrund holen */
  if (auf_schiff) {
    if (grund!=WASSER) {
      Sm();
      alert(romstr585);
      gesperrt=TRUE;
      }
    }
  else {
    if (grund>=STADT_1_EIN && grund!=BUNKER_1 && grund!=BUNKER_2 && grund!=SCHILD) {
      Sm();
      alert(romstr586);
      gesperrt=TRUE;
      }
    }

  if (gesperrt) {                   /* Ist Grund begehbar? */
    Hm();
    load_map(welt[pos_welt]);       /* Altes Land laden */
		sx[0]=sxalt[0];
		sy[0]=syalt[0];							/* Spieler zurÅcksetzen */ 
    get_ground();
    Sm();
    return(FALSE);
    }

	peilgeraet_aus();

  init_oben();
  loc = LAND;

  if (!scrollen) {
    fade_out();
    clear_raster();
    vsync();
    clear_screen(scr1);
		set_raster(0,17,kartep);
		draw_obj(0,leisten,0,scr1,0,0);		/* Obere Leiste reinkopieren */
		pic_move(hlpbuf,17,167,17);			/* Landkarte anzeigen */
    show_buttons(FALSE);                     /* Leiste reinkopieren */
    show_ground();

    fade_in();
    }
  else {
    mouse_off();            /* IKBD-Mouse off */
		draw_obj(0,leisten,0,scr1,0,0);							/* Obere Leiste anzeigen */
    show_ground();

    copy_screen(scr1,scr2);

    switch (rich) {
      case 1:
        copy_screen(hlpbuf,scr2);       /* Neues Bild->scr2 */
        scrl_li();
        break;
      case 3:
        copy_screen(hlpbuf,scr2);       /* Neues Bild->scr2 */
        scrl_re();
        break;
      case 0:                       /* Hoch */
        scrl_ru(hlpbuf);
        pic_move(hlpbuf,17,167,17);
        break;
      case 2:                       /* Runter */
        scrl_ho(hlpbuf);
        pic_move(hlpbuf,17,167,17);
        break;
      }

    mouse_on();         /* IKBD-Mouse on */
    }

	if (rohstoff_cheat) {
		oldlogbase=logbase;
		logbase=scr1;
		for(x=0;x<80;x++)
	  	for(y=0;y<36;y++) 
	    	if (get_raster(x,y)<=GOLD)        /* Hier ist ein Rohstoff */
					fill(15,x*4,25+y*4,x*4+3,28+y*4);
		logbase=oldlogbase;
		}
		
  copy_screen(scr1,scr2);         /* auf 2. Screen Åbertragen */

  Sm();

  switch_sprites();         /* Feindsprites an oder ausschalten */
  return(TRUE);
}

void load_map(nr)
int nr;             /* 0..13 */
{
  /* LÑdt Landschaft nr von Diskette */
	int nr_real;

  if (nr==SEA && map==SEA) return;                  /* von See->See */

  if (nr==SEA) load_objekte(SEA_OBJ,pack_buf);
  else {
		switch (nr) {
			case 16: nr_real=13; break;
			case 17: nr_real=6; break;
			default: nr_real=nr;
			}
		load_objekte(MAPA_OBJ+nr_real,pack_buf);
		}
	clear_screen(hlpbuf);
	draw_obj(0,pack_buf,0,hlpbuf,0,17);
	clear_screen(trf_buf);
	load_bibliothek(TMAPS_DAT,trf_buf);				/* Trafficbuffer laden */
  load_ground(nr);
  map=nr;
}

void load_ground(nr)
int nr;
{
  /* LÑdt den Untergrund eines Landes */
  register int i,j;
	int nr_real;

  if (nr==SEA) {
    for(i=0;i<1800;) {
      ground_buf[i++]=255;		/* Ganze GMAP mit Wasser fÅllen (packed) */
      }
    }
  else {
		switch (nr) {
			case 16: nr_real=13; break;
			case 17: nr_real=6; break;
			default: nr_real=nr;
			}
    load(GMAPS_DAT,ground_buf,1800L*nr_real,1800L);
    del_rohstoffe();             /* nicht benîtigte Rohstoffe lîschen */
    put_minen(nr);                /* eventuelle Minen einzeichnen */
    chg_ground(nr);               /* evtl. UntergrÅnde lîschen */
    }
}

void del_rohstoffe()
{
	/* Da viel zuviele Rohstoffe vorhanden sind, werden abhÑngig vom Level einige wieder
	   entfernt. */
		 
	int x,y;
	int counter,skip;
	int land;
	
	skip=1+(level-1.0)*9.0;						/* Anzahl der Rohstoffe, die Åbersprungen werden 
																		   1..10 */
	counter=startwert;	
	for(x=0;x<80;x++)
  	for(y=0;y<36;y++) {
			land=get_raster(x,y);
    	if (land!=GIMMIG && land>SCHATZ && land<=GOLD)        /* Hier ist ein Rohstoff */
				if (counter--<=0) counter=skip;
 				else put_raster(EBENE,x,y);						/* weglîschen */
			}
}

void put_minen(nr)
int nr;
{
  /* Nummer des Landes (=welt[pos_welt]) */
  int i;
  long save[2];

  for(i=0;i<minenzahl;i++) {
    if (mine[i].land==nr) {             /* Hier ist eine */
      draw_shift(hlpbuf,save,sprite_mem,0,63,mine[i].x-9,mine[i].y-17);
      draw_shift(hlpbuf,save,sprite_mem,0,64,mine[i].x-9,mine[i].y-9);

      put_untergrund(SCHATZ,mine[i].x,mine[i].y);       /* Schatz eintragen */
      }
    }
}

void chg_ground(nr)
int nr;                 /* Nummer des Landes */
{
  /* éndert in der aktuellen GMAP den Untergrund an bestimmten Stellen */
  int i,j;
	int untergrund;
	
  for(i=0;i<ground_nr;i++)
    if (new_ground[i].land==nr) {
			untergrund=get_raster(new_ground[i].x,new_ground[i].y);
			if (untergrund!=new_ground[i].untergrund) 
	      put_raster(new_ground[i].untergrund,new_ground[i].x,new_ground[i].y);
			else {
				ground_nr--;
				for(j=i;j<ground_nr;j++) 
					new_ground[i]=new_ground[i+1];			/* Eintrag unnîtig -> entfernen */
				i--;													/* und zurÅck */
				}
			}
}

void ground_change(wert,land,x,y)
int wert;
int land;
int x,y;                /* X/Y In Pixelkoordinaten */
{
  /* éndert das Land an dieser Stelle, und trÑgt es in die new_ground[]-Liste
     ein */
  register int i;
  register int xr,yr;

  if (ground_nr>=CHG_MAX)
    internal(romstr587);

  xr=x/4;
  yr=(y-25)/4;

  for(i=0;i<ground_nr;i++)
    if (new_ground[i].land==land &&                 /* Diesen Eintrag gab's schon */
            new_ground[i].x==xr && new_ground[i].y==yr) {
      new_ground[i].untergrund=wert;         /* Neuen Untergrund setzen */
      return;
      }

  new_ground[ground_nr].untergrund=wert;
  new_ground[ground_nr].land=land;
  new_ground[ground_nr].x=xr;
  new_ground[ground_nr++].y=yr;
}

void land_an()
{
  /* Schaltet das Land wieder an */
  int old_map;

  Hm();
  loc=LAND;
  leiste_y=18;
  msminy=17;

  writexy_anzeige();                      /* ggf. neue Moneywerte eintragen */

	logbase=scr1;
  show_screen(scr1);
  old_map=map;
  map=-1;
  land_map(old_map,FALSE,0,sx[0],sy[0]); 			/* nicht reinscrollen */

  if (auf_schiff) redraw_buttons(PAUSE|EDITOR|PEILUNG);   /* Wird hier was geÑndert, dann */
  else redraw_buttons(PAUSE|EDITOR|SEARCH|DIG|PEILUNG);   /* auch in sea_move()! */
                                    /* und in re_initialize() !!! */
                                    /* und in such_rohstoffe() */
	get_ground();
  test_ground();
	show_ground();
  initspr4map();
  Sm();
  button=NOTHING;
}

void clear_time()
{
  /* Lîscht die interne Uhr, (VBL_COUNTER) */

	timecounter=vbl_ct;
}

void check_time()
{
  /* öberprÅft, ob 1 Tag um */

  if (timecounter+70<vbl_ct) {               /* Alle 70/50 sec. ein Tag um */
    clear_time();               /* Uhr wieder lîschen */

    heute++;        /* ein Tag weiter */
    day += 1;

    if(day>30) {
      day = 1;
      month += 1;
      if(month>12) {
        month = 1;
        year += 1;
        }
      }

    prep_date();        /* Datumsstring erstellen */
    writexy_date();       /* Neues Datum ausgeben */

    if (day==1) monthly_events();
    daily_events();
    }
}

void show_ground()
{
  switch (ground_num) {
    case STADT_1_HAFEN:
    case STADT_2_HAFEN: leiste_oben(c_name[city_nr[map][ground_num-STADT_1_HAFEN]]);
                        break;
    case STADT_1_EIN:
    case STADT_2_EIN:
    case STADT_3_EIN:
    case STADT_4_EIN: leiste_oben(c_name[city_nr[map][ground_num-STADT_1_EIN]]);
                    break;
    default:        leiste_oben(ground_name[ground_num]);
                    break;
    }
}

void test_mine()
{
  /* öberprÅft, ob der Spieler in eine Mine will */
  register int i;
  int nr;               /* Nummer dieser Mine */
  long button_old;      /* Alter Button-Status */

  nr=-1;
  button_old=bleiste;

  if (ground_num==SCHATZ)
    for(i=0;i<minenzahl;i++)
      if (mine[i].land==map)           /* In diesem Land */
        if (mine[i].x==(sx[0]/4)*4+2 && mine[i].y==((sy[0]-25)/4)*4+27) {
          nr=i;
          break;
          }

  if (nr==-1) return;                   /* Keine Mine */

  if(!yes_no(romstr588)) {
    clear_buttons(TRUE);
    }
  else besuch_mine(nr);

  redraw_buttons(button_old);
  copy_buttons();

  sx[0]=sxalt[0];           /* Spieler zurÅck setzen */
  sy[0]=syalt[0];
  get_ground();
  show_ground();
}

void test_city()
{
  long old_buttons;
  int i;

  if ( (!auf_schiff && ground_num>=STADT_1_EIN && ground_num<=STADT_4_EIN)
     || (auf_schiff && ground_num>=STADT_1_HAFEN && ground_num<=STADT_2_HAFEN)) {

    if (auf_schiff) citynum=city_nr[map][ground_num-STADT_1_HAFEN];
    else citynum=city_nr[map][ground_num-STADT_1_EIN];

    old_buttons=bleiste;

    if (belong[citynum]==99) {                          /* Diese Stadt gehîrt Medusa */
      if (auf_schiff && armee_status!=AN_BORD)          /* Armee an Bord? */
        redraw_buttons(NO);                             /* Nein, kein Angriff mîglich */
      else redraw_buttons(NO|ATTACK);
      show_window(romstr589);
      }
    else {                                              /* Stadt gehîrt xxx */
      if (belong[citynum]>=0) {                         /* Stadt gehîrt Gegner */
        for(i=0;i<GEGNER;i++)
          if (steuer[i].heimat==citynum)                 /* Richtige Stadt? */
            if (steuer[i].angriff_zahl>=5) {             /* Schon 5 mal angegriffen */
              if (auf_schiff && armee_status!=AN_BORD)          /* Armee an Bord? */
                redraw_buttons(NO);                             /* Nein, kein Angriff mîglich */
              else redraw_buttons(NO|ATTACK);

              show_window(romstr590);
              break;
              }
        if (i==GEGNER) {                                /* normale Stadt */
          if (auf_schiff && armee_status!=AN_BORD)          /* Armee an Bord? */
            redraw_buttons(YES|NO);                             /* Nein, kein Angriff mîglich */
          else redraw_buttons(YES|NO|ATTACK);
          show_window(build(romstr591,c_name[citynum]));
          }
        }
      else {                                            /* Eigene Stadt */
        if (auf_schiff && armee_status!=AN_BORD)          /* Armee an Bord? */
          redraw_buttons(YES|NO);                             /* Nein, kein Angriff mîglich */
        else redraw_buttons(YES|NO|ATTACK);
        show_window(build(romstr592,c_name[citynum]));
        }
      }

    wait_mouse(0);

    do {
      wait_klick();
      } while(button != YES  && button != NO && button!=ATTACK);

    if (button==ATTACK) {
      show_window(build(romstr593,c_name[citynum]));
      attack_city(citynum);
      }
    if(button == NO) {
      redraw_buttons(old_buttons);
      citynum=-1;
      sx[0]=sxalt[0];
      sy[0]=syalt[0];
      get_ground();
      }
    if (button==YES || button==NO) hide_window();
    }
}

void test_bunker()
{
	/* Ermittelt, ob der Spieler in einen Bunker will, bzw. darf */
	int bunker_num;
	char *text;
	
  if (ground_num>=BUNKER_1 && ground_num<=BUNKER_2) {
    bunker_num=bunker_nr[map][ground_num-BUNKER_1];
		if (bunker_num>=0 && bunker_num<128 && bunker_bekannt[bunker_num]) {
			sx[0]=sxalt[0];								/* Spielfigur zurÅcksetzen */
			sy[0]=syalt[0];
			if (bunker_num==20) text=romstr594;
			else text=romstr595;
			if (yes_no(text)) {
				peilgeraet_aus();
				dungeon(bunker_num);							/* Wenn ein Stadtbunker geladen
																				wird, wird citynum entsprechend gesetzt und
																				das Hauptprogramm springt automatisch in die
																				entsprechende Stadt. */
				}
			}
		}
}

void such_platz()
{
  /* Sucht einen freien Platz fÅr den Spieler, also keine Berge, Wasser ,.. */
  int start_x,start_y,end_x,end_y;
  int x,y,wert;

  start_x=sx[0]/4-1; end_x=start_x+2;
  start_y=(sy[0]-25)/4; end_y=start_y+2;

  if (start_x<0) start_x=0;
  if (start_y<0) start_y=0;
  if (end_x>79) end_x=79;
  if (end_y>35) end_y=35;

  for(x=start_x;x<=end_x;x++)
    for(y=start_y;y<=end_y;y++) {
      wert=get_raster(x,y);
      if (wert==SCHATZ) continue;
      if (wert<=HUEGEL) goto label;
      }

label:

  sx[0]=x*4; sy[0]=y*4+25;
  get_ground();
}

void test_untergrund()
{
  /* öberprÅft, ob der Spieler sich im Sumpf oder verhexten Wald befindet,
     dann sterben nÑmlich die Leute */
  FLAG gestorben;
  int i;
  long tot;
	int proz;											/* Wieviel Prozent sterben? */
	unsigned long ungeschuetzt;
	float anteil;
		
  gestorben=FALSE;                      /* Bisher sind keine gestorben */

  if ((ground_num==SCHATZ || ground_num==GIMMIG) && treasure_cheat) blitz(0x007);

  if (sterben_jetzt) {
    if (ground_num==SUMPF) {
  		proz=1;
			ungeschuetzt=(armeegesamt[0]-armeeteil[0][5]*60);
      gestorben=TRUE;
      }
    if (ground_num==DRY_WUESTE) {       /* Radioaktiv verseucht */
			ungeschuetzt=(armeegesamt[0]-armeeteil[0][5]*30);
			proz=3;
      gestorben=TRUE;
      }

    if (gestorben && armeegesamt[0]!=0) {                      /* Sind Leute draufgegangen */
      for(i=0;i<EINHEITEN;i++)
	      if (i!=5) {                             /* Wizards sterben nicht */
					anteil=(float)armeeteil[0][i]/armeegesamt[0];		/* Anteil dieser Einheit */
					tot=anteil*ungeschuetzt;						/* Soviel sind ungeschuetzt */
					tot*=proz/100.0;
					if (tot<=0 && anteil*ungeschuetzt>0) tot=1;				/* Wenigstens 1 stirbt */
          minus_einheit(i,tot);
          }
      writexy_anzeige();                    /* Werte ausgeben */
      }
    }

  sterben_jetzt=!sterben_jetzt;         /* Bei jeden 2. Mal sterben */
}

void get_ground()
{
  if(sx[0] != -1)                   /* Sprite angeschaltet ? */
    ground_num = get_untergrund(sx[0],sy[0]);
}

void put_raster(wert,xr,yr)
int wert;
int xr,yr;
{
  /* éndert den Untergrund in der GMAP an Rasterstelle xr,yr */

  put_untergrund(wert,xr*4,yr*4+25);
}

void put_untergrund(wert,x,y)
int wert;                   /* Wert der dort hin soll */
int x,y;                    /* In Pixelkoordinaten */
{
  int zeile;
  int bit,byte;
  char op1,op2;
  int wort,loesch;
  int i;
  int op2_int;

  if (x<0 || x>319 || y<25 || y>168) {
    writexy(0,0,0,str(0,(long)x));
    writexy(0,0,6,str(0,(long)y));
    internal(romstr596);    /* auûerhalb */
    return;
    }

  zeile=(y-25)/4;
  bit=x/4*5;
  byte=bit/8;
  bit%=8;
  bit=11-bit;                                    /* von oben herunter */

  op1=ground_buf[zeile*50+byte];                /* erstes Byte */
  op2=ground_buf[zeile*50+byte+1];              /* nachfolgendes Byte */
  op2_int=op2;                                  /* Wird auf Wort erweitert */
  op2_int&=0x00ff;                              /* ABer nur unterstes Byte gÅltig */

  wort=(op1<<8) | op2_int;                      /* Beide Bytes als Wort */

  loesch=0xffe0;                                /* Die untersten 5 Bit gelîscht */
  for(i=0;i<bit;i++) {
    loesch<<=1;                     /* Eins nach links */
    loesch|=1;                      /* Und unterstes Bit wieder setzen */
    }
  wert<<=bit;                       /* Wert an Position schieben */

  wort&=loesch;                     /* Alten Wert lîschen */
  wort|=wert;                       /* Neuen Wert einodern */

  op2=(char)wort;                   /* Unterstes Byte */
  op1=wort>>8;                      /* oberes Byte */
  ground_buf[zeile*50+byte]=op1;
  ground_buf[zeile*50+byte+1]=op2;
}

int get_untergrund(x,y)
int x,y;
{
  /* Holt den Untergrund an der Stelle X/Y, Dabei sind die Werte 5Bit breit
     und gepackt */

  register int anzahl;
  register int wert;
  register char op2,op1;
  int zeile;
  register int bit;
  int byte;

  if (x<0 || x>319 || y<21 || y>168) {
    internal(romstr597);    /* auûerhalb */
    }

  if (y<25) return(EBENE);                  /* Auûerhalb ist Ebene */

  zeile=(y-25)/4;
  bit=x/4*5;
  byte=bit/8;
  bit%=8;
  bit=7-bit;                                    /* von oben herunter */
  op1=ground_buf[zeile*50+byte];                /* erstes Byte */
  op2=ground_buf[zeile*50+byte+1];              /* nachfolgendes Byte */
  anzahl=bit-4;                                 /* um soviel wird geschoben */
  if (anzahl>=0) {                              /* ist voll in op1 */
    wert=op1>>anzahl;                           /* hier verwendet TURBO_C 'ASR'! */
    wert&=31;                                   /* nur 5 Bits gÅltig */
    }
  else {
    wert=op1<<(-anzahl);                        /* aus op1 holen */
    wert&=31;                                   /* nur 5 Bits gÅltig */
    op2>>=8+anzahl;                             /* Achtung, anzahl ist negativ! */
                /* ^^^ TURBO_C verwendet ASR, d.h. obere Bits lîschen */
    op2&=(1<<(-anzahl))-1;
    wert|=op2;
    }

  return((int)wert);
}

int get_raster(x,y)
int x,y;
{
  /* holt sich aus GMAP den Untergrund, X/Y bereits in Rasterangaben */
  /* ist X/Y auûerhalb, gibt Funktion -1 zurÅck */

  return(get_untergrund(x*4,y*4+25));
}

int get_traffic(land,x,y)
int land,x,y;
{
  /* holt sich aus TMAP den WEG, X/Y bereits in Rasterangaben */
  /* ist X/Y auûerhalb, gibt Funktion -1 zurÅck */

  return(get_trafficxy(land,x*4,y*4+25));
}

void get2_coordinates()
{
  /*     Getmouse nur fuer Sprite 0   */
  /*     Sprites 1 - 4 Koordinaten annaeherung */

  hol_maus();
  sx[0] = mx;
  sy[0] = my;
}

void update_coordinates()
{
  /* Testet, ob Spieler Land verlassen will */

  int newpos,newx,newy;             /* neuen Koordinaten */
  int rich;                         /* Scrollrichtung */
  int boden;
  int newaltx,newalty;

  if (mx <=   8) mx = 8;
  if (my <= 24) my = 24;
  if (mx>=311) mx=311;
  if (my >= 167) my = 167;      /* Mauskoordinaten so verÑndern, daû nicht
                        aus dem Bildschirm gelaufen wird */

  sxalt[0]=sx[0];
  syalt[0]=sy[0];      /* alte Position sichern */

  if (auf_schiff) {                 /* Spieler will Schiff bewegen */
    if (mx > sxalt[0]) {
      boden=get_untergrund(sxalt[0]+1,syalt[0]);
      if (is_water(boden))
        sx[0] = sxalt[0] + 1;
      }
    if (mx < sxalt[0]) {
      boden=get_untergrund(sxalt[0]-1,syalt[0]);
      if (is_water(boden))
        sx[0] = sxalt[0] - 1;
      }
    if (my > syalt[0]) {
      boden=get_untergrund(sxalt[0],syalt[0]+1);
      if (is_water(boden))
        sy[0] = syalt[0] + 1;
      }
    if (my < syalt[0]) {
      boden=get_untergrund(sxalt[0],syalt[0]-1);
      if (is_water(boden))
        sy[0] = syalt[0] - 1;
      }

    /* Konnte Schiff gezogen werden? */
    if (sx[0]==sxalt[0] && sy[0]==syalt[0]) {           /* nein */
      if (mx>sxalt[0]) {                        /* nach rechts */
        test_fluss(sxalt[0]+1,syalt[0]-1);      /* rechts oben testen */
        test_fluss(sxalt[0]+1,syalt[0]+1);      /* rechts unten */
        }
      if (mx<sxalt[0]) {                        /* will nach links */
        test_fluss(sxalt[0]-1,syalt[0]-1);      /* links oben? */
        test_fluss(sxalt[0]-1,syalt[0]+1);      /* links unten? */
        }
      if (my>syalt[0]) {                        /* will nach unten */
        test_fluss(sxalt[0]+1,syalt[0]+1);      /* rechts unten? */
        test_fluss(sxalt[0]-1,syalt[0]+1);      /* links unten? */
        }
      if (my<syalt[0]) {
        test_fluss(sxalt[0]+1,syalt[0]-1);      /* rechts oben? */
        test_fluss(sxalt[0]-1,syalt[0]-1);      /* links oben? */
        }
      }
    }
  else {                /* Spieler bewegt Landarmee */
    if (mx > sxalt[0]) sx[0] = sxalt[0] + 1;      /* neue Position berechnen */
    if (mx < sxalt[0]) sx[0] = sxalt[0] - 1;
    if (my > syalt[0]) sy[0] = syalt[0] + 1;
    if (my < syalt[0]) sy[0] = syalt[0] - 1;
    }

  newpos=-1;                    /* Default, keine Grenze erreicht */

  if (sx[0]>=311) {
    newpos=pos_welt+1;
    if (newpos%15==0) newpos-=15;       /* rechts raus, links rein */
    newx=10;
    newy=newalty=sy[0];
    newaltx=0;
    rich=1;
    }
  if (sx[0]<=8) {
    newpos=pos_welt-1;
    if (newpos%15==14 || newpos<0) newpos+=15;      /* links raus, rechts rein */
    newx=309;
    newy=newalty=sy[0];
    newaltx=315;
    rich=3;
    }
  if (sy[0]>=167) {
    newpos=pos_welt+15;             /* Eine Zeile tiefer */
    if (newpos>=15*15) newpos%=15;  /* unten Raus, oben rein */
    newx=newaltx=sx[0];
    newy=27;
    newalty=20;
    rich=2;
    }
  if (sy[0]<=25) {
    newpos=pos_welt-15;             /* Eine Zeile hoch */
    if (newpos<0) newpos=14*15+(newpos+15)%15;           /* Oben raus, unten rein */
    newx=newaltx=sx[0];
    newy=165;
    newalty=170;
    rich=0;
    }
  if (newpos!=-1) {                 /* Grenze erreicht */
    if (land_map(welt[newpos],TRUE,rich,newx,newy)) {   /* Laden und anzeigen */
      sx[0]=newx;
      sy[0]=newy;                /* Neue X/Y Position setzen */
      sxalt[0]=newaltx;
      syalt[0]=newalty;
      pos_welt=newpos;                        /* Neue Position Åbernehmen */
      initspr4map();            /* Sprites zeichnen */
      }
    get_ground();
    }
}

FLAG is_water(wert)
int wert;
{
  /* Testet, ob ein Untergrund Åbergeben wurde, der beschiffbar ist */

  return(wert>=STADT_1_HAFEN && wert<=WASSER);
}

FLAG is_ground(x,y)
int x,y;
{
  /* Testet, ob Untergrund in Rasterkoordinaten an dieser Stelle
     von der Armee belaufen werden kann. */
  int wert;

  wert=get_raster(x,y);
  return(wert<STADT_1_EIN);
}

void test_fluss(x,y)
int x,y;
{
  /* öberprÅft, ob an diesen Koordinaten ein Fluû liegt, und zieht ggf. hin */

  if (get_untergrund(x,y)==WASSER) {
    sx[0]=x;
    sy[0]=y;
    }
}

void test_ground()
{
        /* Ermittelt Untergrund fÅr Spielersprite */
	register int x,y;
	int schild;

	if (ground_num!=SCHILD) auf_schild=FALSE;

  if (auf_schiff) {                 /* Spieler will Schiff bewegen */
    sn[0]=3;                           /* Default, Schiff rechts */
    if (sy[0]<syalt[0]) sn[0]=5;       /* hoch */
    if (sy[0]>syalt[0]) sn[0]=4;       /* runter */
    if (sx[0]>sxalt[0]) sn[0]=3;       /* Schiff rechts */
    if (sx[0]<sxalt[0]) sn[0]=2;       /* Schiff links */
    }
  else {
    switch(ground_num) {
        case LICHTER_WALD:
          if (sn[0]==0) sn[0]=-1;
          else sn[0]=0;
          break;
        case WALD: sn[0] = 1;     /* Playersymbol ohne Rumpf */
                 break;
        case WASSER:
				case STADT_1_HAFEN:
				case STADT_2_HAFEN:
				case VULKANE:
        case HOHE_BERGE:
                 sx[0]= sxalt[0];			/* Figur zurÅcksetzen */
                 sy[0]= syalt[0];
                 get_ground();
                 break;
				case SCHILD:
					if (!auf_schild) {
						auf_schild=TRUE;
						schild=-1;
				    for(y=0;y<36;y++)
							for(x=0;x<80;x++)
					      if (get_raster(x,y)==SCHILD) {
				    	    schild++;
									if (x==sx[0]/4 && (sy[0]-25)/4==y) {
										show_schild((int)schild_nr[map]+schild);
										goto raus;
										}
									}
	raus:			break;
						}
        default: sn[0]=0;          /* normale Figur */
        }
    }
}

void show_schild(nr)
int nr;
{
	/* Zeigt das Schild mit der entsprechenden Nummer an */
	register char *poi,*start;

#if defined(DEUTSCH)
	load_bibliothek(SCHILD_TXT,pack_buf);
#elif defined(ENGLISCH)
	load_bibliothek(SCHILD_G_TXT,pack_buf);
#elif defined(FRANZ)
	load_bibliothek(SCHILD_TXT,pack_buf);
#endif

	poi=(char *)pack_buf;
	for(;nr>0;nr--) {
		while(*poi!=13 && *poi!=10) poi++;					/* Stringende suchen */
		while(*poi==10 || *poi==13) poi++;					/* Trennzeichen Åberspringen */
		}
	start=poi;
	while(*poi!=13 && *poi!=10) poi++;					/* Stringende suchen */
	*poi=0;																/* EOS setzen */
	alert(start);
}

void schiff_test()
{
  /* öberprÅft, ob in einer Stadt ein Schiff fertig gebaut ist */
  register int i,j;
  int index;

  for(i=0;i<CITIES;i++)
    for(j=0;j<5;j++)
      if (ship_art[i][j]>=0 && ship_fertig[i][j]<=heute) {
        alert(build(romstr598,
        										c_name[i],ship_name[ship_art[i][j]]));
        index=frei_flotte();
        if (index==-1)
          internal(romstr599);

        else {
          schiffbau--;
          schiffbesitz++;
          ship_own[ship_art[i][j]]++;
          flotte[index].typ=ship_art[i][j];
          flotte[index].aboard=0;               /* 0 an Bord */
          flotte[index].salary=0;               /* keine Kosten */
          flotte[index].prod_datum=heute;       /* Produktionsdatum einsetzen */
          flotte[index].ort=i;                  /* Schiff ist in dieser Stadt */
          }
        ship_art[i][j]=-1;      /* Arbeitsdock freigeben */
        }
}

int frei_flotte()
{
  /* Sucht nach einem freien Eintrag in flotte, bei Fehler -1 */
  register int i;

  for(i=0;i<SCHIFFMAX;i++) if (flotte[i].typ==-1) return(i);
  return(-1);
}

void such_xy(nr)
int nr;
{
  /* In steuer[nr] muû die Startstadt eingetragen sein, dann berechnet diese
     Funktion die Start-X/Y des Armeesprites.
     Weiterhin wird die Nummer des Landes bestimmt, in dem
     die Stadt liegt, wo die Armee startet.  */

  int land,stadt,x,y;
  int i;

  for (land=0;land<LAENDER;land++)
    for(stadt=0;stadt<4;stadt++)
      if (city_nr[land][stadt]==steuer[nr].heimat) goto raus;

raus:

  for(i=0;i<225;i++)
		if (welt[i]==land) {
    	steuer[nr].weltpos=i;
    	break;
      }

  for(y=0;y<36;y++)
    for(x=0;x<80;x++)
      if (get_traffic(land,x,y)==TSTADT_1+stadt) {
        steuer[nr].x=x*4+2;
        steuer[nr].y=y*4+25+2;
        return;
        }

	writexy_fast(0,0,str(0,(long)steuer[nr].heimat));
  internal(romstr600);					/* X/Y Position ? */
}

void such_weg(nr)
int nr;
{
  /* Bestimmt einen neuen Wert fÅr aktweg, der aber nicht der Alte sein darf
     Weiterhin wird vx,vy auf Null gesetzt, damit beim ersten Aufruf eine
     neue Richtung bestimmt wird.
     Auch wait wird auf Null gesetzt. */

  register int x,y;
  int weg;
  int anzahl;

  anzahl=0;

  for(x=steuer[nr].x-4;x<=steuer[nr].x+4;x+=4)
    for(y=steuer[nr].y-4;y<=steuer[nr].y+4;y+=4) {
      weg=get_trafficxy(welt[steuer[nr].weltpos],x,y);
      if (weg>KRZG && weg<TPORT_1) {            /* Weg gefunden */
        anzahl++;                               /* Anzahl der Wege suchen */
        }
      }

  if (anzahl==1) steuer[nr].akt_weg=-1;     /* Wenn nur ein Weg, dann alle gÅltig */
  while (such_new(nr,anzahl)) ;             /* Neuen Weg suchen */

  steuer[nr].vx=steuer[nr].vy=0;
  steuer[nr].wait=0;
}

FLAG such_new(nr,anzahl)
int nr,anzahl;
{
  /* Sucht nach einem neuen Weg, wobei anzahl Wege zur VerfÅgung stehen
     und nicht derselbe Weg wie bisher genommen werden darf. */
  int x,y;
  int counter;
  FLAG nochmal;
  int weg;

  counter=zufall(anzahl);
  nochmal=FALSE;

  for(x=steuer[nr].x-4;x<=steuer[nr].x+4;x+=4)
    for(y=steuer[nr].y-4;y<=steuer[nr].y+4;y+=4) {
      weg=get_trafficxy(welt[steuer[nr].weltpos],x,y);
      if (weg>KRZG && weg<TPORT_1) {           /* Weg gefunden */
        counter--;
        if (counter<0) {                        /* Dieser Weg ist es */
          if (steuer[nr].akt_weg+1==weg)        /* nicht nochmal denselben Weg */
            nochmal=TRUE;
          else steuer[nr].akt_weg=weg-1;
          goto raus;
          }
        }
      }

raus:

  return(nochmal);
}

void set_armies()
{
  /* Setzt alle Armeen auf einen bestimmten Ort */
  int i;
	int rnd;

  army_unten=TRUE;                    /* ALs erstes die unteren Armeen bewegen */

	clear_screen(trf_buf);
	load_bibliothek(TMAPS_DAT,trf_buf);		/* Trafficmaps laden */

  for(i=0;i<GEGNER;i++) {
    do {
			do {
	  		rnd=zufall(22);						/* Stadtnummer ermitteln */
				} while ((rnd>=13 && rnd<=15) || (rnd>=19 && rnd<=20));			/* Nicht auf Insel */
	    steuer[i].heimat=rnd; 		         		/* Heimatstadt */
			steuer[i].akt_weg=-1;					/* Jeder Weg ist gÅltig */
      } while(belong[steuer[i].heimat]!=99);
    belong[steuer[i].heimat]=i;         	  /* Stadt gehîrt dieser Armee */
    steuer[i].auf_wasser=FALSE;       	   	/* Nicht mit Schiff unterwegs */
    steuer[i].angriff_zahl=0;         	   	/* Wurde noch nie angegriffen */
	  steuer[i].wait=0;												/* Es wird nicht gewartet */
    steuer[i].sprite=-1;										/* Sprite ist abgeschaltet */
    such_xy(i);                   					/* Holt sich die Startkoordinaten */
    }

  army_unten=TRUE;
  move_armies();                /* Untere HÑlfte bewegen */
  move_armies();                /* Und jetzt die obere HÑlfte */
}

void move_armies()
{
  /* Bewegt die gegnerischen Armeen auf dem Spielfeld */

  register int i;
  int grund;
  int nr;
  int rnd;
  int start,ende;

  if (army_unten) {
    army_unten=FALSE;           								/* Die Untere HÑlfte bewegen */
    start=0;
    ende=GEGNER/2;
    }
  else {
    army_unten=TRUE;            								/* Die obere HÑlfte bewegen */
    start=GEGNER/2;
    ende=GEGNER;
    }

  for(i=start;i<ende;i++)
    if (steuer[i].heimat!=-1) {									/* Existiert die Armee noch? */
			move_army(i);               							/* Berechnet neue Spriteposition */
      nr=steuer[i].sprite;
      if (nr!=-1) {															/* Armee zu sehen? */
        sx[nr]=steuer[i].x;
        sy[nr]=steuer[i].y;        							/* und als Spritekoordinaten sichern */
        grund=get_untergrund(sx[nr],sy[nr]);
	      if (grund==WALD && FALSE) {
          sn[nr]=-1;                        			/* Im Wald abschalten */
          if (steuer[i].wait==0) {              /* wartet noch nicht */
            rnd=zufall(150);
            if (rnd==5) {
              steuer[i].wait=zufall(8000);
              }
            }
          }
        else {                          /* Armee ist nicht im Wald */
          if (steuer[i].auf_wasser) {   /* Armee ist im Wasser */
            if (steuer[i].vx==0) {      /* Bewegt sich senkrecht */
              if (steuer[i].vy>=0) sn[nr]=4;
              else sn[nr]=5;
              }
            else {                      /* Bewegt sich waagrecht */
              if (steuer[i].vx>=0) sn[nr]=3;
              else sn[nr]=2;
              }
            }
          else sn[nr]=6;                  /* Armee ist an Land */
          }
        }
      }
}

void move_army(nr)
register int nr;
{
  /* Bewegt eine gegnerische Armee um ein Pixel weiter */
  int tw;           /* Wert der TMAP an X/Y */

  armee_aktiv=nr;

  if (steuer[nr].wait>0) {							/* Nocheine Weile warten? */
    steuer[nr].wait--;
    return;
    }

  if (steuer[nr].x % 4==2 && (steuer[nr].y-25) % 4==2) {    /* Mitte des Kastens */
    tw=get_trafficxy(welt[steuer[nr].weltpos],steuer[nr].x,steuer[nr].y);
    if (tw==KRZG) {   /* Auf Kreuzung */
      kreuzung(nr);
      }
    if (tw>=TPORT_1 && tw<=TSTADT_4)					/* Auf Stadt/Burg-Eingang */
      stadt_erreicht(nr);

    neue_richtung(nr);              /* Ermittelt neue Richtung fÅr Armee */
    }
  steuer[nr].x+=steuer[nr].vx;
  steuer[nr].y+=steuer[nr].vy;         /* Neue Position errechnen */
}

void kreuzung(nr)
register int nr;
{
  /* Feind befindet sich auf Kreuzung */

  if (steuer[nr].x==10) {           /* linker Rand */
    steuer[nr].x=314;               /* jetzt wieder rechter Rand */
    steuer[nr].weltpos--;           /* ein Land nach links */
    steuer[nr].akt_weg=-1;          /* Alle Wege erlaubt */
    }
  else if (steuer[nr].x==314) {           /* linker Rand */
    steuer[nr].x=10;               /* jetzt wieder rechter Rand */
    steuer[nr].weltpos++;           /* ein Land nach links */
    steuer[nr].akt_weg=-1;          /* Alle Wege erlaubt */
    }
  if (steuer[nr].y==27) {           /* linker Rand */
    steuer[nr].y=163;               /* jetzt wieder rechter Rand */
    steuer[nr].weltpos-=15;           /* ein Land nach links */
    steuer[nr].akt_weg=-1;          /* Alle Wege erlaubt */
    }
  else if (steuer[nr].y==163) {           /* linker Rand */
    steuer[nr].y=27;               /* jetzt wieder rechter Rand */
    steuer[nr].weltpos+=15;           /* ein Land nach links */
    steuer[nr].akt_weg=-1;          /* Alle Wege erlaubt */
    }
  such_weg(nr);                     /* neuen Weg auswÑhlen */
}

void switch_sprites()
{
  /* Schaltet die Feindsprites an oder aus, je nach Land (pos_welt) */
  /* und Abstand zur eigenen Truppe, bzw. Anzahl der Scouts */

  register int i;
  int nr;
  int abstand;
  long dx,dy;
  FLAG anschalten;

  for(i=0;i<GEGNER;i++) {
    if (steuer[i].weltpos==pos_welt) {            		/* Gegner ist in diesem Land */
      dx=steuer[i].x-sx[0];
      dy=steuer[i].y-sy[0];                       		/* Delta X/Y berechnen */
      abstand=(int)sqrt((double)(dx*dx+dy*dy));  			/* Abstand berechnen */
      if (abstand<armeeteil[0][3]/3 || pio_cheat) anschalten=TRUE;
			else if (abstand<8) anschalten=TRUE;				/* ein biûchen sieht man immer */
      else anschalten=FALSE;
      if (steuer[i].auf_wasser) anschalten=TRUE;
      }
    else anschalten=FALSE;                        /* Gegner nicht im Land */

    if (steuer[i].heimat==-1) anschalten=FALSE;    /* Armee ist tot */

    if (anschalten && steuer[i].sprite==-1) {
            /* Gegner soll an sein, ist aber noch nicht angeschaltet */
      nr=such_sprite();                 /* Sucht freien Sprite */
      sx[nr]=steuer[i].x;               /* belegen */
      sy[nr]=steuer[i].y;               /* X/Y setzen */
      if (steuer[i].auf_wasser) sn[nr]=4;
      else sn[nr]=6;
      steuer[i].sprite=nr;
      }
    if (!anschalten && steuer[i].sprite!=-1) {
            /* Gegner soll aus sein, ist aber angeschaltet */
      sx[steuer[i].sprite]=-1;          /* Sprite freigeben */
      steuer[i].sprite=-1;
      }
    }

  if (auf_schiff)                           /* Zur Zeit Schiff aktiv? */
    if (armee_pos!=pos_welt) sn[1]=-1; /* Wenn Armee nicht hier, dann ausschalten */
    else sn[1]=0;                      /* Armee anschalten */
  else sn[1]=-1;                       /* Schiffssprite weg */
}

int such_sprite()
{
  /* sucht nach einem freien Gegner-sprite */
  register int i;

  for(i=2;i<SPR_MAX;i++) if (sx[i]==-1) return(i);
  internal(romstr601);
  return(-1);
}

void stadt_erreicht(nr)
int nr;
{
  /* Die Armee befindet sich in der TMAP auf einem Stadt, Hausfeld */
  int tw;
  register int stadt_nr;
  register int land;
  int wert;
  long stadt_gesamt,grenze;
  register int i;

  land=welt[steuer[nr].weltpos];

  tw=get_trafficxy(land,steuer[nr].x,steuer[nr].y);
  if (tw<=TSTADT_4) {                                     /* Stadt? */
    if (steuer[nr].auf_wasser)
      stadt_nr=city_nr[land][tw-TPORT_1];
    else
      stadt_nr=city_nr[land][tw-TSTADT_1];

    if (belong[stadt_nr]<0 && armeegesamt[nr+1]>50) {  /* Gehîrt die Stadt Spieler? */
      stadt_gesamt=0;
      for(i=0;i<EINHEITEN;i++) stadt_gesamt+=stadt_armee[stadt_nr][i];

      grenze=50*stadt_gesamt/armeegesamt[nr+1];
      if (zufall(100)>grenze)
        angriff_spieler_stadt(stadt_nr,nr+1);
      }
    if (belong[stadt_nr]==nr) {         /* Gehîrt die Stadt dieser Armee? */
      stadt_gesamt=0;
      for(i=0;i<EINHEITEN;i++) stadt_gesamt+=stadt_armee[stadt_nr][i];

      if (armeegesamt[nr+1]>1200 || stadt_gesamt==0)       /* Kann Armee was entbehren? */
        for(i=0;i<EINHEITEN;i++) {
          stadt_armee[stadt_nr][i]+=armeeteil[nr+1][i]/2;
          armeegesamt[nr+1]-=armeeteil[nr+1][i]/2;
          armeeteil[nr+1][i]/=2;
          }
      }
    if (port_city[stadt_nr])              /* Hat Stadt einen Hafen? */
      if (!steuer[nr].auf_wasser) {       /* Armee war an Land */
        if (zufall(5)==3) {               /* Nur manchmal */
          wert=TPORT_1+(tw-TSTADT_1);     /* gesuchter Wert */
          if (such_ort(nr,wert)) {        /* Ort gefunden */
            steuer[nr].akt_weg=-1;        /* Jeder Weg ist gÅltig */
            steuer[nr].auf_wasser=TRUE;   /* Sprite ist jetzt Schiff */
            }
          else {
            internal(romstr602);				/* Hafen nicht gefunden */
            }
          }
        }
      else {                              /* Armee ist schon auf Schiff */
        if (zufall(4)==2) {               /* Nur manchmal */
          wert=TSTADT_1+(tw-TPORT_1);
          if (such_ort(nr,wert)) {
            steuer[nr].akt_weg=-1;        /* Jeder Weg ist gÅltig */
            steuer[nr].auf_wasser=FALSE;
            }
          else {
            internal(romstr603);				/* Eingang nicht gefunden */
            }
          }
        }
    }

  such_weg(nr);								/* und marschieren */
}

#ifndef COPYPROT

void show_traffic(land)
int land;
{
  /* Zeigt die Trafficmap eines bestimmten Landes an */
  int x,y;

  Hm();

  fill(0,0,0,319,199);

  for(y=0;y<36;y++)
    for(x=0;x<80;x++)
      writexy(2,x*4,y*5,str(0,(long)get_traffic(land,x,y)));
  Sm();
}


void writexy_werte(nr)
int nr;
{
  /* Schreibt alle Werte der Armee raus, DEBUG! */
	char land[2];

	logbase=scr1;

  fill(0,0,0,70,26);

  writexy(15,0,0,romstr604);
  land[0]='A'+welt[steuer[nr].weltpos];
	land[1]=0;
	writexy(15,24,0,land);
  writexy(15,0,6,romstr605);
  writexy(15,20,6,str(3,(long)steuer[nr].x));
  writexy(15,36,6,str(0,(long)steuer[nr].y));
  writexy(15,0,12,romstr606);
  writexy(15,60,12,str(0,(long)get_trafficxy(welt[steuer[nr].weltpos],
                    steuer[nr].x,steuer[nr].y)));
  writexy(15,0,18,romstr607);
	writexy(15,40,18,str(0,(long)steuer[nr].akt_weg));

	/* wait_key();
	show_traffic(welt[steuer[nr].weltpos]); */
}


#endif

FLAG such_ort(nr,wert)
int nr,wert;
{
  /* Sucht 24 Pixel nach allen Richtungen nach wert */
  register int x,y;
  register int yo,yu;
  register int *xo,*xu;             /* Zeigt auf xo,xu */
  int xo2,xu2;
  int land;
  int tw;

  land=welt[steuer[nr].weltpos];

  xu2=steuer[nr].x-24;
  xo2=steuer[nr].x+24;
  xu=&xu2;
  xo=&xo2;
  yu=steuer[nr].y-24;
  yo=steuer[nr].y+24;
  for(x=*xu;x<=*xo;x+=4)
    for(y=yu;y<=yo;y+=4) {
      tw=get_trafficxy(land,x,y);
      if (tw==wert) {
        steuer[nr].x=x;
        steuer[nr].y=y;
        return(TRUE);
        }
      }

  return(FALSE);
}

void neue_richtung(nr)
int nr;
{
  /* Armee befindet sich in der Mitte eines Kastens, nun muû eine neue
    Zugrichtung gesucht werden, wobei sie nicht die Alte sein darf,
    da die Armee ja dann rÅckwÑrts ziehen wÅrde. Dabei wird akt_weg benÅtzt. */

  int vvx,vvy;              /* neuer Richtungsvektor */
  register int xx,yy;
  register int rx,ry;          /* Rasterkoordinaten des momentanen Standpunktes */
  int tw;                   /* Trafficwert, Wert der Tmap */
  int alter_weg;            /* Index des alten Weges */
  int land;

  land=welt[steuer[nr].weltpos];
  alter_weg=steuer[nr].akt_weg+1;    /* +1 weil 1. Weg in TMAP Wert 2 hat! */

  rx=steuer[nr].x/4;
  ry=(steuer[nr].y-25)/4;              /* In Rasterkoordinaten wandeln */

  for (yy=ry-1;yy<=ry+1;yy++)
    for (xx=rx-1;xx<=rx+1;xx++)        /* 9 KÑstchen drum'rum prÅfen */
      if (get_traffic(land,xx,yy)==alter_weg && (xx!=rx || yy!=ry)) {
        vvx=xx-rx;
        vvy=yy-ry;          /* Richtungvektor zu diesem Kasten berechnen */
        if (vvx==-steuer[nr].vx && vvy==-steuer[nr].vy) continue;
                        /* Das war gerade der Kasten, aus dem er kam */
        steuer[nr].vx=vvx;
        steuer[nr].vy=vvy;      /* freier Weg, hinziehen */
        return;                 /* und raus aus der Routine */
        }

  /* Es wurde kein entsprechender Weg gefunden, also muû nach einer */
  /* Kreuzung gesucht werden, auf der der Weg forgesetzt wird */

  for (yy=ry-1;yy<=ry+1;yy++)
    for (xx=rx-1;xx<=rx+1;xx++) {       /* 9 KÑstchen drum'rum prÅfen */
      if (get_traffic(land,xx,yy)==KRZG && (xx!=rx || yy!=ry)) {     /* richtiger Weg? */
        vvx=xx-rx;
        vvy=yy-ry;          /* Richtungvektor zu diesem Kasten berechnen */
        if (vvx==-steuer[nr].vx && vvy==-steuer[nr].vy) continue;
                        /* Das war gerade der Kasten, aus dem er kam */
        steuer[nr].vx=vvx;
        steuer[nr].vy=vvy;      /* freier Weg, hinziehen */
        return;                 /* und raus aus der Routine */
        }
      }

  /* Es wurde kein Weg, keine Kreuzung gefunden, also CITY suchen */
  for (yy=ry-1;yy<=ry+1;yy++)
    for (xx=rx-1;xx<=rx+1;xx++) {       /* 9 KÑstchen drum'rum prÅfen */
      tw=get_traffic(land,xx,yy);            /* Wert der Tmap holen */
      if ( (tw>=TPORT_1 && tw<=TSTADT_4) && (xx!=rx || yy!=ry)) {
                            /* richtiger Weg? */
        vvx=xx-rx;
        vvy=yy-ry;          /* Richtungvektor zu diesem Kasten berechnen */
        if (vvx==-steuer[nr].vx && vvy==-steuer[nr].vy) continue;
                        /* Das war gerade der Kasten, aus dem er kam */
        steuer[nr].vx=vvx;
        steuer[nr].vy=vvy;      /* freier Weg, hinziehen */
        return;                 /* und raus aus der Routine */
        }
      }

  internal(romstr608);          /* Es wurde kein Weg gefunden */
}

void make_world()
{
  /* Erstellt die Welt in welt[] */
  int i;
	int pos;

  for(i=0;i<225;i++) welt[i]=SEA;           /* Am Anfang war Åberall Wasser... */

	welt[95]=11;
  welt[96]=10;
  welt[97]=9;
  welt[110]=7;
  welt[111]=4;
  welt[112]=3;
  welt[124]=6;              /* Hauptkontinent */
  welt[125]=5;
  welt[126]=0;
  welt[127]=1;
  welt[128]=2;
	welt[141]=13;

  welt[such_insel()]=8; 
	welt[such_insel()]=12;
	
  pos=such_insel();           /* Inseln plazieren */
	welt[pos]=14;
	welt[pos+15]=16;					/* = 13,N */

  pos=such_insel();
	welt[pos++]=17;						/* = 6,G */
	welt[pos]=15;
}

int such_insel()
{
  /* Sucht einen freien Platz in welt[] */
  int x,y;
  FLAG belegt;
  register int x2,y2;

  do {
    belegt=FALSE;
    x=zufall(15);
    y=zufall(15);
    for(x2=x-3;x2<=x+3;x2++)
      for(y2=y-3;y2<=y+3;y2++)
        if (get_land(x2,y2)!=SEA) {
          belegt=TRUE;
          break;
          }
    } while(belegt);

	return(x+y*15);
}

short get_land(x,y)
int x,y;
{
  /* Holt aus welt[] die Landschaft unter BerÅcksichtigung der Kugerlform */

  if (x<0) x+=15;
  if (x>=15) x-=15;
  if (y<0) y+=15;
  if (y>=15) y-=15;

  return(welt[x+y*15]);
}


