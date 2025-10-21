  	
	/* Medusa II - Bunkersourcecode Teil 2
								 Behandlung der Objekte und Feinde
								 
		(C) 1991 by Till Bubeck, Ziegeleistr. 28, W-7056 Weinstadt, Tel: 07151-66437 */
		
#include "includes.c"           /* Definiert alle Variablen als Extern,... */

int lampenstand;										/* Stand der Lampe */

/* Koordinaten der verschiedenen Medusa-Bilder: */
unsigned char med_koord_x[6] = { 65,56,55,63,55,56 };
unsigned char med_koord_y[6] = { 75,74,79,75,74,71 };

unsigned char kug_koord_x[8] = { 130,120,110,100, 90,89,88,87   };
unsigned char kug_koord_y[8] = { 75,81,87,93,     100,99,98,97   };

void change_all_reagenz(reagenz,relativ)
int reagenz;
int relativ;
{
	/* Zieht bei allen ReagenzglÑsern eins ... */
	int member;
	
	for(member=0;member<PARTY;member++) {
		change_reagenz(member,reagenz,relativ);
		}
	copy_reagenz();
}

void change_reagenz(member,reagenz,relativ)
int member;
int reagenz;
int relativ;
{
	/* Erniedert den Wert und zeigt ihn gleich an */
	
	subtract_reagenz(member,reagenz,relativ);
	draw_rea_stand(member,reagenz);
}

void subtract_reagenz(member,reagenz,relativ)
int member;
int reagenz;
int relativ;
{
	/* Erniedert den Wasserstand einer SÑule */
	int wert;
	char *tot;
	PARTY_MEMBER *partyp;
	int i;
		
	wert=party[member].reagenz[reagenz];
	if (!treffer_cheat) wert+=relativ;					/* Bei Cheat keine VerÑnderung mehr */
	if (wert<=0) {
		wert=0;
		if (member==0) {											/* Spieler? */
			switch (reagenz) {
				case 0: tot=romstr526; break;
				case 1: tot=romstr527; break;								
				case 2: tot=romstr528; break;
				}
			game_over(tot,ns);
			}
		else {
			draw_rea_stand(member,reagenz);					/* Totenkopf hin */

			partyp=&party[member];
			for(i=0;i<8;i++) {
				drop_item(&partyp->tasche[i]);					
				}
			for(i=0;i<2;i++) {
				drop_item(&partyp->hand[i]);					
				}
			drop_item(&partyp->helm);
			drop_item(&partyp->oberteil);
			drop_item(&partyp->unterteil);
			drop_item(&partyp->schuhe);
			}
		}
	if (wert>255) wert=255;
	party[member].reagenz[reagenz]=wert;
}

void drop_item(item_nr)
int *item_nr;
{
	/* Der Spieler ist gestorben, -> Objekt auf den Boden legen */
	
	drop_itemxy(item_nr,bunker_x,bunker_y);
}

void drop_itemxy(item_nr,x,y)
int *item_nr;
int x,y;
{
	/* Der Spieler ist gestorben, -> Objekt auf den Boden legen */
	ITEMS *itemp;
	
	if (*item_nr!=-1) {					/* Hat er was auf der Hand? */
		itemp=&item[*item_nr];
		itemp->bun_nr=akt_bunker;
		itemp->bun_x=x;
		itemp->bun_y=y;
		itemp->pos=zufall(4);			/* In irgendeine Ecke... */
		*item_nr=-1;								/* Und nichts mehr in der Hand.. */

		put_1bit(itemmap,(x-3)+(y-3)*80,1);				/* Hier liegt was */
		}
}


FLAG is_tot(member)
int member;
{
	/* öberprÅft, ob der Member der Party tot ist */
	
	return(party[member].reagenz[0]==0 || party[member].reagenz[1]==0 || party[member].reagenz[2]==0);
}

void draw_reagenz()
{
	/* zeichnet alle ReagenzglÑser neu */
	int member;
	
	Hm();
	for(member=0;member<PARTY;member++) {
		draw_rea(member);
		}

	copy_reagenz();											/* Kopiert ReagenzglÑser auf 2. Screen */
	Sm();
}

void draw_rea(member)
int member;
{
	/* Zeichnet ein Reagenzglas */
	int i;
	
	Hm();
	draw_glaeser(member);
	for(i=0;i<3;i++) {
		reagenz_stand[member*3+i]=0;				/* Auf jeden Fall zeichnen */
		draw_rea_stand(member,i);
		}
	draw_hand(member);
	Sm();
}

void draw_rea_stand(member,rea)
int member;											/* Welches Member? */
int rea;												/* Welches Reagenzglas? */
{
	/* Zeichnet den FlÅssigkeitsstand in einem Reagenzstand neu */
	
	int rea_x;
	int breite,hoehe;
	int wasserhoehe;

	Hm();
	
	if (party[member].reagenz[rea]==0) {						/* Member ist tot? */
		draw_obj(3,reagenz,MOVE,scr1,member*80,0);				/* Totenkopf hin */
		}
	else {
		breite=objekt_breite(1,reagenz);			/* Breite eines Reagenzglases */
		hoehe=objekt_hoehe(1,reagenz);
					
		rea_x=member*80+36+rea*14;				/* Hier startet das Reagenzglas */
	
		wasserhoehe=party[member].reagenz[rea]*hoehe/255;		/* Hîhe der FlÅssigkeit */
		if (wasserhoehe>hoehe) wasserhoehe=hoehe;
		
		if (wasserhoehe!=reagenz_stand[member*3+rea]) {			/* Nur bei VerÑnderung zeichnen */
			draw_obj_part(0,reagenz,36,15,36+breite-1,15+hoehe-1,MOVE,scr1,rea_x,15);	/* Glas leer */
			draw_obj_part(1,reagenz,0,hoehe-wasserhoehe,breite-1,hoehe-1,MOVE,scr1,rea_x,15+hoehe-wasserhoehe);
			reagenz_stand[member*3+rea]=wasserhoehe;
			}
		}
	Sm();
}	

void copy_reagenz()
{
	/* Kopiert die ReagenzglÑser auf den 2. Screen */
	
	Hm();
	cpy_raster(scr1,scr2,0,0,319,44,0,0);
	Sm();
}

void draw_glaeser(member)
int member;
{
	/* Zeichnet die ReagenzglÑser einer bestimmten Person und beschriftet diese */
	void *oldlogbase;
		
	Hm();
	draw_obj(0,reagenz,MOVE,scr1,member*80,0);		/* ReagenzglÑser zeichnen */
	draw_schiessflag(member);
		
	oldlogbase=logbase;
	logbase=scr1;
	if (member==0) {
		writexy(0,32,1,player_name);
		lampenstand=255;								/* auf jeden Fall zeichnen */
		draw_lampenanzeige(); 
		}
	else {
		writexy(0,member*80+32,1,prisoner[party[member].prisoner_nr].name);
		}
	
	logbase=oldlogbase;
	Sm();
}

void draw_schiessflag(member)
int member;
{
	Hm();
	if (party[member].schiessen) {
		draw_obj(5,reagenz,MOVE,scr1,member*80+13,1);
		}
	else fill(0,member*80+13,1,member*80+13+12,5);
	copy_reagenz();
	Sm();
}

void draw_lampenanzeige()
{
	/* Zeichnet die Lampenanzeige neu */
	void *oldlogbase;
	int neue_lampe;
	
	Hm();
	
	oldlogbase=logbase;
	logbase=scr1;
	
	neue_lampe=ycargo_menge[10];
	if (neue_lampe>37) neue_lampe=37;
	
	if (lampenstand!=neue_lampe) {
		lampenstand=neue_lampe;
		fill(12,3,1,7,43);						/* wegmachen */
		line(4,3,3,3,40);
		line(3,4,3,4,40);
		draw_obj(4,reagenz,ODER,scr1,5,40-neue_lampe-2);
		copy_reagenz();
		}
	
	logbase=oldlogbase;
	Sm();
}

void set_enemies(waende)
BUNKERWAND **waende;
{
	/* TrÑgt alle Items, die sich an dieser Stelle im Bunker befinden in die
		 Objektliste ein */
	
	set_enemy(waende,bunker_x,bunker_y,8,162,0);			/* auf Spielerfeld */
	set_enemy(waende,bunker_x+bunker_dx,bunker_y+bunker_dy,28,142,35);
	set_enemy(waende,bunker_x+2*bunker_dx,bunker_y+2*bunker_dy,42,122,110);
	set_enemy(waende,bunker_x+3*bunker_dx,bunker_y+3*bunker_dy,64,112,140);
}

void set_enemy(waende,x,y,tiefe,unterkante,verkleinerung)
BUNKERWAND **waende;
int x,y;															/* Position im Bunker */
int tiefe;													
int unterkante;												/* Unterkante des Objekts */
int verkleinerung;
{
	register ENEMY *ep;
	register int i;
	int ob_nr;
	int breite;
		
	if (betretbar(bunker[x][y])) {	
		ep=enemy;
		for(i=0;i<enemy_max;i++,ep++) 
			if (ep->bun_nr==akt_bunker && ep->x==x && ep->y==y) {					/* Auf diesem Feld? */
				if ((*waende)-startwaende>=WANDMAX) break;						/* Array ist voll */
				ob_nr=ep->grafik+4*ep->next_move/ep->speed;
				if (ep->next_move>(3*ep->speed)/4) ob_nr=ep->grafik+1; 
				if (ep->fight) ob_nr=3;									/* Kampfstellung */
				breite=shrink_obj_breite(verkleinerung,ob_nr,enemy_obj);
				(*waende)->x1=96-breite/2;
				(*waende)->x2=(*waende)->x1+breite-1;
				(*waende)->y=unterkante-(shrink_obj_hoehe(verkleinerung,ob_nr,enemy_obj)-1);
				(*waende)->tiefe=tiefe;
				(*waende)->ob_nr=240+ob_nr;											/* Feind */
				(*waende)->knuepfen=TRUE;
				(*waende)->modus=ODER;
				(*waende)++->ob_x=0;
				}
		}
}

void feuer_enemy(nummer)
int nummer;
{
	/* Dieser Gegner hat in dem Bunker einen Schuû abgegeben: */
	int i;

	enemy[nummer].fight=TRUE;					/* Jetzt kÑmpft er */
	shot.ob_mem=shoots;
	shot.ob_nr=1;

	shot.dx=0;												/* Eine Richtung vorgeben */
	shot.dy=1;
	if (bunker_x==enemy[nummer].x) {
		shot.dx=0;
		if (bunker_y>enemy[nummer].y) 
			shot.dy=1;
		else 
			shot.dy=-1;
		}
	if (bunker_y==enemy[nummer].y) {
		shot.dy=0;
		if (bunker_x>enemy[nummer].x) 
			shot.dx=1;
		else
			shot.dx=-1;
		}

	shot.x=enemy[nummer].x;
	shot.y=enemy[nummer].y;
	shot.entfernung=0;
	shot.gegner=nummer;
	shot.last_vbl=0;
	shot.speed=1;											/* 20 VBLs Speed */
	shot.x1=-1;												/* Kein Restore */

	Hm();
	cpy_raster(scr1,hlpbuf,0,45,319,167,0,0);		/* Gang merken */
	Sm();
}

void do_enemy_shot()
{
	/* Der Gegnerschuss bewegt sich um ein Feld weiter */
	long dif;
	int x,y;
	int shrink;
	int dist;
	int start,ende;
	int staerke,abzug;
	int i;
	int x1_neu,y1_neu,x2_neu,y2_neu;
	
	if (shot.ob_mem!=NULL) {					/* Schuss vorhanden */
		if (shot.last_vbl==0) dif=shot.speed;		/* Erster Schuû sofort und ohne Zeit fÅr draw_bunker() */
		else dif=vbl_ct-shot.last_vbl;				
		if (dif>=shot.speed) {					/* Weiterbewegen */
			shot.last_vbl=vbl_ct;
			dif/=shot.speed;							/* Wieviel Schritte Åberspringen? */
			shot.entfernung+=dif;							
			if (shot.entfernung>40) {
				shot.ob_mem=NULL;						/* Schuss zuende */
				enemy[shot.gegner].fight=FALSE;
				}
			else {
				x=shot.x+shot.entfernung/10*shot.dx;		/* Aktuelles Schussfeld */
				y=shot.y+shot.entfernung/10*shot.dy;
				if (!betretbar(bunker[x][y])) shot.ob_mem=NULL;
				else {
					/* Trefferabfrage: */
					if (x==bunker_x && y==bunker_y) {			/* Treffer am Spieler */
						blitz(0xfff);
						staerke=8+enemy[shot.gegner].speed/15;	/* SchussstÑrke Gegner 8..25 */ 
						for(i=0;i<PARTY;i++) 
							if (!is_tot(i)) {
								abzug=staerke*(1.0-party[i].protection/200.0);		/* 0.5..1.0 */
								change_reagenz(i,2,-abzug);			/* Treffer */
								}
						copy_reagenz();
						shot.ob_mem=NULL;						/* Schuss weg */
						enemy[shot.gegner].fight=FALSE;
						Hm();
						cpy_raster(hlpbuf,scr1,0,0,319,120,0,45);		/* Grafik weg */
						Sm();
						}
					else {	
						/* Schuss zeichnen: */		
						if (bunker_dx==-shot.dx && bunker_dy==-shot.dy)			/* Blick in Richtung des Schusses? */
							for(dist=0;dist<4;dist++) {
								if (!betretbar(bunker[bunker_x+bunker_dx*dist][bunker_y+bunker_dy*dist])) break;
								if (bunker_x+bunker_dx*dist==x && bunker_y+bunker_dy*dist==y) {	/* Schuss kann gesehen werden */
									switch (dist) {
										case 0: start=0; ende=30; break;
										case 1: start=30; ende=140; break;		/* Verkleinerungstufen der Felder */
										case 2: start=140; ende=180; break;
										case 3: start=180; ende=255; break;
										}
									shrink=start+(10-shot.entfernung%10)*(ende-start)/10;		/* Entfernung innerhalb des Feldes */
		
									x1_neu=96-shrink_obj_breite(shrink,shot.ob_nr,shot.ob_mem)/2;
									y1_neu=106-shrink_obj_hoehe(shrink,shot.ob_nr,shot.ob_mem)/2-shrink/10;
									x2_neu=x1_neu+shrink_obj_breite(shrink,shot.ob_nr,shot.ob_mem)+1;
									y2_neu=y1_neu+shrink_obj_hoehe(shrink,shot.ob_nr,shot.ob_mem)+1;
	
									Hm();
										
									if (shot.x1!=-1) {
										if (shot.y1<y1_neu) 
											cpy_raster(hlpbuf,scr1,x1_neu,shot.y1-45,x2_neu,y1_neu-45,x1_neu,shot.y1);
										shot.x1=-1;
										}
									
									shot.x1=x1_neu;
									shot.y1=y1_neu;
									shot.x2=x2_neu;
									shot.y2=y2_neu;
		
									draw_shrink_obj(shrink,shrink_buff,shot.ob_nr,shot.ob_mem,ODER,scr1,
														shot.x1,shot.y1);
									Sm();
									break;
									}
								}		
						}
					}
				}
			}
		}	
}		

void set_items(waende)
BUNKERWAND **waende;
{
	/* TrÑgt alle Items, die sich an dieser Stelle im Bunker befinden in die
		 Objektliste ein */

	LAGE pos[4];											/* Wo liegt welche Ecke? */
	int x,y;
	
	/* Zuerst Objekte suchen, welche auf der Position des Spielers liegen: */

	if (liegt_item(bunker_x,bunker_y)) {
		pos[0].y=pos[1].y=pos[2].y=pos[3].y=162;
		pos[0].tiefe=pos[1].tiefe=pos[2].tiefe=pos[3].tiefe=9;
		switch (richtung) {									/* Blickrichtung? */
			/* Wo ist Eck NO,SO,SW,NW? */
			case NORD:	pos[0].x=135;						/* Ecke 0=NO an Position 135/158 */
									pos[1].x=-1; 
									pos[2].x=-1; 
									pos[3].x=58;	break;				
			case OST:		pos[0].x=58;
									pos[1].x=135; 
									pos[2].x=-1; 
									pos[3].x=-1;	break;				
			case SUED:	pos[0].x=-1;
									pos[1].x=58; 
									pos[2].x=135; 
									pos[3].x=-1;	break;				
			case WEST:	pos[0].x=-1;
									pos[1].x=-1; 
									pos[2].x=58; 
									pos[3].x=135;	break;				
			}
		
		set_them(waende,bunker_x,bunker_y,pos);						/* Items einzeichnen */

		}

	/* Jetzt die Items untersuchen, die in 1 Kasten Entfernung liegen */
	x=bunker_x+bunker_dx;							/* Ein Feld Entfernung */
	y=bunker_y+bunker_dy;

	if (liegt_item(x,y)) {
		switch (richtung) {									/* Blickrichtung? */
			/* Wo ist Eck NO,SO,SW,NW? */
			case NORD:	pos[0].x=122; pos[0].y=132; pos[0].tiefe=29;
									pos[1].x=126; pos[1].y=145; pos[1].tiefe=27;
									pos[2].x=65;  pos[2].y=145; pos[2].tiefe=27;
									pos[3].x=72;  pos[3].y=132; pos[3].tiefe=29;	break;		
			case OST:		pos[0].x=72;  pos[0].y=132; pos[0].tiefe=29;
									pos[1].x=122; pos[1].y=132; pos[1].tiefe=29;
									pos[2].x=126; pos[2].y=145; pos[2].tiefe=27;
									pos[3].x=65;  pos[3].y=145; pos[3].tiefe=27;	break;				
			case SUED:	pos[0].x=65;  pos[0].y=145; pos[0].tiefe=27;
									pos[1].x=72;  pos[1].y=132; pos[1].tiefe=29;
									pos[2].x=122; pos[2].y=132; pos[2].tiefe=29;
									pos[3].x=126; pos[3].y=145; pos[3].tiefe=27;	break;
			case WEST:	pos[0].x=126; pos[0].y=145; pos[0].tiefe=27;
									pos[1].x=65;  pos[1].y=145; pos[1].tiefe=27;
									pos[2].x=72;  pos[2].y=132; pos[2].tiefe=29;
									pos[3].x=122; pos[3].y=132; pos[3].tiefe=29;	break;
			}
		
		set_them(waende,x,y,pos);
		}

	/* Jetzt die Items untersuchen, die in 2 Kasten Entfernung liegen */
	x=bunker_x+2*bunker_dx;							/* Zwei Felder Entfernung */
	y=bunker_y+2*bunker_dy;

	if (liegt_item(x,y)) {
		pos[0].y=pos[1].y=pos[2].y=pos[3].y=122;
		pos[0].tiefe=pos[1].tiefe=pos[2].tiefe=pos[3].tiefe=41;
		switch (richtung) {									/* Blickrichtung? */
			/* Wo ist Eck NO,SO,SW,NW? */
			case NORD:	pos[0].x=-1;
									pos[1].x=121;
									pos[2].x=87;
									pos[3].x=-1;	break;		
			case OST:		pos[0].x=-1;
									pos[1].x=-1;
									pos[2].x=121;
									pos[3].x=87;	break;		
			case SUED:	pos[0].x=87;
									pos[1].x=-1;
									pos[2].x=-1;
									pos[3].x=121;	break;		
			case WEST:	pos[0].x=121;
									pos[1].x=87;
									pos[2].x=-1;
									pos[3].x=-1;	break;		
			}
		
		set_them(waende,x,y,pos);
		}
}

void set_them(waende,x,y,pos)
BUNKERWAND **waende;
int x,y;
LAGE pos[];
{
	/* Setzt die Items entsprechend ihrer Position: */
	int i;
	int ob_nr;
	register ITEMS *itemp;
	int pos_item;

	for(itemp=item,i=0;i<ITEMMAX;i++,itemp++)
		if (itemp->bun_nr==akt_bunker) 				/* Objekt in diesem Bunker? */
			if (itemp->bun_x==x && itemp->bun_y==y) 		/* an dieser Position? */
				if (pos[itemp->pos].x!=-1) {				/* Ist es zu sehen? */
					ob_nr=itemp->grafik;
					pos_item=itemp->pos;
					if ((*waende)-startwaende>=WANDMAX) break;	/* Array ist voll */
					(*waende)->x1=pos[pos_item].x-objekt_breite(ob_nr,items)/2;
					(*waende)->x2=(*waende)->x1+objekt_breite(ob_nr,items)-1;
					(*waende)->y=pos[pos_item].y-(objekt_hoehe(ob_nr,items)-1);
					(*waende)->tiefe=pos[pos_item].tiefe;
					(*waende)->ob_nr=100+ob_nr;
					(*waende)->knuepfen=TRUE;
					(*waende)->modus=ODER;
					(*waende)++->ob_x=0;
					}
}

FLAG move_enemies()
{
	/* Bewegt alle Feinde im Bunker einen Schritt weiter */
	
	register int i;
	register ENEMY *ep;
	int anzahl;
	FLAG richtung[8];
	int dx,dy;
	int new_rich_zahl;
	int neue_richtung;
	FLAG bewegt=FALSE;							/* Wurde was bewegt? */
	long differenz;
	
	differenz=vbl_ct-enemycounter;			/* Soviel VBL's zum letzten Aufruf */
	enemycounter=vbl_ct;						/* Alle 2s eine Einheit */

	if (shot.ob_mem==NULL)					/* Nur bewegen, wenn kein Schuss da */
		for(ep=enemy,i=0;i<enemy_max;i++,ep++) 
			if (ep->bun_nr==akt_bunker) {
				if ((bunker_x==ep->x && abs(bunker_y-ep->y)<5) ||		/* Auf einer Linie mit Spieler */
						(bunker_y==ep->y && abs(bunker_x-ep->x)<5)) 
					if (zufall(10)==0) {						
						feuer_enemy(i);
						bewegt=TRUE;
						}
				if (differenz>ep->next_move) {
					neue_richtung=ep->next_move;
					bewegt=TRUE;
					ep->next_move=ep->speed;
					memset(richtung,FALSE,sizeof(richtung));		/* Alle Komponenten auf False */
					anzahl=0;
					if (enemy_betretbar(ep->x,ep->y-1)) {
						anzahl++;
						richtung[0]=TRUE;
						}
					if (enemy_betretbar(ep->x+1,ep->y)) {
						anzahl++;
						richtung[2]=TRUE;
						}
					if (enemy_betretbar(ep->x,ep->y+1)) {
						anzahl++;
						richtung[4]=TRUE;
						}
					if (enemy_betretbar(ep->x-1,ep->y)) {
						anzahl++;
						richtung[6]=TRUE;
						}
					if (anzahl>1) {									/* Kreuzung */
						new_rich_zahl=zufall(anzahl-1);				/* den wievielten Weg nehmen? */
						}
					else new_rich_zahl=0;						/* Sonst die erste nehmen */
					
					neue_richtung=-1;
					if (richtung[0]) 
						if (new_rich_zahl--<=0) {
							if (anzahl==1 || ep->richtung!=4) {
								neue_richtung=0;
								dy=-1;											/* N */
								dx=0;
								}
							}
					if (richtung[2] && neue_richtung<0) 
						if (new_rich_zahl--<=0) {
							if (anzahl==1 || ep->richtung!=6) {
								neue_richtung=2;
								dy=0;												/* O */
								dx=1;
								}
							}
					if (richtung[4] && neue_richtung<0) 
						if (new_rich_zahl--<=0) {
							if (anzahl==1 || ep->richtung!=0) {
								neue_richtung=4;
								dy=1;												/* S */
								dx=0;
								}
							}
					if (richtung[6] && neue_richtung<0) 
						if (new_rich_zahl--<=0) {
							if (anzahl==1 || ep->richtung!=2) {
								neue_richtung=6;
								dy=0;												/* W */
								dx=-1;
								}
							}
					if (neue_richtung<0) internal(romstr529);
					ep->x+=dx;
					ep->y+=dy;
					ep->richtung=neue_richtung;
					ep->fight=FALSE;						/* Feind kÑmpft nicht mehr */
					}
				else {												/* Noch nicht bewegen */
					ep->next_move-=differenz;				/* Abziehen */
					}
				}
			
	return(bewegt);								/* Wurde Feind bewegt? */
}

void free_prisoner()
{
	/* Spieler will einen Gefangenen befreien */
	unsigned int nr;
	int member;
		
	for(member=1;member<4;member++) 
		if (is_tot(member)) break;
		
	if (member>=4) dungeon_alert(romstr530);
	else {
		nr=bunker[bunker_x+bunker_dx][bunker_y+bunker_dy]-PRISONER_START;
	
		if (yes_no(build(romstr531,prisoner[nr].name))) {
			bunker[bunker_x+bunker_dx][bunker_y+bunker_dy]=DOOR_AUF_START;
			party[member].reagenz[0]=party[member].reagenz[1]=20;
			party[member].reagenz[2]=prisoner[nr].staerke;
			party[member].prisoner_nr=nr;  
			party[member].protection=0;
			party[member].schiessen=FALSE;
			prisoner[nr].befreit=TRUE;							/* Schon befreit */
			draw_reagenz();
			}
		}
}

void hunger_durst()
{
	/* Simuliert den Hunger bzw. Durst */
	long hunger_dif,durst_dif;
	
	hunger_dif=(50-(level-1.0)*20)*50;			
	durst_dif=(40-(level-1.0)*20)*50;
	
	if (vbl_ct>hungercounter+hunger_dif) {			/* Alle 40s eine Einheit runter */
		hungercounter=vbl_ct;
		change_all_reagenz(0,-1);					/* Bei Hunger eins runter */
		}
	if (vbl_ct>durstcounter+durst_dif) {
		durstcounter=vbl_ct;
		change_all_reagenz(1,-1);					/* Bei Durst eins runter */
		}
}

int do_shoot(item_nr)
int item_nr;											/* Welches Item will der Spieler werfen -1=Keines */
{
	/* Spieler will schiessen */
	int i;
	int oldsync,oldhelligkeit;
	int x1,y1,x2,y2;
	int breite,hoehe;
	int links,rechts;
	static long old_schuss_vbl=0;				/* Wann darf der nÑchste Schuss kommen? */
	FLAG kann_schiessen=FALSE;
	static FLAG alert_da=FALSE;
	void *bib;
	int ob_nr;
	FLAG key_throw;
	
	key_throw=FALSE;
	if (item_nr>=0)										/* Objekt zum Werfen? */
		 if (item[item_nr].typ==KEY) key_throw=TRUE;	/* SchlÅssel? */
		
	if (!key_throw && vbl_ct>old_schuss_vbl+50) {	/* Nur einmal pro Sekunde */
	
		if (item_nr>=0) {								/* Spieler will was werfen */
			schussstaerke=2;
			bib=items;
			ob_nr=item[item_nr].grafik;
			}
		else {	
			schussstaerke=0;
			bib=shoots;
			ob_nr=0;
						
			/* Zuerst feststellen, ob der Spieler Munition und Waffe hat */
			for(i=0;i<PARTY;i++) {
				links=party[i].hand[0];
				rechts=party[i].hand[1];
				if (links!=-1 && rechts!=-1) 			/* Beide HÑnde voll */			
					if ((item[links].typ==WAFFE && item[rechts].typ==MUNITION) ||
							(item[links].typ==MUNITION && item[rechts].typ==WAFFE)) {
						if (item[links].typ==MUNITION) {
							if (item[links].flag>0) {				/* Munition da? */
								if (party[i].schiessen) {
									item[links].flag--;
									schussstaerke+=item[rechts].flag;
									}
								else kann_schiessen=TRUE;				/* er kînnte Schiessen */
								}
							}
						else 
							if (item[rechts].flag>0) {				/* Munition in rechter Hand? */
								if (party[i].schiessen) {
									item[rechts].flag--;
									schussstaerke+=item[links].flag;
									}
								else kann_schiessen=TRUE;				/* Er kînnte Schiessen */
								}
						}
				}
			}
			
		if (schussstaerke>0) {							/* Es war noch ein Schuû da... */
			if (schuss_cheat) schussstaerke*=2;				/* Schuss doppelt so stark */
			Hm();
			copy_bunker_screen();							/* auf 2. Screen Åbertragen */
			copy_screen(scr1,hlpbuf);
			
			oldsync=sync;
			sync=1;
		
			oldhelligkeit=helligkeit;
			if (item_nr<0) {								/* Richtiger Schuss */
				while(helligkeit<6) {						/* Schuû volle Helligkeit */
					helligkeit++;
					set_bunker_pal();
					}
				if (oldhelligkeit<=4) {
					helligkeit--;
					set_bunker_pal();
					helligkeit--;										/* Jetzt wieder Helligkeit 4 */
					set_bunker_pal();
					}	
				hippel_sfx(LASER);
				}
					
			for(i=0;i<255;i+=10) {
				if (i>=20) {
					breite=shrink_obj_breite(i-20,ob_nr,bib)+3;
					hoehe=shrink_obj_hoehe(i-20,ob_nr,bib)+3;
				
					x1=96-breite/2;
					y1=106-hoehe/2-(i-20)/10;								/* Max. Ausdehnung des Schusses */
					x2=x1+breite-1;
					y2=y1+hoehe-1;
					cpy_raster(hlpbuf,scr2,x1,y1,x2,y2,x1,y1);				/* Restaurieren */
					}
				draw_shrink_obj(i,shrink_buff,ob_nr,bib,ODER,
							scr2,96-shrink_obj_breite(i,ob_nr,bib)/2,106-shrink_obj_hoehe(i,ob_nr,bib)/2-i/10);
		
				swap_screens();
				if (i<30) {												/* Ganz vorne */
					if (check_4_enemy(0,&item_nr)) break;
					}
				else if (i<140) {										/* 1 Feld Entfernung */
					if (check_4_enemy(1,&item_nr)) break;
					}
				else if (i<180) {										/* 2 Felder Entfernung */
					if (check_4_enemy(2,&item_nr)) break;
					}
				else {
					if (check_4_enemy(3,&item_nr)) break;
					}		
				}	
		
			if (i>=255) drop_itemxy(&item_nr,bunker_x+3*bunker_dx,bunker_y+3*bunker_dy);	/* Ganz weit geflogen */
			
			sync=oldsync;
		
			while(helligkeit!=oldhelligkeit) {
				if (helligkeit<oldhelligkeit) helligkeit++;			/* ZurÅck auf alte Helligkeit */
				else helligkeit--;
				set_bunker_pal();
				}
			Sm();
			}
		else 													/* SchussstÑrke>0 */
			if (kann_schiessen && !alert_da) {
				dungeon_alert(romstr532);
				dungeon_alert(romstr533);
				alert_da=TRUE;								/* Meldung war schon da */
				}														  			
		old_schuss_vbl=vbl_ct;						/* Ab jetzt 1 Sekunde */
		}

	return(item_nr);
}

FLAG check_4_enemy(entfernung,item_nr)
int entfernung;									/* Entfernung zum Spieler */
int *item_nr;										/* Was hat der Spieler in der Hand -1=Nichts */
{
	/* öberprÅft, ob an dieser Stelle ein Gegner bzw. Wand ist. */
	register ENEMY *ep;
	register int i;
	int x,y;
	int size;
	int staerke;
	
	x=bunker_x+bunker_dx*entfernung;
	y=bunker_y+bunker_dy*entfernung;
		
	if (betretbar(bunker[x][y])) {	
		ep=enemy;
		for(i=0;i<enemy_max;i++,ep++) 
			if (ep->bun_nr==akt_bunker && ep->x==x && ep->y==y) {		/* Auf diesem Feld? */
				switch (entfernung) {
					case 0: size=0; y=110; break;
					case 1: size=50; y=96; break;
					case 2: size=90; y=92; break;
					default: size=170; y=87; break;
					}
				copy_screen(hlpbuf,scr1);					/* Kugel weg */
				staerke=ep->staerke;
				staerke-=schussstaerke;
				if (staerke<0) {
					ep->bun_nr=255;							/* Gegner weg */
					explosion(size,96,y,0,6);
					explosion(size,90,y-3,0,6);
					explosion(size,99,y+3,0,6);
					}
				else {
					ep->staerke=staerke;
					explosion(size,96,y,0,3);				/* kleine Explosion */
					}
				if (*item_nr>=0) drop_itemxy(item_nr,ep->x,ep->y);
				return(TRUE);											/* Treffer */
				}
		return(FALSE);												/* Betretbar aber kein Gegner */
		}
	else {
		if (*item_nr>=0) drop_itemxy(item_nr,x-bunker_dx,y-bunker_dy);
		return(TRUE);											/* Abbruch, da nicht betretbar */
		}
}

void explosion(size,x,y,von,bis)
int size;
int x,y;										/* Mittelpunkt */
int von,bis;								/* Animationsbilder */
{
	/* Stellt eine Explosion am Mittelpunkt in Groesse 'size' dar. */
	int i;
	
	Hm();
	copy_screen(scr1,scr2);	
	for(i=von;i<=bis;i++) {
		draw_shrink_obj(size,shrink_buff,i,fire,ODER,scr1,
						x-shrink_obj_breite(size,i,fire)/2,y-shrink_obj_hoehe(size,i,fire)/2);
		wait_once(3);
		}
	copy_screen(scr2,scr1);
	Sm();
}

void copy_bunker_screen()
{
	/* Kopiert Screen1 auf Screen2 und paût die Automapvariablen an */
		
	Hm();
	copy_screen(scr1,scr2);

	auto_nr=0;												/* Alle Changes sind Åbertragen */
	player[0]=player[1];							/* Und Spieler an selber Position */
	Sm();
}

void medusa()
{
	/* Der Spieler hat die letzte TÅr des Bunkers geîffnet -> Medusa kommt */
	int animation;
	FLAG spiegel_da=FALSE;
	int i,j;
	int treffer=0;
	
	Hm();
	
	copy_screen(scr1,scr2);
	copy_screen(scr1,hlpbuf);
	
	Sm();
	
	load_objekte(MEDUSA_OBJ,walls);

	for(i=0;i<PARTY;i++) 
		if (!is_tot(i)) 
			for(j=0;j<2;j++) 
				if (party[i].hand[j]!=-1) 
					if (item[party[i].hand[j]].typ==MIRROR) 
						spiegel_da=TRUE;
			
	if (spiegel_da) dungeon_alert(romstr534);
	else dungeon_alert(romstr535);
	
	do {
		do {
			draw_medusa(0,-1);
			wait_sync_klick(20);
			draw_medusa(1,-1); 
			wait_sync_klick(20);
			} while(zufall(15)==0);
		hol_maus();
		for(animation=2;animation<5;animation++) draw_medusa(animation,-1);
		draw_medusa(5,6);
		draw_medusa(4,7);
		draw_medusa(3,8);
		draw_medusa(3,9);
		if (spiegel_da) {
			for(animation=14;animation>=10;animation--) draw_medusa(3,animation);
			if (treffer++==10) {
				for(i=0;i<10;i++) 
					explosion(0,93+zufall(25)-12,100+zufall(25)-12,0,6);
				copy_screen(hlpbuf,scr1);
				for(i=0;i<5;i++) 
					explosion(0,93+zufall(25)-12,100+zufall(25)-12,0,6);
				finale();
				}
			else explosion(0,90,100,0,3);				/* Kleine Explosion */
			}
		else change_all_reagenz(2,-40);
		} while(TRUE);
}

void draw_medusa(med_nr,kug_nr)
int med_nr,kug_nr;
{
	/* Zeichnet die Medusaanimation auf Screen 2 */
	
	Hm();
	cpy_raster(hlpbuf,scr2,0,45,319,167,0,45);			/* weglîschen */
	draw_obj(med_nr,walls,ODER,scr2,med_koord_x[med_nr],med_koord_y[med_nr]);
	if (kug_nr!=-1) 
		draw_obj((kug_nr>9) ? kug_nr-4 : kug_nr,walls,ODER,scr2,kug_koord_x[kug_nr-6],kug_koord_y[kug_nr-6]);
	Sm();
	
	swap_screens();
	wait_sync_klick(10);
	if (get_key()!=-1) {								/* Taste da */
		show_window(romstr536);
		wait_once(80);
		hide_window();
		}
}

void finale()
{
	long laenge;
	
	Hm();
	
	fade_out();
	clear_raster();

	load_objekte(MED_TOT_OBJ,scr2);
	clear_screen(scr1);
	draw_obj(0,scr2,MOVE,scr1,0,0);
	
#ifdef FRANZ
  laenge=load_bibliothek(OVER_FRC_TXT,scr2+20000);
#endif
#ifdef ENGLISCH
  laenge=load_bibliothek(OVER_GB_TXT,scr2+20000);
#endif
#ifdef DEUTSCH
  laenge=load_bibliothek(OVER_D_TXT,scr2+20000);
#endif

	decrypt((unsigned char *)scr2+20000,laenge);				/* Scroller entschlÅsseln */

  load_objekte(METALL_OBJ,pack_buf);					/* Scroller laden */

  init_scroller(scr1,157,scr2+20000,pack_buf,pack_buf+42240L,FALSE);
  scradr=init_vbl(mcode33);     			            /* Laufschrift an */
	set_raster(0,157,scroller_pal);
	fade_in();
	
	wait_klick();

  fade_out();
	
  clear_raster();
  exit_vbl(scradr);
	
	Sm();
	longjmp(restart,1);
}

