
                  /* Im Zeichen der Medusa (Medusa II) */
     /* Code von Till Bubeck, Ziegeleistr. 28, 7056 Weinstadt */
                           /* 07151-66437 */

                        /* All rights reserved */


    /* File zum Erstellen des Bunker-Datenfiles aus einem NEOchrome Bild */

#include <stdio.h>
#include <stdlib.h>
#include <linea.h>
#include <tos.h>
#include <vdi.h>
#include <aes.h>
#include <geminit.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

#define FLAG char
#define FALSE 0
#define TRUE -1

typedef struct {
  FLAG key,door;
  } TUERE;

char path[64]="D:\\MEDUSA\\MEDUSA_2.COL\\BUNKER\\MAPS\\BUNKER*.NEO",file[64];
char path_file[64];
char str[100];
int tos_pal[16];                /* Alte Palette */
int tos_rez;                    /* Alte Auflösung */

int lkeys;                      /* Anzahl der lokalen Schlüssel */
unsigned int bunker[20000];
int bun_len;

int tos2vdi[16] = {             /* Wandelt Registernummer in Vdi-Farbe */
  0,2,3,6,4,7,5,8,9,10,11,14,12,15,13,1 };
int vdi2tos[16] = {             /* Wandelt VDI-Farbe in Registernummer */
  0,15,1,2,4,6,3,5,7,8,9,10,12,14,11,13 };

TUERE door[256];                /* Alle lokalen Türen */

/* Prototypen: */
  
FLAG auswahl(char filename[]);
void Hm(void);
void Sm(void);
long load(char a[],void *b,long c,long d);
void get_bunker(void);
void get_wert(int x,int y,unsigned int *farbe,unsigned int *wert);
void put_farbe(int farbe,int x,int y);
void line(int color,int x1,int y1,int x2,int y2);
void error(char text[],int x,int y);
void programmende(void);

void main()
{
  int i;
  int filehandle;
  int palette[16];

  open_work();
	linea_init();
	
  for(i=0;i<16;i++) tos_pal[i]=Setcolor(i,-1);      /* Farbe holen */
  tos_rez=Getrez();

  while (auswahl(path_file)) {
    Hm();
    Setscreen(-1L,-1L,0);                           /* Low-Rez */
    load(path_file,palette,4L,32L);
    load(path_file,Logbase(),128L,32000L);
    Setpalette(palette);

    get_bunker();

    Cconout(27);
    Cconout('H');
    Cconws("SAVING...\r\n");

    i=strlen(path_file);
    path_file[--i]='T';
    path_file[--i]='A';
    path_file[--i]='D';
    filehandle = Fcreate(path_file,0);
    Fwrite(filehandle,(long)bun_len*2,bunker);
    Fclose(filehandle);

    Setscreen(-1L,-1L,tos_rez);
    Setpalette(tos_pal);
    Sm();
    }
  programmende();
}

long load(a,b,c,d)
char a[];
void *b;
long c;
long d;
{
  int fh;
  long max;

  fh = Fopen(a,0);

  Fseek(c,fh,0);
  max=Fread(fh,d,b);

  Fclose(fh);
  return(max);
}

void Hm()
{
  v_hide_c(handle);
}

void Sm()
{
  v_show_c(handle,1);
}
 
FLAG auswahl(filename)
char filename[];
{
  int j,i,ok;

  fsel_input(path,file,&ok);
  if (!ok) return (FALSE);

  strcpy(filename,path);
  i=strlen(filename);
  while (filename[--i]!='\\') ;
  for (i++,j=0;j<=strlen(file);j++,i++)
    filename[i]=file[j];
  return(TRUE);
}

void get_bunker()
{
  int x,y;
  unsigned int farbe,wert;
  FLAG zeile_da;
  int xmax,ymax;                    /* Breite/Höhe Bunker */
  int i;

  for(i=0;i<256;i++) door[i].key=door[i].door=FALSE;    /* Nicht vorhanden */

  /* Pass 1: */

  xmax=0;
  ymax=0;
  for(x=0;x<80;x++) {
    zeile_da=FALSE;                     /* Noch nichts gefunden.. */
    for(y=0;y<50;y++) {
      get_wert(x,y,&farbe,&wert);
      if (farbe!=0) {
        zeile_da=TRUE;                  /* Gefunden ... */
        if (x>xmax) xmax=x;
        if (y>ymax) ymax=y;
        }
      put_farbe(14,x,y);
      }
    if (!zeile_da) break;               /* Leere Zeile->Ende */
    }


  /* Pass 2: */
  load(path_file,Logbase(),128L,32000L);        /* Bild neu laden */

  bun_len=0;
  bunker[bun_len++]=xmax;							/* Ausmasse eintragen */
  bunker[bun_len++]=ymax;

  for(x=0;x<=xmax;x++) {
    for(y=0;y<=ymax;y++) {
      get_wert(x,y,&farbe,&wert);
      put_farbe(14,x,y);
      switch (farbe) {
        case 0:                               /* Leeres Feld */
          bunker[bun_len++]=0;
          break;
        case 1:
					if (wert>=32768) {				/* geöffnete Türe */
						bunker[bun_len++]=36000+(wert-32768)-1;
						}
					else
	          bunker[bun_len++]=32768+wert-1;           /* geschlossene Türe */
          break;
				case 4:										
					bunker[bun_len++]=wert;				/* Schlüssel */
					break;
        case 5:
          bunker[bun_len++]=65534;                /* Startposition */
          break;
				case 6:
					bunker[bun_len++]=16384+wert;			/* Aufzug */
					break;
				case 9:
					bunker[bun_len++]=32768;							/* Dummy-Türe */
					break;
				case 11:
					bunker[bun_len++]=32768+16384+wert-1;		/* Gegner */
					break;
				case 12:											/* Item */
					bunker[bun_len++]=10000+wert-1;
					break;
        case 15:
					if (wert==63903) bunker[bun_len++]=65533;	/* Begehbare Wand */
          else bunker[bun_len++]=65535;                /* Wand */
          break;
        default:
          bunker[bun_len++]=0;
          break;
        }
      }
    }
}

void get_wert(rx,ry,farbe,wert)
int rx,ry;                  /* Rasterkoordinaten */
unsigned int *farbe,*wert;
{
  int start_x,end_x;
  int start_y,end_y;
  register int x,y;
  int fxy;
  int bitcounter;

  start_x=rx*4-2;
  start_y=ry*4-2;
  end_x=start_x+3;
  end_y=start_y+3;

  if (start_x<0) start_x=0;
  if (start_y<0) start_y=0;

  bitcounter=0;
  *farbe=0;
  *wert=0;
  for(y=start_y;y<=end_y;y++)
    for(x=start_x;x<=end_x;x++) {
			*wert=2*(*wert);
      fxy=get_pixel(x,y);
			if (fxy==10) fxy=0;							/* Grau ist unsichtbar */
     	if (fxy!=0) {
        (*wert)++;             /* Bit gesetzt! */
        if (*farbe==0) {
        	*farbe=fxy;
					}
         else
          if (*farbe!=fxy) {
            error("NICHT EINFARBIG",x,y);
            return;
            }
        }
      bitcounter++;
      }
  return;
}

void put_farbe(farbe,rx,ry)
int farbe;
int rx,ry;                  /* Rasterkoordinaten */
{
  int start_x,end_x;
  int start_y,end_y;

  start_x=rx*4-2;
  start_y=ry*4-2;
  end_x=start_x+3;
  end_y=start_y+3;

  if (start_x<0) start_x=0;
  if (start_y<0) start_y=0;

  line(farbe,start_x,start_y,start_x,start_y);
}

void line(color,x1,y1,x2,y2)
int color,x1,y1,x2,y2;
{
  /* Zeichnet eine Linie von x1/y1 nach x2/y2 in Farbe color */
  int pxy[4];

  pxy[0]=x1; pxy[1]=y1;
  pxy[2]=x2; pxy[3]=y2;
  vsl_color(handle,tos2vdi[color]);
  v_pline(handle,2,pxy);
}

void error(text,x,y)
char *text;
int x,y;
{
  Cconout(27);
  Cconout('H');

  printf("%s\nPosition: X:%3d Y:%3d\n",text,x,y);

  if ((int)Crawcin()==27) programmende();
}

void programmende()
{
  Setscreen(-1L,-1L,tos_rez);
  Setpalette(tos_pal);
  Sm();
  close_work();
  exit(0);
}
