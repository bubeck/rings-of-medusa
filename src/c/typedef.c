

/* Hier werden die gesamten neuen Datentypen von MEDUSA II definiert */
/* Till Bubeck,Ziegeleistr. 28,7056 Weinstadt    07151-66437 */

typedef signed char FLAG;

typedef struct {            /* Beschreibt die Steuerung der Gegner */
    int heimat;             /* Heimatstadt */
    int angriff_zahl;       /* Zahl der Angriffe, die Spieler schon durchgeführt hat */
    unsigned int wait;      /* Zähler, wenn Armee an einer Kreuzung wartet */
    int weltpos;            /* Position der Armee in der Weltkarte */
    int x,y;                /* X,Y Position der Armee */
    int sprite;             /* Nr des Sprites (0..SPR_MAX) */
    int vx,vy;              /* X/Y Bewegungsvektor */
    int akt_weg;            /* Wegnr, auf der Armee gerade ist */
    FLAG auf_wasser;				/* Flag, ob Armee im Schiff unterwegs ist */
    } ARMEE_STEUER;

typedef struct {
    int typ;                /* Schiffstyp: 0..SCHIFFSTYPEN ,
                               -1: Schiffeintrag ist frei */
    int aboard;             /* Wieviel Leute sind an Bord? */
    long salary;            /* Welche Kosten verursacht das Schiff? */
    long prod_datum;        /* Wann wurde das Schiff in Betrieb genommen? */
    int ort;                /* Wo ist Schiff jetzt?, >=0 bedeutet in entsp. Stadt,
                               =-1 bedeutet beim Spieler beim Rumziehen */
    } SCHIFF;

typedef struct {
  unsigned char rohstoff;               /* Welcher Rohstoff wird gefördert */
  long datum;                /* Wann hat die Mine geöffnet */
  long ges_menge;           /* Wieviel bisher produziert */
  unsigned int turm_zahl;            /* Anzahl an Fördertürmen */
  unsigned int filter_zahl;          /* -"- an Filtern */
	unsigned int silo_zahl;			/* Anzahl an Ölturmen */
  long im_lager;            /* Wieviel ist noch im Lager */
  long zuletzt;             /* Wann war Spieler zuletzt da? */
  long vorkommen;              /* Wieviel ist noch in der Mine drin */
  int land;                 /* In welcher Landschaft */
  int x,y;                  /* An welcher Position */
  } MINE;

typedef struct {
  char untergrund;           /* Neuer Untergrund an der Stelle */
  char land;                 /* Land =welt[pos_welt] */
  char x,y;                  /* Position in Rasterkoordinaten */
  } BODEN_ANDERS;

typedef struct {
  int x,y;                  /* Koordinatenpaar */
  } KOORD;

typedef struct {
  int x,y;                  /* Koordinaten des Mittelpunktes */
  int kante;                /* Kantenlänge */
  } QUADRAT;

typedef struct {
  FLAG on;                  /* Gerät eingeschaltet? */
  int x,y;                  /* Standort des Peilgerätes */
  KOORD ziel;               /* aktuelle Zielkoordinate des Randes */
  int dx,dy;                /* Delta x/y */
  KOORD dest_1,dest_2;      /* Lösch-Zielpunkte der Screens (x negativ->ungültig) */
  KOORD schatz;             /* Koordinaten des Randpunktes, wo Strahl blitzt */
  int umdrehungen;          /* Anzahl der Umdrehungen, die Gerät schon gemacht */
  } PEILGERAET;

enum objekt_modus { MOVE=0,ODER,PALETTE };
enum TUER_STATUS { AUF=0,ZU };

typedef struct {
  int x1;										/* Linker Rand des Objekts auf Screen */
  unsigned char y;					/* Y-Position auf Screen */
  int x2;										/* Rechter Rand des Objekts auf Screen */
  unsigned char tiefe;			/* Tiefe des Objekts auf dem Bildschirm 0=ganz vorne */
  unsigned char ob_nr;			/* Nummer des zugehörigen Objekts */
  unsigned char ob_x;				/* X-Koordinate des linken Randes innerhalb des Objektes */
	FLAG knuepfen;						/* Ist Objekt transparent, d.h. keine Wand ... */
	unsigned char modus;			/* Bestimmt den Modus bei obj_draw */
  } BUNKERWAND;

typedef struct {
	int sprite[32];						/* Nimmt einen Sprite auf (ungeshiftet) */
	} SPRITE;

typedef struct {						/* Einsatz beim Roulettespiel */
	int feld;									/* Feld (0..36) bzw. (Manque,Impair,Rot,Passe,Pair,Schwarz) (37..42)
														   waagrechte Spalten (43,44,45) */
	int chip;									/* Welcher Chip wurde gesetzt? (0..8) */
	} EINSATZ;

typedef struct {
	unsigned char typ;					/* Welche Funktion hat Item? */
	unsigned char grafik;				/* Welche Grafiknummer? */
	unsigned char bun_nr;				/* in welchem Bunker ist Item */
	unsigned char bun_x;				/* auf welchem Feld */
	unsigned char bun_y;				/* -"- */
	unsigned char pos;					/* 0=NO, 1=SO, 2=SW, 3=NW in welcher Ecke? */
	unsigned int flag;					/* Special Function */
	} ITEMS;

typedef struct {							/* Gegnerstruktur */
	unsigned char bun_nr;				/* in welchem Bunker */
	unsigned char x;						/* auf welchem Feld */
	unsigned char y;						/* -"- */
	unsigned char grafik;				/* Welche Grafiknummer? */
	unsigned int richtung:3;		/* 0=N, 1=NO, 2=O, ... */
	signed int fight:1;					/* Flag, in den Kampf verwickelt */
	unsigned int speed:8;				/* Geschwindigkeit in VBL's */
	unsigned int next_move:8;		/* Wann soll als nächstes bewegt werden? */
	unsigned int staerke:8;			/* Staerke des Gegners 0=Tot */
	} ENEMY;

typedef struct {
	void *ob_mem;								/* Nimmt Zeiger auf NEOchrome Objekte auf (NULL=unbenutzt) */
	int ob_nr;									/* Objektnummer des Schusses */
	int dx,dy;			  					/* Schußrichtung */
	int x,y;										/* Startposition des Schusses */
	int entfernung;							/* Entfernung des Schusses vom Startpunkt in 10-tel Feldern */
	int gegner;									/* Nummer des zugehörigen Feindes */
	long last_vbl;							/* VBL der letzten Animationsstufe */
	int speed;									/* Geschwindigkeit in VBL's pro Schritt */
	int x1,y1,x2,y2;						/* Position auf Screen x1=-1 -> kein Schuss da */
	} SHOT;
	
typedef struct {
	unsigned char typ;					/* Welchen Typ hat Objekt? */
	unsigned char grafik;				/* Welche Grafikobjektnummer? */
	unsigned int spezial;
	unsigned int verteilung;		/* Wie oft soll Objekt vorkommen? Prozent*100 */
	} OBJEKT_DISK;

typedef struct {							/* Struktur von BUNKER.DAT */
	unsigned int felder;				/* Wieviele Felder hat dieser Dungeon */
	unsigned int frei;					/* Wieviele Felder sind LEER */
	} STATISTIK;
	
typedef struct {							/* Welche Veränderung ist noch nicht auf 2. Screen */
	unsigned char x,y;
	unsigned char wert;
	} AUTOMAP2;

typedef struct {							/* Member der Party */
	unsigned char prisoner_nr;		/* Welchen Prisoner haben wir hier? */
	unsigned char reagenz[3];		/* (Hunger,Durst,Gesundheit) 0=Schwach, 255=Stark */
	int tasche[8];							/* Welche Items tragen sie mit sich rum, -1=Platz frei */
	int hand[2];								/* Was hat er in der Hand? */
	int helm;						
	int oberteil;								/* Was hat er an? -1=Nichts, 0,.. */
	int unterteil;
	int schuhe;
	unsigned char protection;		/* 0..100 Schutzsumme 0=Schwach */
	FLAG schiessen;							/* Soll die Person schiessen? */
	} PARTY_MEMBER;

typedef struct {							/* Zur Lagebestimmung der Objekte in den Ecken des Bunkers */
	int x;											/* Welche x/y-Position auf Screen hat Ecke ? */
	unsigned char y;
	unsigned char tiefe;				/* und welche Tiefe? */
	} LAGE;

typedef struct {							/* Dient zur Bestimmung, welches Bodenstück im Bunker, welche */
	unsigned char feld;					/* item[].pos, item[].x, item[].y repräsentiert */
	signed char distance;				/* Entfernung zur Spielerposition */
	} BUNKERLAGE;

typedef unsigned int RGB_PAL[16];	/* Farbpalette */

/* Wie soll bei move_objekt vorgegangen werden? 
	 STOP=an der entsprechenden Kante stoppen,
	 ENDE=bei Erreichen der Kante move_objekt abbrechen */
enum clip_modus { STOP_L=1, STOP_R=2, STOP_U=4, STOP_O=8,
									ENDE_R=16, ENDE_L=32, ENDE_U=64, ENDE_O=128 };

enum item_typen { KEY=1, FOOD, WAFFE, HELM, HEMD, HOSE, SCHUHE, SCHATZTRUHE, WATER, HILFE,
									MIRROR, PERGAMENT, MUNITION, SONSTIGE, LEERE_TRUHE };	
									
typedef struct {
	char name[8];
	unsigned char staerke;				/* Gesundheitszustand des Gefangenen */
	FLAG befreit;
	} PRISONER;

