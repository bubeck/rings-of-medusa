  				       
/* Dieses File beinhaltet alle Funktionen, die absolut Atarispezifisch sind
   diese müssen entsprechend für andere Computer ersetzt werden */

#include "includes.c"

long ssp;               /* Supervisorstackpointer */

/* Füllmuster zum kompletten Ausfüllen von Flächen: */
int solid[16] = {
	-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1 };
	
/* Start: */

#ifndef COPYPROT

void flash_amiga()
{
  int i,j;
  int *farbe;

	farbe=(int *)0xdff180;
	  
  for(j=0;j<50;j++)
    for(i=0;i<30000;i++)
      *farbe=i;
}

#endif

void hippel_sfx(sound_nr)
int sound_nr;
{
	/* Schaltet ggf. den Hippel-Sfx ein */

	/* Beim Amiga sind einige Effektnummern vertauscht, bzw. nicht vorhanden. */
		
#ifdef AMIGA
	switch (sound_nr) {
		case ROULETTE_AN: sound_nr=ROULETTE_AUS; break;		/* Kein Effekt */
		case PEILSTRAHL: sound_nr=ROULETTE_AN; break;
		case FX_AUS: sound_nr=ROULETTE_AUS; break;
		}
#endif

	if (music==MUSIK_AUS && effects) hippel_fx(sound_nr);
}

void init_atari()
{
  /* Diese Funktion initialisiert das ST-Betriebsystem, sowie die
     Hardware */
	
  init_hardware();             /* Intialisiert die Hardware */
	
	line_mode(0);												/* Replace */
	line_pattern(solid);								/* Komplett ausgefüllt */
	
  srand(rnd());										/* Zufallszahlengenerator initialisieren */

	init_maus();										/* Maus initialisieren */
	hbl_init();											/* Raster anschalten */
	show_screen(scr1);
	logbase=scr1;
	set_raster(0,0,leistep);
	show_raster();
}

void init_hardware()
{
  /* Initialisiert die Hardware */

  sync=get_sync();               /* Get_Sync() */
  mcode3();                     /* Init_Hardware */
}

long wait_key()
{
  /* Wartet bis eine Taste gedrückt wurde und liefert diese zurück */
  long taste;
  
  while((taste=get_key())==-1L) 
#ifdef COPYPROT 
	;
#else
	if (my_system && mousek==2) hol_maus();
#endif

  return(taste);
}

#ifndef COYPROT

void make_bus_error()
{
 	/* Diese Funktion lößt einen Buserror aus -> Debugger */
  char *x;
  
  x=(char *)5;
  (*x)++;									/* und tschüß.... */
}

#endif

void plot_pixel(color,x,y)
int color,x,y;
{
  /* Zeichnet einen einzigen Punkt auf den Bildschirm */
  
  line(color,x,y,x,y);				/* Nicht gerade elegant, aber zweckmäßig... */
}

#ifndef COPYPROT

void programmende()
{
#ifdef ATARI
  Jdisint(4);               /* Musik aus */
#endif

  digi_aus();
  hbl_exit();               /* Rasterirq's wieder freigeben */
	exit_vbl(eintrag);									/* VBL-Routine raus */

#ifdef ATARI

  Setscreen(oldbase,oldbase,1);    /* alte Bildschirmbasis setzen */
	Setcolor(0, 0x777);
  Setcolor(1, 0x700);
  Setcolor(2, 0x050);               /* Farben setzen (unwichtig) */
  Setcolor(3, 0x000);
  Setcolor(15,0x000);
  Kbshift((int)Kbshift(-1) & 0xffef);
	Blitmode(1);																/* Blitter anschalten */
#endif

	exit(0);
}
#endif

void init_archiv()
{
	/* Öffnet das Archiv, läßt aber noch alle Files auf der Diskette
			d.h. es wird kein Speicher alloziert */
	int i;

	file_offset[OFFSET_DAT]=0;										/* 1. File beginnt ganz vorne */
	file_len[OFFSET_DAT]=10000;										/* hat auf jeden Fall diese Länge */
	file_disk[OFFSET_DAT]=0;											/* und im 1. Archiv */
	ram_max=-10;																	/* Bisher keine Files in Ramdisk */

	load(OFFSET_DAT,&ram_min,0L,2L);							/* Ab welchem File soll Ramdisk beginnen? */
	load(OFFSET_DAT,file_offset,2L,(FILE_ZAHL+1)*4L);
	load(OFFSET_DAT,file_len,2+(FILE_ZAHL+1)*4L,FILE_ZAHL*4L);

  for (i=0;i<FILE_ZAHL;i++) {
    file_disk[i]=file_offset[i]>>24;						/* In welchem Archiv ist File? */
    file_offset[i]&=0x00ffffff;
    }
  file_offset[i]&=0x00ffffff;               /* Disk-Information raus */
}

void fill_ramdisk()
{
  /* Füllt das 'ramdisk'-system */
  int i;
  long akt_len;
	int akt_file;
	long akt_pos,len;
	char *ram_pointer;
	long laenge;

#ifdef AMIGA
	if (fast_len>0) {						/* Noch Fast-Mem vorhanden? */
		mem_strt=fast_mem;
		mem_len=fast_len;
		}
#endif

  for(ram_max=FILE_ZAHL-1;
  		ram_needed(ram_max)-ram_needed(ram_min-1)>mem_len;
  		ram_max--);
  
  ram_size=ram_needed(ram_max)-ram_needed(ram_min-1);

	if ((ram_size&1)!=0) ram_size++;  								/* Ramdisk gerade machen */

  ramdisk=mem_strt;                 
  mem_strt=(char *)mem_strt+ram_size;								/* Speicher belegen */
	mem_len-=ram_size;
	
	akt_file=ram_min;													/* Hier beginnen mit lesen */
  ram_pointer=ramdisk;
  
	do {
		if (file_disk[akt_file]<file_disk[ram_max]) {						/* Schon in letztem Archiv? */
			akt_len=loaddisk(file_disk[akt_file],file_offset[akt_file],0xdfdfdfL,ram_pointer);
			ram_pointer+=akt_len;
			for(i=0;i<FILE_ZAHL;i++) if (file_disk[i]>file_disk[akt_file]) break;
			akt_file=i;																			/* Bis hier hin wurde gelesen */
			}
		else {
		  if (file_disk[ram_min]!=file_disk[ram_max]) len=file_offset[ram_max]+file_len[ram_max];
		  else len=file_offset[ram_max]+file_len[ram_max]-file_offset[ram_min];
			akt_len=loaddisk(file_disk[akt_file],file_offset[akt_file],len,ram_pointer);
			break;
			}
		} while(TRUE);
		
	/* Jetzt die Offsets auf die Ramdisk anpassen */
	akt_pos=0;
	for(i=ram_min;i<=ram_max;i++) {
		file_offset[i]=akt_pos;
		akt_pos+=file_len[i];
    if ((akt_pos&1)!=0) akt_pos++;              /* aufrunden auf gerade Länge */
		}
}

long ram_needed(ram_max)
int ram_max;
{
  /* Berechnet, wie groß Ramdisk sein müßte, damit dieses File noch reingeht */
  register int i;
  register long groesse;

  groesse=0;
  for(i=0;i<=ram_max;i++) {
    groesse+=file_len[i];
    if ((groesse&1)!=0) groesse++;              /* aufrunden auf gerade Länge */
    }
  return(groesse);
}

void load_digisound(file_nr,adresse)
int file_nr;
void *adresse;
{
	/* Lädt einen Digisound und konvertiert diesen entsprechend */
	long len;
	
	if (digi_works) digi_aus();
	
	len=load_bibliothek(file_nr,adresse);
	
#ifdef AMIGA 
		{
		char *adr;
		int *digi;
		int anz_zeilen,anz_seq;
		char *digi_end;
		
		adr=(char *)adresse;
		digi=(int *)adr;
		anz_seq=digi[2];
		anz_zeilen=digi[3];
		digi_end=(char *)adr+len;
		adr+=anz_zeilen*4+anz_seq*24+8;						/* Startadresse der Digidaten */
		for(;adr!=digi_end;) 
			*adr+++=128;
		}
#endif
}

void load_sprites(name)
int name;
{
  /* Lädt die Sprites Datei */

  load_bibliothek(name,sprite_mem);     /* Datei laden, soviel wie geht */
}

void rahmen(color,x1,y1,x2,y2)
int color,x1,y1,x2,y2;
{
  /* Zeichnet einen Rahmen: linke obere Ecke (x1/y1), rechte untere (x2/y2) */

	line(color,x1,y1,x2,y1);
	line(color,x2,y1,x2,y2);
	line(color,x1,y2,x2,y2);
	line(color,x1,y1,x1,y2);
}

unsigned char wandel(zeichen)
unsigned char zeichen;
{
  /* Wandelt ASCII-Zeichen in internen Zeichensatz */

  switch (zeichen) {
    case 0x9a: zeichen='#'-' ';             /* ü */
               break;
    case 0x8e: zeichen='&'-' ';             /* Ä */
               break;
    case 0x99: zeichen='$'-' ';             /* Ö */
               break;
    default: zeichen-=' ';
    }
  return(zeichen);
}

#ifndef COPYPROT

void debug(nr,text,var)
int nr;
char text[];
int var;
{
	/* Gibt eine Debug-Variable auf dem Screen aus: 
		 Wird als nr eine negative Zahl eingegeben, stoppt das Programm, bis zum Tastendruck.
		 Wird Escape eingegeben, so wird in Zukunft ein Aufruf mit dieser negativen Zahl nicht 
		 angehalten. Eine andere negative Zahl stoppt das Programm wieder. */
		 
	static int weiter_nummer=999;
	
	Hm();
	if (my_system) {
		if (hbl_system[0][16]==0) {					/* Farben vorhanden? */
			set_raster(0,0,kartep);
			show_raster();
			}
		if (nr>=0) {												/* Text vorhanden? */
			writexy_fast(0,nr*6,text);
			writexy_fast(strlen(text)*4,nr*6,str(5,(long)var));
			writexy_fast(strlen(text)*4+strlen(str(5,(long)var))*4,nr*6,romstr002);
			}
		else 
			if (weiter_nummer!=nr) {
				writexy_fast(0,193,romstr003);
				if ((int)wait_key()==27) {
					weiter_nummer=nr;
					}
				writexy_fast(0,193,space(21));
				}
		}
	Sm();
}

#endif
	
