#include <osbind.h>
#include <geminit.h>

char path[64]="H:\\MEDUSA\\MEDUSA_2.COL\\MAPS\\TMAPS\\TMAP?.NEO",file[64];
char path_file[64];

#define FALSE 0
#define TRUE -1
#define M_OFF 256
#define M_ON 257

#define ANZAHL 32

int unten[ANZAHL][9] = {
  0,0,0,0,0,0,0,0,0,
  15,0,0,0,0,0,0,0,0,           /* Kreuzung */
  0,15,0,0,0,0,0,0,0,           /* Weg 1 */
  0,0,15,0,0,0,0,0,0,           /* 2 */
  0,0,0,15,0,0,0,0,0,           /* 3 */
  0,0,0,0,15,0,0,0,0,
  0,0,0,0,0,15,0,0,0,
  0,0,0,0,0,0,15,0,0,
  0,0,0,0,0,0,0,15,0,
  0,0,0,0,0,0,0,0,15,
  15,15,0,0,0,0,0,0,0,
  15,0,15,0,0,0,0,0,0,
  0,15,15,0,0,0,0,0,0,
  15,15,15,0,0,0,0,0,0,
  0,0,0,15,15,0,0,0,0,
  0,0,0,15,0,15,0,0,0,
  0,0,0,0,15,15,0,0,0,
  0,0,0,15,15,15,0,0,0,
  0,0,0,0,0,0,15,15,0,
  0,0,0,0,0,0,15,0,15,
  0,0,0,0,0,0,0,15,15,
  0,0,0,0,0,0,15,15,15,
  0,0,0,15,0,0,15,0,0,
  15,0,0,0,0,0,15,0,0,
  15,0,0,15,0,0,0,0,0,				/* 24=Port 1 */
  15,0,0,15,0,0,15,0,0,            /* 25=Port 2 */

  15,15,15,15,15,15,0,0,0,                 /* STADT 1 */
  0,0,0,15,15,15,15,15,15,                 /* STADT 2 */
  15,15,0,15,15,0,15,15,0,                 /* STADT 3 */
  15,15,15,15,15,15,15,15,15,                 /* STADT 4 */
  0,15,15,0,15,15,0,15,15,                 /* BURG 1 */
  15,0,15,0,0,0,15,0,15,                 /* BURG 2 */
   };

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
  int  num = 0;
  int  mx,my,mk;
  int  filehandle;
  int  pxy[4];
  int grund;
  char comp[80*38];

  register int j,i,x,y;

  palbuf = (char *) Malloc (   32L);
  picbuf = (char *) Malloc (32000L);

  open_work();

  while (auswahl(path_file)) {

  num=0;
  Setscreen(-1L,-1L,0);
  Setcolor(0,0);
  Setcolor(15,0x777);

  v_hide_c(handle);

  load(path_file, palbuf,  4L,    32L);
  load(path_file, Logbase(),128L, 32000L);

  Setpalette(palbuf);

  vsl_color(handle,0);

  for (y = 25; y <= 165; y += 4)
  {
    for (x = 0; x < 320; x += 4)
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

      for (i=0;i<9;i++) if (pel[i]!=15) pel[i]=0;

      for(i=0;i<ANZAHL;i++) {             /* Alle 7 Muster durchtesten */
        for(j=0;j<9;j++) if (unten[i][j]!=pel[j]) break;
        if (j==9) break;
        }

      if (i==ANZAHL) {
        c_home();
        printf("Unbekanntes Erkennungsmuster!\n    X:%d   Y:%d\n",x+1,y+1);
        Crawcin();
        }
      grund=i;

      env[num++] = (char) grund;
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
                    bmi     gebrochen       ;geht über Bytegrenze
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
  Fwrite(filehandle, 1800L,comp);
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

