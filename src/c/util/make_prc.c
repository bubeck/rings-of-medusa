
		/* Preisberechnung fÅr Rings of Medusa II 
			 Ω by Till Bubeck (3.6.1991)	*/
			 
#include <stdio.h>
#include <tos.h>
#include <vdi.h>
#include	<aes.h>
#include <geminit.h>
#include <ctype.h>
#include	<stdlib.h>
#include	<setjmp.h>

#include <math.h>
#include "o:\filename.c"
#include "o:\const.c"              /* Alle Konstanten holen */
#include "o:\grund.c"              /* Konstantendefintion der UntergrÅnde */
#include "o:\typedef.c"            /* Typendefinitionen */
#include "o:\global.c"             /* Variablendeklarationen */

typedef struct {
  int nr;
  int x,y;
  int groesse;
  int indu;
  } STADT;

FLAG wait=FALSE;
STADT stadt[CITIES];
int preise[CITIES][WAREN+STALLWAREN+ROHSTOFFE];

char gmap[18][1800];            /* nimmt die GMAP's auf */

int prod_preis[] = {
  3,4,3,3,6,12,97,80,106,56,120,10000,			/* bis Kristall */
	200,5000,500,1000,2500,3500,900,20,			/* bis Chips */
  350,400,																/* Kerosin, Gleiter */
  10000,55,65,89,105,114,20000,130 };					/* Bis Gold */

int prod[CITIES][5] = {     
	1,30,0,0,0,											/* A0 Milch */
	2,3,12,0,0,											/* B0 Crack, Acid, Kristall */
	17,18,0,0,0,										/* B1 Cyborg, Androiden */
	10,0,0,0,0,											/* B2 Arznei */
	6,0,0,0,0,											/* C0 Fische */
	14,0,0,0,0,											/* E0 Fîrderturm */
	19,0,0,0,0,											/* F0 Computer */
	20,0,0,0,0,											/* F1 Chips */
	7,0,0,0,0,											/* F2 Waffen */
	1,0,0,0,0,											/* F3 Milch */
	13,0,0,0,0,											/* G0 Sklaven */
	2,3,0,0,0,											/* H0 KÑse, Salat */
	4,0,0,0,0,											/* H1 épfel */
	6,22,23,0,0,										/* I0 Fische */
	19,0,0,0,0,											/* I1 Computer */
	7,0,0,0,0,											/* I2 Waffen */
	15,0,0,0,0,											/* J0 Filterstufe */
	11,0,0,0,0,											/* K0 Cyclotron */
	16,0,0,0,0,											/* L0 ôltank */
	5,21,24,0,0,										/* M0 Fleisch */
	6,26,25,0,0,										/* M1 Fisch */
	5,0,0,0,0,											/* N0 Fleisch */
	8,27,28,0,0,										/* O0 Crack */
	9,22,23,0,0,										/* P0 Acid 54 */
	10,24,25,0,0,										/* P1 Arznei */
	2,3,0,0,0,											/* P2 KÑse, Salat */
	18,0,0,0,0,											/* Q0 Androiden */
	11,28,29,0,0 };									/* R0 Cyclotron */

/* Prototypen: */
void calculate(void);
void load_gmaps(void);
void trans_gmap(int quelle,int ziel);
void get_koord(void);
int such_land(int stadt,int *nr);
void load_prc(void *fn,void *adr);
int get_ground(int land,int x,int y);
int get_groundxy(int land,int x,int y);
void make_world(void);
int such_insel(void);
short get_land(int x,int y);
int zufall(int grenze);

void main()
{
  /* Berechnet die Preise in den einzelnen StÑdten, je nach Entfernung und
     Produkt, das die StÑdte produzieren */
  int fh;

  load_gmaps();
  make_world();
  get_koord();
  calculate();
  fh=Fcreate("J:\\CITIES.WAR",0);
  if (fh>0) {
    Fwrite(fh,sizeof(preise),preise);
    Fclose(fh);
    }
}

void calculate()
{
  /* Berechnet die Preise */
  int std;
  int waren;
  int i;
  long abstand,abstand_min,dx,dy;
  int std2,city_n,proz;
  long teuerung;
  long preis;

  for(std=0;std<CITIES;std++) {
    printf("Berechne fÅr Stadt %d\n",std);
    for(waren=1;waren<=WAREN+STALLWAREN+ROHSTOFFE;waren++) {
      if (wait) 
				if (waren<23)
        	printf("%s",waren_name[waren-1]);
	      else
  	      printf("%s",metal_name[waren-23]); 
    		
			preis=0;
      if (waren==21 || waren==22) {					/* Stallwaren haben festen Preis */
				preis=prod_preis[waren-1];
				goto raus;
				}

      /* PrÅfen, ob selber produziert wird */
      for(i=0;i<5;i++)
        if (prod[std][i]==waren) break;

      if (i!=5) {                           /* Wird selber produziert */
        preis=prod_preis[waren-1];
        }
      else {                        /* Wird hier nicht produziert */

        /* nÑchste Stadt suchen, in der Ware produziert wird */

        abstand_min=20000;                          /* max. */

        for(std2=0;std2<CITIES;std2++)
          for(i=0;i<5;i++) {
            if (prod[std2][i]==waren) {         /* Stadt produziert es */
              dx=stadt[std2].x-stadt[std].x;
              dy=stadt[std2].y-stadt[std].y;
              abstand=(int)sqrt((double)(dx*dx)+(double)(dy*dy));
              if (abstand<abstand_min) {
                abstand_min=abstand;
                city_n=std2;
                }
              }
            }


        if (abstand_min==20000) {
          printf("Fehler! Ware %d wird nirgends produziert!\n",waren);
          }

        if (wait) printf("    Abstand_min: %ld",abstand_min); 
        proz=abstand_min/10;
        if (prod_preis[waren-1]>100) proz=abstand_min/25;
        else if (prod_preis[waren-1]>20) proz=abstand_min/18;

        teuerung=(long)prod_preis[waren-1]*(long)proz/100L;
        preis=prod_preis[waren-1]+teuerung;
        }
raus:
      if (wait) printf("    Preis: %ld\n",preis); 

      preise[std][waren-1]=preis;
      }
    if (wait) Crawcin();
    else {
      if (Cconis()) {
        wait=TRUE;
        Crawcin();
        Crawcin();
        }
      }
    }
}

void load_gmaps()
{
  /* LÑdt alle GMAP's */

  load_prc("D:\\MEDUSA\\MEDUSA_2.COL\\MAPS\\GMAPS\\GMAPS.DAT",gmap);
	trans_gmap(13,16);
	trans_gmap(6,17);
}

void trans_gmap(quelle,ziel)
int quelle,ziel;
{
	/* Transportiert Gmap quelle nach Ziel */
	register int i;
		
	for(i=0;i<1800;i++) gmap[ziel][i]=gmap[quelle][i];
}

void get_koord()
{
  /* Holt sich die Koordinaten der StÑdte */
  int land;
  int x,y;
  int nr;
  int pos;
  int stadt_nr;
  int x1,y1,wert;

  for(stadt_nr=0;stadt_nr<CITIES;stadt_nr++) {
    printf("Suche Stadt %d    ",stadt_nr);
    land=such_land(stadt_nr,&nr);
    for(pos=0;welt[pos]!=land;pos++) ;
    x=(pos%15)*320;
    y=(pos/15)*150;
    for(x1=0;x1<80;x1++) {
      for(y1=0;y1<36;y1++) {
        wert=get_ground(land,x1,y1);
        if (wert==STADT_1_EIN+nr) goto raus;
        }
      }
    printf("Fehler!, Stadt nicht gefunden %d\n",stadt_nr);
    Crawcin();

raus:

    x+=x1*4;
    y+=y1*4;

    stadt[stadt_nr].x=x;
    stadt[stadt_nr].y=y;
    printf("Koordinaten: %d/%d\n",x,y);
    }
}

int such_land(stadt,nr)
int stadt,*nr;
{
  int land;
  int i;

  for(land=0;land<18;land++) {
    for(i=0;i<4;i++) {
      if (city_nr[land][i]==stadt) {
        *nr=i;
        return(land);
        }
      }
    }
  printf("Finde Land nicht!\n");
}

void load_prc(name,adr)
void *name,*adr;
{
  int fh;

  fh=Fopen(name,0);
  if (fh>=0) {
    Fread(fh,40000L,adr);
    Fclose(fh);
    }
	else printf("Fehler! Datei %s kann nicht geîffnet werden!",name);
}

int get_ground(land,x,y)
int land,x,y;
{
  /* holt sich aus GMAP den WEG, X/Y bereits in Rasterangaben */
  /* ist X/Y auûerhalb, gibt Funktion -1 zurÅck */

  return(get_groundxy(land,x*4,y*4+25));
}

int get_groundxy(land,x,y)
int land,x,y;
{
  /* Holt den Weg an der Stelle X/Y, Dabei sind die Werte 5Bit breit
     und gepackt */

  int zeile;
  int bit,byte;
  char op1,op2;
  int anzahl;
  int wert;

  if (x<0 || x>319 || y<21 || y>168) {
    printf("Fehler!\n");
    Crawcin();
    }

  zeile=(y-25)/4;
  bit=x/4*5;
  byte=bit/8;
  bit%=8;
  bit=7-bit;                                    /* von oben herunter */
  op1=gmap[land][zeile*50+byte];                /* erstes Byte */
  op2=gmap[land][zeile*50+byte+1];              /* nachfolgendes Byte */
  anzahl=bit-4;                                 /* um soviel wird geschoben */
  if (anzahl>=0) {                              /* ist voll in op1 */
    wert=op1>>anzahl;                           /* hier verwendet LASER_C "ASR"! */
    wert&=31;                                   /* nur 5 Bits gÅltig */
    }
  else {
    wert=op1<<(-anzahl);                        /* aus op1 holen */
    wert&=31;                                   /* nur 5 Bits gÅltig */
    op2>>=8+anzahl;                             /* Achtung, anzahl ist negativ! */
                /* ^^^ LASER_C verwendet ASR, d.h. obere Bits lîschen */
    op2&=(1<<(-anzahl))-1;
    wert|=op2;
    }

  return((int)wert);
}

void make_world()
{
  /* Erstellt die Welt in welt[] */
  int i;
	int pos;

  for(i=0;i<225;i++) welt[i]=SEA;           /* Am Anfang war Åberall Wasser... */

	welt[95]=11;
  welt[96]=10;
  welt[97]=9;
  welt[110]=7;
  welt[111]=4;
  welt[112]=3;
  welt[124]=6;              /* Hauptkontinent */
  welt[125]=5;
  welt[126]=0;
  welt[127]=1;
  welt[128]=2;
	welt[141]=13;

  welt[such_insel()]=8;
	welt[such_insel()]=12;

  pos=such_insel();           /* Inseln plazieren */
	welt[pos]=14;
	welt[pos+15]=16;					/* = 13,N */

  pos=such_insel();
	welt[pos++]=17;						/* = 6,G */
	welt[pos]=15;
}

int such_insel()
{
  /* Sucht einen freien Platz in welt[] */
  int x,y;
  FLAG belegt;
  register int x2,y2;

  do {
    belegt=FALSE;
    x=zufall(15);
    y=zufall(15);
    for(x2=x-3;x2<=x+3;x2++)
      for(y2=y-3;y2<=y+3;y2++)
        if (get_land(x2,y2)!=SEA) {
          belegt=TRUE;
          break;
          }
    } while(belegt);

	return(x+y*15);
}

short get_land(x,y)
int x,y;
{
  /* Holt aus welt[] die Landschaft unter BerÅcksichtigung der Kugerlform */

  if (x<0) x+=15;
  if (x>=15) x-=15;
  if (y<0) y+=15;
  if (y>=15) y-=15;

  return(welt[x+y*15]);
}

int zufall(grenze)
int grenze;
{
  /* Zwischen 0..(grenze-1) */
  float x;
  int zahl;

  x=(float)Random()/16777215.0;       /* zwischen 0..1 */
  zahl=(int)(x*grenze);

  return(zahl);
}




