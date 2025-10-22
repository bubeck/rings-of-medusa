/* Französische Version: */

#define FRANZ

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
					 tranlated into german: "ENTRER DANS NEW YORK CITY?"
					 
					 The position of "NEW YORK CITY" is different. In the english version
					 "NYC" is in the middle of the string between "ENTER" and "?".
					 In the german translation "NYC" is at the beginning
					 of the string followed by "ENTRER?".
					 
			Solution:
					 For the last example the string looks:
					  	english: "ENTER #0?"
					   	german: "#0 ENTRENT?"
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
#define romstr006 "TON ARGENT:"
#define romstr007 "TU N A PAS ASSEZ D ARGENT."
#define romstr008 "CA FAIT LONGTEMPS QUE TU AS DES DETTES. TU N OBTIENDRAS PAS D AUTRES MARCHANDISES,AVANT DE LES AVOIR REMBOURSEES."
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
#define romstr020 "VENT VARIABLE_VENT TROP FORT!"				/* ! */
#define romstr021 "FORCE DU VENT #0"							/* ! */
#define romstr022 "X:"										/* ! */
#define romstr023 " Y:"										/* ! */
#define romstr024 "CURSOR X:"								/* ! */
#define romstr025 " CURSOR Y:"							/* ! */
#define romstr026 "C:\\MEDUSA.PI"							/* ! */
#define romstr027 "MEMOIRES LIBRES: #0"						/* [number] */
#define romstr028 "V #0.#1#2"								/* ! */
#define romstr029 "STEFFI"								/* ! */
#define romstr030 "POUR STEFFI"						
#define romstr031 "JE T'AIME!"
#define romstr032 "          "
#define romstr033 "   STEFFI,   "
#define romstr034 "COPYRIGHT 1992 BY STARBYTE"	
#define romstr035 "ECRIT PAR: TILL BUBECK"
#define romstr036 "GRAPHIQUES DE: TORSTEN ZIMMERMANN"
#define romstr037 "MUSIQUE DE: JOCHEN HIPPEL"
#define romstr038 "CONCEPTION DES DIF.MONDES."
#define romstr039 "VERSION (FRANCE) "
#define romstr040 " C+"										/* ! */
#define romstr041 " C-"										/* ! */
#define romstr042 "RELEASE DATE: "
#define romstr043 "RELEASE TIME: "
#define romstr044 "BIENVENUE DANS:"							/* 20 */
#define romstr045 "LE RETOUR DE MEDUSA."					/* 20 */
#define romstr046 "TON NOM S.T.P:"					/* Remember : */
#define romstr047 "CIRION"								/* ! */
#define romstr048 "LOADING: PLEASE WAIT..."					
#define romstr049 "BILDBUFFERS ZU KLEIN:"					/* ! */
#define romstr050 ""											/* ! */
#define romstr051 "MEMORISER L ETAT ACTUEL DU JEU"
#define romstr052 "INUTILISE"
#define romstr053 "CHARGE MAINTENANT:"
#define romstr054 "RoM"										/* ! */
#define romstr055 "L ETAT ACTUEL DU JEU N EST PAS MEMORISE SUR CETTE DISQUETTE. DESIRES-tu q elle le soit?"
#define romstr056 "RoM"										/* ! */
#define romstr057 "inutiliseT"
#define romstr058 "RoM"										/* ! */
#define romstr059 "L ETAT ACTUEL DE CE JEU EST MEMORISE AVEC UNE ANCIENNE VERSION DE ROM 2 -> CHARGEMENT IMPOSSIBLE."
#define romstr060 "ERREUR DANS LA MEMORISATION! (DISQUETTE PROTEGEE OU DEFECTUEUSE?)"
#define romstr061 "TU PEUX MEMORISER SEULEMENT TOUTES LES 5 MINUTES."
#define romstr062 "MEMORISER L ETAT ACTUEL DU JEU"
#define romstr063 "TON NOM S.T.P:"					/* May be longer */
#define romstr064 "includes.c"							/* ! */
#define romstr065 " "											/* ! */
#define romstr066 "ATTENTION! LE MOIS PROCHAIN TU AS #0 A PAYER. TU AS SEULEMENT: #1"		/* [costs][money] */
#define romstr067 "INFO"
#define romstr068 "INFO"
#define romstr069 "POPULATION  :"
#define romstr070 "  OCCUPATION"
#define romstr071 ":"
#define romstr072 "         LA VILLE DE:"
#define romstr073 "MEDUSA."
#define romstr074 "PRODUIT:"
#define romstr075 "INDICATION DU STATUT"
#define romstr076 "TU N AS PAS DE BATEAUX DANS CETTE VILLE, => TU N AS PAS BESOIN D EQUIPAGE."
#define romstr077 "TA PROPOSITION POUR"
#define romstr078 " ENGAGER / MOIS: ^_      `a"
#define romstr079 "  LES GENS VEULENT:"
#define romstr080 "     BATEAU:"
#define romstr081 "EQUIPAGE DU BATEAU:"
#define romstr082 "HOMME A BORD:"
#define romstr083 "  UTILISE:"
#define romstr084 "SOMME D ENGAGEMENT:"
#define romstr085 "SI TU T EN VAS MAITENANT,LE RESTE DES INTERESSES PARTIRAS. SORTIR DU BISTROT?"
#define romstr086 "SI TU CHANGES LES PRIX MAINTENANT,LE RESTE DES INTERESSES PARTIRAS. CHANGER LE PRIX?"
#define romstr087 "SI TU MODIFIES LA RACE MAINTENANT, LE RESTE DES INTERESSES PARTIRA. MODIFIER LA RACE?"
#define romstr088 "TU NE PEUX PLUS OBTENIR DE BATEAUX, CAR TU PEUX EN POSSEDER, AU PLUS #0."
#define romstr089 "TU N AS PAS ASSEZ D ARGENT POUR ACHETER #0:"				/* [ship] */
#define romstr090 "CE PORT PEUT CONSTRUIR SEULEMENT 5 BATEAUX."
#define romstr091 "SOCIETE D ARMARTEURS"
#define romstr092 "BATEAU DE COMMERCE"
#define romstr093 "BATEAU DE GUERRE"
#define romstr094 "GENRE        :"
#define romstr095 "PRIX      :"
#define romstr096 "EQUIPAGE       :"
#define romstr097 "CANONS  :"
#define romstr098 "EN POSSESSION  :"
#define romstr099 "  TERMINE LE:"
#define romstr100 "RESTE D ARGENT :"
#define romstr101 "FRET     :"
#define romstr102 "CAPACITE  :"
#define romstr103 "GRAND MAGASIN"
#define romstr104 "GRAND MAGASIN"
#define romstr105 "GRAND MAGASIN"
#define romstr106 "PARC"
#define romstr107 "PARC"
#define romstr108 "UNITE CHOISIE:"
#define romstr109 "SALAIRE:"
#define romstr110 "SOLDATS      : 100.00% ="
#define romstr111 ":   0.00% ="
#define romstr112 ":   0%"
#define romstr113 "SOMME SALAIRE   :"
#define romstr114 "SALAIRE / MOIS :"
#define romstr115 "RESTE D HOMMES   :"
#define romstr116 "HOMMES CHOISIS: \\]^_      `abc"
#define romstr117 "%6.2f"										/* ! */
#define romstr118 "ICI,TU NE PEUX PAS ACHETER DE SOLDATS, CAR TON ARMEE  N EST PAS LA "
#define romstr119 "GARAGE"
#define romstr120 "GARAGE"
#define romstr121 "GARAGE"
#define romstr122 "BIJOUTIER"
#define romstr123 "BIJOUTIER"
#define romstr124 "CASINO"
#define romstr125 "CASINO"
#define romstr126 "IMPAIR"
#define romstr127 "PAIR"
#define romstr128 "ROUGE"
#define romstr129 "NOIR"
#define romstr130 "MANQUE"
#define romstr131 "PASSE"
#define romstr132 "(3X)"
#define romstr133 "TU AS MISE #0 SUR LA CASE '#1'. GAIN: #0 * #2 = #3"		/* [pool=Einsatz][position][number][number] */
#define romstr134 "FAITES VOS JEUX..."
#define romstr135 "ARGENT:"							/* up to 8 charakters */
#define romstr136 "RIEN NE VA PLUS!."
#define romstr137 "CLIQUE!"
#define romstr138 "PORT"
#define romstr139 "PORT"
#define romstr140 "PORT"
#define romstr141 "LAISSER LA FLOTTE DANS CETTE VILLE"
#define romstr142 "ET ALLER SUR LE CONTINENT"
#define romstr143 "LEVER L ANCRE ET PRENDRE"
#define romstr144 "TOUTES LES MARCHANDISES"
#define romstr145 "A BORD."
#define romstr146 "LA FLOTTE PREND LA MER,"
#define romstr147 "AVEC TOUTES LES MARCHANDISES,"
#define romstr148 "ET TOUS LES SOLDATS."
#define romstr149 "FAIRE TOUS LES BISTROTS DU PORT"
#define romstr150 "ET ENGAGER UN EQUIPAGE"
#define romstr151 "COMMANDER UN BATEAU CHEZ "
#define romstr152 "UN ARMATEUR."
#define romstr153 "BATEAUX DE COMMERCE:"
#define romstr154 "BATEAUX DE GUERRE:"
#define romstr155 "HYDRO-GLISSEUR:"
#define romstr156 "TON ARMEE N EST PAS DANS CETTE VILLE."
#define romstr157 "TU NE PEUX PAS QUITTER CETTE VILLE PAR LES VOIES FLUVIALES, CAR ICI TU N AS PAS DE BATEAUX ."
#define romstr158 "TU NE PEUX PAS QUITTER CETTE VILLE PAR LES VOIES FLUVIALES, CAR ICI TU N AS PAS DE BATEAUX."
#define romstr159 "TU NE PEUX PAS ALLER SUR LE CONTINENT, CAR TON ARMEE N EST PAS LA."
#define romstr160 "TU AS MIS PIED A TERRE."
#define romstr161 "TU AS DEJA MIS PIED A TERRE."
#define romstr162 "TA FLOTTE NE PEUT PAS TRANSPORTER AUTANT DE MARCHANDISES ET DE MATIERES PREMIERES."
#define romstr163 "TA FLOTTE NE PEUT PAS TANSPORTER AUTANT DE SOLDATS."
#define romstr164 "PORT NON TROUVE"
#define romstr165 "ENTREE DU PAYS NON TROUVEE"
#define romstr166 "PAS DE PLACE DANS LE PORT."
#define romstr167 "CHERCHE PAYS() N A PAS TROUVE DE PLACE!"
#define romstr168 "ARMURIER"
#define romstr169 "ARMURIER"
#define romstr170 "   NOMBRE DE SOLDATS AVEC:~"
#define romstr171 "  NOMBRE DE SOLDATS SANS:~"
#define romstr172 " PRIX POUR UN SOLDAT:"
#define romstr173 "UNITE:~"
#define romstr174 "  SOMME:"
#define romstr175 " CHOISI: ^_           `a~"
#define romstr176 "COUTENT:"
#define romstr177 "STATUT"
#define romstr178 "L ARMEE DE SIR "
#define romstr179 "ARMEE:"
#define romstr180 ":"
#define romstr181 "----------"
#define romstr182 "  COMPTE:"
#define romstr183 "BATEAU:"
#define romstr184 ":"
#define romstr185 "--"
#define romstr186 "TU POSSEDES "
#define romstr187 " DES VILLES."
#define romstr188 "TU AS DES "
#define romstr189 " MINES."
#define romstr190 "TU AS DES"
#define romstr191 " MARCHANDISES."
#define romstr192 "TU AS PERDU CAR,"
#define romstr193 "TU DOIS AVANT TOUT ABANDONNER TON BATEAU, AVANT QUE TU PUISSES #0 L ABANDONNER."			/* [city] */
#define romstr194 "TU NE PEUX PAS QUITTER LA VILLE,CAR TES GLISSEURS NE PEUVENT PAS PORTER AUTANT."
#define romstr195 "QUITTER LA VILLE"
#define romstr196 "#0 ABANDONNER?"										/* [city] */
#define romstr197 "CASERNE"
#define romstr198 "CASERNE"
#define romstr199 "LE GARDE NE TE LAISSE PAS PASSER!"
#define romstr200 "TON ARMEE:"
#define romstr201 "RETARDEMENT:"
#define romstr202 ":"
#define romstr203 "<-      ->"
#define romstr204 "----------"
#define romstr205 "--------"
#define romstr206 "\\]^_      `abc"
#define romstr207 "includes.c"								/* ! */
#define romstr208 "TUNNEL"
#define romstr209 "BANQUE"
#define romstr210 "BANQUE"
#define romstr211 "LA BANQUE ROYAL FALCON "
#define romstr212 "     RELEVE DE COMPTE :"
#define romstr213 "   CONDITIONS DU CREDIT :"
#define romstr214 " ARGENT CHOISI : \\]^_        `abc"
#define romstr215 "  TAUX D INTERETS ACTUEL : 4%"
#define romstr216 "    TAUX D INTERET DU CREDIT : 18%"
#define romstr217 "LE TAUX D INTERETS SERA CALCULE, LORSQUE VOUS"
#define romstr218 "REVIENDREZ LA PROCHAINE FOIS A LA BANQUE."
#define romstr219 "MARCHE DES ACTIONS"
#define romstr220 "TU RECOIS #0 D INTERETS!"					/* [amount of money] */
#define romstr221 "TU DOIS PAYER #0 D INTERETS."					/* [amount of money] */
#define romstr222 "TU DOIS PAYER #0 D INTERETS, MAIS TU N AS PAS ASSEZ D ARGENT. VA ET GAGNE-LE."		/* [amount of money] */
#define romstr223 "DIE BANK ANGREIFEN BRINGT #0."				/* [amount of money] */
#define romstr224 "LA BANQUE ROYAL FALCON NE TRAITE PAS D AFFAIRES AVEC UN BRIGAND COMME TOI."
#define romstr225 "MARCHE DES ACTIONS"
#define romstr226 "CHART-ANALYSE COUTE 10000. CONTINUER QUAND MEME?"
#define romstr227 "DEPOT"
#define romstr228 "PRIX"
#define romstr229 "  NOMBRE"
#define romstr230 "TA MARCHANDISE:    NOMBRE"
#define romstr231 "MARCHANDISE CHOISIE:"
#define romstr232 "NOMBRE     : \\]^_`abc"
#define romstr233 "PRIX      :"
#define romstr234 "BATEAUX DE COMMERCE:"
#define romstr235 "GLISSEUR:"
#define romstr236 "CHART-ANALYSE"
#define romstr237 "FRAIS DE COURTIER: 2%"
#define romstr238 "%f"
#define romstr239 "COTATION:"
#define romstr240 "version.c"									/* ! */
#define romstr241 "STRINGS.C"									/* ! */
#define romstr242 "FILENAME.C"								/* ! */
#define romstr243 "CONST.C"									/* ! */
#define romstr244 "GRUND.C"									/* ! */
#define romstr245 "TYPEDEF.C"									/* ! */
#define romstr246 "GLOBAL.C"									/* ! */
#define romstr247 "PROTOTYP.C"								/* ! */
#define romstr248 "MCODE.C"									/* ! */
#define romstr249 "TU N AS AUCUNE FREQUENCE QUARZ."
#define romstr250 "DANS CE PAYS,IL N EXISTE AUCUNE SOUTE A CHARBON."
#define romstr251 "OKAY SIR, NOUS CHERCHONS DES MATIERES PREMIERES..."
#define romstr252 "JE SUIS DESOLE, NOUS N AVONS RIEN TROUVE."
#define romstr253 "TRESOR A #0!"								/* [position] */
#define romstr254 "ENTREE DE LA SOUTE A CHARBON TROUVEE,COORDONNEE #0 !"		/* [position] */
#define romstr255 "#0 A #1."									/* [treasure][position] */
#define romstr256 "FERMETURE DE LA MINE (COUT: 30000)? TU AS: #0"	/* [money] */
#define romstr257 "DESOLE,MAIS NOUS AVONS DEJA 20 MINES."
#define romstr258 "TU AS BESOIN DE 30000 POUR UNE MINE, MAIS TU NAS PAS AUTANT D ARGENT."
#define romstr259 "...SIR !!!! NOUS AVONS TROUVE UN TRESOR! VALEUR: #0"		/* [value] */
#define romstr260 "TU AS REMPLI #0 BROUETTES D ORDURES. TU L AS PERDU."	/* [number] */
#define romstr261 "DRECK"
#define romstr262 "TU N AS PAS DE MACHINE D EXTRACTION."
#define romstr263 "TU AS DEJA 3 MACHINES D EXTRACTION."
#define romstr264 "TU N AS AUCUN SYSTEME DE POMPE!"
#define romstr265 "TU AS DEJA 3 SYSTEMES DE POMPES."
#define romstr266 "TU N AS AUCUN RESERVOIRE DE PETROLE!"
#define romstr267 "TU AS DEJA 3 RESERVOIRES DE PETROLE."
#define romstr268 "ES-TU SUR, DE VOULOIR FERMER CETTE MINE?"
#define romstr269 "MINE"
#define romstr270 "CHARGER LES GLISSEURS AU MAXIMUN.~~"
#define romstr271 "CREER UNE NOUVELLE MACHINE D EXTRACTION~(COUT: 5000)~~"
#define romstr272 "CONSTRUIR UN NOUVEAU SYSTEME DE POMPE~(COUT: 1000)~~"
#define romstr273 "CONSTRUIR DE NOUVEAUX RESERVOIRES DE PETROLE~(COUT: 4000)~~"
#define romstr274 "FERMETURE D UNE MINE~(COUT: 5000)"
#define romstr275 "ENTREPOT:~~"
#define romstr276 "CETTE MINE A ETE OUVERTE~~"
#define romstr277 "UND FÖRDERTE BISHER ÜBER~~~~~~"
#define romstr278 "GLISSEUR:"
#define romstr279 "ORDURE"
#define romstr280 "ORDURE."
#define romstr281 "RINGS OF MEDUSA 2 - CHEATS:"
#define romstr282 "R: MONTRER LES MATIERES PREMIERES"
#define romstr283 "*: BEAUCOUP D ARGENT"
#define romstr284 "W: ANNONCER LES MARCHANDISES"
#define romstr285 "S: ANNONCER LA FORCE DE COMBAT"
#define romstr286 "O: PRESENTER LES TIREURS"
#define romstr287 "T: PRESENTER LES TIREURS"
#define romstr288 "K: ENTRER DANS LA CASERNE"
#define romstr289 "+: GESCHW. ERHÖHEN"
#define romstr290 "0: PLUS D ARGENT"
#define romstr291 "B: BATAILLE NAVALE"
#define romstr292 "F: BATAILLE TERRESTRE"
#define romstr293 "C: MARCHER DANS UNION CITY "
#define romstr294 "X: TOUTES LES PORTES OUVERTES"
#define romstr295 "Z: MONTRER UN COMPTE RENDU"
#define romstr296 "M: UN MOIS DE PLUS"
#define romstr297 "D: ENTRER DANS LA SOUTE A CHARBON"
#define romstr298 "A: AUTOMAPPING"
#define romstr299 "L: LUMIERE AU MAXIMUN"
#define romstr300 "U: IMMORTEL DANS LA SOUTE A CHARBON"
#define romstr301 "DANS QUELLE SOUTE A CHARBON "
#define romstr302 "VOULEZ-VOUS RENTREZ?"
#define romstr303   :"
#define romstr304 "VOLCAN NON TROUVE!"
#define romstr305 "INTERNAL ERROR: \x22#0\x22."						/* ! */
#define romstr306 "CALCULE..."
#define romstr307 "SPECIAL PRESS-VERSION"							/* ! */
#define romstr308 "NO. 3"											/* ! */
#define romstr309 "TON ARMEE VEUT DE L ARGENT,"
#define romstr310 "MAIS TU N EN AS PAS ASSEZ."
#define romstr311 "ATTENTION! TU AS SEULEMENT DE L ARGENT POUR UN MOIS!"
#define romstr312 "NOTICE"
#define romstr313 "NOTICE"
#define romstr314 "0%"
#define romstr315 "100%"
#define romstr316 "TROP CHARGE"
#define romstr317 " PAS UN LA"
#define romstr318 "SAVE.C"										/* ! */
#define romstr319 "AIE!"
#define romstr320 ""
#define romstr321 "Ramdisquette"										/* ! */
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
#define romstr332 "FORCE"
#define romstr333 "ESPRIT COMBATIF"
#define romstr334 "RESISTENCE"
#define romstr335 "MANIABILITE"
#define romstr336 "INTELLIGENCE"
#define romstr337 "ANDRAS"
#define romstr338 "JAVO"
#define romstr339 "SIGMA"
#define romstr340 "DELIA"
#define romstr341 "KRETUS"
#define romstr342 "SHEILA"
#define romstr343 "ROMERA"
#define romstr344 "RANDONNEUR"
#define romstr345 "SPACY"
#define romstr346 "ZACK"
#define romstr347 "KENDO"
#define romstr348 "RAJAN"
#define romstr349 "RAZER"
#define romstr350 "LAWSON"
#define romstr351 "TYCOON"
#define romstr352 "PERSONNE"
#define romstr353 "INFANTERIE  "
#define romstr354 "CHAR      "
#define romstr355 "ARTILLERIE  "
#define romstr356 "ECLAIREUR   "
#define romstr357 "HELICOPTERE  "
#define romstr358 "INFIRMIER   "
#define romstr359 "TROUPE AU SOL "
#define romstr360 "FUSIL"
#define romstr361 "GRENADES"
#define romstr362 "BAZOOKA"
#define romstr363 "HOMME  "
#define romstr364 "ELFE    "
#define romstr365 "NAIN   "
#define romstr366 "HALBLING"
#define romstr367 "ORC     "
#define romstr368 "GNOME    "
#define romstr369 "GEANT   "
#define romstr370 "DEMON   "
#define romstr371 "OGRE    "
#define romstr372 "ZWARK   "
#define romstr373 "PAS LA"
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
#define romstr402 "CHALUTIER"
#define romstr403 "CHALAN"
#define romstr404 "PETROLIER"
#define romstr405 "VEDETTE"
#define romstr406 "DESTROYER"
#define romstr407 "CROISEUR"
#define romstr408 "LAIT"
#define romstr409 "FROMAGE"
#define romstr410 "SALADE"
#define romstr411 "POMME"
#define romstr412 "VIANDE"
#define romstr413 "POISSON"
#define romstr414 "ARMES"
#define romstr415 "CRAQUE"
#define romstr416 "ACIDE 54"
#define romstr417 "MEDICAMENT"
#define romstr418 "BATTERIES"
#define romstr419 "QUARZ"
#define romstr420 "ESCLAVES"
#define romstr421 "CHEVALEMENT DE MINE"
#define romstr422 "POMPE"
#define romstr423 "PETROLIER"
#define romstr424 "CYBORGS"
#define romstr425 "ANDROIDES"
#define romstr426 "ORDINATEUR"
#define romstr427 "PUCE"
#define romstr428 "KEROSEN"
#define romstr429 "GLISSEUR"
#define romstr430 "TRESOR"
#define romstr431 "FER"
#define romstr432 "DIAMANTS"
#define romstr433 "NITRIL"
#define romstr434 "PETROL"
#define romstr435 "GAZ"
#define romstr436 "GIMMICK"
#define romstr437 "OR"
#define romstr438 "PAS DE MOIS"
#define romstr439 "JANVIER"
#define romstr440 "FEVRIER"
#define romstr441 "MARS"
#define romstr442 "AVRIL"
#define romstr443 "MAI"
#define romstr444 "JUIN"
#define romstr445 "JUILLET"
#define romstr446 "AOUT"
#define romstr447 "SEPTEMBRE"
#define romstr448 "OCTOBRE"
#define romstr449 "NOVEMBRE"
#define romstr450 "DECEMBRE"
#define romstr451 "NIVEAU"
#define romstr452 "NIVEAU"
#define romstr453 "NIVEAU"
#define romstr454 "NIVEAU"
#define romstr455 "NIVEAU"
#define romstr456 "NIVEAU"
#define romstr457 "NIVEAU"
#define romstr458 "NIVEAU"
#define romstr459 "NIVEAU"
#define romstr460 "MARAIS"
#define romstr461 "RUINE"
#define romstr462 "DESERT ARIDE"
#define romstr463 "DESERT"
#define romstr464 "SEMI-DESERT"
#define romstr465 "PONT"
#define romstr466 "FORET BOISEE"
#define romstr467 "FORET"
#define romstr468 "COLLINE"
#define romstr469 "WADI"
#define romstr470 "BIDONVILLE"
#define romstr471 "CITY 1 (ENTRANCE)"
#define romstr472 "CITY 2 (ENTRANCE)"
#define romstr473 "CITY 3 (ENTRANCE)"
#define romstr474 "CITY 4 (ENTRANCE)"
#define romstr475 "NIVEAU"
#define romstr476 "NIVEAU"
#define romstr477 "PANNEAU"
#define romstr478 "OBSTACLE"
#define romstr479 "VOLCAN"
#define romstr480 "CITY 1 (PORT)"
#define romstr481 "CITY 2 (PORT)"
#define romstr482 "EAU"
#define romstr483 "MER OUVERTE"
#define romstr484 "includes.c"									/* ! */
#define romstr485 "COMBATANT
#define romstr486 "MAGICIEN"
#define romstr487 "SULTAN"
#define romstr488 "PRESTIDIGITATEUR"
#define romstr489 "CECI EST LA DERNIERE PORTE, QUI TE SEPARE DE  MEDUSA . DESIRES-TU VRAIMENT L OUVRIR?"
#define romstr490 "NOMBRE: "										/* ! */
#define romstr491 "AUTO MAPPING"
#define romstr492 "C EST LA FIN"
#define romstr493 "A PARTIR DE MAINTENANT,TU ES #0!"						/* [status e.g. wizard] */
#define romstr494 "TU AS TROUVE UN TRESOR D UNE VALEUR DE #0 !"		/* [value] */
#define romstr495 "JE NE PEUX PAS MANGER CA!"
#define romstr496 "FAIM    :    %"
#define romstr497 "SOIF     :    %"
#define romstr498 "SANTE:    %"
#define romstr499 "L HABILLEMENT  : #0%"							/* Amount */
#define romstr500 "STATUT:"
#define romstr501 "SÖLDNER           "
#define romstr502 "CECI EST UN BON REPAS!"
#define romstr503 "CECI EST QUELQUE CHOSE A BOIRE."
#define romstr504 "CECI EST UNE TROUSSE DE SOIN."
#define romstr505 "CECI EST UNE CLE DU BLOCKHAUS SECRET DE MEDUSA."
#define romstr506 "CECI EST UNE CLE."
#define romstr507 "CECI EST UNE ARME."
#define romstr508 "CE SONT DES MUNITIONS. IL RESTE ENCORE #0 BALLES."
#define romstr509 "CECI EST UN CASQUE."
#define romstr510 "CECI EST UNE CHEMISE."
#define romstr511 "CECI EST UN PANTALON."
#define romstr512 "CE SONT DES CHAUSSURES."
#define romstr513 "IL Y A DES MURS QUI RESSEMBLENT A DES MURS, MAIS..."
#define romstr514 "DE QUOI PEUT-IL BIEN S AGIR?"
#define romstr515 "  0%"
#define romstr516 "#0%"
#define romstr517 "OBJET FIXE INCONNU."					
#define romstr518 "PAS DE PISTE DE DEPART, DANS LE BLOCKHAUS"
#define romstr519 "ITEMSARRAY TROP GRAND. NEEDED: "
#define romstr520 "J AI BESOIN DE PLUS D ENNEMIS:"
#define romstr521 "JE N AI BESOIN,QUE D ENNEMIS:"
#define romstr522 "ITEMARRY TROP PETIT! NEEDED: #0"
#define romstr523 "SHRINKBUFFER SIZE: "
#define romstr524 ""
#define romstr525 "includes.c"									/* ! */
#define romstr526 "TU ES AFFAME."
#define romstr527 "TU ES ASSOIFFE."
#define romstr528 "TU ES TROP FAIBLE."
#define romstr529 "AUCUNE DIRECTION POUR L ENNEMI!"
#define romstr530 "TU AS DEJA QUATRE PERSONNES."
#define romstr531 "VEUX-TU LIBERER '#0' PRISONNIERS?"				/* [name] */
#define romstr532 "LA FORCE DE TIR EST L ADDITION DE L ENSEMBLE DES TIRS."
#define romstr533 "ZUM EINSCHALTEN IN DAS SCHWARZE FELD NEBEN DEM NAMEN DER PERSON KLICKEN."
#define romstr534 "HEUREUSEMENT QUE TU AS UN MIROIR...."
#define romstr535 "JE CROIS QUE TU AS OUBLIE DE PRENDRE QUELQUE CHOSE D IMPORTANT AVEC TOI...."
#define romstr536 "MES TROUPES NE M OBEISSENT PLUS...."
#define romstr537 "includes.c"									/* ! */
#define romstr538 "i"													/* ! */
#define romstr539 "TA VILLE #0 EST ATTAQUEE PAR #1."			/* [city][name] */
#define romstr540 "BATS-TOI POUR TON EMPIRE!"
#define romstr541 "TU RECOIS #0."					/* [money] */
#define romstr542 "TU AS TOUT PERDU, A L EXCEPTION DE TES GLISSEURS!"
#define romstr543 "ILLEGAL ANI-SPRITE"
#define romstr544 "MEDUSA"
#define romstr545 "TON ARMEE:"
#define romstr546 "'S ARMEE:"
#define romstr547 "'S ARMEE:"
#define romstr548 "NOMBRE DE SOLDATS:"
#define romstr549 "NOMBRE DE SOLDATS:"
#define romstr550 ":"
#define romstr551 ":"
#define romstr552 "%5.2f"
#define romstr553 "%5.2f"
#define romstr554 "CONTRE"
#define romstr555 "VEUX-TU ENVOYER TES ECLAIREURS, POUR VOIR, COMBIEN DE SOLDATS A TON ADVERSAIRE? COUT: 2000."
#define romstr556 "JE SUIS LE PRINCE LE PLUS PUISSANT! RENTRES CHEZ TOI~"
#define romstr557 "AUPRES DE TA FEMME ET AIDES LA A CUISINER!~~"
#define romstr558 "JE PAIES POUR LA PAIX."
#define romstr559 "OKAY,JE PRENDS L ARGENT,ET NE ME BAT PAS...."
#define romstr560 "SI PEU D ARGENT? TU PERDS DE TOUTE FACON LE COMBAT,ET JE RECUPERE TON ARGENT."
#define romstr561 "DESOLE D AVOIR CAUSE TANT DE DIFFICULTES. ET SALUT!!"
#define romstr562 "HA, HA, HA.... TOI,LE PRINCE SI PUISSANT? JE ME MARRE!"
#define romstr563  TU AS PERDU!"
#define romstr564 "TU AS CONQUERI #0."						/* [city] */
#define romstr565 "TU AS DEFENDU TON ARMEE!"
#define romstr566 "nr"
#define romstr567 "COMBAT DE CLANDESTINS INCONNUS"
#define romstr568 "%4.1f"
#define romstr569 "%4.1f"
#define romstr570 "includes.c"									/* ! */
#define romstr571 "seafight.h"									/* ! */
#define romstr572 "ANGLE:"	
#define romstr573 "%4.1f["
#define romstr574 "TIR DE L ADVERSAIRE(V0=%d, ANGLE=%d, Tan=%d.%03d, Const=%d.%03d\n"		/* ! */
#define romstr575 "TIR(V0=%d, ANGLE=%d, Tan=%d.%03d, Const=%d.%03d\n"			/* ! */
#define romstr576 "DISTANCE DE TIR:"
#define romstr577 "TOUCHE!  "
#define romstr578 "DISTANCE"
#define romstr579 "TOUCHE A L ADVERSAIRE: #0"					/* [number of hits] */
#define romstr580 "TOUCHE AU JOUEUR: #0"					/* [number of hits] */
#define romstr581 "BATS-TOI CONTRE #0"						/* [name] */
#define romstr582 "CAR TU T ES NOYE."
#define romstr583 ""
#define romstr584 "includes.c"											/* ! */
#define romstr585 "TA FLOTTE NE PEUT PAS ALLER A TERRE."
#define romstr586 "A CET ENDROIT,LA FRONTIERE N EST PAS FRANCHISSABLE..."
#define romstr587 "GROUND-NR TROP GRAND!"
#define romstr588 "ENTRER DANS LA MINE?"
#define romstr589 "TOI,FILS DU ROI MAUDIT,N A PAS ACCES A LA VILLE DE MEDUSA"
#define romstr590 "TU AS TROP SOUVENT ATTAQUE NOTRE CARAVANE OU NOTRE VILLE.NOUS NE TE LAISSONS PAS ENTRER!"
#define romstr591 "#0 BETRETEN?"											/* [city] */
#define romstr592 "HALLO SIR! NE DESIRES-TU PAS VENIR DANS NOTRE BELLE VILLE #0 ?"	/* [city] */
#define romstr593 "SIR, NOUS ATTAQUONS #0 :"									/* [city] */
#define romstr594 "PENETRER LE BLOCKHAUS SECET DE MEDUSA?"
#define romstr595 "PENETRER LE BLOCKHAUS?"
#define romstr596 "OUT OF GROUND (PUT)"
#define romstr597 "OUT OF GROUND (GET)"
#define romstr598 "LA SOCIETE D ARMATEURS DE #0 A CONSTRUIT UN #1 POUR TA FLOTTE."	/* [city][ship] */
#define romstr599 "PAS UN BATEAU DE LIBRE!"
#define romstr600 "X,Y POSITION?"
#define romstr601 "PAS DE CARBURANT"
#define romstr602 "PORT NON TROUVE!"
#define romstr603 "QUAI D APPAREILLAGE NON TROUVE!"
#define romstr604 "PAYS:"
#define romstr605 "X,Y:"
#define romstr606 "CLANDESTIN:"
#define romstr607 "ACTE DISPARU:"
#define romstr608 "PAS DE NOUVEL ITINERAIRE"
#define romstr609 "includes.c"									/* ! */
#define romstr610 "    ARGENT:~SOLDATS:~SOLDE DES VALEURS:"
#define romstr611 "SUR?"
#define romstr612 "SUR?"
#define romstr613 "SUR?"
#define romstr614 "LIGNE DE L IMAGE"										/* ! */
#define romstr615 "orgzeile"										/* ! */
#define romstr616 "option_nr"										/* ! */
#define romstr617 "LISTEAU VARIABLE TROP GRAND!"
#define romstr618 "LISTEAU #0"									/* ! */
#define romstr619 "includes.c"									/* ! */
#define romstr620 "INSERES S,T.P LA DISQUETTE           'X' DANS LE LECTEUR A:"		
#define romstr621 "INSERES S.T.P TA DISQUEETE SAVE DANS LE LECTEUR A:"
           

