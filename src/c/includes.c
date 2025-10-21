
#include "version.c"

/* Je nach Sprache die entsprechenden Strings einbinden: */
#if (SPRACHE==_DEUTSCH)
  #include "DEUTSCH.C"
#elif (SPRACHE==_ENGLISCH)
  #include "ENGLISCH.C"
#elif (SPRACHE==_FRANZ)
  #include "FRANZ.C"
#endif

#ifndef __TURBOC__              /* Laser_C: */
#include <stdio.h>
#include <osbind.h>
#include <gemdefs.h>
#include <strings.h>
#else
#include <stdio.h>							/* Turbo C: */
#include <tos.h>
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#endif

#include <ctype.h>
#include <math.h>

#include "FILENAME.C"
#include "const.c"              /* Alle Konstanten holen */
#include "grund.c"              /* Konstantendefintion der UntergrÅnde */
#include "typedef.c"            /* Typendefinitionen */
#include "extern.c"
#include "prototype.c"          /* Alle Funktionsprototypen */
#include "mcode.c"							/* Schnittstelle zum Assembler */

extern int handle;              /* Das GEM-Vdi Grafics-handle */

