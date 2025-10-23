               
/* Dieser Part stellt alle Raster-Routinen zur Verfügung, um also
   rechteckige Grafikbereiche zu bearbeiten.
   Weiterhin sind hier sämtliche Funktionen zur Objektebearbeitung
   zu finden.

   Copyright 1990 by Till Bubeck, Ziegeleistr. 28, 7056 Weinstadt
                                  Tel.: 07151-66437                 */

#include "includes.c"           /* Definiert alle Variablen als Extern,... */
#include "x.h"

typedef struct {
  char header[6];           /* Header des Files 'NEOOBJ' */
  UWORD version;              /* Versionsnummer des Files */
  UWORD internal;
  } OBJEKT_HEADER1;

typedef struct {
  uint32_t dateilaenge;       /* Anzahl der nachfolgenden Bytes */
  UWORD anzahl;               /* Anzahl der Objekte in der Bibliothek */
  } OBJEKT_HEADER2;
  
void pic_move(quelle,y1,y2,y3)
void *quelle;           /* Zeiger auf Bufferanfang */
int y1;                 /* Startzeile, ab der übertragen wird */
int y2;                 /* Endzeile, bis zu der übertragen wird */
int y3;                 /* Startzeile auf Screen, wohin übertragen wird */
{
	/* Kopiert aus einem 320 Pixel breiten Buffer auf den Bildschirm, und zwar Zeilenweise
		 von y1-y2 nach y3 auf den Bildschirm */
		 
  Hm();
  cpy_raster(quelle,logbase,0,y1,319,y2,0,y3);
  Sm();
}

void cpy_raster(quelle,ziel,x1,y1,x2,y2,x3,y3)
void *quelle,*ziel;                   /* Adresse Quell- und Zielbildschirm */
int x1,y1,x2,y2;                    /* Koordinaten im Quellscreen */
int x3,y3;                          /* Linke obere Ecke im Zielscreen */
{
  /* Kopiert rechteckiges Grafikstück von einem Screen zum anderen */
  /* Dabei muß der Screen in Low-Res sein und eine Breite von 320 Pixeln haben */

  raster_replace(x1,y1,x2,y2,quelle,320,x3,y3,ziel,320);
}

long load_objekte(char *filename, void *adr)
{
	/* Lädt Objekte und wirft alle unwichtigen Objektheader raus
		 Gibt die Länge der Objektdatei VOR dem Rauswerfen der Header zurück. */
  OBJEKT_HEADER1 *ob_header;
  OBJEKT_HEADER2 *ob_header2;
  long laenge,max_laenge;
  unsigned char *startadr;
  int i,j;
  
  /* check if this object is already loaded: */
  for(i=0;i<SH_OBJ_MAX;i++)
    if (strcmp(sh_obj[i].filename,filename)==0)
      {
	/* all other objects in this area are deleted: */
	for(j=0;j<SH_OBJ_MAX;j++)
	  if (sh_obj[j].atari_mem==adr) sh_obj[j].atari_mem=NULL;

	sh_obj[i].atari_mem=adr;
	return sh_obj[i].len;
      }

  /* not yet loaded. search a free place: */
  for(i=0;i<SH_OBJ_MAX;i++)
    if (sh_obj[i].filename[0]==0) break;
  
  strcpy(sh_obj[i].filename,filename);
  sh_obj[i].object_count = 0;
  sh_obj[i].obj_mem=malloc(70000);
  /* all other objects in this area are deleted: */
  for(j=0;j<SH_OBJ_MAX;j++)
    if (sh_obj[j].atari_mem==adr) sh_obj[j].atari_mem=NULL;
  sh_obj[i].atari_mem=adr;
  
  max_laenge=load_bibliothek(filename,sh_obj[i].obj_mem); 
		
  ob_header=(OBJEKT_HEADER1 *)sh_obj[i].obj_mem;
  // Convert from big endian
  be_2(&ob_header->version);
  be_2(&ob_header->internal);
  if (ob_header->header[0]=='N' &&
        ob_header->header[1]=='E' &&
        ob_header->header[2]=='O' &&          /* Korrekter Header? */
        ob_header->header[3]=='O' &&
        ob_header->header[4]=='B' &&
        ob_header->header[5]=='J' &&
        ob_header->version==0x0101) {
    ob_header2=(OBJEKT_HEADER2 *)(sh_obj[i].obj_mem+sizeof(OBJEKT_HEADER1)+ob_header->internal);
    be_4(&ob_header2->dateilaenge);
    be_2(&ob_header2->anzahl);
		laenge=ob_header2->dateilaenge;
#ifndef unix
    startadr=adr;
#else
    startadr=(unsigned char *)sh_obj[i].obj_mem+2; /* alignment for SPARC */
    *(startadr-1)=*(startadr-2)=0;
#endif
    startadr = sh_obj[i].obj_mem;
    memmove(startadr,&ob_header2->anzahl,laenge);   /* Filenamen überspringen */
    convert_objekte(&sh_obj[i]);            /* Richtig konvertieren */

    max_laenge=(max_laenge+8) & 0xfff8;
    sh_obj[i].len=max_laenge;
    return(sh_obj[i].len);                         /* + Länge der übersprugenen Header */
    }
  else 
    { fprintf(stderr,"RoM: Wrong object format.\n"); od(ob_header->header); abort(); }

}

void convert_objekte(SHARED_OBJECT *sh_obj)
{
	/* Konvertiert einen komplette Objektbibliothek in ein Gerätespezifisches Format: */
  int i;
  UWORD anzahl;
	OBJEKT *obj;
  void *ob_mem;
  UNIX_OBJECT *uo;
	
  ob_mem = sh_obj->obj_mem;
  anzahl=*(UWORD *)ob_mem;

	obj=(OBJEKT *)ob_mem;	
	for(i=0;i<anzahl;i++)
    {
      convert_objekt(i, obj, &sh_obj->objects[i]);
      sh_obj->object_count++;
    }
}

int shrink_obj_breite(size,ob_nr,ob_mem)
int size;									/* 0..255, 0=Normal, 255=miniklein */
int ob_nr;
void *ob_mem;
{
	/* Berechnet die Breite des Objekts, wenn es entsprechend verkleinert ist */
	float groesse;
	
	groesse=(255-size)/255.0;								/* 0.0=0%, 1.0=100% */
	return(groesse*objekt_breite(ob_nr,ob_mem));
}

int shrink_obj_hoehe(size,ob_nr,ob_mem)
int size;									/* 0..255, 0=Normal, 255=miniklein */
int ob_nr;
void *ob_mem;
{
	/* Berechnet die Höhe des Objekts, wenn es entsprechend verkleinert ist */
	float groesse;
	
	groesse=(255-size)/255.0;								/* 0.0=0%, 1.0=100% */
	return(groesse*objekt_hoehe(ob_nr,ob_mem));
}


int objekt_breite(ob_nr,ob_mem)
int ob_nr;
void *ob_mem;
{
  /* Berechnet die Breite des entsprechenden Objekts */
  SHARED_OBJECT *sh_obj;
  
  sh_obj=search_obj_addr(ob_mem);
  
  assert (ob_nr<sh_obj->object_count);
  //if (ob_nr>=sh_obj->object_count) return(0);           /* Soviel Objekte sind nicht da */
  
  return(sh_obj->objects[ob_nr].surface->w);
}

FLAG objekt_exists(ob_nr, ob_mem)
int ob_nr;
void *ob_mem;
{
  SHARED_OBJECT *sh_obj;

  sh_obj=search_obj_addr(ob_mem);
  return (ob_nr<sh_obj->object_count);
}

int objekt_hoehe(ob_nr,ob_mem)
int ob_nr;
void *ob_mem;
{
  /* Berechnet die Höhe des entsprechenden Objekts */
  SHARED_OBJECT *sh_obj;
  
  sh_obj=search_obj_addr(ob_mem);
  
  assert (ob_nr<sh_obj->object_count);
  //if (ob_nr>=sh_obj->object_count) return(0);           /* Soviel Objekte sind nicht da */
  
  return(sh_obj->objects[ob_nr].surface->h);
}

void *objekt_surface(int ob_nr, void *ob_mem)
{
  /* Berechnet die Höhe des entsprechenden Objekts */
  SHARED_OBJECT *sh_obj;

  sh_obj=search_obj_addr(ob_mem);

  assert (ob_nr<sh_obj->object_count);
  //if (ob_nr>=sh_obj->object_count) return(0);           /* Soviel Objekte sind nicht da */
  
  return(sh_obj->objects[ob_nr].surface);
}

raster_t *objekt_palette(ob_nr,ob_mem, raster_no)
int ob_nr;
void *ob_mem;
{
  /* Berechnet die Höhe des entsprechenden Objekts */
  SHARED_OBJECT *sh_obj;
  UWORD *anzahlp,anzahl;
  OBJEKT *obj_header;
  int raster_anzahl;
  raster_t *pal_obj;

  sh_obj=search_obj_addr(ob_mem);

  if (ob_nr>=sh_obj->object_count) return(NULL);           /* Soviel Objekte sind nicht da */
  
  anzahlp=(UWORD *)sh_obj->obj_mem;
  anzahl=*anzahlp++;

  obj_header=(OBJEKT *)anzahlp;
  obj_header+=ob_nr;                /* points to our objekt_header */
      
  raster_anzahl=(obj_header->reserved)>>8;
  if (raster_no >= raster_anzahl)
    return NULL;

  /* calculate start of rasters palettes: */
  pal_obj=(raster_t *)((unsigned char *)anzahlp+obj_header->offset+
		     obj_header->hoehe*obj_header->breite_bytes);

  return (pal_obj+raster_no);
}

unsigned int move_objekt(ob_nr,ob_mem,modus,ziel_y,ziel_mem,clip_modus,clipx1,clipy1,clipx2,clipy2,switch_scr)
int ob_nr;
void *ob_mem;
int modus;											/* Verknüpfungsmodus */
int ziel_y;											/* Retten des Hintergrundes */
void *ziel_mem;
unsigned int clip_modus;					/* Siehe Clipmodus in typedef.c */
int clipx1,clipy1,clipx2,clipy2;
FLAG switch_scr;									/* 2 Bildschirmtechnik? */
{
	/* Bewegt ein Objekt innerhalb eines Rechtecks ggf. unter Berücksichtigung der 2 Bildschirm-
		 technik. */
	int mx1,my1;							/* Diese Mausposition */
	int mx2,my2;							/* letzte Mausposition */
	int breite,hoehe;
	unsigned int abbruch=0;
		
	breite=objekt_breite(ob_nr,ob_mem);					/* Größe des Objekts holen */
	hoehe=objekt_hoehe(ob_nr,ob_mem);
	
	Hm();
	
	if (switch_scr) copy_screen(scr1,scr2);					/* auf 2. Bildschirm übertragen */
	cpy_raster(scr1,ziel_mem,0,clipy1,319,clipy2,0,ziel_y);		/* Hintergrund retten */

	mx2=my2=-1;															/* In jedem Fall zeichnen */
	
	wait_mouse(0);													/* Bitte loslassen */

	do {
		do {
			hol_maus();
			mx1=mx-breite/2;										/* Objekte zentrieren */
			my1=my-hoehe/2;
			if (mx1>1+clipx2-objekt_breite(ob_nr,ob_mem)) {
				if (clip_modus & STOP_R) {
					mx1=1+clipx2-objekt_breite(ob_nr,ob_mem);
					set_mousepos(mx1+breite/2,my1+hoehe/2);
					}
				if (clip_modus & ENDE_R) abbruch=ENDE_R;
				}
			if (mx1<clipx1) {
				if (clip_modus & STOP_L) {
					mx1=clipx1;
					set_mousepos(mx1+breite/2,my1+hoehe/2);
					}
				if (clip_modus & ENDE_L) abbruch=ENDE_L;
				}
			if (my1<clipy1) {
				if (clip_modus & STOP_O) {
					my1=clipy1;
					set_mousepos(mx1+breite/2,my1+hoehe/2);
					}
				if (clip_modus & ENDE_O) abbruch=ENDE_O;
				}
			if (my1>(clipy2-objekt_hoehe(ob_nr,ob_mem))) {
				if (clip_modus & STOP_U) {
					my1=clipy2-objekt_hoehe(ob_nr,ob_mem);
					set_mousepos(mx1+breite/2,my1+hoehe/2);
					}
				if (clip_modus & ENDE_U) abbruch=ENDE_U;
				}
			} while(abbruch==0 && mk==0 && mx2==mx1 && my2==my1);

		if (mx2!=-1 && !switch_scr) cpy_raster(ziel_mem,scr1,0,ziel_y+(my2-clipy1),
								319,ziel_y+(my2-clipy1)+objekt_hoehe(ob_nr,ob_mem)-1,0,my2);
		draw_obj(ob_nr,ob_mem,modus,switch_scr ? scr2 : scr1,mx1,my1);
		if (switch_scr) switch_screens();
    else show_screen(scr1);      /* for X11 */

		if (mx2!=-1 && switch_scr) cpy_raster(ziel_mem,scr2,0,ziel_y+(my2-clipy1),
								319,ziel_y+(my2-clipy1)+objekt_hoehe(ob_nr,ob_mem)-1,0,my2);
		mx2=mx1;
		my2=my1;												/* Alte Mausposition merken */
		} while(mk==0 && abbruch==0);

	if (switch_scr) copy_screen(scr2,scr1);		/* mit gelöschtem Objekt zurückübertragen */

	hol_maus();
	Sm();
	
	return(abbruch);
}

void draw_shrink_obj(size,shrink_buff,ob_nr,ob_mem,modus,scr,x,y)
int size;									/* 0..255: 0 entspricht 100%, 255 entspricht miniklein */
void *shrink_buff;				/* Buffer, der zum Verkleinern benützt wird */
int ob_nr;
void *ob_mem;
int modus;								/* Verknüpfungsmodus */
void *scr;
int x,y;
{
	/* Zeichnet ein verkleinertes Objekt auf den Bildschirm */
	
#if 0
  draw_obj(ob_nr, ob_mem, MOVE, scr1, 0, 0);
  show_screen(scr1);
#endif
  
	draw_shrink_obj_part(size,shrink_buff,ob_nr,ob_mem,
					0,0,objekt_breite(ob_nr,ob_mem)-1,objekt_hoehe(ob_nr,ob_mem)-1,modus,scr,x,y);
}

void draw_shrink_obj_part(size,shrink_buff,ob_nr,ob_mem,x1,y1,x2,y2,modus,scr,x,y)
int size;									/* 0..255: 0 entspricht 100%, 254 entspricht miniklein */
void *shrink_buff;				/* Buffer, der zum Verkleinern benützt wird */
int ob_nr;
void *ob_mem;
int x1,y1,x2,y2;					/* Bereich innerhalb des Objektes */
int modus;								/* Verknüpfungsmodus */
void *scr;
int x,y;
{
	/* zeichnet einen Teil eines verkleinerten Objektes auf den Bildschirm */
	float groesse;
	
	groesse=(255-size)/255.0;								/* 0.0=0%, 1.0=100% */
	if (groesse>0.5) shrink1(groesse,shrink_buff,ob_nr,ob_mem,x1,y1,x2,y2,modus,scr,x,y);
	else if (size!=255) shrink2(groesse,shrink_buff,ob_nr,ob_mem,x1,y1,x2,y2,modus,scr,x,y);
}

void shrink1(float size,void *shrink_buff,int ob_nr,void *ob_mem,int x_start,int y_start,int x_end,int y_end,int modus,void *scr,int x_scr,int y_scr)
{
  float schrittweite;
	float hoehe,breite;
  float x_ziel,y_ziel;
  float x_quelle,y_quelle;
	int ymax,xmax;
	KOORD groesse;

	if (size>=0.999) {									/* Volle Größe */
		draw_obj_part(ob_nr,ob_mem,x_start,y_start,x_end,y_end,modus,scr,x_scr,y_scr);
		}
	else {
		groesse.x=groesse.y=0;						/* Fehler! */
		
	  schrittweite=1.0/(1.0/size-1.0);
		hoehe=y_end-y_start+1;
		breite=x_end-x_start+1;
	
	  for(y_quelle=y_ziel=0.0;y_quelle<hoehe;y_quelle+=schrittweite+1.0,y_ziel+=schrittweite) {
			if (y_quelle+schrittweite>=hoehe) ymax=hoehe;
			else ymax=y_quelle+schrittweite;
			
			draw_obj_part(ob_nr,ob_mem,x_start,y_start+(int)y_quelle,x_end,y_start+ymax-1,
										MOVE,shrink_buff,0,(int)y_ziel);
			groesse.y=(int)y_ziel+(ymax-(int)y_quelle);
	    }
	
	  for(x_quelle=x_ziel=0.0;x_quelle<breite;x_quelle+=schrittweite+1.0,x_ziel+=schrittweite) {
			if (x_quelle+schrittweite>=breite) xmax=breite;
			else xmax=x_quelle+schrittweite;
	    cpy_raster(shrink_buff,shrink_buff,(int)x_quelle,0,xmax-1,groesse.y-1,
	        (int)x_ziel,0);
			groesse.x=(int)x_ziel+(xmax-(int)x_quelle);
	    }
	
		if (modus==MOVE) raster_replace(0,0,groesse.x-1,groesse.y-1,shrink_buff,320,x_scr,y_scr,scr,320);
		else raster_transp(0,0,groesse.x-1,groesse.y-1,shrink_buff,320,x_scr,y_scr,scr,320);
		}
}

void shrink2(float size,void *shrink_buff,int ob_nr,void *ob_mem,int x_start,int y_start,int x_end,int y_end,int modus,void *scr,int x_scr,int y_scr)
{
  float schrittweite;
	float hoehe,breite;
  float x_ziel;
	int y_ziel;
  float x_quelle,y_quelle;

  schrittweite=1.0/size;					/* Jede x. Zeile nehmen */
	hoehe=y_end-y_start+1;
	breite=x_end-x_start+1;

  // Step 1: we copy only every "x. line" into shrink_buff 
  for(y_quelle=0.0,y_ziel=0;y_quelle<hoehe;y_quelle+=schrittweite,y_ziel++) {
		draw_obj_part(ob_nr,ob_mem,x_start,y_start+(int)y_quelle,x_end,y_start+(int)y_quelle,
									MOVE,shrink_buff,0,y_ziel);
    }

  // Step 2: we copy only every "x. col" into shrink_buff 
  for(x_quelle=0.0,x_ziel=0;x_quelle<breite;x_quelle+=schrittweite,x_ziel++) {
		cpy_raster(shrink_buff,shrink_buff,(int)x_quelle,0,(int)x_quelle,y_ziel-1,x_ziel,0);
    }

	if (modus==MOVE) raster_replace(0,0,x_ziel-1,y_ziel-1,shrink_buff,320,x_scr,y_scr,scr,320);
	else raster_transp(0,0,x_ziel-1,y_ziel-1,shrink_buff,320,x_scr,y_scr,scr,320);
}

void draw_grow_obj(ob_nr,ob_mem,modus,screen,x,y)
int ob_nr;
void *ob_mem;
int modus;
void *screen;
int x,y;
{
	/* Zeichnet ein Objekt auf den Screen, unterlegt es aber vorher mit Shrinkboxen */
	int breite,hoehe;
	
	breite=objekt_breite(ob_nr,ob_mem);
	hoehe=objekt_hoehe(ob_nr,ob_mem);

	grow_box(x,y,breite,hoehe);	
	draw_obj(ob_nr,ob_mem,modus,screen,x,y);
}

int draw_obj_center(ob_nr, ob_mem, screen)
int ob_nr;
void *ob_mem;
void *screen;
{
  int breite,hoehe;
  
  clear_screen(screen);
  
  breite=objekt_breite(ob_nr,ob_mem);
  hoehe=objekt_hoehe(ob_nr,ob_mem);

  int x,y;
  draw_shrink_obj(128,shrink_buff,ob_nr,ob_mem,ODER,screen,100,70);
  //draw_obj(ob_nr,ob_mem,ODER,screen,(320-breite)/2,(200-hoehe)/2);
  return (200-hoehe)/2;
}

void draw_obj_all(ob_mem, screen)
void *ob_mem;
void *screen;
{
  int i;
  int y;
  char string[100];
  
  clear_raster();
  set_raster(0,0,leistep);
  show_raster();
  fade_in();
  
  i=0;
  logbase = screen;
  while(objekt_exists(i, ob_mem))
    {
      y = draw_obj_center(i, ob_mem, screen);
      sprintf(string, "OB-NR: %d", i);
      writexy(15,140,y-10,string);
      show_screen(screen);
      delay(2000);
      i++;
    }
}

