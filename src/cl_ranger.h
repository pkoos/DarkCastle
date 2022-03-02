#ifndef CL_RANGER_H_
#define CL_RANGER_H_

//#include <iostream.h>
#include "MaxLength.h"
#include "NumberTypes.h"

#include "character.h"
#include "Affect.h"
#include "mobile.h"
#include "utility.h"
#include "spells.h"
#include "isr.h"
#include "handler.h"
#include "room.h"
#include "terminal.h"
#include "player.h"
#include "levels.h"
#include "connect.h"
#include "fight.h"
#include "interp.h"
#include "db.h"
#include "act.h"
#include "fileinfo.h" // SAVE_DIR
#include "returnvals.h"
#include "inventory.h"
#include "const.h"

extern CWorld world;
extern struct zone_data *zone_table;
extern struct obj_data *object_list; 
extern int rev_dir[];
void save_corpses(void);

int saves_spell(CHAR_DATA *ch, CHAR_DATA *vict, int spell_base, int16 save_type);
void check_eq(CHAR_DATA *ch);
extern struct index_data *mob_index;
int get_difficulty(int);

// ranger 10013 guildmaster - done and checked, Apoc
struct class_skill_defines r_skills[] = { // ranger skills

//   Ability Name       Ability File           Level     Max  Group   Requisites 
//   ------------       ------------           -----     ---  -----   ---------- 
{    "bee sting",       SPELL_BEE_STING,         1,     100,  0,   STRCON },
{    "hide",            SKILL_HIDE,              2,      80,  0,   CONWIS },
{    "kick",            SKILL_KICK,              3,      70,  0,   STRDEX },
{    "dual wield",      SKILL_DUAL_WIELD,        5,      90,  0,   STRDEX },
{    "redirect",        SKILL_REDIRECT,          7,      80,  0,   CONWIS },
{    "eyes of the owl", SPELL_EYES_OF_THE_OWL,   8,      85,  0,   STRCON },
{    "sense life",      SPELL_SENSE_LIFE,        9,      85,  0,   CONWIS },
{    "dodge",    	SKILL_DODGE,    	 10,     70,  0,   CONWIS },
{    "tame",            SKILL_TAME,              11,    100,  0,   STRCON },
{    "double",          SKILL_SECOND_ATTACK,     12,     85,  0,   STRDEX },
{    "free animal",     SKILL_FREE_ANIMAL,       13,    100,  0,   STRCON },
{    "feline agility",  SPELL_FELINE_AGILITY,    14,    100,  0,   STRCON },
{    "bee swarm",       SPELL_BEE_SWARM,         15,    100,  0,   STRCON },
{    "forage",          SKILL_FORAGE,            16,     85,  0,   INTWIS },
{    "entangle",        SPELL_ENTANGLE,          18,     85,  0,   INTWIS },
{    "archery",         SKILL_ARCHERY,           20,     90,  0,   DEXINT },
{    "blindfighting",   SKILL_BLINDFIGHTING,     21,     85,  0,   CONWIS },
{    "parry",           SKILL_PARRY,             22,     80,  0,   CONWIS },
{    "herb lore",       SPELL_HERB_LORE,         23,     85,  0,   INTWIS },
{    "poison",          SPELL_POISON,            25,     85,  0,   INTWIS },
{    "tempest arrows",  SKILL_TEMPEST_ARROW,     26,    100,  0,   DEXINT },
{    "track",           SKILL_TRACK,             28,    100,  0,   DEXINT },
{    "barkskin",        SPELL_BARKSKIN,          29,     85,  0,   INTWIS },
{    "piercing",        SKILL_PIERCEING_WEAPONS, 30,     85,  0,   STRDEX },
{    "slashing",        SKILL_SLASHING_WEAPONS,  30,     85,  0,   STRDEX },
{    "whipping",        SKILL_WHIPPING_WEAPONS,  30,     90,  0,   STRDEX },
{    "ice arrows",      SKILL_ICE_ARROW,         31,    100,  0,   DEXINT },
{    "rescue",          SKILL_RESCUE,            32,     85,  0,   STRDEX },
{    "trip",            SKILL_TRIP,              33,     85,  0,   STRDEX },
{    "ambush",          SKILL_AMBUSH,            35,    100,  0,   DEXINT },
{    "fire arrows",     SKILL_FIRE_ARROW,        36,    100,  0,   DEXINT },
{    "call follower",   SPELL_CALL_FOLLOWER,     38,    100,  0,   STRCON },
{    "stun",            SKILL_STUN,              40,     80,  0,   INTWIS },
{    "granite arrows",  SKILL_GRANITE_ARROW,     41,    100,  0,   DEXINT },
{    "disarm",          SKILL_DISARM,            42,     80,  0,   CONWIS },
{    "staunchblood",    SPELL_STAUNCHBLOOD,      44,     85,  0,   CONWIS },
{    "forest meld",     SPELL_FOREST_MELD,       45,     85,  0,   INTWIS },
{    "camouflage",      SPELL_CAMOUFLAGE,        46,     85,  0,   INTWIS },
{    "creeping death",  SPELL_CREEPING_DEATH,    48,    100,  0,   STRCON },
{ "natural selection",	SKILL_NAT_SELECT,	 50,	100,  0,   DEXINT },
{    "make camp",       SKILL_MAKE_CAMP,          51,  100,  0,   CONWIS },
{    "offhand double",  SKILL_OFFHAND_DOUBLE,    55,    100,  0,   STRDEX },
{      "\n",            0,                       1,      0,   0,   0 }
};

#endif