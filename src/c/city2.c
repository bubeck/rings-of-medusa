 
/* Dieser Part beinhaltet Abwicklungen innerhalb der Stadt */
/* und zwar alles, was in Medusa II neu ist */

#include "includes.c"           /* Definiert alle Variablen als Extern,... */

int window_x,window_y,window_breite,window_hoehe;

void subway()
{
	/* Spieler will in den Personendungeon klettern */
	FLAG landbunker;
	
	leiste_oben(romstr208);	
	landbunker=dungeon((int)personen_bunker[citynum]);
	if (landbunker) {
		verlassen=TRUE;						/* Landbunker->Raus hier */
		game_loaded=TRUE;					/* Ein neuer Bunker wurde geladen */
		}
	else city_restore(citynum);
}

void bank(num)
int num;
{
  long geld=1000L;         /* Select Geld */
  int kontonew,geldnew,moneynew;
  int i;
  long kreditrahmen;                    /* Wieviel Kredit bekommt er maximal */
  FLAG all;
	
  all=FALSE;

  leiste_oben(romstr209);
  c_pic("bank.obj");
  leiste_oben(romstr210);

  Hm();
	open_window(7,69);
	
  center(0,71,romstr211);
  writexy(0,20,94 ,romstr212);
  writexy(0,20,100,romstr213);
  writexy(0,20,116,romstr214);
  writexy(0,20,126,romstr215);
  writexy(0,20,134,romstr216);

  center(0,145,romstr217);
  center(0,151,romstr218);

  writexy(0,204,102,romstr219);

  redraw_buttons(EXIT_BTN|ALL|ATTACK|BUY|SELL|PAUSE);
  Sm();

  kontonew=moneynew=geldnew=TRUE;
  button=NOTHING;

  do {
    Hm();
    if (kontonew) {
      writexy(0,20+18*4,94,str(8,kontostand));
      if (!start_darlehen) {                /* Hat er sein ~ schon? */
        kreditrahmen=3000L;
        }
      else {                                /* Er hat es schon */
				/* Zuerst die Deckung berechnen: */
        kreditrahmen=0;
        for(i=0;i<WAREN;i++) kreditrahmen+=ycargo_menge[i]*waren_preis[i];
        for(i=0;i<STALLWAREN;i++) kreditrahmen+=ystable_menge[i]*waren_preis[i+WAREN];
        for(i=0;i<ROHSTOFFE;i++) kreditrahmen+=ymetal[i]*waren_preis[i+WAREN+STALLWAREN];
        kreditrahmen+=minenzahl*10000L;
				kreditrahmen+=money;
        if (kontostand<0)						/* Er hat schon einen Kredit */
          kreditrahmen+=kontostand*2;
        else kreditrahmen+=kontostand;

        if (kreditrahmen<0) kreditrahmen=0;
        }
      writexy(0,20+18*4,100,str(8,kreditrahmen));
      }
    if (moneynew) {
      writexy_money(44,108);
      }
    if (geldnew) {
      writexy(0,20+23*4,116,str(6,geld));
      wait_once(15);
      }
    Sm();

    if (zinsen>0) {                    /* Er hat Gewinn gemacht */
      alert(build(romstr220,str(0,zinsen)));
      kontostand+=zinsen;
      kontonew=TRUE;
      zinsen=0;
      schulden=FALSE;
      continue;
      }
    if (zinsen<0) {
      if (money>(-zinsen)) {
        alert(build(romstr221,str(0,-zinsen)));
        money+=zinsen;                  /* Zinsen ist negativ! */
        moneynew=TRUE;
        zinsen=0;
        schulden=FALSE;
        continue;
        }
      else {
        alert(build(romstr222,str(0,-zinsen)));
        break;
        }
    }

    bank_zuletzt=heute;             /* Heute zuletzt die Bank betreten */

    wait_klick();
		
    geldnew=moneynew=kontonew=FALSE;

		if (maus_in(303,69,311,77)) button=EXIT_BTN;
		
    if (button==ALL) {
      all=!all;                         /* Flag umdrehen */
      if (all) set_mouse(1);            /* Pfeilzeiger */
      else set_mouse(0);                /* normales Kreuz */
      }

    if (maus_in(127,110,148,125) && geld<1000000L-1000L) {
      geld+=1000;
      geldnew=TRUE;
      }
    if (maus_in(88,110,99,125) && geld>10000L) {
      geld-=10000;
      geldnew=TRUE;
      }
    if (maus_in(99,113,111,123) && geld>1000L) {
      geld-=1000;
      geldnew=TRUE;
      }
    if (maus_in(148,113,160,123) && geld<1000000L-10000L) {
      geld+=10000L;
      geldnew=TRUE;
      }
    if (maus_in(204,102,264,108)) {
      Krec(203,100,264,108);
      aktienmarkt();
      break;                    /* und raus */
      }
    if (button==ATTACK) {
      alert(build(romstr223,str(0,city_bank[num])));
      money+=city_bank[num];
      city_bank[num]=0;
      moneynew=TRUE;
      for(i=0;i<4;i++)
        if (city_nr[map][i]>=0) bank_attacked[city_nr[map][i]]=TRUE;
      }
    if (button==SELL) {
      if (bank_attacked[num])
        alert(romstr224);
      else {
        if (all) geld=kreditrahmen;
        if (geld>kreditrahmen)
          geld=kreditrahmen;

        start_darlehen=TRUE;                  /* Er hat sein Startdarlehen */
        money+=geld;
        kontostand-=geld;
        geld=1000;
        geldnew=moneynew=kontonew=TRUE;
        }
      }
    if (button==BUY) {
      if (all) geld=money;
      if (money<geld)
        geld=money;
      money-=geld;
      kontostand+=geld;
      geld=1000;
      geldnew=moneynew=kontonew=TRUE;
      }
    } while (button!=EXIT_BTN);

	close_window();
	
  test_schulden();              /* Schaut ob Spieler Schlden hat */
  button=NOTHING;
  set_mouse(0);                 /* Kreuz wieder an */
  city_restore(num);
}

void aktienmarkt()
{
  /* Spieler kann mit Aktien spekulieren */
  long spieler_aktie[WAREN];
  int i;

  for(i=0;i<WAREN;i++) spieler_aktie[i]=2000-aktie_da[i];

  buy_sell(romstr225,WAREN,(char *)waren_name,11,aktie_preis,aktie_da,
                spieler_aktie,AKTIEN);
}

void buy_sell(name,waren_max,warenn,warenn_len,waren_preis,waren_city,waren_player,ort)
char *name;                 /* Store, Stable, etc. */
int waren_max;              /* Anzahl der Waren */
char *warenn;               /* Name der Waren als Feld */
int warenn_len;             /* Länge in Bytes eines Namens */
uint16_t waren_preis[];          /* Preis in der Stadt */
long waren_city[];          /* Anzahl der Waren in der Stadt */
long waren_player[];        /* Anazhl der Waren bei Player */
int ort;                    /* siehe const.c */
{
  int seite = 0;
  int chosen_menge = 10;
  int waren_nummer = 0;     /* Erste Ware ist selektiert */
  int moneynew,newprice;
  long betrag;
  FLAG all;                 /* Soll alles gekauft werden? */

  all=FALSE;                /* Am Anfang nicht alles kaufen */
  moneynew=newprice=TRUE;               /* Geld anzeigen */

  show_buy_sell(name,waren_max,warenn,warenn_len,waren_preis,
            waren_city,waren_player,ort,seite,waren_nummer);

  do
  {
    if (moneynew) {
      Hm();
      writexy_money(114,122);
      if (ort==LADEN || ort==JUWELLIER) {
        balken(132,140,frei_cargo(),sum_goods());
        balken(132,152,frei_waggon(),sum_waren()+sum_rohstoffe());
        }
      moneynew=FALSE;
      Sm();
      }
    if (newprice) {
      Hm();
      if (ort==AKTIEN)
        betrag=chosen_menge*kurswert(waren_nummer);    /* Maklergebühr */
      else
        betrag=(long)waren_preis[waren_nummer]*chosen_menge;

      writexy(0,166,116,str(8,(long)betrag));
      writexy(0,178,110,str(4,(long)chosen_menge));
      wait_once(6);
      Sm();
      newprice=FALSE;
      }

    wait_klick();

    if (button==ALL) {
      all=!all;                         /* Flag umdrehen */
      if (all) set_mouse(1);            /* Pfeilzeiger */
      else set_mouse(0);                /* normales Kreuz */
      }
    if(button == BUY && (ort==LADEN || ort==AKTIEN)) {
      button = NOTHING;
      if (all) {                    /* Spieler will alles kaufen */
        chosen_menge=waren_city[waren_nummer];      /* Alles selektieren */
        newprice=TRUE;
        }

      if(waren_city[waren_nummer]<chosen_menge) {    /* Nicht genug da... */
        chosen_menge=waren_city[waren_nummer];      /* Alles was da ist kaufen */
        newprice=TRUE;
        }

      if (ort==AKTIEN)
        betrag=chosen_menge*kurswert(waren_nummer)*1.02;    /* Maklergebühr */
      else
        betrag=(long)waren_preis[waren_nummer]*chosen_menge;

      if (betrag>money)
        money_alert();
      else {
        waren_player[waren_nummer] += chosen_menge;
        money-=betrag;
        moneynew=TRUE;
        waren_city[waren_nummer]-=chosen_menge;
        ausrahmen(waren_nummer,seite);
        store_goods(waren_max,warenn,warenn_len,waren_preis,waren_city,seite,ort);
        your_goods(waren_max,warenn,warenn_len,waren_player,seite);
        einrahmen(waren_nummer,seite);
        }
      }
    if(button == SELL) {
      button = NOTHING;
      if (all) {
        chosen_menge=waren_player[waren_nummer];    /* Alles verkaufen */
        newprice=TRUE;
        }
      if (waren_player[waren_nummer]<chosen_menge) {
        chosen_menge=waren_player[waren_nummer];     /* Alles was da ist verkaufe */
        newprice=TRUE;
        }

      if (ort==AKTIEN)
        betrag=chosen_menge*kurswert(waren_nummer)*0.98;    /* Maklergebühr */
      else
        betrag=(long)waren_preis[waren_nummer]*chosen_menge;

      if (ort==LADEN || ort==AKTIEN) waren_city[waren_nummer] += chosen_menge;
      waren_player[waren_nummer]-=chosen_menge;
      money+=betrag;
      moneynew=TRUE;
      ausrahmen(waren_nummer,seite);
      store_goods(waren_max,warenn,warenn_len,waren_preis,waren_city,seite,ort);
      your_goods(waren_max,warenn,warenn_len,waren_player,seite);
      einrahmen(waren_nummer,seite);
      }

  	if (maus_in(303,69,311,77)) button=EXIT_BTN;						/* Closer */
		
	  if (maus_in(303,83,309,153)) {									/* Scrollbalken */
			Hm();
			draw_obj_part(0,pack_buf+32000,299,14,304,84,MOVE,scr1,304,83);		/* Alten löschen */
			move_objekt(1,pack_buf+32000,MOVE,0,pack_buf,
									STOP_R|STOP_L|STOP_O|STOP_U,304,83,308,154,TRUE);
			hol_maus();
      ausrahmen(waren_nummer,seite);        						/* Alten Rahmen löschen */
			if (my>118 && waren_max>=10) {
				seite=1;
				waren_nummer=10;
				}
			else {
				seite=0;
				waren_nummer=0;
				}
			draw_obj(1,pack_buf+32000,MOVE,scr1,304,(seite==0) ? 83 : 119);	/* Scrollbalken zeichnen */

      store_goods(waren_max,warenn,warenn_len,waren_preis,waren_city,seite,ort);
      your_goods(waren_max,warenn,warenn_len,waren_player,seite);

      einrahmen(waren_nummer,seite);
      writexy(0,114,98,space(warenn_len));
      writexy(0,114,98,&warenn[warenn_len*waren_nummer]);
			Sm();
      newprice=TRUE;
      button = NOTHING;
      }
    if (ort==AKTIEN && maus_in(132,134,184,140)) {             /* Chart Analyse */
      Krec(120,132,204,140);
      if (money<2000) money_alert();
      else {
        if (yes_no(romstr226)) {
          money-=10000;
          chart_show(waren_nummer);
          show_buy_sell(name,waren_max,warenn,warenn_len,waren_preis,
                  waren_city,waren_player,ort,seite,waren_nummer);
          moneynew=newprice=TRUE;               /* Geld anzeigen */
          button=NOTHING;
          }
        }
      }
    if(mk == 1) {
      if (my>=101 && my<=111) {
        if (mx>=189 && mx<=199 && chosen_menge<9990) {
          chosen_menge+=10;
          newprice=TRUE;
          }
        if (mx>=181 && mx<=188 && chosen_menge<9999) {
          chosen_menge+=1;
          newprice=TRUE;
          }
        if (mx>=172 && mx<=180 && chosen_menge>1) {
          chosen_menge-=1;
          newprice=TRUE;
          }
        if (mx>=161 && mx<=171 && chosen_menge>10) {
          chosen_menge-=10;
          newprice=TRUE;
          }
        }

      if(maus_in(13,89,94,149) || maus_in(213,89,294,149)) {
        ausrahmen(waren_nummer,seite);
        if ((my-90)/6+10*seite<waren_max)
          waren_nummer = (my-90)/6 + 10 * seite;
	      einrahmen(waren_nummer,seite);

        Hm();
        writexy(0,114,98,space(warenn_len-1));
        writexy(0, 114,98,&warenn[warenn_len*waren_nummer]);
        newprice=TRUE;
        Sm();
        }
      }
  } while(button != EXIT_BTN);
  button=NOTHING;
  set_mouse(0);                     /* Kreuzzeiger */

	close_window();
}

void show_buy_sell(name,waren_max,warenn,warenn_len,waren_preis,
        waren_city,waren_player,ort,seite,waren_nummer)
char *name;                 /* Store, Stable, etc. */
int waren_max;              /* Anzahl der Waren */
char warenn[];              /* Name der Waren als Array */
int warenn_len;             /* Länge in Bytes eines Namens */
uint16_t waren_preis[];     /* Preis in der Stadt */
long waren_city[];          /* Anzahl der Waren in der Stadt */
long waren_player[];        /* Anazhl der Waren bei Player */
int ort;                    /* siehe const.c */
int seite,waren_nummer;
{
  /* Baut Bildschirm für Buy_sell neu auf */

  Hm();

  if (ort==JUWELLIER) redraw_buttons(EXIT_BTN|SELL|ALL|PAUSE);      /* kein More,Sell */
  else redraw_buttons(EXIT_BTN|BUY|SELL|ALL|PAUSE);

	open_window(7,69);
	draw_obj(1,pack_buf+32000,MOVE,scr1,304,(seite==0) ? 83 : 119);	/* Scrollbalken zeichnen */
	
  center(0,71,name);                 /* Überschrift schreiben */
  writexy(0,14,80,romstr227);
  writexy(0,  54,  80,romstr228);
  if (ort==LADEN || ort==AKTIEN) writexy(0,78,80,romstr229);

  writexy(0, 214,  80,romstr230);
  writexy(0, 114,  92,romstr231);
  writexy(0,114,98,&warenn[warenn_len*waren_nummer]);
  writexy(0, 114, 104,romstr232);
  writexy(0, 114, 116,romstr233);
  if (ort==LADEN || ort==JUWELLIER) {
    writexy(0,132,134,romstr234);             /* Schiff/Wagen Balken */
    writexy(0,132,146,romstr235);
    }
  else {
    writexy(0,132,134,romstr236);
    writexy(0,124,144,romstr237);
		line(0,125,150,191,150);
    }

  store_goods(waren_max,warenn,warenn_len,waren_preis,waren_city,seite,ort);
  your_goods(waren_max,warenn,warenn_len,waren_player,seite);
  einrahmen(waren_nummer,seite);                   /* ersten einrahmen */

  Sm();
}

void store_goods(waren_max,warenn,warenn_len,waren_preis,waren_city,seite,ort)
int waren_max;              /* Maximalzahl der Waren */
char warenn[];              /* Name der Waren als Array */
int warenn_len;             /* Länge eines Warennamens in Byte */
uint16_t waren_preis[];     /* Preise der Waren in der Stadt */
long waren_city[];          /* Anzahl der Waren in der Stadt */
int seite;                  /* darzustellende Seite */
int ort;                    /* siehe const.c */
{
  int i;
  int max;              /* Wert bis zu dem gegangen wird */
  int color;
  long preis;

  Hm();

  max=waren_max-seite*10-1;   /* Werte bis zum Ende ab der Seite */
  if (max>9) max=9;         /* mehr als 0..9 dann nur 10 Werte anzeigen */
  for(i = 0; i <=max; i++) {
		color=0;
    if (ort==LADEN || ort==AKTIEN)
      if (waren_city[i+seite*10]==0)
        color=1;           			           /* Rot */
    writexy(0,14,90+i*6,space(warenn_len));
    writexy(color, 14, 90 + i*6, &warenn[warenn_len*(i+seite*10)]);

    if (ort==AKTIEN) {                          /* Kursschwankung */
      preis=kurswert(i+seite*10);
      }
    else preis=waren_preis[i+seite*10];

    /* writexy(0,60,90+i*6,space(5)); */
    writexy(0, 56, 90 + i*6, str(5,preis));
		
    if (ort==LADEN || ort==AKTIEN) {
      writexy(0,76,90+i*6,str(8,waren_city[i+seite*10]));
    	}
   	}
  Sm();
}

void your_goods(waren_max,warenn,warenn_len,waren_player,seite)
int waren_max;              /* Maximalzahl der Waren */
char warenn[];              /* Name der Waren als Array */
int warenn_len;             /* Länge eines Warennamens in Byte */
long waren_player[];         /* Anzahl der Waren, beim Player */
int seite;                  /* darzustellende Seite */
{
  register int i;
  int max;
  int color;

  max=waren_max-seite*10-1;
  if (max>9) max=9;

  Hm();
  for(i = 0; i <=max; i++) {
    writexy(0,214,90+i*6,space(warenn_len));
    if (waren_player[i+seite*10]==0)
      color=1;                      /* Rot */
    else
      color=0;                      /* Schwarz */
    writexy(color, 214, 90 + i*6, &warenn[warenn_len*(i+seite*10)]);
    writexy(0, 270, 90+i*6,str(7,waren_player[i+seite*10]));
    }
  Sm();
}

void einrahmen(warennummer,seite)
int warennummer,seite;
{
  /* Zeichnet den Rahmen */
  rec(11,90+(warennummer-10*seite)*6-1,54,90+(warennummer-10*seite)*6+5);
  rec(211,90+(warennummer-10*seite)*6-1,254,90+(warennummer-10*seite)*6+5);
}

void ausrahmen(warennummer,seite)
int warennummer,seite;
{
  /* Löscht den Rahmen wieder */
  drec(11,90+(warennummer-10*seite)*6-1,54,90+(warennummer-10*seite)*6+5);
  drec(211,90+(warennummer-10*seite)*6-1,254,90+(warennummer-10*seite)*6+5);
}

long kurswert(waren_nummer)
int waren_nummer;
{
  double day_x,wert;

  day_x=(heute-startdatum)/180.0;
  wert=aktie_preis[waren_nummer]+
          (-aktie_fun(aktie_x[waren_nummer]+day_x)*aktie_preis[waren_nummer]*0.1);
  return((long)wert);
}

void get_city_werte(num)
int num;
{
  /* Holt sich alle Stadtspezifischen Werte von Disk */
  long laenge;
  double winkel,zuwachs;
  int i,j;
  long monate;
  int *prod;                 /* zeiger auf Array der Produktionsfaktoren */
  long produktion;
	int hafennum;
	
	hafennum=0;
	for(i=0;i<num;i++) if (port_city[i]) hafennum++;
	
  laenge = load("cities.war",waren_preis,(long)((WAREN+STALLWAREN+ROHSTOFFE)*num*2),
                    (long)(WAREN+STALLWAREN+ROHSTOFFE)*2); /* Preise holen */
  for(i = 0; i < laenge/2;i++) {
    be_2(&waren_preis[i]);
  }
  load("cities.pub",pub_name,(long)(22*num),20L);   /* 20+0 Byte */
  pub_name[20]=0;
  load("cities.por",port_pub,(long)(31*hafennum),29L);   /* 29+0 Byte */
  port_pub[29]=0;

  winkel=((heute+num*15L)%360L)*(2.0*PI/360.0);  /* Periode=1 Jahr */
  zuwachs=(people[num]*0.1)*sin(winkel);            /* Schwankung um 20% */
  bevoelkerung[num]=people[num]+zuwachs;            /* Bevoelkerungszahl */

  /* Warenpreis berechnen:
        große Bevölkerung  -> teure Preise
        kleine Bevölkerung -> niedere Preise
        */
  /* Mengenzuwachs berechnen: */

  if (port_city[num]) prod=prod_port;
  else prod=prod_land;                  /* richtige Produktionsfaktoren */

  monate=((heute-heute%30)-(city_last[num]-city_last[num]%30))/30L;              /* Anzahl der Monate seit letztes Mal */

  for(i=0;i<WAREN+STALLWAREN;i++) {
    if (waren_menge[num][i]<2147480000) {       /* wegen Overflow */
      produktion=(double)prod[i]*((double)bevoelkerung[num]/3000.0)*(double)monate;
      /* Produziert die Stadt das Produkt? */
      if (i<WAREN) 				                     	/* Nur Waren, keine Stallwaren */
        for(j=0;j<5;j++)
          if (produkt[num][j]==i+1) {
            produktion*=2;                   	/* Wird hier produziert */
        		produktion=(double)produktion*(3.0-aktie_da[i]/1000.0);		/* x3 durch alle Aktien */
				    break;
            }
      waren_menge[num][i]+=produktion;
      }
    waren_preis[i]+=(waren_preis[i]*0.1)*(double)sin((double)winkel);
    }

  if (monate>0) {
    crew_grenze[num]=0;         /* Crew ist wieder interessiert */
    }

  for(i=0;i<RASSEN;i++)
    stadt_besatzung[num][i]+=(double)nachwuchs[i]*((double)bevoelkerung[num]/3000.0)*(double)monate;

  if (city_bank[num]<2147000000L) city_bank[num]+=(bevoelkerung[i]/100)*monate;
  calc_zinsen();                /* Zinsen berechnen */
  test_schulden();              /* Schaut ob Spieler Schulden hat */
}

double aktie_fun(x)
double x;
{
  /* Ermittelt die schwankung der Aktien um 10% um Wert */
  double y;

  y=0.5*sin(x)+0.32*sin(2.3+6.1*x)+0.12*sin(1.8+2.7*x);
  if (y>1.0 || y<-1.0) {
    writexy(0,0,0,fstr(romstr238,y));
    wait_key();
    }
  return(y);
}

void chart_show(waren_nummer)
int waren_nummer;
{
  /* Spieler kann sich die Chart's zeigen lassen */
  int i;
  long datum;
  double xalt,yalt,xneu,yneu;
  double day_x;
  long unter_grenze,ober_grenze;            /* 10% darunter, darüber */
  long delta_y;

  Hm();
  formular(scr1,63);

  unter_grenze=aktie_preis[waren_nummer] * 0.9;
  ober_grenze=aktie_preis[waren_nummer]*1.1;
  delta_y=ober_grenze-unter_grenze;

  writexy(0,264,78,waren_name[waren_nummer]);
  writexy(0,264,90,romstr239);
  writexy(0,264,96,str(0,kurswert(waren_nummer)));

  line(0,50,150,50,70);                     /* Y-Achse */
  line(0,45,145,265,145);                   /* X-Achse */
  for(i=0;i<7;i++) {                        /* X */
    writexy(0,22,143-(i*10),str(5,unter_grenze+((long)delta_y*i)/7));
    line(0,49,145-(i*10),51,145-(i*10));
    }
  for(i=0,datum=heute;i<7;i++,datum-=20) {  /* Y */
    line(0,260-35*i,144,260-35*i,146);
    writexy(0,248-35*i,149,dat_kurz(datum));
    }

  day_x=(heute-startdatum)/180.0;
  xalt=260.0;
  yalt=110.0+aktie_fun(aktie_x[waren_nummer]+day_x)*35.0;
  for(i=0;i<120;i++) {
    day_x-=1.0/180.0;                         /* Ein Tag zurück */
    xneu=xalt-1.75;                           /* 1.75 Pixel/Tag */
    yneu=110.0+aktie_fun(aktie_x[waren_nummer]+(double)day_x)*35.0;
    line(0,(int)xalt,(int)yalt,(int)xneu,(int)yneu);
    xalt=xneu;
    yalt=yneu;
    }

  redraw_buttons(EXIT_BTN|PAUSE);
  Sm();

  do {
    wait_klick();
    if (mk==1) {
      heute+=30;                    /* Ein Monat weiter */
      chart_show(waren_nummer);
      }
    } while(button!=EXIT_BTN);
}

void open_window(x0,y0)
int x0,y0;
{
	/* Öffnet ein Fenster an dieser Position */
	void *window;
	
	Hm();
	
	window=pack_buf+32000;
  load_objekte("window.obj",window);

	window_x=x0-2;
	window_y=y0;
	window_breite=objekt_breite(0,window);				/* für close merken */
	window_hoehe=objekt_hoehe(0,window);
	
	draw_grow_obj(0,window,MOVE,scr1,window_x,window_y);
	show_raster();

	Sm();
}

void close_window()
{
	/* Schliesst ein Fenster */
	Hm();
	
	fill(7,window_x,window_y,window_x+window_breite-1,window_y+window_hoehe-1);			/* Fenster weg */
	shrink_box(window_x,window_y,window_breite,window_hoehe);		

	Sm();
}


