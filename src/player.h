#ifndef PLAYER_H_
#define PLAYER_H_
/************************************************************************
| $Id: player.h,v 1.44 2010/02/19 06:09:18 jhhudso Exp $
| player.h
| Description: This file defines the player vectors..punishment, specials,
|   etc...
*/
#include "NumberTypes.h"
#include "ItemApply.h"
#include "punish.h" // punishment vectors
#include "structs.h" // ubyte
#include "class.h"
/************************************************************************
| CHAR_DATA.misc vector
|  These should probably not be combined, and they probably shouldn't
|  be here, but they are.  Morc XXX
|  I really shouldn't have the flag here to differentiate mob's from PC's
|  but it's as good a place as any and this is all fubar'd anyway.
|  -pir 7/30
*/
#define LOG_BUG            1U
#define LOG_PRAYER         1U<<1
#define LOG_GOD            1U<<2
#define LOG_MORTAL         1U<<3
#define LOG_SOCKET         1U<<4
#define LOG_MISC           1U<<5
#define LOG_PLAYER         1U<<6
#define CHANNEL_GOSSIP     1U<<7
#define CHANNEL_AUCTION    1U<<8
#define CHANNEL_INFO       1U<<9
#define CHANNEL_TRIVIA     1U<<10
#define CHANNEL_DREAM      1U<<11
#define CHANNEL_CLAN       1U<<12
#define CHANNEL_NEWBIE     1U<<13
#define CHANNEL_SHOUT      1U<<14
#define LOG_WORLD          1U<<15
#define LOG_ARENA          1U<<16
#define LOG_CLAN           1U<<17
#define LOG_WARNINGS       1U<<18
#define LOG_HELP           1U<<19

#define LOG_DATABASE	   1U<<20
#define LOG_OBJECTS        1U<<21
#define CHANNEL_TELL       1U<<22
#define CHANNEL_HINTS      1U<<23
#define LOG_VAULT          1U<<24
#define LOG_QUEST          1U<<25
#define LOG_DEBUG          1U<<26
// ...
#define MISC_IS_OBJ        1U<<30
#define MISC_IS_MOB        1U<<31

/************************************************************************
| Thirst, FULL, etc..
*/
#define DRUNK         0
#define FULL          1
#define THIRST        2

/************************************************************************
| Player vectors
| CHAR_DATA->pcdata->toggles
*/
#define PLR_BRIEF        1U
#define PLR_COMPACT      1U<<1
#define PLR_DONTSET      1U<<2
#define PLR_DONOTUSE     1U<<3
#define PLR_NOHASSLE     1U<<4
#define PLR_SUMMONABLE   1U<<5
#define PLR_WIMPY        1U<<6
#define PLR_ANSI         1U<<7
#define PLR_VT100        1U<<8
#define PLR_ONEWAY       1U<<9
#define PLR_DISGUISED    1U<<10
#define PLR_UNUSED       1U<<11
#define PLR_PAGER        1U<<12
#define PLR_BEEP         1U<<13
#define PLR_BARD_SONG    1U<<14
#define PLR_ANONYMOUS    1U<<15
#define PLR_AUTOEAT      1U<<16
#define PLR_LFG          1U<<17
#define PLR_CHARMIEJOIN  1U<<18
#define PLR_NOTAX	     1U<<19
#define PLR_GUIDE	     1U<<20
#define PLR_GUIDE_TOG	 1U<<21
#define PLR_NEWS         1U<<22
#define PLR_50PLUS	     1U<<23
#define PLR_ASCII        1U<<24
#define PLR_DAMAGE       1U<<25
#define PLR_CLS_TREE_A   1U<<26
#define PLR_CLS_TREE_B   1U<<27
#define PLR_CLS_TREE_C   1U<<28 //might happen one day
#define PLR_EDITOR_WEB   1U<<29
#define PLR_REMORTED     1U<<30
#define PLR_NODUPEKEYS   1U<<31

/*  For cut and paste purposes
   switch(GET_CLASS(mob))
   {
      case CLASS_MAGIC_USER:
      case CLASS_CLERIC:
      case CLASS_THIEF:
      case CLASS_WARRIOR:
      case CLASS_ANTI_PAL:
      case CLASS_PALADIN:
      case CLASS_BARBARIAN:
      case CLASS_MONK:
      case CLASS_RANGER:
      case CLASS_BARD:
      case CLASS_DRUID:
      case CLASS_PSIONIC:
      case CLASS_NECROMANCER:
      default:
         break;
   }

*/



/*
 1000+ are reserved, so if you were thinking about using, think
 again.
*/
/* RESERVED: 100-150 for more weapon affects */
/* Morc XXX */


//different stat combos for skill groups
#define STRDEX 1
#define STRCON 2
#define STRINT 3
#define STRWIS 4
#define DEXCON 5
#define DEXINT 6
#define DEXWIS 7
#define CONINT 8
#define CONWIS 9
#define INTWIS 10

#define MAX_PROFESSIONS 2

// Constructor commented out for const.C initialization purposes
struct str_app_type
{
    int16 todam;    /* Damage Bonus/Penalty                */
    int16 carry_w;  /* Maximum weight that can be carrried */
    int16 cold_resistance; /* Cold resistance */
};

struct dex_app_type
{
  int16 tohit;
  int16 ac_mod;
  int16 move_gain;
  int16 fire_resistance;
};

// Constructor commented out for const.C initialization purposes
struct wis_app_type
{
    int16 mana_regen;
    int16 ki_regen;
    int16 bonus;       /* how many bonus skills a player can */
		      /* practice pr. level                 */
    int16 energy_resistance;
    int16 conc_bonus;
    int16 spell_dam_bonus; // For Cleric/Druid/Paladins naturally
};

// Constructor commented out for const.C initialization purposes
struct int_app_type
{
    int16 mana_regen;
    int16 ki_regen;
    int16 easy_bonus;
    int16 medium_bonus;
    int16 hard_bonus;
    int16 learn_bonus;
    int16 magic_resistance;
    int16 conc_bonus;
    int16 spell_dam_bonus; // For Mage/Anti/Bard
};

// Constructor commented out for const.C initialization purposes
struct con_app_type
{
    int16 hp_regen;
    int16 move_regen;
    int16 hp_gain;
    int16 poison_resistance;
};

/* Extern definitions. These are all in const.cpp. */
extern const struct dex_app_type dex_app[];
extern const struct con_app_type con_app[];
extern const struct int_app_type int_app[];
extern const struct str_app_type str_app[];
extern const struct wis_app_type wis_app[];

/* Various function declarations */
int get_saves(CHAR_DATA *ch, int savetype);


#endif
