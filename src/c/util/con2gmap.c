
#include <osbind.h>
#include <geminit.h>
#include "GRUND.C"

char path[100]="D:\\MEDUSA\\MEDUSA_2.COL\\MAPS\\GMAPS\\GMAP?.NEO",file[64];
char path_file[100];
char str[100];

#define FALSE 0
#define TRUE -1
#define M_OFF 256
#define M_ON 257

int bin_wert;

int muster[32][2] = {
  /* Farbe,Bin-Wert */
  2,0,              /* SCHATZ */
  2,4,              /* EISEN */
  2,5,              /* DIAMANTEN */
  2,6,              /* NITHRIL */
  2,3,              /* ™L */
  2,7,              /* GAS */
  12,2,             /* GIMMIG */
  2,2,              /* GOLD */
  8,0,              /* EBENE */
  8,4,              /* SUMPF */
  12,4,             /* RUINE */
  4,0,              /* WšSTE TROCKEN */
  4,2,              /* WšSTE */
  4,1,              /* HALBWšSTE */
  4,4,              /* BRšCKE */
  6,0,              /* WALD */
  6,2,              /* DICHTER WALD */
  1,4,              /* HšGEL */
  9,2,              /* WADI */
  12,1,             /* SLUMS */
  0,1,              /* EINGŽNGE */
  0,6,
  0,5,
  0,3,
  0,7,              /* BUNKEREINGANG 1 */
  0,4,              /* BUNKEREINGANG 2 */
  9,7,              /* SCHILD */
  1,0,              /* HINDERNIS */
  1,2,              /* VULKAN */
  10,1,             /* HAFENEINGŽNGE */
  10,6,
  9,0 };              /* WASSER */


main()
{
  char *picbuf;
  char *palbuf;

  char filename[13];
  int  code = 0;
  char env[80 * 38]; /* = 3040 */
  int  reg[9];
  int  pel[9];
  char drive;
  int  i;
  int  num = 0;
  int  mx,my,mk;
  int  filehandle;
  int  pxy[4];
  int grund;
  char comp[80*38];

  register int x,y;

  palbuf = (char *) Malloc (   32L);
  picbuf = (char *) Malloc (32000L);

  open_work();

  while (auswahl(path_file)) {
  num=0;
  Setscreen(-1L,-1L,0);
  Setcolor(0,0);
  Setcolor(15,0x777);

  load(path_file, palbuf,  4L,    32L);
  load(path_file, picbuf,128L, 32000L);

  Setpalette(palbuf);

  c_cls(handle);
  pic_move(handle, picbuf, 0, 0L, 200);
  v_hide_c(handle);
  vsl_color(handle,0);


  for (y = 25; y <= 165; y += 4)
  {
    for (x = 0; x<320; x += 4)
    {
      pxy[0] = x;
      pxy[1] = y;
      pxy[2] = x+3;
      pxy[3] = y;

      v_pline(handle, 2, pxy);

      v_get_pixel(handle, x + 1, y + 1, &pel[0], &reg[0]);
      v_get_pixel(handle, x + 2, y + 1, &pel[1], &reg[1]);
      v_get_pixel(handle, x + 3, y + 1, &pel[2], &reg[2]);
      v_get_pixel(handle, x + 1, y + 2, &pel[3], &reg[3]);
      v_get_pixel(handle, x + 2, y + 2, &pel[4], &reg[4]);
      v_get_pixel(handle, x + 3, y + 2, &pel[5], &reg[5]);
      v_get_pixel(handle, x + 1, y + 3, &pel[6], &reg[6]);
      v_get_pixel(handle, x + 2, y + 3, &pel[7], &reg[7]);
      v_get_pixel(handle, x + 3, y + 3, &pel[8], &reg[8]);

      if (pel[6]==15) bin_wert=4;
      else bin_wert=0;
      if (pel[7]==15) bin_wert+=2;
      if (pel[8]==15) bin_wert++;

      for(i=0;i<32;i++)
        if (muster[i][0]==pel[0] && muster[i][1]==bin_wert) {
          env[num++]=(char)i;
          break;
          }

      if(i==32) {
        c_home();
        sprintf(str,"Unbekannte Farb/Musterkombination!\n\r X:%d  Y:%d\n\r",x+1,y+1);
        druck(str);
        }

      for(i=1;i<9;i++)
        if (pel[i]!=15)         /* šberprfen, ob ganzer Kasten richtig */
          if (pel[i]!=pel[0]) {
            c_home();
            sprintf(str,"Eck- und Kastenfarbe verschieden!\n\r X:%d  Y:%d\n\r",x+1,y+1);
            druck(str);
            break;
            }

      }
  }

  c_home();
  Cconws("Converting...\r\n");
  for(i=0;i<1900;i++) comp[i]=0;

  asm {
                    movem.l d0-d7/a0-a6,-(a7)

                    lea     env(a6),a0
                    lea     comp(a6),a1
                    moveq   #0,d7           ;Counter
                    moveq   #7,d6           ;Bitposition an der gestartet wird

loop:               moveq   #0,d0
                    move.b  (a0)+,d0        ;Byte holen
                    move.w  d6,d5           ;kopieren der Bitposition
                    sub.w   #4,d5           ;wie oft schieben
                    bmi     gebrochen       ;geht ber Bytegrenze
                    rol.w   d5,d0           ;schieben
                    or.b    d0,(a1)
                    tst.w   d5              ;null geschoben?
                    bne     weiter          ;nein, weiter machen
                    addq.l  #1,a1            ;ja, eine Adresse weiter
                    bra     weiter
gebrochen:          neg.w   d5              ;negieren
                    ror.w   d5,d0           ;nach rechts schieben
                    or.b    d0,(a1)+        ;einodern
                    ror.w   #8,d0           ;Rest holen
                    or.b    d0,(a1)

weiter:             sub.w   #5,d6           ;abziehen
                    bpl     okay            ;nein, allright
                    addq.w  #8,d6
okay:               addq.w  #1,d7
                    cmp     num(a6),d7
                    bne     loop

                    movem.l (a7)+,d0-d7/a0-a6
                    }


  c_home();
  Cconws("SAVING...\r\n");

  i=strlen(path_file);
  path_file[--i]='T';
  path_file[--i]='A';
  path_file[--i]='D';
  filehandle = Fcreate(path_file,0);
  Fwrite(filehandle,1800L,comp);
  Fclose(filehandle);

  Setscreen(-1L,-1L,1);
  Setcolor(0,0x777);
  Setcolor(3,0x000);

  v_show_c(handle,1);
  }
  close_work();
}

load(a,b,c,d)

char a[];
char *b;
long c;
long d;

{
  int fh;

  fh = Fopen(a,0);

  Fseek(c,fh,0);
  Fread(fh,d,b);

  Fclose(fh);
}

c_down(hh)
int hh;
{
  Cconout(27);
  Cconout('B');
}

c_home(hh)
int hh;
{
  Cconout(27);
  Cconout('H');
}

c_cls(hh)
int hh;
{
  Cconout(27);
  Cconout('E');
}

auswahl(filename)
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
  return (TRUE);
}

obj_auswahl(path,file,button)
char *path,*file;
int *button;
{
  int i;

  fsel_input(path,file,button);
}

Sm()
{
  graf_mouse(M_ON,0L);
}

Hm()
{
  graf_mouse(M_OFF,0L);
}

druck(adr)
char adr[];
{
  int i;

  for(i=0;adr[i]!=0;i++) Cconout(adr[i]);
  Crawcin();
}

pic_move(h,quelle,zeile,z2,anzahl)
int h;                  /* handle, wird aber nicht ben”tigt */
long quelle;            /* Zeiger auf Bufferanfang */
int zeile;              /* Zeile des Ursprungsbildes, ab der geschaufelt werden soll */
long z2;                /* Zeile, ab der Bild auf Screen erscheint */
int anzahl;             /* Anzahl der Zeilen, die bertragen werden sollen */
{
  Hm();
  copy_zeilen(quelle+zeile*160L,Logbase()+z2*160L,anzahl);
  Sm();
}

copy_zeilen(src,dest,count)
long *src,*dest;
int count;
{
  int i;

  for(;count>0;count--)
    for(i=0;i<40;i++) *dest++=*src++;
}

