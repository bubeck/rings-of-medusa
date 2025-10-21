
/* Dieses Programm schreibt den Digisound "E:\HASEL.SEQ" auf
   die RÅckseite der Disk in Drive A:
   Diskformat in A: 82/10 */

#include <stdio.h>
#include <osbind.h>

long speicher[125000L];                 /* 500K */

main()
{
  int fh;
  long adr;
  int track,err;

  adr=(long)speicher;

  fh=Fopen("E:\\HASEL.SEQ",0);
  if (fh>=0) {
    Fread(fh,0xdfdfdf,speicher);        /* File lesen */
    Fclose(fh);

    for(track=0;track<82;track++) {
      err=Flopwr(adr+track*5120L,0L,0,1,track,1,10);
      printf("%cH Schreibe Track: %2d    Code: %4d\n",27,track,err);
      if (err<0) {
        Crawcin();
        }
      err=Flopver(adr+track*5120L,0L,0,1,track,1,10);
      printf("%cH   Verify Track: %2d    Code: %4d\n",27,track,err);
      if (err<0) {
        Crawcin();
        }
      }
    }
}

