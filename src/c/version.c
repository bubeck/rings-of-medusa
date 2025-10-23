
/* In dieser Datei wird eingestellt, was für eine Version von

   Rings of Medusa II - The Return of Medusa 
   (C) 1992 by Till Bubeck
	 
	 erstellt werden soll. */


#define COPYPROT 								/* Version mit Kopierschutzabfrage */

/* Hier werden die verschiedenen Sprachtypen definiert: */
#define _DEUTSCH 1
#define _ENGLISCH 2
#define _FRANZ 3

#define SPRACHE _ENGLISCH				/* Welche Sprache kompilieren? */

#define v1 1
#define v2 1
#define v3 5

#define versionsnr "1.15"               /* Versionsnr */

/* Versionen:

    0.01: erste Version, Beginn der Aktienkurse (9.3.1990)
					Bunker, Archiv, draw_obj, neue Raster, neue tastaturroutine,
					viele interne Änderungen
					-> PCW-Show, T. Zimmermann (12.09.90)
		0.02:	nahezu alle Betriebsystemaufrufe rausgeschmissen.
		0.03:	Erste Amiga-Version, Carsten Borgmeier (Power Play Preview)
		0.04: Expressversion für European Computer Trade Show at London
					Erste Objekte in den Bunkern, Roulette fertig (11.4.1991)
		0.05:	neue GMAPs, neue TMAPS, sämtliche Konstanten angepaßt, 
					neue Stadtgrafiken, Schilder, Aufzüge in den Bunkern
					-> erste Volltestversion (10.5.1991)
		0.06: Intro, Automapper, Türen bleiben offen (28.5.1991)
		0.07: Items können bewegt werden, betretbare Wände, Windows in den Städten,
				  verkleinerte Items, neue Mine und Kampfsprites (ASM-Version) (6.6.1991)
		0.08: 
		0.09: komplett neue Paletten in den Bunkern, bewegte Gegner, Musik
		0.10: Amiga Joker Version (20.6.1991)
		0.11: animierte Gegner, neue Reagenzgläser und Objekte,
					Notizblockbug entfernt (29.6.1991)
					-> ASM, Powerplay, Starbyte, Zimmermann
		0.12: Gefangene, Reagenzgläser mit Funktion, Totenkopf.
					-> Starbyte, Zimmermann (4.7.1991)
		0.13: -> Starbyte (8.7.1991)
		0.14: 1. Version des Spielerschusses, Laden und Abspeichern in den Bunkern,
				  Taschenlampe jetzt mit Funktion. -> Starbyte (9.7.1991)
		0.15: Laden und Abspeichern 2. Teil, Schuß des Spielers, Schuß macht heller,
		      Kleidung hat einen Sinn, Prisonerbilder, Charakterausgabe.
					-> Starbyte, ASM (10.7.1991)
		0.16: -> Zimmermann (12.7.1991)
		0.17: Passwortabfrage, neue Cheatabfrage, additives Schiessen, Finale,
					Gegnerschuß jetzt komplett.
		0.18: erste Endversion, noch ohne Schiffskampf.		
		0.19: viele Fehler behoben, Suchradius wird angezeigt, kaputter Schiffskampf
					neue Verteilung der Objekte, bessere Passwortabfrage
					-> Protestversion Starbyte (30.7.1991)
		0.20: Schiffskampf, ICC #2 (1.8.1991)
		1.00: Schiffskampf okay, sonstige Bugs behoben.
					-> ASM, Starbyte (1.8.1991)
		1.01: Roboter schießen nun auch von hinten, Schwierigkeitsgrad des Schiffskampfes
					erhöht. -> ASM (7.8.1991)
		1.02: Lade/Tastaturbug behoben. Neue Passwörter.
					-> Master Atari, Test Amiga (12.8.1991)
		1.03: Roboter nicht auf Aufzugstüren.
		      -> Master Atari (13.8.1991)
		1.04: Amigaversion, Atari Master -> Starbyte, Zimmermann (15.8.1991)
		1.05: Objekte können geworfen werden, kleinere Bugs beseitigt, neue Passwörter
					Amiga, Atari Master -> Starbyte (19.8.1991)
		1.06: Passwortabfrage jetzt im Hellen, "Faites vos jeux".
					Amiga, Atari Master -> Starbyte (20.8.1991)
		1.07: Handshake für Tastatur auf 75 s angepaßt (Amiga 2000-A)
					Amiga Master -> Starbyte (28.8.1991)
		1.08: Suchrahmenbug, Schildbug entfernt
					Atari, Amiga Master -> Starbyte (9.9.1991)
		1.09: Grenze-nicht-passierbar-Bug entfernt (nach Landen), training[] wird mit
		      abgespeichert, bessere Maussteuerung bei move_objekt()
					Amiga, Atari Master -> Starbyte (24.9.1991)					
		1.10: Suchbunkerbug beseitigt.
					Amiga, Atari Master -> Starbyte (25.9.1991)
		1.11: neuer LOADER.SRC mit RAM-Test ohne $ff8001-Zugriff (ST)
					Schlüsselwerfbug, Datendiskettenformat umgestellt
					Amiga, Atari Master -> Starbyte (7.10.1991)
		1.12: englische Version, abschalten der Intro beim Amiga
		      Amiga Master -> Starbyte (2.12.1991)
		1.13: englische Version mit englischen Passwörtern
		      Amiga, Atari Master -> Starbyte (3.1.1992)
		1.14: korrigierte englische Passwörter, wegen Handbuchumbruch
		      Amiga, Atari Master -> Starbyte (27.1.1992)
		1.15: englische Insertmeldung
		      Amiga Master -> Starbyte (10.2.1992)
					
		
    */

