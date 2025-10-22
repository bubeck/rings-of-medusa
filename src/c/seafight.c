 
/* Seeschlacht-Routine - Copyright 1991 by Christian Weber, Zürich
	 erheblich erweitert und fehlerbereinigt von Till Bubeck, Weinstadt	*/
	 
#include "includes.c"           /* Definiert alle Variablen als Extern,... */
#include	<portab.h>

#include "seafight.h"

static void		*objects;				/* Objekt-Bibliothek */
static int		score;					/* Anzahl Treffer */
static int old_score;
static int		munition;
static int old_muni;						/* angezeigte Munition */
static int		schussrichtung;			/* Winkel (0..359 Grad) */
static int		rohrwinkel;				/* Winkel (MIN_WINKEL..MAX_WINKEL)*10 */
static int old_winkel;					/* angezeigter Schusswinkel */
static FLAG gefeuert;						/* Wurde gerade gefeuert? */
static long schussdelay;				/* Wann wurde gefeuert */
static int spielerschiff;				/* Anzahl der Spielerschiffe */

static GSCHIFF	schiff[MAX_SCHIFFE];	/* Gegner-Schiffe */
static int		numschiffe;				/* Anzahl Gegner-Schiffe wird mit jedem Treffer erniedert */
static int gegnerschiffe;				/* Anzahl Gegner-Schiffe */
static int		gegnerstaerke;			/* 0=schwach, 100=stark */
static int		gegner_score;			/* Punktzahl des Gegners */
static int old_gegner_score;
static int schiffschuesse;			/* Anzahl der Schuesse, die unterwegs sind */

static FLUGZEUG	flugzeug[MAX_FLUGZEUGE];	/* Gegner-Flugzeuge */
static int		numflugzeuge;				/* Anzahl Gegner-Flugzeuge */

static SCHUSS	schuss[MAX_SCHUESSE];	/* Schüsse */
static int		numschuesse;			/* Anzahl Schüsse in der Luft */

/****************************************************************************
**	Liefert eine Zufallszahl zwischen min (inkl.) und max (exkl.),
**	ist etwas cleverer als rnd(), welches bei schnell aufeinanderfolgenden
**	Aufrufen die gleichen Zahlen liefert
*/

static int range_rand(int min,int max)
{
	return (min+zufall(max-min));
}

/****************************************************************************
**	Winkel in 0..359 Grad eingrenzen
*/

static int winkel(int w)
{
	while(w<0)		w+=360;
	while(w>359)	w-=360;
	return w;
}

static FLAG winkel_differenz(int w1,int w2,int dif)
{
	/* Überprüft, ob sich w1 und w2 um max. dif unterscheiden */
	int differenz;
	
	differenz=ABS(w1-w2);
	return(differenz<dif || differenz>360-dif);
}

	
/****************************************************************************
**	Konvertiert Welt-Koordinaten(Zylinder) in Screen-Koordinaten(X/Y)
*/

static int convert_koord(ZKOORD *welt,KOORD *screen)
{
	double faktor	= pow((double)MAX_DIST-(double)welt->R,1.0045);

	if(welt->R<0) return 0;					/* Objekt ist hinter dem Betrachter ?! */

	screen->x = 160+(faktor*(winkel(schussrichtung-welt->W+22)-22))/44;
	screen->y = HORIZONT-faktor*(welt->Z-10)/(MAX_DIST/10);
	if((screen->y<Y0) || (screen->y>200)) return 0;

	return 1;
}

/****************************************************************************
**	Zeichnet ein Objekt, dabei wird der Nullpunkt des Objekts nicht oben
**	links, sondern unten in der Mitte angenommen.
*/

static void draw_obj2(int num,int mode,void *scr,int x,int y)
{
	int hoehe  = objekt_hoehe(num,objects);
	int breite = objekt_breite(num,objects);
	int x1,y1,x2,y2;									/* Koordinaten Auf Screen */
	int xo1,yo1,xo2,yo2;							/* sichtbarer Bereich des Objekts */
	
	x1=x-breite/2;								/* Linkes oberes Eck */
	y1=Y0+y-hoehe;
	x2=x1+breite-1;								/* Rechtes unteres Eck */
	y2=y1+hoehe-1;

	xo1=0;
	yo1=0;
	xo2=breite-1;									/* Gesamtes Objekt ist zu sehen */
	yo2=hoehe-1;
	
	if (x1<84) {
		xo1+=84-x1;
		x1=84;
		}
	if (x2>235) xo2-=x2-235;
	if (y2>132) yo2-=y2-132;
	
	if (xo1<=xo2 && yo1<=yo2) {			/* Überhaupt noch was sichtbar */
		draw_obj_part(num,objects,xo1,yo1,xo2,yo2,mode,scr,x1,y1);
		}
				
/*	draw_obj(num,objects,mode,scr,x-(breite/2),Y0+y-hoehe); */
}

/****************************************************************************
**	Zeichnet und verkleinert ein Objekt, dabei wird der Nullpunkt des Objekts
**	nicht oben links, sondern unten in der Mitte angenommen.
**  Dabei ist show_hoehe, die Höhe des unverkleinerten Objekts, die gezeichnet werden soll,
*/

static void draw_shrink_obj2(int zoom,int num,int mode,void *scr,int x,int y,int show_hoehe)
{
	int s_hoehe,s_breite;					/* Größe des verkleinerten Objekts */
	int hoehe,breite;							/* Größe des Original-Objekts */
	int x1,y1,x2,y2;							/* Koordinaten auf Screen */
	int xo1,yo1,xo2,yo2;					/* sichtbarer Bereich innerhalb des Objekts */
	int clip_weg;									/* Wieviel Pixel stehen über */
	double proz;									/* dasselbe Anteilsmäßig an Gesamtgröße [0..1] */	
	
	s_hoehe  = shrink_obj_hoehe(zoom,num,objects);
	s_breite = shrink_obj_breite(zoom,num,objects);
	hoehe  = objekt_hoehe(num,objects);
	breite = objekt_breite(num,objects);

	if (show_hoehe<hoehe) {					/* Nur einen Teil zeichnen */
		clip_weg=hoehe-show_hoehe;
		proz=(double)clip_weg/hoehe;
		hoehe=show_hoehe;
		s_hoehe-=s_hoehe*proz;
		}
		
	x1=x-s_breite/2;
	y1=Y0+y-s_hoehe;
	x2=x1+s_breite-1;								/* Koordinaten Shrink Obj auf Screen */
	y2=y1+s_hoehe-1;
	
	xo1=0;
	yo1=0;
	xo2=breite-1;										/* Volle Größe des Objekts */
	yo2=hoehe-1;
	
	if (x1<84) {
		clip_weg=84-x1;								/* Soviele Pixel stehen links über */
		proz=(double)clip_weg/s_breite;		/* [0..1] Soviel stehen anteilsmässig links über */
		xo1+=breite*proz;							/* Gleichen Anteil von Originalobjekt weg */
		x1=84;
		}
	if (x2>234) {
		clip_weg=x2-234;
		proz=(double)clip_weg/s_breite;
		xo2-=breite*proz;
		}
	if (y2>132) {
		clip_weg=y2-132;
		proz=(double)clip_weg/s_hoehe;
		yo2-=hoehe*proz;
		}	

	if (xo1<=xo2 && yo1<=yo2) {				/* Ist noch was sichtbar */
		draw_shrink_obj_part(zoom,hlpbuf,num,objects,xo1,yo1,xo2,yo2,mode,scr,x1,y1);
		}

/*	draw_shrink_obj(zoom,hlpbuf,num,objects,mode,scr,x-(breite/2),Y0+y-hoehe); */
/*	draw_obj(num,objects,mode,scr,x-(breite/2),Y0+y-hoehe); */
}

/****************************************************************************
**	Zeigt die vorhandene Munition an
*/

static void show_munition(int num)
{
	void *oldlogbase;
	char buf[20];
	int i;

	if (num!=old_muni) {							/* Munition hat sich geändert */

		old_muni=num;		
		strcpy(buf,str(8,num));
	
		for(i=7; i>=0; i--)
		{
			int x;
			
			if (buf[i]==' ') buf[i]='0';
	
			x=4*(buf[i]-'0');
			draw_obj_part(O_ZIFFERN,objects,x,0,x+3,6,MOVE,scr2,15+5*i,Y0+95);
		}
		
		cpy_raster(scr2,scr1,15,Y0+95,70,Y0+95+10,15,Y0+95);		/* Auf anderen Screen */
		}
		
	if (old_winkel!=rohrwinkel) {
		old_winkel=rohrwinkel;

		oldlogbase=logbase;
		logbase=scr2;
		
		writexy_fast(16,136,romstr572);
		writexy_fast(16,142,fstr(romstr573,rohrwinkel/10.0));
		logbase=oldlogbase; 

		cpy_raster(scr2,scr1,16,136,16+7*4,148,16,136);			/* Auf 2. Screen */
		}
}

/****************************************************************************
**	Zeigt die gegnerischen Schiffe an, falls sie sich im Blickfeld befinden
*/

static void show_schiffe(void)
{
	int i,j;
	int hoehe;
	
	for(i=0; i<numschiffe; ++i) {
		int zoom;
		KOORD koord;

		if(convert_koord(&schiff[i].Pos,&koord))	{				/* Schiff sichtbar ? */
			hoehe=objekt_hoehe(schiff[i].Typ,objects);
			if (schiff[i].treffer) {
				hoehe-=schiff[i].untergang;
				schiff[i].untergang++;
				}
			if (hoehe>=0) {											/* Schiff ist noch da */
				zoom = (130L*(schiff[i].Pos.R-MIN_DIST))/(MAX_DIST-MIN_DIST);
				draw_shrink_obj2(zoom,schiff[i].Typ,ODER,scr2,koord.x,koord.y,hoehe);
				}
			else {
				if (schiff[i].Schuss.State!=S_IDLE) schiffschuesse--;		/* Ein Schuss weniger */
				numschiffe--;
				for(j=i;j<numschiffe;j++) schiff[j]=schiff[j+1];		/* gesunkenes Schiff entfernen */
				}
			}
		}
}

/****************************************************************************
**	Zeigt die beiden Kanonen an, in der richtigen Schusshöhe
*/

static void show_kanonen(void)
{
	int ani=O_KANONE+((rohrwinkel/10.0-MIN_WINKEL)/(float)(MAX_WINKEL-MIN_WINKEL)*(NUM_KANONE));
	int y;
	
	/* Die beiden Kanonen */
	if (gefeuert) {
		gefeuert=FALSE;
		y=KAN_Y+10;
		}
	else y=KAN_Y;
	
	draw_obj2(ani,ODER,scr2,KAN1_X,y);
	draw_obj2(ani,ODER,scr2,KAN2_X,y);

	/* Und die beiden Hebel */
	ani = ((rohrwinkel/10-MIN_WINKEL-1)*3)/(MAX_WINKEL-MIN_WINKEL);
	draw_obj_part(O_HINTERGRUND,objects,120,125,137,135,MOVE,scr2,120,125+Y0);
	draw_obj(O_KLINKS+ani,objects,MOVE,scr2,120,17+135-objekt_hoehe(O_KLINKS+ani,objects));
	draw_obj_part(O_HINTERGRUND,objects,181,125,197,135,MOVE,scr2,181,125+Y0);
	draw_obj(O_KRECHTS+ani,objects,MOVE,scr2,181,17+135-objekt_hoehe(O_KRECHTS+ani,objects));
}

/****************************************************************************
**	Zeigt die gegnerischen Schiffe auf dem Radar an 
*/

static void show_radar(void)
{
	int i;

	for(i=0; i<numschiffe; ++i)
	{
		int len =	   ((schiff[i].Typ == O_SCHIFFLINKS1)
					 || (schiff[i].Typ == O_SCHIFFRECHTS1)) ? 0:1;

		double w = DEGTORAD((double)(schussrichtung-schiff[i].Pos.W-90));
		int    x = (int)(schiff[i].Pos.R*cos(w)*((double)(RADAR_RADIUS+10)/MAX_DIST));
		int    y = (int)(schiff[i].Pos.R*sin(w)*((double)RADAR_RADIUS/MAX_DIST));

		line(15,RADAR_X+x,(Y0+RADAR_Y)+y,RADAR_X+x+len,(Y0+RADAR_Y)+y);
	}
}

/****************************************************************************
**	Wählt zufällige Positionen für die gegnerischen Schiffe aus.
*/

static void set_schiffe(void)
{
	int i;

	for(i=0; i<numschiffe; ++i)
	{
		schiff[i].Pos.R	= range_rand(MIN_DIST,870);
		schiff[i].Pos.W	= range_rand(0,360);
		schiff[i].Pos.Z	= 0;
		schiff[i].Typ	= range_rand(O_SCHIFFLINKS1,O_SCHIFFHINTEN+1);
		schiff[i].treffer = FALSE;
		schiff[i].untergang=0;
		schiff[i].Schuss.State=S_IDLE;				/* Schuss frei */
	}
}

/****************************************************************************
**	Schuss auf seiner Bahn weiterbewegen
*/

static void schuss_bahn(SCHUSS *s)
{
	s->T		= vbl_ct-s->T0;					/* Schussdauer aktualisieren */
	s->Pos.R	= s->V0X*s->T;					/* Abstand aktualisieren */

	/* Höhe aktualisieren */
	s->Pos.Z	= (s->Tan*s->Pos.R-s->Const*(s->Pos.R*s->Pos.R))/10;
}

/****************************************************************************
**	Gegnerschiff-Schuss-Handling
*/

static void gegner_schiff_schuss(void)
{
	int i;

	/* Welches Schiff soll schiessen?: */

	if (schiffschuesse<numschiffe) 						/* Sind noch Schiffe ohne Schuß? */
		if(schiffschuesse<(1+gegnerstaerke/7))
		{
			if(range_rand(0,30) < gegnerstaerke)				/* 0,10000 */
			{
				int s_schiff;
				SCHUSS *s;
	
				do
				{
					s_schiff=range_rand(0,numschiffe);					/* Welches Schiff schießt? */
				} while(schiff[s_schiff].Schuss.State != S_IDLE);
	
				s=&schiff[s_schiff].Schuss;
	
				s->Pos.R	= 0;						/* Abstand */
				s->Pos.W	= schiff[s_schiff].Pos.W;	/* Schuss-Richtung */
				s->Pos.Z	= 0;						/* Schuss-Höhe */
				s->T0		= vbl_ct;					/* Abschusszeit */
				s->T		= 0;						/* Flugdauer */
				s->Winkel	= 450;					/* Abschusswinkel 45 Grad */
				s->V0		= range_rand(3,7);		/* 5 m/blank XXX variabel */
				s->V0X		= s->V0*cos(DEGTORAD(s->Winkel/10));
				s->Tan		= tan(DEGTORAD(s->Winkel/10));
				s->State	= S_FLUG;					/* S_Flug */
				s->AnimSeq	= O_KUGEL;
				s->Const	= (G/2)*(s->V0*s->V0)*(cos(DEGTORAD(s->Winkel/10))
								*cos(DEGTORAD(s->Winkel/10)));
	
			/* printf(romstr574,
				s->V0,s->Winkel, (int)s->Tan,(int)((s->Tan*1000))%1000,
				 (int)s->Const,(int)((s->Const*1000))%1000); */
				schiffschuesse++;
			}
		}
	
	/* Schüsse weiterbewegen bzw. animieren: */
	for(i=0; i<numschiffe; ++i)
	{
		SCHUSS *s=&schiff[i].Schuss;

		switch(s->State)
		{
			case S_TURMDREH:
				if(!(vbl_ct&7)) s->AnimSeq++;
				if(s->AnimSeq < (O_TURMDREH+NUM_TURMDREH))
				{
					KOORD scr;
					if(convert_koord(&s->Pos,&scr))
						draw_obj2(s->AnimSeq,ODER,scr2,scr.x+6,scr.y-2);
				}
				else s->State = S_FLUG;
				break;

			case S_FLUG:
			{
				KOORD scr;

				schuss_bahn(s);
				s->Pos.R = schiff[i].Pos.R-s->Pos.R;					/* Abstand vom Spieler */

				if(s->Pos.Z<0)	/* Kugel im Wasser aufgeschlagen */
				{
					s->State=S_IDLE;								/* Schuss ist frei */
					schiffschuesse--;
					if (zufall(munition==0 ? 7 : 100)==0) {
						gegner_score++;
						blitz(0x700);
						if ((gegner_score&1)==0) {
							spielerschiff--;			/* 2 Treffer pro Schiff */
							}
						}
					break;
				}
				else if(s->Pos.R < 0)							/* Kugel zuweit geschossen */
				{
					s->State = S_IDLE;
					schiffschuesse--;
					break;
				}

				if(convert_koord(&s->Pos,&scr))
					draw_obj2(O_KUGEL+(s->Pos.R*NUM_KUGEL)/MAX_DIST,ODER,scr2,scr.x,scr.y);

				break;
			}

			case S_TREFFER:
				if(!(vbl_ct&1)) s->AnimSeq++;
				if(s->AnimSeq < (O_FEUERBALL+NUM_FEUERBALL))
				{
					KOORD scr;
					if(convert_koord(&s->Pos,&scr))
						draw_obj2(s->AnimSeq,ODER,scr2,scr.x,scr.y);
				}
				else
				{
					s->State = S_IDLE;
					schiffschuesse--;
				}
				break;

			case S_KEINTREFFER:
				if(!(vbl_ct&1)) s->AnimSeq++;
				if(s->AnimSeq < (O_FONTAENE+NUM_FONTAENE))
				{
					KOORD scr;
					if(convert_koord(&s->Pos,&scr))
						draw_obj2(s->AnimSeq,ODER,scr2,scr.x,scr.y);
				}
				else
				{
					s->State = S_IDLE;
					schiffschuesse--;
				}
				break;
		}
	}	/* for(alle schiffe) */
}

/****************************************************************************
**	Gegnerische Flugzeug-Angriffe
*/

static void gegner_flugzeuge(void)
{
	int i;

	if(numflugzeuge<(1+gegnerstaerke/15))
	{
		if(range_rand(0,30) < gegnerstaerke)
		{
			FLUGZEUG *f = &flugzeug[numflugzeuge];

			f->Pos.R	= 500;
			f->Pos.W	= winkel(schussrichtung+range_rand(-45,45));
			f->Pos.Z	= 10;							/* 10 Meter über Meer */

			if((f->Pos.W-schussrichtung) > 10) {
				f->AnBase = O_FLUGZEUGLINKS;
				f->Ansq=5;
				}
			else if((f->Pos.W-schussrichtung) < -10) {
				f->AnBase = O_FLUGZEUGRECHTS;
				f->Ansq=5;
				}
			else {
				f->AnBase = O_FLUGZEUGFRONT;
				f->Ansq=7;
				}

			f->Schuss	= 0;
			f->nxt_move=vbl_ct;
			
			numflugzeuge++;
		}
	}

	for(i=0; i<numflugzeuge; ++i)
	{
		FLUGZEUG *f = &flugzeug[i];
		KOORD scr;

		if(convert_koord(&f->Pos,&scr)) {
			draw_obj2(f->AnBase,ODER,scr2,scr.x,scr.y);
			}

		if (vbl_ct>f->nxt_move+50) {
			 f->AnBase++;								/* Eine Animation weiter */
			 f->nxt_move=vbl_ct;
			 f->Ansq--;

			if(f->Ansq <= 0)
			{
				int j;

				for(j=i; j<numflugzeuge; ++j) flugzeug[j] = flugzeug[j+1];
				numflugzeuge--;
				i--;
			}
		}
	}
}

/****************************************************************************
**	Schuss-Handling
*/

static void do_schuss(void)
{
	int i;
	void *oldlogbase;
	
	if(schussdelay)
	{
		if(vbl_ct-schussdelay >= SCHUSSDELAY)		/* XXX variabel ? */
			schussdelay=0;
	}
	else if(mousek==1 && munition && (numschuesse<MAX_SCHUESSE))
	{
		SCHUSS *s=&schuss[numschuesse];

		s->Pos.R	= 0;						/* Abstand */
		s->Pos.W	= schussrichtung;			/* Schuss-Richtung */
		s->Pos.Z	= 0;						/* Schuss-Höhe */
		s->T0		= vbl_ct-20;				/* Abschusszeit */
		s->T		= 0;						/* Flugdauer */
		s->Winkel	= rohrwinkel;				/* Abschusswinkel */
		s->V0		= 5;						/* 5 m/blank XXX variabel */
		s->V0X		= s->V0*cos(DEGTORAD(s->Winkel/10.0));
		s->Tan		= tan(DEGTORAD(s->Winkel/10.0));
		s->State	= S_FLUG;

		s->Const	= (G/2)*(s->V0*s->V0)*(cos(DEGTORAD(s->Winkel/10.0))
						*cos(DEGTORAD(s->Winkel/10.0)));

		/* printf(romstr575,
			s->V0,s->Winkel, (int)s->Tan,(int)((s->Tan*1000))%1000,
			 (int)s->Const,(int)((s->Const*1000))%1000); */

		numschuesse++;
		munition--;
		gefeuert=TRUE;										/* Es wurde geschossen */
		schussdelay = vbl_ct;
	}

	for(i=0; i<numschuesse; ++i)
	{
		SCHUSS *s=&schuss[i];

		switch(s->State)
		{
			case S_FLUG:
			{
				KOORD scr;
				schuss_bahn(s);

				if(s->Pos.Z < 0)	/* Kugel im Wasser aufgeschlagen */
				{
					int j;
					int min_abstand=1000,abstand;
	
					s->State	= S_KEINTREFFER;					/* Default: Kein Treffer: */
					s->AnimSeq	= O_FONTAENE;
					
/*					debug(0,romstr576,s->Pos.R); */

					for(j=0; j<numschiffe; ++j)
					{
						if (winkel_differenz(s->Pos.W,schiff[j].Pos.W,3)) {
							abstand=ABS(s->Pos.R-schiff[j].Pos.R);
							if (abstand<min_abstand) min_abstand=abstand;
							if (abstand<TREFFERRADIUS) {
								min_abstand=0;
								s->State	= S_TREFFER;
								s->AnimSeq	= O_FEUERBALL;
								schiff[j].treffer=TRUE;
								score++;
								break;
								}
							}
					}
					oldlogbase=logbase;
					logbase=scr2;
					switch (min_abstand) {
						case 0:
							writexy_fast(16,148,romstr577);
							writexy_fast(16,154,space(5));
							break;
						case 1000:										/* zu weit weg */
							writexy_fast(16,148,space(10));
							writexy_fast(16,154,space(5));
							break;
						default:
							writexy_fast(16,148,romstr578);
							writexy_fast(16,154,str(4,(long)min_abstand));
							break;
						}
					logbase=oldlogbase; 

					cpy_raster(scr2,scr1,16,148,16+11*4,160,16,148);			/* Auf 2. Screen */

					break;
				}
				else if(s->Pos.R > MAX_DIST)	/* Kugel zuweit geschossen */
				{
					s->State = S_IDLE;					/* Dann Schuß deaktivieren */
					break;
				}

				if(convert_koord(&s->Pos,&scr))
					draw_obj2(O_KUGEL+(s->Pos.R*NUM_KUGEL)/MAX_DIST,ODER,scr2,scr.x,scr.y);
				break;
			}

			case S_TREFFER:
				if(!(vbl_ct&1)) s->AnimSeq++;
				if(s->AnimSeq < (O_FEUERBALL+NUM_FEUERBALL/2)) 
				{
					KOORD scr;
					if(convert_koord(&s->Pos,&scr))
						draw_obj2(s->AnimSeq,ODER,scr2,scr.x,scr.y);
				}
				else
				{
					s->State = S_IDLE;
				}
				break;

			case S_KEINTREFFER:
				if(!(vbl_ct&1)) s->AnimSeq++;
				if(s->AnimSeq < (O_FONTAENE+NUM_FONTAENE))
				{
					KOORD scr;
					if(convert_koord(&s->Pos,&scr))
						draw_obj2(s->AnimSeq,ODER,scr2,scr.x,scr.y);
				}
				else
				{
					s->State = S_IDLE;
				}
				break;
		}
	}	/* for(alle schuesse) */

	/* Alle Schuesse die S_IDLE haben entfernen */
remidle:
	for(i=0; i<numschuesse; ++i) if(schuss[i].State == S_IDLE)
	{
		int j;
		--numschuesse;
		for(j=i; j<numschuesse; ++j) schuss[j]=schuss[j+1];
		goto remidle;
	}

}

void init_seekampf(void)
{
	/* Initialisiert den Seekampf und plaziert die Schiffe */
	
	numschuesse=schiffschuesse=0;				/* Kein Schuss ist unterwegs */
	score=gegner_score=0;
	old_score=old_gegner_score=-1;				/* Noch nicht angezeigt */
	
	numflugzeuge=0;
	old_winkel=old_muni=-1;						/* Bisher keine Munition angezeigt */		

	gefeuert=FALSE;										/* Es wurde nicht gefeuert */
	schussdelay=0;										/* Es wurde nicht gefeuert */
		
	set_schiffe();										/* Plaziert die Schiffe */
}

void show_score(void)
{
	/* Zeigt die Scores an */
	void *oldlogbase;
	
	if (old_score!=score || old_gegner_score!=gegner_score) {
		old_gegner_score=gegner_score;
		old_score=score;
		
		oldlogbase=logbase;
		logbase=scr2;
	
		writexy_fast(8,5,build(romstr579,str(2,(long)score)));
		writexy_fast(231,5,build(romstr580,str(2,(long)gegner_score)));
	
		logbase=oldlogbase;
		
		cpy_raster(scr2,scr1,0,5,319,10,0,5);
		}
}
	
/****************************************************************************
**	Die Seeschlacht-Routine. Stand-alone, benützt nur die Routinen
**	draw_obj() und rnd() aus mcode.s, sowie alle aus raster.c
**  nummer ist die Nummer der Armee.
*/

void Seeschlacht(int nummer)
{
	int i;
	void *oldlogbase;
	float proz;
	int hoehe;

	/* Die folgenden Werte müßen gemäß Spielstand initialisiert werden */

	munition=spielerschiff=0;
	for(i=0;i<SCHIFFMAX;i++) 
		if (flotte[i].typ>=0) 						/* Schiff existent? */
			if (flotte[i].ort==-1) {					/* und beim Spieler? */
				munition+=ship_cannons[flotte[i].typ]*2;
				spielerschiff++;
				}

	gegnerstaerke=99;										/* Volle Staerke */
					
	numschiffe=(armeegesamt[nummer]*MAX_SCHIFFE)/1000;			/* 1000 Mann sind alle Schiffe */
	if (numschiffe<0) numschiffe=0;
	else if (numschiffe>MAX_SCHIFFE) numschiffe=MAX_SCHIFFE;
	gegnerschiffe=numschiffe;								/* Soviel waren vorher da */

	if (gegnerschiffe==0) return;							/* Kein Kampf */
		
	/* Spielbeginn: */	
	objects= pack_buf;
	load_objekte(SEAWAR_OBJ,objects);		/* Seeschlacht-Objekte laden */

	init_seekampf();						/* Variablen vorbereiten, Schiffe plazieren */
	
	Hm(); 

	msminy=0;										/* Maus darf ganz hoch */
	fade_out();
	clear_raster();
	draw_obj(0,leisten,MOVE,scr1,0,0);
	draw_obj(O_HINTERGRUND,objects,MOVE,scr1,0,Y0);
	new_buttons(PAUSE);
	
	oldlogbase=logbase;
	logbase=scr1;
	writexy_fast(123,5,build(romstr581,gegnername[nummer-1]));
	logbase=oldlogbase;
	
	copy_screen(scr1,scr2);
	fade_in();
	logbase=scr1;
		
	while(numschiffe!=0 && spielerschiff!=0)
	{
		hol_maus();
		if (my>167) button_leiste();
		
		draw_obj_part(O_HINTERGRUND,objects,83,0,236,115,MOVE,scr2,83,Y0);		/* Playfield */
		draw_obj_part(O_HINTERGRUND,objects,263,118,307,142,MOVE,scr2,263,135);	/* Radar */

		if (mdeltax!=0) {										/* Maus bewegt? */
			if (ABS(mdeltax)<=3) schussrichtung+=(mdeltax<0) ? 1 : -1 ;
			else {
				int dif;
				
				dif=-mdeltax/5;
				if (abs(dif)>10) dif=(dif>0) ? 10 : -10;
				schussrichtung+=dif;
				}
			mdeltax=0;
			}
			
		schussrichtung=winkel(schussrichtung);					/* in 0..360 bringen */
		
		rohrwinkel = ((mousey*(MAX_WINKEL-MIN_WINKEL))/200.0+MIN_WINKEL)*10.0;	/* Festkomma */
		show_schiffe();
		gegner_schiff_schuss();
		gegner_flugzeuge();
		do_schuss();
		show_kanonen();
		show_radar();
		show_munition(munition);
		show_score();
		switch_screens();
	}

	Sm();
	
	if (spielerschiff==0) {
		Hm();

		draw_obj(O_HINTERGRUND,objects,MOVE,scr1,0,Y0);		/* Playfield */
		
		copy_screen(scr1,scr2);
		hoehe=objekt_hoehe(O_HINTERGRUND,objects);
		for(i=0;i<hoehe-(HORIZONT-8);i++) {
			logbase=scr2;
			fill(9,0,HORIZONT-8+Y0+i-1,319,HORIZONT-8+Y0+i);			/* Wasser wieder hin */
			if (i<17) line(15,0,HORIZONT+Y0,319,HORIZONT+Y0);		/* Und Horizont zeichnen */
			draw_obj_part(O_HINTERGRUND,objects,0,HORIZONT-8+1,319,hoehe-i-1,MOVE,scr2,0,HORIZONT-8+Y0+i+1);
			if (HORIZONT+Y0+i<167) 
				line(9,84,HORIZONT+Y0+i,235,HORIZONT+Y0+i);				/* Horizont aus Objekt löschen */
			wait_sync(3);
			switch_screens();
			}
		Sm();
		game_over(romstr582,romstr583);
		}
	else {
		for(i=0;i<SCHIFFMAX && spielerschiff>0;i++) 
			if (flotte[i].typ>=0 && flotte[i].ort==-1) spielerschiff--;

		for(;i<SCHIFFMAX;i++) 
			if (flotte[i].typ>=0 && flotte[i].ort==-1) verlier_schiff(i);		

		proz=(double)numschiffe/gegnerschiffe;				/* Soviel [0..1] sind noch da */

		armeegesamt[nummer]=0;
		for(i=0;i<EINHEITEN;i++) {
			armeeteil[nummer][i]*=proz;
			armeegesamt[nummer]+=armeeteil[nummer][i];
			}
		}
		
	land_an();
}
