
// Should enemies be displayed in dungeons on the automap?
#define SHOW_ENEMY_ON_AUTOMAP 1

#define DIGI_COUNT 100

#define TIMEOUT 25000                 /* Timeout für Kopierschutz */

#define LADEN 0            /* verschiedene Zustände für ort in buy_sell() */
#define JUWELLIER 1
#define AKTIEN 2

#define MUSIK_AN 1
#define MUSIK_AUS 2                /* 2 Zustände für Hippelspielmusik */

enum hippel_effekte { LASER=0, FLAK, AUFZUG_AN, AUFZUG_AUS, TUER_AN, TUER_AUS, ECHOLOT,
											ROULETTE_AN, ROULETTE_AUS, PEILSTRAHL, FX_AUS, FX_11, FX_12 };

#define VERSION 10

#define MAUS_WEG 1
#define MAUS_DA 0       /* für bstate */

#define TRUE -1
#define FALSE 0

#define PI 3.14159265

#define RAUS 3											/* Buttonnummer wird benutzt, um button_leiste zu verlassen */
#define NOTHING 0
#define EXIT_BTN 1L
#define PEILUNG 2L
#define ATTACK 4L
#define FLEE 8L
#define TERMS 16L
#define INFO_BTN 32L                /* Nummer des Buttons */
#define BUY 64L
#define SELL 128L
#define YES 256L
#define NO 512L
#define SEARCH 1024L
#define DIG 2048L
#define LOAD 4096L
#define SAVE 8192L
#define EDITOR 16384L
#define ALL 32768L
#define PAUSE 65536L
#define PLUS 131072L
#define MINUS 262144L

#define LAND 0
#define CITY 1

#define LAENDER 18
#define SEA LAENDER                 /* Nummer des Meers, in der Karte */
#define BUNKERZAHL 44					 /* 44 Bunker im Spiel */
#define GEGNER 15                   /* Anzahl der gegerischen Armeen */
#define ARMEEN (GEGNER+1)           /* Gesamtzahl der Armeen (also mit Spieler) */
#define CITIES 28                   /* 28 Städte im Spiel */
#define WAREN 20                    /* 20 Waren zum Handeln */
#define WAREN_SEITEN (WAREN/10)     /* 10 pro Seite */
#define RASSEN 10                   /* 10 Rassen zur Verfügung */
#define SCHILDE 6
#define RUESTUNGEN 8
#define WAFFEN 12
#define EIGENSCHAFTEN 5             /* Anzahl der Eigenschaften einer Armee */
#define ROHSTOFFE 8
#define SCHIFFSTYPEN 6
#define SCHIFFMAX 30      /* Spieler kann maximal 30 Schiffe besitzen */
#define MAX_PRICE 995           /* maximal 995 für ein Crewmitglied */
#define BERUFE 10
#define STALLWAREN 2         /* Warenzahl im Zahl */
#define SPR_MAX 25           /* maximal X Sprites GLEICHZEITIG darstellen */
#define SPR_ZAHL 68          /* ANzahl der verschiedenen Spritebilder */
#define EINHEITEN 7          /* Anzahl der verschiedenen Armee ~ */
#define MINE_MAX 20         /* Max. 20 Minen gleichzeitig */
#define CHG_MAX 400          /* Max. 400 Untergrundsänderungen */
#define KAP_KARREN 80       /* Karren nimmt 80 Waren auf */

#define FILES 45            /* 18 Bilderfiles */
#define TIEFE 5             /* für Armeesteuer, max. 5 Verschachtelungen */

#define MAUS_AN 10          /* TRAP #10 zeichnet Maus auf Bildschirm */
#define MAUS_AUS 11         /* Trap #11 löscht sie wieder weg */

#define AN_BORD 1           /* für "standort", für die Armee */
#define AN_LAND 2           /* -"- */
#define NIRGENDS 3          /* Armee nicht vorhanden */

#define ANGRIFF 1
#define RUECKZUG 2          /* Befehle für die Einheiten beim Kampf */
#define HALTEN 0            /* verwendet in auftrag[] */

#define PARTY 4							/* Anzahl der Leute in einer Party */

enum himmelsrichtung { NORD=0,OST,SUED,WEST };

#define WANDMAX 400					/* max. Anzahl an Wandeinträger in BUNKERWAND[] */
#define TUERMAX 140					/* Gesamtzahl der Türen in allen Bunkern */
#define ITEMMAX (TUERMAX+570)	/* Gesamtzahl an Objekten in allen Bunkern (Schlüssel+Items) */
#define ENEMYMAX 247				/* Gesamtzahl der Gegner */
#define ITEMTYPEN 33				/* Wieviel verschiendene Items gibt es? */

#define START_POS 65534				/* Startposition */
#define WALL 65535				
#define HIDDEN_WALL 65533			/* Wand, die begehbar ist */
#define LEER 0 
#define DOOR_ZU_START 32768		/* geschlossene Türen haben Feldnummer von hier ... */
#define DOOR_ZU_END 35999			/* ... bis hier. */
#define DOOR_AUF_START 36000		/* offene Türen von hier ... */
#define DOOR_AUF_END 38000		/* ....bis hier. */
#define ITEM_START 1					/* In der Datei steht, welches Objekt an welcher Stelle */
#define NOKEY_START 10000			/* Ab hier kommen keine Schlüssel mehr */
#define NOKEY_END 15000
#define ITEM_END 15000				/* ist. (Nur ein Objekt pro Feld) */
#define PRISONER_START (49152)	
#define PRISONER_END (49152+10)
#define LIFT_START 16384			/* Start der Aufzugsdaten */
#define LIFT_END	(16384+4096)	/* Ende der Aufzugsdaten */

#define LAMPZAHL 3						/* Zwei verschiedene Lampentypen (Grau und Blau) */

enum statuse { KAEMPFER=0, MAGIER, MOGUL, ZAUBERMEISTER };		/* Spielerstaus */

/* Scan Codes for various Atari keys on keyboard: */
#define ATARI_SCAN_HELP 0x62
#define ATARI_SCAN_UP 0x48
#define ATARI_SCAN_DOWN 0x50
#define ATARI_SCAN_LEFT 0x4b
#define ATARI_SCAN_RIGHT 0x4d
