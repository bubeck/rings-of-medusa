
#ifndef __X_H
#define __X_H

#include <SDL3/SDL.h>

extern int x_screen_num;             /* synonym for screen_num */
extern int x_depth;                  /* depth of diplay */
//extern Pixmap x_screen1,x_screen2,x_screen3,x_swapscreen;
extern SDL_Surface *logbase;

typedef struct {
  SDL_Surface *surface;

  FLAG mapped;       /* object without a palette already mapped? */
  UWORD pal[16];     /* yes -> with this palette */

  FLAG transparent;  /* was it painted in transparent mode? */

  
} UNIX_OBJECT;

/* This structure is used by the UNIX implementation.
 * Every object is loaded just once in a special memory. Then the address
 * where is *should* be inside Atari is stored in atari_mem.
 */
typedef struct {
  char filename[100];
  void *obj_mem;
  void *atari_mem;
  int len;
  int object_count;
  UNIX_OBJECT objects[100];
} SHARED_OBJECT;

/* number of shared objects: */
#define SH_OBJ_MAX 200

SHARED_OBJECT *search_obj_addr(void *atari_mem);

extern SHARED_OBJECT sh_obj[SH_OBJ_MAX];

void be_2(void *addr);
void be_4(void *addr);

int BlitSurface_INDEX8(SDL_Surface* src, const SDL_Rect* srcrect, 
                       SDL_Surface* dst, SDL_Rect* dstrect, uint8_t transparent_color);

extern int blending;

void fade_in_speed(int speed);
void fade_out_speed(int speed);

#endif
