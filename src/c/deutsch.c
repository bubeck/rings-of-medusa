/* Deutsche Version: */

#define DEUTSCH

/* This is a list of all strings of Rings of Medusa II (The Return of Medusa)

    Copyright 1991 by Till Bubeck, Ziegeleistr. 28, 7056 Weinstadt, West-Germany
                      Tel.: 07151-66437
		
		or contact:			Starbyte Software, Nordring 71, 4630 Bochum 1, West-Germany
											Tel.: 0234-680460
		
    to translate R.o.M. you've to translate all strings, except the one's
    marked with "!".
    You can find the maximum length of each string at the end of the line
    in comment.

   * *********************************************************************
   * IMPORTANT: Don't make your string longer than the maximum
   *            length!!!
   *            (Otherwise I have to abbreviate your translation, that is
   *             worse than your badest abbreviation....)
   * *********************************************************************

    If no length is written, than it's 23!

    If you find, "#" at the end of the line, than your translation has
    to be shorter, or at maximum has the same length than the german one.

    ~ in a string means CR+LF, when it's printed on the screen. Don't
    change them. If "~" appears in a string try to make each line (between "~")
    as long as the german one. +/- 2, but try to make it shorter, otherwise
    it can make problems.

    If the right side of a string is filled with spaces up to a special length,
    than your translation has to be shorter, or at maximum has the same length
    as the german one. If your translation is shorter, than fill the rest
    of the string with spaces, too.

    if there's a ":" in a string, then you've to translate this string in such
    a way, that the ":" doesn't change its position!

		******************************************************************
		*
		* Please do NOT kill any kind of comments
		*
		******************************************************************
		
		Build: There's a special function called build(). This function helps you
					 to translate different types of grammar.

			Problem:
					 e.g.: The programm wants to know: "ENTER NEW YORK CITY?"
					 tranlated into german: "NEW YORK CITY BETRETEN?"
					 
					 The position of "NEW YORK CITY" is different. In the english version
					 "NYC" is in the middle of the string between "ENTER" and "?".
					 In the german translation "NYC" is at the beginning
					 of the string followed by "BETRETEN?".
					 
			Solution:
					 For the last example the string looks:
					  	english: "ENTER #0?"
					   	german: "#0 BETRETEN?"
					 That means that #0 represents the city.
					 
					 another example:
					 		"I FOUND #0 AT POSITION #1."
							
							#0 represents treasure, iron, gold, etc.
							#1 represents a special position
							
					 If you like to, you can change the position and order of all #x.
					 
					 possible:
					 		"I FOUND #1 AT POSITION .#0"
							no sense, but writes e.g. "I FOUND 23 West/70 South AT POSITION .Gold"
							
					 For better understandings, I will write (e.g. [treasure][position]) behind
					 strings using #x. That means that #0 represents a kind of treasure and #1
					 reprensents a position.

	comments by the translator:
					If you've any suggestions or problems concerning the translation,
					please write here.
					If you're not sure, whether your translation is correct, than DO
					translate it, and write a short comment,
						e.g. not sure for romstrXX, because I don't understand the contents...
						
										 	
    */

#define romstr001 "includes.c"							/* ! */
#define romstr002 ".   "									/* ! */
#define romstr003 "PLEASE PRESS ANY KEY."					/* ! */
#define romstr004 "includes.c"							/* ! */
#define romstr005 "UNKNOWN MODUL"							/* ! */
#define romstr006 " DEIN GELD:"
#define romstr007 "DU HAST NICHT GENUG GELD."
#define romstr008 "DU HAST SEIT LANGER ZEIT SCHULDEN. BEVOR DU DIE NICHT ZURÜCKGEZAHLT HAST, BEKOMMST DU KEINE WEITEREN WAREN."
#define romstr009 " "											/* ! */
#define romstr010 " "											/* ! */
#define romstr011 ", "										/* ! */
#define romstr012 ". "										/* ! */
#define romstr013 " "											/* ! */
#define romstr014 "/"											/* ! */
#define romstr015 "CROWN-PACKED FILE"						/* ! */
#define romstr016 "includes.c"							/* ! */
#define romstr017 "D:\\MEDUSA.DAT\\MEDUSA_2.COL\\ROM_GRFX.0"	/* ! */
#define romstr018 "\\MEDUSA.DAT\\MEDUSA_2.COL"				/* ! */
#define romstr019 ""											/* ! */
#define romstr020 "VARIABLE WIND_FORM ZU GROSS!"				/* ! */
#define romstr021 "WIND FORM #0"							/* ! */
#define romstr022 "X:"										/* ! */
#define romstr023 " Y:"										/* ! */
#define romstr024 "CURSOR X:"								/* ! */
#define romstr025 " CURSOR Y:"							/* ! */
#define romstr026 "C:\\MEDUSA.PI"							/* ! */
#define romstr027 "FREIER SPEICHER: #0"						/* [number] */
#define romstr028 "V #0.#1#2"								/* ! */
#define romstr029 "STEFFI"								/* ! */
#define romstr030 "FÜR STEFFI"						
#define romstr031 "ICH LIEBE DICH!"
#define romstr032 "               "
#define romstr033 "   STEFFI,   "
#define romstr034 "COPYRIGHT 1991 BY TILL BUBECK"	
#define romstr035 "GESCHRIEBEN VON: TILL BUBECK"
#define romstr036 "GRAFIK VON: TORSTEN ZIMMERMANN"
#define romstr037 "MUSIK VON: JOCHEN HIPPEL"
#define romstr038 "ERSTELLE DIE SPIELWELT."
#define romstr039 "VERSION (GERMANY) "
#define romstr040 " C+"										/* ! */
#define romstr041 " C-"										/* ! */
#define romstr042 "RELEASE DATE: "
#define romstr043 "RELEASE TIME: "
#define romstr044 "WILLKOMMEN ZU:"							/* 20 */
#define romstr045 "THE RETURN OF MEDUSA."					/* 20 */
#define romstr046 "BITTE NAMEN EINGEBEN:"					/* Remember : */
#define romstr047 "CIRION"								/* ! */
#define romstr048 "LADE: BITTE WARTEN..."					/* no : */
#define romstr049 "BILDBUFFERS ZU KLEIN:"					/* ! */
#define romstr050 ""											/* ! */
#define romstr051 "SPIELSTAND LADEN"
#define romstr052 "UNBENUTZT"
#define romstr053 "LADE JETZT:"
#define romstr054 "RoM"										/* ! */
#define romstr055 "DIES IST KEINE SPIELSTANDDISKETTE. SOLL SIE ZU EINER GEMACHT WERDEN?"
#define romstr056 "RoM"										/* ! */
#define romstr057 "UNBENUTZT"
#define romstr058 "RoM"										/* ! */
#define romstr059 "DIESER SPIELSTAND IST MIT EINER ÄLTEREN VERSION VON ROM 2 GESPEICHERT -> NICHT LADBAR."
#define romstr060 "FEHLER BEIM ABSPEICHERN! (DISK SCHREIBGESCHÜTZT ODER DEFEKT?)"
#define romstr061 "DU KANNST NUR ALLE 5 MINUTEN SPEICHERN."
#define romstr062 "SPIELSTAND SPEICHERN"
#define romstr063 "BITTE NAMEN EINGEBEN:"					/* May be longer */
#define romstr064 "includes.c"							/* ! */
#define romstr065 " "											/* ! */
#define romstr066 "ACHTUNG! NÄCHSTEN MONAT MUSST DU #0 ZAHLEN. DU HAST NUR: #1"		/* [costs][money] */
#define romstr067 "INFO"
#define romstr068 "INFO"
#define romstr069 "BEVÖLKERUNG  :"
#define romstr070 "  BELEGUNG"
#define romstr071 ":"
#define romstr072 "         STADT VON:"
#define romstr073 "MEDUSA."
#define romstr074 "PRODUZIERT:"
#define romstr075 "STATUSANZEIGE"
#define romstr076 "DU HAST KEINE SCHIFFE IN DIESER STADT, => DU BRAUCHST KEINE CREW."
#define romstr077 "DEIN ANGEBOT FÜR"
#define romstr078 " HEUER / MONAT: ^_      `a"
#define romstr079 "  LEUTE WOLLEN:"
#define romstr080 "     SCHIFF:"
#define romstr081 "SCHIFF-CREW:"
#define romstr082 "MANN ANBORD:"
#define romstr083 "  GEBRAUCHT:"
#define romstr084 "SUMME HEUER:"
#define romstr085 "WENN DU JETZT GEHST, WIRD DER REST DER INTERESSIERTEN LEUTE GEHEN. KNEIPE VERLASSEN?"
#define romstr086 "WENN DU DEN PREIS JETZT ÄNDERST, WIRD DER REST DER INTERESSIERTEN LEUTE GEHEN. PREIS ÄNDERN?"
#define romstr087 "WENN DU DIE RASSE JETZT ÄNDERST, WIRD DER REST DER INTERESSIERTEN LEUTE GEHEN. RASSE ÄNDERN?"
#define romstr088 "DU KANNST NICHT MEHR SCHIFFE BEKOMMEN, WEIL DU HÖCHSTENS #0 SCHIFFE HABEN KANNST."
#define romstr089 "DU HAST NICHT GENUG GELD UM #0 ZU KAUFEN:"				/* [ship] */
#define romstr090 "DIESER HAFEN KANN NUR 5 SCHIFFE BAUEN."
#define romstr091 "DIE REEDEREI"
#define romstr092 "HANDELSSCHIFF"
#define romstr093 "KRIEGSSCHIFF"
#define romstr094 "TYP        :"
#define romstr095 "PREIS      :"
#define romstr096 "CREW       :"
#define romstr097 "GESCHÜTZE  :"
#define romstr098 "IM BESITZ  :"
#define romstr099 "  FERTIG AM:"
#define romstr100 "GELD ÜBRIG :"
#define romstr101 "FRACHT     :"
#define romstr102 "KAPAZITÄT  :"
#define romstr103 "WARENHAUS"
#define romstr104 "WARENHAUS"
#define romstr105 "WARENHAUS"
#define romstr106 "PARK"
#define romstr107 "PARK"
#define romstr108 "GEWÄLT.EINHEIT:"
#define romstr109 "GEHALT:"
#define romstr110 "SOLDATEN      : 100.00% ="
#define romstr111 ":   0.00% ="
#define romstr112 ":   0%"
#define romstr113 "SUMME GEHALT   :"
#define romstr114 "GEHALT / MONAT :"
#define romstr115 "MÄNNER ÜBRIG   :"
#define romstr116 "GEWÄHLTE MÄNNER: \\]^_      `abc"
#define romstr117 "%6.2f"										/* ! */
#define romstr118 "DU KANNST HIER KEINE SOLDATEN KAUFEN, WEIL DEINE ARMEE NICHT DA IST"
#define romstr119 "GARAGE"
#define romstr120 "GARAGE"
#define romstr121 "GARAGE"
#define romstr122 "JUWELIER"
#define romstr123 "JUWELIER"
#define romstr124 "CASINO"
#define romstr125 "CASINO"
#define romstr126 "IMPAIR"
#define romstr127 "PAIR"
#define romstr128 "ROUGE"
#define romstr129 "NOIR"
#define romstr130 "MANQUE"
#define romstr131 "PASSE"
#define romstr132 "(3X)"
#define romstr133 "DU HAST #0 AUF FELD '#1' GESETZT. GEWINN: #0 * #2 = #3"		/* [pool=Einsatz][position][number][number] */
#define romstr134 "FAITES VOS JEUX..."
#define romstr135 "GELD:"							/* up to 8 charakters */
#define romstr136 "RIEN NE VA PLUS!."
#define romstr137 "KLICK!"
#define romstr138 "HAFEN"
#define romstr139 "HAFEN"
#define romstr140 "HAFEN"
#define romstr141 "FLOTTE IN DIESER STADT"
#define romstr142 "LASSEN UND AN LAND GEHEN"
#define romstr143 "MIT FLOTTE ABLEGEN UND"
#define romstr144 "ALLE WAREN MIT AN BORD"
#define romstr145 "NEHMEN."
#define romstr146 "MIT FLOTTE IN SEE STECHEN,"
#define romstr147 "UND ALLE WAREN, SOLDATEN,"
#define romstr148 "UND GLEITER MITNEHMEN."
#define romstr149 "IN DIE HAFENKNEIPE GEHEN UND"
#define romstr150 "EINE CREW ANHEUERN"
#define romstr151 "EIN SCHIFF IN DER REEDEREI"
#define romstr152 "BESTELLEN."
#define romstr153 "HANDELSCHIFFE:"
#define romstr154 "KRIEGSSCHIFFE:"
#define romstr155 "GLEITER:"
#define romstr156 "DEINE ARMEE IST NICHT IN DIESER STADT."
#define romstr157 "DU KANNST DIESE STADT NICHT AUF DEM WASSERWEG VERLASSEN, WEIL DU KEINE SCHIFFE HIER HAST."
#define romstr158 "DU KANNST DIESE STADT NICHT AUF DEM WASSERWEG VERLASSEN, WEIL DU KEINE SCHIFFE HIER HAST."
#define romstr159 "DU KANNST HIER NICHT AN LAND GEHEN, WEIL DEINE ARMEE NICHT HIER IST."
#define romstr160 "DU BIST GELANDET."
#define romstr161 "DU BIST SCHON GELANDET."
#define romstr162 "DEINE FLOTTE KANN NICHT SO VIELE WAREN UND ROHSTOFFE TRANSPORTIEREN."
#define romstr163 "DEINE FLOTTE KANN NICHT SO VIELE SOLDATEN TRANSPORTIEREN."
#define romstr164 "HAFEN NOT FOUND"
#define romstr165 "LANDEINGANG NOT FOUND"
#define romstr166 "KEIN FREIES LANDFELD."
#define romstr167 "SUCH LAND() HAT KEIN FELD GEFUNDEN!"
#define romstr168 "WAFFENHÄNDLER"
#define romstr169 "WAFFENHÄNDLER"
#define romstr170 "   ZAHL AN SOLDATEN MIT:~"
#define romstr171 "  ZAHL AN SOLDATEN OHNE:~"
#define romstr172 " PREIS FÜR EINEN SOLDAT:"
#define romstr173 "EINHEIT:~"
#define romstr174 "  SUMME:"
#define romstr175 " WÄHLE: ^_           `a~"
#define romstr176 "KOSTEN:"
#define romstr177 "STATUS"
#define romstr178 "DIE ARMEE VON SIR "
#define romstr179 "ARMEE:"
#define romstr180 ":"
#define romstr181 "----------"
#define romstr182 "  KONTO:"
#define romstr183 "SCHIFF:"
#define romstr184 ":"
#define romstr185 "--"
#define romstr186 "DU BESITZT "
#define romstr187 " STÄDTE."
#define romstr188 "DU HAST "
#define romstr189 " MINEN."
#define romstr190 "DU HAST"
#define romstr191 " WAREN."
#define romstr192 "DU HAST VERLOREN WEIL,"
#define romstr193 "DU MUSST ZUERST DEIN SCHIFF VERLASSEN, BEVOR DU #0 VERLASSEN KANNST."			/* [city] */
#define romstr194 "DU KANNST DIE STADT NICHT VERLASSEN, WEIL DEINE GLEITER NICHT SO VIEL TRAGEN KÖNNEN."
#define romstr195 "STADT VERLASSEN"
#define romstr196 "#0 VERLASSEN?"										/* [city] */
#define romstr197 "KASERNE"
#define romstr198 "KASERNE"
#define romstr199 "DIE WACHE LÄSST DICH NICHT VORBEI!"
#define romstr200 "DEINE ARMEE:"
#define romstr201 "ZEITVERZÖGERUNG:"
#define romstr202 ":"
#define romstr203 "<-      ->"
#define romstr204 "----------"
#define romstr205 "--------"
#define romstr206 "\\]^_      `abc"
#define romstr207 "includes.c"								/* ! */
#define romstr208 "SUBWAY"
#define romstr209 "BANK"
#define romstr210 "BANK"
#define romstr211 "DIE ROYAL FALCON BANK"
#define romstr212 "     KONTOSTAND :"
#define romstr213 "   KREDITRAHMEN :"
#define romstr214 " GEWÄHLTES GELD : \\]^_        `abc"
#define romstr215 "  HEUTIGER ZINS : 4%"
#define romstr216 "    KREDIT-ZINS : 18%"
#define romstr217 "DIE ZINSBERECHNUNG WIRD GEMACHT, WENN SIE DIE"
#define romstr218 "BANK WIEDER BETRETEN."
#define romstr219 "AKTIENMARKT"
#define romstr220 "DU BEKOMMST #0 ZINS!"					/* [amount of money] */
#define romstr221 "DU MUSST #0 ZINS ZAHLEN."					/* [amount of money] */
#define romstr222 "DU MUSST #0 ZINS ZAHLEN, HAST ABER NICHT GENUG GELD. GEH UND VERDIENE ES."		/* [amount of money] */
#define romstr223 "DIE BANK ANGREIFEN BRINGT #0."				/* [amount of money] */
#define romstr224 "DIE ROYAL FALCON BANK MACHT KEINE GESCHÄFTE MIT BANKRÄUBERN WIE DU ES BIST."
#define romstr225 "AKTIENMARKT"
#define romstr226 "CHART-ANALYSE KOSTET 10000. TROTZDEM AUSFÜHREN?"
#define romstr227 "LAGER"
#define romstr228 "PREIS"
#define romstr229 "  ANZAHL"
#define romstr230 "DEINE WARE:    ANZAHL"
#define romstr231 "GEWÄHLTE WARE:"
#define romstr232 "ANZAHL     : \\]^_`abc"
#define romstr233 "PREIS      :"
#define romstr234 "HANDELSSCHIFFE:"
#define romstr235 "GLEITER:"
#define romstr236 "CHART-ANALYSE"
#define romstr237 "MAKLER-GEBÜHR: 2%"
#define romstr238 "%f"
#define romstr239 "NOTIERUNG:"
#define romstr240 "version.c"									/* ! */
#define romstr241 "STRINGS.C"									/* ! */
#define romstr242 "FILENAME.C"								/* ! */
#define romstr243 "CONST.C"									/* ! */
#define romstr244 "GRUND.C"									/* ! */
#define romstr245 "TYPEDEF.C"									/* ! */
#define romstr246 "GLOBAL.C"									/* ! */
#define romstr247 "PROTOTYP.C"								/* ! */
#define romstr248 "MCODE.C"									/* ! */
#define romstr249 "DU HAST KEINEN FREQUENZQUARZ."
#define romstr250 "IN DIESEM LAND GIBT ES KEINEN BUNKER."
#define romstr251 "OKAY SIR, WIR SUCHEN NACH ROHSTOFFEN..."
#define romstr252 "TUT MIR LEID, WIR HABEN NICHTS GEFUNDEN."
#define romstr253 "SCHATZ AN #0!"								/* [position] */
#define romstr254 "BUNKEREINGANG AN KOORDINATE #0 GEFUNDEN!"		/* [position] */
#define romstr255 "#0 AN #1."									/* [treasure][position] */
#define romstr256 "MINE ERSCHLIESSEN (KOSTEN: 30000)? DU HAST: #0"	/* [money] */
#define romstr257 "TUT MIR LEID, ABER WIR HABEN SCHON 20 MINEN."
#define romstr258 "DU BRAUCHST 30000 FÜR EINE MINE, HAST ABER NICHT SO VIEL GELD."
#define romstr259 "...SIR !!!! WIR HABEN EINEN SCHATZ GEFUNDEN! WERT: #0"		/* [value] */
#define romstr260 "DU HAST #0 KARREN MIT DRECK GEFÜLLT. DU HAST SIE VERLOREN."	/* [number] */
#define romstr261 "DRECK"
#define romstr262 "DU HAST KEIN FÖRDERSYSTEM."
#define romstr263 "DU HAST SCHON 3 FÖRDERSYSTEME."
#define romstr264 "DU HAST KEIN PUMPENSYSTEM!"
#define romstr265 "DU HAST SCHON 3 FILTERSTUFEN."
#define romstr266 "DU HAST KEINEN ÖLTANK!"
#define romstr267 "DU HAST SCHON 3 ÖLTANKS."
#define romstr268 "BIST DU SICHER, DASS DU DIESE MINE SCHLIESSEN WILLST?"
#define romstr269 "MINE"
#define romstr270 "SOVIEL WIE MÖGLICH AUF DIE~GLEITER LADEN.~~"
#define romstr271 "NEUES FÖRDERSYSTEM ERRICHTEN~(KOSTEN: 5000)~~"
#define romstr272 "NEUES PUMPENSYSTEM EINBAUEN~(KOSTEN: 1000)~~"
#define romstr273 "NEUEN ÖLTANK EINBAUEN~(KOSTEN: 4000)~~"
#define romstr274 "MINE SCHLIESSEN~(KOSTEN: 5000)"
#define romstr275 "LAGERHAUS:~~"
#define romstr276 "DIESE MINE WURDE GEÖFFNET~~"
#define romstr277 "UND FÖRDERTE BISHER ÜBER~~~~~~"
#define romstr278 "GLEITER:"
#define romstr279 "DRECK"
#define romstr280 "DRECK."
#define romstr281 "RINGS OF MEDUSA 2 - CHEATS:"
#define romstr282 "R: ROHSTOFFE ZEIGEN"
#define romstr283 "G: UNENDLICH GELD"
#define romstr284 "W: WAREN ANZEIGEN"
#define romstr285 "S: KAMPFSTÄRKE ANZEIGEN"
#define romstr286 "O: GEGNER ZEIGEN"
#define romstr287 "T: SCHÄTZE ANZEIGEN"
#define romstr288 "K: KASERNE BETRETEN"
#define romstr289 "+: GESCHW. ERHÖHEN"
#define romstr290 "0: GELD AUF NULL"
#define romstr291 "B: SEEKAMPF"
#define romstr292 "F: LANDKAMPF"
#define romstr293 "C: UNION CITY BETRETEN"
#define romstr294 "X: ALLE TÜRE OFFEN"
#define romstr295 "Z: RECHENZEIT ANZEIGEN"
#define romstr296 "M: EINEN MONAT WEITER"
#define romstr297 "D: BUNKER BETRETEN"
#define romstr298 "A: AUTOMAPPING"
#define romstr299 "L: LICHT VOLLE STÄRKE"
#define romstr300 "U: UNSTERBLICH IM BUNKER"
#define romstr301 "WELCHEN BUNKER MÖCHTEN"
#define romstr302 "SIE BETRETEN?"
#define romstr303 "BITTE NUMMER EINGEBEN:"
#define romstr304 "KEIN VULKAN GEFUNDEN!"
#define romstr305 "INTERNAL ERROR: \x22#0\x22."						/* ! */
#define romstr306 "BERECHNE..."
#define romstr307 "SPECIAL PRESS-VERSION"							/* ! */
#define romstr308 "NO. 3"											/* ! */
#define romstr309 "DEINE ARMEE WILL GELD,"
#define romstr310 "ABER DU HAST ZU WENIG."
#define romstr311 "ACHTUNG! DU HAST NUR NOCH FÜR EINEN MONAT GELD!"
#define romstr312 "NOTIZBLOCK"
#define romstr313 "NOTIZBLOCK"
#define romstr314 "0%"
#define romstr315 "100%"
#define romstr316 "ÜBERLADEN"
#define romstr317 " KEINE DA"
#define romstr318 "SAVE.C"										/* ! */
#define romstr319 "AUTSCH!"
#define romstr320 ""
#define romstr321 "Ramdisk"										/* ! */
#define romstr322 "ARAK"
#define romstr323 "TASARKO"
#define romstr324 "ROMOLUS"
#define romstr325 "MAOKI"
#define romstr326 "ISIKIN"
#define romstr327 "NAOMI"
#define romstr328 "TIBRO"
#define romstr329 "KORDON"
#define romstr330 "AREMOR"
#define romstr331 "ASCHAS"
#define romstr332 "STÄRKE"
#define romstr333 "KAMPFGEIST"
#define romstr334 "AUSDAUER"
#define romstr335 "WENDIGKEIT"
#define romstr336 "INTELLIGENZ"
#define romstr337 "ANDRAS"
#define romstr338 "JAVO"
#define romstr339 "SIGMA"
#define romstr340 "DELIA"
#define romstr341 "KRETUS"
#define romstr342 "SHEILA"
#define romstr343 "ROMERA"
#define romstr344 "WANDERER"
#define romstr345 "SPACY"
#define romstr346 "ZACK"
#define romstr347 "KENDO"
#define romstr348 "RAJAN"
#define romstr349 "RAZER"
#define romstr350 "LAWSON"
#define romstr351 "TYCOON"
#define romstr352 "NIEMAND"
#define romstr353 "INFANTERIE  "
#define romstr354 "PANZER      "
#define romstr355 "ARTILLERIE  "
#define romstr356 "AUFKLÄRER   "
#define romstr357 "HELIKOPTER  "
#define romstr358 "SANITÄTER   "
#define romstr359 "BODENTRUPPE "
#define romstr360 "GEWEHR"
#define romstr361 "GRANATEN"
#define romstr362 "BAZOOKA"
#define romstr363 "MENSCH  "
#define romstr364 "ELFE    "
#define romstr365 "ZWERG   "
#define romstr366 "HALBLING"
#define romstr367 "ORC     "
#define romstr368 "GNOM    "
#define romstr369 "RIESE   "
#define romstr370 "TROLL   "
#define romstr371 "OGRE    "
#define romstr372 "ZWARK   "
#define romstr373 "NICHT DA"
#define romstr374 "UNION CITY"
#define romstr375 "OUT'A'FUEL CITY"
#define romstr376 "ROBOT'S LEDGE"
#define romstr377 "FISH AND BONES"
#define romstr378 "VOGAS"
#define romstr379 "DESERT INN"
#define romstr380 "MC DUMNALDS"
#define romstr381 "GALACTICA"
#define romstr382 "CHICKEN TOWN"
#define romstr383 "EAGLE'S NEST"
#define romstr384 "ST FRANK'S"
#define romstr385 "ARCADIA"
#define romstr386 "PALM VILLAGE"
#define romstr387 "NEMESIS"
#define romstr388 "NEW MODE'S CASTLE"
#define romstr389 "CITY OF DARKNESS"
#define romstr390 "MEGALOPOLIS"
#define romstr391 "HOT VALLEY"
#define romstr392 "DEAD END"
#define romstr393 "VALIA"
#define romstr394 "MARKOMENIA"
#define romstr395 "INIFINITE VILLAGE"
#define romstr396 "VARTAS"
#define romstr397 "VLG. NO. 261"
#define romstr398 "CITY OF TABUN"
#define romstr399 "SLIME'S CITY"
#define romstr400 "OUTER SPACE"
#define romstr401 "ISLAND CAPITAL"
#define romstr402 "KUTTER"
#define romstr403 "LASTKAHN"
#define romstr404 "TANKER"
#define romstr405 "SCHNELLBOOT"
#define romstr406 "ZERSTÖRER"
#define romstr407 "KREUZER"
#define romstr408 "MILCH"
#define romstr409 "KÄSE"
#define romstr410 "SALAT"
#define romstr411 "ÄPFEL"
#define romstr412 "FLEISCH"
#define romstr413 "FISCHE"
#define romstr414 "WAFFEN"
#define romstr415 "CRACK"
#define romstr416 "ACID 54"
#define romstr417 "ARZNEI"
#define romstr418 "BATTERIEN"
#define romstr419 "QUARZ"
#define romstr420 "SKLAVEN"
#define romstr421 "FÖRDERTURM"
#define romstr422 "PUMPE"
#define romstr423 "ÖLTANK"
#define romstr424 "CYBORGS"
#define romstr425 "ANDROIDEN"
#define romstr426 "COMPUTER"
#define romstr427 "CHIPS"
#define romstr428 "KEROSIN"
#define romstr429 "GLEITER"
#define romstr430 "SCHATZ"
#define romstr431 "EISEN"
#define romstr432 "DIAMANTEN"
#define romstr433 "NITRIL"
#define romstr434 "ÖL"
#define romstr435 "GAS"
#define romstr436 "GIMMICK"
#define romstr437 "GOLD"
#define romstr438 "NO MONTH"
#define romstr439 "JANUAR"
#define romstr440 "FEBRUAR"
#define romstr441 "MÄRZ"
#define romstr442 "APRIL"
#define romstr443 "MAI"
#define romstr444 "JUNI"
#define romstr445 "JULI"
#define romstr446 "AUGUST"
#define romstr447 "SEPTEMBER"
#define romstr448 "OKTOBER"
#define romstr449 "NOVEMBER"
#define romstr450 "DEZEMBER"
#define romstr451 "EBENE"
#define romstr452 "EBENE"
#define romstr453 "EBENE"
#define romstr454 "EBENE"
#define romstr455 "EBENE"
#define romstr456 "EBENE"
#define romstr457 "EBENE"
#define romstr458 "EBENE"
#define romstr459 "EBENE"
#define romstr460 "SUMPF"
#define romstr461 "RUINE"
#define romstr462 "TROCKENE WÜSTE"
#define romstr463 "WÜSTE"
#define romstr464 "HALBWÜSTE"
#define romstr465 "BRÜCKE"
#define romstr466 "LICHTER WALD"
#define romstr467 "WALD"
#define romstr468 "HÜGEL"
#define romstr469 "WADI"
#define romstr470 "SLUMS"
#define romstr471 "CITY 1 (ENTRANCE)"
#define romstr472 "CITY 2 (ENTRANCE)"
#define romstr473 "CITY 3 (ENTRANCE)"
#define romstr474 "CITY 4 (ENTRANCE)"
#define romstr475 "EBENE"
#define romstr476 "EBENE"
#define romstr477 "SCHILD"
#define romstr478 "HINDERNIS"
#define romstr479 "VULKAN"
#define romstr480 "CITY 1 (PORT)"
#define romstr481 "CITY 2 (PORT)"
#define romstr482 "WASSER"
#define romstr483 "OFFENES MEER"
#define romstr484 "includes.c"									/* ! */
#define romstr485 "KÄMPFER"
#define romstr486 "MAGIER"
#define romstr487 "MOGUL"
#define romstr488 "ZAUBERMEISTER"
#define romstr489 "DIES IST DIE LETZTE TÜR, WELCHE DICH NOCH VON MEDUSA TRENNT. MÖCHTEST DU SIE WIRKLICH ÖFFNEN?"
#define romstr490 "ANZAHL: "										/* ! */
#define romstr491 "AUTO MAPPING"
#define romstr492 "AUS"
#define romstr493 "AB SOFORT BIST DU #0!"						/* [status e.g. wizard] */
#define romstr494 "DU HAST EINEN SCHATZ IM WERT VON #0 GEFUNDEN!"		/* [value] */
#define romstr495 "DAS KANN ICH NICHT ESSEN!"
#define romstr496 "HUNGER    :    %"
#define romstr497 "DURST     :    %"
#define romstr498 "GESUNDHEIT:    %"
#define romstr499 "KLEIDUNG  : #0%"							/* Amount */
#define romstr500 "STATUS:"
#define romstr501 "SÖLDNER           "
#define romstr502 "DIES IST EIN LECKERES ESSEN!"
#define romstr503 "DIES IST ETWAS ZU TRINKEN."
#define romstr504 "DIES IST MEDIZIN."
#define romstr505 "DIES IST EIN SCHLÜSSEL ZU MEDUSA'S GEHEIMBUNKER."
#define romstr506 "DIES IST EIN SCHLÜSSEL."
#define romstr507 "DIES IST EINE WAFFE."
#define romstr508 "DIES IST MUNITION. ES SIND #0 SCHÜSSE ÜBRIG."
#define romstr509 "DIES IST EIN HELM."
#define romstr510 "DIES IST EIN HEMD."
#define romstr511 "DIES IST EINE HOSE."
#define romstr512 "DIES SIND SCHUHE."
#define romstr513 "ES GIBT WÄNDE, DIE WIE WÄNDE AUSSEHEN, ABER..."
#define romstr514 "WAS SOLL DAS SCHON SEIN?"
#define romstr515 "  0%"
#define romstr516 "#0%"
#define romstr517 "UNBEKANNTES FIX-OBJEKT."					
#define romstr518 "KEIN STARTFELD IM BUNKER"
#define romstr519 "ITEMSARRAY ZU GROSS. NEEDED: "
#define romstr520 "ICH BRAUCHE MEHR ENEMYS:"
#define romstr521 "ICH BRAUCHE NUR ENEMIES:"
#define romstr522 "ITEMARRY ZU KLEIN! NEEDED: #0"
#define romstr523 "SHRINKBUFFER SIZE: "
#define romstr524 ""
#define romstr525 "includes.c"									/* ! */
#define romstr526 "DU VERHUNGERT BIST."
#define romstr527 "DU VERDURSTET BIST."
#define romstr528 "DU ZU SCHWACH BIST."
#define romstr529 "KEINE RICHTUNG FÜR DEN FEIND!"
#define romstr530 "DU HAST SCHON VIER LEUTE."
#define romstr531 "WILLST DU GEFANGENEN '#0' BEFREIEN?"				/* [name] */
#define romstr532 "DIE SCHUSSSTÄRKE IST DIE SUMME DER EINGESCHALTETEN EINZELSCHÜSSE."
#define romstr533 "ZUM EINSCHALTEN IN DAS SCHWARZE FELD NEBEN DEM NAMEN DER PERSON KLICKEN."
#define romstr534 "GUT, DASS DU EINEN SPIEGEL HAST...."
#define romstr535 "ICH GLAUBE DU HAST VERGESSEN ETWAS WICHTIGES MITZUNEHMEN...."
#define romstr536 "MEINE GLIEDER GEHORCHEN MIR NICHT MEHR...."
#define romstr537 "includes.c"									/* ! */
#define romstr538 "i"													/* ! */
#define romstr539 "DEINE STADT #0 WIRD VON #1 ANGEGRIFFEN."			/* [city][name] */
#define romstr540 "KÄMPFE FÜR DEIN REICH!"
#define romstr541 "DU BEKOMMST #0 ZU DEINEM GELD."					/* [money] */
#define romstr542 "DU HAST ALLES VERLOREN, AUSSER DEINEN GLEITERN!"
#define romstr543 "ILLEGAL ANI-SPRITE"
#define romstr544 "MEDUSA"
#define romstr545 "DEINE ARMEE:"
#define romstr546 "'S ARMEE:"
#define romstr547 "'S ARMEE:"
#define romstr548 "SUMME SOLDATEN:"
#define romstr549 "SUMME SOLDATEN:"
#define romstr550 ":"
#define romstr551 ":"
#define romstr552 "%5.2f"
#define romstr553 "%5.2f"
#define romstr554 "GEGEN"
#define romstr555 "WILLST DU DEINE AUFKLÄRER AUSSENDEN, UM ZU SCHAUEN, WIEVIEL SOLDATEN DER GEGNER HAT? KOSTEN: 2000."
#define romstr556 "ICH BIN DER MÄCHTIGE PRINZ! GEH HEIM~"
#define romstr557 "ZU DEINER FRAU UND HILF IHR KOCNEN!~~"
#define romstr558 "ICH ZAHLE       FÜR DEN FRIEDEN."
#define romstr559 "OKAY, ICH NEHME DAS GELD, STATT ZU KÄMPFEN...."
#define romstr560 "SO WENIG GELD? DU VERLIERST SOWIESO DEN KAMPF UND ICH BEKOMME DEIN GANZES GELD."
#define romstr561 "ENTSCHULDIGUNG, DASS WIR SOVIEL SCHWIERIGKEITEN GEMACHT HABEN. UUUUUND TSCHÜSS!!"
#define romstr562 "HAR, HAR, HAR.... DU, DER MÄCHTIGE PRINZ? SO'N QUATSCH!"
#define romstr563 "DU HAST VERLOREN!"
#define romstr564 "DU HAST #0 EROBERT."						/* [city] */
#define romstr565 "DU HAST DEINE ARMEE VERTEIDIGT!"
#define romstr566 "nr"
#define romstr567 "UNBEKANNTER KAMPFUNTERGRUND"
#define romstr568 "%4.1f"
#define romstr569 "%4.1f"
#define romstr570 "includes.c"									/* ! */
#define romstr571 "seafight.h"									/* ! */
#define romstr572 "WINKEL:"	
#define romstr573 "%4.1f["
#define romstr574 "GegnerSchuss(V0=%d, Winkel=%d, Tan=%d.%03d, Const=%d.%03d\n"		/* ! */
#define romstr575 "Schuss(V0=%d, Winkel=%d, Tan=%d.%03d, Const=%d.%03d\n"			/* ! */
#define romstr576 "SCHUSSENTFERNUNG:"
#define romstr577 "TREFFER!  "
#define romstr578 "ENTFERNUNG"
#define romstr579 "TREFFER AM GEGNER: #0"					/* [number of hits] */
#define romstr580 "TREFFER AM SPIELER: #0"					/* [number of hits] */
#define romstr581 "KÄMPFE GEGEN #0"						/* [name] */
#define romstr582 "WEIL DU ERTRUNKEN BIST."
#define romstr583 ""
#define romstr584 "includes.c"											/* ! */
#define romstr585 "DEINE FLOTTE KANN NICHT AN LAND GEHEN."
#define romstr586 "GRENZE AN DIESER STELLE NICHT PASSIERBAR..."
#define romstr587 "GROUND-NR ZU GROSS!"
#define romstr588 "DIE MINE BETRETEN?"
#define romstr589 "DU, SOHN DES BÖSEN KÖNIGS HAST KEINEN ZUTRITT ZU MEDUSAS'STADT."
#define romstr590 "DU HAST ZU OFT UNSERE KARAWANE ODER STADT ANGEGRIFFEN. WIR LASSEN DICH NICHT REIN!"
#define romstr591 "#0 BETRETEN?"											/* [city] */
#define romstr592 "HALLO SIR! WILLST DU NICHT IN UNSERE SCHÖNE STADT #0 KOMMEN?"	/* [city] */
#define romstr593 "SIR, WIR GREIFEN #0 AN:"									/* [city] */
#define romstr594 "MEDUSA'S GEHEIMBUNKER BETRETEN?"
#define romstr595 "BUNKER BETRETEN?"
#define romstr596 "OUT OF GROUND (PUT)"
#define romstr597 "OUT OF GROUND (GET)"
#define romstr598 "DIE REEDEREI VON #0 HAT EINEN #1 FÜR DEINE FLOTTE GEBAUT."	/* [city][ship] */
#define romstr599 "KEIN SCHIFF FREI!"
#define romstr600 "X,Y POSITION?"
#define romstr601 "KEIN SPRITE FREI"
#define romstr602 "HAFEN NICHT GEFUNDEN!"
#define romstr603 "LANDEINGANG NICHT GEFUNDEN!"
#define romstr604 "LAND:"
#define romstr605 "X,Y:"
#define romstr606 "UNTERGRUND:"
#define romstr607 "AKT WEG:"
#define romstr608 "KEIN NEUER WEG DA"
#define romstr609 "includes.c"									/* ! */
#define romstr610 "    GELD:~SOLDATEN:~WEHRSOLD:"
#define romstr611 "SICHER?"
#define romstr612 "SICHER?"
#define romstr613 "SICHER?"
#define romstr614 "bildzeile"										/* ! */
#define romstr615 "orgzeile"										/* ! */
#define romstr616 "option_nr"										/* ! */
#define romstr617 "VARIABLE LEISTEN ZU GROSS!"
#define romstr618 "LEISTEN #0"									/* ! */
#define romstr619 "includes.c"									/* ! */
#define romstr620 "BITTE LEGE DISK              'X' IN LAUFWERK A:"		
#define romstr621 "BITTE LEGE DEINE SPIELSTANDDISKETTE IN LAUFWERK A:"
#define romstr622 "ICH KANN NUR DISKS MIT 9/10 SEKTOREN PRO TRACK LESEN. FORMATIEREN MIT DESKTOP."
#define romstr623 "DISKFEHLER... DISK AUSTAUSCHEN ODER ERNEUT VERSUCHEN."
#define romstr624 "LADE, BITTE WARTEN..."
#define romstr625 "ICH GREIFE MEDUSA'S ARMEE AN."

