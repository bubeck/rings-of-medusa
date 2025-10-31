
#include "version.c"

/* Je nach Sprache die entsprechenden Strings einbinden: */
#if (SPRACHE==_DEUTSCH)
  #include "deutsch.c"
#elif (SPRACHE==_ENGLISCH)
  #include "englisch.c"
#elif (SPRACHE==_FRANZ)
  #include "franz.c"
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <signal.h>
#include <setjmp.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>
#include <sys/param.h>
#include <sys/time.h>
#include <errno.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>

#ifndef O_BINARY
#define O_BINARY 0
#endif

#define IS_LITTLE_ENDIAN (*(unsigned char*)&(unsigned int){1} == 1)

#ifdef __TURBOC__
#include <tos.h>
#else
#define cdecl
#endif

#include "unix.h"
#include "filename.c"
#include "const.c"              /* Alle Konstanten holen */
#include "grund.c"              /* Konstantendefintion der Untergründe */
#include "typedef.c"            /* Typendefinitionen */
#include "extern.c"
#include "prototyp.c"           /* Alle Funktionsprototypen */
#include "mcode.c"							/* Schnittstelle zum Assembler */
#include "x.h"
#include "ice.h"

#ifdef _WIN32
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif

extern int handle;              /* Das GEM-Vdi Grafics-handle */

