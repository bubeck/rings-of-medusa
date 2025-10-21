 
/* Dieser Part beinhaltet die gesamte Abwicklung innerhalb der Stadt */

#include "includes.c"           /* Definiert alle Variablen als Extern,... */

/* Reihenfolge, in der die Nummern auf dem Roulette stehen: */
unsigned char roulette[37] = {
	0,32,15,19,4,21,2,25,17,34,6,27,13,36,11,30,8,23,10,5,4,16,33,
	1,20,14,31,9,22,18,29,7,28,12,35,3,26};
/* Welches Feld hat welche Farbe? 1=Rot, 0=Schwarz */
unsigned char roulette_farbe[37] = {
	-1,1,0,1,0,1,0,1,0,1,0,0,1,0,1,0,1,0,1,1,0,1,0,1,0,1,0,1,0,0,1,0,1,0,1,0,1 };
	
/* Werte der Chips im Roulette: */
unsigned int chip_wert[9] = {
	5,10,50,100,500,1000,5000,10000,50000	};
	
unsigned long seed;                     /* Startwert der Zufallszahlen */

/* Start: */

void city()
{
  register int i;
  char zeile[40];

  if (auf_schiff) {                 /* Wir kommen vom Wasserweg her */
    for(i=0;i<SCHIFFMAX;i++)        /* Alle Schiff die unterwegs waren sind */
      if (flotte[i].ort==-1) flotte[i].ort=citynum;     /* jetzt in dieser Stadt */
    }
  else armee_city=citynum;                  /* Armee steht vor dieser Stadt */

  loc = CITY;
  leiste_y=64;
  msminy=63;                     /* hîher darf Maus nicht */

  sx[0]=sxalt[0];
  sy[0]=syalt[0];               /* Sprite zurÅcksetzen */

  option_city();                /* Bereitet Options auf Stadt vor */
  get_city_werte(citynum);          /* Holt Stadtspezifische Werte von Disk */
  city_restore(citynum);

  do {
    verlassen = game_loaded=FALSE;
		button=NOTHING;
		
    hol_maus();

    if (my>169 && mk!=0) button_leiste();

    switch (button) {
			case EXIT_BTN: leave(citynum); break;
      case INFO_BTN: city_info(citynum);
                 button=NOTHING;
                 break;
      case EDITOR: editor();
                 city_restore(citynum);
                 button=NOTHING;
                 break;
      }


    if(mk == 1) {
      if (maus_in(210,63,319,92) && port_city[citynum]) port(citynum);
      else if (maus_in(43,94,131,167)) store(citynum);
      else if (maus_in(14,76,65,167)) bank(citynum);
			else if (maus_in(266,142,306,168)) subway();
      else if (maus_in(172,134,299,167)) park(citynum);
      else if (maus_in(169,92,226,128)) casino(citynum);
      else if (maus_in(301,104,319,144)) armory(citynum);
      else if (maus_in(275,109,301,149)) rohstoffhandel(citynum);
      else if (maus_in(251,90,319,114)) garage(citynum);
      else if (maus_in(168,73,211,91)) barracks(citynum);
      else if (maus_in(128,140,164,165)) leave(citynum);
			
      leiste_oben(c_name[citynum]);
      }
    } while(!verlassen);

  wait_mouse(0);

	loc=LAND;
	
	if (!game_loaded) {									/* Es wurde kein Landbunker geladen */
	  if (gesamt_gehalt>money) {
	    strcpy(zeile,romstr065);
	    strapp(zeile,str(0,gesamt_gehalt));
	    alerts_da=FALSE;									/* Farbprobleme */
			alert(build(romstr066,zeile,str(0,money)));
			alerts_da=TRUE;
	    }
	
	  city_last[citynum]=heute;     /* Heut war Spieler zuletzt in dieser Stadt */
	
	  citynum=-1;
	  get_ground();             /* neue UntergrÅnde holen */
	
	  option_city_ende();
	  land_an();
		}
}

void calc_gehalt()
{
  /* Berechnet die Summe der EinzelgehÑlter nach gesamt_gehalt */
  int i;

  gesamt_gehalt=0;
  for(i=0;i<EINHEITEN;i++)                     /* Armee will ihr Gehalt */
    gesamt_gehalt+=gehalt[i];
  for(i=0;i<SCHIFFMAX;i++)
    if (flotte[i].typ>=0) {                 /* Schiff an? */
      gesamt_gehalt+=flotte[i].salary;
      }
}

long sum_goods()
{
  /* Berechnet die Menge aller Dinge, die mitgefÅhrt werden */
  long zahl;

  zahl=sum_waren()+sum_rohstoffe()+sum_stall();
  return(zahl);
}

long sum_waren()
{
  /* Summiert alle Waren auf, und gibt die Anzahl zurÅck */
  int i;
  long zahl;

  zahl=0;
  for(i=0;i<WAREN;i++) zahl+=ycargo_menge[i];
  return(zahl);
}

long sum_soldaten()
{
  /* Summiert alle Waren auf, und gibt die Anzahl zurÅck */
  int i;
  long zahl;

  zahl=0;
  for(i=0;i<EINHEITEN;i++) zahl+=armeeteil[0][i];
  return(zahl);
}

long frei_waggon()
{
  /* Errechnet wieviel Platz auf den Waggen ist */
  long zahl;
  long min;

  min=(ystable_menge[0]<ystable_menge[1]) ? ystable_menge[0] : ystable_menge[1];
                        /* Mindestzahl aus Karren und Pferden ist einsetzbar */
  zahl=min*KAP_KARREN;     /* Jeder Karren transportiert x Einheiten */
  return(zahl);
}

long frei_cargo()
{
  /* Schaut wieviel Platz auf den Handelsschiffen ist */
  int i;
  long zahl;

  zahl=0;
  for(i=0;i<SCHIFFMAX;i++)
    if (flotte[i].typ>=0 && flotte[i].typ<3)        /* Handelsschiff? */
      if (citynum<0) {                               /* Sind wir in Stadt? */
        if (flotte[i].ort==-1)
          if (flotte[i].aboard==ship_crew[flotte[i].typ])   /* Genug Besatzung? */
            zahl+=ship_spec[flotte[i].typ];
        }
      else
        if (flotte[i].ort==citynum)
          if (flotte[i].aboard==ship_crew[flotte[i].typ])   /* Genug Besatzung? */
            zahl+=ship_spec[flotte[i].typ];

  return(zahl);
}

long frei_war()
{
  /* Schaut wieviel Platz auf den Kriegsschiffen ist */
  int i;
  long zahl;

  zahl=0;
  for(i=0;i<SCHIFFMAX;i++)
    if (flotte[i].typ>=3 && flotte[i].typ<6)        /* Kreigsschiff? */
      if (citynum<0) {                                /* Sind wir in Stadt? */
        if (flotte[i].ort==-1)
          if (flotte[i].aboard==ship_crew[flotte[i].typ])   /* Genug Besatzung? */
            zahl+=ship_spec[flotte[i].typ];
        }
      else
        if (flotte[i].ort==citynum)
          if (flotte[i].aboard==ship_crew[flotte[i].typ])   /* Genug Besatzung? */
            zahl+=ship_spec[flotte[i].typ];

  return(zahl);
}

long sum_rohstoffe()
{
  /* Summiert alle Rohstoffe auf */
  int i;
  long zahl;

  zahl=0;
  for(i=0;i<ROHSTOFFE;i++) zahl+=ymetal[i];
  return(zahl);
}

long sum_stall()
{
  /* Menge aller Stallwaren */

  int i;
  long zahl;

  zahl=0;
  for(i=0;i<STALLWAREN;i++)
    zahl+=ystable_menge[i];

  return(zahl);
}

void city_restore(num)
int num;                /* Nummer der Stadt */
{
  clear_raster();
  
  Hm();
  clear_screen(hlpbuf);
  load_objekte(STADT_OBJ,scr2);
  draw_obj(0,scr2,0,hlpbuf,0,0);							/* Bild darstellen */
  
  load_objekte(CITYBILD_OBJ,scr2);
  draw_obj(0,scr2,0,hlpbuf,0,63);

  if (port_city[num]) {
		draw_obj(1,scr2,0,hlpbuf,226,71);
		}
	
  city_screen();																	/* und anzeigen */
  leiste_oben(c_name[citynum]);
  redraw_buttons(PAUSE|EXIT_BTN|INFO_BTN|EDITOR);      /* Nur VerÑnderungen zeichnen */
  Sm();
}

void c_pic(name_nr)
int name_nr;							/* Nummer des Files */
{
	clear_raster();
  clear_screen(hlpbuf);
  load_objekte(name_nr, scr2);
  draw_obj(0,scr2,MOVE,hlpbuf,0,0);
  formular(hlpbuf,63);
  init_oben();
  city_screen();										/* und anzeigen */
}

void city_screen()
{
	/* Bereitet den Bildschirm auf 'Stadtanzeige' vor, d.h. 45 Zeilen hohes Bild im oberen 
		 Bilschirmbereich, anschliessend die Datumsleiste, dann der Hauptbildschirm, meistens 
		 das Formular, Dann kommen ganz unten die Buttons. */
		 
  init_oben();

  fade_out();                   /* Bild ausblenden */
	clear_screen(scr1);

  pic_move(hlpbuf,0,45,0);										/* Oberes Bild anzeigen */	
 	draw_obj(0,leisten,0,scr1,0,46);						/* Obere Leiste ins Bild */
  pic_move(hlpbuf,62,169,62);           			/* Hauptbildschirm */
  show_buttons(FALSE);                        /* Buttons neu anzeigen */

  fade_in();                    /* Bild einblenden */
}

void city_info(num)
int num;
{
  int i;

	leiste_oben(romstr067);
	c_pic(STADT_OBJ);
	leiste_oben(romstr068);
	
  Hm();
	
  redraw_buttons(PAUSE|EXIT_BTN);
  leiste_oben(c_name[num]);

  writexy(0,20,70,romstr069);
  writexy(0,80,70,str(0,bevoelkerung[num]));
  writexy(0,20,80,romstr070);

  for(i=0;i<RASSEN;i++) {
    writexy(0,20,87+i*6,rassename[i]);
    writexy(0,56,87+i*6,romstr071);
    writexy(0,64,87+i*6,str(0,(long)stadt_besatzung[num][i]));
    }

  writexy(0,140,78,romstr072);

  if (belong[num]==99) writexy(1,220,78,romstr073);
  else if (belong[num]<0) writexy(0,220,78,player_name);
       else writexy(0,220,78,gegnername[belong[num]]);

  if (produce_cheat) {
    writexy(0,140,90,romstr074);
    for(i=0;i<5;i++) {
      if (produkt[citynum][i]==0) break;
      if (produkt[citynum][i]<=WAREN+STALLWAREN)
        writexy(0,144,96+i*6,waren_name[produkt[citynum][i]-1]);
      else
        writexy(0,144,96+i*6,metal_name[produkt[citynum][i]-(WAREN+STALLWAREN+1)]);
      }
    }

	writexy(0,244,114,romstr075);
	rahmen(0,242,111,246+13*4,121);
	
  Sm();

  do {
    wait_klick();
		if (maus_in(242,111,246+13*4,121)) status(ns,ns);
    } while(button != EXIT_BTN);

  button=NOTHING;
  city_restore(num);
}

void hafenkneipe()
{
  /* Einkaufen der Crew in der Hafenkneipe */
  int schiffart;
  int len;
  char schiffname[30];
  int salary=5;
  int men_needed,men_inter;
  FLAG salnew,shipnew,buy_aktion,internew,grafiknew;            /* Flags */
  int i;
  int anzahl;
  int index;                    /* Index in flotte */
  int nummer[SCHIFFSTYPEN];     /* laufende Nummer der Typen */
  FLAG all;
  int chosen_men;

  for(i=0;i<SCHIFFSTYPEN;i++) nummer[i]=0;

  anzahl=0;
  for(i=0;i<SCHIFFMAX;i++)
    if (flotte[i].typ>=0 && flotte[i].ort==citynum) anzahl++;

  if (anzahl==0) {
    alert(romstr076);
    return;
    }

  for(index=0;index<SCHIFFMAX;index++)      /* Ersten Eintrag suchen */
    if (flotte[index].typ!=-1 && flotte[index].ort==citynum) break;
  schiffart=flotte[index].typ;

  nummer[schiffart]=1;              /* Dies ist das erste Schiffs dieses Typs */

  Hm();
  formular(scr1,63);
	
  len=strlen(port_pub);             /* Alle rechten Spaces weglîschen */
  while(port_pub[--len]==' ') ;
  port_pub[len+1]=0;

  center(0,70,port_pub);
  writexy(0,24,90,romstr077);
  writexy(0,24,98,romstr078);
  writexy(0,24,106,romstr079);
  writexy(0,190,90,romstr080);
  writexy(0,190,100,romstr081);
  writexy(0,190,108,romstr082);
  writexy(0,190,116,romstr083);
  writexy(0,190,124,romstr084);
  Sm();

  salnew=shipnew=internew=grafiknew=TRUE;          /* anzeigen */
  all=buy_aktion=FALSE;             /* nicht kaufen */

  do {
    Hm();

    if (salnew) {
      writexy(0,19*4+24,98,str(3,(long)salary));
      men_inter=salary/4-crew_grenze[citynum]/4;
      if (men_inter<0) men_inter=0;
      internew=TRUE;            /* Neue Zahl an Interessenten */
      }

    if (internew) {
      writexy(0,16*4+24,106,str(3,(long)men_inter));
      wait_once(7);
      }

    if (shipnew) {
  	  schiffart=flotte[index].typ; 
      strcpy(schiffname,ship_name[schiffart]);
      len=strlen(schiffname);
      schiffname[len++]=' ';
      strcpy(&schiffname[len],str(0,(long)nummer[schiffart]));
      writexy(0,190+13*4,90,space(20));
      writexy(0,190+13*4,90,schiffname);
      writexy(0,190+13*4,100,str(5,(long)ship_crew[schiffart]));
      writexy(0,190+13*4,108,str(5,(long)flotte[index].aboard));
      men_needed=ship_crew[schiffart]-flotte[index].aboard;
      writexy(0,190+13*4,116,str(5,(long)men_needed));
      writexy(0,190+13*4,124,str(6,flotte[index].salary));

      wait_mouse(0);
      }
		if (grafiknew) {
			grafiknew=FALSE;
      fill(7,12,124,180,154);
			draw_obj(schiffart,pack_buf,MOVE,scr1,
						96-objekt_breite(schiffart,pack_buf)/2,155-objekt_hoehe(schiffart,pack_buf));
			show_raster();
			}
			
    if (men_inter==0 || men_needed==0) redraw_buttons(PAUSE|EXIT_BTN);
    else redraw_buttons(PAUSE|EXIT_BTN|BUY|ALL);

    Sm();

    wait_klick();

    salnew=shipnew=FALSE;

    Hm();
    if (my>85 && my<110 && mx>180+11*4 && mx<180+26*4) {
      next_ship(&index,nummer);
      shipnew=grafiknew=TRUE;
      }
    Sm();
    if (maus_in(115,95,131,104) && salary<MAX_PRICE) {
      if (men_inter!=0 && buy_aktion) chg_price();
      else {
        salary+=5;
        salnew=TRUE;
        }
      if (button==YES) {
        if (salary>crew_grenze[citynum]) crew_grenze[citynum]=salary;
        buy_aktion=FALSE;
        salary+=5;
        salnew=TRUE;
        }
      button=NOTHING;
      }
    if (maus_in(86,95,96,105) && salary>5) {
      if (men_inter!=0 && buy_aktion) chg_price();
      else {
        salary-=5;
        salnew=TRUE;
        }
      if (button==YES) {
        if (salary>crew_grenze[citynum]) crew_grenze[citynum]=salary;
        buy_aktion=FALSE;
        salary-=5;
        salnew=TRUE;
        }
      button=NOTHING;
      }

    if (button==ALL) {
      all=!all;                         /* Flag umdrehen */
      if (all) set_mouse(1);            /* Pfeilzeiger */
      else set_mouse(0);                /* normales Kreuz */
      }

    if (button==EXIT_BTN && buy_aktion) {
      show_window(romstr085);
      redraw_buttons(YES|NO);
      do {
        wait_klick();
        } while (button!=YES && button!=NO);
      hide_window();
      if (button==YES) {
        if (salary>crew_grenze[citynum]) crew_grenze[citynum]=salary;
        button=EXIT_BTN;
        }
      else button=NOTHING;
      }

    if (button==BUY) {
      if (all) {
        if (men_inter<men_needed) chosen_men=men_inter;
        else chosen_men=men_needed;
        }
      else chosen_men=1;

      men_inter-=chosen_men;              /* ein Mann weniger interessiert */
      men_needed-=chosen_men;
      flotte[index].aboard+=chosen_men;       /* einer mehr an Bord */
      flotte[index].salary+=(long)salary*chosen_men;  /* Kosten pro Monat */
      gesamt_gehalt+=(long)salary*chosen_men;
      internew=shipnew=TRUE;
      if (men_inter==0) {
        buy_aktion=FALSE;
        crew_grenze[citynum]=salary;
        }
      else buy_aktion=TRUE;
      }
    } while(button != EXIT_BTN);
  button = NOTHING;
  set_mouse(0);                     /* Kreuzzeiger */
  formular(scr1,63);                 /* Und Bildschirm lîschen */
	show_raster();
}

void chg_price()
{
  long old_leiste;

  old_leiste=bleiste;       /* Alte Leiste retten */
  show_window(romstr086);
  redraw_buttons(YES|NO);
  do {
    wait_klick();
    } while (button!=YES && button!=NO);
  redraw_buttons(old_leiste);
  hide_window();
}

void chg_rasse()
{
  long old_leiste;

  old_leiste=bleiste;       /* Alte Leiste retten */
  show_window(romstr087);
  redraw_buttons(YES|NO);
  do {
    wait_klick();
    } while (button!=YES && button!=NO);
  redraw_buttons(old_leiste);
  hide_window();
}

void next_ship(index,nummer)
int *index,nummer[];
{
  int i;

  (*index)++;
  for(i=*index;i<SCHIFFMAX;i++)
    if (flotte[i].typ!=-1 && flotte[i].ort==citynum) {
      *index=i;                     /* NÑchstes Schiff finden */
      nummer[flotte[*index].typ]++;      /* NÑchste Nummer */
      return;
      }

  for(i=0;i<SCHIFFSTYPEN;i++) nummer[i]=0;

  for(i=0;i<SCHIFFMAX;i++)
    if (flotte[i].typ!=-1 && flotte[i].ort==citynum) {
      *index=i;
      nummer[flotte[*index].typ]=1;  /* ist Nummer 1 */
      return;
      }
}

void reederei()
{
  int ship_type=0;              /* Handelsschiffe */
  int ship_nr=0;                /* Cutter */
  int i,x;
  int nummer;

  show_ships(0,0);           /* Schiffsliste darstellen */

  redraw_buttons(PAUSE|EXIT_BTN|BUY);
  do {
    wait_klick();
		if (maus_in(301,68,312,79)) button=EXIT_BTN;					/* Closefeld */
		
    if(mk == 1) {
      if(mx>78 && my>78 && mx<122 && my<88 && ship_type == 1) {
        ship_type = 0;
        wait_mouse(0);            /* Maus loslassen */
        show_ships(0,ship_nr);      /* Liste darstellen */
        }
      if(mx>238 && my>78 && mx<274 && my<88 && ship_type == 0) {
        ship_type = 1;
        wait_mouse(0);
        show_ships(1,ship_nr);
        }
      if (my>88 && my<98) {       /* neuer Schiffstyp */
        x=75+ship_nr*80;
        drec(x,88,x+55,96);     /* Alte lîschen */
        if (mx<155) ship_nr=0;
        if (mx>210) ship_nr=2;
        if (mx>154 && mx<211) ship_nr=1;
        x=75+ship_nr*80;
        rec(x,88,x+55,96);
        }
      }
    if (button==BUY) {
      if (schiffbesitz+schiffbau>=SCHIFFMAX) {
        alert(build(romstr088,str(0,(long)SCHIFFMAX)));
        }
      else {
        schiffbau++;                         /* Eines mehr ist im Bau */
        nummer=ship_type*3+ship_nr;
        if (money<ship_price[citynum][nummer])
          alert(build(romstr089,ship_name[nummer]));
        else {
          for(i=0;i<5;i++) if (ship_art[citynum][i]==-1) break;
          if (i==5) alert(romstr090);
          else {
            money-=ship_price[citynum][nummer];
            ship_art[citynum][i]=nummer;          /* Schiff bauen */
            ship_fertig[citynum][i]=heute+90;     /* 3 Monate */
            show_ships(ship_type,ship_nr);
            }
          }
        }
      }

    } while(button != EXIT_BTN);
  button = NOTHING;
  formular(scr1,63);             /* und Bildschirm lîschen */
}

void show_ships(ship_type,nr)
int ship_type;              /* 0..1 */
int nr;                     /* 0..2 */
{
  int start_type;           /* Nummer ab der begonnen wird auszugeben */
  int i,j,x,y;

  Hm();

  formular(scr1,63);
	open_window(7,69);
  center(0,71,romstr091);
  writexy(0,80,80,romstr092);
  writexy(0,240,80,romstr093);
  writexy(0,14,90,romstr094);
  writexy(0,14, 100,romstr095);
  writexy(0,14,106,romstr096);
  writexy(0,14,118,romstr097);
  writexy(0,14,124,romstr098);
  writexy(0,14,130,romstr099);

  writexy(3,14,148,romstr100);
  writexy(3,14,154,str(0,money));

  if(ship_type==0)                       /* Handelsschiffe ? */
  {
    writexy(0,14,112,romstr101);
    start_type=0;
    rec(78,78,134,86);
  }
  else
  {
    writexy(0,14,112,romstr102);
    start_type=3;
    rec(238,78,294,86);
  }

  for (i=0;i<3;i++) {
    x=80+i*80;
    writexy(0,x,90,ship_name[start_type+i]);
    writexy(0,x,100,str(0,ship_price[citynum][start_type+i]));
    writexy(0,x,106,str(0,(long)ship_crew[start_type+i]));
    writexy(0,x,112,str(0,(long)ship_spec[start_type+i]));
    writexy(0,x,118,str(0,(long)ship_cannons[start_type+i]));
    writexy(0,x,124,str(0,(long)ship_own[start_type+i]));
    y=130;
    for(j=0;j<5;j++)
      if (ship_art[citynum][j]==start_type+i) {
        writexy(0,x,y,dstr(ship_fertig[citynum][j]));
        y+=6;
        }
    }

  x=75+nr*80;
  rec(x,88,x+55,96);

  Sm();
}

void store(num)
int num;
{
  leiste_oben(romstr103);
  c_pic(STORE_OBJ);
  leiste_oben(romstr104);

  buy_sell(romstr105,WAREN,waren_name,11,waren_preis,waren_menge[num],ycargo_menge,LADEN); 

  city_restore(num);
}

void park(num)
int num;
{
  int company = 0,rasse=0;
  int chosen_men;
  FLAG new_salary,new_company,new_chosen,new_money,new_inter;
  FLAG all;
  int i;
  int neue_rasse;                   /* Wenn Rasse gewechselt wird, wird hier
                                       die neue Rasse gespeichert */
  float summe;
  float rasse_vorher[RASSEN],rasse_nachher[RASSEN];

  leiste_oben(romstr106);
  c_pic(PARK_OBJ);
  leiste_oben(romstr107);

	open_window(7,69);

  redraw_buttons(EXIT_BTN|BUY|ALL|PAUSE);

  Hm();
  writexy(0,15,81,romstr108);
  writexy(0,170,81,romstr109);
  writexy(0,15,91,romstr110);
  for(i=0;i<RASSEN;i++) {
    writexy(0,19,97+i*6,rassename[i]);
    writexy(0,71,97+i*6,romstr111);
    }

  for(i=0;i<EIGENSCHAFTEN;i++) {
    writexy(0,170,115+i*6,eigenschaftname[i]);
    writexy(0,230,115+i*6,romstr112);
    }
  writexy(0,170,151,romstr113);

  writexy(0,170,91,romstr114);
  writexy(0,170,97,romstr115);
  writexy(0,170,103,romstr116);

  Sm();

  rec(15,96,67,102);                /* Human selektieren */

  chosen_men=1;                     /* einer ausgewÑhlt */

  new_salary=new_company=new_chosen=new_money=new_inter=TRUE;
  all=FALSE;

  do {
    Hm();
    if (new_money) {
      new_money=FALSE;
      writexy_money(170,71);
      }
    if (new_chosen) {
      new_chosen=FALSE;
      if (chosen_men>stadt_besatzung[num][rasse])
        chosen_men=stadt_besatzung[num][rasse];
      writexy(0,258,103,str(4,(long)chosen_men));
      wait_once(12);
      new_inter=TRUE;           /* Neue Zahl an Interessenten */
      }
    if (new_company) {
      new_company=FALSE;
      writexy(0,79,81,armeename[company]);
      writexy(0,115,91,str(8,armeeteil[0][company]));
      writexy(0,202,81,str(7,gehalt[company]));
      writexy(0,238,151,str(8,gesamt_gehalt));

      for(i=0;i<RASSEN;i++) {
        writexy(0,79,97+i*6,fstr(romstr117,sold_zahl[company][i]));
        rasse_vorher[0]=armeeteil[0][company]*sold_zahl[company][i]/100.0;
        writexy(0,115,97+i*6,str(8,runde(rasse_vorher[0])));
        }
      for(i=0;i<EIGENSCHAFTEN;i++)
        writexy(0,238,115+i*6,str(3,(long)training[0][company][i]));
      new_inter=new_salary=TRUE;
      }

    if (new_salary) {
      new_salary=FALSE;
      writexy(0,242,91,str(3,(long)preis_einheiten[company]));
      new_inter=TRUE;     /* Neue Zahl an Interessenten, wenn anderes Gehalt */
      }

    if (new_inter) {        /* Anzahl der Interessenten neu berechnen */
      new_inter=FALSE;
      writexy(0,238,97,str(5,stadt_besatzung[num][rasse]));
      }
    Sm();

    wait_klick();

		if (maus_in(303,69,311,77)) button=EXIT_BTN;				/* Closer */
		
    if (button==ALL) {
      all=!all;                         /* Flag umdrehen */
      if (all) set_mouse(1);            /* Pfeilzeiger */
      else set_mouse(0);                /* normales Kreuz */
      }

    if (button==BUY)
      if (armee_status==AN_BORD ||
          (armee_status==AN_LAND && auf_schiff && armee_city==citynum) ||
          (armee_status==AN_LAND && !auf_schiff)) {
        if (all) {
          chosen_men=9999;         /* Alle nehmen */
          new_chosen=TRUE;
          }
        if (chosen_men>stadt_besatzung[num][rasse]) {
          chosen_men=stadt_besatzung[num][rasse];        /* Soviel wie eben da sind */
          new_chosen=TRUE;
          }
        if ((long)preis_einheiten[company]*chosen_men>money) money_alert();
        else
          if (chosen_men!=0) {
            money-=(long)preis_einheiten[company]*chosen_men;               /* Wollen gleich Geld */
            for(i=0;i<EIGENSCHAFTEN;i++) {
              summe=training[0][company][i]*armeeteil[0][company];
              summe+=(float)rassen_eigen[rasse][i]*chosen_men;
              training[0][company][i]=summe/(armeeteil[0][company]+chosen_men);
              }

            for(i=0;i<RASSEN;i++) {
              rasse_vorher[i]=armeeteil[0][company]*sold_zahl[company][i]/100.0;
              rasse_nachher[i]=rasse_vorher[i];
              }
            rasse_nachher[rasse]=rasse_vorher[rasse]+chosen_men;

            stadt_besatzung[num][rasse]-=chosen_men;
            armeeteil[0][company]+=chosen_men;
            armeegesamt[0]+=chosen_men;

            if (armeeteil[0][company]>0)
              for(i=0;i<RASSEN;i++)
                sold_zahl[company][i]=rasse_nachher[i]*100.0/armeeteil[0][company];

            gehalt[company]+=(long)preis_einheiten[company]*chosen_men;         /* Total salary erhîhen */
            gesamt_gehalt+=(long)preis_einheiten[company]*chosen_men;
            new_company=new_inter=new_money=TRUE;
            }
        }
      else {
        alert(romstr118);
        }

    if (my>=75 && my<=87) {							/* Neue Einheit */
      Krec(75,79,127,87);
      company++;
      if (company==7) company=0;
      new_company=TRUE;
      }

    if (maus_in(285,100,295,110) && chosen_men<9989) {  /* ++ */
      chosen_men+=10;
      new_chosen=TRUE;
      }
    if (maus_in(272,100,284,110) && chosen_men<9999) {
      chosen_men++;
      new_chosen=TRUE;
      }
    if (maus_in(244,100,256,110) && chosen_men>1) {     /* - */
      chosen_men--;
      new_chosen=TRUE;
      }
    if (maus_in(230,100,243,110) && chosen_men>11) {
      chosen_men-=10;
      new_chosen=TRUE;
      }
    if (mx<170 && my>97 && my<97+RASSEN*6) {
      neue_rasse=(my-97)/6;
      Hm();
      drec(15,96+rasse*6,67,102+rasse*6);                /* altes deselektieren */
      rasse=neue_rasse;
      rec(15,96+rasse*6,67,102+rasse*6);                /* Neues selektieren */
      new_inter=TRUE;                       /* Neue Zahl an Interessenten */
      Sm();
      }
    } while(button != EXIT_BTN);

  button=NOTHING;
  set_mouse(0);                         /* Kreuz wieder an */
	close_window();
  city_restore(num);
}

void garage(num)
int num;
{
  leiste_oben(romstr119);
  c_pic(STABLE_OBJ);
  leiste_oben(romstr120);

  buy_sell(romstr121,STALLWAREN,waren_name[WAREN],
      11,&waren_preis[WAREN],
      &waren_menge[num][WAREN],ystable_menge,LADEN);

  city_restore(num);
}

void rohstoffhandel(num)
int num;
{
  leiste_oben(romstr122);
  c_pic(JEWELLER_OBJ);
  leiste_oben(romstr123);

  buy_sell(ns,ROHSTOFFE,metal_name,11,&waren_preis[WAREN+STALLWAREN],0L,ymetal,JUWELLIER);
 
  city_restore(num);
}

void test_schulden()
{
  /* testet, ob Spieler noch etwas kaufen kann */

  /* Wenn er Schulden seit lÑnger als 3 Monaten hat, kann er nichts mehr kaufen */
  if (kontostand<0 && heute-bank_zuletzt>3*30) {
    schulden=TRUE;
    }
  else schulden=FALSE;
}

void calc_zinsen()
{
  /* Berechnet die Zinsen, die fÑllig sind, seit Spieler das letzte Mal auf
     der Bank war */
  float geld;
  long tage;

  tage=heute-bank_zuletzt;          /* Wieviel Tage sind vergangen */

  geld=(float)kontostand;
  geld/=100.0;

  zinsen=0;

  if (kontostand<0) {                /* Kredit */
    geld*=0.049315068*tage;               /* 18% im Jahr */
    zinsen=geld;                     /* die fÑlligen Zinsen, hier negativ! */
    }
  if (kontostand>0) {
    geld*=0.010958904*tage;              /* 4% Im Jahr */
    zinsen=geld;                        /* positiv */
    }
}

void casino(num)
int num;
{
	EINSATZ einsatz[10];									/* Spieler kann auf max. 10 Felder setzen */
	int feld;
	
	leiste_oben(romstr124);
	load_objekte(ROULETTE_OBJ,pack_buf);
	c_pic(PUB_OBJ);
	leiste_oben(romstr125);

	redraw_buttons(EXIT_BTN|PAUSE);
	
	draw_roulette();							/* Zeichnet den Roulettetisch neu */
	do {
		einsatz_machen(einsatz);							/* Spieler kann seinen Einsatz setzen */
		if (einsatz[0].feld==-1) break;					/* Kein Einsatz mehr */
		feld=rotier_roulette();
		gewinne(feld,einsatz);
		Hm();
		draw_obj(1,pack_buf,0,scr1,96,73);								/* Filz zeichnen */
		Sm();
		} while(button!=EXIT_BTN);
		
  city_restore(num);
}

#define CHIPX 48													/* Position des Chipwindows */
#define CHIPY 90

void gewinne(feld,einsatz)
int feld;												/* ausgelostes Feld */
EINSATZ einsatz[];
{
	/* Gewinnauswertung des Roulettespiels */
	int i;
	char einsatz_str[10];			/* Zum Aufbauen der Zahlenstrings */
	char feld_str[10];
	char faktor_str[3];
	int faktor;								/* Gewinnfaktor */	
	unsigned long gewinn;
		
	for(i=0;i<10;i++) {
		faktor=0;
		if (einsatz[i].feld==-1) break;						/* EOL */
		else if (einsatz[i].feld==feld) {								/* direkte Zahl */
			faktor=36;
			strcpy(feld_str,str(0,(long)feld));
			}
		else if (einsatz[i].feld==38 && feld%2==1) {						/* Impair */
			faktor=2;
			strcpy(feld_str,romstr126);
			}
		else if (einsatz[i].feld==41 && feld%2==0) {							/* Pair */
			faktor=2;
			strcpy(feld_str,romstr127);
			}
		else if (einsatz[i].feld==39 && roulette_farbe[feld]==1) {		/* Rot */
			faktor=2;
			strcpy(feld_str,romstr128);
			}
		else if (einsatz[i].feld==42 && roulette_farbe[feld]==0) {		/* Schwarz */
			faktor=2;
			strcpy(feld_str,romstr129);
			}
		else if (einsatz[i].feld==37 && feld<=18) {				/* Manque */
			faktor=2;
			strcpy(feld_str,romstr130);
			}
		else if (einsatz[i].feld==40 && feld>18) {				/* Passe */
			faktor=2;
			strcpy(feld_str,romstr131);
			}
		else if (einsatz[i].feld>=43 && einsatz[i].feld<=45) {			/* waagrechte Zeile */
			zeile=45-einsatz[i].feld;
			if ((feld-1)%3==zeile) {
				faktor=3;
				strcpy(feld_str,romstr132);
				}
			}
		if (faktor!=0) {
			strcpy(faktor_str,str(0,(long)faktor));
			strcpy(einsatz_str,str(0,(long)chip_wert[einsatz[i].chip]));
			gewinn=(unsigned long)chip_wert[einsatz[i].chip]*faktor;
			money+=gewinn;
			alert(build(romstr133,
									einsatz_str,feld_str,faktor_str,str(0,gewinn)));
			}
		}
}

void einsatz_machen(einsatz)
EINSATZ einsatz[];
{
	/* Spieler kann seinen Einsatz setzen */
	int chip_nummer,feld;	
	int i;
	int einsatz_nr=0;
	
	Hm();
	raster_replace(CHIPX,CHIPY,CHIPX+objekt_breite(2,pack_buf)-1,CHIPY+objekt_hoehe(2,pack_buf)-1,
													scr1,320,
									0,100,hlpbuf,320);							/* Hintergrund retten */
									
	draw_grow_obj(2,pack_buf,0,scr1,CHIPX,CHIPY);						/* Chipboard zeichnen */
	for(i=0;i<9;i++)
		draw_obj(32+i,pack_buf,0,scr1,CHIPX+30,CHIPY+11+i*6);			/* Chips zeichnen */

	writexy(0,109,75,romstr134);
	roulette_money();
	Sm();
	
	do {
		wait_klick();
		if (maus_in(CHIPX+27,CHIPY+11,CHIPX+37,CHIPY+11+9*6-1)) {
			chip_nummer=(my-(CHIPY+11))/6;
			if (chip_nummer<0) chip_nummer=0;
			if (chip_nummer>8) chip_nummer=8;
			if (money>=chip_wert[chip_nummer]) {						/* Genug Geld? */
				feld=move_chip(chip_nummer);
				if (feld>=0) {
					money-=chip_wert[chip_nummer];
					roulette_money();
					einsatz[einsatz_nr].chip=chip_nummer;
					einsatz[einsatz_nr].feld=feld;
					draw_einsatz(einsatz,einsatz_nr++);
					}
				}
			}
			if (einsatz_nr==10) break;								/* max. 9 EinsÑtze */
		} while(button!=EXIT_BTN && !maus_in(CHIPX+37,CHIPY,CHIPX+45,CHIPY+8));			/* Close */
	
	if (einsatz_nr<10) einsatz[einsatz_nr].feld=-1;															/* letztes Feld */
	
	Hm();
	raster_replace(0,100,objekt_breite(2,pack_buf)-1,100+objekt_hoehe(2,pack_buf)-1,hlpbuf,320,
									CHIPX,CHIPY,scr1,320);								/* Hintergrund hin */
	shrink_box(CHIPX,CHIPY,objekt_breite(2,pack_buf),objekt_hoehe(2,pack_buf));
	Sm();
	
	button=NOTHING;
}

void roulette_money()
{
	/* Gibt im Roulettefenster den Geldstand aus */
	
	Hm();
	writexy(0,232,75,romstr135);
	writexy(0,232+strlen(romstr135)*4,75,str(8,money));
	Sm();
}

void draw_einsatz(einsatz,nr)
EINSATZ einsatz[];
int nr;
{
	/* Zeichnet einen Chipeinsatz auf den Screen */

	Hm();	

	if (einsatz[nr].feld>=1 && einsatz[nr].feld<=36) 
		draw_obj(32+einsatz[nr].chip,pack_buf,0,scr1,
					119+4+((einsatz[nr].feld-1)/3)*13,101+4+(2-(einsatz[nr].feld-1)%3)*11);
	else if (einsatz[nr].feld>=37 && einsatz[nr].feld<=39) 
		draw_obj(32+einsatz[nr].chip,pack_buf,0,scr1,
					119+24+(einsatz[nr].feld-37)*52,87+6);
	else if (einsatz[nr].feld>=40 && einsatz[nr].feld<=42) 
		draw_obj(32+einsatz[nr].chip,pack_buf,0,scr1,
					119+24+(einsatz[nr].feld-40)*52,134+6);
	else if (einsatz[nr].feld>=43 && einsatz[nr].feld<=45) 
		draw_obj(32+einsatz[nr].chip,pack_buf,0,scr1,
					275+7,101+4+(einsatz[nr].feld-43)*11);
	else if (einsatz[nr].feld==0) 
		draw_obj(32+einsatz[nr].chip,pack_buf,0,scr1,106+6,101+15);

	Sm();
}

int move_chip(chip_nummer)
int chip_nummer;
{
	/* Spieler hat auf einen Chip geklickt und kann diesen nun setzen */
	/* Gibt angezogenes Feld zurÅck */
	int feldx,feldy;
	
	move_objekt(32+chip_nummer,pack_buf,MOVE,0,hlpbuf,
								STOP_R|STOP_L|STOP_O|STOP_U,0,73,303,156,TRUE);	/* 2 Screenstechnik */
	mk=1;																	/* geklickt, wegen maus_in() */
	if (maus_in(119,101,274,133)) {						/* Zahlen */
		feldy=2-(my-101)/11;
		feldx=(mx-119)/13;
		return(1+feldx*3+feldy);							/* Feldnummer */
		}
	else if (maus_in(119,87,274,101)) {						/* Manque, Impair, Rot */
		feldx=(mx-119)/52;	
		return(37+feldx);
		}
	else if (maus_in(119,134,274,148)) {						/* Passe, Pair, Schwarz */
		feldx=(mx-119)/52;
		return(40+feldx);
		}
	else if (maus_in(106,101,119,134)) 						/* Null */
		return(0);
	else if (maus_in(275,101,291,133)) {						/* waagrechte Spalten */
		feldy=(my-101)/11;
		return(43+feldy);
		}
	return(-1);																/* ungÅltiger Zug */
}

void draw_roulette()
{
	/* Zeichnet den gesamten Roulettetisch neu */
	
	Hm();
	formular(scr1,63);					
	draw_grow_obj(1,pack_buf,0,scr1,96,73);						/* Filz zeichnen */
	draw_grow_obj(0,pack_buf,0,scr1,15,73);						/* Window fÅr SchÅssel */
	copy_screen(scr1,scr2);
	feld_weiter();																		/* RouletteschÅssel zeichnen */
	switch_screens();
	copy_screen(scr1,scr2);
	show_raster();
	Sm();
}

int rotier_roulette()
{
	/* Dreht die SchÅssel und lost eine Nummer aus */
	/* Gibt die ausgewÑhlte Nummer zurÅck */
	int Nummer;																/* ausgewÑhlte Nummer */
	float Bildps;															/* Bilder pro Sekunde */
	float Plusps;															/* Beschleunigung in Bildern pro Sekunde */
	float Endps;															/* Bilder pro Sekunde nach Beschleunigung */
	int kugelcounter;													/* zÑhlt Kugelpositionen */
	int BahnDist;															/* Abstand der Kugel von oberen Rand */
	int KugelproBild;													/* Anzahl gedrawter Kugeln pro SchÅsselbild */
	int x,y;																	/* Koordinaten fÅr Kugelwackler */
	int i;
	FLAG aus;
	
	Hm();
	
	writexy(0,109,75,romstr136);
	hippel_sfx(ROULETTE_AN);
	aus=FALSE;
	
	copy_screen(scr1,scr2);

	for(i=0;i<zufall(10);i++) zufall(23);	

	Plusps=1.5+zufall(10)/10.0;									/* Beschleunigung in Bildern pro Sekunde */
	Endps=35+zufall(10);											/* maximale Anzahl Bilder pro Sekunde */
	Bildps=Plusps;														/* Anfangsumdrehungszahl */

	while(Bildps<Endps) {											/* Beschleunigen */
		Nummer=feld_weiter();
		switch_screens();
		wait_sync((int)get_sync()/Bildps+0.5);	/* wartet entsprechende Anzahl VBLs */
		Bildps+=Plusps;													/* Drehzahl erhîhen */
		} 
	Bildps-=Plusps;														/* richtiger Wert */
	Bildps/=0.4;
	Bildps=(int)Bildps*0.4;										/* damit SchÅssel auf vollem Feld stehenbleibt */
	if (Nummer&1) Bildps+=0.2;
	
	kugelcounter=-4;													/* = relativer Mittelpunkt der Kugel */
	while(Bildps>1-0.2) {											/* Abbremsen */
		Nummer=feld_weiter();
		if (get_sync()/Bildps<=2*3)							/* SchÅssel nicht wesentlich langsamer */
			KugelproBild=(int)get_sync()/Bildps/2;					/* maximal 1 Kugel pro 2 VBL */
		else KugelproBild=4;									
		if (KugelproBild==0) {									/* keine Kugel */
			switch_screens();
			wait_sync((int)(get_sync()/Bildps+0.5));
			}
		else {
			/* um Kugellinie restaurieren zu kînnen */
			cpy_raster(scr2,hlpbuf,0,81,319,81+objekt_hoehe(9,pack_buf)-1,0,0);
			for(i=1;i<=KugelproBild;i++)	{   /*0,32,319,32+16+11,0,81+32*/
				if (Bildps>=4 || kugelcounter<=68+5) {				/* noch schnell oder Kugel noch sichtbar */
					if (Bildps>=16)									/* noch Ñuûerste Kugelumlaufbahn */
						BahnDist=81+32;
					else														/* Bahnen werden enger */
						BahnDist=81+32-Bildps*2+32;
					if (kugelcounter<6||kugelcounter>63)
						BahnDist+=3;
					else if (kugelcounter<18||kugelcounter>51)
						BahnDist+=2;
					else if (kugelcounter<29||kugelcounter>40)
						BahnDist+=1;
					if (kugelcounter<=5)										/* Kugel nur rechts sichtbar */
						draw_obj_part(31,pack_buf,6-kugelcounter,0,10,10,1,scr2,18+1,BahnDist-2);
					else if (kugelcounter<=63)
		  			draw_obj(31,pack_buf,1,scr2,18+1+kugelcounter-6,BahnDist-2);
			  	else if (kugelcounter<=68+5)					/* Kugel nur links sichtbar */
				  	draw_obj_part(31,pack_buf,0,0,68+5-kugelcounter,10,1,scr2,18+1+kugelcounter-6,BahnDist-2);
					kugelcounter+=5-KugelproBild;				/* weiterrollen */
					if (kugelcounter>=68+6+150)			/* Alle 150/KugelproBild Felder kommt die Kugel neu */
						kugelcounter=-4;
					}
				else if (Bildps<=1)	{									/* letztes Bild */
					if (i&1)														/* 1. und 3. Kugelposition */
						x=18+29-1+1;
					else
						x=18+29+1+1;
					if (i<=2)
						y=81+61;
					else
						y=81+62;
					draw_obj(31,pack_buf,1,scr2,x,y-2);
					}
				else if (Bildps<=1+0.2)	{							/* vorletztes Bild */
					if (i<=2) 													/* 1. und 2. Kugelposition */
						y=81+62;
					else
						y=81+63;
					draw_obj(31,pack_buf,1,scr2,18+53+i+1,y-2);
					}
				else if (Bildps<=1+1.2 && !aus) {					/* vorvorletztes Bild */
					hippel_sfx(ROULETTE_AUS);
					aus=TRUE;
					}
				switch_screens();
				wait_sync((int)(get_sync()/Bildps/KugelproBild+0.5));	/* entsprechende VBL-Anzahl warten */
				cpy_raster(hlpbuf,scr2,0,0,319,objekt_hoehe(9,pack_buf)-1,0,81);	/* restore SchÅssel */
				/* if (mousek) wait_key();				zu PrÅfzwecken */
				}
			}
		Bildps-=0.2;														/* Verzîgerung mit 0.2 Bildern pro Sekunde */
		}
		
	draw_obj(31,pack_buf,1,scr2,18+29,81+62);
	switch_screens();
	
	if (!aus) hippel_sfx(ROULETTE_AUS);				/* War Musik noch an ? */

	while (mousek==0) {
		writexy(0,25,75,romstr137);
		wait_sync_klick(30);
		writexy(0,25,75,space(6));
		if (mousek) break;
		wait_sync_klick(30);
		}
		
	wait_mouse(0);														/* und wieder loslassen */		
	Sm();

	return((int)roulette[Nummer/2]);								/* Feldnummer zurÅckgeben */
}

int feld_weiter()
{
	/* Geht ein Feld weiter und zeigt dieses an, gibt Feld_nr zurÅck */
	static int feld=-1;						/* Index in roulette[] */
	
	feld++;
	if (feld>=37*2)								/* Einmal rum? */
		feld=0;
	draw_feld(feld);
	
	return(feld);
}

void draw_feld(feld_nr)
int feld_nr;									/* index in roulette[] */
{
	/* Zeichnet den Roulettetisch, der auf Position Feld steht */
	/* wobei feld_nr 37*2 ist, also eine Festkommazahl mit einer 
	Nachkommastelle */
	int objekt;
	int objekt1, objekt2;							/* erste Ziffer, zweite Ziffer */
	int obj1x, obj2x;									/* Ordinaten der Ziffern */
	
	if (feld_nr&1) {									/* Zwischenposition */
		objekt1=roulette[feld_nr/2]%10+10;		/* linke Ziffer, in schwarz */
		if (roulette[(feld_nr+1)/2%37]<10) {
			objekt2=roulette[(feld_nr+1)/2%37]+10;	/* rechte Ziffer, Einerstelle, schwarz */
			obj2x=18+33+2+29+1;
			}
		else {
			objekt2=roulette[(feld_nr+1)/2%37]/10+10;	/* rechte Ziffer, Zehnerstelle, schwarz */
			obj2x=18+33+2+24+1;
			}
		switch (feld_nr) {
			case 1:									/* GrÅn auf Rot (0..1) */
				objekt=9;
				objekt1=30;						/* grÅne Null */
				objekt2+=10;					/* -> rot */
				break;
			case 36*2+1:
				objekt=7;							/* Schwarz auf GrÅn (36..0) */
				objekt2=30;						/* grÅne Null */
				break;
			default:
				if (feld_nr%4==3) {				/* Rot auf Schwarz */
					objekt=6;
					objekt1+=10;						/* -> rot */
					}
				else {										/* Schwarz auf Rot */
					objekt=4;
					objekt2+=10;						/* -> rot */
					}
			}
		draw_obj(objekt,pack_buf,0,scr2,18,81);
		if (roulette[feld_nr/2]<10) draw_obj_part(objekt1,pack_buf,6,0,8,6,1,scr2,18+1,81+33+9+3);
		else draw_obj(objekt1,pack_buf,1,scr2,18+1,81+33+9+3);
		if (obj2x==18+33+2+29+1) draw_obj_part(objekt2,pack_buf,0,0,3,6,1,scr2,obj2x,81+33+9+3);
		else draw_obj(objekt2,pack_buf,1,scr2,obj2x,81+33+9+3);
		}
	else {
	  obj2x=18+29+1;
	  objekt1=-11;									/* nur 1 Ziffer, -11 wegen plus 10 */
		switch (feld_nr) {
			case 0:											/* Null (GrÅn) */
				objekt=8;
				objekt2=30;
				break;
			default:
				objekt2=roulette[feld_nr/2]%10+10;			/* hintere Ziffer, in schwarz */
				if (roulette[feld_nr/2]>=10) {						/* 2 Ziffern */
					objekt1=roulette[feld_nr/2]/10+10;		/* vordere Ziffer, in schwarz */
					obj1x=18+24+1;
					obj2x=18+24+9+2+1;
					}
				if (feld_nr%4==2) {					/* Rot */
					objekt=5;
					objekt1+=10;							/* -> rote Ziffern */
					objekt2+=10;
					}
				else 
					objekt=3;
			}
		draw_obj(objekt,pack_buf,0,scr2,18,81);
		if (objekt1>=0)	draw_obj(objekt1,pack_buf,1,scr2,obj1x,81+33+9);
		draw_obj(objekt2,pack_buf,1,scr2,obj2x,81+33+9);
		}
}

void port(num)
int num;                    /* Stadtnummer */
{
  leiste_oben(romstr138);
  load_objekte(SHIPS_OBJ,pack_buf);            /* Schiffsicons -> screen2 */
  c_pic(PORT_OBJ);
  leiste_oben(romstr139);

	open_window(7,69);
		
  do {
    Hm();

    redraw_buttons(EXIT_BTN|PAUSE);
    center(0,71,romstr140);

    writexy(0,16, 90, romstr141);
    writexy(0,16, 96, romstr142);
    writexy(0,16, 108, romstr143);
    writexy(0,16, 114, romstr144);
    writexy(0,16, 120, romstr145);
    writexy(0,16, 132, romstr146);
    writexy(0,16, 138, romstr147);
    writexy(0,16, 144, romstr148);
    writexy(0,160,90,romstr149);
    writexy(0,160,96,romstr150);
    writexy(0,160,108,romstr151);
    writexy(0,160,114,romstr152);
    writexy(0,148,132,romstr153);
    writexy(0,148,138,romstr154);
    writexy(0,148,144,romstr155);

    balken(220,132,frei_cargo(),sum_goods());
    balken(220,138,frei_war(),sum_soldaten());
    balken(220,144,frei_waggon(),sum_waren()+sum_rohstoffe());
    Sm();

    wait_klick();

  	if (maus_in(303,69,311,77)) {						/* Closer */
			break;
			}
    if (mx<150) {
      if (my>85 && my<102) {
        Krec(14,88,134,102);              /* An Land gehen */
        land();
        }

      if (my>102 && my<126) {
        Krec(14,106,134,126);            /* In See stechen */
        sea();
        }
      if (my>126 && my<150) {
        Krec(14,130,134,150);
        sea_soldier();                    /* Mit Armee in See stechen */
        }
      }
    else {
      if (my<102) {
        Krec(158,88,286,102);
        hafenkneipe();
				c_pic(PORT_OBJ);
				open_window(7,69);
        }
      if (my>102 && my<130) {
        Krec(158,106,298,120);
        reederei();
				open_window(7,69);
        }
      }
    } while(button!=EXIT_BTN && !verlassen);

	close_window();
	
  mx=my=0;                              /* Damit in Stadt nichts angewÑhlt wird */
  if (!verlassen) city_restore(num);
}

void sea_soldier()
{
  /* Spieler will in See stechen, und Soldaten mitnehmen */
  int i;

  if (!try_to_sold() || !try_to_load()) return;    /* Versucht die Soldaten zu verladen */

  if (auf_schiff) {                 /* Er kam vom Seeweg in die Stadt */
    if (armee_status==AN_BORD) {
      verlassen=TRUE;
      button=EXIT_BTN;                    /* Stadt wieder auf Wasserweg verlassen */
      }
    else
      if (armee_stadt==citynum) {           /* Ist Armee in dieser Stadt? */
        armee_status=AN_BORD;               /* Armee ist jetzt an Bord */
        armee_pos=-1;                       /* Armeesprite ist weg */
        verlassen=TRUE;
        auf_schiff=TRUE;                    /* Er fÑhrt jetzt Schiff! */
        button=EXIT_BTN;
        }
      else {
        alert(romstr156);
        }
    }
  else {
    for(i=0;i<SCHIFFMAX;i++)        /* Schauen, ob in dieser Stadt ein Schiff ist */
      if (flotte[i].typ>=0)                     /* Schiff? */
        if (flotte[i].ort==citynum)             /* Ist Schiff hier? */
          if (flotte[i].aboard>=ship_crew[flotte[i].typ])   /* Genug Besatzung? */
            break;

    if (i==SCHIFFMAX)
      alert(romstr157);
    else {
      such_hafen();                     /* Sucht den Stadthafen auf der Karte */
      sx[1]=sx[0];
      sy[1]=sy[0];                  /* Armeesprite ist jetzt Sprite #1 */
      sn[1]=sn[0];            /* alle Werte kopieren */
      armee_pos=-1;                 /* Landarmee ist nirgends */
      armee_stadt=citynum;
      armee_status=AN_BORD;             /* Armee ist jetzt an Bord */
      sx[0]=sxalt[0]=hafenx;
      sy[0]=syalt[0]=hafeny;
      sn[0]=3;                 /* Schiff -> */
      auf_schiff=verlassen=TRUE;    /* Spieler ist jetzt auf dem Schiff */
      button=EXIT_BTN;
      }                             /* und will die Stadt verlassen */
    }

  if (verlassen)                    /* Spieler will Stadt verlassen */
    for(i=0;i<SCHIFFMAX;i++)
      if (flotte[i].typ>=0)                       /* Schiff? */
        if (flotte[i].ort==citynum)             /* Schiff in dieser Stadt ? */
          if (flotte[i].aboard==ship_crew[flotte[i].typ])   /* Genug Besatzung? */
            flotte[i].ort=-1;                               /* Schiff ist unterwegs */
}

void sea()
{
  /* Spieler will in See stechen, die Armee aber da lassen */
  int i;

  if (!try_to_load()) return;      /* Alle Waren an Bord bringen */

  if (auf_schiff) {                 /* Er kam vom Seeweg in die Stadt */
    if (armee_status==AN_BORD) {    /* Die Armee ist an Bord */
      such_eingang();
      sx[1]=grundx;                     /* Koordinaten der Armee */
      sy[1]=grundy;
      armee_status=AN_LAND;             /* Armee ist wieder an Land */
      armee_stadt=citynum;              /* und zwar hier */
      armee_pos=pos_welt;               /* In diesem Land */
      }
    verlassen=TRUE;
    button=EXIT_BTN;                    /* Stadt wieder auf Wasserweg verlassen */
    }
  else {
    for(i=0;i<SCHIFFMAX;i++)        /* Schauen, ob in dieser Stadt ein Schiff ist */
      if (flotte[i].typ>=0)         /* Handels- oder Kriegsschiff */
        if (flotte[i].ort==citynum)                 /* Steht Schiff hier? */
          if (flotte[i].aboard==ship_crew[flotte[i].typ])   /* Genug Besatzung? */
            break;

    if (i==SCHIFFMAX)
      alert(romstr158);
    else {
      such_hafen();                     /* Sucht den Stadthafen auf der Karte */
      sx[1]=sx[0];
      sy[1]=sy[0];                      /* Armeesprite ist jetzt Sprite #1 */
      armee_pos=pos_welt;               /* Landarmee ist in diesem Land! */
      armee_stadt=citynum;              /* und in dieser Stadt */
      sn[1]=sn[0];                      /* alle Werte kopieren */
      sx[0]=sxalt[0]=hafenx;
      sy[0]=syalt[0]=hafeny;
      sn[0]=3;                          /* Schiff -> */
      auf_schiff=verlassen=TRUE;        /* Spieler ist jetzt auf dem Schiff */
      button=EXIT_BTN;
      }                             /* und will die Stadt verlassen */
    }

  if (verlassen)                    /* Spieler will Stadt verlassen */
    for(i=0;i<SCHIFFMAX;i++)
      if (flotte[i].typ>=0)          /* Handels- oder Kriegsschiff? */
        if (flotte[i].ort==citynum)                     /* Ist Schiff hier? */
          if (flotte[i].aboard==ship_crew[flotte[i].typ])   /* Genug Besatzung? */
            flotte[i].ort=-1;                       /* Schiff ist unterwegs */
}

void land()
{
  /* Spieler will an Land gehen */

  if (auf_schiff) {
    if (armee_stadt!=citynum && armee_status==AN_LAND) {
      alert(romstr159);
      }
    else {
      if (armee_status==AN_BORD) {      /* Armee war an Bord, X/Y bestimmen */
        such_eingang();
        sx[1]=grundx;               /* Koordinaten der Armee */
        sy[1]=grundy;
        }
      armee_status=AN_LAND;             /* Armee ist wieder an Land */
      armee_stadt=citynum;              /* und zwar hier */
      sx[0]=sxalt[0]=sx[1];
      sy[0]=syalt[0]=sy[1];             /* Spritewerte Åbergeben */
      sn[0]=0;                          /* SPieler ist wieder da */
      sx[1]=-1;                         /* Schiff ist nicht zu sehen */
      auf_schiff=FALSE;                 /* Weiter geht's an Land */
      alert(romstr160);
      }
    }
  else alert(romstr161);
}


FLAG try_to_load()
{
  /* Versuchen alle Waren an Bord zu bringen */
  long freiplatz;                       /* Freier Platz auf Handelsschiffen */

  freiplatz=frei_cargo();
  if(sum_goods()>freiplatz) {
    alert(romstr162);
    return(FALSE);
    }
  return(TRUE);
}

FLAG try_to_sold()
{
  /* Versuchen alle Soldaten an Bord zu bringen */
  long soldatenzahl;                /* Anzahl der Soldaten, die geladen werden */
  long freiplatz;                   /* Freier Platz auf den Kriegsschiffen */

  soldatenzahl=sum_soldaten();
  freiplatz=frei_war();
  if(soldatenzahl>freiplatz) {
    alert(romstr163);
    return(FALSE);
    }
  return(TRUE);
}

void such_hafen()
{
  /* sucht den zugehîrigen Hafen, zu der Stadt, in der Spieler ist */
  int x,y;
  int hafen_wert;    /* Untergrundwerte */
  int wasserx,wassery;
  int stadt;

  for(stadt=0;stadt<4;stadt++)
    if (city_nr[map][stadt]==citynum) break;

  hafen_wert=STADT_1_HAFEN+stadt;
  if (hafen_wert>STADT_2_HAFEN) internal(romstr164);

  for(x=0;x<80;x++)
    for(y=0;y<36;y++)           /* Nach Eingang suchen */
      if (get_raster(x,y)==hafen_wert) {
        hafenx=x;
        hafeny=y;
        }

  if (get_raster(hafenx+1,hafeny)==WASSER) {
    wasserx=hafenx+1;
    wassery=hafeny;
    }
  else if (get_raster(hafenx-1,hafeny)==WASSER) {
    wasserx=hafenx-1;
    wassery=hafeny;                          /* Wasser neben dem Hafen ? */
    }
  else if (get_raster(hafenx,hafeny+1)==WASSER) {
    wasserx=hafenx;
    wassery=hafeny+1;
    }
  else if (get_raster(hafenx,hafeny-1)==WASSER) {
    wasserx=hafenx;
    wassery=hafeny-1;
    }
	
  hafenx=wasserx*4+2;
  hafeny=25+wassery*4+2;              /* in Normalkoordinaten umrechnen */
}

void such_eingang()
{
            /* sucht den zugehîrigen Eingang, zu der Stadt, in der Spieler ist */
  int grund_wert;    /* Untergrundwerte */
  int stadt;

  for(stadt=0;stadt<4;stadt++)
    if (city_nr[map][stadt]==citynum) break;

  grund_wert=STADT_1_EIN+stadt;
  if (grund_wert>STADT_2_EIN) internal(romstr165);

	such_land(grund_wert);
}

void such_land(grund_wert)
int grund_wert;
{
	/* Sucht in der aktiven Groundmap nach dem entsprechenden Eintrag und positioniert
	   den Spieler auf ein naheliegendes freies Landfeld */
	
	register int x,y;
	int eingangx,eingangy;
	
  for(x=0;x<80;x++)
    for(y=0;y<36;y++)           /* Nach Eingang suchen */
      if (get_raster(x,y)==grund_wert) {
        grundx=x;
        grundy=y;

				eingangx=-1;
				
			  if (is_ground(grundx+1,grundy)) {
			    eingangx=grundx+1;
			    eingangy=grundy;
			    }
			  if (is_ground(grundx-1,grundy)) {
			    eingangx=grundx-1;
			    eingangy=grundy;                          /* Wasser neben dem Hafen ? */
			    }
			  if (is_ground(grundx,grundy+1)) {
			    eingangx=grundx;
			    eingangy=grundy+1;
			    }
			  if (is_ground(grundx,grundy-1)) {
			    eingangx=grundx;
			    eingangy=grundy-1;
			    }
			
				if (eingangx==-1) internal(romstr166);
				
			  grundx=eingangx*4+2;
			  grundy=25+eingangy*4+2;              /* in Normalkoordinaten umrechnen */
				return;
				}
	
	internal(romstr167);
}

void armory(num)
int num;
{
  int company;
  long costs;                   /* Kosten fÅr die selektierten Prozent */
  long anzahl;                   /* Anzahl an SOldaten die selektiert sind */
  int selektion;                /* Was, Schild, RÅstung, oder Schwert */
  FLAG newmoney,newcompany,newwerte,newselect;
  FLAG all;

  leiste_oben(romstr168);
  c_pic(ARMORY_OBJ);
  leiste_oben(romstr169);

	open_window(7,69);
	
  redraw_buttons(EXIT_BTN|BUY|ALL|PAUSE);

  company=0;                        /* Infantry */
  anzahl=100;

  Hm();
    writexy(0,16,90,romstr170
										romstr171
										romstr172);

    writexy(0,16,120,romstr173
										 romstr174);
    writexy_money(16,144);
    writexy(0,132,120,romstr175
											romstr176);

  writexy_ausruestung(company);                /* Schreibt die Preise der AusrÅstung hin */
  Sm();

  newmoney=newselect=newcompany=newwerte=TRUE;
  all=FALSE;
  selektion=0;                                  /* Schild selektiert */
  rec(112,80,168,88);

  do {
    Hm();
    if (newcompany) {
      newcompany=FALSE;
      writexy(0,52,120,armeename[company]);
      writexy(0,52,126,str(8,armeeteil[0][company]));
      newwerte=TRUE;
      }
    if (newwerte) {
      newwerte=FALSE;
      writexy_ausruestung(company);
      newselect=TRUE;
      }
    if (newmoney) {
      newmoney=FALSE;
      writexy_money(16,144);
      }
    if (newselect) {
      newselect=FALSE;
      writexy(0,180,120,str(8,anzahl));
      costs=anzahl*preis_aus[selektion];
      writexy(0,168,126,str(9,costs));
      wait_once(12);
      }
    Sm();

    wait_klick();

		if (maus_in(303,69,311,77)) button=EXIT_BTN;				/* Closer */

    if (mk==1) {
      if (maus_in(112,70,300,100)) {
        Hm();
        drec(112+selektion*56,80,168+selektion*56,88);
        selektion=(mx-112)/56;
        if (selektion>2) selektion=2;
        rec(112+selektion*56,80,168+selektion*56,88);
        newselect=TRUE;
        Sm();
        }
      if (maus_in(50,110,100,140)) {            /* Company */
        company++;
        if (company==EINHEITEN) company=0;
        newcompany=TRUE;
        Krec(50,118,106,126);
        }
      if (maus_in(160,117,176,130))            /* - */
        if (anzahl>100) {
          anzahl-=100;
          newselect=TRUE;
          }
      if (maus_in(212,117,230,130))
        if (anzahl<99999900) {
          anzahl+=100;
          newselect=TRUE;
          }
      }

    if (button==ALL) {
      all=!all;                         /* Flag umdrehen */
      if (all) set_mouse(1);            /* Pfeilzeiger */
      else set_mouse(0);                /* normales Kreuz */
      }
    if (button==BUY) {
      if (all) anzahl=99999900;
      if (anzahl>armeeteil[0][company]-ausruestung[selektion][company])
         anzahl=armeeteil[0][company]-ausruestung[selektion][company];
      costs=anzahl*preis_aus[selektion];
      if (costs>money) {
        money_alert();
        }
      else {
        ausruestung[selektion][company]+=anzahl;
        money-=costs;
        newwerte=newwerte=newmoney=TRUE;
        }
      }
    } while (button!=EXIT_BTN);

  button=NOTHING;
  set_mouse(0);                         /* Normales Kreuz */

	close_window();
  city_restore(num);
}

void writexy_ausruestung(company)
int company;
{
  /* Schreibt die Preise der AusrÅstung hin */

  Hm();
  writexy_aus(0,112,company);
  writexy_aus(1,168,company);
  writexy_aus(2,224,company);
  Sm();
}

void writexy_aus(nr,x,company)
int nr,x,company;
{
  writexy(1,x+16,82,aus_name[nr]);
  writexy(0,x,90,str(9,ausruestung[nr][company]));
  writexy(0,x,96,str(9,(long)armeeteil[0][company]-ausruestung[nr][company]));
  writexy(0,x,102,str(9,(long)preis_aus[nr]));
}

void status(txt1,txt2)
char txt1[],txt2[];
{
  /* Gibt den momentanen Status aus */
  char zeile[80];
  int i;
  int stadt_zahl;
  long warenzahl;
  int ring_zahl;

  loc = CITY;
  leiste_y=64;
  msminy=63;                     /* hîher darf Maus nicht */

	clear_raster();
	
  option_city();                /* Wenn Anzeige an, dann ausschalten */

  Hm();
  clear_screen(hlpbuf);
  load_objekte(BUBECK_OBJ,scr2);
	draw_obj(2,scr2,0,hlpbuf,160-objekt_breite(2,scr2)/2,0);
  formular(hlpbuf,63);

  city_screen();													/* und anzeigen */

  if (txt1[0]!=0 && effects) {
		load_digisound(LACHEN_SEQ,pack_buf+64000L);
		play_digi(pack_buf+64000L,FALSE,0,0); 					/* Lachen */
		}
		
  leiste_oben(romstr177);
  redraw_buttons(EXIT_BTN|PAUSE);

  strcpy(zeile,romstr178);
  strapp(zeile,player_name);               /* String anhÑngen */

  center(0,72,zeile);
  writexy(0,16,84,romstr179);
  for(i=0;i<EINHEITEN;i++) {
    writexy(0,24,90+i*6,armeename[i]);
    writexy(0,72,90+i*6,romstr180);
    writexy(0,80,90+i*6,str(10,armeeteil[0][i]));
    }
  writexy(0,80,132,romstr181);
  writexy(0,76,138,str(11,armeegesamt[0]));
  writexy_money(16,150);
  writexy(0,120,150,romstr182);
  writexy(0,156,150,str(10,kontostand));

  writexy(0,128,84,romstr183);
  for(i=0;i<SCHIFFSTYPEN;i++) {
    writexy(0,132,90+i*6,ship_name[i]);
    writexy(0,176,90+i*6,romstr184);
    writexy(0,184,90+i*6,str(2,(long)ship_own[i]));
    }
  writexy(0,184,90+SCHIFFSTYPEN*6,romstr185);
  writexy(0,184,96+SCHIFFSTYPEN*6,str(2,(long)schiffbesitz));

  stadt_zahl=0;
  for(i=0;i<CITIES;i++)
    if (belong[i]<0) stadt_zahl++;

  strcpy(zeile,romstr186);
  strapp(zeile,str(0,(long)stadt_zahl));
  strapp(zeile,romstr187);
  writexy(0,204,84,zeile);

  strcpy(zeile,romstr188);
  strapp(zeile,str(0,(long)minenzahl));
  strapp(zeile,romstr189);
  writexy(0,204,114,zeile);

  warenzahl=0;
  for(i=0;i<WAREN;i++)
    warenzahl+=ycargo_menge[i];

  writexy(0,204,120,romstr190);
  strcpy(zeile,str(0,warenzahl));
  strapp(zeile,romstr191);
  writexy(0,204,126,zeile);

  if (txt1[0]!=0) {
    writexy(0,204,138,romstr192);
    writexy(1,204,144,txt1);
    writexy(1,204,150,txt2);
    }

  Sm();

  do {
    wait_klick();
    } while(button!=EXIT_BTN);

	if (txt1[0]!=0) totenkopf();
}

FLAG is_mine(land,x,y)
int land,x,y;                    /* Schaut, ob an dieser Rasterkoordinate eine
                            Mine ist */
{
  int i;

  for(i=0;i<minenzahl;i++)
    if (mine[i].land==land && mine[i].x==x*4+2 && mine[i].y==y*4+27)
        return(TRUE);

  return(FALSE);
}

void leave(num)
int num;
{
	loc=LAND;										
	
  if (auf_schiff) alert(build(romstr193,c_name[num]));
  else
    if (sum_waren()+sum_rohstoffe()>frei_waggon())
      alert(romstr194);
    else {
      leiste_oben(romstr195);
      if(yes_no(build(romstr196,c_name[num]))) {
        verlassen = TRUE;
        }
      }

	loc=CITY;
}

void barracks(num)
int num;
{
  int i;
  FLAG army_changed;                    /* Wurde ein Soldat verschoben? */
  FLAG money_changed;                   /* Wurde das Geld verÑndert */
  FLAG menge_changed;                   /* Wurde die Anzahl der Transnfersold. */
  long summe;                           /* Summe der Soldaten */
  float sum;
  int company;
  int transfer;                         /* Anzahl der Soldaten, die transferiert */
  long anzahl_vorher;

  leiste_oben(romstr197);
  c_pic(BARRACKS_OBJ);
  leiste_oben(romstr198);

  if (belong[num]>=0 && !kaserne_cheat) {
    alert(romstr199);
    city_restore(num);
    return;
    }

  redraw_buttons(EXIT_BTN|PAUSE);

  Hm();
  writexy(0,16,72,romstr200);
  writexy(0,216,72,romstr201);

  for(i=0;i<EINHEITEN;i++) {
    writexy(0,24,84+i*6,armeename[i]);
    writexy(0,24+13*4,84+i*6,romstr202);
    writexy(0,24+13*4+13*4,84+i*6,romstr203);
    draw_delay(216,84+i*6,adelay[num][i]);
    }
  writexy(0,24+15*4,84+7*6,romstr204);
  writexy(0,24+38*4,84+7*6,romstr205);
  writexy(0,120,150,romstr206);

  Sm();
  army_changed=money_changed=menge_changed=TRUE;

  transfer=100;

  do {
    Hm();
    if (army_changed) {
      army_changed=FALSE;
      summe=0;
      for(i=0;i<EINHEITEN;i++) {
        writexy(0,24+15*4,84+i*6,str(10,armeeteil[0][i]));
        writexy(0,24+38*4,84+i*6,str(8,stadt_armee[num][i]));
        summe+=stadt_armee[num][i];
        }
      writexy(0,24+15*4,84+8*6,str(10,armeegesamt[0]));
      writexy(0,24+38*4,84+8*6,str(8,summe));
      }
    if (money_changed) {
      money_changed=FALSE;
      writexy_money(16,150);
      }
    if (menge_changed) {
      menge_changed=FALSE;
      for(i=0;i<EINHEITEN;i++)
        writexy(0,140,84+i*6,str(4,(long)transfer));
      writexy(0,120+5*4,150,str(4,(long)transfer));
      wait_once(10);
      }
    Sm();

    wait_klick();

    if (mk==1) {
      Hm();
      if (mx>=216) {                    /* Delay verÑndern */
        company=(my-84)/6;
        if (company>=0 && company<EINHEITEN) {
          if (mx>296) mx=296;
          adelay[num][company]=((mx-216)*100)/80;        /* Wert zwischen 0..99 */
          draw_delay(216,84+company*6,adelay[num][company]);
          }
        }
      if (my>=144 && my<=159) {
        if (mx>=116 && mx<=127)                /* -- */
          if (transfer>=110) {
            transfer-=100;
            menge_changed=TRUE;
            }
        if (mx>=128 && mx<=141)                /* - */
          if (transfer>=20) {
            transfer-=10;
            menge_changed=TRUE;
            }
        if (mx>=157 && mx<=168)             /* + */
          if (transfer<9990) {
            transfer+=10;
            menge_changed=TRUE;
            }
        if (mx>=169 && mx<=180)
          if (transfer<9900) {
            transfer+=100;
            menge_changed=TRUE;
            }
        }
      if (mx>=126 && mx<=140) {             /* <- */
        company=(my-84)/6;
        if (company>=0 && company<EINHEITEN && stadt_armee[num][company]!=0 &&
                  transfer!=0) {
          if (stadt_armee[num][company]<transfer) {
            transfer=stadt_armee[num][company];
            menge_changed=TRUE;
            }
          for(i=0;i<EIGENSCHAFTEN;i++) {
            sum=training[0][company][i]*armeeteil[0][company];
            sum+=stadt_training[num][company][i]/100.0*transfer;
            training[0][company][i]=sum/(armeeteil[0][company]+transfer);
            }
          stadt_armee[num][company]-=transfer;
          anzahl_vorher=armeeteil[0][company];
          armeeteil[0][company]+=transfer;
          armeegesamt[0]+=transfer;
          gehalt[company]+=preis_einheiten[company]*(long)transfer;
          gesamt_gehalt+=preis_einheiten[company]*(long)transfer;

          for(i=0;i<3;i++)
            if (stadt_aus[num][i][company]<transfer) {
              ausruestung[i][company]+=stadt_aus[num][i][company];
              stadt_aus[num][i][company]=0;
              }
            else {
              ausruestung[i][company]+=transfer;
              stadt_aus[num][i][company]-=transfer;
              }

          if (anzahl_vorher==0)                         /* Vorher waren keine da... */
            for(i=0;i<RASSEN;i++)                       /* -> Alle Rassen 10% */
              sold_zahl[company][i]=10.0;

          army_changed=TRUE;
          }
        }
      if (mx>=150 && mx<=170) {                         /* -> */
        company=(my-84)/6;
        if (company>=0 && company<EINHEITEN && armeeteil[0][company]!=0) {
          if (armeeteil[0][company]<transfer) {
            transfer=armeeteil[0][company];
            menge_changed=TRUE;
            }
          if (transfer!=0) {
            for(i=0;i<EIGENSCHAFTEN;i++) {
              sum=stadt_training[num][company][i]/100.0*stadt_armee[num][company];
              sum+=training[0][company][i]*transfer;
              stadt_training[num][company][i]=100*sum/(stadt_armee[num][company]+transfer);
              }
            armeeteil[0][company]-=transfer;
            armeegesamt[0]-=transfer;
            gehalt[company]-=(long)transfer*preis_einheiten[company];
            gesamt_gehalt-=(long)transfer*preis_einheiten[company];

            /* Wenn alle Tot, dann Rassen % = 0, Training%= 50 */
            if (armeeteil[0][company]==0) {            /* Alle tot? */
              for(i=0;i<RASSEN;i++) sold_zahl[company][i]=0.0;
              for(i=0;i<EIGENSCHAFTEN;i++) training[0][company][i]=50.0;
              }

            for(i=0;i<3;i++)                 /* Alle AusrÅstungen weitergeben */
              if (ausruestung[i][company]>armeeteil[0][company]) {
                stadt_aus[num][i][company]+=(ausruestung[i][company]-armeeteil[0][company]);
                ausruestung[i][company]=armeeteil[0][company];
                }

            stadt_armee[num][company]+=transfer;
            }
          army_changed=TRUE;
          }
        }

      Sm();
      }

  } while(button != EXIT_BTN);

  button = NOTHING;
  city_restore(num);
}

void draw_delay(x,y,wert)
int x,y;
unsigned char wert;
{
  /* Zeitverzîgerung beim Angriff */
  int dx;
	
  dx=((int)wert*80)/100;
  fill(1,x,y,x+dx,y+4);       			           		/* Roter Balken */
  if (wert!=100) fill(2,x+dx,y,x+80,y+4);           	  		/* GrÅner Balken */
}

