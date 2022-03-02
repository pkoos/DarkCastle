#ifndef CL_THIEF_H_
#define CL_THIEF_H_


#include "MaxLength.h"
#include "NumberTypes.h"
#include "character.h"
#include "utility.h"
#include "spells.h"
#include "levels.h"
#include "player.h"
#include "obj.h"
#include "room.h"
#include "handler.h"
#include "mobile.h"
#include "fight.h"
#include "connect.h"
#include "interp.h"
#include "act.h"
#include "db.h"
#include <string.h>
#include "returnvals.h"
#include "clan.h"
#include "arena.h"
#include "const.h"
#include "inventory.h"

#include "class.h"

// thief 3022 guildmaster - done and checked, Apoc
struct class_skill_defines t_skills[] = { // thief skills

//   Ability Name       Ability File          Level    Max  Group  Requisites
//   ------------       ------------          -----    ---  -----  ----------
{    "backstab",        SKILL_BACKSTAB,         1,      90,  0,  STRDEX },  
{    "sneak",           SKILL_SNEAK,            2,      90,  0,  DEXINT },  
{    "parry",           SKILL_PARRY,            4,      40,  0,  CONINT },
{    "stalk",           SKILL_STALK,            6,     100,  0,  DEXINT },
{    "hide",            SKILL_HIDE,             7,      90,  0,  DEXINT },  
{    "dual wield",      SKILL_DUAL_WIELD,       10,     90,  0,  CONINT },  
{    "trip",            SKILL_TRIP,             11,     85,  0,  DEXINT },
{    "palm",            SKILL_PALM,             12,    100,  0,  DEXWIS },  
{    "slip",            SKILL_SLIP,             13,    100,  0,  DEXWIS },  
{    "dodge",           SKILL_DODGE,            15,     90,  0,  DEXWIS },
{    "jab",             SKILL_JAB,              17,    100,  0,  STRDEX },
{    "pocket",          SKILL_POCKET,           20,    100,  0,  DEXINT },
{    "appraise",        SKILL_APPRAISE,         21,    100,  0,  DEXWIS },
{    "pick",            SKILL_PICK_LOCK,        22,    100,  0,  DEXWIS },  
{    "steal",           SKILL_STEAL,            25,    100,  0,  DEXINT },
{    "blindfighting",   SKILL_BLINDFIGHTING,    28,     80,  0,  DEXWIS },
{    "piercing",        SKILL_PIERCEING_WEAPONS,30,     90,  0,  CONINT },
{    "slashing",        SKILL_SLASHING_WEAPONS, 30,     60,  0,  CONINT },
{    "bludgeoning",     SKILL_BLUDGEON_WEAPONS, 30,     70,  0,  CONINT },
{    "stinging",        SKILL_STINGING_WEAPONS, 30,     85,  0,  CONINT },  
{    "deceit",          SKILL_DECEIT,           31,    100,  0,  DEXWIS },  
{    "circle",          SKILL_CIRCLE,           35,    100,  0,  DEXINT },  
{    "disarm",          SKILL_DISARM,           38,     85,  0,  STRDEX },  
{    "dual backstab",   SKILL_DUAL_BACKSTAB,    40,    100,  0,  STRDEX },  
{    "eyegouge",        SKILL_EYEGOUGE,         42,    100,  0,  STRDEX },  
{    "vitalstrike",     SKILL_VITAL_STRIKE,     45,    100,  0,  STRDEX },  
{    "cripple",		    SKILL_CRIPPLE,		    50,    100,  0,  STRDEX },
{    "escape",		    SKILL_ESCAPE,		    51,    100,  0,  DEXWIS },
{    "critical hit",	SKILL_CRIT_HIT,		    55,    100,  0,  CONINT },
{    "\n",              0,                      1,      0,   0,  0 }
};

extern int rev_dir[];
extern CWorld world;

extern struct index_data *mob_index;
extern struct index_data *obj_index;
extern int top_of_world;
extern struct zone_data *zone_table;

int find_door(CHAR_DATA *ch, char *type, char *dir);
int get_weapon_damage_type(struct obj_data * wielded);
int check_autojoiners(CHAR_DATA *ch, int skill = 0);
int check_joincharmie(CHAR_DATA *ch, int skill = 0);

#endif