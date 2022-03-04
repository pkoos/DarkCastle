#ifndef CL_WARRIOR_H_
#define CL_WARRIOR_H_

#include "Maximums.h"
#include "character.h"
#include "player.h"
#include "fight.h"
#include "utility.h"
#include "handler.h"
#include "levels.h"
#include "connect.h"
#include "mobile.h"
#include "room.h"
#include "act.h"
#include "db.h"
#include "DC.h"
#include "returnvals.h"
#include "race.h"
#include "interp.h"
#include "const.h"

#include "class.h"
#include "spells.h"
// warrior 3023 guildmaster - done and checked, Apoc
struct class_skill_defines w_skills[] = { // warrior skills

//   Ability Name       Ability File            Level  Max  Group  Requisite
//   ------------       ------------            -----  ---  -----  ---------
{    "kick",            SKILL_KICK,               1,    80,  0,   STRDEX },
{    "bash",            SKILL_BASH,               2,    85,  0,   STRDEX },
{    "redirect",        SKILL_REDIRECT,           4,    90,  0,   DEXINT },
{    "rescue",          SKILL_RESCUE,             5,    70,  0,   DEXINT },
{    "double",          SKILL_SECOND_ATTACK,      7,    90,  0,   STRDEX },
{    "disarm",          SKILL_DISARM,             10,   70,  0,   DEXINT },
{    "headbutt",        SKILL_HEADBUTT,           12,   80,  0,   STRDEX },
{    "shield block",    SKILL_SHIELDBLOCK,        15,   85,  0,   STRCON },
{    "retreat",         SKILL_RETREAT,            17,  100,  0,   DEXINT },
{    "frenzy",          SKILL_FRENZY,             18,   80,  0,   STRCON },
{    "parry",           SKILL_PARRY,              20,   90,  0,   STRCON },
{    "blindfighting",   SKILL_BLINDFIGHTING,      21,   80,  0,   STRCON },
{"enhanced regeneration",SKILL_ENHANCED_REGEN,    22,   80,  0,   STRCON },
{    "triple",          SKILL_THIRD_ATTACK,       23,   90,  0,   STRDEX },
{    "hitall",          SKILL_HITALL,             25,   80,  0,   STRDEX },
{    "dual wield",      SKILL_DUAL_WIELD,         28,   90,  0,   DEXINT },
{    "bludgeoning",     SKILL_BLUDGEON_WEAPONS,   30,   90,  0,   CONWIS },
{    "crushing",        SKILL_CRUSHING_WEAPONS,   30,   90,  0,   CONWIS },
{    "piercing",        SKILL_PIERCEING_WEAPONS,  30,   90,  0,   CONWIS },
{    "slashing",        SKILL_SLASHING_WEAPONS,   30,   90,  0,   CONWIS },
{    "whipping",        SKILL_WHIPPING_WEAPONS,   30,   90,  0,   CONWIS },
{    "tactics",         SKILL_TACTICS,            31,  100,  0,   DEXINT },
{    "archery",         SKILL_ARCHERY,            32,   70,  0,   CONWIS },
{    "stun",            SKILL_STUN,               35,   85,  0,   DEXINT },
{    "guard",           SKILL_GUARD,              37,  100,  0,   STRCON },
{    "deathstroke",     SKILL_DEATHSTROKE,        39,  100,  0,   STRDEX },
{    "riposte",         SKILL_RIPOSTE,            40,  100,  0,   STRCON },
{    "two handers",     SKILL_TWO_HANDED_WEAPONS, 42,   85,  0,   CONWIS },
{    "stinging",        SKILL_STINGING_WEAPONS,   43,   90,  0,   CONWIS },
{    "skewer",          SKILL_SKEWER,             45,  100,  0,   STRDEX },
{    "blade shield",    SKILL_BLADESHIELD,        47,  100,  0,   STRCON },
{    "combat mastery",  SKILL_COMBAT_MASTERY,     50,  100,  0,   DEXINT },
{    "onslaught",       SKILL_ONSLAUGHT,          51,  100,  0,   DEXINT },
//{    "battlesense",     SKILL_BATTLESENSE,        53,  100,  1,   STRCON },
//{    "perseverance",    SKILL_PERSEVERANCE,       53,  100,  2,   CONWIS },
{    "triage",          SKILL_TRIAGE,             55,  100,  0,   DEXINT },
//{    "smite",           SKILL_SMITE,              57,  100,  1,   STRDEX },
//{    "leadership",      SKILL_LEADERSHIP,         57,  100,  2,   STRDEX },
//{    "execute",         SKILL_EXECUTE,            60,  100,  1,   CONWIS },
//{    "defenders stance",SKILL_DEFENDERS_STANCE,   60,  100,  2,   STRCON },
{    "\n",              0,                        1,    0,   0,   0 }
};

extern CWorld world;
extern struct index_data *obj_index;
 
bool ARE_GROUPED( CHAR_DATA *sub, CHAR_DATA *obj);
int attempt_move(CHAR_DATA *ch, int cmd, int is_retreat = 0);

extern struct index_data *mob_index;

#endif