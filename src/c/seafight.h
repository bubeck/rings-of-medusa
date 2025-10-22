
#define Y0 17		/* Größe der oberen Leiste */


/* Der Einfachheit halber benutzen wir Zylinderkoordinaten */

typedef struct
{
	long R,W,Z;		/* R=Abstand, W=Winkel, H=Höhe */
} ZKOORD;

typedef struct
{
	long X,Y,Z;		/* Dasselbe in X/Y/Z */
} KKOORD;


/* Schüsse */

enum State { S_IDLE,S_TURMDREH,S_FLUG,S_TREFFER,S_KEINTREFFER };

typedef struct
{
	ZKOORD		Pos;			/* Aktuelle Position */
	long		T0;				/* Abschusszeit in BLANKs */
	long		T;				/* Vergangene Zeit seit dem Abschuss */
	int			Winkel;			/* Abschusswinkel in Grad */
	int			V0;				/* Abschussgeschwindigkeit [m/blank] */
	float		V0X;			/* Abschussgeschwindigkeit [m/blank] in X-Richtung */
	float		Tan;			/* tan(Winkel) */
	float		Const;			/* g/2 * v0^2 * cos^2(Winkel) */
	enum State	State;			/* Was macht der Schuss denn gerade */
	int			AnimSeq;		/* Animationszähler */
} SCHUSS;

#define O_KUGEL			99	/* Kanonenkugel (groß -> klein) */
#define NUM_KUGEL		9	/* Anzahl Zoomstufen der Kugel */
#define O_FEUERBALL		93	/* Start der Explosionssequenz */
#define NUM_FEUERBALL	7	/* Anzahl Bilder der Explosionssequenz */
#define O_FONTAENE		108	/* Schuss fällt ins Wasser */
#define NUM_FONTAENE	7	/* Anzahl Bilder der Animationsssequenz */
#define SCHUSSDELAY		40	/* Anzahl Frames zwischen den Schüssen */
#define TREFFERRADIUS	30	/* Treffer innerhalb von 15 m */
#define O_TURMDREH		27	/* Feindlicher Geschützturm dreht sich */
#define NUM_TURMDREH	8	/* Anzahl Bilder der Sequenz */

/* Schiffe */

typedef struct
{
	ZKOORD	Pos;			/* Aktuelle Position */
	int		Typ;			/* O_SCHIFFirgendwas */
	int		Turmwinkel;		/* Winkel des Geschützturms */
	FLAG		treffer;			/* Wenn, True -> dann sinken */
	int untergang;				/* Wie tief schon gesunken? */
	SCHUSS	Schuss;			/* Gegnerischer Schuss */
} GSCHIFF;					/* Den Typ SCHIFF gab's leider schon */

#define MAX_SCHIFFE		25	/* Maximale Anzahl Schiffe */
#define MAX_SCHUESSE	4	/* Maximale Anzahl gleichzeitiger Salven */

#define MIN_DIST		480L		/* Gegner minimal 600 Meter entfernt */
#define MAX_DIST		900L		/* Gegner maximal 1100 Meter entfernt */

#define HORIZONT		67		/* Meereshöhe */


/* Flugzeuge */

typedef struct
{
	ZKOORD	Pos;			/* Aktuelle Position */
	int		Ansq;			/* Animationssequenz */
	int		AnBase;			/* 1. Frame */
	int		Schuss;			/* Schuss-Flag */
	long 	nxt_move;			/* Wann soll nächste Animation kommen (vbl_ct) */
} FLUGZEUG;

#define MAX_FLUGZEUGE		8		/* Maximale Anzahl Flugzeuge */

#define O_FLUGZEUGFRONT		70		/* Waagerechter Angriff */
#define O_FLUGZEUGLINKS		77		/* Linkskurve */
#define O_FLUGZEUGRECHTS	82		/* Rechtskurve */


/* Objektnummern */

#define O_HINTERGRUND	0
#define O_ZIFFERN		1	/* Alle in einem Objekt */
#define O_KLINKS		2	/* mit 3 Stufen */
#define O_KRECHTS		5	/* mit 3 Stufen */

#define O_SCHIFFLINKS1	19
#define O_SCHIFFLINKS2	20
#define O_SCHIFFLINKS3	21
#define O_SCHIFFRECHTS1	22
#define O_SCHIFFRECHTS2	23
#define O_SCHIFFRECHTS3	24
#define O_SCHIFFVORNE	25
#define O_SCHIFFHINTEN	26


/* Kanonen und so */

#define	O_KANONE		10	/* mit NUM_KANONE Stufen */
#define NUM_KANONE		9

#define MIN_WINKEL		60	/* 20, Minimaler Schusswinkel */
#define MAX_WINKEL		71	/* 75, Maximaler Schusswinkel */

#define KAN1_X			127
#define KAN2_X			190
#define KAN_Y			116


/* Radar-Schirm */

#define RADAR_X			285		/* Mittelpunkt des Radars */
#define RADAR_Y			130
#define RADAR_RADIUS	11		/* Radius des Radars */


/* Rechen-Makros & Konstante */

#define ABS(x)				((x)<0? -(x):(x))
#define DEGTORAD(w)			((w)*(PI/180.0))
#define RADTODEG(w)			((w)*(180.0/PI))
#define G (9.80665/5000)		/* Normale Fallbeschleunigung in m/vbl */

