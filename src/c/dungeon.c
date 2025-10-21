      
/* Medusa II:

	 gesamte Abhandlung der Bunker

	 Ω 1991 by Till Bubeck, Ziegeleistr. 28, 7056 Weinstadt
						 Tel.: 07151-66437 (Semensterferien/Wochenende)
							oder 07071/49612 (Studium)
																		  */

#include "includes.c"           /* Definiert alle Variablen als Extern,... */

char *prisoner_obj;									/* NEOchrome Objekt */

char s_status[][20] = {								/* Status des SPielers */
	romstr485,
	romstr486,
	romstr487,
	romstr488 };


FLAG dungeon(nr)
int nr;
{
	/* Spieler betritt Dungeon nr */
	long taste;
	long next_key_vbl;
	int btn_nr;
	int bun_x1,bun_y1;
	unsigned int old_tuer;
	int land;
	FLAG new_draw;							/* Soll Bunker neu gezeichnet werden */
	FLAG moved;									/* Hat sich der Spieler bewegt, oder nur gedreht? */
	FLAG bewegt;								/* Wurden die Feinde bewegt? */
	int x,y;
	int nummer;
	char *pw;
	FLAG first_taste;
		
	/* Simulationsverzîgerer initialisieren: */
	hungercounter=durstcounter=enemycounter=lampencounter=vbl_ct;
	
	akt_bunker=(unsigned char)nr;					/* Spieler ist jetzt in diesem Bunker */

	loc = CITY;
	leiste_y=64;
	msminy=0; 								  /* hîher darf Maus nicht */

	sx[0]=sxalt[0];
	sy[0]=syalt[0]; 					  /* Sprite zurÅcksetzen */

	option_city();						  /* Bereitet Options auf Stadt vor */

	load_waende();						  /* LÑdt die BunkerwÑnde */
	init_dungeon(akt_bunker,START_POS);		   /* Holt Dungeon von der Disk */

	Hm();
	init_oben();

	clear_raster();

	fade_out(); 							  									/* Bild ausblenden */
	clear_screen(scr1); 												/* Bildschirm lîschen */
	show_buttons(FALSE);

	redraw_buttons((cheat_on) ? EXIT_BTN|LOAD|SAVE|PAUSE : LOAD|SAVE|PAUSE);					
	
	copy_screen(scr1,scr2);
	Sm();

	set_raster(0,45,(tv_modus) ? tv_pal : bunker_pal);
	draw_bunker(TRUE);						/* Spieler hat sich bewegt->Automap updaten */
	draw_reagenz();								/* Zeichnet die ReagenzglÑser neu */
	
	fade_in();
	
	lampencounter=0;
	
	button=NOTHING;
	do {

		if (cheat_on && (bleiste&EXIT_BTN)==0) {			/* Exitbutton zuschalten */
			redraw_buttons(bleiste|EXIT_BTN);
			copy_buttons();
			}
			
		new_draw=moved=FALSE;

		do_taschenlampe();								/* ggf. Taschenlampe dunkler machen */
		hunger_durst();										/* Hunger bzw. Durst simulieren */
		bewegt=move_enemies();						/* ggf. Feinde bewegen */
		
		taste=0;													/* Nichts gedrÅckt */
		hol_maus(); 											/* Mauskoordinaten holen */
		bun_x1=bunker_x+bunker_dx;					/* Feld in einer Einheit Entfernung */
		bun_y1=bunker_y+bunker_dy;

		if (maus_in(0,46,144,124)) {					/* Schiessen oder TÅre îffnen */
			old_tuer=bunker[bun_x1][bun_y1];
			bedien_tuer(-1);								/* Nichts in der Hand */
			if (old_tuer==START_POS) break;				/* Ausgang gefunden! */
			}			

		if (mk==2 || maus_in(24,125,167,167) || my<46) {			/* User will Item aufnehmen bzw. zur Figur */
			bedien_item(bunker_x,bunker_y);
			}
		if (is_open_door(bunker[bun_x1][bun_y1]) && is_aufzug(bunker[bunker_x][bunker_y])) /* Aufzug? */
			if (maus_in(145,91,157,118))							/* Aufzugsknopf? */
				aufzug(bunker[bunker_x][bunker_y]);
		
		if (mk==0) first_taste=TRUE;							/* erster Repeat nach 15 VBL's */
		
		if (option_nr==4) {														/* Steuerpanel zu sehen */
			if (maus_in(115,173,163,194)) {								/* Im Bereich der Knîpfe? */
				btn_nr=(my-173)/12*3+(mx-115)/16;									/* Nr des Buttons */
				switch (btn_nr) {
					case 0: taste=0x520000;												/* Entsprechenden Tastendruck simulieren */
						 break;
					case 1: taste=0x480000;
						 break;
					case 2: taste=0x470000;
						 break;
					case 3: taste=0x4b0000;
						 break;
					case 4: taste=0x500000;
						 break;
					case 5:	taste=0x4d0000;
						 break;
					default: first_taste=TRUE; taste=-1; break;
					}

				if (taste!=-1L) 								/* Taste da? */
					if (first_taste) {							/* Neu gedrÅckt? */
						next_key_vbl=vbl_ct+15;					/* Dann nÑchste Taste in 15 VBL's */
						first_taste=FALSE;						/* Und dann normal */
						}
					else {
						if (vbl_ct>next_key_vbl) next_key_vbl=vbl_ct+3;	/* Zeit fÅr Repeat gewartet? */
						else taste=-1;							/* Nein, weiter warten */
						}
					
				}
			else if (my>169 && mk!=0) button_leiste();		/* Im Bereich der Icons */
			}
		else if (my>169 && mk!=0) button_leiste();		/* Bei anderem Board */

		
		if (taste==0) {										/* Nur, wenn keine Richtung geklickt wurde */
			taste=get_key();
			if (taste!=-1L) next_key_vbl=vbl_ct+15;		/* in 15 VBL's den ersten Repeat */
			else {
				taste=0;
				if (keypress!=0) {				/* Taste noch gedrÅckt */
					if (vbl_ct>next_key_vbl) {
						taste=keypress;
						next_key_vbl=vbl_ct+3;			/* Alle 3 VBL's ein Tastenrepeat */
						}
					}
				}
			}

		taste/=65536L;							 				/* Scancode der Taste holen */
		switch ((int)taste) {
			case 0x62: if (cheat_on) {
									cheat_auswert(TRUE);					/* Help */
									 draw_whole=2;
									 copy_screen(scr2,scr1);
									 }
								 break;
			case 0x39: 
				old_tuer=bunker[bun_x1][bun_y1];
				mk=1;
				mx=my=100;										/* TÅr îffnen simulieren */
				bedien_tuer(-1);							/* Nichts in der Hand */
				if (old_tuer==START_POS) goto bunker_ende;	/* Ausgang gefunden! */
				break;
			case 0x1c: 
				if (cheat_on) {
					Hm();
					for(x=3;x<80;x++)
						for(y=3;y<50;y++) {
							if (is_wand(bunker[x][y])) insert_automap(1,x,y);
							if (is_door(bunker[x][y])) insert_automap(2,x,y);
							if (liegt_item(x,y)) insert_automap(3,x,y);
							}
					auto_nr=0;
					draw_whole=2;									/* 2 mal zeichnen */
					automapping=TRUE;
					Sm();
					}
				break;
			case 0x47:												/* Clr/Home */
				press_button(2);
				richtung++;
				if (richtung==4) richtung=0;
				berechne_richtung();		  					/* Neue Delta-Werte berechnen */
				new_draw=TRUE;
				break;
			case 0x52:								  				/* Insert */
				press_button(0);
				richtung--;
				if (richtung<0) richtung=3;
				berechne_richtung();
				new_draw=TRUE;
				break;
			case 0x50:								  				/* Pfeil runter */
				if (betretbar(bunker[bunker_x-bunker_dx][bunker_y-bunker_dy])) {
					bunker_x-=bunker_dx;			  		
					bunker_y-=bunker_dy;
					press_button(4);
					new_draw=moved=TRUE;		   			/* Neue Position zeigen */
					}
				else wandgelaufen();
				break;
			case 0x48:												/* Pfeil hoch */
				if (betretbar(bunker[bunker_x+bunker_dx][bunker_y+bunker_dy])) {
					bunker_x+=bunker_dx;
					bunker_y+=bunker_dy;
					press_button(1);
					new_draw=moved=TRUE;					/* Neue Position zeigen */
					}
				else wandgelaufen();
				break;
			case 0x4b:												/* Pfeil links */
				richtung--;											/* Nach links drehen */
				if (richtung<0) richtung=3;
				berechne_richtung();
				if (betretbar(bunker[bunker_x+bunker_dx][bunker_y+bunker_dy])) {
					bunker_x+=bunker_dx;
					bunker_y+=bunker_dy;
					press_button(3);
					richtung++;															/* ZurÅckdrehen */
					if (richtung>3) richtung=0;
					berechne_richtung();
					new_draw=moved=TRUE;
					}
				else {
					richtung++;															/* ZurÅckdrehen */
					if (richtung>3) richtung=0;
					berechne_richtung();
					wandgelaufen();
					}
				break;
			case 0x4d:
				richtung++;																/* Nach rechts drehen */
				if (richtung>3) richtung=0;
				berechne_richtung();
				if (betretbar(bunker[bunker_x+bunker_dx][bunker_y+bunker_dy])) {
					bunker_x+=bunker_dx;
					bunker_y+=bunker_dy;
					press_button(5);
					richtung--;															/* ZurÅckdrehen */
					if (richtung<0) richtung=3;
					berechne_richtung();
					new_draw=moved=TRUE;
					}
				else {
					richtung--;															/* ZurÅckdrehen */
					if (richtung<0) richtung=3;
					berechne_richtung();
					wandgelaufen();
					}
				break;
			default:
				break;
			}

		if (shot.ob_mem==NULL || new_draw || bewegt) {				/* Spieler oder Feinde haben sich bewegt */
			new_draw=FALSE;
			draw_bunker(moved);
			}
		do_enemy_shot();
		
		switch (button) {
			case LOAD:  load_game(); 
									ycargo_menge[10]++;					/* Eine Batterie mehr */
									lampencounter=0;						/* und gleich wieder weg */
									draw_reagenz();
									mk=0;
								  button=NOTHING;
									break;
			case SAVE: 	save_game();
									mk=0;
								  button=NOTHING;
									break;
			}
		
		} while(button!=EXIT_BTN);

bunker_ende:

	if (akt_bunker==21 || akt_bunker==22) ycargo_menge[10]=0;		/* Keine Batterien nach dem Startbunker mehr */

#ifdef COPYPROT

	exit_anzahl++;										/* Einmal Ausgang mehr */

	pw=password-561;
	if (*(pw+561)==0 || exit_anzahl>=10) {		/* Bei jedem 10. Bunker */
		int anzahl,nr;
		long laenge;
		char *password_okay;
		FLAG okay;
		int i;

		exit_anzahl=0;
		password[0]=0;

		helligkeit=6;												/* Volle Helligkeit */
		set_bunker_pal();
		
#if defined(DEUTSCH)
		laenge=load_bibliothek(PASS_D_TXT,shrink_buff);
#elif defined(ENGLISCH)
		laenge=load_bibliothek(PASS_GB_TXT,shrink_buff);
#endif

		decrypt(shrink_buff,laenge);
	
		anzahl=atoi(such_string(1,shrink_buff));				/* Anzahl der Passwîrter */
		nr=zufall(anzahl);

		do {
			Hm();
	  	show_window(build(such_string(2,shrink_buff),							/* Text */
										such_string(nr*4+3,shrink_buff),						/* Seite */
										such_string(nr*4+4,shrink_buff),						/* Zeile */
										such_string(nr*4+5,shrink_buff)));						/* Wort */
										
		 	input(120,123,20,password);
  		hide_window();
			Sm();

			password_okay=such_string(nr*4+6,shrink_buff);
		
			okay=TRUE;
			for(i=0;i<=strlen(password_okay);i++) 
				if (password[i]!=password_okay[i]) okay=FALSE;
			} while(!okay);
		}
#endif

	for(land=0;land<LAENDER;land++)
		for (nummer=0;nummer<2;nummer++)
			if ((bunker_nr[land][nummer] & 127)==akt_bunker) {		/* Landbunker? */
		
			for(pos_welt=0;pos_welt<15*15;pos_welt++)
				if (welt[pos_welt]==land) {
					break;
					}
			
			map=land;
			load_ground(map);							/* UntergrÅnde laden */
		
			such_land(BUNKER_1+nummer);				/* Bunker suchen */
			sx[0]=sxalt[0]=grundx;
			sy[0]=syalt[0]=grundy;										/* Spielersprite setzen */
			sn[0]=0;
			get_ground(); 					  				/* neue UntergrÅnde holen */
			option_city_ende();							/* Options im Board ggf. zeichnen */
			land_an();
			akt_bunker=-1;
			return(TRUE);								/* Jetzt in einem Landbunker */
			}
	akt_bunker=-1;
	return(FALSE);									/* Jetzt in einem Stadtbunker */
}

void aufzug(feld)
unsigned int feld;
{
	/* Der Spieler ist in einem Aufzug und hat auf den Knopf gedrÅckt... */
	int hoehe,breite;
	int y;
	int stocks,akt_stock;
	FLAG nach_oben;
	unsigned int aufzug_feld;

	stocks=feld & 7;
	nach_oben=(feld>>3)&1;

	Hm();
	schliesse_tuer();

	breite=objekt_breite(5,walls);				/* Wand mitte */
	hoehe=objekt_hoehe(5,walls);

	copy_bunker_screen();							/* Scr1->Scr2 */
	copy_screen(scr1,hlpbuf);

	hippel_sfx(AUFZUG_AN);
	
	if (nach_oben) {
		/* Zuerst TÅr aus den Bereich scrollen: */
		for(y=hoehe-1;y>=0;y--) {
			cpy_raster(scr1,scr2,24,51,166,150,24,52);
			draw_obj_part(5,walls,8,y,breite-1,y,MOVE,scr2,24,51);
			wait_once(1);
			switch_screens();
			}
		/* Steine schnell scrollen: */
		for(akt_stock=0;akt_stock<=stocks;akt_stock++)
			for(y=hoehe-1;y>=0;y--) {
				cpy_raster(scr1,scr2,24,51,166,150,24,52);
				draw_obj_part(5,walls,8,y,breite-1,y,MOVE,scr2,24,51);
				switch_screens();
				}
		/* TÅr reinscrollen: */
		for(y=hoehe-1;y>=0;y--) {
			cpy_raster(scr1,scr2,24,51,166,150,24,52);
			cpy_raster(hlpbuf,scr2,24,51+y,167,51+y,24,51);
			wait_once(1);
			switch_screens();
			}
		}
	else {
		/* Zuerst TÅr aus den Bereich scrollen: */
		for(y=0;y<hoehe;y++) {
			cpy_raster(scr1,scr2,24,52,167,150,24,51);
			draw_obj_part(5,walls,8,y,breite-1,y,MOVE,scr2,24,150);
			wait_once(1);
			switch_screens();
			}
		/* Steine schnell scrollen: */
		for(akt_stock=0;akt_stock<=stocks;akt_stock++)
			for(y=0;y<hoehe;y++) {
				cpy_raster(scr1,scr2,24,52,167,150,24,51);
				draw_obj_part(5,walls,8,y,breite-1,y,MOVE,scr2,24,150);
				switch_screens();
				}
		/* TÅr reinscrollen: */
		for(y=0;y<hoehe+1;y++) {
			cpy_raster(scr1,scr2,24,52,167,150,24,51);
			cpy_raster(hlpbuf,scr2,24,51+y,167,51+y,24,150);
			wait_once(1);
			switch_screens();
			}
		}
	/* Gesuchtes Feld aufbauen: */
	aufzug_feld=LIFT_START+(stocks | (((nach_oben^1)<<3)&8) | (akt_bunker<<4));
	akt_bunker=(unsigned char)((feld>>4)&255);					/* Spieler ist jetzt in diesem Bunker */

	hippel_sfx(AUFZUG_AUS);

	init_dungeon(akt_bunker,aufzug_feld); 		  /* Holt Dungeon von der Disk */
		
	Sm();
	oeffne_tuer();
}

void schliesse_tuer()
{
	/* Schlieût eine TÅre (fÅr den Aufzug) */
	unsigned int tuer;
	int i;
	int breite1,breite2,hoehe;
	
	tuer=bunker[bunker_x+bunker_dx][bunker_y+bunker_dy]-DOOR_AUF_START;
	put_2bit(tueren,tuer,ZU);						/* TÅr ist jetzt zu */
	
	Hm();
	copy_bunker_screen();						/* Scr1->Scr2 -> Beide Screens gleich */
	copy_screen(scr1,hlpbuf);

	draw_dungeon(hlpbuf);							/* Zeichnet den Bunker auf den hlpbuf */

	hippel_sfx(TUER_AN);
	
	breite1=objekt_breite(11,walls);					/* SchiebetÅr links */
	hoehe=objekt_hoehe(11,walls);
	breite2=objekt_breite(12,walls);					/* SchiebetÅr rechts */

	for(i=64;i>=0;i--) {
		cpy_raster(hlpbuf,scr2,0,69,319,69+hoehe-1,0,69);				/* Hintergrund hin */
		if (i<breite1)
			draw_obj_part(11,walls,													/* SchiebetÅr links */
						i,0,breite1-1,hoehe-1,
						1,scr2,49,69);
		if (i<breite2)
			draw_obj_part(12,walls,													/* SchiebetÅr rechts */
						0,0,breite2-i-1,hoehe-1,
						1,scr2,94+i,69);

		switch_screens();
		}
	/* bunker[bunker_x+bunker_dx][bunker_y+bunker_dy]=DOOR_START+1;		/* Zune TÅre hin */  */

	Sm();

	hippel_sfx(TUER_AUS);
}

int bedien_tuer(item_nr)
int item_nr;												/* Das hat der Spieler in der Hand */
{
	/* Der Spieler will eine TÅr îffnen */
	unsigned int feld;
	unsigned int door_nr;
	FLAG tuer_offen=FALSE;
	
	feld=bunker[bunker_x+bunker_dx][bunker_y+bunker_dy];
	if (maus_in(75,88,116,129) && is_door(feld) && !is_open_door(feld)) {			/* Geschlossene TÅre? */
		door_nr=feld-DOOR_ZU_START+1;						/* Nummer der TÅre berechnen */
		if (door_cheat) item_nr=door_nr-1;					/* SchlÅssel ist da */
		if (feld==START_POS) door_nr=1;					/* StarttÅre geht immer auf */
		if (feld>=PRISONER_START && feld<=PRISONER_END) {	/* Gefangener */
			free_prisoner();
			tuer_offen=TRUE;
			}
		else 
			if (door_nr==1) {
				oeffne_tuer();					/* TÅr 1 ohne SchlÅssel îffnen */
				tuer_offen=TRUE;
				}
			else if (item_nr>=0 && item[item_nr].typ==KEY && item[item_nr].flag==door_nr) {				/* fÅr diese TÅr? */
				if (door_nr==133) {
					if (yes_no(romstr489)) {
						oeffne_tuer();
						tuer_offen=TRUE;
						medusa();
						}
					}
				else {
					oeffne_tuer();
					tuer_offen=TRUE;
					item[item_nr].bun_nr=254;					/* SchlÅssel ist weg */
					item_nr=-1;										/* Nichts mehr in der hand */
					}
				}
		}

	if (!tuer_offen) {
		item_nr=do_shoot(item_nr);				/* Keine TÅr -> Schiessen */
		}
		
	return(item_nr);											/* Das hat er jetzt in der Hand */
}

void oeffne_tuer()
{
	/* ôffnet eine TÅre */

	unsigned int tuer,tuer_nr,tuer_typ;
	int i;
	int breite1,breite2,hoehe;

	tuer=bunker[bunker_x+bunker_dx][bunker_y+bunker_dy];					/* Alte TÅre merken */
	tuer_nr=tuer-DOOR_ZU_START;

	if (tuer==START_POS) {
		tuer_nr=0;
		}
		
	bunker[bunker_x+bunker_dx][bunker_y+bunker_dy]=tuer_nr+DOOR_AUF_START;	/* Offene TÅre hin */

	Hm();
	copy_bunker_screen();						/* Scr1->Scr2 , Beide Screens gleich */
	copy_screen(scr1,hlpbuf);

	draw_dungeon(hlpbuf);							/* Zeichnet den Bunker auf den hlpbuf */
	if (tuer==START_POS) {
		load_objekte(AUSGANG_OBJ,reagenz);					/* Ausgangsbild laden */
		draw_obj(0,reagenz,PALETTE,hlpbuf,50,45);			/* Palette setzen */
		show_raster();
		draw_obj(0,reagenz,ODER,hlpbuf,50,69);				/* Landschaft zeichnen */
		}

	Sm();

	hippel_sfx(TUER_AN);
	
	if (is_aufzug_door(bunker_x+bunker_dx,bunker_y+bunker_dy)) tuer_typ=1;		/* SchiebetÅr */
	else tuer_typ=tuer%3;
	
	switch (tuer_typ) {
		case 0:
		case 1:
			breite1=objekt_breite(11,walls);					/* SchiebetÅr links */
			hoehe=objekt_hoehe(11,walls);
			breite2=objekt_breite(12,walls);					/* SchiebetÅr rechts */

			for(i=0;i<65;i++) {
				cpy_raster(hlpbuf,scr2,0,69,319,69+hoehe-1,0,69);				/* Hintergrund hin */
				if (i<breite1)
					draw_obj_part(11,walls,													/* SchiebetÅr links */
								i,0,breite1-1,hoehe-1,
								1,scr2,49,69);
				if (i<breite2)
					draw_obj_part(12,walls,													/* SchiebetÅr rechts */
								0,0,breite2-i-1,hoehe-1,
								1,scr2,94+i,69);

				swap_screens();
				}
			break;

		case 2:
			breite1=objekt_breite(13,walls);								/* VierertÅre */
			hoehe=objekt_hoehe(13,walls);

			for(i=0;i<=hoehe;i++) {
				cpy_raster(hlpbuf,scr2,0,69,319,69+hoehe*2-1,0,69);				/* Hintergrund hin */
				draw_obj_part(13,walls,									/* Und VierteltÅre hin */
						i,i,breite1-1,hoehe-1,
						1,scr2,49,69);
				draw_obj_part(13,walls,
						0,i,breite1-i-1,hoehe-1,
						1,scr2,49+breite1+i,69);
				draw_obj_part(13,walls,
						i,0,breite1-1,hoehe-i-1,
						1,scr2,49,69+hoehe+i);
				draw_obj_part(13,walls,
						0,0,breite1-i-1,hoehe-i-1,
						1,scr2,49+breite1+i,69+hoehe+i);
				swap_screens();
				}
			break;
		}

	hippel_sfx(TUER_AUS);
	
	if (tuer!=START_POS) put_2bit(tueren,tuer_nr,AUF);		/* TÅr ist jetzt auf */

	hol_maus();
}

void press_button(nr)
int nr;
{
	/* Spieler hat auf einen Richtungsknopf gedrÅckt, dieser muû jetzt gedrÅckt gezeichnet werden */
	int zeile,spalte;

	if (option_nr==4) {							/* Nur wenn Board zu sehen ist: */
		zeile=nr/3;
		spalte=nr%3;
		/* Knopf eins rechts und eins runter zeichnen: */
		Hm();
		draw_obj_part(4+option_nr,leisten,19-1+spalte*16,4-1+zeile*12,31+spalte*16,13+zeile*12,
										0,scr1,96+19+spalte*16,169+4+zeile*12);
		Sm();
		button_pressed=nr;
		}
}

void release_button()
{
	/* Der Aufbau des Bunkers ist fertig, jetzt muû der Knopf wieder losgelassen werden: */

	if (button_pressed!=-1) {
		zeile=button_pressed/3;
		spalte=button_pressed%3;
		Hm();
		draw_obj_part(4+option_nr,leisten,19+spalte*16,4+zeile*12,31+1+spalte*16,13+1+zeile*12,
										0,scr2,96+19+spalte*16,169+4+zeile*12);
		Sm();
		button_pressed=-1;
		}
}

void berechne_richtung()
{
	/* Berechnet aus Richtung den entsprechenden Bunker_dx/dy wert */
	int himmels[3];
	
	switch (richtung) {
		case NORD: bunker_dx=0; bunker_dy=-1; break;
		case OST:  bunker_dx=1; bunker_dy=0; break;
		case SUED: bunker_dx=0; bunker_dy=1; break;
		default: 	bunker_dx=-1; bunker_dy=0;
		}

	print_richtung();
	Hm();
	cpy_raster(scr1,scr2,171,176,214,192,171,176);		/* Auf 2. Screen Åbertragen */
	Sm();
}

void print_richtung()
{
	/* Zeichnet die Blickrichtung in die Anzeigebox */
	int links,mitte,rechts;
		
	if (option_nr==4) {									/* Bunkeranzeige */
		switch (richtung) {
			case NORD: links=10+3; mitte=10+0; rechts=10+1; break;		/* Welche Objekte sind links, oben, rechts */
			case OST:  links=10+0; mitte=10+1; rechts=10+2; break;
			case SUED: links=10+1; mitte=10+2; rechts=10+3; break;
			default: 	 links=10+2; mitte=10+3; rechts=10+0; break;
			}
		Hm();
		draw_obj(links,leisten,MOVE,scr1,171,183);
		draw_obj(mitte,leisten,MOVE,scr1,187,176);
		draw_obj(rechts,leisten,MOVE,scr1,203,183);
		Sm();
		}
}

void wandgelaufen()
{
	/* Der Spieler ist gegen eine Wand gelaufen */
	int member;

	change_all_reagenz(2,-1);				/* Bei Gesundheit eins abziehen */	
}

void draw_bunker(moved)
FLAG moved;											/* Hat sich Spieler bewegt? */
{
	/* Zeichnet den Bunker auf Scr2 und stellt ihn dar */
	void *oldlogbase;
	
	draw_dungeon(scr2);

	oldlogbase=logbase;
	logbase=scr2;
	draw_updates();											/* Updates der Automap zeichnen */
	if (moved && automapping) map_update();			/* Automap updaten */
	if (automapping) draw_player();
	logbase=oldlogbase;
	
	swap_screens();
	release_button();												/* Wenn Knopf gedrÅckt, dann loslassen */
		
	if (shot.ob_mem!=NULL) {						/* Schuss ist unterwegs */
		Hm();
		cpy_raster(scr1,hlpbuf,0,45,319,167,0,0);		/* Gang merken */
		Sm();
		}
}

void draw_dungeon(screen)
void *screen;													/* Zeichenscreen */
{
	/* Bunker neu zeichnen auf speziellen screen */
	int dx,dy;
	int x1,y1;
	int x2,y2;
	int x3,y3;
	int akt;
	int x_links,x_rechts;
	FLAG spiegeln;
	int wandcounter;								/* Nummer des nÑchsten freien wand[]-Eintrags */
	BUNKERWAND wand[WANDMAX];						/* Hier werden die BunkerwÑnde vor dem Zeichnen eingetragen
																		 und nach der Tiefe sortiert. */
	register BUNKERWAND *waende;							/* -> wand[] */
	void *bibliothek;
	int ob_nr;

	startwaende=wand;												/* Hier beginn die WÑnde */
	spiegeln=(richtung&1) ^ (bunker_x&1) ^ (bunker_y&1);			/* ab und zu spiegeln */

	wandcounter=bau_waende(spiegeln,wand);						/* Baut die WÑnde im Array auf */

	if (FALSE && my_system) {
		writexy_fast(0,0,romstr490);
		writexy_fast(32,0,str(2,(long)wandcounter));
		}
		
	/* Pass 1: WÑnde zeichnen (only solids) */
	draw_obj(0,walls,0,screen,0,46);					/* Hintergrund zeichnen */

	x_links=-1;																			/* Ganz links starten */
	while (such_kante(wand,wandcounter,TRUE,&x_links,&akt,&x_rechts)) {
		if (wand[akt].ob_nr>=240) {					/* Feind? */
			bibliothek=enemy_obj;
			ob_nr=wand[akt].ob_nr-240;
			}
		else if (wand[akt].ob_nr>=220) {				/* Feindschuû? */
			ob_nr=wand[akt].ob_nr-220;
			bibliothek=fire;
			}
		else if (wand[akt].ob_nr>=100) {
			bibliothek=items;								/* Objekt */
			ob_nr=wand[akt].ob_nr-100;
			}
		else {
			bibliothek=walls;								/* Wand */
			ob_nr=wand[akt].ob_nr;
			}
		draw_obj_part(ob_nr,bibliothek,
								wand[akt].ob_x+(x_links-wand[akt].x1),0,
								wand[akt].ob_x+(x_rechts-wand[akt].x1),objekt_hoehe(ob_nr,bibliothek)-1,
								MOVE,screen,x_links,wand[akt].y);
		x_links=x_rechts+1;
		}

	if (spiegeln) 																		/* ab und zu spiegeln */
		mirror(screen,0,46,191,167);

	/* Pass 2: Nur Transparente Objekte darauf zeichnen */
	x_links=-1;																			/* Ganz links starten */
	while (such_kante(wand,wandcounter,FALSE,&x_links,&akt,&x_rechts)) {
		if (wand[akt].knuepfen) {
			draw_hidden(wand,wandcounter,x_links,x_rechts,screen);					/* Alle dahinter zeichnen */
			}
		x_links=x_rechts+1;
		}
}

void draw_hidden(wand,wandcounter,x1,x2,screen)
BUNKERWAND wand[];
int wandcounter;								/* Anzahl an Wnadteilen */
int x1,x2;											/* Linker und rechter Rand des Objektes */
void *screen;
{
	/* Es wurde ein Objekt gefunden, welches ganz vorne liegt, aber transparent ist.
		 Deshalb mÅssen alle dahinter liegenden Objekte ebenfalls gezeichnet werden... */
	int x_links,x_rechts;
	int xl_alt;
	int akt;

	x_links=xl_alt=x1;
	while (such_kante(wand,wandcounter,TRUE,&x_links,&akt,&x_rechts)) {				/* Nur solids */
		if (x_links>x2) break;
		if (x_rechts>x2) x_rechts=x2;

		/* Ab jetzt alle davorliegenden transparenten Objekte von hinten nach vorne zeichnen: */
		/* Gab es eine LÅcke zwischen den solid Objects? */
		if (x_links>xl_alt) draw_transparent(999,wand,wandcounter,xl_alt,x_links-1,screen);
		draw_transparent(wand[akt].tiefe,wand,wandcounter,x_links,x_rechts,screen);

		xl_alt=x_links=x_rechts+1;
		}

	/* Sind rechts der solid objects noch irgendwelche Transparent? */
	if (xl_alt<=x2)	draw_transparent(999,wand,wandcounter,xl_alt,x2,screen);
}

void draw_transparent(ganz_hinten,wand,wandcounter,x_links,x_rechts,screen)
int ganz_hinten;
BUNKERWAND wand[];
int wandcounter;
int x_links,x_rechts;
void *screen;
{
	/* Zeichnet alle Transparenten Objekte im Bereich von x_links bis x_rechts und zwar
		 von hinten nach vorne... */

	register int tiefemax,i;
	int xl,xr;
	int yoben;
	int breite,hoehe;
	int links,rechts;
	float mult;
	int old_line_mode;
	void *old_pattern;
	void *old_logbase;
	
	void *bibliothek;								/* Welche Grafikbibliothek soll genommen werden? */
	int ob_nr;											/* tatsÑchliche Objektnummer */
	int verkleinerung;
	
	while (TRUE) {
		tiefemax=-1;
		for(i=0;i<wandcounter;i++)
			if (wand[i].knuepfen)
				if (x_links<=wand[i].x2 && x_rechts>wand[i].x1) 					/* Objekt in diesem Bereich? */
					if (wand[i].tiefe>tiefemax && wand[i].tiefe<ganz_hinten) {
						tiefemax=wand[i].tiefe;
						}

		if (tiefemax==-1) break;												/* Nichts mehr da -> Raus */

		for(i=0;i<wandcounter;i++)
			if (wand[i].knuepfen)
				if (x_links<=wand[i].x2 && x_rechts>wand[i].x1) 					/* Objekt in diesem Bereich? */
					if (wand[i].tiefe==tiefemax) {
						xl=x_links;
						xr=x_rechts;
						if (xl<wand[i].x1) xl=wand[i].x1;
						if (xr>wand[i].x2) xr=wand[i].x2;

						if (wand[i].ob_nr>=240) {				/* Feind? */
							bibliothek=enemy_obj;
							ob_nr=wand[i].ob_nr-240;
							}
						else if (wand[i].ob_nr>=220) {				/* Feindschuû? */
							bibliothek=fire;
							ob_nr=wand[i].ob_nr-220;
							}
						else if (wand[i].ob_nr>=100) {
							bibliothek=items;					/* Objekt */
							ob_nr=wand[i].ob_nr-100;
							}
						else {
							bibliothek=walls;					/* Wand */
							ob_nr=wand[i].ob_nr;
							}
		
						switch (wand[i].tiefe) {
							case 29: verkleinerung=57; break;			/* Item */
							case 41: verkleinerung=100; break;			/* Item */
							case 28: verkleinerung=57; break;			/* Enemy */
							case 42: verkleinerung=100; break;			/* Enemy */
							case 64: verkleinerung=130; break;			/* Enemy */
							default: verkleinerung=0; break;
							}				
						if (verkleinerung) {					/* Item, muû verkleinert werden */
							breite=objekt_breite(ob_nr,bibliothek);		/* unverkleinerte Breite */
							mult=(255-verkleinerung)/255.0;			/* 0.0=0% 1.0=100% */
							links=(wand[i].ob_x+(xl-wand[i].x1))/mult;
							rechts=(wand[i].ob_x+(xr+1-wand[i].x1))/mult;	/* +1 zur Vermeidung von Rundungsfehlern */
							if (rechts>=breite) rechts=breite-1;

							draw_shrink_obj_part(verkleinerung,shrink_buff,ob_nr,bibliothek,
							links,0,rechts,objekt_hoehe(ob_nr,bibliothek)-1,
							(int)wand[i].modus,screen,xl,wand[i].y); 
							}
						else draw_obj_part(ob_nr,bibliothek,
													wand[i].ob_x+(xl-wand[i].x1),0,
													wand[i].ob_x+(xr-wand[i].x1),
														objekt_hoehe(ob_nr,bibliothek)-1,
													(int)wand[i].modus,screen,xl,wand[i].y);

						if (wand[i].tiefe==25) {								/* Aufzugsknopf */
							hoehe=objekt_hoehe(ob_nr,bibliothek);
							if ((bunker[bunker_x][bunker_y]>>3)&1) yoben=wand[i].y+hoehe/2;
							else yoben=wand[i].y;
							old_line_mode=line_mode(2);						/* Transparent */
							old_pattern=line_pattern(point_pat);
							old_logbase=logbase;
							logbase=screen;
							fill(14,xl,yoben,wand[i].x2-2,yoben+hoehe/2);
							line_mode(old_line_mode);
							line_pattern(old_pattern);
							logbase=old_logbase;
							}
						}
		ganz_hinten=tiefemax;											/* Jetzt alle vor diesem Objekt */
		}
}

FLAG such_kante(wand,wandcounter,only_solid,x1,akt,x2)
BUNKERWAND wand[];
int wandcounter;
FLAG only_solid;												/* Sollen nur Solid Objects gefunden werden? */
int *x1;
int *akt,*x2;													/* Hier wird Ergebnis zurÅckgegeben */
{
	/* Sucht beginnend ab x1 nach einer Kante, wobei die Kante mit der kleinsten Tiefe
			bevorzugt wird. */
	int i;
	int tiefemin;
	int xmin;
	int l_index,r_index;

	tiefemin=999;
	l_index=-1;
	for(i=0;i<wandcounter;i++)
		if (!only_solid || !wand[i].knuepfen) 													/* Nur solid Objects please */
			if (*x1>=wand[i].x1 && *x1<=wand[i].x2) 									/* Liegt x1 innerhalb Objekt? */
				if (wand[i].tiefe<tiefemin) {
					tiefemin=wand[i].tiefe;
					xmin=wand[i].x2;
					l_index=i;
					}
	if (l_index==-1) {															/* hier kommt eine LÅcke */
		tiefemin=xmin=999;
		for(i=0;i<wandcounter;i++)
			if (!only_solid || !wand[i].knuepfen) {										/* Please only solid */
				if (wand[i].x1>*x1 && wand[i].x1<=xmin) {				/* Mîglichst weit links */
					if (wand[i].x1==xmin) {												/* An gleicher Startposition? */
						if (wand[i].tiefe<tiefemin) {								/* Dann den, der weiter vorne */
							xmin=wand[i].x1;
							tiefemin=wand[i].tiefe;
							l_index=i;
							}
						}
					else {
						tiefemin=wand[i].tiefe;
						xmin=wand[i].x1;
						l_index=i;
						}
					}
				}
		if (l_index==-1) return(FALSE);										/* Kein Objekt mehr da */
		*x1=wand[l_index].x1;															/* Jetzt ab diesem Objekt */
		}

	/* Feststellen bis zu welchem rechten Rand */
	xmin=wand[l_index].x2;
	r_index=-1;
	for(i=0;i<wandcounter;i++)
		if (!only_solid || !wand[i].knuepfen) 											/* Solid only */
			if (wand[i].x1>=*x1 && wand[i].x1<=xmin && wand[i].tiefe<tiefemin) {														/* Vor letztem Objekt? */
				xmin=wand[i].x1-1;
				r_index=i;
				}

	*akt=l_index;
	if (r_index>=0) *x2=wand[r_index].x1-1;								/* Bis zum nÑchsten Objekt */
	else *x2=wand[l_index].x2;														/* Dieses hier ganz anzeigen */

	return(TRUE);
}

int bau_waende(spiegeln,waende)
FLAG spiegeln;
BUNKERWAND *waende;
{
	/* baut im Array wand[] die Bunkerwand auf und gibt zurÅck wieviel EintrÑge benutzt wurden */
	int dx,dy;
	int x1,y1;
	int x2,y2;
	int x3,y3;
	BUNKERWAND *start;
	unsigned int tuer,door_nr;
	int ob_nr;
	
	for(x1=0;x1<WANDMAX;x1++) (waende+x1)->modus=MOVE;		/* Alle Objekte klopfen */

	start=waende;									/* Anfangswert des Pointers merken */

	switch(richtung) {
		case NORD:	dx=1;  dy=0; break;	 /* 90 nach rechts drehen */
		case OST: 	dx=0;  dy=1; break;
		case SUED:	dx=-1; dy=0; break;
		case WEST:	dx=0;  dy=-1;
		default: break;
		}

	if (spiegeln) {
		dx=-dx;
		dy=-dy;
		}

	x1=bunker_x+bunker_dx;							/* Feld in 1 Einheit Entfernung */
	y1=bunker_y+bunker_dy;
	x2=x1+bunker_dx;										/* Feld in 2 Einheiten Entfernung */
	y2=y1+bunker_dy;
	x3=x2+bunker_dx;										/* Feld in 3 Einheiten Entfernung */
	y3=y2+bunker_dy;

	if (is_wand(bunker[bunker_x-dx][bunker_y-dy])) {	/* Feld unmittelbar links des Spielers */
		waende->x1=0;
		waende->x2=23;
		waende->y=46+0;
		waende->tiefe=10;									/* Ganz vorne */
		waende->ob_nr=1;
		waende->knuepfen=FALSE;
		waende++->ob_x=0;
		}
	if (is_wand(bunker[bunker_x+dx][bunker_y+dy])) {	/* Feld unmittelbar rechts des Spielers */
		waende->x1=168;
		waende->x2=191;
		waende->y=46+0;
		waende->tiefe=10;									/* Ganz vorne */
		waende->ob_nr=2;
		waende->knuepfen=FALSE;
		waende++->ob_x=8;
		}
	if (bunker[bunker_x][bunker_y]>=LIFT_START && bunker[bunker_x][bunker_y]<=LIFT_END)
		if (is_door(bunker[x1][y1])) {
			waende->x1=145;
			waende->x2=145+objekt_breite(19,walls)-1;
			waende->y=93;
			waende->tiefe=25;
			waende->ob_nr=19;
			waende->knuepfen=TRUE;					/* Aufzugsknopf an die Wand */
			waende->modus=ODER;							/* Aufzug */
			waende++->ob_x=0;
			}
	if (is_wand(bunker[x1+dx][y1+dy])) {			/* Wand rechts */
		waende->x1=144;
		waende->x2=191;
		waende->y=46+5;
		waende->tiefe=30;
		waende->ob_nr=4;
		waende->knuepfen=FALSE;
		waende++->ob_x=0;
		}
	if (is_wand(bunker[x1-dx][y1-dy])) {			/* Wand links */
		waende->x1=0;
		waende->x2=47;
		waende->y=46+5;
		waende->tiefe=30;
		waende->ob_nr=3;
		waende->knuepfen=FALSE;
		waende++->ob_x=0;
		}
	if (is_wand(bunker[x1][y1])) {				/* Wand mitte */
		waende->x1=24;
		waende->x2=167;
		waende->y=46+5;
		waende->tiefe=26;
		waende->knuepfen=FALSE;
		waende->ob_x=8;
	 	if (is_door(bunker[x1][y1])) {
			waende->knuepfen=TRUE;
			waende->modus=ODER;
	 		waende++->ob_nr=8;							/* TÅrrahmen */
			if (!is_open_door(bunker[x1][y1])) {			/* TÅr ist zu! */
				(waende-1)->knuepfen=FALSE;				/* geschl. TÅr ist !transparent */
				tuer=bunker[x1][y1];
				if (tuer>=PRISONER_START && tuer<=PRISONER_END) {
					waende->x1=68;
					waende->x2=68+objekt_breite(20,walls)-1;
				 	waende->y=46+28;
					waende->tiefe=22;
					waende->knuepfen=TRUE;
					waende->ob_x=0;
			 		waende++->ob_nr=20;					/* Prisonerschild */
					}				
				door_nr=tuer-DOOR_ZU_START+1;						/* Nummer der TÅre berechnen */
				if (door_nr>1 && tuer!=START_POS && !(tuer>=PRISONER_START && tuer<=PRISONER_END)) {					/* SchlÅssel nîtig? */
					if (item[door_nr-1].grafik<8) ob_nr=22;				/* Chequekarte */
					else ob_nr=21;									/* SchlÅssel */
					waende->x1=97-objekt_breite(ob_nr,walls)/2;
					waende->x2=waende->x1+objekt_breite(ob_nr,walls)-1;
				 	waende->y=109-objekt_hoehe(ob_nr,walls)/2;
					waende->tiefe=22;
					waende->knuepfen=TRUE;
					waende->ob_x=0;
			 		waende++->ob_nr=ob_nr;					/* SchlÅsselloch */
					}				
					
				if (is_aufzug_door(x1,y1)) tuer=1;			/* SchiebetÅr */
				switch (tuer%3) {
					case 0:
					case 1:
									waende->x1=49;
									waende->x2=49+objekt_breite(11,walls)-1;
								 	waende->y=46+23;
									waende->tiefe=24;
									waende->knuepfen=TRUE;
									waende->ob_x=0;
							 		waende++->ob_nr=11;					/* SchiebetÅr links */

									waende->x1=94;
									waende->x2=94+objekt_breite(12,walls)-1;
								 	waende->y=46+23;
									waende->tiefe=23;
									waende->knuepfen=TRUE;
									waende->ob_x=0;
									waende->modus=ODER;
							 		waende++->ob_nr=12;					/* SchiebetÅr rechts */
									break;
					case 2:	waende->x1=48;
									waende->x2=48+objekt_breite(13,walls)-1;
								 	waende->y=46+23;
									waende->tiefe=24;
									waende->knuepfen=TRUE;					/* VierertÅr */
									waende->ob_x=0;
							 		waende++->ob_nr=13;

									*waende=*(waende+1)=*(waende+2)=*(waende-1);

									waende++->y=46+23+objekt_hoehe(13,walls)-1;

									waende->x1=48+objekt_breite(13,walls)-1;
									waende++->x2=48+2*objekt_breite(13,walls)-1;

									*waende=*(waende-1);

									waende++->y=46+23+objekt_hoehe(13,walls)-1;

									waende->x1=74;
									waende->x2=74+objekt_breite(14,walls)-1;
								 	waende->y=46+42;
									waende->tiefe=23;
									waende->knuepfen=TRUE;
									waende->modus=ODER;
									waende->ob_x=0;
							 		waende++->ob_nr=14;
									break;

					}
				}
			}
	 	else
			waende++->ob_nr=5;						/* Normale Wand */
		}

	/* WÑnde in 2 Einheiten Entfernung: */

	if (is_wand(bunker[x2-dx][y2-dy])) {				/* Feld 1 links des Spielers */
		waende->x1=0;
		waende->x2=63;
		waende->y=46+13;
		waende->tiefe=50;								/* Platte */
		waende->ob_nr=6;
		waende->knuepfen=FALSE;
		waende++->ob_x=64;
		}

	if (is_wand(bunker[x2+dx][y2+dy])) {				/* Feld 1 rechts des Spielers */
		waende->x1=128;
		waende->x2=191;
		waende->y=46+13;
		waende->tiefe=50;								/* Platte */
		waende->ob_nr=6;
		waende->knuepfen=FALSE;
		waende++->ob_x=0;
		}

	if (is_wand(bunker[x2][y2])) {						/* Feld vor dem Spieler (1 Entfernung) */
		waende->x1=48;
		waende->x2=143;
		waende->y=46+13;
		waende->tiefe=40;								/* Platte */
		waende->knuepfen=FALSE;
		waende->ob_x=16;
		if (is_door(bunker[x2][y2])) {
			waende->ob_x=0;
			waende->knuepfen=TRUE;
			waende->modus=ODER;
			waende++->ob_nr=9;						/* TÅrrahmen */
			if (!is_open_door(bunker[x2][y2])) {					/* TÅr ist zu */
				(waende-1)->knuepfen=FALSE;						/* geschl. TÅr ist !transparent */
				tuer=bunker[x2][y2];
				if (is_aufzug_door(x2,y2)) tuer=1;					/* SchiebetÅr */
				switch (tuer%3) {
					case 0:
					case 1:
						waende->x1=65;
						waende->x2=65+objekt_breite(15,walls)-1;
						waende->y=70;
						waende->tiefe=39;
						waende->ob_nr=15;								/* SchiebetÅr */
						waende->knuepfen=TRUE;
						waende++->ob_x=0;
						break;
					case 2:
						waende->x1=65;
						waende->x2=65+objekt_breite(16,walls)-1;
						waende->y=70;
						waende->tiefe=39;
						waende->ob_nr=16;								/* SchiebetÅr */
						waende->knuepfen=TRUE;
						waende++->ob_x=0;
						break;
					}
				}
			}
		else
			waende++->ob_nr=6;
		}

	/* WÑnde in 3 Einheiten Entfernung: */

	if (is_wand(bunker[x3+dx][y3+dy])) {					/* Wand rechts des SPielers */
		waende->x1=117;
		waende->x2=191;
		waende->y=46+19;
		waende->tiefe=70;										/* SchrÑge */
		waende->ob_nr=7;
		waende->knuepfen=FALSE;
		waende++->ob_x=5;
		}

	if (is_wand(bunker[x3-dx][y3-dy])) {						/* Feld links des Spielers */
		waende->x1=0;
		waende->x2=73;
		waende->y=46+19;
		waende->tiefe=70;								/* Platte */
		waende->ob_nr=7;
		waende->knuepfen=FALSE;
		waende++->ob_x=16;
		}

	if (is_wand(bunker[x3][y3])) {											/* Feld vor dem Spieler */
		waende->x1=64;
		waende->x2=127;
		waende->y=46+19;
		waende->tiefe=61;								/* Platte */
		waende->knuepfen=FALSE;
		waende->ob_x=16;
		if (is_door(bunker[x3][y3])) {
			waende->ob_x=0;
			waende->knuepfen=TRUE;					/* offener TÅrrahmen ist transparent */
			waende->modus=ODER;
			waende++->ob_nr=10;						/* TÅrrahmen */
			if (!is_open_door(bunker[x3][y3])) {
				(waende-1)->knuepfen=FALSE;			/* geschlossener TÅrrahmen ist nicht transparent */
				tuer=bunker[x3][y3];
				if (is_aufzug_door(x3,y3)) tuer=1;				/* SchiebetÅr */
				switch(tuer%3) {
					case 0:
					case 1:
						waende->x1=76;
						waende->x2=76+objekt_breite(17,walls)-1;
						waende->y=46+29;
						waende->tiefe=60;
						waende->ob_nr=17;								/* SchiebetÅr */
						waende->knuepfen=TRUE;
						waende++->ob_x=0;
						break;
					case 2:
						waende->x1=76;
						waende->x2=76+objekt_breite(18,walls)-1;
						waende->y=46+29;
						waende->tiefe=60;
						waende->ob_nr=18;								/* VierertÅr */
						waende->knuepfen=TRUE;
						waende++->ob_x=0;
						break;
					}
				}
			}
		else
			waende++->ob_nr=7;
		}

	set_enemies(&waende);					/* Setzt die Gegner in die Bunker */
	set_items(&waende);						/* Und noch die Items drauflegen */

	return(waende-start);					/* Soviel EintrÑge wurden benutzt */
}

void set_bunker_pal()
{
	/* Je nachdem, wie stark die Taschenlampe ist, wird die entsprechende Bunkerpalette 
		 gesetzt */
	int i;
		
	for(i=0;i<16;i++) bunker_raster[i]=(tv_modus) ? tv_pal[i] : bunker_pal[i];	/* Palette Åbertragen */
	
	set_raster(0,45,bunker_raster);
	fade_raster(45,6-helligkeit);				/* und entsprechend abschwÑchen */
	show_raster();										/* Und anzeigen */
}

void do_taschenlampe()
{
	/* Von Zeit zu Zeit wird die Taschenlampe schwÑcher */
	FLAG dunkler=FALSE;
		
	if (vbl_ct>lampencounter+6*50*60) {
		lampencounter=vbl_ct;					/* Alle 6 Minuten eine Einheit */
		dunkler=TRUE;
		if (ycargo_menge[10]>0) {
			ycargo_menge[10]--;					/* Eine Batterie weniger */
			draw_lampenanzeige();
			}
		}

	helligkeit=ycargo_menge[10];
	if (helligkeit>6) helligkeit=6;			/* Volle LampenstÑrke */
	
	if (dunkler) set_bunker_pal();
}

void draw_whole_automap()
{
	/* Zeichnet die gesamte Automap neu */
	register y,x;
	int wert;
	int farbe;
	
	fill(0,194,48,319,167);						/* Weglîschen */
	
	if (automapping) 
		for(y=0;y<30;y++)
			for(x=0;x<31;x++) {
				wert=get_automap(x+auto_x,y+auto_y);
				if (wert!=0) draw_automap(wert,x+auto_x,y+auto_y);
				}
	else {
		writexy_fast(240,100,romstr491);
		writexy_fast(240+16,106,romstr492);
		rahmen(15,238,98,242+12*4,112);
		}
}

void draw_automap(wert,x,y)
int wert;
int x,y;											/* Angabe in AutomapKoordinaten */
{
	/* zeichnet einen Kasten der Automapping Funktion.
		 Angabe der Koordinaten in Automapping-Koordinaten, Wert als Wert des Arrays */
	int farbe;
		
	switch (wert) {
		case 0: farbe=0; break;					/* Nichts */
		case 1:	farbe=13; break;					/* Wand */
		case 2:	if (is_open_door(bunker[x+3][y+3])) farbe=2;	/* Geîffnete TÅr */
						else farbe=10;							/* geschlossene TÅr */
						break;
		case 3: farbe=6; break;					/* Item */
		case 4: farbe=15; break;					/* Spieler */
		}
	x-=auto_x;
	y-=auto_y;
	if (x>=0 && x<31 && y>=0 && y<30)
		draw_kasten(farbe,194+x*4,48+y*4);
}

void draw_kasten(farbe,x,y)
int farbe,x,y;
{
	/* Zeichnet einen Kasten in die Automapping-Funktion */
	int line_color;
	
	fill(farbe,x,y,x+2,y+2);

	line_color=(farbe!=0) ? 11 : 0;
	line(line_color,x+1,y+3,x+3,y+3);					/* Schatten */
	line(line_color,x+3,y+1,x+3,y+2);
}

void map_update()
{
	/* Der Spieler hat sich ein Feld weiter bewegt, jetzt muû die Automap auf den neuesten
		 Stand gebracht werden. */

	auto_nr=0;											/* Bisher keine Updates */

	if ((bunker_x-3)-auto_x<2) {
		if (auto_x>=5) {
			auto_x-=5;
			draw_whole=1;
			}
		else if (auto_x!=0) {
			auto_x=0;
			draw_whole=1;									/* Noch einmal gesamte Karte zeichnen */
			}
		}
	if ((bunker_y-3)-auto_y<3) {
		if (auto_y>=5) {
			auto_y-=5;							/* Scroll nach oben */
			draw_whole=1;
			}
		else if (auto_y!=0) {
			auto_y=0;
			draw_whole=1;
			}
		}
	if ((bunker_y-3)-auto_y>27) {
		auto_y+=5;
		draw_whole=1;
		}
	if ((bunker_x-3)-auto_x>28) {
		auto_x+=5;
		draw_whole=1;
		}
	if (draw_whole>=1) draw_whole_automap();
	
	if (is_wand(bunker[bunker_x-1][bunker_y])) insert_automap(1,bunker_x-1,bunker_y);
	if (is_wand(bunker[bunker_x+1][bunker_y])) insert_automap(1,bunker_x+1,bunker_y);
	if (is_wand(bunker[bunker_x][bunker_y-1])) insert_automap(1,bunker_x,bunker_y-1);
	if (is_wand(bunker[bunker_x][bunker_y+1])) insert_automap(1,bunker_x,bunker_y+1);
	if (is_wand(bunker[bunker_x-1][bunker_y-1])) insert_automap(1,bunker_x-1,bunker_y-1);
	if (is_wand(bunker[bunker_x+1][bunker_y+1])) insert_automap(1,bunker_x+1,bunker_y+1);
	if (is_wand(bunker[bunker_x+1][bunker_y-1])) insert_automap(1,bunker_x+1,bunker_y-1);
	if (is_wand(bunker[bunker_x-1][bunker_y+1])) insert_automap(1,bunker_x-1,bunker_y+1);
	
	if (is_door(bunker[bunker_x-1][bunker_y])) insert_automap(2,bunker_x-1,bunker_y);
	if (is_door(bunker[bunker_x+1][bunker_y])) insert_automap(2,bunker_x+1,bunker_y);
	if (is_door(bunker[bunker_x][bunker_y-1])) insert_automap(2,bunker_x,bunker_y-1);
	if (is_door(bunker[bunker_x][bunker_y+1])) insert_automap(2,bunker_x,bunker_y+1);
	if (is_door(bunker[bunker_x-1][bunker_y-1])) insert_automap(2,bunker_x-1,bunker_y-1);
	if (is_door(bunker[bunker_x+1][bunker_y+1])) insert_automap(2,bunker_x+1,bunker_y+1);
	if (is_door(bunker[bunker_x+1][bunker_y-1])) insert_automap(2,bunker_x+1,bunker_y-1);
	if (is_door(bunker[bunker_x-1][bunker_y+1])) insert_automap(2,bunker_x-1,bunker_y+1);
}	

void draw_updates()
{
	/* Zeichnet auf dem aktuellen AutomapScreen die Updates, die noch nicht gezeichnet wurden */
	int i;
	
	if (draw_whole>=1) {
		draw_whole--;
		draw_whole_automap();
		}
	
	if (player[0].x>=0)									/* Spieler vorhanden? */
		draw_automap(get_automap(player[0].x,player[0].y),player[0].x,player[0].y);	/* Alten Spieler lîschen */
	
	for(i=0;i<auto_nr;i++) 	
		draw_automap(automap_2[i].wert,automap_2[i].x,automap_2[i].y);
		
	auto_nr=0;													/* Keine Updates mehr */
}

void draw_player()
{
	/* Zeichnet den Spieler in die Automap */
	
	player[0]=player[1];							/* Jetzt ein Screen weiter */
	
	player[1].x=bunker_x-3;						/* -3 wegen Bunkerrand */
	player[1].y=bunker_y-3;
	draw_automap(4,bunker_x-3,bunker_y-3);		/* Spieler einzeichnen */
}

void insert_automap(wert,x,y)
int wert;
int x,y;
{
	/* An der Stelle x,y soll der Wert [0..3] eingetragen werden */

	x-=3;													/* Bunker startet bei x=y=3 */
	y-=3;
	put_automap(wert,x,y);				/* In Array eintragen */
	draw_automap(wert,x,y);

	if (auto_nr<16) {
		automap_2[auto_nr].x=x;
		automap_2[auto_nr].y=y;
		automap_2[auto_nr++].wert=wert;		/* fÅr 2. Screen merken */
		}
}

void put_automap(wert,x,y)
int wert;
int x,y;
{
	/* Speichert den Wert an Position x,y (80x50, d.h. ohne Rand) ab. */

	put_2bit(automap,y*80+x,wert);
}

int get_automap(x,y)
int x,y;
{
	/* Holt sich den Wert aus Position x,y (80x50, d.h. ohne Rand) */
	register int index,shift;
	
	if (x<0 || y<0 || x>79 || y>49) return(0);					/* Clippen */

	return(get_2bit(automap,80*y+x));
}

void bedien_item(x,y)
int x,y;															/* Position im Bunker */
{
	/* User will mit Items arbeiten */
	
	ITEMS *itemp;
	int item_nr=-1;								
	int member_nr;
	int such_x,such_y;
	int pos_index;
	BUNKERLAGE pos[4];
	int hand,member;
	unsigned int ende;
	int mauspos;
	long wert;
	
	/* Welches BodenstÅck im Bunker reprÑsentiert welche Position? 
		 pos[0]=hinten links
		 pos[1]=hinten rechts
		 pos[2]=vorne links
		 pos[3]=vorne rechts */
		 
	switch (richtung) {
		case NORD:	pos[0].feld=2; pos[0].distance=1;
								pos[1].feld=1; pos[1].distance=1;
								pos[2].feld=3; pos[2].distance=0;
								pos[3].feld=0; pos[3].distance=0; break;
		case OST:		pos[0].feld=3; pos[0].distance=1;
								pos[1].feld=2; pos[1].distance=1;
								pos[2].feld=0; pos[2].distance=0;
								pos[3].feld=1; pos[3].distance=0; break;
		case SUED:	pos[0].feld=0; pos[0].distance=1;
								pos[1].feld=3; pos[1].distance=1;
								pos[2].feld=1; pos[2].distance=0;
								pos[3].feld=2; pos[3].distance=0; break;
		case WEST:	pos[0].feld=1; pos[0].distance=1;
								pos[1].feld=0; pos[1].distance=1;
								pos[2].feld=2; pos[2].distance=0;
								pos[3].feld=3; pos[3].distance=0; break;
		}

	do {
		if (item_nr!=-1) {
			do {
				if (mousey>46) {
					ende=move_objekt(item[item_nr].grafik,items,ODER,0,hlpbuf,
									STOP_R|STOP_L|STOP_U|STOP_O|ENDE_O,0,46,319,168,TRUE);
					mauspos=44-objekt_hoehe(item[item_nr].grafik,item_rea)/2;
					if (ende!=0 && mousey>mauspos) set_mousepos(mousex,mauspos);
					}
				else {
					ende=move_objekt(item[item_nr].grafik,item_rea,ODER,0,hlpbuf,
									STOP_R|STOP_L|STOP_U|ENDE_U|STOP_O,0,0,319,45,TRUE);
					mauspos=47+objekt_hoehe(item[item_nr].grafik,items)/2;
					if (ende!=0 && mousey<mauspos) set_mousepos(mousex,mauspos);
					}
				} while(ende!=0);								/* tatsÑchlicher Abbruch */	
			auto_nr=0;												/* Alle Changes sind Åbertragen */
			player[0]=player[1];							/* Und Spieler an selber Position */
			draw_whole=0;											/* Kein Neuzeichnen der Automap */
			}
		else hol_maus();
		
		if (mk==1 && my<=42) {						/* In die Hand nehmen... */
			if (my<26) hand=0;							/* Linke Hand? */
			else hand=1;										/* Rechte Hand? */
			member=mx/80;
			if (maus_in(member*80+10,1,member*80+27,7) && item_nr==-1) {		/* Schiessflag */
				party[member].schiessen=!party[member].schiessen;
				draw_schiessflag(member);
				}
			else {
				swap(&party[member].hand[hand],&item_nr);
				draw_haende();									/* Die HÑnde neu zeichnen */
				}
			wait_mouse(0);
			}
		if (maus_in(0,46,144,124)) {					/* TÅre îffnen 77,104,106,114 */
			item_nr=bedien_tuer(item_nr);
			}
		if (maus_in(24,125,167,167)) {					/* User will Item aufnehmen/Ablegen */
			if (my<147) pos_index=0;					/* Hinten */
			else pos_index=2;								/* Vorne */
			if (mx>=96) pos_index++;					/* Rechts */

			such_x=x+bunker_dx*pos[pos_index].distance;		/* Feld, auf das Spieler klickt */
			such_y=y+bunker_dy*pos[pos_index].distance;
			
			if (item_nr==-1) {							/* Er hat keines -> aufnehmen */
				for(itemp=item,item_nr=0;item_nr<ITEMMAX;item_nr++,itemp++)
					if (itemp->bun_nr==akt_bunker && 
								itemp->bun_x==such_x && itemp->bun_y==such_y && itemp->pos==pos[pos_index].feld) break;
				if (item_nr>=ITEMMAX) item_nr=-1;			/* Kein Item vorhanden */
				else {	
					switch (item[item_nr].typ) {
						case SCHATZTRUHE:
							item[item_nr].typ=LEERE_TRUHE;
							item[item_nr].grafik=70;

							if (akt_bunker==21 || akt_bunker==22) 
								wert=1000+zufall(2000);		/* Startbunker */
							else {
								wert=20000L+zufall(20000);
								schatznummer++;
								}

							if (schatznummer==2 || schatznummer==4 || schatznummer==10) {
								spieler_status++;
								dungeon_alert(build(romstr493,s_status[spieler_status]));
								switch (spieler_status) {
									case MAGIER:
										produce_cheat=TRUE;
										break;
									case MOGUL:
										strength_cheat=automapping=TRUE;
										draw_whole=2;
										break;
									case ZAUBERMEISTER:
										schuss_cheat=TRUE;
										break;
									}
								}
							else {
								dungeon_alert(build(romstr494,str(0,wert)));
								money+=wert;
								}
						case LEERE_TRUHE:
							item_nr=-1;							/* Kann nicht aufgenommen werden */							
							break;
						default:
							item[item_nr].bun_nr=254;				/* Item jetzt weg */
							break;
						}
					draw_bunker(FALSE);						/* Objekt weglîschen */
					}
				}
			else {													/* Item ablegen */
				itemp=&item[item_nr];					
				itemp->bun_nr=akt_bunker;
				itemp->bun_x=such_x;
				itemp->bun_y=such_y;
				itemp->pos=pos[pos_index].feld;
				item_nr=-1;										/* Item ist jetzt weg */
				put_1bit(itemmap,(such_x-3)+(such_y-3)*80,1);	/* Hier liegt was */
				draw_bunker(FALSE);
				wait_mouse(0);
				}
			}
			
		if (mk==2) {							/* Item zur Figur nehmen */
			if (my<46) member_nr=mx/80;
			else member_nr=0;
			item_nr=belade_figur(item_nr,member_nr);		/* Figur will mit Objekt 'i' beladen werden */
			}				
		} while(item_nr!=-1);							/* Solange er noch was in der Hand hat */

	while (is_key()) wait_key();						/* Tastaturbuffer lîschen */
	hol_maus();
}

int belade_figur(item_nr,member_nr)
int item_nr;
int member_nr;
{
	/* der Spieler will eine Figur beladen und hat Objekt 'i' in der Hand */
	int tasche_nr;
	int tmp;
	FLAG neuer_member;
	int hand;
	int nr;
	int rasterpos;
	int ende;
	int mauspos;
	int reagenz;
	long laenge;
	long old_buttons;
	
	old_buttons=bleiste;
	redraw_buttons(PAUSE|EXIT_BTN);				/* Nur Exit ist offen */
	
	if (is_tot(member_nr)) member_nr=0;		/* Wenn tot auf Spieler gehen */

	laenge=load_objekte(MENSCH_OBJ,walls);				/* öber die WÑnde laden */
	prisoner_obj=walls+laenge;
	load_objekte(PRISONER_OBJ,prisoner_obj);
	
	Hm();
	draw_obj(0,walls,MOVE,scr1,0,45);					/* Formular zeichnen */
	draw_obj(1,walls,MOVE,scr1,0,rasterpos=45+objekt_hoehe(0,walls));	/* Formular zeichnen 2 */
	show_raster();
	draw_obj(3,walls,MOVE,scr1,200,54);					/* Auge zeichnen */
	draw_mund(4);										
	Sm();
	
	neuer_member=TRUE;
		
	do {
		if (neuer_member) {
			neuer_member=FALSE;
			draw_taschen(member_nr);					/* Alle Tascheninhalte zeichnen */
			anziehen(member_nr);
			bild_ausgabe(member_nr);					/* Bild des Prisoners zeigen */
			}
		
		button=NOTHING;
		
		if (item_nr!=-1) {
			if (item[item_nr].typ<HELM || item[item_nr].typ>SCHUHE) {		/* Keine KleidungsstÅcke */
				move_objekt(item[item_nr].grafik,item_rea,ODER,0,hlpbuf,
															STOP_R|STOP_L|STOP_O|STOP_U,0,0,319,rasterpos,TRUE);
				}
			else 													
				do {
					if (mousey>rasterpos) {
						ende=move_objekt(item[item_nr].grafik-51+7,walls,MOVE,0,hlpbuf,
										STOP_R|STOP_L|STOP_U|STOP_O|ENDE_O,0,rasterpos,319,168,TRUE);
						mauspos=rasterpos-2-objekt_hoehe(item[item_nr].grafik,item_rea)/2;
						if (ende!=0 && mousey>mauspos) set_mousepos(mousex,mauspos);
						}
					else {
						ende=move_objekt(item[item_nr].grafik,item_rea,ODER,0,hlpbuf,
										STOP_R|STOP_L|STOP_U|ENDE_U|STOP_O,0,0,319,rasterpos-1,TRUE);
						mauspos=rasterpos+1+objekt_hoehe(item[item_nr].grafik-51+7,walls)/2;
						if (ende!=0 && mousey<mauspos) set_mousepos(mousex,mauspos);
						}
					} while(ende!=0);								/* tatsÑchlicher Abbruch */	
			}
		else {
			hol_maus();
			if (my>169) button_leiste();
			}
		
		if (item_nr!=-1) {
			if (maus_in(252,49,281,rasterpos)) {					/* Mund */
				switch (item[item_nr].typ) {
					case FOOD: reagenz=0; break;
					case WATER: reagenz=1; break;
					case HILFE: reagenz=2; break;
					default: reagenz=-1; break;
					}
				if (reagenz>=0) {
					mund(4);											/* Mund bewegen */
					subtract_reagenz(member_nr,reagenz,item[item_nr].flag);	/* NÑhrwert */
					item[item_nr].bun_nr=254;					/* Objekt ist weg */
					item_nr=-1;										/* und weg bei der Maus */
					charakter_ausgabe(member_nr);					/* Und Werte neu ausgeben */
					}
				else alert(romstr495);
				}
			else if (maus_in(194,48,220,rasterpos)) {				/* Auge */
				schauen(item_nr);
				}
			}
		else {
			nr=mx/80;
			if (maus_in(nr*80+10,1,nr*80+27,7)) {		/* Schiessflag */
				party[nr].schiessen=!party[nr].schiessen;
				draw_schiessflag(nr);
				}
			}
			
		if (maus_in(217,87,258,120)) {						/* Oberteil */
			if (item_nr==-1 || (item_nr!=-1 && item[item_nr].typ==HEMD)) {
				swap(&party[member_nr].oberteil,&item_nr);
				anziehen(member_nr);
				wait_mouse(0);
				}
			}
		else if (maus_in(217,120,258,140)) {				/* Hose */
			if (item_nr==-1 || (item_nr!=-1 && item[item_nr].typ==HOSE)) {
				swap(&party[member_nr].unterteil,&item_nr);
				anziehen(member_nr);
				wait_mouse(0);
				}
			}
		else if (maus_in(228,63,248,89)) {					/* Helm */
			if (item_nr==-1 || (item_nr!=-1 && item[item_nr].typ==HELM)) {
				swap(&party[member_nr].helm,&item_nr);
				anziehen(member_nr);
				wait_mouse(0);
				}
			}
		else if (maus_in(228,140,248,154)) {					/* Schuhe */
			if (item_nr==-1 || (item_nr!=-1 && item[item_nr].typ==SCHUHE)) {
				swap(&party[member_nr].schuhe,&item_nr);
				anziehen(member_nr);
				wait_mouse(0);
				}
			}
			
		if (maus_in(151,72,161,81)) {					/* Close */
			break;	
			}

		if (mk==1 && my<=42) {						/* In die Hand nehmen... */
			nr=mx/80;												/* Welches Member? */
			if (!is_tot(nr)) 
				if (nr==member_nr) {						/* das mit den Taschen? */
					tasche_nr=(mx-nr*80-3)/20+((my-9)/18)*4;		/* Nummer der Tasche berechnen */
					swap(&party[nr].tasche[tasche_nr],&item_nr);
					draw_tasche(tasche_nr,nr);
					}
				else {
					if (my<26) hand=0;							/* Linke Hand? */
					else hand=1;										/* Rechte Hand? */
					swap(&party[nr].hand[hand],&item_nr);
					draw_hand(nr);									/* Die HÑnde neu zeichnen */
					}
			wait_mouse(0);
			}
		if (mk==2 && my<46) {						/* Neuen Member wÑhlen */
			if (mx/80!=member_nr) {					/* nicht derselbe? */
				if (!is_tot(mx/80)) {
					neuer_member=TRUE;
					draw_rea(member_nr);					/* Reagenzglas beim alten Member */
					member_nr=mx/80;
					}
				else break;									/* Rechtsklick auf Toter->Raus */
				}
			mk=0;
			}
		} while (!(button==EXIT_BTN || mk==2 && my>46));	/* Bis zum Rechtsklick auf obere Leiste */
			
	Hm();
	draw_rea(member_nr);							/* und zurÅck zu den HÑnden */
	copy_reagenz();
	Sm();
	
	load_objekte(BUNKER_OBJ,walls);				/* WÑnde wieder einladen */
	
	auto_nr=0;												/* Alle Changes sind Åbertragen */
	player[0]=player[1];							/* Und Spieler an selber Position */
	draw_whole=1;											/* Und neu zeichnen der Automap */

	redraw_buttons(old_buttons);
	copy_buttons();
	
	set_raster(0,rasterpos,NULL);				/* Figurraster lîschen */
	draw_bunker(FALSE);
	set_bunker_pal();
	show_raster();
	
	Hm();	
	fill(0,192,45,319,167);						/* Streifen wegmachen */
	draw_whole_automap();
	copy_screen(scr1,scr2);						
	Sm();
	
	button=NOTHING;
	return(item_nr);									/* Spieler hat dieses Objekt in der Hand */
}

void draw_haende()
{
	/* zeichnet alle HÑnde neu */
	int member;
	
	Hm();
	for(member=0;member<PARTY;member++) draw_hand(member);
	cpy_raster(scr1,scr2,0,0,319,42,0,0);
	Sm();
}

void draw_hand(member)
int member;
{
	/* zeichnet die Hand eines Members neu */
	int index;
	int hand;
	int x,y;

	if (!is_tot(member)) {
		Hm();
		draw_obj_part(0,reagenz,10,9,30,43,MOVE,scr1,member*80+10,9);	/* Altes lîschen */
	
		x=19+member*80;
		y=10+6;
		for(hand=0;hand<2;hand++,y+=18) {
			index=party[member].hand[hand];
			if (index!=-1) 
				draw_obj(item[index].grafik,item_rea,ODER,scr1,
								x-objekt_breite(item[index].grafik,item_rea)/2,y-objekt_hoehe(item[index].grafik,item_rea)/2);
			}
		Sm();
	}
}							

void draw_taschen(member_nr)
int member_nr;
{
	/* Zeichnet alle Taschen des Members */
	
	int i;
	void *oldlogbase;

	Hm();	
	draw_obj(2,reagenz,MOVE,scr1,member_nr*80,0);			/* Hintergrund zeichnen */
	draw_schiessflag(member_nr);
	oldlogbase=logbase;
	logbase=scr1;
	if (member_nr==0) {
		writexy(0,32,1,player_name);
		}
	else {
		writexy(0,member_nr*80+32,1,prisoner[party[member_nr].prisoner_nr].name);
		}
	
	logbase=oldlogbase;

	for(i=0;i<8;i++) {
		draw_tasche(i,member_nr);
		}
	Sm();
}

void draw_tasche(tasche_nr,member_nr)
int tasche_nr;
int member_nr;
{
	/* Zeichnet das Objekt, welches sich in der Tasche befindet */
	
	int x,y;
	int ob_nr;
	
	Hm();

	x=member_nr*80+(tasche_nr%4)*19+3;						/* linkes, oberes Eck der Tasche */
	y=(tasche_nr/4)*18+8;
	draw_obj_part(2,reagenz,x-member_nr*80,y,x-member_nr*80+19,y+18,MOVE,scr1,x,y);

	if (party[member_nr].tasche[tasche_nr]!=-1) {
		ob_nr=item[party[member_nr].tasche[tasche_nr]].grafik;
		draw_obj(ob_nr,item_rea,ODER,scr1,
								x+8-objekt_breite(ob_nr,item_rea)/2,y+8-objekt_hoehe(ob_nr,item_rea)/2); 
		} 
	Sm();
}

void charakter_ausgabe(member)
int member;
{
	/* Gibt die Charakterwerte aus */
	int i;
	
	Hm();	

	writexy(0,26,90, romstr496);
	writexy(0,26,98, romstr497);
	writexy(0,26,106,romstr498);
	
	for(i=0;i<3;i++) {
		writexy(0,26+12*4,90+i*8,str(3,(long)party[member].reagenz[i]*100/255));
		}
		
	writexy(0,26,116,build(romstr499,str(3,(long)party[member].protection)));
	
	writexy(0,26,135,romstr500);
	writexy(0,26+8*4,135,(member==0) ? s_status[spieler_status] : romstr501);

	Sm();
}

void bild_ausgabe(member)
int member;
{
	/* Gibt die Charakterwerte aus */
	int prisoner_nr;
	int breite;
	char *name;
	
	Hm();	
	if (member==0) {
		prisoner_nr=10;
		name=player_name;
		}
	else {
		prisoner_nr=party[member].prisoner_nr;
		name=prisoner[prisoner_nr].name;
		}
	breite=objekt_breite(prisoner_nr,prisoner_obj);
	fill(7,102,86,146,132);
	draw_obj(prisoner_nr,prisoner_obj,MOVE,scr1,146-breite,86);		/* Kopf zeichnen */
	writexy(0,146-breite/2-strlen(name)*2,
											86+4+objekt_hoehe(prisoner_nr,prisoner_obj),name);
	
	Sm();
}

void schauen(item_nr)
int item_nr;
{
	/* Auge schaut */
	char *text;
		
	Hm();
	draw_obj(2,walls,MOVE,scr1,200,54);			/* Auge zeichnen */
	if (item_nr!=-1) {
		switch (item[item_nr].typ) {
			case FOOD: text=romstr502; break;
			case WATER: text=romstr503; break;
			case HILFE: text=romstr504; break;
			case KEY: if (item_nr<11 || item_nr==119 || item_nr==131 || item_nr==132)
								text=romstr505;
							else 
								text=romstr506;
							break;
			case WAFFE: text=romstr507; break;
			case MUNITION: text=build(romstr508,
															str(0,(long)item[item_nr].flag)); break;
			case HELM: text=romstr509; break;
			case HEMD: text=romstr510; break;
			case HOSE: text=romstr511; break;
			case SCHUHE:	text=romstr512; break;
			case PERGAMENT: text=romstr513; break;
			default: text=romstr514; break;
			}
		alert(text);
		}
	wait_mouse(0);
	draw_obj(3,walls,MOVE,scr1,200,54);			/* Auge zeichnen */
	Sm();
}

void mund(start)
int start;
{
	int i,j;

	for(i=0;i<6;i++) {								/* 10 mal kauen */
		draw_mund(start+1);
		wait_sync(8);
		for(j=start+2;j>=start;j--) {
			draw_mund(j);
			wait_sync(8);
			}
		}
}

void draw_mund(ob_nr)
int ob_nr;
{
	Hm();
	draw_obj(ob_nr,walls,MOVE,scr1,260,54);
	Sm();
}

void anziehen(member)
int member;
{
	/* Zieht die Figur entsprechend der Kleidung des Members an */
	int item_nr;
	long prot;
	
	Hm();
	draw_obj_part(1,walls,216,5,257,89,MOVE,scr1,216,45+objekt_hoehe(0,walls)+5);	/* Figur lîschen */
	draw_kleidung(party[member].helm,237,75,176+8,83);
	draw_kleidung(party[member].oberteil,237,90,272+8,93);
	draw_kleidung(party[member].unterteil,237,118,172+8,127);
	item_nr=party[member].schuhe;
	if (item_nr!=-1) draw_kleidung(item_nr,237,156-objekt_hoehe(item[item_nr].grafik-51+7,walls),272+8,148);
	else writexy(0,272+8,148,romstr515);

	prot=prot_erhoeh(party[member].helm,20);
	prot+=prot_erhoeh(party[member].oberteil,35);
	prot+=prot_erhoeh(party[member].unterteil,30);
	prot+=prot_erhoeh(party[member].schuhe,10);

	party[member].protection=prot/(20+35+30+10);
	
	charakter_ausgabe(member);
	
	Sm();
}

long prot_erhoeh(item_nr,wertung)
int item_nr;
int wertung;					/* Wie ist das KleidungsstÅck gewichtet */
{
	if (item_nr!=-1) return(item[item_nr].flag*wertung);
	else return(0);
}

void draw_kleidung(item_nr,x_center,y,text_x,text_y)
int item_nr;
int x_center;
int y;
int text_x,text_y;									/* Koordinaten der Prozentangabe */
{
	/* Zeichnet ein KleidungsstÅck */
	int ob_nr;
	int prozent;
	
	if (item_nr!=-1) {
		ob_nr=item[item_nr].grafik-51+7;
		draw_obj(ob_nr,walls,MOVE,scr1,x_center-objekt_breite(ob_nr,walls)/2,y);
		prozent=item[item_nr].flag;
		}
	else prozent=0;

	writexy(0,text_x,text_y,build(romstr516,str(3,(long)prozent)));
}

int get_1bit(feld,pos)
unsigned char feld[];
int pos;
{
	/* Holt aus einem Feld aus Bytes den Eintrag an Position 'pos', wobei jeder Eintrag
	   1 Bit groû ist. */
	register int index,shift;
	
	index=pos/8;
	shift=7-(pos%8);
	return((feld[index]>>shift)&1);
}

void put_1bit(feld,pos,wert)
unsigned char feld[];
int pos;
int wert;
{
	/* Speichert in einem Feld aus Bytes den Eintrag an Position 'pos', wobei jeder Eintrag
	   1 Bit groû ist. */
	register int index;
	register int shift;
	register unsigned char maske;
	
	index=pos/8;
	shift=7-(pos%8);
	maske=~(1<<shift);						/* Maske zum Lîschen bereechnen */
	feld[index]&=maske;						/* ausmaskieren */
	feld[index]|=wert<<shift;			/* einodern */
}

int get_2bit(feld,pos)
unsigned char feld[];
int pos;
{
	/* Holt aus einem Feld aus Bytes den Eintrag an Position 'pos', wobei jeder Eintrag
	   2 Bit groû ist. */
	register int index,shift;
	
	index=pos/4;
	shift=6-(pos%4)*2;
	return((feld[index]>>shift)&3);
}

void put_2bit(feld,pos,wert)
unsigned char feld[];
int pos;
int wert;
{
	/* Speichert in einem Feld aus Bytes den Eintrag an Position 'pos', wobei jeder Eintrag
	   2 Bit groû ist. */
	register int index;
	register int shift;
	register unsigned char maske;
	
	index=pos/4;
	shift=6-(pos%4)*2;
	maske=~(3<<shift);						/* Maske zum Lîschen bereechnen */
	feld[index]&=maske;						/* ausmaskieren */
	feld[index]|=wert<<shift;			/* einodern */
}

FLAG liegt_item(x,y)
int x,y;
{
	/* öberprÅft, ob an dieser Stelle Items liegen */
	
	return(get_1bit(itemmap,(x-3)+(y-3)*80)==1);
}

FLAG is_aufzug_door(x,y)
int x,y;
{
	/* öberprÅft, ob es sich bei diesem Feld um eine AufzugstÅre handelt */
	
	if (is_door(bunker[x][y]) && 
				(is_aufzug(bunker[x-1][y]) ||
				is_aufzug(bunker[x+1][y]) ||			
				is_aufzug(bunker[x][y-1]) ||			
				is_aufzug(bunker[x][y+1]))) return(TRUE);
	else return(FALSE);
}

FLAG is_aufzug(feld)
unsigned int feld;
{
	/* öberprÅft, ob dieses Feld ein Aufzugsfeld ist */
	
	return(feld>=LIFT_START && feld<=LIFT_END);
}

FLAG is_door(feld)
unsigned int feld;
{
	/* öberprÅft, ob dieses Feld eine TÅr ist (offen oder geschlossen) */

	return (feld==START_POS || (feld>=DOOR_ZU_START && feld<DOOR_AUF_END) || 
														(feld>=PRISONER_START && feld<=PRISONER_END));
}

FLAG is_open_door(feld)
unsigned int feld;
{
	/* öberprÅft, ob Feld eine geîffnete TÅre ist: */
	
	return(feld>=DOOR_AUF_START && feld<=DOOR_AUF_END);
}

FLAG betretbar(feld)
unsigned int feld;
{
	/* öberprÅft, ob Feld in Bunker betretbar ist */

	if (feld==START_POS || feld==WALL || (feld>=DOOR_ZU_START && feld<=DOOR_ZU_END) ||
						(feld>=PRISONER_START && feld<=PRISONER_END))
		return(FALSE);
	else
		return(TRUE);
}

FLAG enemy_betretbar(x,y)
unsigned int x,y;
{
	/* öberprÅft, ob ein Feld im Bunker fÅr die Feinde betretbar ist */
	
	return(betretbar(bunker[x][y]) && !is_aufzug_door(x,y));
}

FLAG is_wand(feld)
unsigned int feld;
{
	/* öberprÅft, ob dieses Feld entweder eine Wand, oder eine TÅr ist */

	return (feld==START_POS || feld==HIDDEN_WALL || feld==WALL || 
					(feld>=DOOR_ZU_START && feld<=DOOR_AUF_END) || (feld>=PRISONER_START && feld<=PRISONER_END));
}

void init_dungeon(nr,startfeld)
int nr;
unsigned int startfeld;
{
	/* Holt sich das Dungeon von der Disk und initialisiert es */
	int i;
	register int x,y;
	unsigned int objekt;
	unsigned int tuer_nr,tuer;
	unsigned int tuer_ist_status,tuer_soll_status;
	unsigned int feld;
	unsigned int prisoner_nr;
	
	load_dungeon(nr); 			 				/* LÑdt Dungeon von Disk */

	bunker_x=-1;
	/* Startposition suchen: */
	for(x=0;x<86;x++)
		for(y=0;y<56;y++) {
			feld=bunker[x][y];
			if (feld==startfeld) {		 /* Startposition suchen */
				bunker_x=x;
				bunker_y=y;
				}
			
			if (feld==START_POS && nr==21) bunker[x][y]=LEER;	/* Startfeld in Bunker #0 lîschen */
			
			if (feld>=PRISONER_START && feld<=PRISONER_END) {	/* Prisoner */
				prisoner_nr=feld-PRISONER_START;
				if (prisoner[prisoner_nr].befreit) {
					bunker[x][y]=DOOR_AUF_START;	/* Schon befreit */
					}
				}
			if (feld>=ITEM_START && feld<=ITEM_END) {		/* Item? */
				if (feld>=NOKEY_START) {
					if (!bunker_betreten[nr]) {
						objekt=feld-NOKEY_START;					
						if (platz_in_item()) {				/* kann noch aufgenommen werden */
							switch(objekt) {
								case 0:
									item[item_max].bun_nr=nr;
									item[item_max].bun_x=x;
									item[item_max].bun_y=y;				/* Item aufnehmen */
									item[item_max].pos=zufall(4);
									item[item_max].typ=SCHATZTRUHE;
									item[item_max++].grafik=66;			/* Schatzkiste */
									break;
								case 1:
									item[item_max].bun_nr=nr;
									item[item_max].bun_x=x;
									item[item_max].bun_y=y;				/* Item aufnehmen */
									item[item_max].pos=zufall(4);
									item[item_max].typ=PERGAMENT;
									item[item_max++].grafik=69;			/* Pergament */
									break;
								default: internal(romstr517); break;
								}								
							}
						}
					}
				else {
					objekt=feld-ITEM_START;					/* SchlÅsselnummer bestimmen */
					if (item[objekt].bun_nr==255) {			/* Wurde Item schon bewegt? */
						item[objekt].bun_nr=nr;				/* In diesem Bunker */
						item[objekt].bun_x=x;
						item[objekt].bun_y=y;
						item[objekt].pos=zufall(4);				/* In irgendeiner Ecke */
						}
					}
				bunker[x][y]=LEER;						/* Objekt weg nehmen */
				}

			if (feld>=DOOR_ZU_START && feld<=DOOR_AUF_END) {		/* TÅr? */
				if (is_open_door(feld)) {
					tuer_nr=feld-DOOR_AUF_START;
					tuer_ist_status=AUF;
					}
				else {
					tuer_nr=feld-DOOR_ZU_START;
					tuer_ist_status=ZU;
					}
				tuer=get_2bit(tueren,tuer_nr);							/* Sollstatus der TÅr holen */
				tuer_soll_status=tuer&1;
				if ((tuer&2)!=0) {										/* TÅrstatus noch unbekannt */
					if (tuer_soll_status!=0) 
						tuer_soll_status=1-tuer_ist_status;					/* invertieren */
					else	
						tuer_soll_status=tuer_ist_status;					/* normal nehmen */
					put_2bit(tueren,tuer_nr,tuer_soll_status);				/* -> eintragen */
					}
				bunker[x][y]=(tuer_nr!=0 && tuer_soll_status==AUF) ? DOOR_AUF_START+tuer_nr : DOOR_ZU_START+tuer_nr;
				}
			}

	for(i=0;i<50*80;i++) put_1bit(itemmap,i,0);			/* Alle Items lîschen */
	for(i=0;i<ITEMMAX;i++) {
		if (item[i].bun_nr==nr) {						/* Objekt in diesem Bunker? */
			if (item[i].bun_x==255) {						/* Position undefiniert */
				do {
					x=zufall(80);								/* Leeres Feld suchen */
					y=zufall(50);
					} while(bunker[x][y]!=LEER);
				item[i].bun_x=x;
				item[i].bun_y=y;							/* Objekt an diese Stelle */
				item[i].pos=zufall(4);
				}
			put_1bit(itemmap,(item[i].bun_x-3)+(item[i].bun_y-3)*80,1);	/* Hier liegt was... */
			}
		}
	for(i=0;i<enemy_max;i++) {
		if (enemy[i].bun_nr==nr) {					/* Gegner in diesem Bunker? */
			if (enemy[i].x==255) {						/* Position undefiniert */
				do {
					x=zufall(80);								/* Leeres Feld suchen */
					y=zufall(50);
					} while (!(bunker[x][y]==LEER && (bunker[x-1][y]==LEER || bunker[x][y-1]==LEER || bunker[x+1][y]==LEER || bunker[x][y+1]==LEER)));
				enemy[i].x=x;
				enemy[i].y=y;							/* Gegner an diese Stelle */
				}
			}
		}

	shot.ob_mem=NULL;								/* Schuss unbenutzt */
	
	if (bunker_x==-1) internal(romstr518);

	if (betretbar(bunker[bunker_x+1][bunker_y])) richtung=OST;
	else if (betretbar(bunker[bunker_x][bunker_y+1])) richtung=SUED;
	else if (betretbar(bunker[bunker_x-1][bunker_y])) richtung=WEST;
	else if (betretbar(bunker[bunker_x][bunker_y-1])) richtung=NORD;
	else if (is_door(bunker[bunker_x+1][bunker_y])) richtung=OST;
	else if (is_door(bunker[bunker_x][bunker_y+1])) richtung=SUED;
	else if (is_door(bunker[bunker_x-1][bunker_y])) richtung=WEST;
	else richtung=NORD;

	berechne_richtung();					/* deltas berechnen */
	if (startfeld==START_POS) {
		bunker_x+=bunker_dx;				/* Beim Startfeld eins weitergehen */
		bunker_y+=bunker_dy;
		}
	
	auto_nr=0;												/* Keine VerÑnderung */
	player[0].x=player[1].x=-1;					/* Kein Spieler bisher gezeichnet */
	draw_whole=2;											/* gesamte Karte auf beide Screens zeichnen */
	
	for(i=0;i<80*50/8*2;i++) automap[i]=0;			/* Kein Feld bisher betreten */
	
	auto_x=bunker_x-15;
	auto_y=bunker_y-15;
	if (auto_x<0) auto_x=0;					/* Position des Fensters bestimmen */
	if (auto_y<0) auto_y=0;
	if (auto_x>79-31) auto_x=79-31;
	if (auto_y>49-30) auto_y=49-30;

	bunker_bekannt[nr]=bunker_betreten[nr]=TRUE;			/* Bunkereingang ist bekannt */
}

void load_dungeon(nr)
int nr;
{
	unsigned int xmax,ymax;
	unsigned int objekt;
	register int x,y;
	int i;
	register unsigned int *bunker_buf;
	unsigned int tuer_nr,tuer;
	unsigned int tuer_ist_status,tuer_soll_status;
	
	xmax=ymax=0;

	for(x=0;x<86;x++)
		for(y=0;y<56;y++)
			bunker[x][y]=WALL; 							/* Bunker ist leer */
	
	bunker_buf=scr2;										/* Bunker hier hin laden */
	load_bibliothek(BUNKER0_DAT+nr,bunker_buf);

	xmax=*bunker_buf++;
	ymax=*bunker_buf++;									/* Grîûe des Bunkers holen */

	for(x=0;x<=xmax;x++) 
		for(y=0;y<=ymax;y++) 
			bunker[x+3][y+3]=*bunker_buf++; 				/* Und Bunker Åbertragen */
}

void init_objekte()
{
	/* LÑdt die Objekte von Disk und initialisiert die entsprechenden Strukturen */
	int i,j,bunker;
	int item_nr,item_pos;
	OBJEKT_DISK *obj;
	long laenge;
	STATISTIK *statistik;
	float anz_float,rest_float;
	int anz_int,rest_int;
	float verteilung;
	int anzahl;
	
	for(i=0;i<PARTY;i++) 
		for(j=0;j<8;j++) 
			party[i].tasche[j]=-1;			/* Tasche leer */
	
	for(i=0;i<TUERMAX;i++) {
		item[i].bun_nr=255;					/* Position des SchlÅssels ist unbestimmt */
		item[i].pos=zufall(4);			/* Liegt in irgendeiner Ecke */
		item[i].typ=KEY;
		item[i].grafik=zufall(36);		/* Irgendein SchlÅssel */
		item[i].flag=i+1;						/* FÅr TÅr i */
		}
	
	obj=pack_buf+32000;										/* In hlpbuf laden */
	laenge=load_bibliothek(OBJEKT_DAT,obj);
	if (laenge&1) laenge++;								/* LÑnge gerade machen */
	
	statistik=pack_buf+32000+laenge;					/* Bunkerstatistik hier hin */
	load_bibliothek(BUNKER_DAT,statistik);
	
	item_max=TUERMAX;											/* Ab hier kommen die Items */
	for(item_nr=0;item_nr<ITEMTYPEN;item_nr++) {			/* Soviele verschiedene Items gibt es */
		rest_float=0.0;
		for(bunker=zufall(BUNKERZAHL),anzahl=0;anzahl<BUNKERZAHL;anzahl++,bunker++) {
			if (bunker>=BUNKERZAHL) bunker=0;
			anz_float=((float)statistik[bunker].frei*obj[item_nr].verteilung*2.0)/(100.0*1000.0);
			anz_int=(int)anz_float;
			rest_float+=anz_float-anz_int;				/* Nachkommastellen aufsummieren */
			rest_int=(int)rest_float;
			anz_int+=rest_int;								/* Wenn Reste mehr als 1 dann mitnehmen */
			rest_float-=rest_int;								/* und Reste entsprechend vermindern */
			for(i=0;i<anz_int;i++,item_max++) {
				if (!platz_in_item()) break;			/* kein Platz in item[] mehr frei! */
				item[item_max].bun_nr=bunker+1;			/* Item soll in diesen Bunker */
				item[item_max].bun_x=255;				/* Position noch nicht genau bestimmt */
				item[item_max].typ=obj[item_nr].typ;
				item[item_max].grafik=obj[item_nr].grafik;
				item[item_max].flag=obj[item_nr].spezial;
				}
			}
		}
	
#ifndef COPYPROT
	if (my_system)
		if (item_max+50<ITEMMAX) debug(1,romstr519,item_max);
#endif

	verteilung=60.0+80.0*level;
	verteilung*=4;
	rest_float=0.0;
	enemy_max=0;
	for(bunker=0;bunker<BUNKERZAHL;bunker++) {
		anz_float=((float)statistik[bunker].frei*verteilung*2.0)/(100.0*1000.0);
		if (bunker==19) anz_float*=5;					/* Endbunker 5 mal soviele Gegner */
		anz_int=(int)anz_float;
		rest_float+=anz_float-anz_int;					/* Nachkommastellen aufsummieren */
		rest_int=(int)rest_float;
		anz_int+=rest_int;								/* Wenn Reste mehr als 1 dann mitnehmen */
		rest_float-=rest_int;								/* und Reste entsprechend vermindern */
		for(i=0;i<anz_int;i++,enemy_max++) {
			if (enemy_max>=ENEMYMAX) internal(romstr520);			/* kein Platz in item[] mehr frei! */
			enemy[enemy_max].bun_nr=bunker+1;			/* Item soll in diesen Bunker */
			enemy[enemy_max].x=255;						/* Position noch unbestimmt */
			enemy[enemy_max].speed=zufall(110)+40;
			enemy[enemy_max].next_move=0;
			enemy[enemy_max].staerke=20+zufall(230);
			}
		}

#ifndef COPYPROT
	if (enemy_max<ENEMYMAX) debug(2,romstr521,enemy_max);
#endif
}

FLAG platz_in_item()
{
	/* öberprÅft, ob noch ein weiteres Objekt in item[] aufgenommen werden kann. */

	if (my_system) 	 
		if (item_max>=ITEMMAX) internal(build(romstr522,item_max));
	return(item_max<ITEMMAX);
}

void load_waende()
{
	/* LÑdt alle Grafiken des Bunkers in den Speicher */
	long laenge;

	reagenz=pack_buf;
	laenge=load_objekte(REAGENZ_OBJ,reagenz);
	
	items=reagenz+laenge;
	laenge=load_objekte(ITEM_OBJ,items);

	item_rea=items+laenge;
	laenge=load_objekte(ITEM_REA_OBJ,item_rea);
	
	enemy_obj=item_rea+laenge;
	laenge=load_objekte(TERM1_OBJ+akt_bunker%2,enemy_obj);

	fire=enemy_obj+laenge;
	laenge=load_objekte(FIRE_OBJ,fire);

	shoots=fire+laenge;
	laenge=load_objekte(SHOTS_OBJ,shoots);
			
	walls=shoots+laenge;
	laenge=load_objekte(BUNKER_OBJ,walls);		/* Walls mÅssen direkt vor Shrink_Buff liegen */

	shrink_buff=walls+laenge;					/* Ab hier Verkleinerungsbuffer */

#ifndef COPYPROT 
	if (my_system) { 
		if (sizeof(memory)-(shrink_buff-memory)<16000) {
			writexy_fast(0,0,romstr523);
			writexy_fast(19*4,0,str(5,(long)sizeof(memory)-(shrink_buff-memory)));
			debug(-617,romstr524,0);
			}
		}
#endif
}


