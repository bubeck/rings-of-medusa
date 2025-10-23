/****************************************************************
 *
 *        unix.c
 *
 *        All special variables, functions for "Return of Medusa"
 *        UNIX-Version.
 *
 * Author: Till Bubeck
 *         Ziegeleistr. 28, 71384 Weinstadt, Fed. Rep. of Germany
 *         EMail: bubeck@peanuts.informatik.uni-tuebingen.de
 *
 ****************************************************************/

#include "includes.c"

#ifdef __has_include
#  if __has_include(<execinfo.h>)
#    include <execinfo.h>
#    define HAVE_EXECINFO 1
#  else
#    define HAVE_EXECINFO 0
#  endif
#else
#  define HAVE_EXECINFO 0
#endif

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

/* pointer to music, not used in unix version */
void *game_music;

/* digisound isn't played now: */
volatile int digi_works=0;

long mem_len;
void *mem_strt;

SDL_Surface *logbase;
volatile int mdeltax;
volatile int steuerzeichen;
volatile long vbl_ct;
volatile Uint64 vbl_last_tick;  /* Wann wurde der letzte VBL ausgeführt? SDL_GetTicks64 */

/* how much percent of performance are used until wait_sync(1)
   measured in vbl_last_tick(). A value of 0 means, that it is ultra
   fast and wait_sync(1) had to wait for full VBL. A value of 100
   means, that wait_sync(1) is exactly reached after 1 VBL. 200 means,
   that it took 2 VBL to reach wait_sync(1). */
volatile int performance_percent; 


int musik_an;


void print_stacktrace() {

#if HAVE_EXECINFO
    void *buffer[100];
    int nptrs = backtrace(buffer, 100);
    char **strings = backtrace_symbols(buffer, nptrs);
    
    if (strings == NULL) {
        perror("backtrace_symbols");
        exit(EXIT_FAILURE);
    }
    
    for (int j = 0; j < nptrs; j++)
        printf("%s\n", strings[j]);
    
    free(strings);
#endif
}

typedef struct {
  char filename[100];
  void *unix_mem;
} unix_mem_t;

#define UNIX_MEM_MAX 100
unix_mem_t unix_mem[UNIX_MEM_MAX];

#define MEM_MAPPER_MAX 100
mem_mapper_t mem_mapper[MEM_MAPPER_MAX];

void map_init()
{
  int i;

  /* Delete previous entries with atari_mem */
  for (i = 0; i < MEM_MAPPER_MAX; i++) {
    mem_mapper[i].atari_mem = NULL;
    mem_mapper[i].local_mem = NULL;
  }

  /* Check, if we already loaded "filename" */
  for (i = 0; i < UNIX_MEM_MAX; i++) {
    unix_mem[i].unix_mem = NULL;
    unix_mem[i].filename[0] = 0;
  }
}
  
void *map_new(char *filename, void *atari_mem)
{
  int i;
  unix_mem_t *unix_p;
  
  /* Delete previous entries with atari_mem */
  for (i = 0; i < MEM_MAPPER_MAX; i++) {
    if (mem_mapper[i].atari_mem == atari_mem) {
      mem_mapper[i].atari_mem = NULL;
    }
  }

  /* Check, if we already loaded "filename" */
  unix_p = NULL;
  for (i = 0; i < UNIX_MEM_MAX; i++) {
    if (strcmp(filename, unix_mem[i].filename) == 0) {
      unix_p = &unix_mem[i];
      break;
    }
  }

  if (unix_p == NULL) {
    for (i = 0; i < UNIX_MEM_MAX; i++) {
      if (unix_mem[i].filename[0] == 0) {
	strcpy(unix_mem[i].filename, filename);
	unix_mem[i].unix_mem = malloc(file_size(filename));
	
	unix_p = &unix_mem[i];
	
      }
    }
  }

  return NULL;
}
