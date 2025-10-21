;
; Batchfile fr Rings of Medusa II
; ½ 1990 by Till Bubeck
;
megamax\ccom.ttp main.c -i \..,megamax\headers\ >errors.out
megamax\ccom.ttp init.c -i \..,megamax\headers\ >errors.out
megamax\ccom.ttp city.c -i \..,megamax\headers\ >errors.out
megamax\ccom.ttp city2.c -i \..,megamax\headers\ >errors.out
megamax\ccom.ttp land.c -i \..,megamax\headers\ >errors.out
megamax\ccom.ttp atari.c -i \..,megamax\headers\ >errors.out
megamax\ccom.ttp io.c -i \..,megamax\headers\ >errors.out
megamax\ccom.ttp fight.c -i \..,megamax\headers\ >errors.out
megamax\ccom.ttp dungeon.c -i \..,megamax\headers\ >errors.out
megamax\ccom.ttp leisten.c -i \..,megamax\headers\ >errors.out
megamax\ccom.ttp raster.c -i \..,megamax\headers\ >errors.out
megamax\ld.ttp -v megamax\init.o leisten.o raster.o dungeon.o main.o megamax\line_a.o mcode.o fight.o atari.o init.o city.o city2.o land.o io.o -o MEDUSA.prg megamax\libc.a

