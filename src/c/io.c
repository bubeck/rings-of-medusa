  
/* Dieser Part stellt alle wichtigen EIN/AUSGABE-Routinen zur Verfügung,
   wie z.B. die Spritedarstellung, die Schriftausgabe etc...

   Copyright 1989 by Till Bubeck, Ziegeleistr. 28, 7056 Weinstadt
                                  Tel.: 07151-66437                 */

#include "includes.c"           /* Definiert alle Variablen als Extern,... */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>

#ifndef O_BINARY
#define O_BINARY 0
#endif

int windx,windy;                /* Koordinaten des Fensters */

RGB_PAL kartep = {     0x000,0x300,0x520,0x642,0x753,0x030,0x242,0x353,
                   0x464,0x057,0x077,0x333,0x444,0x555,0x666,0x777};
RGB_PAL formularp = {  0x000,0x700,0x050,0x007,0x431,0x542,0x653,0x764,
                   0x707,0x321,0x077,0x222,0x400,0x444,0x555,0x777 };
RGB_PAL leistep = {    0x000,0x603,0x740,0x760,0x770,0x577,0x066,0x050,
                   0x044,0x004,0x325,0x436,0x536,0x647,0x757,0x777};

/* Palette of METALL_1.IFF: Saved by NEOchrome V2.22 by Chaos, Inc. */
RGB_PAL scroller_pal2 = { 
	0x0000,0x0002,0x0013,0x0024,0x0035,0x0046,0x0057,0x0067,
	0x0377,0x0577,0x0677,0x0777,0x0000,0x0000,0x0000,0x0700 };

char build_str[200];					/* Hier baut build seine strings auf */
unsigned long sync_ct=0;

/* Start: */

#if FALSE

void linker(file_nr,fun_nr,par1,par2,par3)
int file_nr,fun_nr;
int par1,par2;
void *par3;
{
  /* Diese Funktion ist die C-Implementierung des Run-Time Linkers */
  
  switch (file_nr) {
    case 0: fight_main(fun_nr,par1,par2,par3); break;
    default: internal(romstr005);
    }
}

#endif

void writexy(farbe,x,y,string)
int farbe,x,y;
char string[];
{
  /* Gibt String aus */
  int hintergrund;

  hintergrund=get_pixel(x-x%4,y);
  writexy_it(farbe,hintergrund,x,y,string);
}

void writexy_it(farbe,hintergrund,x,y,string)
int farbe,hintergrund,x,y;
char string[];
{
  /* Gibt String aus, schnell */
  int i;
  int x_anfang;

  x_anfang=x;

  for(i=0;string[i]!=0;i++,x+=4) {
    if (string[i]!='~') char_out(farbe,hintergrund,x,y,string[i]);
    else {
      x=x_anfang-4;                   /* CR/LF */
      y+=6;
      }
    }
}

void writexy_trans(farbe,x,y,string)
int farbe,x,y;
char string[];
{
  /* Gibt String aus, aber Transparent, d.h. ohne Zerstören des Backgrounds */
  int i;
  int x_anfang;

  x_anfang=x;

  for(i=0;string[i]!=0;i++,x+=4) {
    if (string[i]!='~') char_trans(farbe,x,y,string[i]);
    else {
      x=x_anfang-4;                   /* CR/LF */
      y+=6;
      }
    }
}

void writexy_fast(x1,y1,string)
int x1,y1;
char string[];
{
  /* Gibt String aus, sehr schnell, Schriftfarbe weiß auf schwarz */
  register int i;

  for(i=0;string[i]!=0;i++,x1+=4) char_fast(x1,y1,string[i]);
}

void center(farbe,y,str)
int farbe,y;
char str[];
{
  const char *start = str;
  const char *p;
  char zeile[100];
  
  while ((p = strchr(start, '\n')) != NULL) {
    sprintf(zeile, "%.*s", (int)(p - start), start);
    writexy(farbe,(320-strlen(zeile)*4)/2,y,zeile);
    y += 7;
    start = p + 1; // nach dem '\n' weitermachen
  }

  // falls letzte Zeile nicht mit \n endet
  if (*start != '\0') {
    sprintf(zeile, "%s", start);
    writexy(farbe,(320-strlen(zeile)*4)/2,y,zeile);
  }
}

void writexy_money(x,y)
int x,y;
{
  /* Gibt an der Stelle geld aus */

  writexy(3,x,y,romstr006);
  writexy(3,x+48,y,str(10,money));
}

FLAG yes_no(string)
char string[];
{
  /* Macht eine Ja/Nein Abfrage */
  /* Gibt TRUE zurück wenn Ja angeklickt */
  long old_buttons;

	show_window(string);
  old_buttons=bleiste;
  redraw_buttons(YES|NO|PAUSE);

  do {
    wait_klick();
    } while (button!=YES && button!=NO);

  redraw_buttons(old_buttons);
	hide_window();
	
  return(button==YES);
}

void show_window(string)
char string[];
{
  /* Zeigt das Fenster in der Mitte des Bildschirms an */

  show_windxy(106,90,string);
}

void show_windxy(x,y,string)
int x,y;
char string[];      
{
  /* Zeigt das Fenster an der Position an */
  int len[5];               /* Stringlängen */
  int anzahl;
  int delta_y;               /* Delta Y für die Strings */
  int i,j;
	int ob_nr;
	char string_neu[150];
	char string_format[150];
  void *oldlogbase;
	
	char *zeilen[5];					/* Pointer auf die 5 Zeilen */
	int laenge;
	int breite;
	int color;
	
	while (is_key()) wait_key();			/* Tastaturbuffer leeren */
	
  windx=x; windy=y;         /* Koordinaten des Fensters für Hide_window sichern */

	laenge=strlen(string);

	/* Alle doppelten Spaces eliminieren: */
	for(i=0,j=0;i<=laenge;) {
	  string_neu[j]=string[i++];
	  if (string_neu[j++]==' ' && string[i]==' ') j--;
	  }

	for(i=0;i<strlen(string_neu);i++) 				/* @->Space */
		if (string_neu[i]=='@') string_neu[i]=' ';
		
	/* Die richtige Breite raussuchen: */	
	for(breite=20;breite<35;breite++) {
		strcpy(string_format,string_neu);
		if (word_wrap(breite,zeilen,string_format)) break; 						/* Paßt es bei dieser Breite? */
		}

  for(i=0;i<5;i++) {
    len[i]=strlen(zeilen[i]);
    if (len[i]==1 && *zeilen[i]==' ') len[i]=0;					/* Nur Space->String leer */
    }
  
  for(anzahl=0;anzahl<5;anzahl++) if (len[anzahl]==0) break;

  if (anzahl>0) delta_y=40/anzahl;        /* Delta_Y für die Zeilen bestimmen */

  wind_aktiv=TRUE;          /* Fenster ist aktiv */
  Hm();

  if (loc==LAND) ob_nr=0;
  else ob_nr=1;

	/* Da bei der Amigaversion dieser raster_transp mit einer Zielbreite von !=320 nicht funktioniert,
		 mußte ich mir so behelfen: */
		 
  oldlogbase=logbase;
  //logbase=&x_window;
  logbase=scr1;

#ifdef AMIGA
	memset(window_back,0,sizeof(window_back));
	raster_transp(x,y,x+105,y+47,scr1,320,0,0,window_back,112);	/* Hintergrund retten */
#else
  raster_replace(x,y,x+105,y+47,logbase,320,0,0,window_back,112);  /* Hintergrund retten */
#endif

	grow_box(windx,windy,106,48);

	if (alerts_da) {
    draw_obj(ob_nr,wind_form,ODER,logbase,x,y);
		color=0;
		}
	else {
		fill(0,x,y,x+105,y+47);
		color=15;
		}
		
  for(i=0;i<anzahl;i++)
    writexy_trans(color,
		  x+6+(24-len[i])/2*4, y+1+delta_y/2+delta_y*i,zeilen[i]);
  show_screen(logbase);
  logbase=oldlogbase;

  Sm();
}

FLAG word_wrap(breite,zeilen,string)
int breite;
char *zeilen[5],string[];
{
	/* Bei String handelt es sich um einen langen String, in dem keine doppelten Spaces
		 mehr vorkommen, und der auf die 5 Zeilen verteilt wird.
		 Achtung! Der String wird verändert, es werden /0 eingefügt. Die Startadressen
		 der 5 Zeilen werden in zeilen[5] zurückgegeben. Sollte eine der Zeilen überlang sein,
		 also länger als die angegebene Breite, wird FALSE zurückgegeben, andernfalls TRUE. */
		 
	int laenge;
	int startpos,endpos;
	int zeile;
	int i,j;

	laenge=strlen(string);

	startpos=endpos=0;	
	for(zeile=0;zeile<5;zeile++) {									/* max. 5 Zeilen */
		if (startpos>=laenge) zeilen[zeile]=&string[endpos];			/* Nullstring */
		else {
			zeilen[zeile]=&string[startpos];					/* Hier startet die Zeile */
			for(i=endpos=startpos,j=0;i<=laenge;j++,i++) 
				if (string[i]==' ' || string[i]==0) {
			  	if (j<=breite) endpos=i;											/* Paßt Wort noch in Zeile? */
			  	else if (startpos==endpos) endpos=i;					/* Nein, Wort zu lang? -> nehmen */
			  	}
			if (startpos==endpos) endpos=i;									/* Kein Space mehr in letzter Zeile */
			if (zeile<4) string[endpos]=0;								/* Bei letzter Zeile keine Endemarkierung */
			startpos=endpos+1;
			}
		}

	/* Überprüfen, ob es Überlängen gab: */	
	for(zeile=0;zeile<5;zeile++) 
		if (strlen(zeilen[zeile])>breite) return(FALSE);
	return(TRUE);
}

char *cdecl build(char *format,...)						/* no prototype */
{
  /* Achtung! Diese Funktion hat eine variable Anzahl von Parametern... 
  	 sie arbeitet ähnlich wie sprintf, es wird nämlich in den Formatstring die Parameter
  	 eingesetzt, und zwar kann im Formatstring die Reihenfolge geändert werden.
  	 Dies hat den Grund, damit Übersetzungen den Satzbau eines Satzes verändert können, in
  	 dem ein Objekt vorkommt.
  	 Der Kommentar 'no prototype' sagt meinem automatischen Prototypengenerator, daß er für
  	 diese Funktion keinen Prototyp gegenerieren soll. Er steht in GLOBAL.C */

	va_list argpointer;									/* Pointer auf die variablen Argumente */
  char *string[10];
  int len,len2;
  int i,j,k,z;
  int nr;
	int param_max;
			
	param_max=-1;											/* Bisher kein Parameter geholt */  
  j=0;
  len=strlen(format);
  for(i=0;i<=len;i++) {
  	build_str[j++]=format[i];
  	if (format[i]=='#') {							/* Formatkommando? */
			i++;														/* Parameter überspringen */
  		if (format[i]!='#') {						/* Noch ein '#'? */
				j--;
  			nr=format[i]-'0';							/* Nummer des Parameters bestimmen */
  			if (nr<0 || nr>9) nr=0;
  			if (nr>param_max) {						/* Soviel Parameter habe ich noch nicht geholt */
  				va_start(argpointer,format);				/* Argumentpointer rücksetzen */
  				for(k=0;k<=nr;k++) {
  					string[k]=va_arg(argpointer,char *);		/* String abholen */
  					}
  				param_max=nr;								/* Soviel habe ich jetzt geholt */
  				va_end(argpointer);						/* und rücksetzen */
					}
  			len2=strlen(string[nr]);
  			for(z=0;z<len2;z++) build_str[j++]=*(string[nr]+z);
  			}
  		}
  	}
	return(build_str);
}  			

void hide_window()
{
  /* Hintergrund des Fensters wiederherstellen */

  Hm();
	raster_replace(0,0,105,47,window_back,112,windx,windy,scr1,320);
	shrink_box(windx,windy,106,48);
  Sm();

  wind_aktiv=FALSE;         /* Fenster ist wieder weg */
}

void money_alert()
{
  /* Zu wenig Geld */
  alert(romstr007);
}

void dungeon_alert(string)
char string[];
{
	/* Da in den Dungeons manchmal die Palette schwarz ist, muß die Alertbox in der
	   vollen Helligkeit dargestelklt werden. */

	if (helligkeit>1) 	
		alert(string);				/* Alertbox in den Reagenzgläsern */
}

void alert(string)
char string[];
{
  /* Zeigt Alertbox in der Mitte des Bildschirms an, wartet auf Klick und
     löscht die Alertbox wieder weg */

	alertxy(106,90,string);
}

void alertxy(x,y,string)
int x,y;
char string[];
{
  /* Siehe Alert(), nur an Position X/Y */
  show_windxy(x,y,string);
  klick();
  hide_window();
}

void grow_box(x0,y0,breite,hoehe)
int x0,y0;
int breite,hoehe;
{
	/* Zeichnet eine sich öffnende Box */
	int old_line;
	int x1,y1,x2,y2;
	float x,y;
	
	Hm();
	
	old_line=line_mode(1);								/* EOR */	

	for(y=x=0.0;x<breite/2.0;y+=hoehe/20.0,x+=breite/20.0) {
		x1=x0+breite/2-x;
		x2=x0+breite/2+x;
		y1=y0+hoehe/2-y;
		y2=y0+hoehe/2+y;
		rahmen(7,x1,y1,x2,y2);							/* Shrinkbox zeichnen */
		wait_once(2);
    //show_screen(logbase);
		rahmen(7,x1,y1,x2,y2);							/* und wieder weg */
		}
	line_mode(old_line);
	
	Sm();
}

void shrink_box(box_x,box_y,box_breite,box_hoehe)
int box_x,box_y,box_breite,box_hoehe;
{
	int old_line;
	int x1,y1,x2,y2;
	float x,y;
	
	Hm();
	
	old_line=line_mode(1);								/* EOR */	
	for(y=x=0.0;x<box_breite/2.0;y+=box_hoehe/20.0,x+=box_breite/20.0) {
		x1=box_x+x;
		x2=box_x+box_breite-x;
		y1=box_y+y;
		y2=box_y+box_hoehe-y;
		rahmen(7,x1,y1,x2,y2);							/* Shrinkbox zeichnen */
		wait_once(2);
		rahmen(7,x1,y1,x2,y2);							/* und wieder weg */
		}
	line_mode(old_line);

	Sm();
}

void draw_shapes()
{
  /* Grundsätzliches zur hier verwendeten 2-Bildschirmtechnik:
     Zu sehen ist grundsätzlich der Screen1, aufgebaut wird auf Screen2!
     Der Mauszeiger ist demzufolge auf Screen1 zu sehen. */

  draw_mobs();                      /* Zeichnet die Mobs auf Screen2 */
  swap_screens();                   /* Screens vertauschen, Mauscursor .. */
  delete_mobs();                    /* Alte Mobs auf altem Screen löschen */
}

void draw_mobs()
{
  /* zeichnet sämtliche Sprites auf Screen2 */
  register int i;
  SPRITE_SAVE *save;

  /* copy_zeilen(scr1+(leiste_y-13)*160L,scr2+(leiste_y-13)*160L,5); */    /* Leiste übertragen */
  cpy_raster(scr1,scr2,0,leiste_y-13,319,leiste_y-9,0,leiste_y-13);

  if (scr2==scrn_1) save=save1;
  else save=save2;

  for(i=0;i<SPR_MAX;i++)
    if (sx[i]>=8 && sn[i]>=0) {       /* Sprites zeichnen+Bild retten */
      draw_shift(scr2,save,sprite_mem,i,sn[i],sx[i]-8,sy[i]-8);
      }
}

void delete_mobs()
{
  /* Löscht alle alten Mobs auf altem Screen */
  SPRITE_SAVE *save;
  register int i;

  if (scr2==scrn_1) save=save1;       /* Savebuf für Screen 1 */
  else save=save2;

  for(i=SPR_MAX-1;i>=0;i--) {          /* von hinten löschen (wegen Überschneidungen) */
    undraw_shape(save,i);
    }
}

void wait_once(anzahl)
int anzahl;
{
  /* Wartet einmalig die angegebene Anzahl an VBL's */
  
  wait_sync(0);											/* Timer rücksetzen */
  wait_sync(anzahl);								/* und solange warten */
}

long get_ms_since_progstart()
{
  struct timeval now;
  long ms;
  static time_t prog_start_time = 0;
  
  if (prog_start_time == 0) {
    prog_start_time = time(NULL);
  }

  gettimeofday(&now, NULL);
  now.tv_sec -= prog_start_time;

  ms = now.tv_sec * 1000 + now.tv_usec / 1000;
  return ms;
}

long get_vbl_ct()
{
  long ms = get_ms_since_progstart();
  return ms / 20;                          // 50 Hz
}

void wait_sync(anzahl)
int anzahl;
{
	/* Wartet bis 'anzahl' VBLs seit letztem wait_sync() vergangen sind. */
  bool shown = false;
  
#if 0
  while (sync_ct+anzahl>get_vbl_ct()) {
    show_last_screen();
    vbl_ct = get_vbl_ct();
  }
  sync_ct=vbl_ct=get_vbl_ct();									/* VBL-Counter rücksetzen */
#endif

  if (anzahl==speed) {
    Uint64 duration = SDL_GetTicks() - vbl_last_tick;
    //printf("duration: %ld\n", duration);
    performance_percent = 100*duration/20;
  }
  
  while (sync_ct+anzahl>vbl_ct) {
    if (!shown) {
      show_last_screen();
      shown = true;
    }
  }
  sync_ct=vbl_ct;
}

void wait_sync_noshow(int anzahl)
{
  while (sync_ct+anzahl>vbl_ct) {
  }
  sync_ct=vbl_ct;
}

void wait_sync_klick(anzahl)
int anzahl;
{
	/* Wartet bis 'anzahl' VBLs seit letztem wait_sync vergangen sind. Kann mit Maustaste
	   und Tastatur unterbrochen werden. */
	
  button=NOTHING;
  do {
    hol_maus();            				/* Koordinaten holen */
    if (my>169 && mk!=0) {
      button_leiste();
      mk=0;               				/* Mausklick als Kriterium aus */
      }
		wait_sync(1);
    } while (--anzahl>0 && button==NOTHING && mk==0 && !is_key());
}

void wait_sync_klick_release(anzahl)
int anzahl;
{
  wait_sync_klick(anzahl);
  do {
    hol_maus();
  } while(mk!=0);
}

void cls()
{
  register int zeile,spalte;

  Hm();
  formular(scr1,63);
  Sm();

  for(zeile=0;zeile<15;zeile++) {
    for(spalte=0;spalte<74;spalte++)
      memo[zeile][spalte]=' ';
    memo[zeile][spalte]=0;                  /* EOS */
    }

  home();
}

void home()
{
  zeile=12;
  spalte=3;
}

#ifndef COPYPROT

void print(str)
char str[];
{
  /* Schreibt String auf Bildschirm an Cursorposition */

  writexy(0,spalte*4,zeile*6,str);
  spalte+=strlen(str);
  if (spalte>80) crlf();
}

void print_long(wert)
long wert;
{
  print(str(0,wert));
}

void print_int(wert)
int wert;
{
  print(str(0,(long)wert));
}

void nprint(str)
char str[];
{
  /* Schreibt String auf Bildschirm an Cursorposition */

  Cconws(str);
}

void nprint_long(wert)
long wert;
{
  nprint(str(0,wert));
}

void nprint_int(wert)
int wert;
{
  nprint(str(0,(long)wert));
}

void nhome()
{
  Cconout(27);
  Cconout('H');
}

void ncrlf()
{
  Cconout(13);
  Cconout(10);
}
#endif

void crlf()
{
  spalte=3;
  zeile++;
  if (zeile>26) home();
}

void writexy_date()
{
  /* Schreibt nur das Datum auf den Bildschirm in leiste_oben Zeile */

  int y;

  y = leiste_y-13;

  writexy_fast(227+4, y, date);
  strcpy(c1,date);
}

void leiste_oben(grund)
char grund[];
{
  int y;
  int len;

  y=leiste_y-13;

  show_pos();                           /* Zeigt die N/O Position an */

  if(strcmp(grund,b1)!=0) {
    writexy_fast(123,y,grund);
    len=strlen(grund);
    writexy_fast(123+len*4,y,space(20-len));
    }
  if(strcmp(date,c1)) {
    writexy_fast(231,y,date);
    }

  strcpy(b1,grund);         /* abspeichern */
  strcpy(c1,date);
}

void init_oben()
{
  /* Initialisiert die obere Leiste */
  a1[0]=b1[0]=c1[0]=0;      /* nächsten Print ausführen */
}

void show_pos()
{
  /* Zeigt die Position an, an der Spieler sich befindet */
  /* in Integerarithmetik, damit schneller... */

  writexy_fast(8,leiste_y-13,calc_pos(pos_welt,sx[0],sy[0]));
}

char *calc_pos(pos,x,y)
int pos;                /* Position in der Weltkarte */
int x,y;                /* Position in Pixel */
{
  /* Berechnet die Position dieses Punktes, und stellt einen string zusammen */
  /* Gibt die Adresse des Strings zurück */
  register int grad,minuten;
  register long abstand;
  register int pixeln_s;

  pixeln_s=pos/15*151+(y-17)-1132;
/*  pixeln_s-=1132;             */              /* In der Mitte ist der Ursprung */
  abstand= pixeln_s<0 ? -pixeln_s : pixeln_s;
  abstand*=100;
  grad=abstand/1258;
  minuten=(abstand%1258*477)/10000L;

  pos_string[0]=grad/10+'0';
  pos_string[1]=grad%10+'0';
  pos_string[4]=minuten/10+'0';
  pos_string[5]=minuten%10+'0';
  pos_string[8]= pixeln_s<0 ? 'N' : 'S';

  pixeln_s=pos%15*320+x-2400;
  abstand= pixeln_s<0 ? -pixeln_s : pixeln_s;
  abstand*=100;
  grad=abstand/1333;
  minuten=(abstand%1333*450)/10000L;

  if (grad>=100) {
    pos_string[10]='1';
    grad-=100;
    }
  else pos_string[10]=' ';

  pos_string[11]=grad/10+'0';
  pos_string[12]=grad%10+'0';
  pos_string[15]=minuten/10+'0';
  pos_string[16]=minuten%10+'0';
#ifdef ENGLISCH
  pos_string[19]= pixeln_s<0 ? 'W' : 'E';
#endif
#ifdef DEUTSCH
  pos_string[19]= pixeln_s<0 ? 'W' : 'O';
#endif
#ifdef FRANZ
  pos_string[19]= pixeln_s<0 ? 'O' : 'E';
#endif

  return(pos_string);
}

void klick()
{
	/* Wartet auf Tastendruck oder Mausklick */
	
  while (!is_key() && mousek!=0) show_last_screen();
  while (!is_key() && mousek==0) show_last_screen();
  while (!is_key() && mousek!=0) show_last_screen();
	if (is_key()) get_key();				/* Taste wegwerfen */
}

FLAG maus_in(x1,y1,x2,y2)
int x1,y1,x2,y2;
{
  /* Prüft ob Mauszeiger in angegebenen Bereich geklickt hat */

  return (mk==1 && mx>=x1 && mx<=x2 && my>=y1 && my<=y2);
}

void wait_klick()
{
/* wartet auf Mausklick und checkt auf hleiste und bleiste */

  button=NOTHING;
  do {
    hol_maus();            /* Koordinaten holen */
    //printf("wait_klick: %d %d %d\n", mx, my, mk);
    if (my>169 && mk!=0) {
      button_leiste();
      mk=0;               /* Mausklick als Kriterium aus */
      }
    //printf("wait_klick: %ld %d\n", button, mk);
    show_last_screen();
    } while (button==NOTHING && mk==0);
}

void button_leiste()
{
  int i;
  long nr;

  button = NOTHING;         /* Nichts wurde angewählt */

  //puts("button_leiste");

  do {
    hol_maus();

    if (mx>96 && mx<223 && my>169) {
      do_board();
      }
    else {
      if (mk==1) {
        for(i=0;i<76;i+=4) {
          nr=menu_nr[i/4];
          if (mx>=menu_koord[i] && mx<=menu_koord[i+2] &&
              my>=menu_koord[i+1] && my<=menu_koord[i+3] &&
              (bleiste & nr)!=0) {
                  button=nr;
                  selekt(i);           /* Kurz aufblitzen lassen */
				          if (button==BUY && schulden) schuldner();
									if (button==PAUSE) {
										wait_mouse(0);
										wait_once(20);
										pause_game();
										}
                  }
          }
        wait_mouse(0);
        }
      }
    } while(my>169 && mk!=0 && button == NOTHING);

  if (button!=NOTHING) mk=0;    /* Wenn was angeklickt wurde, dann kein Mausknopf */
}

void pause_game()
{
  /* Spieler hat in der unteren Leiste den Mauszeiger länger nicht bewegt */
  int mx2,my2;

  fade_out();                               /* Bildschirm ausblenden */

  mx2=mousex;
  my2=mousey;

  while(mousex==mx2 && mousey==my2 && !is_key()) ;        /* Warten bis Mausbewegt oder Taste */

  fade_in();                                   /* Bildschirm wieder an */
  mk=0;                         /* Kein Knopf gedrückt */
}

void schuldner()
{
  /* Spieler hat seit 3 Monaten Schulden auf der Bank, jetzt kann er nichts
     mehr kaufen */

  alert(romstr008);
  button=NOTHING;
}

void selekt(i)
int i;                  /* Offset des Eintrags in menu_koord[] */
{
  /* Button kurz aufblitzen lassen */

  Hm();
  change_pixel(i,6,0);                  /* Farbe 6 nach 0 wandeln */
  wait_once(3);													/* 3 VBLs warten */
  wait_mouse(0);
  change_pixel(i,0,6);                  /* und von 0 nach 6 wandeln */
  Sm();
}

void change_pixel(i,alt,neu)
int i;
int alt,neu;
{
  int x,y;

  for(x=menu_koord[i];x<=menu_koord[i+2];x++)
    for(y=menu_koord[i+1];y<=menu_koord[i+3];y++)
      if (get_pixel(x,y)==alt) plot_pixel(neu,x,y);
}

void prep_date()
{
  dlstr(heute,date);
}

long zufall_long(grenze)
long grenze;
{
  /* Zwischen 0..(grenze-1) */
  long zahl;

  zahl=((long)rand()<<15)+rand();					/* entsprechend große Zufallszahl */
  return(zahl%grenze);
}

int zufall(grenze)
int grenze;
{
  /* Zwischen 0..(grenze-1) */

	if (grenze==0) return(0);
  return(rand()%grenze);
}

int input(x,y,len,eingabe)
int x,y,len;
char eingabe[];
{
  int pos=0;
  char zeichen[3];
  long taste;
  void *oldlogbase;

  oldlogbase=logbase;
  //logbase=&x_window;
  logbase=scr1;

  Hm();

  while (get_key()!=-1) ;          /* Tastaturpuffer löschen */

  zeichen[1]='@';           /* Cursor */
  zeichen[2]=0;
  writexy(0,x,y,&zeichen[1]);         /* Cursor zeigen */

  do {
    show_last_screen();
    taste=wait_key();
    zeichen[0]=(unsigned char)taste;

    if (taste==8 || taste==127) {
      if (pos>0) {
        writexy(0,x+pos*4,y,romstr009);        /* Cursor weg */
        pos--;
        writexy(0,x+pos*4,y,&zeichen[1]);     /* Cursor hin */
        }
      }
    else
      if (pos<len && zeichen[0]!=13) {
        if (islower((int)taste)) taste=toupper(taste);
#if 0
				switch ((int)taste) {
					case 'ü': taste='Ü'; break;
					case 'ö': taste='Ö'; break;
					case 'ä': taste='Ä'; break;
					}
#endif
        if (isalnum((int)taste) || (char)taste==' ' /* || (char)taste=='Ü' || (char)taste=='Ö' || (char)taste=='Ä' */) {
          zeichen[0]=(char)taste;
          writexy(0,x+pos*4,y,zeichen);
          eingabe[pos++]=zeichen[0];
          }
        }
    } while(zeichen[0]!=13);
  eingabe[pos]=0;

  writexy(0,x+pos*4,y,romstr010);          /* Cursor weg */
  Sm();

  logbase=oldlogbase;

  return(pos);
}

void rec(x1,y1,x2,y2)
int x1,y1,x2,y2;
{
  /* Zeichnet einen Rahmen und wartet bis Mausknopf losgelassen wird */

  Hm();
  rahmen(0,x1,y1,x2,y2);
  wait_mouse(0);
  Sm();
}

void drec(x1,y1,x2,y2)
int x1,y1,x2,y2;
{
  /* Löscht einen Rahmen auf dem Formularuntergrund und
     wartet bis Mausknopf losgelassen wird */

  Hm();
  rahmen(7,x1,y1,x2,y2);
  wait_mouse(0);
  Sm();
}

void Krec(x1,y1,x2,y2)
int x1,y1,x2,y2;
{
  /* Zeichnet einen Rahmen, wartet bis Maus losgelassen wird, und löscht
     den Rahmen wieder */

  rec(x1,y1,x2,y2);
  drec(x1,y1,x2,y2);
}

void wait_mouse(a)
int a;
{
  while(mousek != a) ask_X();
}

char *fstr(char *a,float b)
{
 sprintf(zahl_str,a,b);
 return(zahl_str);
}

long runde(float wert)
{
  /* Rundet den Floatwert */
  long erg;

  erg=wert;
  if ((wert-erg)>=0.5) erg++;
  return(erg);
}

char *str(len,zahl)
int len;
long zahl;
{
  /* wandelt zahl in string und füllt auf die Länge von len Zeichen mit
     Spaces auf. */
  register int pos;
  register long long_wert;
  register unsigned int int_wert;
  FLAG negativ;

  if (zahl<0) {                 /* negativ? */
    negativ=TRUE;
    zahl=-zahl;                 /* Positiv machen */
    }
  else negativ=FALSE;

  long_wert=zahl;

  zahl_str[79]=0;               /* Ende des Strings */
  pos=79;

  if (long_wert==0)
    zahl_str[--pos]='0';
  else {
    while(long_wert>65535L) {           /* langsame Longroutine */
      zahl_str[--pos]=long_wert%10+'0';
      long_wert/=10;
      }
    int_wert=long_wert;
    while(int_wert>0) {            /* schnelle Integer-Routine */
      zahl_str[--pos]=int_wert%10+'0';
      int_wert/=10;
      }
    }

  if (negativ) zahl_str[--pos]='-';             /* Minus davor */

  while(79-pos<len) zahl_str[--pos]=' ';

  return(&zahl_str[pos]);
}

void dlstr(datum,string)
long datum;
char string[];
{
  /* wandelt Datum in ausführlichen Klartext z.B. January, 1st 1432 */
  int month,year,day;
  int i;
  
#ifdef ENGLISCH
	int len;
#endif  

  year=datum/360;
  month=(datum%360)/30+1;
  day=datum-(year*360)-((month-1)*30)+1;

#ifdef ENGLISCH
  strcpy(string,month_name[month]);
  strapp(string,romstr011);
  strapp(string,str(2,(long)day));
  len=strlen(string);

  string[len]='T'; string[len+1]='H';

  if (day>19 || day<10)
    switch (day % 10) {
      case 1: string[len]='S'; string[len+1]='T'; break;
      case 2: string[len]='N'; string[len+1]='D'; break;
      case 3: string[len]='R'; string[len+1]='D'; break;
      }
  string[len+2]=' '; string[len+3]=0;

  strapp(string,str(0,(long)year));
#else
  strcpy(string,str(2,(long)day));
  strapp(string,romstr012);
  strapp(string,month_name[month]);
  strapp(string,romstr013);
  strapp(string,str(0,(long)year));
#endif

  for(i=strlen(string);i<20;i++) string[i]=' ';
  string[i]=0;
}

char *dat_kurz(datum)
long datum;
{
  /* Wandelt Datum in Kurzstring z.B. 28/2 */
  long tag,monat,jahr;

  jahr=datum/360;
  monat=(datum%360)/30+1;
  tag=datum-(jahr*360)-((monat-1)*30)+1;

  strcpy(dat_str,str(2,tag));
  strapp(dat_str,romstr014);
  strapp(dat_str,str(0,monat));
  return(dat_str);
}

char *dstr(datum)
long datum;
{
    /* wandelt DATUM in String z.b: 1.1.1432 */

  long tag,monat,jahr;
  int i;

  jahr=datum/360;
  monat=(datum%360)/30+1;
  tag=datum-(jahr*360)-((monat-1)*30)+1;

  for(i=0;i<20;i++) dat_str[i]=0;

  strcpy(dat_str,str(2,tag));
  dat_str[strlen(dat_str)]='.';
  strcpy(&dat_str[strlen(dat_str)],str(2,monat));
  dat_str[strlen(dat_str)]='.';
  strcpy(&dat_str[strlen(dat_str)],str(4,jahr));
  return(dat_str);
}

char *space(len)
int len;
{
  zahl_str[len--]=0;
  if (len>=0) for (;len>=0;len--) zahl_str[len]=' ';
  return(zahl_str);
}

int file_exists(char *filename) {
  char file[200];
  int fd;
  
  create_data_filename(filename, file);
  if ((fd=open(file,O_RDONLY))<0)
    return FALSE;
  close(fd);
  return TRUE;
}

int file_exists_internal(char *file) {
  int fd;
  if ((fd=open(file,O_RDONLY))<0)
    return FALSE;
  close(fd);
  return TRUE;
}

void create_data_filename(char *filename_in, char *filename_out)
{
  strcpy(filename_out,"../../data/");
  strcat(filename_out,filename_in);
  if (file_exists_internal(filename_out)) 
    return;
  strcpy(filename_out,"data/");
  strcat(filename_out,filename_in);
  if (file_exists_internal(filename_out)) 
    return;
  strcpy(filename_out,"");
  strcat(filename_out,filename_in);
}

size_t file_size(char *filename) {
  char file[200];
  int fd;
  
  create_data_filename(filename, file);
  if ((fd=open(file,O_RDONLY|O_BINARY))<0)
    { perror(file); abort(); }
  off_t pos = lseek(fd, (off_t)0, SEEK_END);
  close(fd);
  return pos;
}

int load(filename,adr,offset,laenge)
char *filename;
void *adr;
long offset;
long laenge;
{
  /* Lädt File, entweder von der Diskette/Festplatte oder aus der
     Ramdisk und gibt die Anzahl der gelesenen Bytes zur"uck. */
  int fd;
  char file[200];
  int len;
  off_t off = (off_t)offset;
  size_t size;

  size = file_size(filename);

  if (offset + laenge > size) {
    laenge = size - offset;
  }
  
#if 0
  printf("loading: \"%s\", size=%zu, offset=%ld, len=%ld\n",filename, size, offset, laenge);
#endif
  create_data_filename(filename, file);
  if ((fd=open(file,O_RDONLY|O_BINARY))<0)
    { perror(file); abort(); }
  if (offset!=0) {
    off_t pos = lseek(fd, off, SEEK_SET);
    if (pos<0) 
      { printf("pos=%ld\n", pos);
	perror(file); abort(); }
  }
  
  len=read(fd,adr,(size_t)laenge);
  if (len<0) 
    {printf("len=%d\n", len);
      perror(file); abort(); }
#if 0
  printf("loading: \"%s\", read=%d\n", filename, len);
  od(adr);
#endif
  close(fd);

  return len;
}

void od(void *adr)
{
  unsigned char *p = adr;
  for (int i = 0; i < 16; i++) {
	  printf("%02x ", (int)(*(p+i)));
    }
  for (int i = 0; i < 16; i++) {
    char c = *(p+i);
    if (!isprint(c)) c = '?';
    printf("%c ", c);
    }
  printf("\n");
}


int load_bibliothek(filename,buffer)
char *filename;                
void *buffer;
{
  /* Lädt eine Datei aus der Bibliothek und entpackt sie mit dem ICE!-Packer, falls sie
     gepackt ist. Gibt die Länge der ungepackten Datei zurück */
  int laenge;
	char *ziel;
	
  laenge=load(filename,buffer,0L,0xdfdfdfL);       /* File von Disk laden */
  	ziel=buffer;
		if (ziel[0]=='I' && ziel[1]=='C' && ziel[2]=='E' && ziel[3]=='!') {
    void *tmp = malloc(100000);
    laenge=ice_unpack(buffer, tmp);
    memcpy(buffer, tmp, laenge);
    free(tmp);
			}
		if (*ziel++=='C' && *ziel++=='r' && *ziel++=='w' && *ziel=='!')
			internal(romstr015);

	return(laenge);
}

void Sm()
{
  /* Schaltet die Maus an */

  maus_zaehler--;
  if (maus_zaehler<0) maus_zaehler=0;

  if (maus_zaehler==0) {
    draw_mouse(scr1,mousex,mousey);      /* MAus anschalten */
    ms_on=TRUE;                   /* Maus wieder bewegen */
    }
}

void Hm()
{
  /* Schaltet die Maus ab */

  if (maus_zaehler==0) {
    ms_on=FALSE;               /* VBL-Maus anhalten */
    undraw_mouse(scr1);               /* Maus weglöschen */
    }

  maus_zaehler++;               /* Sooft abgeschaltet */
}

void initspr4map()
{
  /* Bereitet Spriteverwaltung auf Landschaft vor */

  switch_sprites();                     /* Sprites an- oder abschalten */
  sprite_init();                        /* Sprites zeichnen etc. */
}

void sprite_init()
{
  /* zeichnet alle Sprites auf scr1, und initialisiert die Sprite-
     verwaltung */

  int i;
  SPRITE_SAVE *save;

  for(i=0;i<SPR_MAX;i++) {
    save1[i].screen=0L;                   /* Alle Spritehintergrundpointer ungültig */
    save2[i].screen=0L;
    }

  if (scr1==scrn_1) save=save1;
  else save=save2;

  Hm();

  copy_screen(scr1,scr2);
  /* copy_zeilen(scr1,hlpbuf,169);  */          /* Hintergrund -> hlpbuf */
  cpy_raster(scr1,hlpbuf,0,0,319,168,0,0);

  for(i=0;i<SPR_MAX;i++)
    if(sx[i]!=-1 && sn[i]>=0) {
      draw_shift(scr1,save,sprite_mem,i,sn[i],sx[i]-8,sy[i]-8);
      }

  Sm();
}

void formular(screen,y)
void *screen;               /* Screen auf den das Formular kommt */
int y;											/* Startzeile y */
{
  /* Stellt ein leeres Formular dar */

  Hm();
	set_raster(0,y,formularp);
  draw_obj(0,formular_adr,ODER,screen,0,y);
  Sm();
}

void formular_big(screen, y1, y2)
void *screen;
int y1,y2;
{
  formular(screen, y1);
  draw_obj_part(0,formular_adr,
		0, 30,
		objekt_breite(0, formular_adr)-1, objekt_hoehe(0, formular_adr)-1,
		MOVE, logbase, 0, y2 - objekt_hoehe(0, formular_adr) + 30);
}

char *strapp(str1,str2)
char str1[],str2[];
{
  /* Hängt String2 am String1 an */

  strcpy(&str1[strlen(str1)],str2);
  return(str1);
}

void clear_screen(adr)
void *adr;
{
  SDL_Surface *surface;
  int x, y;
	
  surface = adr;

  Uint8* pixels = (Uint8*)surface->pixels;
  for (y = 0; y < surface->h; y++)
    for (x = 0; x < surface->w; x++)
      pixels[y * surface->pitch + x] = 0;

#if 0
  XSetForeground(x_display,x_gc,BlackPixel(x_display,x_screen_num));
  XFillRectangle(x_display,*(Pixmap *)adr,x_gc,0,0,640,400);
#endif
}

void copy_screen(quelle,ziel)
void *quelle,*ziel;
{
  /* Kopiert aktuellen Screen auf Zielscreen */

#ifdef unix
  cpy_raster(quelle,ziel,0,0,319,199,0,0);      
#else
	Hm();
	memcpy(ziel,quelle,32000L);										/* Ein Bildschirm ist 32000 Bytes groß */
	Sm();
#endif
}

void vbl_routine()
{
  if (ms_on) {
    undraw_mouse(scr1);                   /* Maus auf Screen1 löschen */
    draw_mouse(scr1,mousex,mousey);    		/* Auf Screen1 zeichnen */
    }
  if (unlim_money_cheat) money=1000000L;         /* Immer 1000000 */
}

void swap_screens()
{
  /* Diese Routine vertauscht die beiden Screens (scr1,scr2) und schaltet
     den Mauscursor auf den neuen Screen um */
  void *tmp;
	FLAG old_ms_on;
	
  if (ms_on) 
  	draw_mouse(scr2,mousex,mousey);        /* Maus auf 2. Bildschirm anschalten */

	old_ms_on=ms_on;

#if 0
  copy_screen(scr1,&x_swapscreen);
  copy_screen(scr2,scr1);
  copy_screen(&x_swapscreen,scr2);
#endif
	
  //copy_screen(scr1,&x_window);
  tmp=scr1; scr1=scr2; scr2=tmp; 
  //XFlush(x_display);
			
  show_screen(scr1);               /* Screen1 darstellen (war vorher scr2)  */

  ms_on=FALSE;                    /* Jetzt keine VBL Maus */
  ms_on=old_ms_on;            /* VBL-Maus ggf. wieder an */

#if 1
  if (speed!=0) wait_sync(speed);                      /* Wartet auf 2. VBL */
#endif

  if (ms_on) undraw_mouse(scr2);             /* Alte Maus auf Screen1 löschen */

#if 1
	logbase=scr1;
#endif
}

void switch_screens()
{
  /* Diese Routine stellt jetzt Screen2 dar, vertauscht die Variablen
     und macht einen Vsync */

  void *tmp;

  tmp=scr1;
  scr1=scr2;
  scr2=tmp;

	show_screen(scr1);
}

void init_maus()
{
  /* Initialsiert die Mausroutinen, d.h. die eigene Mausroutine wird ins
     System eingehängt */

  ms_on=FALSE;                    /* VBL Maus aus */
  set_mouse(0);                    /* Kreuz ist angesagt */
  maus_zaehler=0;                   /* Der 1. Abschalttiefe=1 */
  mousex=160;
  mousey=155;                       /* Startwerte der Maus */
  mousek=0;

  save_1[0]=0;
  save_2[0]=0;                      /* Buffer sind ungültig */
  scrn_1=scr1;                      /* für Vergleich merken */

  eintrag=init_vbl(vbl_routine);            /* in VBL-Queue eintragen */

  ms_on=TRUE;                 /* Maus ist zu sehen ! */

  Sm();                         /* Maus darstellen */
}


