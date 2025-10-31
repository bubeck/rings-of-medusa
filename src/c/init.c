          	            
/* Dieser Part regelt die grundsätzlichen Installationen von Crown, sowie
   das entsprechende Abmelden, z.B. des Speichers etc... */

#include <assert.h>
#include "includes.c"           /* Definiert alle Variablen als Extern,... */

int file_anzahl;            /* für Show-Dir */
int soundnr;              /* Für IRQ-Musik */

int schoner_x=9999,schoner_y;        /* Für Bildschirmschoner */

//char *music_end;                /* Hier endet die Musik in den Buffern */

/* Start: */

void vari_init()
{
  /* Initialisiert die globalen Variablen von Crown */
  int i;
  register int zeile,spalte;

	my_system=FALSE;

#ifndef COPYPROT
	if (tos_da) {									/* Handelt es sich um meine Festplatte? */
		int fh;

		fh=Fopen(romstr017,0);
		if (fh>0) {
			my_system=TRUE;
			Fclose(fh);
			}
		}

	if (tos_da && my_system)
  	if (Dgetdrv()>1) {							/* Wurde von Harddisk gebootet? */
	    Dsetdrv('D'-'A');
  	  Dsetpath(romstr018);         /* gestartet (beim Entwickeln) */
    	}

#ifdef AMIGA
	my_system=TRUE;
#endif

#endif

  ns=romstr019;                            /* Pointer auf einen Nullstring */

  maus_sendung=TRUE;                    /* Maus im VBL erfragen */
  wind_aktiv=FALSE;                  /* Im Moment ist kein Window da */
  treffer_cheat=door_cheat=cheat_on=FALSE;                       /* Keine Cheats da */
  rohstoff_cheat=unlim_money_cheat=citycheat=FALSE;            /* Cheat Mode aus */
  produce_cheat=zeit_cheat=kaserne_cheat=treasure_cheat=FALSE;
	schuss_cheat=strength_cheat=pio_cheat=staerke_cheat=FALSE;
  effects = TRUE;
  zeit_cheat = FALSE;

	alerts_da=FALSE;							/* Noch sind keine Alertboxen da! */
  speed=2;                      /* 2 Vsyncs zwischen Bildwechsel */

  bstate=MAUS_DA;                    /* Maus ist zu sehen */
  verlassen=FALSE;                  /* User will Programm nicht verlassen */
  msminy=17;

  for(i=0;i<SPR_MAX;i++) sx[i]=-1;      /* Alle Sprites freigeben */
  for(i=0;i<GEGNER;i++) {
    steuer[i].sprite=-1;
    steuer[i].heimat=-1;         /* Armee nicht benutzt */
    }

  for(zeile=0;zeile<15;zeile++) {               /* Memo löschen */
    for(spalte=0;spalte<74;spalte++)
      memo[zeile][spalte]=' ';
    memo[zeile][spalte]=0;                  /* EOS */
    }

  home();

#ifndef COPYPROT
  if (my_system) cheat_on=TRUE;          									/* Cheats sind bei Festplatte immer da */
#endif

  pos_string[2]='[';                         /* Grad Zeichen */
  pos_string[3]=' ';
  pos_string[6]=39;                          /* apostroph */
  pos_string[7]=' ';
  pos_string[9]=' ';
  pos_string[13]='[';                       /* Vorbereiten für calc_pos */
  pos_string[14]=' ';
  pos_string[17]=39;
  pos_string[18]=' ';
  pos_string[20]=0;

	level=1.0;										/* Spiellevel */
	
	exit_anzahl=99;								/* Beim nächsten Exit bitte ein Passwort */
}

void load_pics()
{
  load_leisten();											/* Lädt die Leisten in leisten[] */
  load_objekte("formular.obj",formular_adr);  /* Formularobjekt laden */
}

void load_alerts()
{
	/* Lädt die Alertboxen */
	long laenge;
	
	clear_screen(scr1);
	set_raster(0,0,kartep);
	show_raster();
	
  laenge=load_objekte("alert.obj",wind_form); 
	
#ifndef unix
	if (laenge>sizeof(wind_form)) {
		internal(build(romstr021,str(0,(long)sizeof(wind_form))));
		}
#endif

  alerts_da=TRUE;														/* Jetzt sind die Alertboxen geladen */
}

void hol_maus()
{
  /* Holt die Mauskoordinaten */
	static long counter;

  ask_X();

  mx=mousex;
  my=mousey;            /* Holt sich die aktuellen Mauswerte aus den IRQ */
  mk=mousek;          	/* Variablen, und übergibt sie dem Aufrufer */

  //printf("maus %d %d %d\n", mx, my, mk);
  
	if (mx==schoner_x && my==schoner_y && !is_key()) {						/* Maus nicht bewegt? */
		if (vbl_ct>counter) {
			pause_game();
			counter=vbl_ct+6000L;									/* 2 min geben */
			}
		}
	else {
		schoner_x=mx;
		schoner_y=my;
		counter=vbl_ct+6000L;
		}

 #ifndef COPYPROT
  if (FALSE && mk==2 && my_system) {          /* rechter Knopf gedrückt */
    fill(0,0,0,100,12);
    writexy(15,0,0,romstr022);
    writexy(15,8,0,str(3,(long)mx));
    writexy(15,20,0,romstr023);
    writexy(15,32,0,str(3,(long)my));
    writexy(15,0,6,romstr024);           /* Bei rechtem Mausknopf X/Y ausgeben */
    writexy(15,36,6,str(3,(long)(mx-mx%4)));
    writexy(15,48,6,romstr025);
    writexy(15,88,6,str(3,(long)(my-my%6)));
#if 0
    mk=2;                               /* Keine Taste gedrückt */ 
#endif
    }
  if ((my==msminy || my==0) && mk==3 && my_system) programmende();

	if (my_system && keypress>>16==59)			/* F1 */
    save_pic();                         /* Momentanes Picture speichern */
#endif
 
}

#ifndef COPYPROT

void konvert(scr)
unsigned int *scr;
{
	/* Packt ein Bild Pixelorientiert, d.h. alle 4 Bits eines Pixels stehen nebeneinander */
	register int k;
	register unsigned int wert;
	register int i;
	unsigned int plane[4];
	int j;
	
	j=0x1234;
	
	for(j=0;j<4000;j++) {
		plane[0]=*scr;
		plane[1]=*(scr+1);
		plane[2]=*(scr+2);				/* Screen holen */
		plane[3]=*(scr+3);
		for(i=0;i<4;i++) {
			wert=0;
			for(k=0;k<4;k++) {
				wert*=2;
				if (plane[0]&32768) wert++;
				plane[0]*=2;
				wert*=2;
				if (plane[1]&32768) wert++;
				plane[1]*=2;
				wert*=2;
				if (plane[2]&32768) wert++;
				plane[2]*=2;
				wert*=2;
				if (plane[3]&32768) wert++;
				plane[3]*=2;
				}
			*scr++=wert;
			}
		}
}

void konvert_back(scr)
unsigned int *scr;
{
	/* Packt ein Bild Pixelorientiert, d.h. alle 4 Bits eines Pixels stehen nebeneinander */
	register int k;
	register unsigned int wert;
	register int i;
	unsigned int plane[4];
	int j;
	
	j=0x1234;
	
	plane[0]=plane[1]=plane[2]=plane[3]=0;
	
	for(j=0;j<4000;j++) {
		for(i=0;i<4;i++) {
			wert=*scr++;
			for(k=0;k<4;k++) {
				plane[0]<<=1;
				plane[1]<<=1;
				plane[2]<<=1;
				plane[3]<<=1;
				if (wert&32768) plane[0]++;
				if (wert&16384) plane[1]++;
				if (wert&8192) plane[2]++;
				if (wert&4096) plane[3]++;
				wert<<=4;
				}
			}
		*(scr-4)=plane[0];
		*(scr-3)=plane[1];
		*(scr-2)=plane[2];
		*(scr-1)=plane[3];
		}
}

void save_pic()
{
  /* SPieler hat rechte Shifttaste gedrückt, und kann jetzt eine Diashow machen */
  long taste;
  char filename[40];
  int fh;

  strcpy(filename,romstr026);

  blitz(0x777);

	do {
	  taste=wait_key();
		} while(!isprint((char)taste));
		
#if 0
  if (islower((int)taste)) toupper(taste);
  filename[12]=(char)taste;
  filename[13]=0;
  fh=Fcreate(filename,0);
  if (fh>=0) {
    Fwrite(fh,50*34L,hbl_system);           /* HBL-Offsets */
		Hm();
		konvert(scr1);
    Fwrite(fh,32000L,scr1);           		/* Bild speichern */
		konvert_back(scr1);
		Sm();
    Fclose(fh);
    }
#endif
}


#endif

void swap(a,b)
int *a,*b;
{
  /* vertausch a mit b */
  int tmp;

  tmp=*a;
  *a=*b;
  *b=tmp;
}

void alloc_mem()
{
  /* holt den benötigten Speicher */
  char *poi_wert;

  formular_adr=malloc(20000);

#ifndef COPYPROT
#if 0
	if (tos_da)
		oldbase=Logbase();															/* Alte Bildschirmbasis holen */
#endif
#endif
  
  poi_wert=memory;                              /* Zeiger auf Speicherbereich */

#ifndef unix
  scr1=logbase=(char *)((poi_wert/256L+1)*256L);           /* auf Pageanfang bringen */
	scr2=scr1+32000;
  hlpbuf=memory+32000;
#endif
  pack_buf=memory+32000;                          /* Pack_Buf ist 64K groß! */

  // trf_buf=pack_buf+32000;           /* Hier kommt der Traffic hin */
  trf_buf = malloc(32000);
}

void show_free()
{
  /* Zeigt den verbleibenden, freien Speicher an */
	char ver1[2],ver2[2],ver3[2];
	
  if (mousex==0 || my_system) {
    writexy_fast(0,0,build(romstr027,str(0,mem_len)));
		ver1[0]=v1+'0';
		ver2[0]=v2+'0';
		ver3[0]=v3+'0';
		ver1[1]=ver2[1]=ver3[1]=0;
		writexy_fast(0,6,build(romstr028,ver1,ver2,ver3));
#ifndef unix
		track_display=TRUE;
#endif
    }
}

void load_music()
{
  /* Lädt die Lademusik und startet Sie */
  long laenge=0;
	
#ifdef ATARI
  clear_screen(scr2);
	laenge=load_bibliothek(LADEMUSI_IMG,scr2);
	play_music(scr2,1);					/* Diese Funktion wird als Musik angesprungen */
#elsifdef AMIGA
  clear_screen(scr2);
	laenge=load_bibliothek(LADE_AMI_IMG,scr2);
	play_music(scr2,1);
#else
  play_music("lademusi.wav", 1);
#endif

  //musik_an=TRUE;                  /* und spielen bitte */
  //musik_an=FALSE;
  //music_end=scr2+laenge;          /* Hier endet die Musik */
}

void print_ascii_title(void)
{
  char buffer[2000];
  int len;

  len=load("medasc.txt",buffer,0,sizeof(buffer));
  assert(len!=sizeof(buffer));
  buffer[len]=0;
  puts(buffer);
}

void init_medusa()
{
  /* first of all print out ascii medusa title: */
  print_ascii_title();
		
  /* initialisiert alle Dateistrukturen und Speicherstrukturen von Crown */
	alloc_mem();							/* Verteilt den verfügbaren Speicher */
  vari_init();                  /* Initialisiert die globalen Variablen */
  init_atari();                 /* Initialisiert das Atari System */

	hol_maus();									/* Mausposition erfragen */
	
	init_archiv();								/* Archiv initialisieren */

  load_music();                 /* Lademusik an */

	load_pics();                  /* Alle benötigten Files laden */
	intro();										/* Diashow anzeigen */

#ifdef AMIGA
	intro_off();								/* Beim Amiga gleich abschalten! */
#endif
  show_free();                  /* Zeigt den freien Speicher an */

  fill_ramdisk();            			/* Ramdisk-system auffüllen */

#ifndef AMIGA	
	intro_off();								/* Und Intro abschalten */
#endif

  load_alerts();							/* Und die Alertboxen laden */

  play_music("gamemusic.wav",1);      /* Musik abspielen */
  music=MUSIK_AN;                          /* Musik spielt */
}

char *such_string(nr,adr)
int nr;
char *adr;
{
	/* Sucht in einem Feld aus Strings des String nr und liefert dessen Adresse */
	
	while(nr>1) {
		nr--;
		while((*adr++)!=0) ;
		}
	return(adr);

}

void ice_deinterleave(uint16_t *A0) {
    int D7, D6, D5;
    uint16_t D0, D1, D2, D3, D4;
    
    for (D7 = 3999; D7 >= 0; D7--) {
        for (D6 = 3; D6 >= 0; D6--) {
            // Wert holen
            D4 = *A0++;
            
            for (D5 = 3; D5 >= 0; D5--) {
                // Schiebe D4 nach links, trage Carry in D0-D3
                uint16_t carry;
                
                // Erste Verschiebung
                carry = (D4 & 0x8000) ? 1 : 0;
                D4 <<= 1;
                D0 = (D0 << 1) | carry;
                
                // Zweite Verschiebung
                carry = (D4 & 0x8000) ? 1 : 0;
                D4 <<= 1;
                D1 = (D1 << 1) | carry;
                
                // Dritte Verschiebung
                carry = (D4 & 0x8000) ? 1 : 0;
                D4 <<= 1;
                D2 = (D2 << 1) | carry;
                
                // Vierte Verschiebung
                carry = (D4 & 0x8000) ? 1 : 0;
                D4 <<= 1;
                D3 = (D3 << 1) | carry;
            }
        }
        
        // movem.w D0-D3,-8(A0)
        // Schreibe die 4 Words zurück (8 Bytes vor aktuellem A0)
        A0[-4] = D0;
        A0[-3] = D1;
        A0[-2] = D2;
        A0[-1] = D3;
    }
}

void show_demo_pic(char *filename)
{
  uint8_t *mem = malloc(file_size(filename));

  load_bibliothek(filename, mem);
#if 1
  uint16_t *raster = (uint16_t *)mem;
  for(int i = 0; i < 50 * 17; i++) {    // 50 raster, each Y+Palette
    be_2(raster);
    raster++;
  }
#endif

  //  fade_in();
  clear_raster();
  clear_screen(scr1);

  for(int i = 0; i < 50; i++) {
    uint16_t *raster;
    raster = (uint16_t *)(mem + i * 34);
    //be_2(raster);
    set_raster(0, *raster, raster + 1);
    if (*(raster+17) == 0) break;
  }

  sort_rasters();
  print_rasters();
  
  uint8_t *pixel_data = mem + 50 * 34;
  //ice_deinterleave((uint16_t *)pixel_data);
  



  
  int src_idx = 0;
  for (int y = 0; y < 200; y++) {
    for (int x = 0; x < 320; x += 2) {
      uint8_t byte = pixel_data[src_idx++];
            
      // Erstes Pixel (obere 4 Bits)
      uint8_t color_idx1 = (byte >> 4) & 0x0F;
      set_pixel_surface(logbase, x, y, color_idx1);
            
      // Zweites Pixel (untere 4 Bits)
      uint8_t color_idx2 = byte & 0x0F;
      set_pixel_surface(logbase, x + 1, y, color_idx2);
    }
  }
  show_screen(logbase);
  show_raster();
  fade_in();
  
  delay(5000);

  //fade_out();

  
  free(mem);
}

void do_demo()
{
  fade_out();

  clear_raster();
  clear_screen(scr1);

  set_raster(0,0,leistep);
  logbase=scr1;
  clear_screen(logbase);

  for (char pic = 'a'; pic <= 'j'; pic++) {
    char filename[100];

    sprintf(filename, "demo/medusa.pi%c", pic);
    if (file_exists(filename))
      show_demo_pic(filename);
  }
}

void re_initialize()
{
  /* Diese Funktion führt alle Installationen durch, die beim ersten Starten
     bzw. jedem weiteren Starten von Crown durchgeführt werden müssen */
  char version[100];
	int len;
	FLAG steffi_cheat=FALSE;
	char *cheat=romstr029;
	char *cheat_p;
	int i,j;
	int x,y;

	alerts_da=TRUE;								/* Alertboxen sind vorhanden */
	
	Hm();

  fade_out();

  clear_raster();
	clear_screen(scr1);

	set_raster(0,0,leistep);
  //logbase=&x_window;
  logbase=scr1;
  clear_screen(logbase);
  center(15,100,romstr030);           // FOR STEFFI
  show_screen(scr1);
	fade_in();

#if 0
  load_digisound("lachen.seq", pack_buf);
  load_digisound("medusa.seq", pack_buf);
  load_digisound("krieg.seq", pack_buf);
  load_digisound("klong.seq", pack_buf);
  load_digisound("got_you.seq", pack_buf);
  
  load_digisound("krieg.seq",pack_buf+32000L);
  play_digi(pack_buf+32000L,FALSE,5,14);
#endif
  
  // totenkopf();
  //blitz(0x777);
  
  load_objekte("bubeck.obj",pack_buf);

	wait_sync(0);
	if (voller_titel) {
		cheat_p=cheat;
		for(i=150;i>=0;i--) {
			if (steffi_cheat)
				for(j=0;j<3;j++) {
					do {
						x=zufall(319-7);
						y=zufall(199-6);
					} while (x>=121-6 && x<=133+15*4 && y>92-6 && y<125);
          draw_obj_part(1,pack_buf,0,0,6,5,MOVE,logbase,x,y);
					if (i%100==20) writexy_fast(128,112,romstr031);
					else if (i%100==50) writexy_fast(128,112,romstr032);
					}
			hol_maus();
			wait_sync(1);
			if (i==0 && steffi_cheat) wait_sync(150);
			if (is_key()) {
        if (toupper((int)wait_key())!=*cheat_p++) cheat_p=cheat; /* Falscheingabe */
				else if (*cheat_p==0) {
					steffi_cheat=TRUE;
					center(15,100,romstr033);
					i=1000;
          draw_obj(1,pack_buf,MOVE,logbase,0,0); /* Einmal für Palette zeichnen */
					show_raster();
					}
				}
			}
		}
	fade_out();

  logbase=scr1;
	clear_screen(scr1);

  //  show_screen(scr1);
  
	draw_obj(2,pack_buf,MOVE,scr1,80,15);					/* RoM II Logo 80,15 */ 
	
	/* 
	for(i=250;i>=0;i-=10) {
		draw_shrink_obj(i,pack_buf+32000,2,pack_buf,
			MOVE,scr1,160-shrink_obj_breite(i,2,pack_buf)/2,40-shrink_obj_hoehe(i,2,pack_buf)/2);
		}	*/
	
  center(15,118,romstr034);
  center(15,130,romstr035);
	center(15,138,romstr036);
  center(15,146,romstr037);
  center(15,160,romstr038);
  strcpy(version,romstr039);            
  strapp(version,versionsnr);          				
#ifdef COPYPROT
  strapp(version,romstr040);
#else
  strapp(version,romstr041);
#endif

  center(15,180,version);
	strcpy(version,romstr042);
	strapp(version,__DATE__);
	len=strlen(version);
	version[len-9]-='a'-'A';
	version[len-10]-='a'-'A';
	center(15,187,version);
	strcpy(version,romstr043);
	strapp(version,__TIME__);

	center(15,194,version);

  show_screen(scr1);
  //SDL_Delay(6000);
  
  fade_in(); 
  unterschrift("bubeck.off",136,75);
  unterschrift("zimmer.off",125,90);

  Sm();

  load_sprites("sprites.img");    /* lädt die Sprites */

  anfangseinstellungen();       /* Crown Variablen rücksetzen */
	set_armies();									/* Armeen setzen */

  loc=LAND;
  leiste_y=18;
  msminy=17;

  if (voller_titel) {
		set_raster(0,90,kartep);
		set_raster(0,93,kartep);
		show_raster();

		Hm();
    //logbase=&x_window;
    logbase=scr1;
    clear_raster();
	  show_window(ns);
		center(0,97,romstr044);
		center(0,105,romstr045);
		center(0,115,romstr046);
    input(136,123,11,player_name);
    hide_window();
		Sm();
    }

  if (player_name[0]==0)                            /* Spieler hat nicht eingegeben */
    strcpy(player_name,romstr047);

  wait_sync(0);                 /* Sync-Zähler rücksetzen */
  clear_time();                 /* Zeit rücksetzen */

	schoner_x=mx+1;							/* Um Bildschirmschoner kaltzustellen */
}

void unterschrift(char *filename, int x, int y)
{
  /* Zeichnet Unterschrift T.Bubeck auf Bildschirm an x/y */
  unsigned char *koord;
  int xneu,yneu;
	FLAG first_paint;
  unsigned char *mem;

	if (voller_titel) {
		Hm();

		first_paint=TRUE;

    mem=calloc(1000,1);
    load_bibliothek(filename,mem);        /* Koordinaten holen */
    koord=(unsigned char *)mem;
    load_objekte("bubeck.obj",pack_buf);
		copy_screen(scr1,scr2);

#if 0
    printf("scr1\n");
    show_screen(scr1);
    //SDL_Delay(3000);
    printf("scr2\n");
    show_screen(scr2);
    //SDL_Delay(3000);
    printf("fueller: %d %d\n", objekt_breite(0, pack_buf), objekt_hoehe(0, pack_buf));
#endif
    
		cpy_raster(scr2,hlpbuf,0,y,319,y+40,0,y);				/* Copyrights retten */

    //printf("hlpbuf\n");
    //show_screen(hlpbuf);
    //SDL_Delay(3000);
    
	  while(*koord!=255) {
			cpy_raster(hlpbuf,scr2,0,y,319,y+40,0,y);			/* Füller weglöschen */
	    xneu=x+*koord++;
	    yneu=y+*koord++;
			logbase=scr2;
	    plot_pixel(15,xneu,yneu);
			cpy_raster(scr2,hlpbuf,0,y,319,y+40,0,y);			/* Unterschrift retten */
			draw_obj_part(0,pack_buf,0,0,objekt_breite(0,pack_buf)-1,objekt_hoehe(0,pack_buf)-1,
												ODER,scr2,xneu,yneu);			/* Füller ohne Raster zeichnen */
			if (first_paint) {
				draw_obj(0,pack_buf,1,scr2,xneu,yneu);			/* Füller mit Rastern zeichnen */
				show_raster();
				first_paint=FALSE;
				}
			swap_screens();
      //SDL_Delay(3000);
	    }
		cpy_raster(hlpbuf,scr1,0,y,319,y+40,0,y);			/* Füller weglöschen */
    show_screen(scr1);
		Sm();
    
    free(mem);
		}
}

void anfangseinstellungen()
{
  register int j,i;
  register int k;
  long proz_10;
  UWORD *waren_mem;
  long laenge;

  strength_cheat=FALSE;
  produce_cheat=FALSE;
  pio_cheat=FALSE;
  staerke_cheat=FALSE;

  for(i=0;i<SPR_MAX;i++) {
    sx[i]=-1;
    }

	spieler_status=KAEMPFER;						/* Noch ist Spieler gar nichts */
	schatznummer=0;											/* Bisher auch kein Schatz gefunden */
	
  ground_nr=0;                  /* noch keine Untergrundsänderung */
  minenzahl=0;                  /* Spieler hat keine Mine */
  kontostand=0;                 /* Nichts auf dem Konto */
  start_darlehen=FALSE;         /* Nein, er hat ~ noch nicht */

	make_world();								/* Welt erstellen */
	
  citynum=-1;           			/* Spieler auf keiner Stadt oder Burg */
  button=NOTHING;               /* Kein Button angeklickt */
  auf_schiff=FALSE;             /* Spieler ist nicht auf dem Wasser */
	auf_schild=FALSE;					  /* kein Schild betreten */
  sx[0]=sxalt[0]=92;
  sy[0]=syalt[0]=140;                    /* Startposition des Spielers */
  sn[0]=0;
  pos_welt=110;                 /* Spieler in Startland setzen */
  map=welt[pos_welt];           /* map initialisieren */
  vulkan_aktiv=FALSE;           /* Im Moment kein Vulkan aktiv */

  startwert=zufall(5);       	/* 0..4, für unterschiedliche Spiele */

  schiffbesitz=schiffbau=0;     /* Keine im Besitz und im Bau */
  armee_status=AN_LAND;         /* Armee ist an Land */

  year=2168;
  month=10;                      /* Datum setzen */
  day=14;

  startdatum=year*360L+(month-1)*30L+(day-1);
  heute=startdatum;                 /* 15. Oktober 2168 */

  money=0L;                 /* Spieler hat kein Geld */

	/* Bunker initialisieren: */
	for(i=0;i<TUERMAX;i++) put_2bit(tueren,i,2);		/* Alle Türen sind undefiniert, d.h. wie
																				  im Bunkerfile eingetragen */
	for(i=0;i<PARTY;i++) {
		for(j=0;j<3;j++) party[i].reagenz[j]=(i>0) ? 0 : 128;			/* 50% gefüllt */
		party[i].hand[0]=party[i].hand[1]=				/* Leere Hände */
		party[i].helm=party[i].unterteil=party[i].oberteil=party[i].schuhe=-1;
		party[i].schiessen=FALSE;
		}
 
	/* Alle Bunker unbekannt: */	
	for(i=0;i<BUNKERZAHL;i++) 
		bunker_bekannt[i]=bunker_betreten[i]=FALSE;
	
	init_objekte();									/* Objekte initialisieren */
	automapping=FALSE;							/* Kein Automapping */
	helligkeit=0;										/* Volle Lichtstärke zu Beginn */
	
	for(i=0;i<10;i++) prisoner[i].befreit=FALSE;
	
  /* Spielerarmee initialisieren: */

  last_fight=0;                 /* noch nie miteinander gekämpft */

  gesamt_gehalt=0;
  armeegesamt[0]=0;             /* rücksetzen */
  motivation[0]=50.0;           /* Motivation bei 50% */
  for(i=0;i<EINHEITEN;i++) {
    for(k=0;k<EIGENSCHAFTEN;k++) training[0][i][k]=0.0;
    for(j=0;j<RASSEN;j++) sold_zahl[i][j]=0.0;                  /* 0% */
    armeeteil[0][i]=0;
    gehalt[i]=5*armeeteil[0][i];        /* 5 Taler pro Soldat */
    gesamt_gehalt+=gehalt[i];
    armeegesamt[0]+=armeeteil[0][i];
    }
#if 0
	if (my_system) {
		armeeteil[0][3]=1000;				/* 1000 Aufklärer rein */
		armeegesamt[0]=1000;
    } 
#endif
		
  for(i=0;i<3;i++)
    for(j=0;j<EINHEITEN;j++)
      ausruestung[i][j]=0;

  for(i=0;i<GEGNER;i++) {

#if EINHEITEN!=7
#error Einheiten!=7 !
#endif

    motivation[i+1]=50.0;           /* Anfangsmotivation ist 50% */
    armeegesamt[i+1]=0;
    armeeteil[i+1][0]=45+zufall(50);
    armeeteil[i+1][1]=40+zufall(40);
    armeeteil[i+1][2]=20+zufall(10);
    armeeteil[i+1][3]=10+zufall(15);
    armeeteil[i+1][4]=15+zufall(20);
    armeeteil[i+1][5]=2+zufall(2);
    armeeteil[i+1][6]=5+zufall(5);
    for(j=0;j<EINHEITEN;j++) {
      armeegesamt[i+1]+=armeeteil[i+1][j];
      /* Trainingstand gleich wie bei Spieler: */
      for(k=0;k<EIGENSCHAFTEN;k++) {
        training[i+1][j][k]=50.0;               /* Training: 50% */
        }
      }
    }

  sterben_jetzt=FALSE;                          /* Beim nächsten Sumpf nicht sterben */

  for(i=0;i<SCHIFFMAX;i++) flotte[i].typ=-1;     /* Spieler hat noch kein Schiff */
  for(i=0;i<SCHIFFSTYPEN;i++) ship_own[i]=0;

  /* Durchschnittspreise der Waren=Aktienpreis ermitteln: */
  for(i=0;i<WAREN;i++) aktie_preis[i]=0;         /* Durchschnitt löschen */

  waren_mem=malloc(2000);
  laenge = load_bibliothek("cities.war",waren_mem);
  for(i = 0; i < laenge/2;i++) {
    be_2(&waren_mem[i]);
  }

  /* cities.war beinhaltet die Waren-Preise der verschiedenen Städte */
  for(i=0;i<CITIES;i++) {
#if 0
    memcpy((char *)waren_preis,hlpbuf+(long)((WAREN+STALLWAREN+ROHSTOFFE)*i*2),WAREN*2L);
#else
    for(j=0;j<WAREN+STALLWAREN+ROHSTOFFE;j++) {
      int index;
      index=i*(WAREN+STALLWAREN+ROHSTOFFE)+j;
      waren_preis[j] = waren_mem[index];
    }
#endif
    
    for(j=0;j<WAREN;j++)
      {
	if ((long)aktie_preis[j]+waren_preis[j]<65534L) {
        aktie_preis[j]+=waren_preis[j];       /* aufsummieren, nicht teilen */
    }
      }
    }

  for(i=0;i<WAREN;i++) {
    ycargo_menge[i]=0L;                     /* Spieler-Waren löschen */
    aktie_da[i]=2000;                       /* 2000 Aktien pro Sorte */
    aktie_x[i]=zufall(50);                  /* Startposition des Aktienwertes */
		aktie_preis[i]*=2;
		aktie_preis[i]/=CITIES;				/* Doppelte des Durchschnittspreises */
    }
	ycargo_menge[10]=7;						/* Ein paar Batterien hat er schon */
	
  for(i=0;i<ROHSTOFFE;i++) ymetal[i]=0L;        /* Rohstoffe löschen */
  for(i=0;i<STALLWAREN;i++) ystable_menge[i]=0; /* Stallwaren löschen */

  bank_zuletzt=heute;               /* heute war er zuletzt in der Bank */


  for(i=0;i<CITIES;i++) {
    for(j=0;j<EINHEITEN;j++)
      for(k=0;k<EIGENSCHAFTEN;k++)
        stadt_training[i][j][k]=100*(20.0+(float)((wirkung[j][k])*12));   /* 20% .. 80% */

    belong[i]=99;                    /* Stadt gehört Medusa */

    city_last[i]=heute-30L;         /* zuletzt betreten, vor einem Monat */

    for(j=0;j<WAREN+STALLWAREN;j++) waren_menge[i][j]=0;   /* keine (Stall)-Waren da */

    bank_attacked[i]=FALSE;             /* Bank nicht angegriffen */
    city_bank[i]=950L+(long)zufall(100);       /* Summe aller Konten einer Stadt */
    ship_price[i][0]=9000L+zufall(2000);             /* Cutter */
    ship_price[i][1]=17000L+zufall(2000);             /* Cargo */
    ship_price[i][2]=34000L+zufall(2000);             /* Freighter */
    ship_price[i][3]=29000L+zufall(2000);             /* Troop */
    ship_price[i][4]=34000L+zufall(2000);          /* Galley */
    ship_price[i][5]=39000L+zufall(2000);          /* Battleship */

    for(j=0;j<5;j++) ship_art[i][j]=-1;         /* kein Schiff im Bau */

    bevoelkerung[i] = people[i];        /* auf Anfangswert setzen */
    for(j=0;j<RASSEN;j++) {
      stadt_besatzung[i][j]=0;
      }
    crew_grenze[i]=0;    /* In allen Preisklassen vorhanden */

    stadt_armee[i][0]=400L+(people[i]*16L)/200L;       /* Inf 200..1000 */
    stadt_armee[i][1]=200L+(people[i]*8L)/200L;       /* Cav 100..500 */
    stadt_armee[i][2]=100L+(people[i]*4L)/200L;       /* Art 50..250 */
    stadt_armee[i][3]=100L+(people[i]*4L)/200L;       /* Pio 50..250 */
    stadt_armee[i][4]=50L+(people[i]*2L)/200L;       /* DRR 25..125 */
    stadt_armee[i][5]=10L+(people[i]*4L)/2000L;       /* Wiz 5..25 */
    stadt_armee[i][6]=100L+(people[i]*4L)/200L;       /* Arc 50..250 */

    /* Schwankung um 10% nach beiden Seiten reinrechnen */
    for(j=0;j<EINHEITEN;j++) {
      proz_10=((long)stadt_armee[i][j]*10L)/100L;         /* 10 % */
      stadt_armee[i][j]=stadt_armee[i][j]-proz_10+(long)zufall((int)proz_10*2);
      adelay[i][j]=0;                   /* Zeitverzögerung beim Angriff */
      for(k=0;k<3;k++) stadt_aus[i][k][j]=(stadt_armee[i][j]*100L)/66L;   /* 2/3 ausgerüstet */
      }
  }

	belong[13]=belong[14]=belong[15]=belong[19]=belong[20]=GEGNER;
  for(i=22;i<CITIES;i++) belong[i]=GEGNER;              /* Inselstädte sind frei */
}

void intro()
{
	/* Erzählt die Geschichte von Medusa II 
		 Erläuterung zur Fallunterscheidung AMIGA:
		 	 Da beim Amiga der Scroller auf raster_replace basiert, ist es nicht möglich, während
			 der Scroller läuft, den raster_replace() zu benutzen (wird von draw_obj(,,MOVE,,) benutzt).
			 Deshalb: draw_obj(,,ODER,,)			*/			 
			 
	int steuer_neu;
	long laenge;
  ULONG scroller_laenge;
	char *metal_obj,*bild_buf;
	int y;
  char *mem;
  char *intropic;
  char filename[100];
  unsigned char *scrtext;
	
	clear_raster();
	show_raster();
	
#ifndef COPYPROT
	
  if (my_system) {
		if (mousex==mx && mousey==my) {		/* Maus nicht bewegt */
			set_raster(0,0,formularp);
			voller_titel=FALSE;
      printf("my_system -> voller_titel = FALSE\n");
			center(15,100,romstr048);		
			set_raster(0,0,formularp);
			show_raster();
			return;
			}
		}
#endif
		
  Hm();
  //clear_screen(scr2);
	clear_screen(scr1);
  blending = 0;
	show_screen(scr1);														/* scr1 anzeigen */

  mem=malloc(40000);

  load_objekte("bubeck.obj",mem);
  draw_obj(2,mem,MOVE,scr1,(320-objekt_breite(2,mem))/2,0);      // RoM II Logo
      
  formular_big(logbase,63,199);
  center(0,80,
	 "WELCOME TO RINGS OF MEDUSA 2\nTHE RETURN OF MEDUSA!\n\n"
	 "THIS PROGRAM IS BASED ON THE ORIGINAL SOURCE CODE AND GRAPHICS.\n"
	 "THIS MEANS, THAT THE GAMEPLAY IS IDENTICAL.\n"
	 "THEY HAVE BEEN PORTED BY TILL BUBECK, THE ORIGINAL AUTHOR\n"
	 "FROM ATARI/AMIGA TO MODERN SYSTEMS BY USING SDL3.\n\n"
	 "TO HELP YOU PLAYING, VARIOUS CHEATS ARE AVAILABLE:\n"
	 "IN THE DUNGEONS PRESS \"C\" OR \"PRTSCR\" TO ENABLE THEM.\n"
	 "THEN PRESS \"H\" FOR HELP. IN CHEAT MODE, THE EXIT BUTTON\n"
	 "IN A DUNGEON ALLOWS YOU TO EXIT THE DUNGEON IMMEDIATELY.\n\n"
	 "HAVE FUN PLAYING THE OLD GAME!\n"
	 "TILL BUBECK"
	 );
  
  show_last_screen();
	fade_in();
  wait_sync(0); 
  wait_sync_klick_release(3000);
	fade_out();
  
	clear_screen(scr1);
  //load_objekte("bubeck.obj",mem);
  free(mem);
  
  while (is_key()) wait_key();      /* Tastaturbuffer löschen */
  //fade_out();
  clear_screen(scr1);

#if 0
  printf("WAIT X\n");
  while(1) {
    ask_X();
    int32_t taste=get_key();
    if (taste != -1) 
      printf("%lx\n", taste);
  }
#endif
  
  scrtext=malloc(10000);
		
#ifdef FRANZ
  scroller_laenge=load_bibliothek("init_frc.txt",scrtext);
#endif
#ifdef ENGLISCH
  scroller_laenge=load_bibliothek("init_gb.txt",scrtext);
#endif
#ifdef DEUTSCH
  scroller_laenge=load_bibliothek("init_d.txt",scrtext);
#endif

#if 0
  decrypt(scrtext,scroller_laenge);    /* Scroller entschlüsseln */
#endif
	
  metal_obj=malloc(45000);
  laenge=load_objekte("metall.obj",metal_obj);        /* Scroller laden */
	
  init_scroller(scr1,157,scrtext,metal_obj,metal_obj+laenge,FALSE);
#if 0
  // Not necessary to do in VBL as done in loop (2025-08-17)
  scradr=init_vbl(mcode33);     			            /* Laufschrift an */
#else
  scradr = NULL;
#endif
	set_raster(0,157,scroller_pal);
	fade_in();
	
	bild_buf=metal_obj+laenge+54000L;				/* Hier endet der Scrollbuffer */
#ifndef COPYPROT
	if (my_system) {
		if (memory+sizeof(memory)-bild_buf<30000) {			
			writexy_fast(0,6*3,romstr049);
			writexy_fast(22*4,6*3,str(0,(memory+sizeof(memory)-bild_buf)));
			debug(-2289,romstr050,0);
			}
		}
#endif

  intropic=malloc(40000);

  // voller_titel = 1;
  
  while (voller_titel && mousek==0 && !is_key()) {
    do_scroller();
		steuer_neu=steuerzeichen;
		if (steuer_neu>=16 && steuer_neu<=22) {
			clear_raster();
			set_raster(0,157,scroller_pal);
      if (steuer_neu<=20) load_objekte("bubeck.obj",intropic);
      //clear_screen(scr1);
      fill(0,0,0,319,156);
			if (steuer_neu<=20)
#if defined(AMIGA)
				draw_obj(2,bild_buf,ODER,scr1,80,0);		/* Rings of Medusa II Logo */
#elif defined(ATARI) || defined(unix)
        draw_obj(2,intropic,MOVE,scr1,80,0);    /* Rings of Medusa II Logo */
#endif
			show_raster();

      strcpy(filename,"intro_a.obj");
      filename[6]+=steuer_neu-16;
      load_objekte(filename,intropic);
      y=(steuer_neu>=21) ? 0 : 107-objekt_hoehe(0,intropic)/2;
			set_raster(0,y,black);					/* Damit Aufbau nicht zu sehen ist */
			show_raster();

#ifdef AMIGA
			draw_obj(0,bild_buf,ODER,scr1,160-objekt_breite(0,bild_buf)/2,y); 
#else
      draw_obj(0,intropic,MOVE,scr1,160-objekt_breite(0,intropic)/2,y); 
#endif
      //set_raster(0,157,scroller_pal);
			show_raster();	
			steuerzeichen=steuer_neu=0;							/* Bearbeitet */
			}
		}

	while (is_key()) wait_key();			/* Tastendruck ggf. abholen */

  free(scrtext);
  free(metal_obj);
}

void intro_off()
{
#ifndef COPYPROT
	if (!voller_titel) return;
#endif

  fade_out();
	
  clear_raster();
  exit_vbl(scradr);
  clear_screen(scr1);
  Sm();
	
#if defined(AMIGA)
	set_raster(0,0,kartep);					/* Damit Trackdisplay sichtbar */
	show_raster();
	
	center(15,100,romstr048);
#endif
}

void decrypt(txt,laenge)
unsigned char *txt;
long laenge;
{
	/* Entschlüsselt einen Scroller */
	register int wert2,wert1;
	register int j;
	
  for(;laenge>0;laenge--,txt++) {
    wert1=*txt;
    wert2=0;
    for(j=0;j<8;j++) {
      wert2*=2;
      if ((wert1 & 1)!=0) wert2++;
      wert1/=2;
      }
    *txt=wert2;
    }
}

void load_game()
{
  /* Lädt ein Spiel und initialisiert CROWN entsprechend */
  int nr;
	char dir[11][70];
	long old_buttons;
	
	copy_screen(scr1,scr2);
  formular(scr1,63);
	show_raster();

  old_buttons=bleiste;
	redraw_buttons(EXIT_BTN|PAUSE);

	show_user_dir(dir);
	center(0,73,romstr051);
	
  while (button!=EXIT_BTN) {
    show_last_screen();
    wait_klick();
    if (mk==1) {                        /* Knopf wurde gedrückt */
      nr=(my-97)/8;
      if (nr>=0 && nr<8) {
      	if (strcmp(dir[nr],romstr052)!=0) {
	        Hm();
	        formular(scr1,63);
	        center(0,100,romstr053);
					center(0,110,dir[nr]);
  delay(5000);
  
	        load_it(nr);                      /* Game laden */
					load_waende();
	        Sm();
	        button=EXIT_BTN;
	        }
				}
      }
    } 

	set_raster(0,63,NULL);					/* Formularraster löschen */
	show_raster();

	copy_screen(scr2,scr1);
	auto_nr=0;												/* Alle Changes sind übertragen */
	player[0]=player[1];							/* Und Spieler an selber Position */

	button=NOTHING;
	redraw_buttons(old_buttons);
	copy_buttons();
}

void show_user_dir(dir)
char dir[11][70];
{
	/* Zeigt das Directory einer Save-Game Disk an */
	char id[4];
	int i;
	long old_buttons;
	int disk_nr;
	FLAG disk_error;
	
	button=NOTHING;
	disk_nr=-3;												/* Datendiskette */
	
	do {
		disk_error=FALSE;
		insert_disk(disk_nr);							/* Datadisk einlegen */
		floppy_read(id,0,0,1,0,0,1,3);					/* Vorher Track 2: Ist es eine Medusa-datendisk? */
		if (strcmp(id,romstr054)!=0) {
		  old_buttons=bleiste;
		  redraw_buttons(YES|NO|EXIT_BTN|PAUSE);
			show_window(romstr055);						/* Spielstanddiskette machen? */
			
		  do {
	show_last_screen();
		    wait_klick();
		    } while (button!=YES && button!=NO && button!=EXIT_BTN);

		  redraw_buttons(old_buttons);
			hide_window();
			
			if (button==EXIT_BTN) return;
			if (button==YES) {
				strcpy(id,romstr056);
				for(i=0;i<11;i++) strcpy(dir[i],romstr057);

				if (floppy_write(id,0,0,1,0,0,1,3))					/* Vorher Track 2, Id Schreiben */
					disk_error=TRUE;
				else if (floppy_write(dir,0,2,1,0,2,1+770/512,770%512))
					disk_error=TRUE;
				}
			else disk_nr--;								/* Alert 'Disk rein' an */
			}
		else floppy_read(dir,0,2,1,0,2,1+770/512,770%512);		/* Dir bei Spielstand lesen */

		if (disk_error) {
			alert(romstr623);									/* Diskfehler! */
			disk_nr--;
			id[0]=0;													/* Falsche Disk */
			}
		} while(strcmp(id,romstr058)!=0) ;

	Hm();
	for(i=0;i<8;i++) {
		center(0,97+i*8,dir[i]);
		}
	Sm();
}

void load_it(nr)
int nr;
{
  /* Lädt das File */
	
  make_adresses();                  /* Adressen nach hlpbuf speichern */
	load_data(nr,pack_buf);			 /* Abgespeichertes File laden */
	
  if (*(int *)pack_buf!=save_version) {
		alert(romstr059);
		}		
	else {
		copyvar(FALSE);                   /* und übernehmen */
  	vbl_ct_save=vbl_ct;					 /* Heute wurde zuletzt gespeichert. */
		}
	draw_whole=2;									/* Automap neu zeichnen */
}

long copyvar(saveflag)
FLAG saveflag;                  /* True=Speicher, FAlse,=LAden */
{
  /* Kopiert sämtliche Variablenwerte in pack_buf-pack_buff+40000 */
  intptr_t *poi;
  long laenge;
  long total = 0;
  char *scrpoi,*adresse;

  //poi=(long *)hlpbuf;
  poi = save_buffer;
  scrpoi=pack_buf;                          /* Hier hin */
  while(*poi!=-1L) {
    adresse=(void *)*poi++;
    laenge=*poi++;
    if (saveflag) memcpy(scrpoi,adresse,laenge);
    else memcpy(adresse,scrpoi,laenge);
#if 0
    if (saveflag) {
      printf("Saving %ld from %p to %p\n", laenge, scrpoi, adresse);
    } else {
      printf("Loading %ld from %p to %p\n", laenge, adresse, scrpoi);
    }
#endif
    scrpoi+=laenge;
    total += laenge;
    }

  return(total);                  /* Länge zurück */
}

void save_it(nr)
int nr;
{
  /* Speichert das File */

  Hm();

  make_adresses();                  /* Adressen nach hlpbuf speichern */
  copyvar(TRUE);                       /* Kopiert Werte in scr2 */
	
	if (save_data(nr,pack_buf)<0)
		alert(romstr060);
	else vbl_ct_save=vbl_ct;		/* Heute wurde zuletzt gespeichert */

  Sm();
}

void save_game()
{
  /* Speichert ein Spiel */
	char dir[11][70];
	int nr;
	long old_buttons;
	
  if (0 && vbl_ct<vbl_ct_save+5*50*60) {
    dungeon_alert(romstr061);
		}
  else {
		copy_screen(scr1,scr2);
		
    formular(scr1,63);
		show_raster();
  	old_buttons=bleiste;
	  redraw_buttons(EXIT_BTN|PAUSE);

		show_user_dir(dir);
		center(0,73,romstr062);					/* Spielstand speichern */
		
		while (button!=EXIT_BTN) {
			wait_klick();
			if (mk==1) {
				nr=(my-97)/8;
				if (nr>=0 && nr<8) {
	        Hm();
	        formular(scr1,63);
	        center(0,100,romstr063);
					input(20,110,68,dir[nr]);
					if (dir[nr][0]!=0) {
						if (floppy_write(dir,0,2,1,0,2,1+770/512,770%512))		/* Dir schreiben */
							alert(romstr623);
		        else save_it(nr);                      					/* Game laden */ 
						load_waende();				
		        }
	        Sm();
	        button=EXIT_BTN;
	        }
	    	}
    	}

		set_raster(0,63,NULL);					/* Formularraster löschen */
		show_raster();
		
		copy_screen(scr2,scr1);
		auto_nr=0;												/* Alle Changes sind übertragen */
		player[0]=player[1];							/* Und Spieler an selber Position */

		button=NOTHING;
		redraw_buttons(old_buttons);
		copy_buttons();
    }
	
}

void totenkopf()
{
	/* Stellt den Totenkopf dar und läßt ihn ein bißchen reden */
	int anzahl;
	long laenge;
	
  laenge=load_objekte("totenkop.obj",pack_buf);
	if (laenge&1) laenge++;
	
  load_digisound("lachen.seq",pack_buf+laenge);
	
	Hm();

	fade_out(); 							  /* Bild ausblenden */
	clear_raster();
	clear_screen(scr1); 				/* Bildschirm löschen */

	zeichne_totenkopf(0);					/* Totenkopf in Grundposition zeichnen */
	switch_screens();
	fade_in();

	for(anzahl=0;anzahl<100;anzahl++) {
  	if (effects && !digi_works) play_digi(pack_buf+laenge,FALSE,0,0); /* Lachen */
		zeichne_totenkopf(zufall(14));
		switch_screens();
		}

	Sm();
}

void zeichne_totenkopf(y)
int y;
{
	/* Zeichnet den Totenkopf auf den Bildschirm und den Kiefer um y nach unten verschoben */

	draw_obj(0,pack_buf,0,scr2,0,0);
	draw_obj(1,pack_buf,1,scr2,95,100+y);
}


