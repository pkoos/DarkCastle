#ifndef SKILL_IDS_H_
#define SKILL_IDS_H_

#define SKILL_BASE                   300
#define SKILL_TRIP                   300
#define SKILL_DODGE                  301
#define SKILL_SECOND_ATTACK          302
#define SKILL_DISARM                 303
#define SKILL_THIRD_ATTACK           304
#define SKILL_PARRY                  305
#define SKILL_DEATHSTROKE            306
#define SKILL_CIRCLE                 307
#define SKILL_BERSERK                308
#define SKILL_HEADBUTT               309
#define SKILL_EAGLE_CLAW             310
#define SKILL_QUIVERING_PALM         311
#define SKILL_PALM                   312
#define SKILL_STALK                  313
#define SKILL_UNUSED                 314
#define SKILL_DUAL_BACKSTAB          315
#define SKILL_HITALL                 316
#define SKILL_STUN                   317
#define SKILL_SCAN                   318
#define SKILL_CONSIDER               319
#define SKILL_SWITCH                 320
#define SKILL_REDIRECT               321
#define SKILL_AMBUSH                 322
#define SKILL_FORAGE                 323
#define SKILL_TAME                   324
#define SKILL_TRACK                  325
#define SKILL_SKEWER		     326
#define SKILL_SLIP		     327
#define SKILL_RETREAT		     328
#define SKILL_RAGE		     329
#define SKILL_BATTLECRY		     330
#define SKILL_ARCHERY		     331
#define SKILL_RIPOSTE                332
#define SKILL_LAY_HANDS              333
#define SKILL_INSANE_CHANT           334
#define SKILL_GLITTER_DUST           335
#define SKILL_SNEAK                  336
#define SKILL_HIDE                   337
#define SKILL_STEAL                  338
#define SKILL_BACKSTAB               339
#define SKILL_PICK_LOCK              340
#define SKILL_KICK                   341
#define SKILL_BASH                   342 
#define SKILL_RESCUE                 343
#define SKILL_BLOOD_FURY             344
#define SKILL_DUAL_WIELD             345
#define SKILL_HARM_TOUCH             346
#define SKILL_SHIELDBLOCK            347
#define SKILL_BLADESHIELD            348
#define SKILL_POCKET                 349
#define SKILL_GUARD                  350
#define SKILL_FRENZY                 351
#define SKILL_BLINDFIGHTING          352
#define SKILL_FOCUSED_REPELANCE      353
#define SKILL_VITAL_STRIKE           354
#define SKILL_CRAZED_ASSAULT         355
#define SKILL_DIVINE_PROTECTION      356
#define SKILL_BLUDGEON_WEAPONS       357
#define SKILL_PIERCEING_WEAPONS      358
#define SKILL_SLASHING_WEAPONS       359
#define SKILL_WHIPPING_WEAPONS       360
#define SKILL_CRUSHING_WEAPONS       361
#define SKILL_TWO_HANDED_WEAPONS     362
#define SKILL_HAND_TO_HAND           363
#define SKILL_BULLRUSH               364
#define SKILL_FEROCITY               365
#define SKILL_TACTICS                366
#define SKILL_DECEIT                 367
#define SKILL_RELEASE		     368
#define SKILL_FEARGAZE		     369
#define SKILL_EYEGOUGE		     370
#define SKILL_MAGIC_RESIST	     371
#define NEW_SAVE		     372 // Savefix.
#define SKILL_SPELLCRAFT	     373
#define SKILL_DEFENSE 		     374 // MArtial defense
#define SKILL_KNOCKBACK              375
#define SKILL_STINGING_WEAPONS       376
#define SKILL_JAB		     377
#define SKILL_APPRAISE               378
#define SKILL_NATURES_LORE           379
#define SKILL_FIRE_ARROW             380
#define SKILL_ICE_ARROW              381
#define SKILL_TEMPEST_ARROW          382
#define SKILL_GRANITE_ARROW          383
#define DO_NOT_USE_I_SUCK	     384 // Oh how convenient skills are for this, 
					// and so bad looking. yay. I want mysql.
#define META_REIMB		     385
#define SKILL_COMBAT_MASTERY		     386
#define SKILL_FASTJOIN		     387
#define SKILL_ENHANCED_REGEN         388
#define SKILL_CRIPPLE                389
#define SKILL_NAT_SELECT             390
#define SKILL_CLANAREA_CLAIM         391
#define SKILL_CLANAREA_CHALLENGE     392
#define SKILL_COMMUNE                393
#define SKILL_SCRIBE                 394
#define SKILL_MAKE_CAMP              395
#define SKILL_BATTLESENSE            396
#define SKILL_PERSEVERANCE           397
#define SKILL_TRIAGE                 398
#define SKILL_SMITE                  399
#define SKILL_LEADERSHIP             400
#define SKILL_EXECUTE                401
#define SKILL_DEFENDERS_STANCE       402
#define SKILL_BEHEAD                 403
#define SKILL_PRIMAL_FURY            404
#define SKILL_VIGOR		     405
#define SKILL_ESCAPE		     406
#define SKILL_CRIT_HIT		     407
#define SKILL_BATTERBRACE	     408
#define SKILL_FREE_ANIMAL            409
#define SKILL_OFFHAND_DOUBLE	     410
#define SKILL_ONSLAUGHT		     411
#define SKILL_COUNTER_STRIKE	     412
#define SKILL_IMBUE		     413
#define SKILL_ELEMENTAL_FILTER	     414
#define SKILL_ORCHESTRATE	     415
#define SKILL_TUMBLING		     416
#define SKILL_BREW                   417
#define SKILL_PURSUIT		     418
//			                     419
//warrior
#define SKILL_LEGIONNAIRE			420
#define SKILL_GLADIATOR				421
// Barbarian
#define SKILL_BATTLERAGER			422
#define SKILL_CHIEFTAN				423
//thief
#define SKILL_PILFERER				424
#define SKILL_ASSASSIN				425
//mage
#define SKILL_WARMAGE				426
#define SKILL_SPELLBINDER			427
//cleric
#define SKILL_ZEALOT				428
#define SKILL_RITUALIST				429
//druid
#define SKILL_ELEMENTALIST			430
#define SKILL_SHAPESHIFTER			431
//anti-paladin
#define SKILL_CULTIST				432
#define SKILL_REAVER				433
//paladin
#define SKILL_TEMPLAR				434
#define SKILL_INQUISITOR			435
//ranger
#define SKILL_SCOUT					436
#define SKILL_TRACKER				437
//monk
#define SKILL_SENSEI				438
#define SKILL_SPIRITUALIST			439
//bard
#define SKILL_TROUBADOUR			440
#define SKILL_MINISTREL				441

#define SKILL_MAX                   441

// if you add a skill, make sure you update "skills[]" in spells.C
// as well as SKILL_MAX

#endif
