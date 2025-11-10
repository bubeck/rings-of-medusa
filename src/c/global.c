                   
    /* Global File Version 2.0 - by Till Bubeck */
#include <SDL3/SDL.h>

int save_version;        /* Version des Savefiles */

char player_name[12];       /* Name des Spielers */
unsigned char spieler_status;					/* Magier, Mogul ... */

 /* ------------------------------------------------------------------------
                            INTERNE VARIABLEN
    ------------------------------------------------------------------------ */

mem_mapper_t digi_mem[DIGI_COUNT];

char *ns;                       /* Zeiger auf einen Nullstring */

char insert_disk[] = romstr620;		/* Für den Diskhandler */
char insert_game[] = romstr621;
char standard_disk[] = romstr622;		

FLAG cheat_on;                  /* Cheatabfrage eingeschaltet? */
FLAG treffer_cheat;						/* Kann Spieler im Bunker getroffen werden? */
FLAG rohstoff_cheat;				  /* Werden alle Rohstoffe angezeigt? */
FLAG door_cheat;							/* Können alle Türen des Bunkers geöffnet werden */
FLAG produce_cheat;             /* Wird in city_info produkte angezeigt? */
FLAG unlim_money_cheat;         /* Jeder VBL 100000 */
FLAG citycheat;                 /* Wenn TRUE, dann in Stadt 0 */
FLAG strength_cheat;            /* TRUE bedeutet, daß beim Kampf die Stärke gezeigt wird */
FLAG pio_cheat;                 /* Zeigt alle Gegner an */
FLAG kaserne_cheat;             /* Kann Spieler Kaserne betreten */
FLAG treasure_cheat;            /* Bildschirm flackert Blau, wenn Spieler auf Schatz */
FLAG staerke_cheat;             /* Armee ist ein bißchen stärker */
FLAG zeit_cheat;                /* Zeigt die verbleibende Zeit an */
FLAG schuss_cheat;					  /* Spielerschuß im Dungeon doppelt so stark */
FLAG my_system;									/* Zeigt an, ob Medusa auf meinem Rechner läuft (Cheats..) */

int speed;                      /* Anzahl der Vsyncs zwischen Bildwechsel */

int armee_x,armee_y;            /* Position der Armee wenn Schiff aktiv */
int armee_stadt;                /* Stadt, in der Armee sich befindet */
int armee_pos,armee_status;     /* button: AN_BORD oder AN_LAND */

int vulkanx,vulkany;        /* X/Y Koordinaten des Vulkans */
int vulkan_n,vulkan_stufe;  /* Nr des Vulkansprites, Stufe der Animation */
FLAG vulkan_aktiv;
int vulkan_counter;         /* Zähler zur Animationverzögerung */

char zahl_str[80];
char dat_str[20];           /* für Datumswandlung */

void *scradr;							/* Adresse des VBL-Queue Eintrags des Scrollers */
/* Palette of TANIS_1.IFF: Saved by NEOchrome V2.24 by Chaos, Inc. */
RGB_PAL scroller_pal = { 
	0x0000,0x0300,0x0400,0x0510,0x0520,0x0630,0x0740,0x0750,
	0x0760,0x0770,0x0774,0x0776,0x0777,0x0620,0x0200,0x0730 };
RGB_PAL black = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

int sync_50_60;                   /* Syncronisationsfrequenz 50,60 Hz */
unsigned long vbl_ct_save;	/* Wann wurde das letzte Mal gespeichert? */

int copylist[]= {
  13,14,
  12,13,
  10,12,
  9,11,
  7,10,                     /* Zum Drehen der unteren Leiste */
  6,9,
  4,8,                      /* Dabei wird hier z.B. Zeile 4 -> 8 kopiert */
  3,7,
  1,6,
  0,5,

  17,16,
  18,17,
  20,18,                    /* untere Hälfte der Leiste */
  21,19,
  23,20,
  24,21,
  26,22,
  27,23,
  29,24,
  30,25 };

/* Füllmuster zum Ausmaskieren der Icons in der Optionleiste: */
UWORD point_pat[16] = {
	0x5555,0xaaaa,0x5555,0xaaaa,
	0x5555,0xaaaa,0x5555,0xaaaa,
	0x5555,0xaaaa,0x5555,0xaaaa,
	0x5555,0xaaaa,0x5555,0xaaaa };

int disk_buf[256];                  /* 512 Bytes Diskbuffer */

char password[30] = { romstr320 };						

FLAG voller_titel=TRUE;				/* Soll volle Titelsequenz kommen? */
/* How much bigger is the window for the screen, than 320x200: */
int gfx_scale_factor = 3;     /* Faktor zur Vergroesserung des Fensters */
int crt_effect = 1;     /* Should we do tube tv like graphics */
volatile int msminy;              /* kleinster Wert der Mouse y */
int leiste_y;               /* unterer Rand der Leiste */
int wind_aktiv;           /* Flag, ob Window aktiv ist */

SDL_Surface *window_back;             /* sichert den Hintergrund eines Windows */
char wind_form[10000];              /* Enthält die Alertboxen */
FLAG alerts_da;											/* Flag, ob die Alertboxen bereits geladen sind */
ULONG *formular_adr;           /* Adresse des Formulars */

int *maus_koord;            /* Pointer auf die Mauskoordinaten (X/Y) */
int maus_zaehler;           /* Anzahl der Abschaltvorgänge für Hm(),Sm() */
FLAG ms_on;                /* Flag, ob Maus dargestellt werden soll */
volatile int save_1[134],save_2[134];       /* nimmt den Hintergrund der Maus auf */
                                    /* 1. Wort=0: Buffer ungültig */
                                    /* 1. Wort!=0: Buffer gültig */
volatile void *scrn_1;                          /* Screen1 */
volatile int mousex,mousey,mousek;     /* Werte der Maus, wird durch IRQ erneuert */

jmp_buf restart;                /* für longjmp um Crown erneut zu starten */

char a1[16];                    /* alten Strings der list_info anzeige */
char b1[22];
char c1[30];

long bleiste;               /* Status der Buttons in Bleiste */
long bleiste_old;           /* die alte Bleiste */
int option_nr;              /* Nummer des Boards, das zu sehen ist */
int menu_koord[] = {
  4,173,19,182,             /* SAVE */
  4,183,19,194,             /* LOAD */
  19,173,32,182,            /* EDITOR */
  73,173,90,183,            /* unbelegt (Peilgerät) */
  38,172,52,182,            /* BUY */
  38,182,52,194,            /* SELL */          /* Koordinaten der Icons */
  55,173,66,182,            /* ALL */
  55,183,66,194,            /* INFO */
  19,183,32,194,            /* EXIT */
  73,184,90,194,            /* PEILUNG */
  230,173,246,183,          /* YES */
  230,183,246,194,          /* NO */
  252,172,266,183,          /* FLEE */
  266,172,283,183,          /* ATTACK */
  252,183,283,194,          /* TERMS */
  299,173,315,184,          /* SEARCH */
  299,184,315,194,          /* DIG */
  283,172,292,183,          /* PLUS */
  283,183,292,194 };        /* MINUS */

long menu_nr[]= {
  SAVE,LOAD,EDITOR,PAUSE,BUY,SELL,ALL,INFO_BTN,EXIT_BTN,     /* Wertigkeit der Icons */
  PEILUNG,YES,NO,FLEE,ATTACK,TERMS,SEARCH,DIG,PLUS,MINUS };      /* Rückgabe in button */

char memo[15][75];      /* Nimmt Editorscreen auf */
int leisten[20000/sizeof(int)];     /* Nimmt die Objekte der Leisten auf, bitte auf gerade Adresse */

SPRITE sprite_mem[SPR_ZAHL];								/* Speicher für die ungeshifteten Spritedaten */
SDL_Surface *sprite_surfaces[SPR_ZAHL];

/* Speicher   scr1,  scr2,hlpbuf,pack_buf,bunker_objekte */
char memory[32256L+32000L+32000L+32000L+32000L+32000L+24000L];

SDL_Surface *hlpbuf;           /* kann ein Bild aufnehmen (32128) frei verfügbar */
char *pack_buf;         /* Hilfsspeicher, zum Entpacken */
SDL_Surface *scr1;             /* 2 Bildschirmtechnik */
SDL_Surface *scr2;
void *oldbase;           /* alte Bildschirmbasis */
FLAG effects;     /* Effekte und/oder Musik an/aus */
int music;							/* Welche Hippelspielmusik ist an? */
SPRITE_SAVE save1[SPR_MAX],save2[SPR_MAX];             /* Offsets für Hintergrund */
int sx[SPR_MAX];           /* Spritekoordinaten */
int sy[SPR_MAX];           /* 0=Sprite aus */
int sn[SPR_MAX];       /* Spriteform */
int sxalt[SPR_MAX];        /* Zielkoordinaten */
int syalt[SPR_MAX];
int sn_save[SPR_MAX];                     /* Shapenummer der normalen Sprites retten */
int sx_save[SPR_MAX];
int sy_save[SPR_MAX];
int sxalt_save[SPR_MAX];
int syalt_save[SPR_MAX];

int anir[SPR_MAX];            /* -1,1 je nachdem in welche Richtung animiert wird */
FLAG ani[SPR_MAX];            /* Wird Sprite animiert? */
int auftrag[SPR_MAX];         /* Aufträge der einzelnen Einheiten */

int hafenx,hafeny;      /* Koordinaten des Hafens, der Stadt */
int grundx,grundy;      /* Koordinaten des Eingangs bei such_eingang() */
int schleife;

int prevx,prevy;      /* Spritekoordinaten, wenn Spieler Stadt verläßt */
long button;          /* angeklickter Button in bottom_leiste */
int rez;                   /* Auflösung, die vorher eingestellt war */
FLAG verlassen;       /* Flag, ob Programmende gewünscht */
FLAG game_loaded;			/* Im Subway wurde ein Landbunker geladen -> Stadt quick quit */

int mx, my, mk;            /* Mauskoordinaten, Maustasten */
int bstate;            /* Flag, ob Maus an- oder abgeschaltet */
FLAG maus_sendung;      /* Sollen Mauskoordinaten im VBL erfragt werden? */

float level;						/* Level des Spiels 1.0-2.0 */

long kontostand;        /* Geld auf dem Konto */
FLAG start_darlehen;        /* Hat er sein Startdarlehen schon? */

extern int fnt_buf[];       /* Definiert in MCODE.O */

char *ramdisk;
long ram_size;              /* Basisadresse der romstr321, größe */
long file_offset[FILE_ZAHL+1];       /* Platz für die Fileoffsets */
long file_len[FILE_ZAHL];           /* Größe der Files */
char file_disk[FILE_ZAHL];          /* Welches File in welcher Datei */
int ram_max;                        /* Letztes File, daß noch in der Ramdisk steht */
int ram_min;												/* Erstes File, daß in der Ramdisk ist */
int file_handle;                    /* File_Handle der Grafik-Disk-Datei */

extern volatile int os_ver;          /* TOS-Version von MCODE.O */
extern volatile int digi_works;     /* Flag, ob Digisound gerade arbeitet */

int zeile,spalte;           /* Cursorposition */
int dummy;                  /* Dummyparameter für Funktionnsparameter */

long *eintrag;                   /* Eintragsadresse in VBL_Liste */

char filename[100];             /* Filename, der bei fsel_input zurückgeht */
char savepfad[100];             /* Pfad, auf den Spiele gespeichert werden */

  /* ---------------------------------------------------------------------
                        FUNKTIONSPROTOTYPEN
    --------------------------------------------------------------------- */

/* 
 * Achtung! Die Funktion build hat eine variable Anzahl an Parametern!
 * (siehe IO.C -> build)
 */
cdecl char *build(char *format,...);
void Seeschlacht(int nummer);

  /* ---------------------------------------------------------------------
                                     MINEN
     --------------------------------------------------------------------- */
 
int minenzahl;                          /* Wieviel Minen hat der Spieler? */

MINE mine[MINE_MAX];                /* Hier sind alle Werte gespeichert */
                        /* Das array wird von vorne nach hinten belegt, und
                           wenn ein Eintrag frei wird, werden die anderen
                           aufgerückt */

  /* ---------------------------------------------------------------------
                                     BUNKER
     --------------------------------------------------------------------- */

unsigned char tueren[TUERMAX*2/8];		/* eine Tür hat 2 Bit, Darstellung:
																   Bit 1=0: Tür bereits eingetragen, dabei
																   	Bit 0: Tuerstatus (0=Auf, 1=Zu)

																   Bit 1=1: Tür noch nicht eingetragen
																     Bit 0: Tuerstatus (0=wie in Bunkerfile eingetragen)
																				   (1=invertiert) */
ITEMS item[ITEMMAX];							/* Bunkerobjekte */
ENEMY enemy[ENEMYMAX];						/* Bunkergegner */
int enemy_max;										/* Wieviel Einträge sind tatsächlich benutzt? */
unsigned char schatznummer;				/* Wieviel Schätze hat der Spieler schon gefunden? */

PARTY_MEMBER party[PARTY];				/* Mitglieder der Party */
 
FLAG automapping;									/* Automapping an oder aus */
int helligkeit;										/* Helligkeit der Lampe */

FLAG bunker_bekannt[BUNKERZAHL];				/* Welcher Bunker ist bekannt? */
FLAG bunker_betreten[BUNKERZAHL];				/* Welcher Bunker wurde schon betreten? */

unsigned char exit_anzahl;					/* Anzahl der bisher durchgeführten Exits 
																			für Passwortabfrage */

/* In welchem Land geht es in welchen Bunker? 
	 Bit 7 gesetzt -> nur Ausgang */
unsigned char bunker_nr[LAENDER][2] = {
  { 20,39 },                /* A Endbunker, Tunnelbunker->C */
  { 21+128,-1 },            /* B Entwicklerausgang Startbunker */
  { 36,38 },                /* C Tunnelbunker->E, Tunnelbunker->A*/
  { 8,-1 },                 /* D */
  { 37,-1 },                /* E Tunnelbunker->C */
  { 5,6 },                  /* F */
  { 1,-1 },                 /* G */
  { 2,22+128 },             /* H */
  { 4,-1 },                 /* I */
  { -1,-1 },                /* J */
  { 11,-1 },                /* K */
  { 41,40 },                /* L Tunnel */
  { 13,-1 },                /* M */
  { -1,15 },                /* N */
  { 14,-1 },                /* O */
  { 17,-1 },                /* P */
  { -1,18 },                /* Q */
  { 27,-1 } };
	
/* In welcher Stadt ist welcher Eingang zum Personenbunker 43=Leerpersonendungeon*/
signed char personen_bunker[CITIES] = {
		26,43,24,43,23,43,43,19,43,43,32,43,34,43,43,43,43,33,43,30,43,43,43,35,43,43,43,42 };
	
unsigned int bunker[86][56];			/* Bunker mit 3 Rand überall */
unsigned char automap[50*80/4];		/* Bitfeld (2 Bit pro Eintrag) welche Position ist betreten */
unsigned char itemmap[50*80/4];		/* 2-er Bitfeld, als 1-er genutzt, zeigt an, wo Items liegen */

unsigned char reagenz_stand[PARTY*3];	/* Flüssigkeitsstände der Reagenzen */

int auto_nr;
KOORD player[2];								/* Position des Spielers in der Automap in Automap-Koordinaten
																	[0]=Position auf letztem Screen, [1]=Position auf akt Screen */
int auto_x,auto_y;								/* Position des Automapping-Bildschirms */
AUTOMAP2 automap_2[16];						/* Veränderungen, die noch nicht auf 2. Screen sind */
unsigned char draw_whole;					/* Wie oft soll gesamte Karte auf 2. Screen gezeichnet werden? */

/* NEOchrome Grafikbibliotheken: */
char *walls;											/* Grafikobjekte der Bunkerwände */
char *items;											/* Grafikobjekte der Objekte */
char *item_rea;										/* Grafikobjekte der Objekte im Reagenzglasbereich */
char *reagenz;										/* Grafikobjekte der Reagenzgläser */
char *enemy_obj;									/* Grafikobjekt des Feindes */
char *fire;												/* Grafikobjekte der Explosion */
char *shoots;											/* Grafikobjekte des Spielerschusses */

void *shrink_buff;                /* Speicherbereich zum Verkleinern von Objekten (SDL_Surface) */

BUNKERWAND *startwaende;					/* Hier beginnen die Wände (zum Anzahl berechnen) */
int bunker_x,bunker_y;					    /* Position Spieler im Dungeon */
int bunker_dx,bunker_dy;				    /* Blickrichtung in Delta */
int richtung; 									    /* Blickrichtung im Bunker */
int button_pressed=-1;						/* Welcher Knopf war im Bunker gedrückt? */
signed char akt_bunker=-1;				/* Bunkernummer, in der Spieler ist */

unsigned int item_max;						/* welcher item[] Eintrag ist frei? */

/* Palette of TERM1.IFF: Saved by NEOchrome V2.25 by Chaos, Inc. */
RGB_PAL bunker_pal = { 
	0x0000,0x0774,0x0760,0x0740,0x0213,0x0124,0x0135,0x0055,
	0x0200,0x0411,0x0621,0x0021,0x0432,0x0543,0x0664,0x0777 };		/* Gelbstufen */
/* Palette of ITEM.IFF: Saved by NEOchrome V2.25 by Chaos, Inc. */
RGB_PAL tv_pal = { 
	0x0000,0x0774,0x0760,0x0740,0x0213,0x0124,0x0135,0x00DD,
	0x0200,0x0411,0x0621,0x0222,0x0333,0x0444,0x0555,0x0666 };
FLAG tv_modus=FALSE;

RGB_PAL bunker_raster;						/* tatsächliche Rasterpalette */

unsigned long enemycounter;				/* Wann soll sich Gegner wieder bewegen? */
unsigned long lampencounter;				/* Wann soll Lampe dunkler werden */
unsigned long hungercounter;				/* Wann soll Hunger größer werden? */
unsigned long durstcounter;				/* Wann soll Durst größer werden? */

int helligkeit;										/* in welcher Helligkeit [0..4] */

SHOT shot;												/* Max. 1 gegnerischer Schüsse gleichzeitig */
int schussstaerke;								/* Wie stark ist der Spielerschuß? */

/* Namen der Gefangenen vorinitialisieren: */
PRISONER prisoner[10] = {
	{ romstr322,40 },
	{ romstr323,50 },
	{ romstr324,100 },
	{ romstr325,150 },
	{ romstr326,240 },
	{ romstr327,60 },
	{ romstr328,255 },
	{ romstr329,20 },
	{ romstr330,178 },
	{ romstr331,220 } };
	
  /* ---------------------------------------------------------------------
                              EIGENSCHAFTEN
     --------------------------------------------------------------------- */

char eigenschaftname[EIGENSCHAFTEN][17] = {
                                 romstr332,       			/* Koerperkraft  */
                                 romstr333,       		/* Kampfgeist */
                                 romstr334,       		/* Ausdauer */
                                 romstr335,       		/* Schnelligkeit */
                                 romstr336        		/* IQ */
                                 };

/* ------------------------------------------------------------------------
                                ARMEEN
--------------------------------------------------------------------------- */

char gegnername[GEGNER+1][13] = {
  	  romstr337,
	  	romstr338,
	    romstr339,
  	  romstr340,
    	romstr341,
			romstr342,
			romstr343,
			romstr344,
			romstr345,
			romstr346,
			romstr347,
			romstr348,
			romstr349,
			romstr350,
			romstr351,
      romstr352 };

char armeename[EINHEITEN][13] = {
                          romstr353,
                          romstr354,
                          romstr355,
                          romstr356,
                          romstr357,
                          romstr358,
                          romstr359     };

FLAG sterben_jetzt;             /* Läuft Spieler in Sumpf, stirbt Armee nur bei
                jedem 2. VBL - Das ist das Flag, das hin und her springt. */

long armeegesamt[ARMEEN];   /* Gesamtstärke der Armee */
long armeeteil[ARMEEN][EINHEITEN];  /* Zahl der Soldaten je Armeeteil & Heer */
float sold_zahl[EINHEITEN][RASSEN];   /* Wieviel % je Rasse bei jeder Einheit */
unsigned long gehalt[EINHEITEN];       /* Gehalt je Einheit */
unsigned long gesamt_gehalt;            /* Gehalt der Soldaten+Matrosen */

int wirkung[EINHEITEN][EIGENSCHAFTEN] = {
    /* legt fest, welche Wirkung die einzelnen Eigenschaften bei den Einheiten
       haben, d.h. die Scouts müssen z.B. nicht stark sein, aber schlau etc. */
   { 5,5,5,1,1 },               /* 5=sehr wichtig, 1=weniger wichtig */
   { 1,5,1,5,3 },
   { 1,5,1,3,5 },
   { 1,1,5,5,5 },
   { 3,5,3,3,3 },
   { 1,1,1,5,5 },
   { 1,5,3,5,3 } };

float training[ARMEEN][EINHEITEN][EIGENSCHAFTEN];     /* Stand der Eigenschaften 0-100% */
float motivation[ARMEEN];                     /* Motivation 0-100% */

int kraft_boden[EINHEITEN][8] = {
  /* Ebene, Wald, verz. Wald, Hügel, Sumpf, Stadt, Burg, Schiff */
     { 10,    10,    5,          8,      8,    10,    10,    8 },
     { 20,    5,     3,          15,     3,    15,    15,    1 },
     { 10,    3,     1,          15,     1,    20,    20,    20 },
     { 8,     20,    8,          15,     15,   10,    10,    8 },
     { 20,    3,     1,          20,     20,   15,    15,    15 },
     { 8,     10,    20,         15,     20,   20,    20,    20 },
     { 20,    3,     1,          10,     8,    15,    15,    15 } };

float kampfkraft[2][EINHEITEN];          /* 0=Spieler, 1=Gegner */
FLAG im_kampf[2][EINHEITEN];            /* Ist die Einheit im Kampf ? */
float kampf_teil[2][EINHEITEN];        /* 0=Spieler, 1=Gegner (=armeeteil) */
float kampf_gesamt[2];                  /* entspricht armeegesamt */

long last_fight;                        /* Datum des letzten Kampfes */

long ausruestung[3][EINHEITEN];          /* 0=SHIELD, 1=GARMENT, 2=SWORD in % */
int preis_aus[3] = { 30,10,40 };        /* Preise für einen Mann */
int preis_1[3];                         /* Preis für ein Prozent */
char aus_name[3][10] = {                /* Name der Ausrüstung */
    romstr360,
    romstr361,
    romstr362 };

int preis_einheiten[EINHEITEN]= {        /* Was ist der Preis für die Einheiten */
  15,              /* Inf */
  20,              /* cav */
  14,              /* Art */
  11,              /* Sco */
  25,              /* DRR */
  110,             /* Wiz */
  15 };            /* Arc */


            /* ------------ ARMEESTEUERUNG ---------------*/

ARMEE_STEUER steuer[GEGNER];
FLAG army_unten;                /* Wird jetzt die erste Hälfte bewegt? */

/*-----------------------------------------------------------------------*/
/*                                RASSEN                                 */
/*-----------------------------------------------------------------------*/

char  rassename[RASSEN+1][9] = { romstr363,
                                 romstr364,
                                 romstr365,
                                 romstr366,
                                 romstr367,
                                 romstr368,
                                 romstr369,
                                 romstr370,
                                 romstr371,
                                 romstr372,
                                 romstr373 };

int  rassen_eigen[RASSEN][EIGENSCHAFTEN+2] = {  
	/* NORM_EIGANSCHAFTEN +  BEIDE ALTER
                                     WERTE WERDEN SPAETER IN HF_RASTER
                                     KOPIERT */

/*   (% !!)     STR FIG END QUI IQ  AGE1 AGE2      */
               { 50, 50, 50, 50, 50, 16, 75 },            /* Human */
               { 40, 37, 28, 75, 70, 16, 66 },            /* Elf */
               { 64, 67, 81, 22, 17, 12, 87 },            /* Dwarf */
               { 34, 24, 42, 92, 58, 8, 105 },            /* Halfling */
               { 79, 33, 75, 44, 19, 25, 63 },            /* Orc */
               { 16, 33, 26, 89, 86, 5, 134 },            /* Gnome */
               { 99, 27, 91, 19, 14, 12, 78 },            /* Giant */
               { 92, 32, 87, 21, 18, 20, 55 },            /* Troll */
               { 85, 39, 79, 31, 16, 16, 98 },            /* Ocre */
               { 18, 40, 32, 81, 79, 12, 111} };          /* Zwark */


/*--------------------------------------------------------------------------*/
/*                    DIMENSIONIERUNGEN FUER STÄDTE                         */
/*--------------------------------------------------------------------------*/

extern RGB_PAL scrollerp;
extern RGB_PAL kartep;            /* definiert in IO.C */
extern RGB_PAL leistep;
extern RGB_PAL formularp;

int citynum;                    /* Nummer der Stadt, in der Spieler gerade ist */

int city_nr[LAENDER][4] = {     /* welche Nummer hat CITY_1,.. absolut ? */
  { 0,-1,-1,-1 },                 /* Land 1 */
  { 1,2,3,-1 },                   /* B */
  { 4,-1,-1,-1 },
  { -1,-1,-1,-1 },
  { 5,-1,-1,-1 },
  { 6,7,8,9 },                    /* F */
  { 10,-1,-1,-1 },
  { 11,12,-1,-1 },
  { 13,14,15,-1 },                /* Insel I */
  { 16,-1,-1,-1 },
  { 17,-1,-1,-1 },
  { 18,-1,-1,-1 },                /* L */
  { 19,20,-1,-1 },                /* Insel M */
  { 21,-1,-1,-1 },
  { 22,-1,-1,-1 },                /* O */
  { 23,24,25,-1 },
  { 26,-1,-1,-1 },
  { 27,-1,-1,-1 } };

FLAG port_city[CITIES] = {     /* Ist Stadt eine Hafenstadt ? */
	TRUE,
	TRUE,FALSE,FALSE,
	TRUE,
	
	FALSE,
	FALSE,FALSE,FALSE,FALSE,
	TRUE,
	TRUE,TRUE,
	TRUE,FALSE,FALSE,
	TRUE,
	FALSE,
	FALSE,
	TRUE,TRUE,
	FALSE,
	TRUE,
	TRUE,FALSE,FALSE,
	FALSE,											/* 16:=(13=N) */
	TRUE };											/* 17:=(6=G) */
	
char c_name[CITIES][22] = {
		romstr374,
		romstr375,
    romstr376,
		romstr377,
		romstr378,
		romstr379,
		romstr380,
		romstr381,
		romstr382,
		romstr383,
		romstr384,
		romstr385,
    romstr386,
		romstr387,
		romstr388,
		romstr389,
		romstr390,
		romstr391,
		romstr392,
		romstr393,
		romstr394,
		romstr395,
    romstr396,
		romstr397,
		romstr398,
		romstr399,
		romstr400,
		romstr401 };
		

int belong[CITIES]; /* -1 = gehoert dir, ansonsten Armeenummer */

int people[CITIES] = {
	2200,
	7000,1900,2300,
	1700,
	
	3500,
	3600,3300,3700,4000,
	8000,
	6000,6200,
	1300,1500,1700,
	8300,
	2200,
	2000,
	2100,2400,
	4300,
	3000,
	7500,3500,2500,
	3400,
	7900 };
	
unsigned char produkt[CITIES][5] = {         /* Wer produziert was? */
    { 12,0,0,0,0 },             /* A0 */
    { 11,0,0,0,0 },
    { 10,0,0,0,0 },             /* B0 */
    { 15,0,0,0,0 },
    { 14,0,0,0,0 },
    { 10,0,0,0,0 },
    { 10,8,0,0,0 },             /* C0 */
    { 1,2,0,0,0 },              /* E0 */
    { 4,0,0,0,0 },
    { 5,0,0,0,0 },
    { 3,0,0,0,0 },
    { 9,0,0,0,0 },              /* F0 */
    { 23,0,0,0,0 },
    { 17,0,0,0,0 },
    { 16,0,0,0,0 },
    { 24,23,27,13,0 },           /* G0 */
    { 23,27,0,0,0 },
    { 10,0,0,0,0 },             /* H0 */
    { 23,0,0,0,0 },
    { 23,24,19,0,0 },
    { 23,0,0,0,0 },
    { 23,0,0,0,0 },             /* i0 */
    { 23,0,0,0,0 },
    { 6,25,0,0,0 },
    { 23,27,0,0,0 },            /* j0 */
    { 7,23,0,0,0 },
    { 27,1,2,3,0 },             /* k0 */
#if 0
    { 26,24,27,4,5 },
    { 25,18,0,0,0 },
    { 1,2,3,27,0 },             /* l0 */
    { 25,4,5,20,0 },
    { 1,2,3,5,0 },              /* m0 */ 
#endif
    { 4,26,27,25,0 } };

long bevoelkerung[CITIES];              /* tatsächlicher Wert, simuliert */

long stadt_besatzung[CITIES][RASSEN];       /* Welche Leute kann man kaufen */

int nachwuchs[RASSEN]= {
  5,1,3,1,3,2,3,3,2,1 };    /* wieviel Soldaten/Monat produziert Stadt */
long stadt_armee[CITIES][EINHEITEN];  /* Wieviel Soldaten sind in der Stadt */
int stadt_training[CITIES][EINHEITEN][EIGENSCHAFTEN];   /* Trainingsstand der Kaserne */
long stadt_aus[CITIES][3][EINHEITEN];               /* Ausrüstung der Kaserne */
unsigned char adelay[CITIES][EINHEITEN];   /* Zeitverzögerung beim Angriff */

FLAG bank_attacked[CITIES];
long city_bank[CITIES];         /* Alle Konten einer Stadt, ATTACK */
long bank_zuletzt;              /* Wann wurde Bank zuletzt betreten */
long zinsen;                    /* Welche Zinsen wurden fällig */
FLAG schulden;                  /* Wenn True, kann Spieler nichts mehr kaufen */

char pub_name[21];              /* Name des Pubs der Stadt */

int armee_city;                /* Stadt, in der Armee steht */
char port_pub[30];                  /* Name der Hafenkneipe der Stadt */

long waren_menge[CITIES][WAREN+STALLWAREN];     /* Wieviel Waren+Stall sind in der Stadt ? */
unsigned int waren_preis[WAREN+STALLWAREN+ROHSTOFFE];   /* Preis der Waren+Stall in DAr Stadt */

int crew_grenze[CITIES];            /* Ab diesem Preis sind Schiffsbesatz. interessiert */

long ship_price[CITIES][SCHIFFSTYPEN];       /* Baupreis je nach Stadt */
long ship_fertig[CITIES][5];        /* 5 Schiffe pro Hafen, Abgabedatum */
int ship_art[CITIES][5];            /* Welches Schiff im Bau  -1=leer */

/* ---------------------------------------------------------------------------
                            TRANSPORTMITTEL
 --------------------------------------------------------------------------*/

char ship_name[SCHIFFSTYPEN][12] = {
                            romstr402,
                            romstr403,
                            romstr404,
                            romstr405,
                            romstr406,
                            romstr407 };

int ship_crew[SCHIFFSTYPEN] = { 15, 30, 60, 15, 30, 60 }; 		/* wieviel Leute benötigt */
int ship_cannons[SCHIFFSTYPEN] = { 0, 0, 1, 4, 10, 20 }; 		/* Kannonen je Typ */
int ship_spec[SCHIFFSTYPEN] = { 400, 1000, 2000, 100, 500, 200 };     /* Cargo/Capac */

int ship_own[SCHIFFSTYPEN];				/* Typen im Besitz */
SCHIFF flotte[SCHIFFMAX];               /* gesamte Flotte */

int schiffbesitz;                       /* Wieviel besitzt Spieler? */
int schiffbau;                          /* Wieviel sind jetzt im Bau? */

/*--------------------------------------------------------------------------*/
/*                                 WAREN                                    */
/*--------------------------------------------------------------------------*/

long  money;              /* 1000 Taler Startkapital */

unsigned int aktie_preis[WAREN];       	/* Was kostet die Aktie durchschnittlich */
long aktie_x[WAREN];           		/* Wo steht die Aktie im Grafen */
long aktie_da[WAREN];          		/* Wieviel gibt's noch? */

char waren_name[WAREN+STALLWAREN][11] = {
    romstr408,							/* 1 */
    romstr409,														
    romstr410,							/* 3 */
    romstr411,
    romstr412,						/* 5 */
    romstr413,
		romstr414,							/* 7 */
    romstr415,
    romstr416,						/* 9 */
    romstr417,
		romstr418,					/* 11 */
		romstr419,
    romstr420,						/* 13 */
    romstr421,
		romstr422,							/* 15 */
		romstr423,
		romstr424,						/* 17 */
    romstr425,
    romstr426,						/* 19 */
    romstr427,

    romstr428,
    romstr429 	};

int prod_port[WAREN+STALLWAREN]={         /* Produktionszahlen der Waren/Monat/3000 Leute */
  /* MIL,KÄS,SAL,ÄPF,FLE,FIS,WAF,CRA,ACI,ARZ,CYC,KRI,SKL,FÖR,PUM,ÖLT,CYB,AND,COM,CHI,KER,GLE */
      20, 18, 25, 25, 24, 15, 10, 12, 16, 20, 25,  1, 15,  1,  3,  1, 17, 21, 12, 30, 14, 2 }; 

int prod_land[WAREN+STALLWAREN] = {        /* produktionsfaktoren bei !port_cities */
      40, 35, 42, 32, 27, 30, 12,  4,  5,  9, 12,  2, 19,  1,  1,  1, 8,  9,  4,  20, 20, 4 };    

char metal_name[ROHSTOFFE][11]={
                     				 romstr430,
										           romstr431,
                               romstr432,
                               romstr433,
														 romstr434,
														 romstr435,
														 romstr436,
                               romstr437  };

long ycargo_menge[WAREN];
long ymetal[ROHSTOFFE];         /* Menge an Rohstoffen */
long ystable_menge[STALLWAREN];

/*--------------------------------------------------------------------------*/
/*                       JAHRES & TAGESZEITEN                               */
/*--------------------------------------------------------------------------*/

int  year;
int  month;     /* Januar  */
int  day;       /* JE 30 TAGE */        /* 1. Januar 1432 */
char date[30];      /* nimmt Datum für List_Info auf */
long heute;         /* heutiges Datum in Summenform: Jahre*360+Mon*30+tage */
long startdatum;    /* Datum bei Spielbeginn */
long city_last[CITIES];                 /* Datum, des letzten Stadtbesuchs */

char month_name[13][11]={ romstr438, /* Monate beginnen bei 1 !! */
                          romstr439,
                          romstr440,
                          romstr441,
                          romstr442,
                          romstr443,
                          romstr444,
                          romstr445,
                          romstr446,
                          romstr447,
                          romstr448,
                          romstr449,
                          romstr450  };

/* --------------------------------------------------------------------
                        Landschaften
   ----------------------------------------------------------------- */

int startwert;								/* Bei jedem Spielstart anders [0..4] */

uint8_t ground_buf[1800];  /* Nimmt den Untergrund des momentanes Bildes auf */

int ground_nr;          /* Anzahl der Untergrundänderungen */
BODEN_ANDERS new_ground[CHG_MAX];        /* Ändert sich irgendwo der Unter-
            grund, wird z.B. ein Schatz gehoben, so wird hier die
            Änderung eingetragen und bei load_map umgesetzt */

char ground_name[32+1][22] = {
						   romstr451,							/* Schatz */
               romstr452,							/* Eisen */
               romstr453,							/* Dias */
               romstr454,							/* Nithril */
							 romstr455,							/* Öl */
							 romstr456,							/* Gas */
						   romstr457,							/* Gimmig */
               romstr458,							/* Gold */
               romstr459,
               romstr460,
               romstr461,
               romstr462,
               romstr463,
               romstr464,
							 romstr465,
							 romstr466,
							 romstr467,
               romstr468,
							 romstr469,
							 romstr470,
               romstr471,
               romstr472,
               romstr473,
               romstr474,
							 romstr475,									/* Bunker 1 */
							 romstr476,									/* Bunker 2 */
							 romstr477,
							 romstr478,
							 romstr479,
               romstr480,
               romstr481,
               romstr482,
               romstr483 };

/* in welcher Zeile startet Schild 0 in dem entsprechenden Land? */
unsigned char schild_nr[LAENDER] = {
		0,2,4,6,7,0,9,11,13,0,14,0,0,0,15,18,0,19 };
		
FLAG auf_schiff;            /* Fährt Spieler gerade mit Schiff? */
FLAG auf_schild;						/* Ist Spieler gerade auf einem Schild? */

short loc;                  /* CITY oder LAND, je nach Aufenthaltsort */

void *trf_buf;     					/* nimmt die Verkehrsrouten auf=Pack_buf */

int  ground_num;             	/* Untergrund des Spielers */

int pos_welt;       /* Welchen Index in weltarray Spieler hat */
int map;            /* = welt[pos_welt] */
char pos_string[25];        /* Hier wird Position aufgebaut */

unsigned char welt[15*15];          /* Gibt an welches Land, welche Nummer hat */

PEILGERAET peilgeraet[2];       /* Standort der 2 Peilgeräte */

/* --------------------------------------------------------------------
                            KAMPF
   -------------------------------------------------------------------- */

  /* Dieses Array beschreibt die Animationsbilder für die Kampfsprites
     z.B. Kavallry 11,12,13 und zurück */

int animation[] = {
  /* Eigene Armee: */
 -1,-1,
 7,8,                        /* Infantry */
 9,10,
 11,13,                       /* Cavallry */
 14,15,                       /* Kanone */
 16,17,
 18,20,
 21,22,
 23,24,
 25,27,
 28,29,
 30,31,
 32,34,
 35,36,
 37,38,
 39 };

/* Startshape jedes Sprites */
int ani_start[] = {
  7,11,14,7,18,9,7,37,
  21,25,29,21,32,23,21,37 };

/* Koordinaten, die die Einheiten einnehmen, wenn sie nicht im Kampf sind. */

int deckung_kampf[] = {                 /* Deckung bei Kampf Armee<->Armee */
 /* Spieler: */
  27,39,                    /* Inf */
  39,43,                    /* Cav */
  52,38,                    /* Art */
  66,43,                    /* pio */
  49,10,                     /* DRR */
  12,44,                     /* Wizra */
  80,39,                    /* Archer */
  11,35,                     /* Zelt */

 /* Gegner: */
  293,39,
  281,43,
  268,38,
  254,43,
  271,10,
  309,44,
  240,39,
  309,35 };

int deckung_city[] = {              /* Deckung bei Angriff gegen Stadt */
 /* Spieler: */
  27,39,                    /* Inf */
  39,43,                    /* Cav */
  52,38,                    /* Art */
  66,43,                    /* pio */
  49,10,                     /* DRR */
  12,44,                     /* Wizra */
  80,39,                    /* Archer */
  11,35,                     /* Zelt */

 /* Gegner: */
  242,39,
  230,43,
  217,38,
  203,43,
  220,10,
  258,44,
  189,39,
  258,35 };

int deckung[16*2];          /* Wirklicher Wert, während der Kampfroutine */

int geschw[EINHEITEN] = {                   /* Wie schnell sind die Einheiten (in Pixel) */
  1,3,0,2,4,1,1 };

int front_s,front_c;        /* Fronten der beiden Armeen */
int schild_einheit;         /* Die Einheit, die gerade am Schild ist */
FLAG ari_kaempft[2];        /* Flag, ob Ari's bereits schießen oder nicht */
FLAG demo_angriff;          /* wird TRUE, wenn Spielerstadt angegriffen wird */
int stadt_nr;               /* wenn demo_angriff, dann hier Stadt_nr */
float effektivitaet;        /* Wie schnell sterben die Leute (PLUS/MINUS) */
FLAG new_effekt;                   /* Neue Effektivitaet */
FLAG effekt2;                     /* Schon auf 2. Screen? */

int motiv[2];               /* Mativation in der Kampfroutine */
int gewinner;               /* 0=Spieler, 1=Gegner */

int oberkante;              /* Bis wohin ist die Kanone voll? */
int fuellung[12];           /* Was ist in der Kanone drin? [0]=ganz unten */
int fuell_poi;              /* Pointer zur nächsten Füllposition */
int kanone;                   /* STellung der Kanone 0..3 */
KOORD zuendpos[4] = {
  { 153,130 },
  { 153,125 },
  { 153,121 },
  { 153,119 } };


