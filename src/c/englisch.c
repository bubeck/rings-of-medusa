/* Englische Version: */

#define ENGLISCH

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
		* Please do NOT kill any kind of comments (marked by /* xxx */).
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


                        TRANSLATOR'S NOTES

Hi! A couple of questions have cropped up while translating.

- Strings 006, 135: Keeping ":" in its place throughout the strings was a 
                    heck of a job. Due to lack of space in the two strings 
                    mentioned here I was forced to translate "Geld" into 
                    "gold", hoping that gold is the currency you use in 
                    ROM.
                    corrected T.B.
                    
- String 70: "Belegung" was understood as the number of inhabitants actually 
             present.    
             Okay, T.B.

- String 74: "Produziert" can either be present tense or past tense. I 
             understood it as "has been produced", ie. past tense.
             corrected T.B.
             
- Strings 170, 171: "Zahl an Soldaten". Number of soldiers or an order ("Pay 
                    them...")? Translated as number of soldiers.
             Okay, T.B.
             
- String 606: "Untergrund" was understood as "Bodenbeschaffenheit", ie 
              terrain.
             Okay, T.B.
             
- String 607: What on earth does "AKT WEG" mean?? Translated as path, since a 
              path is meant in the next string.
              Okay, T.B.
              
That's it. Good luck with the game!

 	
    */

#define romstr001 "includes.c"							/* ! */
#define romstr002 ".   "									/* ! */
#define romstr003 "PLEASE PRESS ANY KEY."					/* ! */
#define romstr004 "includes.c"							/* ! */
#define romstr005 "UNKNOWN MODUL"							/* ! */
#define romstr006 "YOUR MONEY:"
#define romstr007 "YOU DO NOT HAVE ENOUGH MONEY."
#define romstr008 "YOU HAVE HAD DEBTS FOR A LONG TIME NOW. YOU WILL NOT GET ANY MORE GOODS BEFORE HAVING PAID BACK YOUR DEBTS."
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
#define romstr027 "FREE MEMORY    : #0"						/* [number] */
#define romstr028 "V #0.#1#2"								/* ! */
#define romstr029 "STEFFI"								/* ! */
#define romstr030 "FOR STEFFI"						
#define romstr031 "  I LOVE YOU!"
#define romstr032 "               "
#define romstr033 "   STEFFI,   "
#define romstr034 "COPYRIGHT 1992 BY STARBYTE"	
#define romstr035 "WRITTEN BY: TILL BUBECK"
#define romstr036 "GRAPHICS: TORSTEN ZIMMERMANN"
#define romstr037 "MUSIC BY: JOCHEN HIPPEL"
#define romstr038 "CREATING THE GAME WORLD."
#define romstr039 "VERSION (USA+GB) "
#define romstr040 " C+"										/* ! */
#define romstr041 " C-"										/* ! */
#define romstr042 "RELEASE DATE: "
#define romstr043 "RELEASE TIME: "
#define romstr044 "WELCOME TO:"							/* 20 */
#define romstr045 "THE RETURN OF MEDUSA."					/* 20 */
#define romstr046 "PLEASE ENTER NAME:"						/* 20 */
#define romstr047 "CIRION"								/* ! */
#define romstr048 "LOADING: PLEASE WAIT..."					/* No : */
#define romstr049 "BILDBUFFERS ZU KLEIN:"					/* ! */
#define romstr050 ""											/* ! */
#define romstr051 "LOAD SAVED GAME"
#define romstr052 "NOT USED"
#define romstr053 "LOADING:"
#define romstr054 "RoM"										/* ! */
#define romstr055 "THIS IS NO SAVE GAME DISK. DO YOU WANT TO TURN IT INTO ONE?"
#define romstr056 "RoM"										/* ! */
#define romstr057 "NOT USED"
#define romstr058 "RoM"										/* ! */
#define romstr059 "THIS GAME WAS SAVED WITH AN OLDER VERSION OF ROM 2 -> UNABLE TO LOAD."
#define romstr060 "ERROR WHILE SAVING! (DISK COPY PROTECTED OR DAMAGED?)"
#define romstr061 "YOU CAN ONLY SAVE EVERY 5 MINUTES."
#define romstr062 "SAVE GAME"
#define romstr063 "PLEASE ENTER NAME:"					/* May be longer */
#define romstr064 "includes.c"							/* ! */
#define romstr065 " "											/* ! */
#define romstr066 "WARNING! YOU MUST PAY #0 NEXT MONTH. YOU ONLY HAVE: #1"		/* [costs][money] */
#define romstr067 "INFO"
#define romstr068 "INFO"
#define romstr069 "POPULATION  :"
#define romstr070 "  PRESENT"
#define romstr071 ":"
#define romstr072 "         CITY OF  :"
#define romstr073 "MEDUSA."
#define romstr074 "PRODUCING:"
#define romstr075 "STATUS SCREEN"
#define romstr076 "YOU HAVE NO SHIPS IN THIS TOWN, => YOU DO NOT NEED A CREW."
#define romstr077 "YOUR OFFER FOR"
#define romstr078 " WAGES / MONTH: ^_      `a"
#define romstr079 " PEOPLE DEMAND:"
#define romstr080 "       SHIP:"
#define romstr081 "SHIP'S CREW:"
#define romstr082 " MEN ABOARD:"
#define romstr083 "     NEEDED:"
#define romstr084 "TOTAL WAGES:"
#define romstr085 "IF YOU LEAVE NOW THE REMAINING CANDIDATES WILL GO, TOO. EXIT TAVERN?"
#define romstr086 "IF YOU CHANGE THE PRICE NOW THE REMAINING CANDIDATES WILL LEAVE. CHANGE PRICE?"
#define romstr087 "IF YOU CHANGE RACE NOW THE REMAINING CANDIDATES WILL LEAVE. CHANGE RACE?"
#define romstr088 "YOU CANNOT PURCHASE MORE SHIPS AS YOU CAN OWN NO MORE THAN #0 SHIPS."
#define romstr089 "YOU DO NOT HAVE ENOUGH MONEY TO BUY #0."				/* [ship] */
#define romstr090 "THIS HARBOUR CAN ONLY BUILD 5 SHIPS."
#define romstr091 "THE SHIPPING COMPANY"
#define romstr092 "TRADING SHIP"
#define romstr093 "WARSHIP"
#define romstr094 "TYPE       :"
#define romstr095 "PRICE      :"
#define romstr096 "CREW       :"
#define romstr097 "CANNONS    :"
#define romstr098 "OWNED BY   :"
#define romstr099 "FINISHED ON:"
#define romstr100 "MONEY LEFT :"
#define romstr101 "FREIGHT    :"
#define romstr102 "CAPACITY   :"
#define romstr103 "DEPARTMENT STORE"
#define romstr104 "DEPARTMENT STORE"
#define romstr105 "DEPARTMENT STORE"
#define romstr106 "PARK"
#define romstr107 "PARK"
#define romstr108 "UNIT CHOSEN   :"
#define romstr109 "WAGES :"
#define romstr110 "SOLDIERS      : 100.00% ="
#define romstr111 ":   0.00% ="
#define romstr112 ":   0%"
#define romstr113 "TOTAL WAGES    :"
#define romstr114 "WAGES  / MONTH :"
#define romstr115 "MEN LEFT       :"
#define romstr116 "MEN CHOSEN     : \\]^_      `abc"
#define romstr117 "%6.2f"										/* ! */
#define romstr118 "YOU CANNOT HIRE SOLDIERS HERE AS YOUR ARMY IS ELSEWHERE."
#define romstr119 "GARAGE"
#define romstr120 "GARAGE"
#define romstr121 "GARAGE"
#define romstr122 "JEWELLER"
#define romstr123 "JEWELLER"
#define romstr124 "CASINO"
#define romstr125 "CASINO"
#define romstr126 "IMPAIR"
#define romstr127 "PAIR"
#define romstr128 "ROUGE"
#define romstr129 "NOIR"
#define romstr130 "MANQUE"
#define romstr131 "PASSE"
#define romstr132 "(3X)"
#define romstr133 "YOU HAVE PLACED #0 ON FIELD '#1'. GAIN  : #0 * #2 = #3"		/* [pool=Einsatz][position][number][number] */
#define romstr134 "FAITES VOS JEUX..."
#define romstr135 "MONEY:"							/* up to 8 charakters */
#define romstr136 "RIEN NE VA PLUS!."
#define romstr137 "CLICK!"
#define romstr138 "HARBOUR"
#define romstr139 "HARBOUR"
#define romstr140 "HARBOUR"
#define romstr141 "LEAVE FLEET IN THIS TOWN"
#define romstr142 "AND GO ASHORE"
#define romstr143 "SET SAIL WITH FLEET AND"
#define romstr144 "TAKE ALL GOODS"
#define romstr145 "ABOARD."
#define romstr146 "SET SAIL WITH FLEET AND"
#define romstr147 "TAKE ALL GOODS, SOLDIERS"
#define romstr148 "AND GLIDERS ALONG."
#define romstr149 "ENTER HARBOUR TAVERN AND"
#define romstr150 "HIRE A CREW"
#define romstr151 "ORDER A VESSEL AT THE"
#define romstr152 "SHIPPING COMPANY."
#define romstr153 "TRADING SHIPS:"
#define romstr154 "WARSHIPS     :"
#define romstr155 "GLIDERS:"
#define romstr156 "YOUR ARMY IS NOT IN THIS TOWN."
#define romstr157 "YOU CANNOT LEAVE THIS TOWN BY WATER AS YOU DO NOT HAVE ANY SHIPS HERE."
#define romstr158 "YOU CANNOT LEAVE THIS TOWN BY WATER AS YOU DO NOT HAVE ANY SHIPS HERE."
#define romstr159 "YOU CANNOT GO ASHORE HERE AS YOUR ARMY IS NOT PRESENT."
#define romstr160 "YOU HAVE LANDED."
#define romstr161 "YOU HAVE ALREADY LANDED."
#define romstr162 "YOUR FLEET CANNOT TRANSPORT THIS AMOUNT OF GOODS AND RAW MATERIALS."
#define romstr163 "YOUR FLEET CANNOT TRANSPORT THIS NUMBER OF SOLDIERS."
#define romstr164 "HARBOUR NOT FOUND"
#define romstr165 "LAND OPENING NOT FOUND"
#define romstr166 "NO LANDING SPACE FREE."
#define romstr167 "SEARCH LAND() NO SPACE FOUND!"
#define romstr168 "WEAPON DEALER"
#define romstr169 "WEAPON DEALER"
#define romstr170 "NUMBER OF SOLDIERS WITH:~"
#define romstr171 "NO. OF SOLDIERS WITHOUT:~"
#define romstr172 "PRICE OF A SOLDIER     :"
#define romstr173 "UNIT   :~"
#define romstr174 "  TOTAL:"
#define romstr175 "CHOOSE: ^_           `a~"
#define romstr176 "COST  :"
#define romstr177 "STATUS"
#define romstr178 "THE ARMY OF SIR "
#define romstr179 "ARMY :"
#define romstr180 ":"
#define romstr181 "----------"
#define romstr182 "ACCOUNT:"
#define romstr183 "SHIP  :"
#define romstr184 ":"
#define romstr185 "--"
#define romstr186 "YOU OWN "
#define romstr187 " TOWNS."
#define romstr188 "YOU HAVE "
#define romstr189 " MINES."
#define romstr190 "YOU HAVE"
#define romstr191 " GOODS."
#define romstr192 "YOU HAVE LOST BECAUSE "
#define romstr193 "YOU MUST LEAVE YOUR SHIP BEFORE YOU CAN EXIT #0."			/* [city] */
#define romstr194 "YOU CANNOT LEAVE TOWN BECAUSE YOUR GLIDERS CANNOT CARRY THIS MUCH."
#define romstr195 "EXIT TOWN"
#define romstr196 "EXIT #0?"										/* [city] */
#define romstr197 "BARRACKS"
#define romstr198 "BARRACKS"
#define romstr199 "THE GUARDS DON'T LET YOU PASS!"
#define romstr200 "YOUR ARMY  :"
#define romstr201 "TIME DELAY     :"
#define romstr202 ":"
#define romstr203 "<-      ->"
#define romstr204 "----------"
#define romstr205 "--------"
#define romstr206 "\\]^_      `abc"
#define romstr207 "includes.c"								/* ! */
#define romstr208 "SUBWAY"
#define romstr209 "BANK"
#define romstr210 "BANK"
#define romstr211 "THE ROYAL FALCON BANK"
#define romstr212 "AMOUNT DEPOSITED:"
#define romstr213 "CREDIT AVAILABLE:"
#define romstr214 " MONEY CHOSEN   : \\]^_        `abc"
#define romstr215 "TODAY'S INTEREST: 4%"
#define romstr216 "CREDIT INTEREST : 18%"
#define romstr217 "INTEREST WILL BE CALCULATED THE NEXT TIME"
#define romstr218 "YOU ENTER THE BANK."
#define romstr219 "STOCK MARKET"
#define romstr220 "YOU GAIN #0 INTEREST!"					/* [amount of money] */
#define romstr221 "YOU MUST PAY #0 INTEREST."					/* [amount of money] */
#define romstr222 "YOU MUST PAY #0 INTEREST BUT DO NOT HAVE ENOUGH MONEY. GO AND EARN SOME."		/* [amount of money] */
#define romstr223 "ATTACKING THE BANK, YOU GAIN #0."				/* [amount of money] */
#define romstr224 "THE ROYAL FALCON BANK DOES NOT DO BUSINESS WITH BANK ROBBERS LIKE YOU."
#define romstr225 "STOCK MARKET"
#define romstr226 "CHART ANALYSIS COSTS 10000. STILL CARRY THROUGH?"
#define romstr227 "STORAGE"
#define romstr228 "PRICE"
#define romstr229 "  AMOUNT"
#define romstr230 "YOUR GOODS:    AMOUNT"
#define romstr231 "GOODS CHOSEN :"
#define romstr232 "AMOUNT     : \\]^_`abc"
#define romstr233 "PRICE      :"
#define romstr234 "TRADING SHIPS :"
#define romstr235 "GLIDERS:"
#define romstr236 "CHART ANALYSIS"
#define romstr237 "BROKERAGE    : 2%"
#define romstr238 "%f"
#define romstr239 "QUOTATION:"
#define romstr240 "version.c"									/* ! */
#define romstr241 "STRINGS.C"									/* ! */
#define romstr242 "FILENAME.C"								/* ! */
#define romstr243 "CONST.C"									/* ! */
#define romstr244 "GRUND.C"									/* ! */
#define romstr245 "TYPEDEF.C"									/* ! */
#define romstr246 "GLOBAL.C"									/* ! */
#define romstr247 "PROTOTYP.C"								/* ! */
#define romstr248 "MCODE.C"									/* ! */
#define romstr249 "YOU HAVE NO FREQUENCY QUARTZ."
#define romstr250 "THERE ARE NO SHELTERS IN THIS COUNTRY."
#define romstr251 "OKAY SIR, WE'LL SEARCH FOR RAW MATERIALS..."
#define romstr252 "I'M SORRY, WE HAVEN'T FOUND ANYTHING."
#define romstr253 "TREASURE AT #0!"								/* [position] */
#define romstr254 "SHELTER ENTRANCE FOUND AT COORDINATE #0!"		/* [position] */
#define romstr255 "#0 AT #1."									/* [treasure][position] */
#define romstr256 "   EXCAVATE MINE (COST  : 30000)? YOU OWN: #0"	/* [money] */
#define romstr257 "I'M SORRY, WE ALREADY OWN 20 MINES."
#define romstr258 "YOU NEED 30000 FOR A MINE BUT DON'T HAVE ENOUGH MONEY."
#define romstr259 "...SIR !!!! WE HAVE FOUND TREASURE!       VALUE  : #0"		/* [value] */
#define romstr260 "YOU HAVE FILLED #0 CARTS WITH DIRT. YOU HAVE LOST THEM."	/* [number] */
#define romstr261 "DIRT"
#define romstr262 "YOU DON'T HAVE A CONVEYING SYSTEM."
#define romstr263 "YOU ALREADY OWN 3 CONVEYING SYSTEMS."
#define romstr264 "YOU DON'T HAVE A PUMPING SYSTEM!"
#define romstr265 "YOU ALREADY HAVE 3 FILTER STAGES."
#define romstr266 "YOU DON'T HAVE AN OIL TANK!"
#define romstr267 "YOU ALREADY OWN 3 OIL TANKS."
#define romstr268 "ARE YOU SURE YOU WANT TO CLOSE THIS MINE?"
#define romstr269 "MINE"
#define romstr270 "LOAD AS MUCH AS POSSIBLE ONTO ~THE GLIDERS.~~"
#define romstr271 "INSTALL NEW CONVEYING SYSTEM~(COST  : 5000)~~"
#define romstr272 "INSTALL NEW PUMPING SYSTEM ~(COST  : 1000)~~"
#define romstr273 "INSTALL NEW OIL TANK ~(COST  : 4000)~~"
#define romstr274 "CLOSE MINE     ~(COST  : 5000)"
#define romstr275 "WAREHOUSE:~~"
#define romstr276 "THIS MINE WAS OPENED~~"
#define romstr277 "AND HAS YIELDED MORE THAN~~~~~~"
#define romstr278 "GLIDERS:"
#define romstr279 "DIRT"
#define romstr280 "DIRT."
#define romstr281 "RINGS OF MEDUSA 2 - CHEATS:"
#define romstr282 "R: SHOW RAW MATERIALS"
#define romstr283 "*: UNLIMITED MONEY"
#define romstr284 "W: SHOW GOODS"
#define romstr285 "S: SHOW STRENGTH"
#define romstr286 "O: SHOW ENEMIES"
#define romstr287 "T: SHOW TREASURE"
#define romstr288 "K: ENTER BARRACKS"
#define romstr289 "+: INCREASE SPEED"
#define romstr290 "0: MONEY TO NOUGHT"
#define romstr291 "B: SEA COMBAT"
#define romstr292 "F: LAND COMBAT"
#define romstr293 "C: ENTER UNION CITY"
#define romstr294 "X: OPEN ALL DOORS"
#define romstr295 "Z: SHOW COMPUTING TIME"
#define romstr296 "M: ONE MONTH LATER"
#define romstr297 "D: ENTER FORTIFIED SHELTER"
#define romstr298 "A: AUTO MAPPING"
#define romstr299 "L: LIGHT AT FULL INTENSITY"
#define romstr300 "U: IMMORTAL INSIDE SHELTER"
#define romstr301 "WHICH SHELTER DO YOU WISH"
#define romstr302 "TO ENTER?"
#define romstr303 "PLEASE ENTER NUMBER  :"
#define romstr304 "NO VOLCANO FOUND!"
#define romstr305 "INTERNAL ERROR: \x22#0\x22."						/* ! */
#define romstr306 "CALCULATING..."
#define romstr307 "SPECIAL PRESS VERSION"							/* ! */
#define romstr308 "NO. 3"											/* ! */
#define romstr309 "YOUR ARMY WANTS MONEY"
#define romstr310 "BUT YOU DON'T HAVE ENOUGH."
#define romstr311 "WARNING! YOU ONLY HAVE MONEY LEFT FOR A MONTH!"
#define romstr312 "NOTE PAD"
#define romstr313 "NOTE PAD"
#define romstr314 "0%"
#define romstr315 "100%"
#define romstr316 "OVERLOADED"
#define romstr317 "NONE HERE "
#define romstr318 "SAVE.C"										/* ! */
#define romstr319 "OUCH!"
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
#define romstr332 "STRENGTH"
#define romstr333 "FIGHTING MORALE"
#define romstr334 "ENDURANCE"
#define romstr335 "DEXTERITY"
#define romstr336 "INTELLIGENCE"
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
#define romstr352 "NOBODY"
#define romstr353 "INFANTRY    "
#define romstr354 "TANK        "
#define romstr355 "ARTILLERY   "
#define romstr356 "SCOUT       "
#define romstr357 "HELICOPTER  "
#define romstr358 "AMBULANCE   "
#define romstr359 "GROUND TROOP"
#define romstr360 "GUN"
#define romstr361 "GRENADES"
#define romstr362 "BAZOOKA"
#define romstr363 "HUMAN   "
#define romstr364 "ELF     "
#define romstr365 "DWARF   "
#define romstr366 "HALFLING"
#define romstr367 "ORC     "
#define romstr368 "GNOME   "
#define romstr369 "GIANT   "
#define romstr370 "TROLL   "
#define romstr371 "OGRE    "
#define romstr372 "ZWARK   "
#define romstr373 "NOT HERE"
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
#define romstr402 "CUTTER"
#define romstr403 "BARGE"
#define romstr404 "TANKER"
#define romstr405 "SPEEDBOAT"
#define romstr406 "DESTROYER"
#define romstr407 "CRUISER"
#define romstr408 "MILK"
#define romstr409 "CHEESE"
#define romstr410 "SALAD"
#define romstr411 "APPLES"
#define romstr412 "MEAT"
#define romstr413 "FISH"
#define romstr414 "WEAPONS"
#define romstr415 "CRACK"
#define romstr416 "ACID 54"
#define romstr417 "MEDICINE"
#define romstr418 "BATTERIES"
#define romstr419 "QUARTZ"
#define romstr420 "SLAVES"
#define romstr421 "HOIST.TOWR"
#define romstr422 "PUMP"
#define romstr423 "OIL TANK"
#define romstr424 "CYBORGS"
#define romstr425 "ANDROIDS"
#define romstr426 "COMPUTERS"
#define romstr427 "CHIPS"
#define romstr428 "KEROSENE"
#define romstr429 "GLIDERS"
#define romstr430 "TREASURE"
#define romstr431 "IRON"
#define romstr432 "DIAMONDS"
#define romstr433 "NITRIL"
#define romstr434 "OIL"
#define romstr435 "GAS"
#define romstr436 "GIMMICK"
#define romstr437 "GOLD"
#define romstr438 "NO MONTH"
#define romstr439 "JANUARY"
#define romstr440 "FEBRUARY"
#define romstr441 "MARCH"
#define romstr442 "APRIL"
#define romstr443 "MAY"
#define romstr444 "JUNE"
#define romstr445 "JULY"
#define romstr446 "AUGUST"
#define romstr447 "SEPTEMBER"
#define romstr448 "OCTOBER"
#define romstr449 "NOVEMBER"
#define romstr450 "DECEMBER"
#define romstr451 "PLAINS"
#define romstr452 "PLAINS"
#define romstr453 "PLAINS"
#define romstr454 "PLAINS"
#define romstr455 "PLAINS"
#define romstr456 "PLAINS"
#define romstr457 "PLAINS"
#define romstr458 "PLAINS"
#define romstr459 "PLAINS"
#define romstr460 "SWAMP"
#define romstr461 "RUINS"
#define romstr462 "ARID DESERT"
#define romstr463 "DESERT"
#define romstr464 "SEMI-DESERT"
#define romstr465 "BRIDGE"
#define romstr466 "SPARSE WOODS"
#define romstr467 "FOREST"
#define romstr468 "HILL"
#define romstr469 "WADI"
#define romstr470 "SLUMS"
#define romstr471 "CITY 1 (ENTRANCE)"
#define romstr472 "CITY 2 (ENTRANCE)"
#define romstr473 "CITY 3 (ENTRANCE)"
#define romstr474 "CITY 4 (ENTRANCE)"
#define romstr475 "PLAINS"
#define romstr476 "PLAINS"
#define romstr477 "SIGN"
#define romstr478 "OBSTACLE"
#define romstr479 "VOLCANO"
#define romstr480 "CITY 1 (PORT)"
#define romstr481 "CITY 2 (PORT)"
#define romstr482 "WATER"
#define romstr483 "OPEN SEA"
#define romstr484 "includes.c"									/* ! */
#define romstr485 "FIGHTER"
#define romstr486 "MAGE"
#define romstr487 "MOGUL"
#define romstr488 "WIZARD"
#define romstr489 "THIS IS THE LAST DOOR THAT SEPARATES YOU FROM MEDUSA. DO YOU REALLY WANT TO OPEN IT?"
#define romstr490 "ANZAHL: "										/* ! */
#define romstr491 "AUTO MAPPING"
#define romstr492 "OFF"
#define romstr493 "YOU ARE NOW A #0!"						/* [status e.g. wizard] */
#define romstr494 "YOU HAVE FOUND TREASURE WORTH #0!"		/* [value] */
#define romstr495 "I CAN'T EAT THAT!"
#define romstr496 "HUNGER    :    %"
#define romstr497 "THIRST    :    %"
#define romstr498 "HEALTH    :    %"
#define romstr499 "CLOTHES   : #0%"							/* Amount */
#define romstr500 "STATUS:"
#define romstr501 "MERCENARY         "
#define romstr502 "THIS IS A DELICIOUS MEAL!"
#define romstr503 "THIS IS SOMETHING TO DRINK."
#define romstr504 "THIS IS A MEDICINE."
#define romstr505 "THIS IS A KEY TO MEDUSA'S SECRET SHELTER."
#define romstr506 "THIS IS A KEY."
#define romstr507 "THIS IS A WEAPON."
#define romstr508 "THIS IS AMMUNITION. THERE ARE #0 SHOTS LEFT."
#define romstr509 "THIS IS A HELMET."
#define romstr510 "THIS IS A SHIRT."
#define romstr511 "THESE ARE TROUSERS."
#define romstr512 "THESE ARE SHOES."
#define romstr513 "THERE ARE WALLS THAT LOOK LIKE WALLS BUT..."
#define romstr514 "SO WHAT IS THIS SUPPOSED TO BE?"
#define romstr515 "  0%"
#define romstr516 "#0%"
#define romstr517 "UNIDENTIFIED FIXING OBJECT."					
#define romstr518 "NO STARTING SPACE IN SHELTER"
#define romstr519 "DECREASE ITEM ARRAY TO     : "
#define romstr520 "I NEED MORE ENEMIES    :"
#define romstr521 "I ONLY NEED ENEMIES    :"
#define romstr522 "INCREASE ITEM ARRAY TO   : #0"
#define romstr523 "SHRINKBUFFER SIZE: "
#define romstr524 ""
#define romstr525 "includes.c"									/* ! */
#define romstr526 "YOU HAVE STARVED."
#define romstr527 "YOU HAVE DIED OF THIRST."
#define romstr528 "YOU ARE TOO WEAK."
#define romstr529 "NO DIRECTION FOR THE ENEMY!"
#define romstr530 "YOU ALREADY HAVE FOUR PEOPLE."
#define romstr531 "RELEASE PRISONER '#0'?"				/* [name] */
#define romstr532 "SHOOTING POWER EQUALS THE SUM OF SINGLE SHOTS SWITCHED ON."
#define romstr533 "TO SWITCH ON, CLICK ON THE BLACK FIELD NEXT TO THE CHARACTER'S NAME."
#define romstr534 "A GOOD THING THAT YOU HAVE A MIRROR...."
#define romstr535 "I BELIEVE YOU FORGOT TO TAKE ALONG SOMETHING IMPORTANT..."
#define romstr536 "MY LIMBS NO LONGER OBEY ME...."
#define romstr537 "includes.c"									/* ! */
#define romstr538 "i"													/* ! */
#define romstr539 "YOUR TOWN #0 IS BEING ATTACKED BY #1."			/* [city][name] */
#define romstr540 "FIGHT FOR YOUR EMPIRE!"
#define romstr541 "#0 IS ADDED TO YOUR MONEY."					/* [money] */
#define romstr542 "YOU HAVE LOST EVERYTHING SAVE YOUR GLIDERS!"
#define romstr543 "ILLEGAL ANTI-SPRITE"
#define romstr544 "MEDUSA"
#define romstr545 "YOUR ARMY  :"
#define romstr546 "'S ARMY :"
#define romstr547 "'S ARMY :"
#define romstr548 "TOTAL SOLDIERS:"
#define romstr549 "TOTAL SOLDIERS:"
#define romstr550 ":"
#define romstr551 ":"
#define romstr552 "%5.2f"
#define romstr553 "%5.2f"
#define romstr554 "VERSUS"
#define romstr555 "DO YOU WISH TO SEND YOUR SCOUTS TO FIND OUT THE SIZE OF THE OPPONENT'S ARMY?  THIS WILL COST: 2000."
#define romstr556 "I AM THE MIGHTY PRINCE! GO HOME~"
#define romstr557 "TO YOUR WIFE AND HELP HER COOK!~~"
#define romstr558 "I'LL PAY        FOR PEACE."
#define romstr559 "RIGHT, I'LL TAKE THE MONEY INSTEAD OF FIGHTING...."
#define romstr560 "THAT LITTLE MONEY? YOU'LL LOSE THE BATTLE ANYHOW AND I SHALL GET ALL YOUR GOLD."
#define romstr561 "SORRY TO HAVE CAUSED YOU SO MUCH TROUBLE. BYEEE NOW!!"
#define romstr562 "HAR, HAR, HAR.... YOU, THE MIGHTY PRINCE? WHAT RUBBISH!"
#define romstr563 "YOU HAVE LOST!"
#define romstr564 "YOU CONQUERED #0."						/* [city] */
#define romstr565 "YOU DEFENDED YOUR ARMY!"
#define romstr566 "nr"
#define romstr567 "UNKNOWN FIGHTING TERRAIN"
#define romstr568 "%4.1f"
#define romstr569 "%4.1f"
#define romstr570 "includes.c"									/* ! */
#define romstr571 "seafight.h"									/* ! */
#define romstr572 "ANGLE :"	
#define romstr573 "%4.1f["
#define romstr574 "GegnerSchuss(V0=%d, Winkel=%d, Tan=%d.%03d, Const=%d.%03d\n"		/* ! */
#define romstr575 "Schuss(V0=%d, Winkel=%d, Tan=%d.%03d, Const=%d.%03d\n"			/* ! */
#define romstr576 "DISTANCE OF SHOT:"
#define romstr577 "HIT!  "
#define romstr578 "DISTANCE"
#define romstr579 "HITS LANDED      : #0"					/* [number of hits] */
#define romstr580 "HITS TAKEN        : #0"					/* [number of hits] */
#define romstr581 "FIGHT AGAINST #0"						/* [name] */
#define romstr582 "BECAUSE YOU HAVE DROWNED."
#define romstr583 ""
#define romstr584 "includes.c"											/* ! */
#define romstr585 "YOUR FLEET CANNOT GO ASHORE."
#define romstr586 "BORDER IMPASSABLE HERE..."
#define romstr587 "GROUND NO. TOO LARGE!"
#define romstr588 "ENTER MINE?"
#define romstr589 "YOU, SON OF THE EVIL KING, ARE DENIED ENTRY TO MEDUSA'S CITY."
#define romstr590 "YOU HAVE ATTACKED OUR CARAVAN OR TOWN TOO OFTEN. WE SHALL NOT LET YOU ENTER!"
#define romstr591 "ENTER #0?"											/* [city] */
#define romstr592 "HELLO, SIR! DON'T YOU WANT TO VISIT OUR BEAUTIFUL CITY #0?"	/* [city] */
#define romstr593 "SIR, NOW ATTACKING #0  :"									/* [city] */
#define romstr594 "ENTER MEDUSA'S SECRET SHELTER?"
#define romstr595 "ENTER SHELTER?"
#define romstr596 "OUT OF GROUND (PUT)"
#define romstr597 "OUT OF GROUND (GET)"
#define romstr598 "THE SHIPPING COMPANY HAS BUILT A #1 FOR YOUR FLEET."	/* [city][ship] */
#define romstr599 "NO SHIP FREE!"
#define romstr600 "X,Y POSITION?"
#define romstr601 "NO SPRITE AVAILABLE"
#define romstr602 "HARBOUR NOT FOUND!"
#define romstr603 "LAND OPENING NOT FOUND!"
#define romstr604 "LAND:"
#define romstr605 "X,Y:"
#define romstr606 "TERRAIN   :"
#define romstr607 "PATH   :"
#define romstr608 "NO NEW PATH HERE"
#define romstr609 "includes.c"									/* ! */
#define romstr610 "   MONEY:~SOLDIERS:~WAGES   :"
#define romstr611 "SURE?"
#define romstr612 "SURE?"
#define romstr613 "SURE?"
#define romstr614 "bildzeile"										/* ! */
#define romstr615 "orgzeile"										/* ! */
#define romstr616 "option_nr"										/* ! */
#define romstr617 "VARIABLE LAST TOO LARGE!"
#define romstr618 "LEISTEN #0"									/* ! */
#define romstr619 "includes.c"									/* ! */
#define romstr620 "PLEASE INSERT DISK           'X' IN DRIVE    A:"		
#define romstr621 "PLEASE INSERT YOUR SAVE GAME DISK IN DRIVE      A:"
#define romstr622 "I CAN ONLY USE DISKS WITH 9/10 SEKTORS PER TRACK. FORMAT WITH DESKTOP."
#define romstr623 "DISK ERROR! TRY AGAIN OR CHANGE DISK."

