/***************************************************************************
 *  file: spells.h , Implementation of magic spells.       Part of DIKUMUD *
 *  Usage : Spells                                                         *
 *  Copyright (C) 1990, 1991 - see 'license.doc' for complete information. *
 *                                                                         *
 *  Copyright (C) 1992, 1993 Michael Chastain, Michael Quan, Mitchell Tse  *
 *  Performance optimization and bug fixes by MERC Industries.             *
 *  You can use our stuff in any way you like whatsoever so long as ths   *
 *  copyright notice remains intact.  If you like it please drop a line    *
 *  to mec@garnet.berkeley.edu.                                            *
 *                                                                         *
 *  This is free software and you are benefitting.  We hope that you       *
 *  share your changes too.  What goes around, comes around.               *
 ***************************************************************************/
/* $Id: spells.h,v 1.142 2015/06/14 02:38:12 pirahna Exp $ */

#ifndef SPELLS_H_
#define SPELLS_H_

#include "AttackTypes.h"
#include "SkillTimers.h"
#include "SkillIds.h"
#include "SpellIds.h"
#include "NumberTypes.h"
#include "handler.h"
#include "obj.h"
#include <map>

void extractFamiliar(CHAR_DATA *ch);

bool skill_success(CHAR_DATA *ch, CHAR_DATA *victim, int skillnum, int mod=0);
bool canPerform(char_data * const &ch, const int_fast32_t &learned, std::string failMessage = std::string());


/* New skill quest thingy. */
struct skill_quest
{
  struct skill_quest *next;
  char *message;
  int num;
  int clas;
  int level;
};

struct skill_stuff
{
  char *name;
  int difficulty;
};

void barb_magic_resist(char_data *ch, int old, int nw);
struct skill_quest *find_sq(int sq);
struct skill_quest *find_sq(char *);
int dam_percent(int learned, int damage);
void check_maxes(CHAR_DATA *ch);



/* 
 * KI usage is here to avoid code duplication
 */
#define KI_BLAST      0
#define KI_PUNCH      1
#define KI_SENSE      2
#define KI_STORM      3
#define KI_SPEED      4
#define KI_PURIFY     5
#define KI_DISRUPT    6
#define KI_STANCE     7
#define KI_AGILITY    8
#define KI_MEDITATION 9
#define KI_TRANSFER   10
#define KI_OFFSET     250     // why this is done differently than the rest, I have no
                              // idea....ki skills are 250-296.  -pir

#define FUCK_PTHIEF                  297
#define FUCK_GTHIEF                  298
#define FUCK_CANTQUIT                299




#define SKILL_SONG_BASE              525
#define SKILL_SONG_LIST_SONGS        525
#define SKILL_SONG_WHISTLE_SHARP     526
#define SKILL_SONG_STOP              527
#define SKILL_SONG_TRAVELING_MARCH   528
#define SKILL_SONG_BOUNT_SONNET      529
#define SKILL_SONG_INSANE_CHANT      530
#define SKILL_SONG_GLITTER_DUST      531
#define SKILL_SONG_SYNC_CHORD        532
#define SKILL_SONG_HEALING_MELODY    533
#define SKILL_SONG_STICKY_LULL       534
#define SKILL_SONG_REVEAL_STACATO    535
#define SKILL_SONG_FLIGHT_OF_BEE     536
#define SKILL_SONG_JIG_OF_ALACRITY   537
#define SKILL_SONG_NOTE_OF_KNOWLEDGE 538
#define SKILL_SONG_TERRIBLE_CLEF     539
#define SKILL_SONG_SOOTHING_REMEM    540
#define SKILL_SONG_FORGETFUL_RHYTHM  541
#define SKILL_SONG_SEARCHING_SONG    542
#define SKILL_SONG_VIGILANT_SIREN    543
#define SKILL_SONG_ASTRAL_CHANTY     544
#define SKILL_SONG_DISARMING_LIMERICK 545
#define SKILL_SONG_SHATTERING_RESO   546
#define SKILL_SONG_UNRESIST_DITTY    547
#define SKILL_SONG_FANATICAL_FANFARE 548
#define SKILL_SONG_DISCHORDANT_DIRGE 549
#define SKILL_SONG_CRUSHING_CRESCENDO 550
#define SKILL_SONG_HYPNOTIC_HARMONY  551
#define SKILL_SONG_MKING_CHARGE      552
#define SKILL_SONG_SUBMARINERS_ANTHEM 553
#define SKILL_SONG_SUMMONING_SONG    554
#define SKILL_SONG_MAX               554
// if you add a song, make sure you update "songs[]" in sing.C
// as well as SKILL_SONG_MAX

// God commands that are "bestow"/"revoke"able
#define COMMAND_BASE                 600
#define COMMAND_STRING               600
#define COMMAND_IMP_CHAN             601
#define COMMAND_STAT                 602
#define COMMAND_SNOOP                603
#define COMMAND_FIND                 604
#define COMMAND_POSSESS              606
#define COMMAND_RESTORE              607
#define COMMAND_PURLOIN              608
#define COMMAND_ARENA                609
#define COMMAND_SET                  610
#define COMMAND_SQSAVE               611
#define COMMAND_WHATTONERF           612
#define COMMAND_FORCE                613
#define COMMAND_SEND                 614
#define COMMAND_LOAD                 615
#define COMMAND_SHUTDOWN             616
#define COMMAND_MP_EDIT              617
#define COMMAND_RANGE                618
#define COMMAND_MPSTAT               619
#define COMMAND_SEDIT                621
#define COMMAND_SOCKETS              622
#define COMMAND_PUNISH               623
#define COMMAND_SQEDIT		     624
#define COMMAND_OCLONE		     625
#define COMMAND_RELOAD 		     626
#define COMMAND_HEDIT		     627
#define COMMAND_OPSTAT		     629
#define COMMAND_OPEDIT		     630
#define COMMAND_EQMAX		     631
#define COMMAND_LOG 		     632
#define COMMAND_ADDNEWS		     633
#define COMMAND_PRIZE		     634
#define COMMAND_QEDIT                635
#define COMMAND_RENAME		     636
#define COMMAND_FINDPATH             637
#define COMMAND_FINDPATH2            638
#define COMMAND_ADDROOM              639
#define COMMAND_NEWPATH              640
#define COMMAND_LISTPATHSBYZONE      641
#define COMMAND_LISTALLPATHS         642
#define COMMAND_TESTHAND             643
#define COMMAND_DOPATHPATH           644
#define COMMAND_DO_THE_THING         645
#define COMMAND_QUEST                646
#define COMMAND_TESTPORT             647
#define COMMAND_REMORT		     648
#define COMMAND_TESTHIT		     649
#define COMMAND_TESTUSER             650
// make sure up you update bestowable_god_commands_type bestowable_god_commands[]
// if you modify this command list any


#define SKILL_TRADE_BASE             700
#define SKILL_TRADE_POISON           700
#define SKILL_TRADE_MAX              700
// make sure you update tradeskills[] in combinables.cpp if you add to this


#define SKILL_RECALL                 800
#define INTERNAL_SLEEPING            801

#define SKILL_FLAMESLASH	     850 // just used internally for flameslash, do not want in skill lists etc

/*
 * Only for dragon breaths, not char abilities.
 */
#define SPELL_FIRE_BREATH            900
#define SPELL_GAS_BREATH             901
#define SPELL_FROST_BREATH           902
#define SPELL_ACID_BREATH            903
#define SPELL_LIGHTNING_BREATH       904

#define BASE_TIMERS 1100

// NOTE  "skill" numbers 1500-1599 are reserved for innate skill abilities
// These are in innate.h

///////////////////////////////////////////////////////////////////////

#define TAR_IGNORE         1
#define TAR_CHAR_ROOM      1<<1
#define TAR_CHAR_WORLD     1<<2
#define TAR_FIGHT_SELF     1<<3
#define TAR_FIGHT_VICT     1<<4
#define TAR_SELF_ONLY      1<<5
#define TAR_SELF_NONO      1<<6
#define TAR_OBJ_INV        1<<7
#define TAR_OBJ_ROOM       1<<8
#define TAR_OBJ_WORLD      1<<9
#define TAR_OBJ_EQUIP      1<<10
#define TAR_NONE_OK        1<<11
#define TAR_SELF_DEFAULT   1<<12
#define TAR_ROOM_EXIT      1<<13

////////////////////////////////////////////////////////////////////////

#define ETHEREAL_FOCUS_TRIGGER_ACT     1
#define ETHEREAL_FOCUS_TRIGGER_MOVE    2
#define ETHEREAL_FOCUS_TRIGGER_SOCIAL  3

////////////////////////////////////////////////////////////////////////



typedef	int	SPELL_FUN	( ubyte level, CHAR_DATA *ch,
				  char *arg, int type,
				  CHAR_DATA *tar_ch,
				  struct obj_data *tar_obj,
                                  int skill );

// NOTE:  If you change this structure, keep in mind how it is used in guild.C
// The min_level_XXX stuff MUST be updated in guild.C if you change this.  It is
// using an offset from min_level_magic depending on class *(min_level_magic+2bytes)
struct spell_info_type
{
    uint32_t	beats;			/* Waiting time after spell	*/
    ubyte	minimum_position;	/* Position for caster		*/
    ubyte	min_usesmana;		/* Mana used			*/
    int16	targets;		/* Legal targets		*/
    SPELL_FUN *	spell_pointer;		/* Function to call		*/
    int16      difficulty; 		/* Spell difficulty */
};


#define SPELL_TYPE_SPELL    0
#define SPELL_TYPE_POTION   1
#define SPELL_TYPE_WAND     2
#define SPELL_TYPE_STAFF    3
#define SPELL_TYPE_SCROLL   4


#define FIRE_ELEMENTAL	88
#define WATER_ELEMENTAL	89
#define AIR_ELEMENTAL	90
#define EARTH_ELEMENTAL	91

#define WILD_OFFENSIVE  0
#define WILD_DEFENSIVE  1

/*
 * Attack types with grammar.
 */
struct attack_hit_type
{
  char *singular;
  char *plural;
};

#define RARE1_PAPER	1
#define RARE2_PAPER	1<<1
#define RARE3_PAPER	1<<2
#define RARE4_PAPER	1<<3
#define RARE5_PAPER	1<<4
//#define FREE_SLOT	1<<5

#define CLERIC_PEN 	1<<6
#define MAGE_PEN 	1<<7
#define DRUID_PEN 	1<<8
#define ANTI_PEN	1<<9
#define RANGER_PEN	1<<10
#define NONE_PEN	1<<11
//#define FREE_SLOT	1<<12

#define MAGIC_INK	1<<13
#define FIRE_INK	1<<14
#define EVIL_INK	1<<15
//#define FREE_SLOT	1<<16

#define FLASHY_DUST	1<<17
#define EXPLOSIVE_DUST	1<<18
#define GENERIC_DUST	1<<19
//#define FREE_SLOT	1<<20

#define FILTER_FIRE	1
#define	FILTER_MAGIC	2
#define FILTER_COLD	3
#define	FILTER_ENERGY	4
#define FILTER_ACID	5
#define FILTER_POISON	6

/*
 * reasons for stopping following
 * passed as the cmd arg to stop_follow()
 */
#define STOP_FOLLOW	0
#define END_STALK	1
#define CHANGE_LEADER	2
#define BROKE_CHARM	3
#define BROKE_CHARM_LILITH	4

  #define DETECT_GOOD_VNUM 6302
  #define DETECT_EVIL_VNUM 6301
  #define DETECT_INVISIBLE_VNUM 6306
  #define SENSE_LIFE_VNUM 6304
  #define INFRA_VNUM 6308
  #define INVIS_VNUM 6303
  #define FARSIGHT_VNUM 6307
  #define SOLIDITY_VNUM 6309
  #define LIGHTNING_SHIELD_VNUM 6310
  #define INSOMNIA_VNUM 6311
  #define HASTE_VNUM 6312
  #define TRUE_VNUM 6305

// search through a character's list to see if they have a particular skill
// if so, return their level of knowledge
// if not, return 0
inline int has_skill(CHAR_DATA *ch, int16 skill) {
	struct char_skill_data * curr = ch->skills;
	struct obj_data *o;
	int bonus = 0;

	if (IS_MOB(ch))
		return 0;

	if (affected_by_spell(ch, SKILL_DEFENDERS_STANCE) && skill == SKILL_DODGE)
		return affected_by_spell(ch, SKILL_DEFENDERS_STANCE)->modifier;

	if (affected_by_spell(ch, SPELL_VILLAINY))
		bonus += affected_by_spell(ch, SPELL_VILLAINY)->modifier / 5;

	if (affected_by_spell(ch, SPELL_HEROISM))
		bonus += affected_by_spell(ch, SPELL_HEROISM)->modifier / 5;

	while (curr) {
		if (curr->skillnum == skill) {
			if (!IS_NPC(ch))
				for (o = ch->pcdata->skillchange; o; o = o->next_skill) {
					int a;
					for (a = 0; a < o->num_affects; a++) {
						if (o->affected[a].location == skill * 1000) {
							bonus += o->affected[a].modifier;
							if ((int) curr->learned + bonus > 150)
								bonus = 150 - curr->learned;
						}
					}
				}
			return ((int) curr->learned) + bonus;
		}
		curr = curr->next;
	}
	return 0;
}


#endif
