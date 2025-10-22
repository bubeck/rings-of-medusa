
/* Hier wird alles was mit Kampf zu tun hat geregelt: */

#include "includes.c"           /* Definiert alle Variablen als Extern,... */

#define KUGEL 1
#define PULVER_LOSE 2       /* Was ist wo in der Kanone */
#define PULVER_FEST 3

FLAG show_gegner;               /* Soll beim Kampf die Werte des Gegners angezeigt werden? */

int medusa_army[EINHEITEN] = {             /* Größe von Medusa's Army */
  10000L,                    /* Inf */
  7000L,                    /* Cav */
  1500L,                    /* Art */
  3000L,                    /* pio */
  2500L,                     /* DRR */
  200L,                     /* Wizra */
  3500L };                    /* Archer */

FLAG geeinigt;                      /* Haben Sie sich bei Terms geeinigt? */

/* Prototyp: */
void Seeschlacht(int nummer);

/* Start: */

void test_fight()
{
  /* Überprüft, ob Spieler mit einem Gegner zusammengetroffen ist */
  int i;

  for(i=0;i<GEGNER;i++)
    if (steuer[i].heimat!=-1)
      if (steuer[i].weltpos==pos_welt && abs(sx[0]-steuer[i].x)<3 &&
          abs(sy[0]-steuer[i].y)<3)
        if (heute-last_fight>2) {          /* mindestens 2 Tage zwischen 2 Kämpfen */
          if (steuer[i].auf_wasser && auf_schiff)       /* Beide im Wasser? */
            Seeschlacht(i+1);
          if (!steuer[i].auf_wasser && !auf_schiff)     /* Beide an Land? */
            fight(i+1);                     /* Kampf mit Armee romstr538 */
          }
}

void minus_einheit(company,tote)
int company;
long tote;
{
  /* Diese Funktion gleicht alle anderen Arrays an, wenn von einer
     Spielereinheit 'company', 'tote' Leute sterben. */
  int i;

  if (tote<=0) return;          /* Keine Toten oder negativ, raus */
  if (tote>armeeteil[0][company]) tote=armeeteil[0][company];

  armeeteil[0][company]-=tote;
  if (armeeteil[0][company]<0) armeeteil[0][company]=0;
  armeegesamt[0]-=tote;
  if (armeegesamt[0]<0) armeegesamt[0]=0;

  /* Wehrsold angleichen: */
  gehalt[company]-=tote*preis_einheiten[company];
  calc_gehalt();

  /* Wenn alle Tot, dann Rassen % = 0, Training%= 50 */
  if (armeeteil[0][company]==0) {            /* Alle tot? */
    for(i=0;i<RASSEN;i++) sold_zahl[company][i]=0.0;
    for(i=0;i<EIGENSCHAFTEN;i++) training[0][company][i]=50.0;
    }

  for(i=0;i<3;i++)                 /* Alle Ausrüstungen weitergeben */
    if (ausruestung[i][company]>armeeteil[0][company])
      ausruestung[i][company]=armeeteil[0][company];
}

void angriff_spieler_stadt(num,armeenr)
int num;                                /* Nummer der Stadt */
int armeenr;                            /* Nummer der Armee */
{
  /* Gegner greift eine Stadt an, die dem Spieler gehört */
  int i,j;
	void *oldlogbase;
	float train[EINHEITEN][EIGENSCHAFTEN];
		
	load_digisound(KRIEG_SEQ,pack_buf+32000L);
	
  if (effects) play_digi(pack_buf+32000L,FALSE,5,14);

  show_window(build(romstr539,c_name[num],gegnername[armeenr-1]));

  loc=CITY;
  leiste_y=64;
  msminy=63;

  option_city();

	clear_raster();
  clear_screen(hlpbuf);
	load_objekte(EBENE_OBJ,scr2);
  formular(hlpbuf,63);
	draw_obj(0,scr2,0,hlpbuf,0,0);								/* Hintergrund zeichnen */
	draw_obj(1,scr2,1,hlpbuf,0,0);								/* Stadt zeichnen */

	oldlogbase=logbase;
	logbase=hlpbuf;
	line(0,0,63,319,63);											/* Oberste Formularzeile löschen */
	logbase=oldlogbase;
	
	/* Werte in Floats kopieren */
  kampf_gesamt[0]=0;
  for(i=0;i<EINHEITEN;i++) {
    kampf_teil[1][i]=armeeteil[armeenr][i];     /* Gegner */
    kampf_teil[0][i]=stadt_armee[num][i];       /* Stadt */
    kampf_gesamt[0]+=stadt_armee[num][i];       /* Werte aufsummieren */
    }
  kampf_gesamt[1]=armeegesamt[armeenr];

  motiv[1]=motivation[armeenr];
  motiv[0]=100;                     /* Stadt zu 100% motiviert */

  for(i=0;i<16;i+=2) {
    deckung[i]=320-deckung_city[i+16];  /* Deckungswerte der Stadt übergeben */
    deckung[i+1]=deckung_city[i+17];
    deckung[16+i]=deckung_kampf[i+16];  /* Gegnerwerte */
    deckung[17+i]=deckung_kampf[i+17];
    }

  for(i=0;i<EINHEITEN;i++)
    for(j=0;j<EIGENSCHAFTEN;j++) {
      train[i][j]=stadt_training[num][i][j]/100.0;
      }

  calc_it(1,EBENE,training[armeenr],motivation[armeenr]);   /* Stärken der Armeen berechnen */
  calc_it(0,EBENE,train,100.0);                 /* Stadt ist voll motiviert */

  init_kampf_sprites();                     /* Liest die Kampfsprites */
  sx[EINHEITEN]=-1;                         /* Stadt hat kein Zelt */

  init_oben();

  city_screen();														/* und anzeigen */
  ground_buf[0]=EBENE;                      /* Kampf mit Stadt immer in Ebene */
  show_ground();                            /* Obere Leiste beschriften */
  sprite_init();                            /* Sprites vorbereiten */

  demo_angriff=TRUE;                        /* Spieler kann nicht beeinflussen */
  stadt_nr=num;                             /* Nummer der Stadt, die angegriffen wird */
  kampf(gegnername[armeenr-1]);

  /* Soldaten in Original Array übertragen */
  armeegesamt[armeenr]=0;
  for(i=0;i<EINHEITEN;i++) {
    armeeteil[armeenr][i]=(long)kampf_teil[1][i];
    armeegesamt[armeenr]+=armeeteil[armeenr][i];
    stadt_armee[num][i]=kampf_teil[0][i];
    }

  motivation[armeenr]=motiv[1];           /* Motivation wieder zurückübergeben  */

  restore_sprites();

  if (kampf_gesamt[0]<1.0) {                /* Stadt hat verloren */
    belong[num]=armeenr-1;                  /* Stadt gehört jetzt Gegner */
    }

  option_city_ende();
  land_an();                              /* Also Land an */
}

void attack_city(num)
int num;
{
  /* Spieler will Stadt angreifen. */
  int i,j;
  long tote;
  float proz,proz_summe;
	void *oldlogbase;
	float train[EINHEITEN][EIGENSCHAFTEN];

	load_digisound(KRIEG_SEQ,pack_buf+32000L);
			
  if (effects) play_digi(pack_buf+32000L,FALSE,5,14);

  loc=CITY;
  leiste_y=64;
  msminy=63;

  option_city();

	clear_raster();
  clear_screen(hlpbuf);
	load_objekte(EBENE_OBJ,scr2);
  formular(hlpbuf,63);
	draw_obj(0,scr2,0,hlpbuf,0,0);								/* Hintergrund zeichnen */
	draw_obj(1,scr2,1,hlpbuf,320-objekt_breite(1,scr2),0);								/* Stadt zeichnen */

	oldlogbase=logbase;
	logbase=hlpbuf;
	line(0,0,63,319,63);											/* Oberste Formularzeile löschen */
	logbase=oldlogbase;

    /* Werte in Floats kopieren */
  kampf_gesamt[1]=0;
  for(i=0;i<EINHEITEN;i++) {
    kampf_teil[0][i]=armeeteil[0][i];               /* SPieler */
    kampf_teil[1][i]=stadt_armee[num][i];       /* Stadt */
    kampf_gesamt[1]+=stadt_armee[num][i];       /* Werte aufsummieren */
    }
  kampf_gesamt[0]=armeegesamt[0];

  motiv[0]=motivation[0];
  motiv[1]=100;                     /* zu 100% motiviert */

  for(i=0;i<16*2;i++) deckung[i]=deckung_city[i];  /* Deckungswerte übergeben */

  for(i=0;i<EINHEITEN;i++)
    for(j=0;j<EIGENSCHAFTEN;j++)
      train[i][j]=stadt_training[num][i][j]/100.0;

  calc_it(0,EBENE,training[0],motivation[0]);   /* Stärken der Armeen berechnen */
  calc_it(1,EBENE,train,100.0);                 /* Voll motiviert */

  if (staerke_cheat)
    for(i=0;i<EINHEITEN;i++)
      kampfkraft[0][i]*=1.5;                    /* stärker */

  /* Jetzt noch die Armory reinrechnen: */
  for(i=0;i<EINHEITEN;i++) {
    proz_summe=1.0;
    for(j=0;j<3;j++) {
      if (armeeteil[0][i]!=0)
        proz=(float)ausruestung[j][i]/armeeteil[0][i];
      else proz=0.0;
      proz_summe+=proz*0.333333333;                /* Jede Ausrüstung bringt 33% */
      }
    kampfkraft[0][i]*=proz_summe;

    proz_summe=1.0;
    for(j=0;j<3;j++) {
      if (stadt_armee[num][i]!=0)
        proz=(float)stadt_aus[num][j][i]/stadt_armee[num][i];
      else proz=0.0;
      proz_summe+=proz*0.333333333;                /* Jede Ausrüstung bringt 33% */
      }
    kampfkraft[1][i]*=proz_summe;
    }

  init_kampf_sprites();                     /* Liest die Kampfsprites */
  sx[EINHEITEN*2+1]=-1;                     /* Gegner hat kein Zelt */

  init_oben();

  city_screen();														/* und anzeigen */
  ground_buf[0]=EBENE;                      /* Kampf mit Stadt immer in Ebene */
  show_ground();                            /* Obere Leiste beschriften */
  sprite_init();                            /* Sprites vorbereiten */

  demo_angriff=FALSE;                       /* Spieler kann Einfluß nehmen */
  kampf(c_name[num]);

  /* Einheiten angleichen */
  for(i=0;i<EINHEITEN;i++) {
    tote=armeeteil[0][i]-kampf_teil[0][i];  /* Anzahl an Toten der Einheit */
    minus_einheit(i,tote);
    }

  /* Gegnerische Armee in Originalarray übertragen: */
  for(i=0;i<EINHEITEN;i++) {
    stadt_armee[num][i]=kampf_teil[1][i];
    }

  motivation[0]=motiv[0];           /* Motivation wieder zurückübergeben  */

  restore_sprites();

  if (belong[citynum]>=0 && belong[citynum]<99)
    for(i=0;i<GEGNER;i++)                 /* Besitzer der Stadt suchen */
      if (steuer[i].heimat==num) {
        steuer[i].angriff_zahl=100;       /* Stadt: Betreten verboten */
        break;
        }

  option_city_ende();

  if (kampf_gesamt[1]<1.0) {               /* Stadt hat verloren */
    belong[num]=-1;                         /* Stadt gehört jetzt Spieler */
    citynum=num;
    }
  else {                                    /* Stadt hat gewonnen */
    citynum=-1;                             /* Spieler betritt Stadt nicht */
    sx[0]=sxalt[0];
    sy[0]=syalt[0];
    land_an();                              /* Also Land an */
    }

  button=NOTHING;
}

void fight(nr)
int nr;
{
  /* Spieler ist mit Armee nr zusammengetroffen */
  int i,j;
  long gewinn;
  long armeestaerke;                    /* Stärke der gegneri. Armee */
  long tote;
  float proz,proz_summe;
	void *oldlogbase;

	load_digisound(KRIEG_SEQ,pack_buf+32000L);
		
  if (effects) play_digi(pack_buf+32000L,FALSE,5,14);

  Hm();
  show_window(romstr540);

  last_fight=heute;                 /* Heute wurde zuletzt gekämpft */

  loc=CITY;
  leiste_y=64;
  msminy=63;

  option_city();

	clear_raster();
  clear_screen(hlpbuf);
	load_objekte(EBENE_OBJ,scr2);
  formular(hlpbuf,63);
	draw_obj(0,scr2,0,hlpbuf,0,0);								/* Hintergrund zeichnen */

	oldlogbase=logbase;
	logbase=hlpbuf;
	line(0,0,63,319,63);											/* Oberste Formularzeile löschen */
	logbase=oldlogbase;

    /* Werte in Floats kopieren */
  for(i=0;i<EINHEITEN;i++) {
    kampf_teil[0][i]=armeeteil[0][i];
    kampf_teil[1][i]=armeeteil[nr][i];
    }
  kampf_gesamt[0]=armeegesamt[0];
  kampf_gesamt[1]=armeegesamt[nr];
  armeestaerke=armeegesamt[nr];

  motiv[0]=motivation[0];
  motiv[1]=motivation[nr];

  for(i=0;i<16*2;i++) deckung[i]=deckung_kampf[i];  /* Deckungswerte übergeben */

  calc_staerke(0);                  /* Stärken der Armeen berechnen */
  calc_staerke(nr);

  if (staerke_cheat)
    for(i=0;i<EINHEITEN;i++)
      kampfkraft[0][i]*=1.5;                    /* stärker */

  /* Jetzt noch die Armory reinrechnen: */
  for(i=0;i<EINHEITEN;i++) {
    kampfkraft[1][i]*=1.66666666;                 /* gegner ist zu 66% ausgerüstet */
    proz_summe=1.0;
    for(j=0;j<3;j++) {
      if (armeeteil[0][i]!=0)
        proz=(float)ausruestung[j][i]/armeeteil[0][i];
      else proz=0.0;
      proz_summe+=proz*0.333333333;                /* Jede Ausrüstung bringt 33% */
      }
    kampfkraft[0][i]*=proz_summe;
    }

  init_kampf_sprites();                     /* Liest die Kampfsprites */

  init_oben();

  city_screen();														/* Und anzeigen */	
  show_ground();                            /* Obere Leiste beschriften */
  sprite_init();                            /* Sprites vorbereiten */
  Sm();

  demo_angriff=FALSE;                       /* Spieler kann beeinflussen */
  kampf(gegnername[nr-1]);                  /* Simuliert den Kampf gegen Armee nr */

  /* Einheiten angleichen */
  for(i=0;i<EINHEITEN;i++) {
    tote=armeeteil[0][i]-kampf_teil[0][i];  /* Anzahl an Toten der Einheit */
    minus_einheit(i,tote);
    }

  /* Gegnerische Armee in Originalarray übertragen: */
  armeegesamt[nr]=0;
  for(i=0;i<EINHEITEN;i++) {
    armeeteil[nr][i]=(long)kampf_teil[1][i];
    armeegesamt[nr]+=armeeteil[nr][i];
    }

  motivation[0]=motiv[0];           /* Motivation wieder zurückübergeben  */
  motivation[nr]=motiv[1];

  if (!geeinigt) {                      /* Gewinnverteilung nur bei Kampf */
    if (kampf_gesamt[1]<1.0) {             /* Gegner hat verloren */
      gewinn=armeestaerke*10L;
      alert(build(romstr541,str(0,gewinn)));
      money+=gewinn;
      }
    if (kampf_gesamt[0]<1.0) {            /* SPieler hat verloren */
      alert(romstr542);
      start_darlehen=FALSE;                   /* Er bekommt wieder Darlehen */
      money=0L;
      for(i=0;i<WAREN;i++) ycargo_menge[i]=0;
      for(i=0;i<ROHSTOFFE;i++) ymetal[i]=0;
      }
    steuer[nr-1].angriff_zahl++;          /* Einmal mehr geprügelt */
    }

  restore_sprites();                    /* Spritewerte zurückkopieren */

  option_city_ende();
  land_an();
}

void init_kampf_sprites()
{
  /* Initialisiert die Variablen für die Kampfsprites */
  int i;

  save_sprites();                       /* Alte Spritepositionen retten */
  init_k_sprites(0,0);                  /* Gegner 0=Spieler */
  init_k_sprites(1,EINHEITEN+1);

  for(i=(EINHEITEN+1)*2;i<(EINHEITEN+1)*2+3;i++)
    sn[i]=-1;                           /* Kein Schild */
}

void save_sprites()
{
  /* Rettet die Originalsprites in die Save_arrays */
  int i;

  for(i=0;i<SPR_MAX;i++) {                      /* Landschaftsprites retten */
    sxalt_save[i]=sxalt[i];                     /* X Koordinaten der MAP Sprites retten */
    syalt_save[i]=syalt[i];                     /* Y s.o. */
    sn_save[i]=sn[i];                   /* Shapenummer retten */
    sx_save[i]=sx[i];
    sy_save[i]=sy[i];

    sx[i]=-1;                           /* Sprites sind frei */
    sn[i]=-1;
    ani[i]=FALSE;                       /* Kein Sprite wird animiert */
    auftrag[i]=HALTEN;
    }
}

void init_k_sprites(armee,index)
int armee;                          /* Nummer der Armee */
int index;                          /* Index, ab dem sn[] etc. gefüllt wird */
{
  /* Bereitet sx,sy,sn,anir für die Animation vor */
  int i;
  int offset;                   /* Offset im array */

  for(i=0;i<EINHEITEN;i++) {                  /* Alle Einheiten+Zelt machen */
    offset=i+index;
    anir[offset]=1;                              /* nach oben animieren */
    if (kampf_teil[armee][i]==0.0) {              /* Einheit tot */
      sn[offset]=-1;                           /* Sprite weg */
      }
    else {
      sn[offset]=ani_start[offset];             /* Start der Animation */
      sx[offset]=deckung[offset*2];
      sy[offset]=deckung[offset*2+1];
      ani[offset]=TRUE;                         /* Wird animiert */
      }
    }

  offset=i+index;                           /* Zelt */
  sn[offset]=ani_start[offset];             /* Start der Animation */
  sx[offset]=deckung[offset*2];
  sy[offset]=deckung[offset*2+1];
  ani[offset]=TRUE;                         /* Wird animiert */
  anir[offset]=1;
}

void restore_sprites()
{
  /* Kopiert die Originalwerte wieder in die Spritearrays sx,sy,sn zurück */
  int i;

  for(i=0;i<SPR_MAX;i++) {
    sxalt[i]=sxalt_save[i];
    syalt[i]=syalt_save[i];
    sn[i]=sn_save[i];
    sx[i]=sx_save[i];
    sy[i]=sy_save[i];
    }
}

void animate(start,ende)
int start,ende;             /* Beginn der Animation, und Ende */
{
  /* Animiert die Kampfsprites */
  int i,j;

  for(i=start;i<ende;i++) {      /* Alle Sprites testen, ob animiert wird */
    if (sx[i]>=0 && ani[i]) {       /* Lebt Einheit noch und soll animiert wd? */
      for(j=0;j<200;j+=2)                   /* Eintrag in Tabelle suchen */
        if (sn[i]<animation[j]) break;
      if (j==200)
        internal(romstr543);
      j-=2;

      sn[i]+=anir[i];                       /* Nächste Animationsstufe */

      if (sn[i]>animation[j+1]) {   /* oberer Wert überschritten */
        sn[i]-=2;
        anir[i]=-1;                 /* und nach unten weiter */
        }
      if (sn[i]<animation[j]) {      /* unterer Wert unterschritten */
        sn[i]+=2;
        anir[i]=1;                  /* und nach oben weiter */
        }
      }
    }
}

void kampf(gegnern)
char gegnern[];                   /* Name des Gegners */
{
  /* Kampfsimulation */
  int i;
  FLAG stattgefunden;               /* Flag, ob Kampf begonnen hat odern nicht */
  int company;
  int counter;
  FLAG kampf_ende;
  char namen[100];

  long durchgang;                   /* Zählt die Anzahl der Bilderneuerungen */

  kampf_ende=geeinigt=FALSE;

  if (demo_angriff) stattgefunden=TRUE;         /* Es gab einen Kampf */
  else stattgefunden=FALSE;              /* Bisher noch kein Kampf */

  company=0;                        /* Infantry angewählt */
  counter=0;
  durchgang=0;
  effektivitaet=1.0;                /* Normal stark */
  new_effekt=TRUE;

  if (demo_angriff)                 /* Spieler kann keinen Einfluß nehmen */
    redraw_buttons(PLUS|MINUS|PAUSE);
  else {
    if (sx[EINHEITEN*2+1]<0 || strcmp(romstr544,gegnern)==0)   /* Gegner hat kein Zelt->Angriff auf Stadt */
      redraw_buttons(PAUSE|ATTACK|FLEE|PLUS|MINUS);          /* Dann keine Terms */
    else redraw_buttons(PAUSE|ATTACK|FLEE|TERMS|PLUS|MINUS);
    }

  for(i=0;i<EINHEITEN;i++) im_kampf[0][i]=im_kampf[1][i]=FALSE; /* keiner kämpft */
  ari_kaempft[0]=ari_kaempft[1]=FALSE;              /* Ari's schießen noch nicht */
  for(i=0;i<SPR_MAX;i++) auftrag[i]=HALTEN;
  front_s=0;
  front_c=320;

  draw_shapes();
  draw_shapes();

  Hm();
  writexy(0,15,74,romstr545);

#ifdef FRANZ
  strcpy(namen,romstr546);
  strapp(namen,gegnern);
#else
  strcpy(namen,gegnern);
  strapp(namen,romstr547);
#endif

  writexy(0,200,74,namen);                          /* Name des Gegners */

  writexy(0,15,82,romstr548);
  writexy(0,200,82,romstr549);
  for(i=0;i<EINHEITEN;i++) {
    writexy(2,23,88+i*6,armeename[i]);
    writexy(0,71,88+i*6,romstr550);
    writexy(2,208,88+i*6,armeename[i]);
    writexy(0,256,88+i*6,romstr551);
    }

  if (!demo_angriff)
    rec(19,87+company*6,71,93+company*6);     /* Infantry selektieren */

  schild_einheit=0;                         /* Infantry mit Schild */

  computer_move();            /* Armeen des Gegners angreifen lassen */

  if (strength_cheat)             /* Kampfkraft der einzelnen Einheiten anzeigen */
    for(i=0;i<EINHEITEN;i++) {
      writexy(0,120,88+i*6,fstr(romstr552,kampfkraft[0][i]));
      writexy(0,160,88+i*6,fstr(romstr553,kampfkraft[1][i]));
      }
  else writexy(0,140,106,romstr554);

  /* copy_zeilen(scr1+72*160L,scr2+72*160L,128); */ /* Kopiert Werte -> Screen2 */
  cpy_raster(scr1,scr2,0,72,319,199,0,72);

  Sm();

  if (money>=2000 && !demo_angriff) {
    if (yes_no(romstr555)) {
      money-=2000;
      show_gegner=TRUE;
      }
    else show_gegner=FALSE;
    }

  Hm();
  /* copy_zeilen(scr2+72*160L,scr1+72*160L,60); */  /* Werte auf beide Screens */
  cpy_raster(scr2,scr1,0,72,319,131,0,72);

  Sm();

  while(!kampf_ende && button!=EXIT_BTN) {
    hol_maus();

    if (mk==1) {
			if (my>169) button_leiste();
			
      if (mx==0) button=EXIT_BTN;                       /* raus hier */

      if (mx<80 && my>86 && my<130 && !demo_angriff) {
        Hm();
        drec(19,87+company*6,71,93+company*6);     /* Einheit deselektieren */
        company=(my-88)/6;
        schild_einheit=company;                   /* Schild neu setzen */
        rec(19,87+company*6,71,93+company*6);     /* Einheit selektieren */
        /* copy_zeilen(scr1+72*160L,scr2+72*160L,60); */ /* Kopiert Werte -> Screen2 */
        cpy_raster(scr1,scr2,0,72,319,131,0,72);
        Sm();
        }
      }

    if (demo_angriff) {
      move_spieler(durchgang);
      }

    if (button==PLUS && effektivitaet<15.0) {
      new_effekt=TRUE;
      effektivitaet*=2.0;
      }
    if (button==MINUS && effektivitaet>1.5) {
      new_effekt=TRUE;
      effektivitaet/=2.0;
      }

    if (button==ATTACK) {
      attack(company);            /* Kompanie greift an */
      stattgefunden=TRUE;         /* Kampf hat stattgefunden */
      button=NOTHING;
      }
    if (button==FLEE) {
      flee(company);
      button=NOTHING;
      }
    if (button==TERMS) {
      if (terms()) {                /* Haben Sie sich geeinigt? */
        geeinigt=TRUE;              /* Ja, haben sie */
        button=EXIT_BTN;
        break;                      /* Raus aus der Schleife */
        }
      else button=NOTHING;              /* Der Kampf geht weiter */
      redraw_buttons(PAUSE|ATTACK|FLEE|PLUS|MINUS);          /* Dann keine Terms */
      copy_buttons();
      }

    mach_schreie();             /* Computer soll im Digisound schreien */
    computer_move();          /* Computer soll ziehen */
    move_einheiten();           /* Einheiten bewegen */
    move_schild();              /* Schild mitziehen */
    war();                    /* Krieg simulieren */
    show_soldiers();            /* aktuellen Zahlen anzeigen */

    counter++;
    if (counter==1) {
      animate(EINHEITEN+1,2*EINHEITEN+2);       /* Computer animieren */
      }
    if (counter==2) {
      counter=0;
      animate(0,EINHEITEN+1);           /* Spieler animieren */
      }

    durchgang++;                        /* Und wieder ein Bild mehr */
    draw_shapes();
    check_end(gegnern);              /* Krieg von einer Partei gewonnen? */
    }

  if (!stattgefunden) {                         /* Es kam nicht zum Kampf */
    motiv[1]+=100-motiv[1]/4;       /* Motivation des Gegners steigt */
    motiv[0]-=100-motiv[0]/3;         /* eigene Motivation sinkt ab */
    }
  if (!geeinigt) {                          /* Nur wenn es zum Kampf kam */
    if (kampf_gesamt[1]==0.0) {               /* Spieler hat Kampf gewonnen */
      motiv[0]+=100-motiv[1]/4;         /* eigene Motivation steigt */
      motiv[1]-=100-motiv[0]/3;         /* Motivation Gegner sinkt ab */
      }
    else {                              /* Spieler hat verloren */
      motiv[0]-=100-motiv[1]/3;         /* eigene Motivation sinkt */
      motiv[1]+=100-motiv[0]/4;         /* Motivation Gegner steigt */
      if (effects) {
				load_digisound(GOT_YOU_SEQ,pack_buf+32000L);
				play_digi(pack_buf+32000L,FALSE,0,0);
				}
      }
    }

  if (motiv[0]>100) motiv[0]=100;
  if (motiv[0]<0) motiv[0]=0;
  if (motiv[1]>100) motiv[1]=100;
  if (motiv[1]<0) motiv[1]=0;

  button=NOTHING;
  clear_buttons(TRUE);                      /* Nur Redraw */
  copy_buttons();                           /* Leiste->Screen2 */
}

void mach_schreie()
{
  /* Computer läßt über den Digisound ein paar Schreie los. */
  int rnd;

  if (!digi_works && effects) {
    rnd=zufall(50);
    if (rnd<5) play_digi(pack_buf+32000L,FALSE,rnd,rnd);
    }
}

void move_spieler(durchgang)
long durchgang;
{
  /* Die Stadt eines Spielers wird angegriffen, jetzt wehrt sie sich automatisch
     analog zu den Einstellungen, die der Spieler in den Barracks gemacht hat. */
  int i;

  for(i=0;i<EINHEITEN;i++)
    if (auftrag[i]!=ANGRIFF)
      if (adelay[stadt_nr][i]!=100 && durchgang>=adelay[stadt_nr][i]) attack(i);
}

FLAG terms()
{
  /* Spieler will verhandeln */
  long select;
  FLAG abbruch;                             /* Wird TRUE, wenn Kampf zuende */
  long oldleiste;

  select=1000;
  abbruch=FALSE;

  Hm();
  oldleiste=bleiste;
  redraw_buttons(EXIT_BTN|PLUS|MINUS|PAUSE);

  writexy(0,20,135,romstr556
										romstr557
										romstr558);
  writexy_money(210,135);

  Sm();

  do {
    Hm();
    writexy(0,60,153,str(5,select));
    wait_once(10);
    Sm();

    wait_klick();

    if (button==MINUS && select>1000) {               /* - */
      select-=1000;
      }
    if (button==PLUS && select+1000<=money && select<99000L) {  /* + */
      select+=1000;
      }
    if (maus_in(18,133,176,148)) {
      Krec(19,133,173,147);
      if (bloeff()) {                       /* Computer geht drauf ein */
        abbruch=TRUE;
        }
      button=EXIT_BTN;                          /* und raus */
      }
    if (maus_in(19,150,160,160)) {
      Krec(19,151,152,159);
      if (pay_you(select))
        abbruch=TRUE;
      button=EXIT_BTN;
      }
    } while(button!=EXIT_BTN);

  Hm();
  redraw_buttons(oldleiste);
  copy_buttons();                           /* Buttons auf 2. Bildschirm */
  fill(7,20,135,210,157);
  fill(7,210,135,299,141);
  Sm();
  return(abbruch);
}

FLAG pay_you(angebot)
long angebot;
{
  /* Spieler bietet dem Computer ein Angebot, geht er drauf ein? */
  long dif;

  dif=kampf_gesamt[1]-kampf_gesamt[0];
  if (dif*10<=angebot) {                    /* Computer geht drauf ein! */
    alert(romstr559);
    money-=angebot;
    return(TRUE);
    }
  else {
    alert(romstr560);
    return(FALSE);
    }
}

FLAG bloeff()
{
  /* Nimmt Computer den Bloeff an? */
  long zufalls_wert;

  zufalls_wert=kampf_gesamt[1]/6L;      /* Je nachdem wieviel Leute er hat */
  if (zufall(100)>zufalls_wert) {       /* Okay er ist der Stärkere */
    alert(romstr561);
    return(TRUE);                           /* Computer geht drauf ein */
    }
  else {
    alert(romstr562);
    return(FALSE);
    }
}

void move_einheiten()
{
  /* Die Einheiten ziehen und neue Fronten berechnen, sowie festellen, wer
     im Kampf steht */
  register int i;
  FLAG change;              /* ob am Bildschirm was geändert wurde */
	void *oldlogbase;
	
  for(i=0;i<SPR_MAX;i++) {
    if (sx[i]>=0) {                 /* Nur wenn Einheit noch da */
      switch (auftrag[i]) {         /* Je nach Auftrag */
        case ANGRIFF:
          if (i<EINHEITEN) {        /* Einheit des Spielers? */
            if (sx[i]<front_c) {
              sx[i]+=geschw[i];       /* Einheit zieht */
              if (sn[i]>=21)            /* vorher Rückzugsprite? */
                sn[i]-=14;
              }
            }
          else {                        /* Computereinheit */
            if (sx[i]>front_s) {
              sx[i]-=geschw[i-EINHEITEN-1];
              if (sn[i]<21)             /* Vorher Rückzugsprite? */
                sn[i]+=14;              /* Jetzt wieder Angriff */
              }
            }
          break;
        case RUECKZUG:
          if (i<EINHEITEN) {            /* Einheit des Spielers */
            if (sx[i]>deckung[i*2]) {        /* Noch nicht Zelt erreicht */
              sx[i]-=geschw[i];
              if (sn[i]<21)                /* vorher Angriffsprite? */
                sn[i]+=14;                  /* jetzt Rückzug */
              }
            else auftrag[i]=HALTEN;
            }
          else {                             /* Computer geht zurück */
            if (sx[i]<deckung[i*2]) {
              sx[i]+=geschw[i-EINHEITEN-1];
              if (sn[i]>=21)                /* Vorher Angriff? */
                sn[i]-=14;                  /* jetzt Rückzug */
              }
            else auftrag[i]=HALTEN;
            }
          break;
        case HALTEN: break;
        }
      }
    }

  /* Jetzt die Position der Fronten berechnen: */
  front_s=0;
  front_c=320;
  for(i=0;i<EINHEITEN;i++) {
    if (sx[i]>=0) {                     /* Nur wenn Einheit noch lebt */
      if (sx[i]>front_s) front_s=sx[i];
      }
    if (sx[i+EINHEITEN+1]>=0) {
      if (sx[i+EINHEITEN+1]<front_c) front_c=sx[i+EINHEITEN+1];
      }
    }

	oldlogbase=logbase;
	logbase=scr2;												/* Auf scr2 printen */
  change=FALSE;

  /* Jetzt berechnen, welche Einheit im Kampf steht */
  for(i=0;i<EINHEITEN;i++) {
    if (sx[i]>=0) {                 /* Lebt Einheit noch ? */
      if (sx[i]>=front_c) {         /* Steht Einheit im Feindesland? */
        if (!im_kampf[0][i]) {    /* Wenn noch nicht im Kampf, dann..*/
          im_kampf[0][i]=TRUE;    /* Einheit kämpft jetzt */
          writexy(1,23,88+i*6,armeename[i]);
          change=TRUE;
          }
        }
      else {                      /* Steht hinter der Front */
        if (im_kampf[0][i]) {     /* Wenn bisher im Kampf, dann.. */
          im_kampf[0][i]=FALSE;
          writexy(2,23,88+i*6,armeename[i]);
          change=TRUE;
          }
        }
      }
    if (sx[i+EINHEITEN+1]>=0) {         /* Computereinheit */
      if (sx[i+EINHEITEN+1]<=front_s) {         /* Steht Einheit im Feindesland? */
        if (!im_kampf[1][i]) {    /* Wenn noch nicht im Kampf, dann..*/
          im_kampf[1][i]=TRUE;    /* Einheit kämpft jetzt */
          writexy(1,208,88+i*6,armeename[i]);
          change=TRUE;
          }
        }
      else {                      /* Steht hinter der Front */
        if (im_kampf[1][i]) {     /* Wenn bisher im Kampf, dann.. */
          im_kampf[1][i]=FALSE;
          writexy(2,208,88+i*6,armeename[i]);
          change=TRUE;
          }
        }
      }
    }

  /* Wenn Ari nicht in direkten Kampf verwickelt, dann Anzeige je nach Auftrag */
  if (!im_kampf[0][2])          /* Ari-Kampfanzeige nur, wenn sie nicht im
                                direkten Kampf stehen */
    if (sx[2]>=0)
      switch (auftrag[2]) {
        case ANGRIFF:
          if (!ari_kaempft[0]) {        /* Schießt Ari schon? */
            writexy(1,23,88+2*6,armeename[2]);       /* Ari schießt jetzt */
            ari_kaempft[0]=TRUE;
            change=TRUE;
            }
          break;
        case RUECKZUG:
        case HALTEN:
          if (ari_kaempft[0]) {         /* Nur wenn Ari noch schießt */
            writexy(2,23,88+2*6,armeename[2]);       /* Ari schießt nicht mehr */
            ari_kaempft[0]=FALSE;
            change=TRUE;
            }
          break;
        }

  if (!im_kampf[1][2])          /* Ari-Kampfanzeige nur, wenn sie nicht im
                                direkten Kampf stehen */
    if (sx[10]>=0)
      switch (auftrag[10]) {
        case ANGRIFF:
          if (!ari_kaempft[1]) {        /* Schießt Ari schon? */
            writexy(1,208,88+2*6,armeename[2]);       /* Ari schießt jetzt */
            ari_kaempft[1]=TRUE;
            change=TRUE;
            }
          break;
        case RUECKZUG:
        case HALTEN:
          if (ari_kaempft[1]) {         /* Nur wenn Ari noch schießt */
            writexy(2,208,88+2*6,armeename[2]);       /* Ari schießt nicht mehr */
            ari_kaempft[1]=FALSE;
            change=TRUE;
            }
          break;
        }

  if (change) {
    Hm();
    cpy_raster(scr2,scr1,0,72,319,131,0,72);
    Sm();
    }

	logbase=oldlogbase;
}

void move_schild()
{
  /* Bewegt das Schild mit der jeweiligen Einheit mit */

  if (sx[schild_einheit]>=0) {                      /* Lebt Einheit noch? */
    sn[(EINHEITEN+1)*2]=43+schild_einheit*2;             /* Schild links */
    sn[(EINHEITEN+1)*2+1]=44+schild_einheit*2;           /* Schild rechts */
    sn[(EINHEITEN+1)*2+2]=57;                          /* Pfeil runter */
    sx[(EINHEITEN+1)*2]=sx[schild_einheit]-4;             /* Schild links */
    sx[(EINHEITEN+1)*2+1]=sx[schild_einheit]+12;           /* Schild rechts */
    sx[(EINHEITEN+1)*2+2]=sx[schild_einheit]-4;               /* Pfeil runter */
    sy[(EINHEITEN+1)*2]=sy[schild_einheit]-16;             /* Schild links */
    sy[(EINHEITEN+1)*2+1]=sy[schild_einheit]-16;           /* Schild rechts */
    sy[(EINHEITEN+1)*2+2]=sy[schild_einheit]-8;               /* Pfeil runter */
    if (sy[(EINHEITEN+1)*2]<0) {                  /* Paßt nicht auf Bildschirm */
      sy[(EINHEITEN+1)*2]=sy[schild_einheit]+16;             /* Schild links */
      sy[(EINHEITEN+1)*2+1]=sy[schild_einheit]+16;           /* Schild rechts */
      sy[(EINHEITEN+1)*2+2]=sy[schild_einheit]+8;               /* Pfeil runter */
      sn[(EINHEITEN+1)*2+2]=58;                             /* Pfeil rauf */
      }
    }
  else {
    sx[(EINHEITEN+1)*2]=-1;                 /* Sprite weg */
    sx[(EINHEITEN+1)*2+1]=-1;               /* Sprite weg */
    sx[(EINHEITEN+1)*2+2]=-1;               /* Sprite weg */
    }
}

void check_end(gegnern)
char gegnern[];                     /* Name des Gegners */
{
  /* Überprüft, ob der Krieg von einer Partei verloren wurde */

  if (kampf_gesamt[0]==0.0) {
    alert(romstr563);
    gewinner=1;
    button=EXIT_BTN;
    }
  if (kampf_gesamt[1]==0.0) {           /* Gegner hat keine Männer mehr */
    if (sx[EINHEITEN*2+1]<0)            /* Gegner hat kein Zelt, also Cityattack */
      alert(build(romstr564,gegnern));
    else alert(romstr565);
    gewinner=0;
    button=EXIT_BTN;
    }
}

void computer_move()
{
  /* Computer berechnet seinen Zug und setzt seine Armeen ein: */
  int company;
  int langsamx;
  int offset;

  /* Zuerst die langsamste Einheit ermitteln : */
  langsamx=0;
  for(company=0;company<EINHEITEN;company++) {
    offset=company+EINHEITEN+1;
    if (sx[offset]>=0)                         /* Lebt Einheit noch? */
      if (company!=2)                       /* Keine Ari's */
        if (auftrag[offset]==ANGRIFF)
          if (sx[offset]>langsamx) {
            langsamx=sx[offset];
            }
    }

  for(company=0;company<EINHEITEN;company++) {

    /* Nur dann einsetzen, wenn Einheit noch lebt: */

    offset=company+EINHEITEN+1;
    if (sx[offset]>=0)
      switch (auftrag[offset]) {
        case ANGRIFF:
          if (front_c<langsamx-10)          /* Nur dann ggf. umdrehen, wenn gestreckte Front ist */
            if (front_s>=sx[offset]-8) {    /* schnellere Einheit zu dicht dran? */
              auftrag[offset]=RUECKZUG;     /* auf die anderen warten, zurück */
              }
          break;
        case RUECKZUG:
        case HALTEN:
          if (sx[offset]>=langsamx) {      /* auf Höhe der Langsamsten? */
            auftrag[offset]=ANGRIFF;     /* wieder vorstürmen */
            }
          break;
        }
    }
}

void war()
{
  /* Krieg mit dem Gegner simulieren */
  int i;
  long beteiligt[2];                    /* Soldaten, auf dem Schlachtfeld */
  float verlust[2];                     /* Verluste der Parteien */
  float kraftsumme[2];                  /* Summe aller Kampfkräfte */
  float mult[2];                        /* Multiplikator für 1 */
  float dead;                            /* Tote Soldaten */
  long summes,summec;

  shoot_ari();                          /* Artilleriekampf */

  beteiligt[0]=beteiligt[1]=0;
  summes=summec=0;

  for(i=0;i<EINHEITEN;i++) {
    if (im_kampf[0][i]) beteiligt[0]+=kampf_teil[0][i];
    if (im_kampf[1][i]) beteiligt[1]+=kampf_teil[1][i];
    summes+=kampf_teil[0][i];
    summec+=kampf_teil[1][i];
    }

  if (summec==0 || summes==0) return;               /* Einer hat verloren */

  if (beteiligt[0]==0 || beteiligt[1]==0) return;   /* Keiner beteiligt->Raus */

  /* Beide Parteien haben Armeen auf dem Schlachtfeld -> Verluste auf
     beiden Seiten, abhängig von Anzahl der Soldaten, Kampfkraft */

  /* Verluste auf beiden Seiten berechnen: */
  verlust[0]=verlust[1]=0.0;
  kraftsumme[0]=kraftsumme[1]=0.0;

  for(i=0;i<EINHEITEN;i++) {
    if (im_kampf[0][i]) {               /* Einheit des Spielers aktiv? */
      verlust[1]+=kampf_teil[0][i]*kampfkraft[0][i]*(0.001*effektivitaet);
                        /* Ja -> Verluste des Gegners erhöhen sich */
      kraftsumme[0]+=1.0/kampfkraft[0][i];
      }
    if (im_kampf[1][i]) {               /* gegner. Einheit aktiv? */
      verlust[0]+=kampf_teil[1][i]*kampfkraft[1][i]*(0.001*effektivitaet);
      kraftsumme[1]+=1.0/kampfkraft[1][i];
      }
    }

  mult[0]=1.0/kraftsumme[0];          /* Multiplikator berechnen */
  mult[1]=1.0/kraftsumme[1];

  /* Verluste verteilen: */
  for(i=0;i<EINHEITEN;i++) {
    if (im_kampf[0][i]) {
      dead=verlust[0]*mult[0]*1.0/kampfkraft[0][i];
      kampf_teil[0][i]-=dead;
      if (kampf_teil[0][i]<1) {             /* Lebt Einheit noch? */
        kampf_teil[0][i]=0.0;               /* Nein, alle tot! */
        im_kampf[0][i]=FALSE;               /* nicht mehr im kampf */
        sx[i]=-1;                           /* Sprite weg */
        }
      }
    if (im_kampf[1][i]) {
      dead=verlust[1]*mult[1]*1.0/kampfkraft[1][i];
      kampf_teil[1][i]-=dead;
      if (kampf_teil[1][i]<1) {
        kampf_teil[1][i]=0.0;
        im_kampf[1][i]=FALSE;
        sx[EINHEITEN+1+i]=-1;               /* Sprite weg */
        }
      }
    }
}

void shoot_ari()
{
  /* Verluste auf beiden Seiten berechnen, wenn die Artillerie feuert */
  float verluste[2];
  long beteiligt[2];            /* Wieviel Soldaten stehen im kampf */
  int anzahl[2];                /* Wieviel Einheiten stehen im Kampf? */
  int i;

  /* Keine Artillerie schießt, raus */
  if (auftrag[2]!=ANGRIFF && auftrag[10]!=ANGRIFF) return;

  beteiligt[0]=beteiligt[1]=0;
  anzahl[0]=anzahl[1]=0;

  for(i=0;i<EINHEITEN;i++)
    if (i!=5) {                         /* Wizards werden nicht getroffen */
      if (kampf_teil[0][i]>0.0) {
        beteiligt[0]+=kampf_teil[0][i];
        anzahl[0]++;
        }
      if (kampf_teil[1][i]>0.0) {
        beteiligt[1]+=kampf_teil[1][i];
        anzahl[1]++;
        }
      }

  verluste[1]=kampf_teil[0][2]*effektivitaet/200.0;  /* 200 Aris machen einen Tot */
  verluste[0]=kampf_teil[1][2]*effektivitaet/200.0;

  /* Ari's kämpfen nur, wenn sie nicht direkt in Kampf verwickelt sind */
  for(i=0;i<EINHEITEN;i++)
    if (i!=5) {                                 /* Wizards werden nicht getroffen */
      if (!im_kampf[1][2] && auftrag[10]==ANGRIFF)     /* Wenn gegn. nicht im Kampf,.. */
        if (kampf_teil[0][i]>0.0) {
          kampf_teil[0][i]-=verluste[0]/anzahl[0];
          if (kampf_teil[0][i]<1.0) {           /* Lebt Einheit noch? */
            kampf_teil[0][i]=0.0;               /* Nein, alle tot! */
            im_kampf[0][i]=FALSE;               /* nicht mehr im kampf */
            sx[i]=-1;                           /* Sprite weg */
            auftrag[i]=HALTEN;
            }
          }
      if (!im_kampf[0][2] && auftrag[2]==ANGRIFF)
        if (kampf_teil[1][i]>0.0) {
          kampf_teil[1][i]-=verluste[1]/anzahl[1];
          if (kampf_teil[1][i]<1.0) {       /* Lebt Einheit noch ? */
            kampf_teil[1][i]=0.0;
            im_kampf[1][i]=FALSE;
            sx[EINHEITEN+1+i]=-1;               /* Sprite weg */
            auftrag[EINHEITEN+1+i]=HALTEN;
            }
          }
      }
}

void attack(company)
int company;
{
  /* Spieler will mit Company angreifen */

  if (sx[company]<0) return;
        /* Einheit ist tot */

  if (effects) play_digi(pack_buf+32000L,FALSE,15,16);
  auftrag[company]=ANGRIFF;
}

void flee(company)
int company;
{
  /* Spieler zieht company zurück */

  if (effects) play_digi(pack_buf+32000L,FALSE,17,19);
  auftrag[company]=RUECKZUG;
}

void calc_staerke(armee_nr)
int armee_nr;
{
  /* Berechnet die Staerke der Armee romstr566 der einzelnen Einheiten,
     je nach Untergrund, Motivation etc. */
  int nr;

  if (armee_nr==0) nr=0;
  else nr=1;

  calc_it(nr,ground_num,training[armee_nr],motivation[armee_nr]);
}

void calc_it(int nr,int untergrund,float training[][EIGENSCHAFTEN],float motivation)
{
  /* Berechnet aus diesen Faktoren die Kampfkraft, und schreibt sie
     in kampf_kraft[nr] */
  int i,j,index;
  int wert;
  float summe;

  switch(untergrund) {       /* Kampfuntergrund */
    case EISEN:
    case DIAMANTEN:
    case NITHRIL:
    case GOLD:
		case OEL:
		case GAS:
    case GIMMIG:
    case SCHATZ:
    case EBENE:
		case HALBWUESTE:
		case WUESTE:
    case BRUECKE:
        index=0; break;         /* Index für kraft_boden bestimmen */
    case WALD:
    case LICHTER_WALD:
        index=1; break;
    case DRY_WUESTE:
        index=2; break;
		case RUINE:
    case HUEGEL:
		case WADI:
		case SLUMS:
        index=3; break;
    case SUMPF:
        index=4; break;
    case STADT_1_EIN:
    case STADT_2_EIN:
    case STADT_3_EIN:
    case STADT_4_EIN:
        index=5; break;
    case BUNKER_1:
    case BUNKER_2:
        index=6; break;
    case WASSER:
        index=7; break;
    default: 
			if (my_system) internal(romstr567);
			else index=0;							/* Sonst Ebene */
    }

  for(i=0;i<EINHEITEN;i++) {
    kampfkraft[nr][i]=(float)kraft_boden[i][index]/10.0;
    summe=0.0;
    for(j=0;j<EIGENSCHAFTEN;j++) {
      wert=training[i][j]*wirkung[i][j];        /* 0..500 */
      summe+=(float)wert/125.0;                 /* 0..4 */
      }
    summe/=EIGENSCHAFTEN;           /* Durchschnitt bilden 0..4 */
    kampfkraft[nr][i]*=summe;
    kampfkraft[nr][i]*=motivation/500.0+0.9;      /* 0.9 .. 1.1 */
    }
}

void show_soldiers()
{
  /* Zeigt auf dem Schlachtfeld an, wieviel Soldaten Spieler und Gegner haben. */
  register int company;
  long summes,summeg;
	void *oldlogbase;
	
	oldlogbase=logbase;
	logbase=scr2;

  if (effekt2) {
    effekt2=FALSE;
    writexy(0,140,74,fstr(romstr568,effektivitaet));
    }
  if (new_effekt) {
    new_effekt=FALSE;
    effekt2=TRUE;
    writexy(0,140,74,fstr(romstr569,effektivitaet));
    }

  summes=summeg=0;
  for(company=0;company<EINHEITEN;company++) {
    summes+=(long)kampf_teil[0][company];
    summeg+=(long)kampf_teil[1][company];
    writexy(0,75,88+company*6,str(8,(long)kampf_teil[0][company]));
    if (show_gegner) writexy(0,260,88+company*6,str(8,(long)kampf_teil[1][company]));
    }

  writexy(0,75,82,str(9,summes));
  if (show_gegner) writexy(0,260,82,str(9,summeg));

	logbase=oldlogbase;
	
  kampf_gesamt[0]=summes;
  kampf_gesamt[1]=summeg;
}

void verlier_schiff(nr)
int nr;
{
  /* Spieler verliert Schiff in flotte[i] */
  int i;

  ship_own[flotte[nr].typ]--;               /* Ein Schiff dieses Typs weniger */
  gesamt_gehalt-=flotte[nr].salary;
  for(i=nr+1;i<SCHIFFMAX;i++)
    flotte[i-1]=flotte[i];

  flotte[SCHIFFMAX-1].typ=-1;             /* hinterster Eintrag frei */

  schiffbesitz--;
  writexy_anzeige();                          /* Neue Werte eintragen */
}

