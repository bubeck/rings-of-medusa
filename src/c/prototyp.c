
#include "x.h"

/* Prototype-Deklarationen:
   » 1990 by Till Bubeck, Ziegeleistr. 28, 7056 Weinstadt, Tel: 07151/66437   */


uint16_t swap_uint16(uint16_t val);

/* Prototypen für: RASTER.C: */

void pic_move(void *quelle,int y1,int y2,int y3);
void cpy_raster(void *quelle, void *ziel,int x1,int y1,int x2,int y2,int x3,int y3);
long load_objekte(char *filename,void *adr);
void convert_objekte(SHARED_OBJECT *sh_obj);
int shrink_obj_breite(int size,int ob_nr,void *ob_mem);
int shrink_obj_hoehe(int size,int ob_nr,void *ob_mem);
FLAG objekt_exists(int ob_nr, void *ob_mem);
int objekt_breite(int ob_nr,void *ob_mem);
int objekt_hoehe(int ob_nr, void *ob_mem);
void *objekt_surface(int ob_nr, void *ob_mem);
raster_t *objekt_palette(int ob_nr,void *ob_mem, int raster_no);
unsigned int move_objekt(int ob_nr,void *ob_mem,int modus,int ziel_y,void *ziel_mem,unsigned int clip_modus,int clipx1,int clipy1,int clipx2,int clipy2,FLAG switch_scr);
void draw_shrink_obj(int size,void *shrink_buff,int ob_nr,void *ob_mem,int modus,void *scr,int x,int y);
void draw_shrink_obj_part(int size,void *shrink_buff,int ob_nr,void *ob_mem,int x1,int y1,int x2,int y2,int modus,void *scr,int x,int y);
void shrink1(float size,void *shrink_buff,int ob_nr,void *ob_mem,int x_start,int y_start,int x_end,int y_end,int modus,void *scr,int x_scr,int y_scr);
void shrink2(float size,void *shrink_buff,int ob_nr,void *ob_mem,int x_start,int y_start,int x_end,int y_end,int modus,void *scr,int x_scr,int y_scr);
void draw_grow_obj(int ob_nr,void *ob_mem,int modus,void *screen,int x,int y);
void draw_obj_all(void *ob_mem,void *screen);


/* Prototypen für: ATARI.C: */

void flash_amiga(void);
void hippel_sfx(int sound_nr);
void init_atari(void);
void init_hardware(void);
long wait_key(void);
void make_bus_error(void);
void plot_pixel(int color,int x,int y);
void programmende(void);
void init_archiv(void);
void fill_ramdisk(void);
long ram_needed(int ram_max);
void load_digisound(char *filename,void *adresse);
void load_sprites(char *filename);
void rahmen(int color,int x1,int y1,int x2,int y2);
unsigned char wandel(unsigned char zeichen);
void debug(int nr,char text[],int var);


/* Prototypen für: CITY.C: */

void city(void);
void calc_gehalt(void);
long sum_goods(void);
long sum_waren(void);
long sum_soldaten(void);
long frei_waggon(void);
long frei_cargo(void);
long frei_war(void);
long sum_rohstoffe(void);
long sum_stall(void);
void city_restore(int num);
void c_pic(char *filename);
void city_screen(void);
void city_info(int num);
void hafenkneipe(void);
void chg_price(void);
void chg_rasse(void);
void next_ship(int *index,int nummer[]);
void reederei(void);
void show_ships(int ship_type,int nr);
void store(int num);
void park(int num);
void garage(int num);
void rohstoffhandel(int num);
void test_schulden(void);
void calc_zinsen(void);
void casino(int num);
void gewinne(int feld,EINSATZ einsatz[]);
void einsatz_machen(EINSATZ einsatz[]);
void roulette_money(void);
void draw_einsatz(EINSATZ einsatz[],int nr);
int move_chip(int chip_nummer);
void draw_roulette(void);
int rotier_roulette(void);
int feld_weiter(void);
void draw_feld(int feld_nr);
void port(int num);
void sea_soldier(void);
void sea(void);
void land(void);
FLAG try_to_load(void);
FLAG try_to_sold(void);
void such_hafen(void);
void such_eingang(void);
void such_land(int grund_wert);
void armory(int num);
void writexy_ausruestung(int company);
void writexy_aus(int nr,int x,int company);
void status(char txt1[],char txt2[]);
FLAG is_mine(int land,int x,int y);
void leave(int num);
void barracks(int num);
void draw_delay(int x,int y,unsigned char wert);


/* Prototypen für: CITY2.C: */

void subway(void);
void bank(int num);
void aktienmarkt(void);
void buy_sell(char *name,int waren_max,char *warenn,int warenn_len,uint16_t waren_preis[],long waren_city[],long waren_player[],int ort);
void show_buy_sell(char *name,int waren_max,char warenn[],int warenn_len,uint16_t waren_preis[],long waren_city[],long waren_player[],int ort,int seite,int waren_nummer);
void store_goods(int waren_max,char warenn[],int warenn_len,uint16_t waren_preis[],long waren_city[],int seite,int ort);
void your_goods(int waren_max,char warenn[],int warenn_len,long waren_player[],int seite);
void einrahmen(int warennummer,int seite);
void ausrahmen(int warennummer,int seite);
long kurswert(int waren_nummer);
void get_city_werte(int num);
double aktie_fun(double x);
void chart_show(int waren_nummer);
void open_window(int x0,int y0);
void close_window(void);


/* Prototypen für: DUNGEON.C: */

FLAG dungeon(int nr);
void aufzug(unsigned int feld);
void schliesse_tuer(void);
int bedien_tuer(int item_nr);
void oeffne_tuer(void);
void press_button(int nr);
void release_button(void);
void berechne_richtung(void);
void print_richtung(void);
void wandgelaufen(void);
void draw_bunker(FLAG moved);
void draw_dungeon(void *screen);
void draw_hidden(BUNKERWAND wand[],int wandcounter,int x1,int x2,void *screen);
void draw_transparent(int ganz_hinten,BUNKERWAND wand[],int wandcounter,int x_links,int x_rechts,void *screen);
FLAG such_kante(BUNKERWAND wand[],int wandcounter,FLAG only_solid,int *x1,int *akt,int *x2);
int bau_waende(FLAG spiegeln,BUNKERWAND *waende);
void set_bunker_pal(void);
void do_taschenlampe(void);
void draw_whole_automap(void);
void draw_automap(int wert,int x,int y);
void draw_kasten(int farbe,int x,int y);
void map_update(void);
void draw_updates(void);
void draw_player(void);
void insert_automap(int wert,int x,int y);
void put_automap(int wert,int x,int y);
int get_automap(int x,int y);
void bedien_item(int x,int y);
int belade_figur(int item_nr,int member_nr);
void draw_haende(void);
void draw_hand(int member);
void draw_taschen(int member_nr);
void draw_tasche(int tasche_nr,int member_nr);
void charakter_ausgabe(int member);
void bild_ausgabe(int member);
void schauen(int item_nr);
void mund(int start);
void draw_mund(int ob_nr);
void anziehen(int member);
long prot_erhoeh(int item_nr,int wertung);
void draw_kleidung(int item_nr,int x_center,int y,int text_x,int text_y);
int get_1bit(unsigned char feld[],int pos);
void put_1bit(unsigned char feld[],int pos,int wert);
int get_2bit(unsigned char feld[],int pos);
void put_2bit(unsigned char feld[],int pos,int wert);
FLAG liegt_item(int x,int y);
FLAG is_aufzug_door(int x,int y);
FLAG is_aufzug(unsigned int feld);
FLAG is_door(unsigned int feld);
FLAG is_open_door(unsigned int feld);
FLAG betretbar(unsigned int feld);
FLAG enemy_betretbar(unsigned int x,unsigned int y);
FLAG is_wand(unsigned int feld);
void init_dungeon(int nr,unsigned int startfeld);
void load_dungeon(int nr);
void init_objekte(void);
FLAG platz_in_item(void);
void load_waende(void);


/* Prototypen für: DUNG2.C: */

void change_all_reagenz(int reagenz,int relativ);
void change_reagenz(int member,int reagenz,int relativ);
void subtract_reagenz(int member,int reagenz,int relativ);
void drop_item(int *item_nr);
void drop_itemxy(int *item_nr,int x,int y);
FLAG is_tot(int member);
void draw_reagenz(void);
void draw_rea(int member);
void draw_rea_stand(int member,int rea);
void copy_reagenz(void);
void draw_glaeser(int member);
void draw_schiessflag(int member);
void draw_lampenanzeige(void);
void set_enemies(BUNKERWAND **waende);
void set_enemy(BUNKERWAND **waende,int x,int y,int tiefe,int unterkante,int verkleinerung);
void feuer_enemy(int nummer);
void do_enemy_shot(void);
void set_items(BUNKERWAND **waende);
void set_them(BUNKERWAND **waende,int x,int y,LAGE pos[]);
FLAG move_enemies(void);
void free_prisoner(void);
void hunger_durst(void);
int do_shoot(int item_nr);
FLAG check_4_enemy(int entfernung,int *item_nr);
void explosion(int size,int x,int y,int von,int bis);
void copy_bunker_screen(void);
void medusa(void);
void draw_medusa(int med_nr,int kug_nr);
void finale(void);


/* Prototypen für: FIGHT.C: */

void test_fight(void);
void minus_einheit(int company,long tote);
void angriff_spieler_stadt(int num,int armeenr);
void attack_city(int num);
void fight(int nr);
void init_kampf_sprites(void);
void save_sprites(void);
void init_k_sprites(int armee,int index);
void restore_sprites(void);
void animate(int start,int ende);
void kampf(char gegnern[]);
void mach_schreie(void);
void move_spieler(long durchgang);
FLAG terms(void);
FLAG pay_you(long angebot);
FLAG bloeff(void);
void move_einheiten(void);
void move_schild(void);
void check_end(char gegnern[]);
void computer_move(void);
void war(void);
void shoot_ari(void);
void attack(int company);
void flee(int company);
void calc_staerke(int armee_nr);
void calc_it(int nr,int untergrund,float training[][EIGENSCHAFTEN],float motivation);
void show_soldiers(void);
void verlier_schiff(int nr);


/* Prototypen für: INIT.C: */

void vari_init(void);
void load_pics(void);
void load_alerts(void);
void hol_maus(void);
void konvert(unsigned int *scr);
void konvert_back(unsigned int *scr);
void save_pic(void);
void swap(int *a,int *b);
void alloc_mem(void);
void show_free(void);
void load_music(void);
void init_medusa(void);
char *such_string(int nr,char *adr);
void re_initialize(void);
void unterschrift(char *filename,int x,int y);
void anfangseinstellungen(void);
void intro(void);
void intro_off(void);
void decrypt(unsigned char *txt,long laenge);
void load_game(void);
void show_user_dir(char dir[11][70]);
void load_it(int nr);
long copyvar(FLAG saveflag);
void save_it(int nr);
void save_game(void);
void totenkopf(void);
void zeichne_totenkopf(int y);


/* Prototypen für: IO.C: */

void od(void *adr);
size_t file_size(char *filename);
void create_data_filename(char *filename_in, char *filename_out);
void linker(int file_nr,int fun_nr,int par1,int par2,void *par3);
void writexy(int farbe,int x,int y,char string[]);
void writexy_it(int farbe,int hintergrund,int x,int y,char string[]);
void writexy_trans(int farbe,int x,int y,char string[]);
void writexy_fast(int x1,int y1,char string[]);
void center(int farbe,int y,char str[]);
void writexy_money(int x,int y);
FLAG yes_no(char string[]);
void show_window(char string[]);
void show_windxy(int x,int y,char string[]);
FLAG word_wrap(int breite,char *zeilen[5],char string[]);
void hide_window(void);
void money_alert(void);
void dungeon_alert(char string[]);
void alert(char string[]);
void alertxy(int x,int y,char string[]);
void grow_box(int x0,int y0,int breite,int hoehe);
void shrink_box(int box_x,int box_y,int box_breite,int box_hoehe);
void draw_shapes(void);
void draw_mobs(void);
void delete_mobs(void);
void wait_once(int anzahl);
void wait_sync(int anzahl);
void wait_sync_klick(int anzahl);
void wait_sync_noshow(int anzahl);
void cls(void);
void home(void);
void print(char str[]);
void print_long(long wert);
void print_int(int wert);
void nprint(char str[]);
void nprint_long(long wert);
void nprint_int(int wert);
void nhome(void);
void ncrlf(void);
void crlf(void);
void writexy_date(void);
void leiste_oben(char grund[]);
void init_oben(void);
void show_pos(void);
char *calc_pos(int pos,int x,int y);
void klick(void);
FLAG maus_in(int x1,int y1,int x2,int y2);
void wait_klick(void);
void button_leiste(void);
void pause_game(void);
void schuldner(void);
void selekt(int i);
void change_pixel(int i,int alt,int neu);
void prep_date(void);
long zufall_long(long grenze);
int zufall(int grenze);
int input(int x,int y,int len,char eingabe[]);
void rec(int x1,int y1,int x2,int y2);
void drec(int x1,int y1,int x2,int y2);
void Krec(int x1,int y1,int x2,int y2);
void wait_mouse(int a);
char *fstr(char *a,float b);
long runde(float wert);
char *str(int len,long zahl);
void dlstr(long datum,char string[]);
char *dat_kurz(long datum);
char *dstr(long datum);
char *space(int len);
int load(char *filename,void *adr,long offset,long laenge);
int load_bibliothek(char *filename,void *buffer);
void Sm(void);
void Hm(void);
void initspr4map(void);
void sprite_init(void);
void formular(void *screen,int y);
char *strapp(char str1[],char str2[]);
void clear_screen(void *adr);
void copy_screen(void *quelle,void *ziel);
void vbl_routine(void);
void swap_screens(void);
void switch_screens(void);
void init_maus(void);


/* Prototypen für: LAND.C: */

FLAG land_map(short nr,FLAG scrollen,int rich,int x,int y);
void load_map(int nr);
void load_ground(int nr);
void del_rohstoffe(void);
void put_minen(int nr);
void chg_ground(int nr);
void ground_change(int wert,int land,int x,int y);
void land_an(void);
void clear_time(void);
void check_time(void);
void show_ground(void);
void test_mine(void);
void test_city(void);
void test_bunker(void);
void such_platz(void);
void test_untergrund(void);
void get_ground(void);
void put_raster(int wert,int xr,int yr);
void put_untergrund(int wert,int x,int y);
int get_untergrund(int x,int y);
int get_raster(int x,int y);
int get_traffic(int land,int x,int y);
void get2_coordinates(void);
void update_coordinates(void);
FLAG is_water(int wert);
FLAG is_ground(int x,int y);
void test_fluss(int x,int y);
void test_ground(void);
void show_schild(int nr);
void schiff_test(void);
int frei_flotte(void);
void such_xy(int nr);
void such_weg(int nr);
FLAG such_new(int nr,int anzahl);
void set_armies(void);
void move_armies(void);
void move_army(register nr);
void kreuzung(register nr);
void switch_sprites(void);
int such_sprite(void);
void stadt_erreicht(int nr);
void show_traffic(int land);
void writexy_werte(int nr);
FLAG such_ort(int nr,int wert);
void neue_richtung(int nr);
void make_world(void);
int such_insel(void);
short get_land(int x,int y);


/* Prototypen für: MAIN.C: */

int main(int argc, char *argv[]);
void draw_peilgeraete(void);
void delete_peilgeraete(void);
void sea_move(void);
int rnd_winkel(void);
void peilung(void);
void peilgeraet_aus(void);
void such_rohstoffe(void);
void such_roh(long pio_zahl);
FLAG such_roh2(int radius,int start_wert,int end_wert);
void mach_mine(void);
void schatz_found(void);
void neue_mine(void);
void besuch_mine(int nr);
int zeichne_mine(int nr);
int draw_mine(void *screen,int nr);
void animier_mine(int oel_pos);
void check_cheat(void);
void cheat_auswert(FLAG bunker);
void cheat(int x,int y,char text[],FLAG cheat_var);
void simulation(void);
void vulkane(void);
void ani_vulkan(void);
void such_vulkan(void);
void cdecl int_err(char *error);
void internal(char *error);
void daily_events(void);
void monthly_events(void);
void editor(void);
long minimum(long w1,long w2);
void balken(int x,int y,long max,long real);
void game_over(char txt1[],char txt2[]);
void make_adresses(void);
void blitz(int color);
void autsch(void);


/* Prototypen für: LEISTEN.C: */

void do_board(void);
void bunker_steuerung(void);
void check_scroller(void);
void wait_close(void);
void option_city(void);
void option_city_ende(void);
void new_option(void);
void draw_options(void);
void writexy_anzeige(void);
void clear_buttons(FLAG redraw);
void redraw_buttons(long butt);
void new_buttons(long butt);
void init_buttons(FLAG redraw,long butt);
void show_buttons(FLAG redraw);
void copy_buttons(void);
void options(void);
void flip(int option_nr);
void dehn(int option_nr);
void zeichne(int option_nr,int bildzeile,int orgzeile);
void stauch(void);
void load_leisten(void);

void vbl_execute();
SDL_Surface *get_obj_surface(int ob_nr, void *ob_basis);

