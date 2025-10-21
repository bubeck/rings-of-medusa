#include <osbind.h>
#include <geminit.h>
#include "GRUND.C"

char path[64]="H:\\MEDUSA\\MEDUSA_2.COL\\MAPS\\TMAPS\\*.NEO",file[64];
char path_file[64];

#define FALSE 0
#define TRUE -1
#define M_OFF 256
#define M_ON 257

int unten[8][3] = {
  0,0,0,
  15,0,0,
  0,15,0,
  0,0,15,           /* erkennungsmuster in der untersten Zeile */
  15,15,0,
  15,0,15,
  0,15,15,
  15,15,15
  };

main()
{
  char *picbuf;
  char *palbuf;

  char filename[13];
  int  code = 0;
  char env[80 * 38]; /* = 3040 */
  char drive;
  int  i;
  int  num = 0;
  int  mx,my,mk;
  int  filehandle;
  int  pxy[4];
  int grund;
  int max_i;
  int farbe,dummy;
  register int zeile;
  register int spalte;
  register int x,y;

  palbuf = (char *) Malloc (   32L);
  picbuf = (char *) Malloc (32000L);

  open_work();

  do {
  Cconout(27);
  Cconout('E');
  Cconws("Dieses Programm konvertiert eine GMAP.NEO in eine TMAP.NEO-Datei");

  if (!auswahl(path_file)) break;

  Setscreen(-1L,-1L,0);
  Setcolor(0,0);
  Setcolor(15,0x777);

    v_hide_c(handle);

  load(path_file, palbuf,  4L,    32L);
  load(path_file, Logbase(),128L, 32000L);

  Setpalette(palbuf);

  vsl_color(handle,0);

  vsf_interior(handle,1);
  vsf_perimeter(handle,0);

  for (y = 17; y <= 167; y += 4)
  {
    for (x = 0; x <= 319; x += 4)
    {
      for(zeile=y+1;zeile<=y+3;zeile++)
        for(spalte=x+1;spalte<=x+3;spalte++) {
          get_pixel(spalte,zeile,&farbe);  /* Farbe holen */
          if (farbe!=15) goto raus;
          }

raus:
      hline(x+1,y+1,x+3,farbe);
      hline(x+1,y+2,x+3,farbe);
      hline(x+1,y+3,x+3,farbe);
      }
  }

  for(i=0;path_file[i]!=0;i++) if (path_file[i]=='\\') max_i=i;

  path_file[++max_i]='T';               /* Tmap-Datei */
  filehandle = Fcreate(path_file,0);
  Fwrite(filehandle,4L,"");
  Fwrite(filehandle,32L,0xff8240L);
  Fwrite(filehandle,92L,"");
  Fwrite(filehandle,32000L,Logbase());
  Fclose(filehandle);

    v_show_c(handle,1);

  Setscreen(-1L,-1L,1);
  Setcolor(0,0x777);
  Setcolor(3,0x000);

  } while(TRUE);
  close_work();
}

get_pixel(x,y,farbe)
int x,y,*farbe;
{
  int color;

  asm {
        MOVEM.L D0-D7/A0-A6,-(A7)

        DC.W    0XA000
        MOVEA.L 12(A0),A0               ; PTSIN
        MOVE.W  x(A6),(A0)
        MOVE.W  y(A6),2(A0)
        DC.W    0XA002                  ; GET_PIXEL
        MOVE.W  D0,color(A6)

        MOVEM.L (A7)+,D0-D7/A0-A6
        }
  *farbe=color;
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

hline(x1,y,x2,farbe)
int x1,y,x2,farbe;
{
  int x;

  for(x=x1;x<=x2;x++) put_pixel(x,y,farbe);
}

put_pixel(x,y,farbe)
int x,y,farbe;
{
  asm {
    MOVEM.L D0-D7/A0-A6,-(A7)
    DC.W    0XA000
    MOVE.L  12(A0),A0
    MOVE.W  x(A6),(A0)
    MOVE.W  y(A6),2(A0)
    DC.W    0XA000
    MOVE.L  8(A0),A0
    MOVE.W  farbe(A6),(A0)
    DC.W    0XA001

    MOVEM.L (A7)+,D0-D7/A0-A6
    }
}
