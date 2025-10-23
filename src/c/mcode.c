
/* Hierbei handelt es sich um das Bindeglied zwischen Assembler und C
	 für Rings of Medusa II von:
	
	 Till Bubeck, Ziegeleistr. 28, 7056 Weinstadt, Tel.: 07151-66437	*/
	
#include "x.h"
#include "extern.c"

/* Zuerst die Prototypen der Assemblerfunktionen:
   Die Funktionen sind auf Grund der Einschränkungen auf max. 8 Zeichen
   durchnumeriert:
   Eine ausführliche Funktionsbeschreibung findet sich in mcode.s,
   dem Assemblersourcecode. */

extern void cdecl mcode1(int x,int y, unsigned char zeichen);
extern void cdecl mcode2(int farbe,int hintergrund,int x,int y,
                unsigned char zeichen);
extern void mcode3(void);
extern void cdecl mcode4(int farbe,int x,int y, unsigned char zeichen);

extern void cdecl mcode7(void *adresse);
extern void mcode8(void);
extern void mcode9(void);
extern void cdecl mcode10(void *adresse);
extern void cdecl mcode11(void *adresse);
extern void cdecl mcode12(void *adresse);
extern void cdecl mcode13(int sync);
extern int mcode14(void);
extern void cdecl mcode15(void *quelle,void *ziel,int anzahl);
extern void cdecl mcode16(void *basis);
extern void cdecl mcode17(void *basis,int x,int y);
extern void *cdecl mcode18(void (*vektor)());
extern void cdecl mcode19(void *basis,void *save,void *sprmem,
											int snr,int shp,int x,int y);
extern void cdecl mcode20(void *save,int spritenr);
extern void cdecl mcode21(void *adr,int bildzeile,int orgzeile);
extern void cdecl mcode22(int quelle,int ziel);
extern void cdecl mcode23(UWORD *pal,int line,int res);
extern void mcode24(void);
extern void mcode25(void);
extern void mcode26(void);
extern long cdecl mcode27(int zeilen,void *quelle,void *ziel);
extern long cdecl mcode28(void *quelle,void *ziel);
extern void cdecl mcode29(int wert);
extern int cdecl mcode30(int land,int x,int y);

extern void cdecl mcode32(void *screen,int y,
                  unsigned char *text,void *font,void *scroll_buff,int copy);
extern void mcode33(void);
extern void mcode34(void);
extern void mcode35(void);
extern void cdecl mcode36(void *adr,int round,int start,int ende);
extern int cdecl mcode37(int disk);
extern void cdecl mcode38(int nr);
/* 39=Convert */

extern int mcode46(void);
extern void cdecl mcode47(int ob_nr,void *ob_basis,int modus,void *scr,int x,int y);
extern void cdecl mcode48(int ob_nr,void *ob_basis,int x1,int y1,int x2,int y2,
													int modus,void *scr,int x,int y);
extern void cdecl mcode49(int x1,int y1,int x2,int y2,void *src_scr,int src_breite,
													int x3,int y3,void *dest_scr,int dest_breite);
extern void cdecl mcode50(int x1,int y1,int x2,int y2,void *src_scr,int src_breite,
													int x3,int y3,void *dest_scr,int dest_breite);
extern void cdecl mcode51(void *scr,int x1,int y1,int x2,int y2);
extern long cdecl mcode52(void *quelle,void *ziel);
extern void cdecl mcode53(int color,int x1,int y1,int x2,int y2);
extern void cdecl mcode54(int color,int x1,int y1,int x2,int y2);
extern void mcode55(void);
extern void mcode56(void);
extern int32_t mcode57(void);
extern int cdecl mcode58(int modus);
extern int cdecl mcode59(int x,int y);
extern UWORD *cdecl mcode60(UWORD *pattern);
extern long cdecl mcode61(int archiv,long offset,long len,void *adr);
extern void mcode62(void);
extern void cdecl mcode63(void *vector,int songnr);
extern int cdecl mcode64(int nr,void *adr);
extern int cdecl mcode65(int nr,void *adr);
extern unsigned int mcode66(void);
extern FLAG mcode67(void);
extern void cdecl mcode68(void *adr,int disk_nr,int strack,int ssektor,int soffset,int dtrack,int dsektor,int doffset);
extern void cdecl mcode69(int disk_nr);
extern int cdecl mcode70(void *adr,int disk_nr,int strack,int ssektor,int soffset,int dtrack,int dsektor,int doffset);
extern void cdecl mcode71(int ob_nr,void *obj, UNIX_OBJECT *uo);
extern void cdecl mcode72(int rasterzeile,int anzahl);
extern void cdecl mcode73(int x,int y);
extern void mcode74(void);
extern void mcode75(void);

/* Jetzt bekommen die Assemblerfunktionen noch richtige Namen, damit
   sie besser anzusprechen sind: */

#define char_fast(x,y,zeichen) mcode1(x,y,zeichen)
#define char_out(farbe,hintergrund,x,y,zeichen) mcode2(farbe,hintergrund,x,y,zeichen)

#define char_trans(farbe,x,y,zeichen) mcode4(farbe,x,y,zeichen)
#define destall_ramdisk() mcode5()

#define exit_vbl(adresse) mcode7(adresse)
#define scrl_li() mcode8()
#define scrl_re() mcode9()
#define scrl_ho(adresse) mcode10(adresse)
#define scrl_ru(adresse) mcode11(adresse)
#define show_screen(adresse) mcode12(adresse)
#define set_sync(wert) mcode13(wert)
#define get_sync() mcode14()
#define copy_zeilen(quelle,ziel,anzahl) mcode15(quelle,ziel,anzahl)
#define undraw_mouse(basis) mcode16(basis)
#define draw_mouse(basis,x,y) mcode17(basis,x,y)
#define init_vbl(adresse) mcode18(adresse)
#define draw_shift(bs,sv,sprmem,snr,shp,x,y) mcode19(bs,sv,sprmem,snr,shp,x,y)
#define undraw_shape(save,sprite_nr) mcode20(save,sprite_nr)

#define copy(quelle,ziel) mcode22(quelle,ziel)
#define set_raster(res,line,pal) mcode23(pal,line,res)
#define clear_raster() mcode24()
#define fade_out() mcode25()
#define fade_in() mcode26()
#define pack(zeilen,quelle,ziel) mcode27(zeilen,quelle,ziel)
#define entpack(quelle,ziel) mcode28(quelle,ziel)
#define set_mouse(wert) mcode29(wert)
#define get_trafficxy(land,x,y) mcode30(land,x,y)

#define init_scroller(scr,y,text,font,scroll_buff,copy) mcode32(scr,y,text,font,scroll_buff,copy)
#define do_scroller() mcode33()
#define hbl_init() mcode34()
#define hbl_exit() mcode35()
#define digi_aus() mcode36(0L,FALSE,0,0)
#define play_digi(adr,round,start,end) mcode36(adr,round,start,end)

#define hippel_fx(nr) mcode38(nr)

#define set_mousepos(x,y) mcode73(x,y)
#define check_protection mcode46
#define draw_obj(ob_nr,ob_basis,modus,scr,x,y) mcode47(ob_nr,ob_basis,modus,scr,x,y)
#define draw_obj_part(ob_nr,ob_basis,x1,y1,x2,y2,modus,scr,x,y) mcode48(ob_nr,ob_basis,x1,y1,x2,y2,modus,scr,x,y)
#define raster_replace(x1,y1,x2,y2,src_scr,src_breite,x3,y3,dest_scr,dest_breite) mcode49(x1,y1,x2,y2,src_scr,src_breite,x3,y3,dest_scr,dest_breite)
#define raster_transp(x1,y1,x2,y2,src_scr,src_breite,x3,y3,dest_scr,dest_breite) mcode50(x1,y1,x2,y2,src_scr,src_breite,x3,y3,dest_scr,dest_breite)
#define mirror(scr,x1,y1,x2,y2) mcode51(scr,x1,y1,x2,y2)
#define ice_unpack(quelle,ziel) mcode52(quelle,ziel)
#define line(color,x1,y1,x2,y2) mcode53(color,x1,y1,x2,y2)
#define fill(color,x1,y1,x2,y2) mcode54(color,x1,y1,x2,y2)
#define calc_raster() mcode55()
#define show_raster() mcode56()
#define get_key() mcode57()
#define line_mode(modus) mcode58(modus)
#define get_pixel(x,y) mcode59(x,y)
#define line_pattern(pattern) mcode60(pattern)
#define loaddisk(archiv,offset,laenge,adr) mcode61(archiv,offset,laenge,adr)
#define vsync() mcode62()
#define play_music(function,nr) mcode63(function,nr)
#define save_data(nr,adr) mcode64(nr,adr)
#define load_data(nr,adr) mcode65(nr,adr)
#define rnd() mcode66()
#define is_key() mcode67()
#define floppy_read(adr,disk,strack,ssektor,soffset,dtrack,dsektor,doffset) mcode68(adr,disk,strack,ssektor,soffset,dtrack,dsektor,doffset)
#define insert_disk(nr) mcode69(nr)
#define floppy_write(adr,disk,strack,ssektor,soffset,dtrack,dsektor,doffset) mcode70(adr,disk,strack,ssektor,soffset,dtrack,dsektor,doffset)
#define convert_objekt(ob_nr,obj,unix_object) mcode71(ob_nr,obj,unix_object)
#define fade_raster(rasterzeile,anzahl) mcode72(rasterzeile,anzahl)
#define mouse_off() mcode74()
#define mouse_on() mcode75()

/* Jetzt kommen die Variablendefinitionen, die in mcode.o definiert werden */

/* Die momentan gedrückte Taste, Scancode+Ascii. Bleibt
 * Solange auf ihrem Wert, bis die Taste losgelassen wurde.
 * Dann wird ihr Wert 0.
 */
extern volatile uint32_t keypress;  

extern volatile long vbl_ct;           /* Wird bei jedem VBL um eins erhöht */
extern volatile Uint64 vbl_last_tick;  /* Wann wurde der letzte VBL ausgeführt? SDL_GetTicks64 */
extern void *mem_strt;				/* Start des freien Speichers */
extern long mem_len;					/* Länge des freien Speichers */
extern void *fast_mem;				/* Nur bei Amiga: Adresse des Fast-Mem */
extern long fast_len;					/* Only Amiga: Länge des Fast_Mem */
extern int tos_da;						/* Flag, ob TOS da ist oder nicht */
extern int musik_an;					/* Flag, ob Musik spielen soll 0=AUS */
extern void *game_music;			/* Zeiger auf den Beginn der Spielmusik */
extern raster_t hbl_system[];    /* Das HBL-System */
extern int chcrypt[];         	/* in Mcode.o zum Überpüfen */
#if 0
extern SDL_Surface *logbase;         /* Log. Bildschirmadresse, es wird immer auf diesen
																		Screen gezeichnet, wenn nicht explizit ein Parameter
																		die Screenadresse angibt. */
#endif
extern volatile int steuerzeichen;				/* Wird im Scroller ein unbekanntes Zeichen gefunden,
																   so wird es hierhin geschrieben */
extern char track_display;		/* Sollen Tracks angeziegt werden? */
extern volatile int mdeltax,mdeltay;		/* Maus Delta Werte */
extern int amiga_raw;
																   
