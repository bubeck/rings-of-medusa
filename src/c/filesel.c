/************************************************************************/
/*  FSEL.C - Definition for easy use of fsel_box                        */
/*      Copyright 1990 Till Bubeck, Ziegeleistr. 28, 7056 Weinstadt     */
/************************************************************************/

char path_file[150];

char *fileselect(path,file,ext_info)
char path[],file[];
char ext_info[];
{
  /* Benutzer kann Pfad und File übergeben, dabei werden dann
     die Adresse des kompletten Zugriffspfades zurückgegeben,
     Sollte der Benutzer Abbruch anklicken, wird NULL zurückgegeben */

  int i;
  int backslash;
  int version;
  int button;

  version=tos_version();

  if (version>=0x0104)                     /* TOS 1.4 */
    fsel_exinput(path,file,&button,ext_info);
  else
    fsel_input(path,file,&button);

  if (button==0) return(0L);

  for(i=0;path[i]!=0;i++) {                 /* Pfad kopieren */
    path_file[i]=path[i];
    if (path[i]=='\\') backslash=i+1;       /* Letzten Backslash merken */
    }
  i=0;
  do {
    path_file[backslash+i]=file[i];         /* Filenamen anhängen */
    } while(file[i++]!=0);

  return(path_file);
}

int tos_version()
{
  long ssp;
  long *adr;
  SYSBASE *sys;
  int version;

  ssp=Super(0L);                /* Supervisor an */
  adr=(long *)0x4f2;                    /* Dort liegt Pointer auf Sysbase */
  sys=(SYSBASE *)(*adr);
  version=sys->version;
  Super(ssp);

  return(version);
}



