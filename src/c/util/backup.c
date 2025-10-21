
/* Version 1.0 */
/* Backup Accessory - speichert alle ver„nderten Files auf Harddisk */

#include <stdio.h>
#include <obdefs.h>
#include <gemdefs.h>
#include <geminit.h>
#include <osbind.h>
#include <osdefs.h>

#include "E:\\BACKUP.H"

#define TRUE 1
#define FALSE 0

int drive;                  /* aktuelles Laufwerk */
int drv_map;                /* m”gliche Laufwerke */
long *save,*save2,*org;                 /* sichert den Bildschirm */
int i,button;
char zielpfad[80];              /* Zielpfadnamen */
short flag;
OBJECT *box;
int deskx,desky,deskw,deskh;
int x,y,w,h;
dta *dtadr;
int time;                   /* Uhrzeit der Abspeicherung */
long memory;
long max_len;               /* maximale L„nge des Speichers */

main()
{
  char anfang[4];
  int i;

  open_work();
  if (!rsrc_load("BACKUP.RSC")) {
    form_alert(1,"[3][|Can't find my RSC-File!][ Cancel ]");
    close_work();
    exit(0);
    }

  rsrc_gaddr(R_TREE,BOX,&box);
  wind_get(0,WF_WORKXYWH,&deskx,&desky,&deskw,&deskh);

  dtadr=(dta *)Fgetdta();
  flag=FALSE;
  drive=Dgetdrv();
  if (Fsfirst("TIME.DAT",0)<0) {
    form_alert(1,"[3][|Can't find TIME.DAT!][ Cancel ]");
    close_work();
    exit(0);
    }
  time=dtadr->dta_time;             /* Uhrzeit holen */

  save=(long *)Malloc(32000L);      /* Bildschirm sichern */
  save2=save;
  org=(long *)Logbase();            /* Bildschirmspeicher */
  max_len=Malloc(-1L);
  max_len-=60000L;                  /* 60K freilassen */
  memory=Malloc(max_len);

  drv_map=Drvmap();         /* alle m”glichen Laufwerke holen */
  if ((drv_map&16==16))
    if (Fsfirst("E:\\LASER_C",16)>=0) {  /* Ordner vorhanden */
      strcpy(zielpfad,"E:\\LASER_C\\*.*");
      flag=TRUE;
      }
  if (!flag) {
    strcpy(zielpfad,"A:\\*.*");         /* Defaultpfad */
    }

  for(i=0;i<8000;i++) *save++=*org++;   /* Bildschirm sichern */
  fsel_input(zielpfad,"",&button);
  org=(long *)Logbase();
  save=save2;
  for(i=0;i<8000;i++) *org++=*save++;   /* Bildschirm restaurieren */

  if (button==1) {                  /* Okay */
    i=strlen(zielpfad);
    for(;i>0;i--) if (zielpfad[i]=='\\') break;
    zielpfad[i]=0;

    form_center(box,&x,&y,&w,&h);
    form_dial(FMD_START,0,0,0,0,x,y,w,h);
    form_dial(FMD_GROW,deskw/2,deskh/2,4,4,x,y,w,h);
    set_name("");                   /* Name setzen */
    box[WRITE].ob_state=DISABLED;
    objc_draw(box,0,5,deskx,desky,deskw,deskh);
    anfang[0]='\\';
    anfang[1]=0;
    backup(anfang);                       /* und sichern */
    Fclose(Fcreate("TIME.DAT",0));          /* Uhrzeit neu sichern */

    form_dial(FMD_SHRINK,deskw/2,deskh/2,4,4,x,y,w,h);
    form_dial(FMD_FINISH,0,0,0,0,x,y,w,h);
    }

  rsrc_free();
  Mfree(save2);                     /* speicher freigeben */
  Mfree(memory);
  close_work();
  Pterm0();
}

set_name(text)
char text[];
{
  /* Setzt den text in die Zeile in der Objectbox ein */
  int i;
  char *str1,*str2;

  str1=box[NAME1].ob_spec;
  str2=box[NAME2].ob_spec;
  for(i=0;i<30;i++)
    str1[i]=text[i];
  for(i=0;i<30;i++)
    str2[i]=text[i+30];
  if (strlen(text)<=30) str2[0]=0;
}

draw_name()
{
  /* Zeichnet den Namen neu */
  int clipx,clipy,clipw,cliph;

  objc_offset(box,NAME1,&clipx,&clipy);
  clipw=box[NAME1].ob_width+box[NAME2].ob_width;
  cliph=box[NAME1].ob_height;
  objc_draw(box,0,5,clipx,clipy,clipw,cliph);
}

backup(quellpfad)
char quellpfad[];
{
  /* Sichert alles, was in diesem Pfad ist */

  char hier_pfad[80];
  dta dta_loc;
  int len;
  long old_dta;

  Dsetpath(quellpfad);
  old_dta=(long)Fgetdta();
  Fsetdta(&dta_loc);

  if (Fsfirst("*.*",20)>=0)               /* Alle Files kopieren */
    do {
      if (dta_loc.dta_attr==16) {         /* Ordner */
        if (dta_loc.dta_name[0]=='.') continue;
        strcpy(hier_pfad,quellpfad);
        strcpy(&hier_pfad[strlen(hier_pfad)],dta_loc.dta_name);
        len=strlen(hier_pfad);
        hier_pfad[len]='\\';
        hier_pfad[len+1]=0;
        backup(hier_pfad);
        Dsetpath(quellpfad);                 /* und wieder schliežen */
        }
      else test_file(quellpfad,dta_loc);

      if (Cconis())
        if ((int)Crawcin()==27) {
          rsrc_free();
          Mfree(save2);                     /* speicher freigeben */
          Mfree(memory);
          close_work();
          Pterm0();
          }
      } while(Fsnext()>=0);

  Fsetdta(old_dta);
}

test_file(pfad,dta_act)
char pfad[];
dta dta_act;
{
  char quellfile[80];
  int len;
  int x,y,w,h;

  strcpy(quellfile,pfad);
  len=strlen(quellfile);
  strcpy(&quellfile[len],dta_act.dta_name);
  set_name(quellfile);
  draw_name();

  if (time<dta_act.dta_time) {      /* File muž kopiert werden */
    objc_offset(box,WRITE,&x,&y);
    w=box[WRITE].ob_width;
    h=box[WRITE].ob_height;
    box[WRITE].ob_state=NORMAL;
    objc_draw(box,0,5,x,y,w,h);
    copy_file(pfad,dta_act.dta_name);
    box[WRITE].ob_state=DISABLED;
    objc_draw(box,0,5,x,y,w,h);
    }
}

copy_file(pfad,filename)
char pfad[],filename[];
{
  char quellfile[80];
  char destfile[80];
  int len;
  int fhr,fhw;
  long anzahl;

  len=strlen(filename);         /* kein .o file */
  if (filename[len-1]=='O' && filename[len-2]=='.') return(0);

  strcpy(quellfile,pfad);
  len=strlen(quellfile);
  strcpy(&quellfile[len],filename);     /* Quellfile erstellt */

  strcpy(destfile,zielpfad);
  strcpy(&destfile[strlen(destfile)],quellfile);

  if ((fhr=Fopen(quellfile,0))>=0) {
    if ((fhw=Fcreate(destfile,0))>=0) {
      do {
        anzahl=Fread(fhr,max_len,memory);
        Fwrite(fhw,anzahl,memory);
        } while (anzahl==max_len);
      Fclose(fhw);
      Fclose(fhr);
      }
    else {
      form_error(-fhw);
      Fclose(fhr);
      }
    }
  else {
    form_error(-fhr);
    }
}

