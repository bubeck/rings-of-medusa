/****************************************************************
 *
 *        x.c
 *
 *        all changes for "Return of Medusa" to use X11R5 for
 *        painting of grafics.
 *
 * Author: Till Bubeck
 *         Ziegeleistr. 28, 71384 Weinstadt, Fed. Rep. of Germany
 *         EMail: bubeck@peanuts.informatik.uni-tuebingen.de
 *
 ****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <math.h>
#include <assert.h>
#include <sys/param.h>
#include <getopt.h>  
#include <time.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>

#define cdecl

#include "includes.c"
#include "extern.c"

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture* screen_texture;
SDL_Surface* screen_surface;
SDL_Surface* screen_surface_faded;

/* 0=normal picture, 7=dark picture */
int fading = 0;

/* 255=normal picture, 0=dark */
int blending = 255;

static int choosen_linemode=0;
static UWORD *active_pattern=NULL;



int screen_num;
int x_screen_num;             /* synonym for screen_num */

int x_depth;                  /* depth of diplay */

SDL_Surface *sdl_screen1,*sdl_screen2,*sdl_screen3,*sdl_screen4,*sdl_swapscreen;
SDL_Surface *font_surface;
//Pixmap x_swapscreen;

unsigned int display_width, display_height;

/* screen currently shown */
//Pixmap exposure_screen;

/* here are all keypresses stored: */
struct {
  int32_t buf[20];      // upper 16 bit = scancode, lower 16 bit = ASCII
  int write;   // points into buf where the next key will be stored
  int read;    // points into buf, where we must read if write!=read.
} keybuffer;

volatile uint32_t keypress;      // upper 16 bit = scancode, lower 16 bit = ASCII

SHARED_OBJECT sh_obj[SH_OBJ_MAX];

#define VBL_QUEUE_SIZE 10

/* Array für die VBL-Queue */
static void (*vbl_queue[VBL_QUEUE_SIZE])() = {NULL};

/**********************************************************************
 * Medusa specific globals:
 */

/** How do rasters work? "rasters" is a pointer to raster1 or raster2.
    It is used by the VBL to display the current rasters.

    On Atari raster1 and raster2 do not have absolute Y values, but
    offsets to the next raster line. On other systems, we have
    absolute Y in raster1/2 as well as in hbl_system.

    We do not modify raster1/2 while being displayed, so typically the
    raster change is build in raster2 and after 1 VBL, raster points
    to raster2. 1 VBL later back to raster1.

    raster2 is build by mcode55 from hbl_system. So "set_raster" works
    on hbl_system. hbl_system stores absolute Y values (instead of
    offsets as raster1 and raster2) and when everything is ready, the
    call to mcode55 sorts and rebuilds hbl_system to raster2. It is
    shown as described above. */
    
#define ANZ_RASTER 50
raster_t hbl_system[ANZ_RASTER];       /* Hier werden alle aktiven Raster gespeichert */
raster_t rasters1[ANZ_RASTER];
raster_t rasters2[ANZ_RASTER];
raster_t *rasters;                 /* either rasters1 or rasters2 */

UWORD *search_pal(int y);
void set_surface_PAL(SDL_Surface *surface, RGB_PAL pal);
static bool init_digi_audio(void);

// How many bits are used on the target system for a RGB channel?
#define COLOR_WIDTH 8

// How much must we shift atari color (3 bit to left) for target color?
#define COLOR_SHIFT (COLOR_WIDTH-3)

void screenshot();

int convert_3bit_to_8bit(int value) {
    if (value < 0 || value > 7) {
        return 0;
    }
    return (value * 255) / 7;
}

void atari_color_to_rgb(UWORD color, uint8_t *r, uint8_t *g, uint8_t *b)
{
  *r = (uint8_t)convert_3bit_to_8bit((color >> 8) & 0b111);
  *g = (uint8_t)convert_3bit_to_8bit((color >> 4) & 0b111);
  *b = (uint8_t)convert_3bit_to_8bit((color >> 0) & 0b111);
}

void fade_palette(UWORD *pal, int value)
{
  int i;
  int r,g,b;
  
  for(i=0;i<16;i++) {
    r = (pal[i] >> 8) & 0b111;
    g = (pal[i] >> 4) & 0b111;
    b = (pal[i] >> 0) & 0b111;
    r -= value; if (r<0) r=0;
    g -= value; if (g<0) g=0;
    b -= value; if (b<0) b=0;
    pal[i] = (r << 8) | (g << 4) | (b << 0);
  }
}

UWORD rgb_to_atari_color(uint8_t r, uint8_t g, uint8_t b)
{
  assert(0); // Fehlt * 255/7
  return (r & 0x111) << 8 | (g & 0x111) << 4 | (b & 0b111);
}

void atari_palette_to_colors(RGB_PAL atari_pal, SDL_Color *sdl_pal)
{
  uint8_t r, g, b;
  
  for (int i = 0; i < 16; i++)
    {
      atari_color_to_rgb(atari_pal[i], &r, &g, &b);
      sdl_pal[i].r = r;
      sdl_pal[i].g = g;
      sdl_pal[i].b = b;
      if (i == 0)
	sdl_pal[i].a = 0;
      else
	sdl_pal[i].a = 255;
    }
}

void rst2rst(raster_t *src, raster_t *dst)
{
  for(int i = 0; i < ANZ_RASTER; i++)
    *dst++ = *src++;
}

void rst12rst2()
{
  rst2rst(rasters1, rasters2);
}

void rst22rst1()
{
  rst2rst(rasters2, rasters1);
}

void print_rasters(void)
{
  int i,j;
  raster_t tmp;

  for(i=0;i<ANZ_RASTER;i++)
    if (hbl_system[i].y!=255)
      {
	printf("RASTER Y=%03d(i=%02d)", hbl_system[i].y, i);
	for (j=0; j<16;j++) {
	  printf(" 0x%03x", hbl_system[i].pal[j]);
	}
	printf("\n");
      }
  
}

void set_surface_PAL(SDL_Surface *surface, RGB_PAL pal)
{
  uint8_t r, g, b;
  SDL_Palette *palette = SDL_GetSurfacePalette(surface);

  if (palette == NULL) {
    palette = SDL_CreateSurfacePalette(surface);
  }
  
  if (palette && palette->ncolors >= 16) {
    SDL_Color colors[16];
    for (int i = 0; i < 16; i++) {
      atari_color_to_rgb(pal[i], &r, &g, &b);
      colors[i].r = r;
      colors[i].g = g;
      colors[i].b = b;
      colors[i].a = (i == 0? 0 : 255);
      // printf("set_surface_PAL[%d] = 0x%03x %d,%d,%d,%d\n", i, pal[i], colors[i].r, colors[i].g, colors[i].b, colors[i].a);
    }
    SDL_SetPaletteColors(palette, colors, 0, 16);
    SDL_SetSurfacePalette(surface, palette);
  } else {
    printf("palette=NULL %p\n", palette);
    print_stacktrace();
    assert(0);
    exit(1);
  }
}

void print_palette(RGB_PAL pal)
{
  printf("pal = { ");
  for (int i=0; i<16; i++) 
    printf(" 0x%03x", pal[i]);
  printf("}");
}

void mcode1(int x, int y, unsigned char zeichen)
{
  /* Schreibt Zeichen in Low-Res auf den Bildschirm an Position X/Y,
   * in der Farbe 15.
   */
  
  mcode2(15,0,x,y,zeichen);
}
	
void mcode2(int farbe, int hintergrund, int x, int y, unsigned char zeichen)
{
  /* Gibt ein Zeichen aus, "uberschreibt aber den Hintergrund */

  /* Hintergrund ausmalen: */
  fill(hintergrund,x,y,x+4,y+5);
  mcode4(farbe,x,y,zeichen);
}

// Schreibt Zeichen auf den Bildschirm (logbase) an Position X/Y.
// Dabei scheint durch die Buchstaben der Hintergrund durch (Transparent)
void mcode4(int farbe, int x, int y, unsigned char zeichen)
{
  /* Gibt ein Zeichen aus, der Hintergrund bleibt transparent */
  UWORD *pal;
  uint8_t r, g, b;

  if (zeichen == 0xdc) zeichen = '#';      // Ueh
  if (zeichen == 0xc4) zeichen = '&';      // Aeh
  if (zeichen == 0xd6) zeichen = '$';      // Oeh

  if (zeichen < 32) zeichen = 'X';
  if (zeichen > '_') zeichen = 'Y';

  zeichen -= 32;

#if 0
  pal=search_pal(y);
  atari_color_to_rgb(pal[farbe], &r, &g, &b);

  r = g = b = 255;
#endif
  
  SDL_Rect src = {4 * zeichen, 0, 4, 5};         // Bereich aus der Textur
  SDL_Rect dst = {x, y, 4, 5};           // Bildschirmziel

  // set correct color for font pixels
  Uint8* pixels_src = (Uint8*)font_surface->pixels;
  Uint8* pixels_dst = (Uint8*)logbase->pixels;
  for (int y = 0; y < src.h; y++) {
    for (int x = 0; x < src.w; x++) {
      if (pixels_src[(src.y + y) * font_surface->pitch + (src.x + x)] != 0) {
	pixels_dst[(dst.y + y) * logbase->pitch + (dst.x + x)] = farbe;
      }
    }
  }
  
  // SDL_SetRenderDrawColor(renderer, r, g, b, 255);

  // Ziel-Texture als Render-Target setzen
  //SDL_SetRenderTarget(renderer, logbase);
    
  //SDL_SetTextureColorMod(font_texture, r, g, b);
  //SDL_RenderCopy(renderer, font_texture, &src, &dst);
  
  //SDL_BlitSurface(font_surface, &src, logbase, &dst);
  //BlitSurface_INDEX8(font_surface, &src, logbase, &dst, 0);   // transparent_color = 0
 
  //XSetForeground(x_display,x_gc,pal[farbe]);
  //XDrawString(x_display,*logbase,x_gc,x*2+1,y*2+9,text,1);
}

uint16_t fnt_buf_be[] = {  
  //
  //  NEOchrome V2.16 cut buffer contents (left justified):
  //  by Chaos, Inc. of the Delta Force (member of The Union)
  //
  //    pixels/scanline    = $0140 (bytes/scanline: $0028)
  //  # scanlines (height) = $0005
  //
  //  Monochrome mask (1 plane; background=0/non-background=1)
  //
  0x0255,0x5452,0x2250,0x0001,0x2166,0x4727,0x2200,0x1046,
  0x7263,0x6773,0x5215,0x4562,0x6263,0x7555,0x5572,0x4400,
  0x0008,0x0000,0x0000,0x0000,0x0250,0x2122,0x4122,0x0001,
  0x5311,0x5441,0x5522,0x2721,0x7554,0x5444,0x5215,0x4755,
  0x5554,0x2555,0x5515,0x2844,0x0814,0x0000,0x0000,0x0000,
  0x0205,0x5250,0x4177,0x2702,0x5122,0x7662,0x2300,0x4012,
  0x7764,0x5665,0x7216,0x4555,0x6562,0x2555,0x2222,0x5428,
  0x142a,0x0000,0x0000,0x0000,0x0005,0x5470,0x4122,0x2004,
  0x5141,0x1152,0x5122,0x2720,0x7554,0x5445,0x5255,0x4555,
  0x4551,0x2557,0x5240,0x2810,0x2214,0x0000,0x0000,0x0000,
  0x0202,0x2150,0x2250,0x4024,0x2176,0x1622,0x2604,0x1042,
  0x7563,0x6743,0x5225,0x7552,0x4356,0x2725,0x5270,0x1000,
  0x0022,0x0000,0x0000,0x0000 };

uint16_t swap_uint16(uint16_t val) {
    return (val >> 8) | (val << 8);
}

uint32_t swap_uint32(uint32_t val) {
    return ((val >> 24) & 0x000000FF) |
           ((val >> 8)  & 0x0000FF00) |
           ((val << 8)  & 0x00FF0000) |
           ((val << 24) & 0xFF000000);
}

void be_4(void *adr)
{
  uint8_t *p;
  uint8_t tmp;

  if (IS_LITTLE_ENDIAN) {
    p = adr;
    tmp = *p;
    *p = *(p+3);
    *(p+3) = tmp;
    be_2(p+1);
  }
}

void be_2(void *adr)
{
  uint8_t *p;
  uint8_t tmp;

  if (IS_LITTLE_ENDIAN) {
    p = adr;
    tmp = *p;
    *p = *(p+1);
    *(p+1) = tmp;
  }
}

uint16_t convert_from_be_2(uint16_t be)
{
  if (IS_LITTLE_ENDIAN) {
    return swap_uint16(be);
  } else {
    return be;
  }
}

uint32_t convert_from_be_4(uint32_t be)
{
  if (IS_LITTLE_ENDIAN) {
    return swap_uint32(be);
  } else {
    return be;
  }
}

void convert_be_to_le(uint16_t *src, uint16_t *dst, size_t nbytes)
{
  nbytes /= 2;
  while(nbytes > 0) {
    *dst = swap_uint16(*src);
    dst++;
    src++;
    nbytes--;
  }
}

SDL_Surface *font_prepare(uint16_t *font)
{
  uint8_t value;
  uint8_t *font8;
  int pitch;
  SDL_Surface *surface;
  uint16_t fnt_buf[sizeof(fnt_buf_be)/2]; 
  RGB_PAL pal;
  
  convert_be_to_le(font, fnt_buf, sizeof(fnt_buf_be));

  // surface = SDL_CreateRGBSurfaceWithFormat(0, 0x140, 5, 8, SDL_PIXELFORMAT_INDEX8);
  surface = SDL_CreateSurface(0x140, 5, SDL_PIXELFORMAT_INDEX8);

  pal[0] = 0;
  pal[1] = 0x777;
  //set_surface_PAL(surface, pal);
  set_surface_PAL(surface, leistep);
  
  Uint8* pixels = (Uint8*)surface->pixels;
  font8 = (uint8_t *)fnt_buf;
  for (int y = 0; y < 5; y++) {
    for (int x = 0; x < 0x140; x++) {
      value = font8[y * 0x28 + x / 8];
      int b = 7 - x % 8;
      uint8_t pixel = (value >> b) & 0x01;
      if (pixel) {
	pixels[y * surface->pitch + x] = 1;
      } else {
	pixels[y * surface->pitch + x] = 0;
      }
    }
  }
  
  return surface;
}

#define TOO_SMALL 0
#define BIG_ENOUGH 1

void ask_X(void)
{
  /* fragt beim X nach, was es Neues gibt. Diese Funktion wird ab und zu
   * angesprungen, und soll den Kontakt zum X Server halten, bzw. auf
   * Events reagieren.
   */
  int root_x_return,root_y_return,win_x_return,win_y_return;
  unsigned int mask_return;
  char key;
  char buffer[20];
  int width, height;
  int newpos;
  
#if 0
  fprintf(stderr,"asking X\n");
#endif

  //vbl_ct = get_vbl_ct();
  
  float x, y;
  uint32_t buttons = SDL_GetMouseState(&x, &y);
  //printf("Mausposition: %f, %f %d\n", x, y, buttons);
  mousex = (int)(x / gfx_scale_factor);
  mousey = (int)(y / gfx_scale_factor);
  if (buttons == 4) buttons = 2;           // rechte Maus hat 4 bei SDL.
  mousek = buttons;

  SDL_Event event;
  
  while (SDL_PollEvent(&event)) {
    //printf("SDL_PollEvent(%d)\n", event.type);
    if (event.type == SDL_EVENT_QUIT)
      exit(1);
    if (event.type == SDL_EVENT_KEY_UP) {
      const char *keysym;
      keysym = SDL_GetKeyName(event.key.key);
      key = keysym[0];
      //printf("KEY_UP %c\n", key);
      keypress = 0;
    }
    if (event.type == SDL_EVENT_KEY_DOWN) {
      const char *keysym;
      SDL_Keycode keycode = event.key.key;

      keypress = 0;
      switch(keycode) {
      case SDLK_UP:
	keypress = (ATARI_SCAN_UP << 16);
	break;
      case SDLK_BACKSPACE:
	keypress = 8;
	break;
      case SDLK_DELETE:
	keypress = 127;
	break;
      case SDLK_RETURN:
	keypress = 13;
	break;
      case SDLK_HOME:
	screenshot();
	break;
      case SDLK_SYSREQ:
      case SDLK_F10:
	printf("HELP\n");
	keypress = (ATARI_SCAN_HELP << 16);    // Help
	break;
      case SDLK_DOWN:
	keypress = (ATARI_SCAN_DOWN << 16);
	break;
      case SDLK_LEFT:
	keypress = (ATARI_SCAN_LEFT << 16);
	break;
      case SDLK_RIGHT:
	keypress = (ATARI_SCAN_RIGHT << 16);
	break;
      default:
	keysym = SDL_GetKeyName(event.key.key);
	key = keysym[0];
	//printf("KEY_DOWN %c\n", key);
	keypress = key;
	break;
      }
      
	{
	  //printf("WRITE keybuffer[%d]=0x%02x,%c\n", keybuffer.write, (keypress>>16), keypress & 255);
	  keybuffer.buf[keybuffer.write]=keypress;
	  newpos = keybuffer.write + 1;
	  if (newpos>=sizeof(keybuffer.buf)/sizeof(keybuffer.buf[0]))
	    newpos=0;
	  if (newpos!=keybuffer.read)
	    keybuffer.write = newpos;
	}
    }
  }

#if 0
  while (XCheckMaskEvent(x_display, ExposureMask | 
		       KeyPressMask | ButtonPressMask | ButtonReleaseMask | 
		       StructureNotifyMask, &report))
    switch  (report.type) {
    case Expose:
      /* unless this is the last contiguous expose,
       * don't draw the window */
      if (report.xexpose.count != 0)
	break;
      
      /* place graphics in window, */
      show_screen(scr2);
      break;
    case ConfigureNotify:
      /* window has been resized, change width and
       * height to send to draw_text and draw_graphics
       * in next Expose */
      width = report.xconfigure.width;
      height = report.xconfigure.height;
      if (width != 640 || height != 400)
	{ fprintf(stderr,"Window must have size 640x400!\n"); exit(-1); }
      break;
    case ButtonPress:
      /* trickle down into KeyPress (no break) */
      mousek|=report.xbutton.button & 3;
      mousex=report.xbutton.x/2;
      mousey=report.xbutton.y/2;
      break;
    case ButtonRelease:
      mousek&=~ (report.xbutton.button & 3);
      mousex=report.xbutton.x/2;
      mousey=report.xbutton.y/2;
      break;
    case KeyPress:
      if (XLookupString(&report.xkey,buffer,sizeof(buffer),&key,&compose)>0)
	{
	  newpos=keybuffer.write;
	  if (newpos++>=sizeof(keybuffer.buf))
	    newpos=0;
	  if (newpos!=keybuffer.read)
	    {
	      keybuffer.buf[keybuffer.write]=key;
	      keybuffer.write=newpos;
	    }
	}
      break;
      XUnloadFont(x_display, x_font->fid);
      XFreeGC(x_display, x_gc);
      XCloseDisplay(x_display);
      exit(1);
    default:
      /* all events selected by StructureNotifyMask
       * except ConfigureNotify are thrown away here,
       * since nothing is done with them */
      break;
    } /* end switch */
#endif
}

void copyTextureRegion(SDL_Texture* source, SDL_Texture* destination, 
		       SDL_FRect* src_rect, SDL_FRect* dst_rect) {
    // Ziel-Texture als Render-Target setzen
    SDL_SetRenderTarget(renderer, destination);
    
    // Nur den gewünschten Bereich kopieren
    SDL_RenderTexture(renderer, source, src_rect, dst_rect);
    
    // Zurück zum Standard-Target
    SDL_SetRenderTarget(renderer, NULL);
}

// 20ms warten = 50 Hz
#define WAIT_TICKS 20

int vbl_thread_function(void *data) {
  while(1) {
    Uint64 ticks = SDL_GetTicks();
    int duration = ticks - vbl_last_tick;
    
    vbl_last_tick = ticks;
    vbl_ct++;
    vbl_execute();

    // Compensate, if the timer is not accurate:
    int wait = WAIT_TICKS - (duration-WAIT_TICKS);
    if (wait > 0) {
      assert(wait < 5000);
      SDL_Delay(wait);
    }
  }
    
  return 0; // Return-Wert des Threads
}

// Audio-Manager Struktur
typedef struct {
    SDL_AudioDeviceID device;
    SDL_AudioStream* stream;
    bool is_playing;
    bool is_initialized;
} AudioManager;

// Globaler Audio-Manager
AudioManager g_audio = {0};

// Audio-System initialisieren
bool audio_init() {
    if (g_audio.is_initialized) return true;
    
    // Audio-Spec definieren
    SDL_AudioSpec spec = {
        .format = SDL_AUDIO_F32,
        .channels = 2,
        .freq = 44100
    };
    
    // Audio-Device öffnen
    g_audio.device = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec);
    if (g_audio.device == 0) {
        printf("Audio-Device Fehler: %s\n", SDL_GetError());
        return false;
    }
    
    g_audio.is_initialized = true;

    init_digi_audio();
    
    return true;
}

// WAV laden und abspielen
bool play_audio(char *filename) {
    if (!g_audio.is_initialized) {
        printf("Audio nicht initialisiert!\n");
        return false;
    }
    
    // Vorherige Wiedergabe stoppen
    if (g_audio.stream) {
        SDL_DestroyAudioStream(g_audio.stream);
        g_audio.stream = NULL;
    }
    
    // WAV-Datei laden
    SDL_AudioSpec file_spec;
    Uint8* audio_buf;
    Uint32 audio_len;
    char filename2[200];

    create_data_filename(filename, filename2);
    
    if (!SDL_LoadWAV(filename2, &file_spec, &audio_buf, &audio_len)) {
      printf("WAV Ladefehler: %s %s\n", filename2, SDL_GetError());
      return false;
    }
    
    //printf("WAV geladen: %s (%d bytes)\n", filename2, audio_len);
    
    // Audio-Stream erstellen
    SDL_AudioSpec output_spec = {
        .format = SDL_AUDIO_F32,
        .channels = 2,
        .freq = 44100
    };
    
    g_audio.stream = SDL_CreateAudioStream(&file_spec, &output_spec);
    if (!g_audio.stream) {
        printf("Stream-Fehler: %s\n", SDL_GetError());
        SDL_free(audio_buf);
        return false;
    }
    
    // Stream mit Device verbinden
    SDL_BindAudioStream(g_audio.device, g_audio.stream);
    
    // Audio-Daten in Stream laden
    SDL_PutAudioStreamData(g_audio.stream, audio_buf, audio_len);
    SDL_FlushAudioStream(g_audio.stream);
    
    // Wiedergabe starten
    SDL_ResumeAudioDevice(g_audio.device);
    g_audio.is_playing = true;
    
    // Buffer freigeben
    SDL_free(audio_buf);
    
    return true;
}

// Audio-Status prüfen
bool is_audio_playing() {
    if (!g_audio.stream) return false;
    return SDL_GetAudioStreamQueued(g_audio.stream) > 0;
}

// Audio stoppen
void stop_audio() {
    if (g_audio.stream) {
        SDL_PauseAudioDevice(g_audio.device);
        SDL_ClearAudioStream(g_audio.stream);
        g_audio.is_playing = false;
    }
}

// Audio pausieren/fortsetzen
void toggle_audio() {
    if (g_audio.is_playing) {
        SDL_PauseAudioDevice(g_audio.device);
        g_audio.is_playing = false;
    } else {
        SDL_ResumeAudioDevice(g_audio.device);
        g_audio.is_playing = true;
    }
}

// Volume setzen (0.0 - 1.0)
void set_volume(float volume) {
    if (g_audio.stream) {
        SDL_SetAudioStreamGain(g_audio.stream, volume);
    }
}

// Audio-System beenden
void audio_cleanup() {
    if (g_audio.stream) {
        SDL_DestroyAudioStream(g_audio.stream);
        g_audio.stream = NULL;
    }
    if (g_audio.device) {
        SDL_CloseAudioDevice(g_audio.device);
        g_audio.device = 0;
    }
    g_audio.is_initialized = false;
    g_audio.is_playing = false;
}

void release_x(void)
{
  //SDL_CaptureMouse(false);
  //SDL_SetWindowMouseGrab(window, false);
  //SDL_SetWindowRelativeMouseMode(window, true);
    printf("Release mouse\n");
    sleep(2);
}

void mcode3(void)
{
  /*
   * initializes hardware registers on Atari/Amiga.
   * Can be used to initialize whatever the programmer like to. Is called
   * before any call to a grafix primitive.
   */
  unsigned int border_width = 4;  /* four pixels */
  int x, y;                       /* window position */
  unsigned int width, height;     /* window size */
  char *window_name = "Return of Medusa";
  char *icon_name = "RoM";
#if 0
  static Pixmap screen1,screen2;
#endif
  int i;

    /* Note that in a real application, x and y would default to 0
   * but would be settable from the command line or resource database.
   */
  x=y=0;
  width=320; height=200;

  keybuffer.write = keybuffer.read = 0;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
 
    audio_init();

    vbl_last_tick = SDL_GetTicks();
    SDL_Thread *thread = SDL_CreateThread(vbl_thread_function, "VBLThread", NULL);
	
    window = SDL_CreateWindow(window_name, width*gfx_scale_factor, height*gfx_scale_factor, 0);
    SDL_SetWindowResizable(window, true);

    SDL_CaptureMouse(false);
    //SDL_SetRelativeMouseMode(false);
    SDL_ShowCursor();
    //SDL_SetHint(SDL_HINT_GRAB_KEYBOARD, "0");
    // Oder explizit:
    //SDL_SetWindowGrab(window, false);
    if (window) {
      SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    }
    renderer = SDL_CreateRenderer(window, NULL);

    SHARED_OBJECT *sh_obj;
    void *icon = malloc(6000);
    load_objekte("reagenz.obj", icon);
    sh_obj=search_obj_addr(icon);
    SDL_SetWindowIcon(window, sh_obj->objects[3].surface);   // Objekt 3=Totenkopf
      
    // Bildschirm löschen und mit schwarzem Hintergrund füllen
    //SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Farbe für den Pixel setzen (Rot)
    //SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    //SDL_RenderDrawPoint(renderer, 400, 300);  // Pixel in der Mitte

    //SDL_RenderPresent(renderer);

    //SDL_Delay(3000); // 3 Sekunden warten

    //SDL_DestroyRenderer(renderer);
    //SDL_DestroyWindow(window);
    //SDL_Quit();

    screen_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width * gfx_scale_factor, height * gfx_scale_factor);
    SDL_SetTextureScaleMode(screen_texture, SDL_SCALEMODE_NEAREST);
    SDL_SetTextureBlendMode(screen_texture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(screen_texture, 255);
    
    screen_surface = SDL_CreateSurface(width * gfx_scale_factor, height * gfx_scale_factor, SDL_PIXELFORMAT_RGBA8888);
    screen_surface_faded = SDL_CreateSurface(width * gfx_scale_factor, height * gfx_scale_factor, SDL_PIXELFORMAT_RGBA8888);

    font_surface = font_prepare(fnt_buf_be);

#if 0
    //SDL_RenderClear(renderer);
    SDL_FRect src = {0, 0, 320, 5};         // Bereich aus der Textur
    SDL_FRect dst = {0, 0, 320, 5};     // Bildschirmziel, doppelt so groß

    SDL_Texture* font_texture = SDL_CreateTextureFromSurface(renderer, font_surface);

    SDL_RenderTexture(renderer, font_texture, &src, &dst);
    SDL_RenderPresent(renderer);
    SDL_Delay(3000); // 3 Sekunden warten
#endif
    
    // load_font(&x_font);
  
    //sdl_screen1 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    sdl_screen1 = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_INDEX8);
    sdl_screen2 = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_INDEX8);
    sdl_screen3 = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_INDEX8);
    sdl_screen4 = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_INDEX8);
    shrink_buff = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_INDEX8);

  /* See mcode12: By calling mcode12(scr1), ATARI will show this
     screen physically. The second screen ist scr2. Typically
     "logbase" will have the same address as scr2 and all drawing
     routines will draw to logbase. This means, they will draw to
     scr2.  If drawing is complete, the main loop will call
     "swap_screens". That will swap scr1 and scr2 and again show scr1
     (which was scr2 previously).

     SDL allows to draw to a background buffer, that will be shown by
     "SDL_RenderPresent(renderer);". This could be used instead of the
     above two screen technique. However, the code assumes, that scr2
     has the content of scr1 after "swap_screens" which is not true
     for this.

     So we allocate two textures: scr1 and scr2. logbase is another
     pointer which normally points to scr2 and is used by all drawing
     routines. So we draw into scr2. "mcode12" will then copy the
     given texture to the window and show it. This is most close to
     Atari behaviour.
  */

     
  scr1=logbase=sdl_screen1;
  scr2=sdl_screen2;
  hlpbuf=sdl_screen3;
  window_back=sdl_screen4;

  set_surface_PAL(scr1, leistep);
  set_surface_PAL(scr2, leistep);
  set_surface_PAL(hlpbuf, leistep);
  set_surface_PAL(window_back, leistep);
  

#if 0
  printf("HELLO\n");
  writexy_trans(2, 10, 10, "HELLO");
  set_pixel_surface(logbase, 100,100, 15);
  fill(15, 50, 50, 60, 60);
  show_screen(logbase);
  SDL_Delay(3000); // 3 Sekunden warten

#endif

  return;

#if 0
  /* get events, use first to display text and graphics */
  while (1)  {
    XNextEvent(display, &report);
    switch  (report.type) {
    case Expose:
      /* unless this is the last contiguous expose,
       * don't draw the window */
      if (report.xexpose.count != 0)
	break;
      
      /* if window too small to use */
      if (window_size == TOO_SMALL)
	TooSmall(win, gc, x_font);
      else {
	/* place graphics in window, */
	draw_graphics(win, gc, width, height);
      }
      break;
    case ConfigureNotify:
      /* window has been resized, change width and
       * height to send to draw_text and draw_graphics
       * in next Expose */
      width = report.xconfigure.width;
      height = report.xconfigure.height;
      if ((width < size_hints.min_width) ||
	  (height < size_hints.min_height))
	window_size = TOO_SMALL;
      else
	window_size = BIG_ENOUGH;
      break;
    case ButtonPress:
      /* trickle down into KeyPress (no break) */
    case KeyPress:
      XUnloadFont(display, x_font->fid);
      XFreeGC(display, gc);
      XCloseDisplay(display);
      exit(1);
    default:
      /* all events selected by StructureNotifyMask
       * except ConfigureNotify are thrown away here,
       * since nothing is done with them */
      break;
    } /* end switch */
  } /* end while */
#endif
}

void mcode7(void *adr)
{
  /* Entfernt eine VBL-Routine aus der VBL-Queue */
  if (adr == NULL) {
    return;
  }
    
  /* Handle in Array-Index umwandeln */
  int index = (int)((intptr_t)adr - 1);
    
  /* Gültigkeit des Index prüfen */
  if (index >= 0 && index < VBL_QUEUE_SIZE) {
    vbl_queue[index] = NULL;
  }
}

/* vbl_execute: Iteriert über alle Einträge und ruft die Funktionen auf */
void vbl_execute()
{
    for (int i = 0; i < VBL_QUEUE_SIZE; i++) {
        if (vbl_queue[i] != NULL) {
	  //printf("vbl_execute(%p)\n", vbl_queue[i]);
	  vbl_queue[i]();
	  //printf("vbl_execute(%p) done\n", vbl_queue[i]);
        }
    }
}

void mcode8(void)
{
  /* scroll left */
}

void mcode9(void)
{
  /* scroll right */
}

void mcode10(void *adr)
{
  /* scroll up */
}

void mcode11(void *adr)
{
  /* scroll down */
}

static SDL_Surface *last_screen = NULL;

void show_last_screen()
{
  void *logbase_save;

  logbase_save = logbase;

  // mcode12 sets logbase to "last_screen".
  mcode12(last_screen);
  
  logbase = logbase_save;
}

int BlitSurface_INDEX8(SDL_Surface* src, const SDL_Rect* srcrect, 
                       SDL_Surface* dst, SDL_Rect* dstrect, uint8_t transparent_color) {
    // Null-Pointer checks
    if (!src || !dst) {
      printf("NULL surface pointer\n");
      exit(1);
      return SDL_SetError("Null surface pointer");
    }
    
    // Format validation
    if (src->format != SDL_PIXELFORMAT_INDEX8 || 
        dst->format != SDL_PIXELFORMAT_INDEX8) {
        return SDL_SetError("Surfaces must be INDEX8 format");
    }
    
    // Source rectangle setup
    SDL_Rect src_rect;
    if (srcrect) {
        src_rect = *srcrect;
    } else {
        src_rect.x = 0;
        src_rect.y = 0;
        src_rect.w = src->w;
        src_rect.h = src->h;
    }
    
    // Destination rectangle setup
    SDL_Rect dst_rect;
    if (dstrect) {
        dst_rect = *dstrect;
    } else {
        dst_rect.x = 0;
        dst_rect.y = 0;
        dst_rect.w = src_rect.w;
        dst_rect.h = src_rect.h;
    }
    
    // Clipping gegen Source-Surface
    if (src_rect.x < 0) {
        dst_rect.x -= src_rect.x;
        src_rect.w += src_rect.x;
        src_rect.x = 0;
    }
    if (src_rect.y < 0) {
        dst_rect.y -= src_rect.y;
        src_rect.h += src_rect.y;
        src_rect.y = 0;
    }
    if (src_rect.x + src_rect.w > src->w) {
        src_rect.w = src->w - src_rect.x;
    }
    if (src_rect.y + src_rect.h > src->h) {
        src_rect.h = src->h - src_rect.y;
    }
    
    // Clipping gegen Destination-Surface
    if (dst_rect.x < 0) {
        src_rect.x -= dst_rect.x;
        src_rect.w += dst_rect.x;
        dst_rect.x = 0;
    }
    if (dst_rect.y < 0) {
        src_rect.y -= dst_rect.y;
        src_rect.h += dst_rect.y;
        dst_rect.y = 0;
    }
    if (dst_rect.x + src_rect.w > dst->w) {
        src_rect.w = dst->w - dst_rect.x;
    }
    if (dst_rect.y + src_rect.h > dst->h) {
        src_rect.h = dst->h - dst_rect.y;
    }
    
    // Prüfen ob noch etwas zu kopieren ist
    if (src_rect.w <= 0 || src_rect.h <= 0) {
        return 0; // Nichts zu kopieren, aber kein Fehler
    }
    
    // Surface-Lock falls nötig
    bool src_locked = false, dst_locked = false;
    if (SDL_MUSTLOCK(src)) {
        if (!SDL_LockSurface(src)) {
            return -1;
        }
        src_locked = true;
    }
    if (SDL_MUSTLOCK(dst)) {
        if (!SDL_LockSurface(dst)) {
            if (src_locked) SDL_UnlockSurface(src);
            return -1;
        }
        dst_locked = true;
    }
    
    //printf("Blit: %d,%d %d/%d -> %d,%d\n", src_rect.x, src_rect.y, src_rect.w, src_rect.h, dst_rect.x, dst_rect.y);
      
    // Pixel-Daten Setup
    uint8_t* src_pixels = (uint8_t*)src->pixels;
    uint8_t* dst_pixels = (uint8_t*)dst->pixels;
    
    int src_pitch = src->pitch;
    int dst_pitch = dst->pitch;
    
    // Start-Adressen berechnen
    uint8_t* src_ptr = src_pixels + (src_rect.y * src_pitch) + src_rect.x;
    uint8_t* dst_ptr = dst_pixels + (dst_rect.y * dst_pitch) + dst_rect.x;
    
    // Pixel-für-Pixel kopieren mit Transparenz-Check
    int copy_width = src_rect.w;
    int copy_height = src_rect.h;
    
    for (int y = 0; y < copy_height; y++) {
        uint8_t* src_row = src_ptr + (y * src_pitch);
        uint8_t* dst_row = dst_ptr + (y * dst_pitch);
        
        for (int x = 0; x < copy_width; x++) {
            uint8_t pixel = src_row[x];
            if (pixel != transparent_color) {
                dst_row[x] = pixel;
            }
        }
    }
    
    // Surface-Unlock
    if (dst_locked) SDL_UnlockSurface(dst);
    if (src_locked) SDL_UnlockSurface(src);
    
    // Destination rectangle zurückgeben
    if (dstrect) {
        dstrect->x = dst_rect.x;
        dstrect->y = dst_rect.y;
        dstrect->w = copy_width;
        dstrect->h = copy_height;
    }
    
    return 0;
}

void show_raster_part(SDL_Surface *screen, int y1, int y2, SDL_Surface *surface, RGB_PAL pal)
{
  SDL_Rect src = {0, y1, 320, y2-y1+1};
  SDL_Rect dst = {src.x * gfx_scale_factor, src.y * gfx_scale_factor,
		   src.w * gfx_scale_factor, src.h * gfx_scale_factor};

#if 0
  printf("show_raster_part y1=%d y2=%d ", y1, y2);
  print_palette(pal);
  printf("\n");
#endif

  set_surface_PAL(surface, pal);
  SDL_SetSurfaceColorKey(surface, false, 0);  // Palettenindex 0 nicht transparent
  SDL_BlitSurfaceScaled(surface, &src, screen, &dst, SDL_SCALEMODE_NEAREST);
  //SDL_BlitSurface(surface, &src, screen, &dst);
  return;

#if 0

  
  SDL_Surface* surface_raster_part = SDL_CreateSurface(src.w, src.h, SDL_PIXELFORMAT_INDEX8);
  
  set_surface_PAL(surface_raster_part, pal);
#if 0
  SDL_BlitSurface(surface, &src, surface_raster_part, NULL);
#else
  BlitSurface_INDEX8(surface, &src, surface_raster_part, NULL, 255);
#endif
  
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface_raster_part);
  SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
  
  dst.x = src.x * gfx_scale_factor;
  dst.y = src.y * gfx_scale_factor;
  dst.w = src.w * gfx_scale_factor;
  dst.h = src.h * gfx_scale_factor;
  
  SDL_RenderTexture(renderer, texture, NULL, &dst);
  SDL_DestroyTexture(texture);
  SDL_DestroySurface(surface_raster_part);
#endif
}

void show_surface(SDL_Surface *surface)
{
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_RenderClear(renderer);
  SDL_RenderTexture(renderer, texture, NULL, NULL);
  SDL_DestroyTexture(texture);
  SDL_RenderPresent(renderer);
}

void color_fade(UWORD *color, int fading)
{
  uint8_t r,g,b;

  r = (*color >> 8) & 0b111;
  g = (*color >> 4) & 0b111;
  b = (*color >> 0) & 0b111;
  
  r = (r <= fading ? 0 : r - fading);
  g = (g <= fading ? 0 : g - fading);
  b = (b <= fading ? 0 : b - fading);

  *color = (r << 8) | (g << 4) | (b << 0);
}

void palette_fade(RGB_PAL palette, int fading)
{
  int i;

  for (i=0; i<16; i++) 
    color_fade(&palette[i], fading);
}

// CRT Effekt auf Surface anwenden (Scanlines + Vignette + Noise)
void apply_crt_effect(SDL_Surface *surface) {
    int width = surface->w;
    int height = surface->h;
    Uint32 *pixels = (Uint32 *)surface->pixels;
    //SDL_PixelFormatDetails *fmt = surface->format;
    const SDL_PixelFormatDetails *format = SDL_GetPixelFormatDetails(surface->format);
 
    for (int y = 0; y < height; y++) {
        float scanline = 1.0f - 0.2f * (y % gfx_scale_factor); // jede 2. Zeile dunkler
        for (int x = 0; x < width; x++) {
            Uint32 *p = pixels + y * surface->pitch / 4 + x;
            Uint8 r, g, b, a;
	    SDL_GetRGBA(*p, format, NULL, &r, &g, &b, &a);
	    
            // Vignette
#if 0
            float dx = (x - width / 2) / (float)(width / 2);
            float dy = (y - height / 2) / (float)(height / 2);
            float vignette = 1.0f - (dx * dx + dy * dy) * 0.5f;
            if (vignette < 0.0f) vignette = 0.0f;
#else
            float vignette = 1;
#endif
	    
            // Rauschen
            float noise = (rand() % 20 - 10) / 255.0f;
            
            // Effekt anwenden
            r = (Uint8)fminf(255, fmaxf(0, r * scanline * vignette + noise * 255));
            g = (Uint8)fminf(255, fmaxf(0, g * scanline * vignette + noise * 255));
            b = (Uint8)fminf(255, fmaxf(0, b * scanline * vignette + noise * 255));
            
            *p = SDL_MapRGBA(format, NULL, r, g, b, a);
        }
    }
}

void screenshot()
{
  char filename[64];

  SDL_Surface *surf = SDL_CreateSurface(screen_surface->w, screen_surface->h, SDL_PIXELFORMAT_RGBA32);

  const SDL_PixelFormatDetails *format = SDL_GetPixelFormatDetails(surf->format);
  Uint32 black = SDL_MapRGB(format, NULL, 0, 0, 0);
  SDL_FillSurfaceRect(surf, NULL, black);
  
  SDL_BlitSurface(screen_surface, NULL, surf, NULL);

  time_t t = time(NULL);
  struct tm *tm_info = localtime(&t);

  // Format: Jahr-Monat-Tag-Stunde-Minute
  strftime(filename, sizeof(filename), "return-of-medusa-%Y-%m-%d-%H-%M-%S.bmp", tm_info);
  printf("Screenshot saved in \"%s\"\n", filename);
  SDL_SaveBMP(surf, filename);
  SDL_DestroySurface(surf);
}

void create_screen_texture(void *screen)
{
  int r, previous_r;
  int y1, y2;

#if 0
  UWORD color = 0x603;
  color_fade(&color, 6);
  assert(0);
#endif
  
#if 0
  printf("show_screen: %p\n", screen);

  print_rasters();
#endif
  
  /* Zeigt den "ubergebenen Bildschirm an: */
  SDL_Surface *t = screen;
  last_screen = screen;

#if 0
  const SDL_PixelFormatDetails *format = SDL_GetPixelFormatDetails(screen_surface->format);
  Uint32 black = SDL_MapRGB(format, NULL, 0, 0, 0);
  SDL_FillSurfaceRect(screen_surface, NULL, black);
#endif
  
  /* we have to copy the given screen to the renderer in several parts.
   * Each part is a raster area defined by their Y values because
   * we have different colors in the rasters.
   */
  if (rasters[0].y != 0) {
    print_rasters();
    printf("NO RASTERS SET. NO COLORS, DO NOTHING. rasters[0].y != 0\n");
    return;
  }

  RGB_PAL palette_faded;
  
  previous_r = 0;
  for (r=1; r < ANZ_RASTER; r++) {
    if (rasters[r].y != 255) {
      y1 = rasters[previous_r].y;
      y2 = rasters[r].y - 1;
      for(int i=0; i<16;i++) 
	palette_faded[i] = rasters[previous_r].pal[i];
      //if (fading != 0) palette_fade(palette_faded, fading);
      show_raster_part(screen_surface, y1, y2, t, palette_faded);
      previous_r = r;
    }
  }
  y1 = rasters[previous_r].y;
  if (y1 == 255) y1 = 0;
  y2 = 199;
  show_raster_part(screen_surface, y1, y2, t, rasters[previous_r].pal);

  if (crt_effect && gfx_scale_factor > 1)
    apply_crt_effect(screen_surface);

  SDL_UpdateTexture(screen_texture, NULL, screen_surface->pixels, screen_surface->pitch);
  
  //SDL_Delay(3000);
  
}

void show_screen_texture()
{
  /* Füllt den gesamten Screen in
     Schwarz. Warum ist das
     überhaupt notwendig? Eigentlich
     sollten doch die obigen
     raster_part den alten Inhalt
     überschreiben? */
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Schwarz
  SDL_RenderClear(renderer);

  SDL_SetTextureAlphaMod(screen_texture, blending);
  SDL_RenderTexture(renderer, screen_texture, NULL, NULL);
  //SDL_DestroyTexture(texture);
  
  if (zeit_cheat) {
    /* Show performance */
    //printf("performance_percent: %d\n", performance_percent);
    int y = 200 * gfx_scale_factor * performance_percent / 100;
    if (performance_percent >= 100) {
      y = 199 * gfx_scale_factor;
      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Rot
    } else {
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Weiß
    }
    for (int i=0; i < gfx_scale_factor;i++) {
      SDL_RenderLine(renderer, 0, y+i, 320 * gfx_scale_factor, y+i);
    }
  }
    
  SDL_RenderPresent(renderer);
}

void mcode12(void *screen)
{
  create_screen_texture(screen);
  show_screen_texture();
  logbase = screen;
  vsync();
}

void mcode13(int sync)
{
  /* setzt die Monitorfrequenz */
}

int mcode14(void)
{
  /* holt die Monitorfrequenz */

  return 50;
}

void mcode15(void *src, void *dst, int count)
{
  /* kopiert Anzahl an Biilschirmzeilen von src nach dst. */
  
}

void mcode16(void *screen)
{
  /* L"oscht den Mauszeiger: */
}

void mcode17(void *screen, int x, int y)
{
  /* Zeichnet den Mauszeiger: */
}

void *mcode18(void (*adr)())
{
  /* tr"agt Routine in VBL-Queue ein: */
  if (adr == NULL) {
    return NULL;
  }

  //printf("init_vbl(%p)\n", adr);
  /* Nach freiem Platz im Array suchen */
  for (int i = 0; i < VBL_QUEUE_SIZE; i++) {
    if (vbl_queue[i] == NULL) {
      vbl_queue[i] = adr;
      /* Index + 1 zurückgeben als Handle (0 ist ungültig) */
      return (void*)(intptr_t)(i + 1);
    }
  }
    
  /* Kein freier Platz gefunden */
  return NULL;
}

void mcode19(void *screen, void *savescr, void *spritedata,
	     int sprite_nr, int shape, int x, int y)
{
  SPRITE_SAVE *save = savescr;

  //printf("mcode19: sprite_nr=%d x=%d y=%d screen=%p save=%p\n", sprite_nr, x, y, screen, save);
  
  /* Zeichnet ein Sprite auf dem Bildschirm. */
  SDL_Surface *surface = sprite_surfaces[shape];

  save[sprite_nr].screen = screen;
  save[sprite_nr].x = x;
  save[sprite_nr].y = y;
  
  /* Hintergrund retten */
  SDL_Rect src = {x, y, surface->w, surface->h};
  SDL_Rect dst = {0, 0, surface->w, surface->h};
  BlitSurface_INDEX8(screen, &src, save[sprite_nr].saved_screen, &dst, 255);
  
  //printf("draw_shift(shape=%d, x=%d, y=%d\n", shape, x, y);
  BlitSurface_INDEX8(surface, NULL, screen, &src, 0);
}

void mcode20(void *savescr, int sprite_nr)
{
  /* L"oscht den Sprite vom Bildschirm */
  SPRITE_SAVE *save = savescr;
  
  if (save[sprite_nr].screen == NULL) return;

  int x = save[sprite_nr].x;
  int y = save[sprite_nr].y;

  SDL_Rect src = {0, 0, 16, 8};
  SDL_Rect dst = {x, y, 16, 8};
  //printf("mcode20: sprite_nr=%d x=%d y=%d screen=%p save=%p\n", sprite_nr, x, y, save[sprite_nr].screen, save);
  BlitSurface_INDEX8(save[sprite_nr].saved_screen, &src, save[sprite_nr].screen, &dst, 255);
  save[sprite_nr].screen = NULL;
}

void sort_rasters_only(void)
{
  /* sortiert alle Raster: */
  int i,j;
  raster_t tmp;

  for(j=0;j<ANZ_RASTER;j++)
    for(i=0;i<ANZ_RASTER-1;i++)
      if (hbl_system[i+1].y!=255 && hbl_system[i].y>hbl_system[i+1].y)
	{ 
	  tmp=hbl_system[i];
	  hbl_system[i]=hbl_system[i+1];
	  hbl_system[i+1]=tmp;
	  //fprintf(stderr,"swapped a raster\n");
	}
}

void sort_rasters(void)
{
  int i,j;

  // Sort all rasters according to Y:
  sort_rasters_only();

  // Delete all rasters, which have the same palette
  for(i=0;i<ANZ_RASTER-1;i++)
    if (hbl_system[i].y!=255 && hbl_system[i+1].y!=255) {
      for (j=0; j<16; j++) {
	if (hbl_system[i].pal[j] != hbl_system[i+1].pal[j]) {
	  break;
	}
      }
      if (j==16) {
	// They have the same color palette, so delete second raster
	hbl_system[i+1].y = 255;
      }
    }

  // Sort again (unnessary)
  sort_rasters_only();
}

/* Pointer to RGB_PAL is returned. */
UWORD *get_palette(int y)
{
  int r;

  /* Raster an dieser Position schon vorhanden? */
  for(r=0;r<ANZ_RASTER;r++)
    if (hbl_system[r].y==y)
      {
	return hbl_system[r].pal;
      }
  return NULL;
}

void mcode23(UWORD *palette, int zeile, int res)
{
  /* setzt die angegebenen Farbpalette ab Zeile 'zeile'.
   * bei palette=NULL wird der Raster gelöscht. */
  int i,r;

  /* Raster an dieser Position schon vorhanden? */
  for(r=0;r<ANZ_RASTER;r++)
    if (hbl_system[r].y==zeile) break;         /* Ja -> raus */

  if (palette == NULL) {
    if (r != ANZ_RASTER) {
      // Delete raster
      hbl_system[r].y=255;
      return;
    } else {
      printf("mcode23: finde kein raster in Zeile %d\n", zeile);
      assert(0);
    }
  }
  
  if (r==ANZ_RASTER)                                  /* Nein -> freien Eintrag */
    for(r=0;r<ANZ_RASTER;r++)                         /* suchen */
      if (hbl_system[r].y==255) break;

  hbl_system[r].y=zeile;

  /* dann die ben"otigten Farben in der Colormap eintragen: */
  for(i=0;i<16;i++)
    {
#if 0
      hbl_system[r].pal[i].r = ((*palette & 0xf00)/256) * 9362;
      hbl_system[r].pal[i].g = ((*palette & 0x0f0)/16) * 9362;
      hbl_system[r].pal[i].b = (*palette++ & 0x00f) * 9362;
      hbl_system[r].pal[i].a = 255;
#else
      hbl_system[r].pal[i] = palette[i];
#endif
    }

}

UWORD *search_pal(int y)
{
  /* searches, which palette is active in scanline 'y'. */
  int raster_akt;

  for(raster_akt=0;raster_akt==0 || hbl_system[raster_akt].y!=255;raster_akt++)
    if (y<hbl_system[raster_akt].y) break;
  raster_akt--;

  return hbl_system[raster_akt].pal;
}

void mcode24(void)
{
  /* schaltet alle Raster ab: */
  int i;

  /* Alle Raster sind unbenutzt: */
  for(i=0;i<ANZ_RASTER;i++)
    hbl_system[i].y=255;

  // Ohne Raster würde auf dem ST die Standardpalette kommen.
  // Diese tragen wir als Raster 0 ein, damit wir überhaupt Farben haben
  mcode23(black, 0, 0);    
}

void fade_out_speed(int speed)
{
  /* blendet das Bild aus: */
  for (fading = 0; fading <= 7; fading++) {
    blending = (int)((7.0 - fading) * (255.0/7.0));
    show_screen_texture();
    wait_sync_noshow(speed);
  }
  blending = 0;
}

void mcode25(void)
{
  fade_out_speed(3);
}

void fade_in_speed(int speed)
{
  /* blendet das Bild ein: */

  mcode55();
  rst22rst1();
  
  show_last_screen();
  for (fading = 7; fading >= 0; fading--) {
    blending = (int)((7.0 - fading) * (255.0/7.0));
    show_screen_texture();
    wait_sync_noshow(speed);
  }
  blending = 255;
}

void mcode26(void)
{
  fade_in_speed(3);
}

long mcode27(int zeilen, void *quelle, void *ziel)
{
  /* Packt ein Bild platzsparend */

  return 0;
}

long mcode28(void *quelle, void *ziel)
{
  /* Entpackt ein Grafikbild */

  return 0;
}

void mcode29(int form)
{
  int cursor;
  
  switch(form)
    {
    case 0: cursor = SDL_SYSTEM_CURSOR_CROSSHAIR; break;
    case 1: cursor = SDL_SYSTEM_CURSOR_DEFAULT; break;
    default: fprintf(stderr,"Unknown Mouseform(%d)\n",form); exit(-1);
    }
  
  SDL_SetCursor(SDL_CreateSystemCursor(cursor));
}

int mcode30(int land, int x, int y)
{
  /* holt einen Wert aus der Trafficmap: */
  /* dies ist eine direkte "Ubersetzung aus dem MC68000-Assemblercode in C */
  int d0,d1,d2,d3,d4;
  unsigned char *a0;

  /* innerhalb der Trafficmap? */
  if (x<0 || x>319 || y<25 || y>168) return 0;

  d0=x; d1=y;
  d3=0;
  d1-=25;
  d1>>=2;
  d1*=50;
  
  d0>>=2;
  d2=d0;
  d0<<=2;
  d0+=d2;
  
  d3=d0;
  d0>>=3;
  d0+=d1;
  d3&=7;
  d2=7;
  d2-=d3;

  a0=trf_buf;
  a0+=land*1800;

  d3=*(a0+d0);
  d4=*(1+a0+d0);
  
  d2-=4;
  if (d2>=0)
    {
      d3>>=d2;
      goto schultz_jetzt;
    }

  d3<<=8;
  d3|=d4&0xff;
  
  d0=8;
  d0+=d2;
  d3>>=d0;

 schultz_jetzt:
  d3&=31;
  return d3;
}

static unsigned char *scrolltext;
static unsigned char *scrollpos;
static void *scrollfont;
static int scrolly;
static int delta_x;
static void *scroll_screen;

static void restart_scroller(void)
{
  scrollpos=scrolltext-1;
  delta_x=-200;
}

void mcode32(void *screen, int y, unsigned char *text, void *font, 
	     void *scrollbuf, int copy)
{
  /* bereitet den Scroller vor: */

  scroll_screen = screen;
  scrolltext=text;
  scrollpos=text;
  scrollfont=font;
  scrolly=y;

  restart_scroller();
}

void mcode33(void)
{
  /* Scrollt die Laufschrift um eins weiter */
  static int zeichen;
  unsigned char sonderzeichen[] = ".,:' ";
  int i;
  int x_char;
  int breite;

 restart:

  if (delta_x<=-2)          /* sind noch Spalten da? */
    {
      scrollpos++;
      if (*scrollpos==0)       /* scrolltext zu Ende -> neu starten */
	{ restart_scroller(); goto restart; }

      zeichen=*scrollpos;
      
      for(i=0;sonderzeichen[i]!=0;i++)              /* ist es ein Sonderzeic */
	if (sonderzeichen[i]==zeichen) break;
      if (sonderzeichen[i]!=0)              
	zeichen=26*2+i;             /* Sonderzeichen am Ende des Alph. */
      else
	{
	  if (zeichen>'Z')
	    zeichen-='a'-'Z'-1;     /* Kleinbuchstaben */
	  zeichen-='A';             /* in internen Code wandeln */
	  if (zeichen<0)
	    { steuerzeichen=zeichen+'A';  goto restart; }
	}
      
      delta_x=(objekt_breite(zeichen,scrollfont)+1) & 0xfffe;
    }

  mcode49(2,scrolly,319,scrolly+41,scroll_screen,160,0,scrolly,scroll_screen,160);
  // logbase=&x_window;
  logbase = scroll_screen;
  fill(0,318,scrolly,319,scrolly+41);

  if (delta_x<=0)           /* Wortzwischenraum behandeln */
    {
      delta_x-=2;
      return;
    }

  breite=(objekt_breite(zeichen,scrollfont)+1)&0xfffe;
  x_char=breite-delta_x;
  delta_x-=2;

  /* Bildschirm um 2 Pixel nach links scrollen: */
  draw_obj_part(zeichen,scrollfont,x_char,0,
		x_char+1,objekt_hoehe(zeichen,scrollfont),
		MOVE,scroll_screen,318,scrolly);

  wait_sync(1);
}

void mcode34(void)
{
  /* schaltet die HBL-Raster an: */

  mcode24();   // clear raster
  mcode55();   // compute raster
  rst22rst1();
  rasters = rasters1;
}

void mcode35(void)
{
  /* Schaltet alle Raster wieder aus (Disable IRQ): */
}

// Global state variables
//static bool digi_works = false;
static bool halts_maul = true;
static uint16_t anzahl_seq;
static int16_t anzahl_zeilen;
static uint16_t anz_start;
static uint16_t seq_counter;
static bool roundabout;

static uint32_t *seq_tabelle;
static uint32_t *zeilen_tabelle;
static uint32_t *naechste_zeile;
static uint8_t *start_digi;
static uint8_t *digi_pos;
static uint8_t *digi_end;
static uint8_t *start_sequenz;

// SDL3 Audio variables
//static SDL_AudioDeviceID audio_device = 0;
static SDL_AudioStream *audio_stream = NULL;
static uint8_t *current_sample_data = NULL;
static uint32_t current_sample_pos = 0;
static uint32_t current_sample_len = 0;
static int current_target_freq = 8000;
static bool audio_playing = false;

// Forward declarations
static void nxt_zeile(void);
static void digi_zuende(void);

static bool is_audio_stream_finished(SDL_AudioStream *stream) {
    int queued = SDL_GetAudioStreamQueued(stream);
    int available = SDL_GetAudioStreamAvailable(stream);
    
    // Stream ist fertig wenn keine Daten mehr queued sind
    // UND keine verarbeiteten Daten zum Abholen bereitstehen
    return (queued == 0 && available == 0);
}

// SDL3 Audio Stream Get Callback - provides audio data when requested
static void audio_get_callback(void *userdata, SDL_AudioStream *stream, int additional_amount, int total_amount) {
    //printf("audio_get_callback(addiotional_amount=%d, total_amount=%d\n", additional_amount, total_amount);
    if (!audio_playing || !current_sample_data || current_sample_pos >= current_sample_len) {
      if (digi_works && is_audio_stream_finished(stream))
	digi_zuende();
      return;
    }
    
    // Calculate how much data we can provide
    uint32_t remaining_samples = current_sample_len - current_sample_pos;
    uint32_t samples_to_write = (remaining_samples < (uint32_t)total_amount) ? remaining_samples : (uint32_t)total_amount;

    //samples_to_write = remaining_samples;
    
    if (samples_to_write > 0) {
        //printf("Playing sample current_sample_data=%p, current_sample_pos=%d, samples_to_write=%d\n", current_sample_data, current_sample_pos, samples_to_write);
      
        // Put sample data into stream
        int written = SDL_PutAudioStreamData(stream, current_sample_data + current_sample_pos, samples_to_write);
	if (written < 0) {
	  printf("PutAudioStreamData error: %s\n", SDL_GetError());
	  exit(1);
	}
        current_sample_pos += samples_to_write;
        
        // Check if we've finished this sample
        if (current_sample_pos >= current_sample_len) {
            // Load next sample
            nxt_zeile();
        }
    } else {
      printf("Audio-Callback: Nothing to do\n");
      if (digi_works && is_audio_stream_finished(stream))
	digi_zuende();
    }
}

// Load next sample
static void nxt_zeile(void) {

  //printf("nxt_zeile %d %d\n", seq_counter, anzahl_zeilen);
  
  seq_counter--;
    if (seq_counter > 0) {
      current_sample_pos = 0;
      return; // Play this sequence again
    }
    
    // Move to next line
    anzahl_zeilen--;
    if (anzahl_zeilen >= 0) {
        // Continue with this line
    } else {
        if (roundabout) {
            // Start from beginning
            naechste_zeile = zeilen_tabelle;
            anzahl_zeilen = anz_start;
        } else {
            if (anzahl_zeilen <= -1) {
                // Stop playing
                //digi_zuende();
                return;
            }
            seq_counter = 1; // Stop next time
            return;
        }
    }
    
    uint8_t *line_ptr = (uint8_t *)naechste_zeile;
    
    // Read number of repetitions
    uint8_t repetitions = *line_ptr++;
    seq_counter = repetitions;
    
    // Read next sequence number
    uint8_t seq_num = *line_ptr++;
    
    // Calculate sequence address
    uint32_t *seq_ptr = (uint32_t *)((uint8_t *)seq_tabelle + (seq_num * 24));
    
    // Read sequence start
    uint32_t seq_start = *seq_ptr++;
    //seq_start = convert_from_be_4(seq_start);
    
    digi_pos = start_digi + seq_start;
    start_sequenz = digi_pos;
    
    // Read sequence length
    //uint32_t seq_len = *seq_ptr++;
    //digi_end = start_digi + seq_start + seq_len - 2;
    uint32_t seq_end = *seq_ptr++;
    digi_end = start_digi + seq_end - 2;
    
    // Skip direction flag (not used in this implementation)
    line_ptr++;
    
    // Read frequency
    uint8_t freq_idx = *line_ptr++;
    naechste_zeile = (uint32_t *)line_ptr;
    
    // Set sample frequency (minimum 1 kHz, maximum 20 kHz)
    freq_idx = (freq_idx > 0) ? freq_idx - 1 : 0;
    if (freq_idx >= 20) {
        freq_idx = 19; // Default to 20 kHz
    }
    //assert(freq_idx == 4);
    
    // Direct frequency calculation from index (1-20 kHz)
    current_target_freq = (freq_idx + 1) * 1000;

    SDL_AudioSpec spec = {
        .freq = current_target_freq,
        .format = SDL_AUDIO_U8,
        .channels = 1
    };
    SDL_SetAudioStreamFormat(audio_stream, &spec, NULL);
 
    // Set up sample data for SDL
    current_sample_data = (uint8_t *)digi_pos;
    current_sample_pos = 0;
    current_sample_len = digi_end - digi_pos;
    
    // Note: In a more sophisticated implementation, you might want to
    // create a new audio stream with the correct sample rate
    printf("Playing sample: pos=%p, len=%u, freq=%d Hz\n", 
           digi_pos, current_sample_len, current_target_freq);
}

// Stop digital sound playback
static void digi_zuende(void) {
    audio_playing = false;
    digi_works = false;
    halts_maul = true;
    
    current_sample_data = NULL;
    current_sample_pos = 0;
    current_sample_len = 0;
    
    //printf("Digital sound playback stopped\n");
}

// Initialize SDL3 audio system
static bool init_digi_audio(void) {
    
    // Create audio stream with 8-bit signed mono audio (typical for Amiga samples)
    SDL_AudioSpec spec = {
      // .freq = 22050,
        .freq = 1000,
        .format = SDL_AUDIO_U8,
        .channels = 1
    };
    
    audio_stream = SDL_CreateAudioStream(&spec, NULL);
    if (!audio_stream) {
        printf("Failed to create audio stream: %s\n", SDL_GetError());
        //SDL_CloseAudioDevice(audio_device);
        //audio_device = 0;
        return false;
    }

    // Set the get callback that will provide audio data
    if (!SDL_SetAudioStreamGetCallback(audio_stream, audio_get_callback, NULL)) {
        printf("Failed to set audio stream callback: %s\n", SDL_GetError());
        SDL_DestroyAudioStream(audio_stream);
        //SDL_CloseAudioDevice(audio_device);
        audio_stream = NULL;
        //audio_device = 0;
        return false;
    }
    
    // Bind the stream to the device
    if (!SDL_BindAudioStream(g_audio.device, audio_stream)) {
        printf("Failed to bind audio stream: %s\n", SDL_GetError());
        SDL_DestroyAudioStream(audio_stream);
        //SDL_CloseAudioDevice(audio_device);
        audio_stream = NULL;
        //audio_device = 0;
        return false;
    }
    
    return true;
}

// Cleanup SDL3 audio
static void cleanup_audio(void) {
    if (audio_stream) {
        SDL_DestroyAudioStream(audio_stream);
        audio_stream = NULL;
    }
#if 0
    if (audio_device) {
        SDL_CloseAudioDevice(audio_device);
        audio_device = 0;
    }
#endif
    SDL_Quit();
}

// Main function to play digital sound
void mcode36(void *startadr, int roundabout_flag, int startzeile, int endzeile)
{
    // Stop any currently playing sound
    if (digi_works) {
        digi_zuende();
    }
    
    digi_works = false;
    halts_maul = true; // Allow Hippel to play
    
    // If start address is 0, just turn off
    if (startadr == 0) {
        return;
    }

#if 0
    // Initialize audio system if not already done
    if (!audio_device && !init_audio()) {
        return;
    }
#endif
    
    int i;
    for (i = 0; i < DIGI_COUNT; i++) {
      if (digi_mem[i].atari_mem == startadr) {
	break;
      }
    }
    if (i == DIGI_COUNT) {
      printf("Unable to find %p in digi_mem\n", startadr);
      exit(1);
    }

    uint32_t *data_ptr = (uint32_t *)digi_mem[i].local_mem;
    
    // Check for 'SAMP' signature (handle endianness)
    uint32_t signature = *data_ptr++;
    if (signature != 0x53414D50 && signature != 0x504D4153) { // 'SAMP' or 'PMAS'
        printf("Invalid sample signature: 0x%08X\n", signature);
	exit(1);
    }
    
    halts_maul = false; // Turn off Hippel
    
    uint16_t *word_ptr = (uint16_t *)data_ptr;
    
    // Read number of sequences
    anzahl_seq = *word_ptr++;
    
    // Read number of lines in editor
    uint16_t editor_lines = *word_ptr++;
    
    // Validate line ranges
    if (startzeile < 0 || endzeile >= editor_lines || startzeile > endzeile) {
        printf("Invalid line range: start=%d, end=%d, total=%d\n", 
               startzeile, endzeile, editor_lines);
        exit(1);
    }
    
    // Calculate number of lines to play
    int lines_to_play = endzeile - startzeile + 1;
    anzahl_zeilen = lines_to_play;
    anz_start = lines_to_play - 1;
    
    // Set roundabout flag
    roundabout = roundabout_flag;
    
    // Set up pointers
    seq_tabelle = (uint32_t *)word_ptr;
    
    // Skip sequence table
    uint8_t *line_start = (uint8_t *)word_ptr + (anzahl_seq * 24);
    
    // Add offset for start line
    line_start += startzeile * 4;
    zeilen_tabelle = (uint32_t *)line_start;
    naechste_zeile = (uint32_t *)line_start;
    
    // Calculate start of digital data
    start_digi = (uint8_t *)((uint8_t *)word_ptr + (anzahl_seq * 24) + (editor_lines * 4));
    
    seq_counter = 1; // Load immediately
    nxt_zeile(); // Set all parameters
    
    // Start audio playback
    audio_playing = true;
    digi_works = true;
    
    //printf("Digital sound playback started\n");
}

// Check if digital sound is currently playing
bool is_digi_playing(void) {
    return digi_works && audio_playing;
}

// Stop digital sound playback (external interface)
void stop_digi(void) {
    if (digi_works) {
        digi_zuende();
    }
}

// Example usage and cleanup
void digi_shutdown(void) {
    stop_digi();
    cleanup_audio();
}

void mcode36_2(void *startadr, int roundabout, int startzeile, int endzeile)
{
  UWORD *pu;
  char *p;
  char *edit_lines;
  
  /* spielt einen Digi ab */
  int i;
  for (i = 0; i < DIGI_COUNT; i++) {
    if (digi_mem[i].atari_mem == startadr) {
      break;
    }
  }
  if (i == DIGI_COUNT) {
    printf("Unable to find %p in digi_mem\n", startadr);
    exit(1);
  }

  p = digi_mem[i].local_mem;
  p += 4;         // 'SAMP'
  pu = (UWORD *)p;

  //anz_seq=*pu++;
  //anz_zeilen=*pu++;

  p = (char *)pu;
  //p += anz_seq * 24;

  edit_lines = p;

  anzahl_zeilen = endzeile - startzeile + 1;
  
  // zeilen_tabelle = &edit_lines[startzeile*4];
  naechste_zeile = zeilen_tabelle;
  seq_counter = 1;
  nxt_zeile();
}

void mcode38(int efx)
{
  /* spielt einen Hippelsound ab */
}

void mcode39(void *screen, int breite, int hoehe, void *palette)
{
  /* konvertiert ein Farbbild in Graustufen */
}

SHARED_OBJECT *search_obj_addr(void *atari_mem)
{
  /* searches the internal translation table for the correct address of the
   * object.
   */
  int i;

  for(i=0;i<SH_OBJ_MAX;i++)
    if (sh_obj[i].filename[0]!=0 && sh_obj[i].atari_mem==atari_mem)
      return &sh_obj[i];

  fprintf(stderr,"RoM: object not found in search_obj_addr()\n");
  assert(0);
  exit(-1);
}

void mcode47(int ob_nr,void *ob_basis,int modus,void *scr_basis,int x,int y)
{
  /* Zeichnet ein Objekt aus einer Objekt-Bibliothek auf den Bildschirm.
   * Sollte das Objekt mit Rastern abgespeichert worden sein, so werden
   * diese auch gesetzt.
   */

  for (int i=0; i < 200;i++) {
    raster_t *pal = objekt_palette(ob_nr, ob_basis, i);
    if (pal) {
      set_raster(0, y+pal->y, pal->pal);
    } else {
      break;
    }
  }
  
  draw_obj_part(ob_nr,ob_basis,0,0,
		objekt_breite(ob_nr,ob_basis)-1,objekt_hoehe(ob_nr,ob_basis)-1,
		modus,scr_basis,x,y);
}

SDL_Surface *get_obj_surface(int ob_nr, void *ob_basis)
{
  UNIX_OBJECT *uo;
  SHARED_OBJECT *sh_obj;
  
  sh_obj=search_obj_addr(ob_basis);

  if (ob_nr>sh_obj->object_count)
    {
      fprintf(stderr,"RoM: object number (%d) too big (%d) of %s in mcode48()\n",
	      ob_nr,sh_obj->object_count, sh_obj->filename);
      //raise(SIGTRAP);
      exit(-1);
    }
  uo = &sh_obj->objects[ob_nr];
  return uo->surface;
}

void show_obj(int ob_nr, void *ob_basis) {
  SDL_Surface *surface;

  surface = get_obj_surface(ob_nr, ob_basis);
  set_surface_PAL(surface, leistep);
  show_surface(surface);
}

// #define draw_obj_part(ob_nr,ob_basis,x1,y1,x2,y2,modus,scr,x,y) mcode48(ob_nr,ob_basis,x1,y1,x2,y2,modus,scr,x,y)
void mcode48(int ob_nr,void *ob_basis,int x1, int y1,int x2,int y2,
	     int modus,void *scr_basis,int x,int y)
{
  /* Zeichnet einen Teil eines Objekt aus einer Objekt-Bibliothek
   * auf den Bildschirm.
   */
  UWORD *anzahlp,anzahl;
  OBJEKT *obj_header;
  int raster_anzahl;
  UNIX_OBJECT *uo;
  UWORD *pal;
  raster_t *pal_obj;
  SHARED_OBJECT *sh_obj;
  register unsigned char *p,*p_end,*pi;
  int i;

  sh_obj=search_obj_addr(ob_basis);

  anzahlp=(UWORD *)sh_obj->obj_mem;
  anzahl=*anzahlp++;

  if (ob_nr>sh_obj->object_count)
    {
      fprintf(stderr,"RoM: object number (%d) too big (%d) of %s in mcode48()\n",
	      ob_nr,sh_obj->object_count, sh_obj->filename);
      //raise(SIGTRAP);
      exit(-1);
    }
  else
    {
      obj_header=(OBJEKT *)anzahlp;
      obj_header+=ob_nr;                /* points to our objekt_header */
      
      raster_anzahl=(obj_header->reserved)>>8;
      uo = &sh_obj->objects[ob_nr];

      SDL_Rect src = {x1, y1, x2-x1+1, y2-y1+1};
      SDL_Rect dst = {x, y, src.w, src.h};

      //printf("draw_obj_part: %d,%d %d/%d -> %d,%d\n", src.x, src.y, src.w, src.h, dst.x, dst.y);
      
      switch (modus)
	{
	case MOVE:
	  if (raster_anzahl!=0)              /* object with saved rasters */
	    {
	      
	      BlitSurface_INDEX8(uo->surface, &src, scr_basis, &dst, 255);
#if 0
	      XPutImage(x_display,*(Pixmap *)scr_basis,x_gc,ximage,
			x1*2,y1*2,x*2,y*2,(x2-x1)*2+2,(y2-y1)*2+2);
#endif
	      /* calculate start of rasters palettes: */
	      pal_obj=(raster_t *)((unsigned char *)anzahlp+obj_header->offset+
				 obj_header->hoehe*obj_header->breite_bytes);
	      /* create color in colormap */
#if 0
	      for(i=0;i<raster_anzahl;i++)
		set_raster(0,y+pal_obj[i].y,pal_obj[i].pal);
#endif
	    }
	  else
	    {
	      pal=search_pal(y);           /* which colors to use? */
	      if (uo->mapped)
		{
		  /* mapped with same palette? */
		  for(i=0;i<16;i++)
		    if (pal[i]!=uo->pal[i]) break;

		  if (i!=16)               /* different palette, map back */
		    {
		      fprintf(stderr,"RoM: palette changed in mcode48()\n");
		      //exit(-1);
		    }
		}
	      else                    /* not yet mapped: */
		{
#if 0
		  /* enter current palette: */
		  uo->mapped=TRUE;
		  for(i=0;i<16;i++)
		    uo->pal[i]=pal[i];
#endif
		  
#if 0
		  p=uo->image->data;
		  p_end=p+uo->image->width*uo->image->height;
		  for(;p!=p_end;p++)
		    *p=pal[*p];
#endif
		}
	      
	      // SDL_SetRenderDrawColor(renderer, r, g, b, 255);

	      // Ziel-Texture als Render-Target setzen
	      //SDL_SetRenderTarget(renderer, logbase);
	      
	      //SDL_SetTextureColorMod(font_texture, r, g, b);
	      //SDL_RenderCopy(renderer, font_texture, &src, &dst);

	      //SDL_SetSurfaceColorKey(uo->surface, false, 0);  // Palettenindex 0 nicht transparent
	      //SDL_BlitSurface(uo->surface, &src, scr_basis, &dst);
	      BlitSurface_INDEX8(uo->surface, &src, scr_basis, &dst, 255);

#if 0
	      XPutImage(x_display,*(Pixmap *)scr_basis,x_gc,ximage,
			x1*2,y1*2,x*2,y*2,(x2-x1)*2+2,(y2-y1)*2+2);
#endif
	    }
	  break;
	case ODER:
	  if (1 || raster_anzahl!=0)
	    {
#if 0
	      XSetFunction(x_display,x_gc,GXandInverted);
	      XPutImage(x_display,(Drawable)*ximage,x_gc,mask,x1*2,y1*2,x1*2,y1*2,
			(x2-x1)*2,(y2-y1)*2);
	      if (!uo->transparent)       /* noch nicht transp. gezeichnet */
		{
		  uo->transparent=TRUE;
		  p=uo->mask->data;
		  p_end=p+uo->mask->width*uo->mask->height;
		  pi=uo->image->data;

		  /* alle in der Maske nicht gesetzten Bits auf 0 setzen. */
		  for(;p!=p_end;p++,pi++)
		    if (*p) *pi=0;
		}
#endif

	      // SDL_SetRenderDrawColor(renderer, r, g, b, 255);

	      // Ziel-Texture als Render-Target setzen
	      //SDL_SetRenderTarget(renderer, logbase);
	      
	      //SDL_SetTextureColorMod(font_texture, r, g, b);
	      //SDL_RenderCopy(renderer, font_texture, &src, &dst);

	      //SDL_SetSurfaceColorKey(uo->surface, true, 0);  // Palettenindex 0 transparent
	      //SDL_BlitSurface(uo->surface, &src, scr_basis, &dst);
	      
	      BlitSurface_INDEX8(uo->surface, &src, scr_basis, &dst, 0);

#if 0
	      XSetFunction(x_display,x_gc,GXand);
	      XPutImage(x_display,*(Pixmap *)scr_basis,x_gc,mask,
			x1*2,y1*2,x*2,y*2,(x2-x1)*2,(y2-y1)*2);
	      XSetFunction(x_display,x_gc,GXor);
	      XPutImage(x_display,*(Pixmap *)scr_basis,x_gc,ximage,
			x1*2,y1*2,x*2,y*2,(x2-x1)*2,(y2-y1)*2);
	      XSetFunction(x_display,x_gc,GXcopy);
#endif
	    } else {
	    printf("Raster == 0\n");
	    //raise(SIGTRAP);
	    exit(1);
	  }
	  break;
	default:
	  fprintf(stderr,"RoM: unknown mode (%d) in mcode48\n",modus); exit(-1);
	}
    }
}

// Diese Routine kopiert einen rechteckigen Bildschirmbereich von einem Platz
// zu einem anderen. Dabei werden Überlappungen NICHT berücksichtigt.
// Der neue Bereich ersetzt den alten vollständig (replace).
// Die Routine clippt den Zielbereich. Als Bildschirmbreite wird die
// übergebene genommen, als Bildschirmhöhe wird 200 angenommen.
void mcode49(int x1, int y1, int x2, int y2, void *src, int s_width,
	     int x3, int y3, void *dest, int d_width)
{
  /* kopiert einen Rechteckigen Bildschirmbereich */
#if 0
  XCopyArea(x_display,*(Pixmap *)src,*(Pixmap *)dest,x_gc,
	    x1*2,y1*2,(x2-x1)*2+2,(y2-y1)*2+2,x3*2,y3*2);
#endif
  
  SDL_Rect src_rect = {x1, y1, x2-x1 + 1, y2-y1 + 1};
  SDL_Rect dst_rect = {x3, y3, src_rect.w, src_rect.h};

  // SDL_SetRenderDrawColor(renderer, r, g, b, 255);

  // Ziel-Texture als Render-Target setzen
  //SDL_SetRenderTarget(renderer, logbase);
    
  //SDL_SetTextureColorMod(font_texture, r, g, b);
  //SDL_RenderCopy(renderer, font_texture, &src, &dst);
  //SDL_BlitSurface(src, &src_rect, dest, &dst_rect);
  BlitSurface_INDEX8(src, &src_rect, dest, &dst_rect, 255);
}

void mcode50(int x1, int y1, int x2, int y2, void *src, int s_width,
	     int x3, int y3, void *dest, int d_width)
{
  /* kn"upft einen Rechteckigen Bildschirmbereich ein */
  SDL_Rect src_rect = {x1, y1, x2-x1 + 1, y2-y1 + 1};
  SDL_Rect dst_rect = {x3, y3, src_rect.w, src_rect.h};

  // SDL_SetRenderDrawColor(renderer, r, g, b, 255);

  // Ziel-Texture als Render-Target setzen
  //SDL_SetRenderTarget(renderer, logbase);
    
  //SDL_SetTextureColorMod(font_texture, r, g, b);
  //SDL_RenderCopy(renderer, font_texture, &src, &dst);
  //SDL_BlitSurface(src, &src_rect, dest, &dst_rect);
  BlitSurface_INDEX8(src, &src_rect, dest, &dst_rect, 0);
}

void mcode51(void *scr, int x1, int y1, int x2, int y2)
{
  /* spiegelt einen Bildschirmbereich vertical */
  SDL_Surface *surf = (SDL_Surface *)scr;
  uint8_t *pixels = (uint8_t *)surf->pixels;
  int pitch = surf->pitch; // Bytes pro Zeile

  for (int y = y1; y <= y2; y++) {
    uint8_t *left  = pixels + y * pitch + x1;
    uint8_t *right = pixels + y * pitch + x2;

    while (left < right) {
      uint8_t tmp = *left;
      *left  = *right;
      *right = tmp;

      left++;
      right--;
    }
  }
#if 0
  XSetFunction(x_display,x_gc,GXcopy);

  width=(x2-x1)/2; height=y2-y1+1;
  for(x=0;x<width;x++)
    {
      XCopyArea(x_display,*(Pixmap *)scr,x_swapscreen,x_gc,
		(x1+x)*2,y1*2,2,height*2+2,0,0);
      XFlush(x_display);
      XCopyArea(x_display,*(Pixmap *)scr,*(Pixmap *)scr,x_gc,
		(x2-x)*2,y1*2,2,height*2+2,(x1+x)*2,y1*2);
      XFlush(x_display);
      XCopyArea(x_display,x_swapscreen,*(Pixmap *)scr,x_gc,
		0,0,2,height*2+2,(x2-x)*2,y1*2);
      XFlush(x_display);
    }
  XFlush(x_display);
#endif
}

long mcode52(void *src, void *dst)
{
  /* entpackt durch den ICE!-Packer */

  // return ice_decrunch(src, dst, 10000);
  return ice_24_depack(src, dst);
}

// Hilfsfunktion zum Setzen eines Pixels
void set_pixel_surface(SDL_Surface *surface, int x, int y, Uint8 color) {
    if (!surface || x < 0 || y < 0 || x >= surface->w || y >= surface->h) {
      printf("set_pixel_outside\n");
      exit(1);
      return; // Außerhalb der Surface-Grenzen
    }
    
    Uint8* pixels = (Uint8*)surface->pixels;
    pixels[y * surface->pitch + x] = color;
}

// Hilfsfunktion zum Setzen eines Pixels
Uint8 get_pixel_surface(SDL_Surface *surface, int x, int y) {
    if (!surface || x < 0 || y < 0 || x >= surface->w || y >= surface->h) {
        return 0; // Außerhalb der Surface-Grenzen
    }
    
    Uint8* pixels = (Uint8*)surface->pixels;
    return pixels[y * surface->pitch + x];
}

// Zeichnet eine Linie auf dem Screen, der durch logbase bestimmt wird. Zum
// Zeichnen der Linie kann auch eine Farbe angegeben werden.
void mcode53(int color, int x1, int y1, int x2, int y2)
{
  /* Zeichnet eine Linie auf dem Bildschirm */
  //UWORD *pal;
  Uint8 c;
  
  //pal=search_pal(y1);

  int dx = abs(x2 - x1);
  int dy = abs(y2 - y1);
  int sx = (x1 < x2) ? 1 : -1;
  int sy = (y1 < y2) ? 1 : -1;
  int err = dx - dy;
    
  int x = x1;
  int y = y1;
    
  while (1) {

    // [1] Linepattern prüfen
    UWORD word = active_pattern[y%16];
    if (word & (1 << (x%16))) { 
      
	switch(choosen_linemode) {
	case 0:     // Replace
	case 2:   // transparent
	  set_pixel_surface(logbase, x, y, (Uint8)color);
	  break;
	case 1:     // Eor
	  c = get_pixel_surface(logbase, x, y);
	  c ^= (Uint8)color;
	  set_pixel_surface(logbase, x, y, c);
	  break;
	}
      }
      
    if (x == x2 && y == y2) {
      break;
    }
        
    int e2 = 2 * err;
        
    if (e2 > -dy) {
      err -= dy;
      x += sx;
    }
        
    if (e2 < dx) {
      err += dx;
      y += sy;
    }
  }
  
#if 0
  XSetForeground(x_display,x_gc,(color<0) ? -color : pal[color]);
  XDrawLine(x_display,*(Pixmap *)logbase,x_gc,x1*2,y1*2,x2*2+1,y2*2+1);
#endif
}

void mcode54(int color, int x1, int y1, int x2, int y2)
{
  /* F"ullt ein Fl"ache auf dem logbase */
  UWORD *pal;
  uint8_t r, g, b;
  
  //  pal=search_pal(y1);
  //atari_color_to_rgb(pal[color], &r, &g, &b);

  
  SDL_Rect rect = { x1, y1, x2 - x1 + 1, y2 - y1 + 1};
  //  SDL_FillSurfaceRect(logbase, &rect, color);

  Uint8* pixels = (Uint8*)logbase->pixels;
  for (int y = y1; y <= y2; y++) {
    mcode53(color, x1, y, x2, y);
#if 0
    for (int x = x1; x <= x2; x++) {
      pixels[y * font_surface->pitch + x] = color;
    }
#endif
  }
  
    
  //XSetForeground(x_display,x_gc,pal[color]);
  //XSetFillStyle(x_display,x_gc,FillStippled);
  //XFillRectangle(x_display,*logbase,x_gc,x1*2,y1*2,(x2-x1)*2+2,(y2-y1)*2+2);
}

void mcode55(void)
{
  /* berechnet die Raster neu: Geht durch hbl_system und baut daraus raster2 auf. rasters2 ist eine nach Y sortierte Version von hbl_system. */
  sort_rasters();
  rst2rst(hbl_system, rasters2);
}

void mcode56(void){
  /* show_raster();
   * Zeigt die gesetzten Raster an.
   */
  mcode55();
  rasters = rasters2;
  show_last_screen();
  mcode62();     // vsync()

  rst22rst1();    // und zurück kopieren
  rasters = rasters1;
  show_last_screen();
  mcode62();     // vsync
}

int32_t mcode57(void)
{
  /* Liest ein Zeichen aus dem Tastaturbuffer.
   * Ist keines vorhanden, wird -1 zur"uckgegeben.
   */
  int32_t key;

  if (!is_key()) return -1;
  
  key=keybuffer.buf[keybuffer.read];
  //printf("READ keybuffer[%d]=0x%02x,%c\n", keybuffer.read, (key>>16), key & 255);
  keybuffer.read++;
  if (keybuffer.read>=sizeof(keybuffer.buf)/sizeof(keybuffer.buf[0]))
    keybuffer.read=0;

  return key;
}

int mcode58(int linemode)
{
  /* Mit Hilfe dieser Funktion können die Parameter für die Line-Funktion
   * eingestellt werden. Da die Fill Funktion die Line-Funktion benützt, gelten
   * die Einstellungen auch für die Fill-Funktion:
   * Die Funktion gibt den alten Linientyp zurück,
   * bei einer Übergabe von -1 wird der Linientyp nicht verändert.
   * 0=Replace, 1=Eor, 2=Transparent
   */
  int function;
  int previous_linemode;
  
  previous_linemode = choosen_linemode;
  if (linemode>=0) 
    {
      choosen_linemode=linemode;      /* save choosen linemode for future */
    }
  
  return previous_linemode;
}

// Bestimmt die Farbe eines Pixels auf logbase
int mcode59(int x, int y)
{
  /* Bestimmt die Farbe eines Pixels auf dem Bildschirm */
  return get_pixel_surface(logbase, x, y);
}

UWORD *mcode60(UWORD *pattern)
{
  /*
   * Bestimmt das Muster, mit dem gefüllt werden soll. Diese
   * Einstellung gilt für Linienziehen, wie für füllen.  Es wird
   * hierbei ein Pointer auf 16 UWORDs übergeben, in denen das
   * Füllmuster zeilenweise abgespeichert wird. Es wird also, je nach
   * y-Zeile % 16 die entsprechende Zeile aus dem Muster geholt, und
   * damit die Linie verknüpft. 1 UWORD hat 16 Bit = 16 Pixel
   * Breite. 16 UWORDs für 16 Zeilen.
   *
   * Ist das gesamte Integerarray mit -1 gefüllt, also alle Bits
   * gesetzt, wird ganz normal gefüllt.  Wird als Integeradresse -1L
   * angegeben, so wird die Adresse des letzten Füllmusters
   * zurückgegeben.
   */
  UWORD *last_pattern;

  last_pattern=active_pattern;
  if (pattern!=NULL)
    {
#if 0
      if (active_pattern)
	XFreePixmap(x_display,stipple);
#endif
      
      active_pattern=pattern;
#if 0
      stipple=XCreateBitmapFromData(x_display,x_window,(char *)pattern,16,16);
      XSetFillStyle(x_display,x_gc,FillStippled);
      XSetStipple(x_display,x_gc,stipple);
#endif
    }
  
  return last_pattern;
}

long mcode61(int archiv, long offset, long len, void *adr)
{
  /* L"adt ein File aus einem Archiv: */

  return 0;
}

void mcode62(void)
{
  /* Routine wartet einen VBL */
  long old_vbl_ct = vbl_ct;
  while(vbl_ct==old_vbl_ct) ;
}

void mcode63(void *music_routine, int song_nr)
{
  /* plays a song using 'music_routine'. */
  if (musik_an)
    play_audio(music_routine);
}
   
int mcode64(int nr, void *adr)
{
  /* Speichert einen Spielstand auf Diskette ab */
  int etrack;

  etrack = nr * 9 + 11;
  
  mcode70(adr, 0, etrack - 8, 1, 0, etrack, 9, 511);

  return 0;
}

int mcode65(int nr, void *adr)
{
  /* Lädt einen Spielstand auf Diskette ab */
  int etrack;

  etrack = nr * 9 + 11;
  
  mcode68(adr, 0, etrack - 8, 1, 0, etrack, 9, 511);

  return 0;
}

unsigned int mcode66(void)
{
  /* liefert eine 16-Bit Zufallszahl */
  return rand() & 0xffff;
}

FLAG mcode67(void)
{
  /* Routine "uberpr"uft, ob eine Taste gedr"uckt ist. */

  ask_X();

  return keybuffer.read!=keybuffer.write;
}

int current_disk_nr = 0;

size_t compute_disk_offset(int laufwerk,int strack,int ssektor,int soffset)
{
  int sektor_size = 512;
  int sektor_per_track = 9;
  int track_per_side = 80;
  
  return laufwerk * (track_per_side * sektor_per_track * sektor_size)
    + strack * (sektor_per_track * sektor_size)
    + ssektor * sektor_size
    + soffset; 
}

void compute_disk_filename(char *filename)
{
  sprintf(filename, "RoM-floppy%d.img", current_disk_nr);
}

void mcode68(void *adr,int laufwerk,int strack,int ssektor,int soffset,
                          int etrack,int esektor,int eoffset)
{
  /* Liest einen Sektor von Diskette */
  char filename[100];
  size_t offset_start = compute_disk_offset(laufwerk, strack, ssektor, soffset);
  size_t offset_end   = compute_disk_offset(laufwerk, etrack, esektor, eoffset);
  size_t len = offset_end - offset_start;
  int fd;
  
  compute_disk_filename(filename);

  if ((fd=open(filename,O_RDONLY, O_BINARY))<0)
    {
      memset(adr, 0, len);
      return;
    }
  off_t pos = lseek(fd, (off_t)offset_start, SEEK_SET);
  len=read(fd,adr,len);
  close(fd);
}

void mcode69(int disk_nr)
{
  /* User soll bestimmte Diskette einlegen. */
  current_disk_nr = disk_nr;
}

int mcode70(void *adr,int laufwerk,int strack,int ssektor,int soffset,
	    int dtrack,int dsektor,int doffset)
{
  /* Schreibt einen Sektor von Diskette */
  char filename[100];
  size_t offset_start = compute_disk_offset(laufwerk, strack, ssektor, soffset);
  size_t offset_end   = compute_disk_offset(laufwerk, dtrack, dsektor, doffset);
  size_t len = offset_end - offset_start;
  int fd;
  
  compute_disk_filename(filename);

  if ((fd=open(filename,O_WRONLY|O_CREAT|O_BINARY, 0644))<0)
    {
      perror("mcode70");
      exit(1);
    }
  off_t pos = lseek(fd, (off_t)offset_start, SEEK_SET);
  len=write(fd,adr,len);
  close(fd);

  return 0;
}

// #define convert_objekt(ob_nr,obj) mcode71(ob_nr,obj)
void mcode71(int ob_nr, void *obj, UNIX_OBJECT *uo)
{
  /* Konvertiert ein Objekt ins ger"atespezifische Format: */
  UWORD anzahl;
  UWORD *anzahlp;
  OBJEKT *obj_header;
  int raster_anzahl;
  raster_t *pal;
  int i;
  unsigned char *mem;
  int bb_adr,b_offset;
  UWORD *plane;
  int color;
  unsigned char *picture,*maskmem;
  int x,y;
  int raster_akt;
  UWORD b_mask;
  raster_t x_palette[100];

  anzahlp=(UWORD *)obj;
  anzahl=*anzahlp++;

  if (ob_nr>anzahl)
    { fprintf(stderr,"RoM: object number too big in mcode71()\n"); exit(-1); }
  else
    {
      obj_header=(OBJEKT *)anzahlp;
      obj_header+=ob_nr;                /* points to our objekt_header */

      /* Convert from big endiann */
      be_4(&obj_header->offset);  /* Offset in den Objektspeicher */
      be_2(&obj_header->breite);  /* Breite des Objekts in Pixeln */
      be_2(&obj_header->hoehe);   /* Höhe des Objekts in Pixeln */
      be_2(&obj_header->breite_bytes);  /* Breite des Objekts in Bytes */
      be_2(&obj_header->x_neo);   /* Position innerhalb Neochrom-Bild (unwichtig) */
      be_2(&obj_header->y_neo);   /* -'- */
      be_4(&obj_header->reserved);

      uo->surface = SDL_CreateSurface(obj_header->breite, obj_header->hoehe, SDL_PIXELFORMAT_INDEX8);
      
      raster_anzahl=(obj_header->reserved)>>8;
      // raster_anzahl = 0;
      //printf("ob_nr: %d raster_anzahl=%d\n", ob_nr, raster_anzahl);
      
      if (raster_anzahl==0)              /* object without saved rasters */
	{
	  raster_anzahl=1;
	  x_palette[0].y=0;
	  for(i=0;i<16;i++)
	    x_palette[0].pal[i]=i;
	}
      else         
	{
	  /* calculate start of rasters palettes: */
	  pal=(raster_t *)((unsigned char *)anzahlp+obj_header->offset+
			 obj_header->hoehe*obj_header->breite_bytes);
	  /* create color in colormap */
	  for(i=0;i<raster_anzahl;i++)
	    {
	      be_2(&pal[i].y);
	      x_palette[i].y=pal[i].y;
	      //printf("pal[raster:%d].y=%d\n", i, pal[i].y);
	      for(int j=0; j < 16; j++)
		{
		  be_2(&pal[i].pal[j]);    // ATARI color
		  //printf("pal[raster:%d][%d]=%03x\n", i, j, pal[i].pal[j]);
		}
	      //AllocPalette(pal[i].pal,x_palette[i].pal);
	    }
	  set_surface_PAL(uo->surface, pal[0].pal);
	}

      uo->mapped=FALSE;
      uo->transparent=FALSE;

      /* start of object image: */
      mem=(unsigned char *)anzahlp+obj_header->offset;

#if 0
      /* convert from big endian: */
      plane=(UWORD *)mem;
      for(y=0;y<obj_header->hoehe * obj_header->breite_bytes/2; y++)
	{
	  be_2(plane+y);
	}
#endif
      
#if 0
      /* allocate a new XImage and put object into it. */
      picture=malloc((obj_header->breite*2)*(obj_header->hoehe*2));
      maskmem=malloc((obj_header->breite*2)*(obj_header->hoehe*2));
      
      ximage=XCreateImage(x_display,x_vis,x_depth,ZPixmap,0,picture,
			  obj_header->breite*2,obj_header->hoehe*2,
			  8,obj_header->breite*2);
      mask=XCreateImage(x_display,x_vis,x_depth,ZPixmap,0,maskmem,
			obj_header->breite*2,obj_header->hoehe*2,
			8,obj_header->breite*2);
#endif 
      for(y=0;y<obj_header->hoehe;y++)
	{
	  for(raster_akt=0;raster_akt<raster_anzahl;raster_akt++)
	    if (y<x_palette[raster_akt].y) break;
	  raster_akt--;
	  
	  for(x=0;x<obj_header->breite;x++)
	    {
#if 0
	      bb_adr=y*obj_header->breite_bytes+((x>>4)<<3);
	      plane=(UWORD *)&mem[bb_adr];
	      b_offset=15-(x%16);
#if 0
	      printf("bb_adr: %d   b_offset: %d\n",bb_adr,b_offset);
	      getchar();
#endif
	      b_mask=1<<b_offset;
	      color=0;
	      if (*plane++ & b_mask) color+=1;
	      if (*plane++ & b_mask) color+=2;
	      if (*plane++ & b_mask) color+=4;
	      if (*plane++ & b_mask) color+=8;
#else
	      color = atari_get_pixel(mem, obj_header->breite_bytes, x, y);
#endif
	      Uint8* pixels = (Uint8*)uo->surface->pixels;
	      pixels[y * uo->surface->pitch + x] = color;
#if 0
	      picture[(y*2)*(obj_header->breite*2)+x*2]=
		picture[(y*2)*(obj_header->breite*2)+x*2+1]=
		  picture[(y*2+1)*(obj_header->breite*2)+x*2]=
		    picture[(y*2+1)*(obj_header->breite*2)+x*2+1]=
		      x_palette[raster_akt].pal[color];
	      
	      maskmem[(y*2)*(obj_header->breite*2)+x*2]=
		maskmem[(y*2)*(obj_header->breite*2)+x*2+1]=
		  maskmem[(y*2+1)*(obj_header->breite*2)+x*2]=
		    maskmem[(y*2+1)*(obj_header->breite*2)+x*2+1]=
		      (color!=0) ? 0 : 255;
#endif	      
	    }
	}

#if 0
      show_surface(uo->surface);
      SDL_Delay(3000);
#endif
      
#if 0
      uo->image=ximage;
      uo->mask=mask;

      // We use the x/y_neu variable to point to memory of
      obj_header->x_neo=(unsigned int)uo>>16;
      obj_header->y_neo=(unsigned int)uo & 0xffff;
#endif 
      
#if 0
      XPutImage(x_display,x_window,x_gc,ximage,0,0,0,0,320*2,200*2);
#endif
    }
}

void mcode72(int rasterzeile, int anzahl)
{
  /* Blendet eine bereits bestehende Rasterzeile ab */
  UWORD *pal;
  
  pal = get_palette(rasterzeile);
  if (pal == NULL) return;
  fade_palette(pal, anzahl);
}

void mcode73(int x, int y)
{
  /* setzt die Maus auf einen bestimmten Punkt */
}

void mcode74(void)
{
  /* Schaltet die Maus ab */
}

void mcode75(void)
{
  /* Schaltet die Maus an */
}

#if 0  
draw_graphics(win, gc, window_width, window_height)
Window win;
GC gc;
unsigned int window_width, window_height;
{
  int x, y;
  int fd;
  unsigned char mem[80000];
  unsigned short size;
  unsigned short *plane;
  int i;
  int bb_adr;
  int b_offset;
  int color;
  static int first=1;
  static XImage *ximage;
  unsigned char *picture;

  if (first)
    {
      first=0;
      picture=malloc(640*400);
      ximage=XCreateImage(x_display,x_vis,x_depth,ZPixmap,0,picture,
			  640,400,8,640);
      
      if ((fd=open("/home/bubeck/odie.pi1",O_RDONLY))<0)
	{ perror(NULL); exit(-1); }
      read(fd,mem,34);
      read(fd,mem,sizeof(mem));     /* read data */
      close(fd);
      
      for(y=0;y<200;y++)
	for(x=0;x<320;x++)
	  {
	    bb_adr=y*160+((x>>4)<<3);
	    plane=(short *)&mem[bb_adr];
	    b_offset=15-(x%16);
#if 0
	    printf("bb_adr: %d   b_offset: %d\n",bb_adr,b_offset);
	    getchar();
#endif
	    color=0;
	    for(i=0;i<4;i++)
	      if (plane[i]&(1<<b_offset))
		color+=1<<(i);
	    
	    picture[y*2*640+x*2]=c_index[color];
	    picture[y*2*640+x*2+1]=c_index[color];
	    picture[(y*2+1)*640+x*2]=c_index[color];
	    picture[(y*2+1)*640+x*2+1]=c_index[color];
	  }
    }

  XPutImage(x_display,win,gc,ximage,0,0,0,0,320*2,200*2);
}

TooSmall(win, gc, font_info)
Window win;
GC gc;
XFontStruct *font_info;
{
        char *string1 = "Too Small";
        int y_offset, x_offset;

        y_offset = font_info->ascent + 2;
        x_offset = 2;

        /* output text, centered on each line */
        XDrawString(display, win, gc, x_offset, y_offset, string1, 
                        strlen(string1));
}
#endif
