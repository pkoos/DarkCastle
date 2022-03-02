#ifndef CL_PALADIN_H_
#define CL_PALADIN_H_

#include "MaxLength.h"
#include "character.h"
#include "player.h"
#include "fight.h"
#include "utility.h"
#include "spells.h"
#include "handler.h"
#include "levels.h"
#include "connect.h"
#include "mobile.h"
#include "room.h"
#include "act.h"
#include "db.h"
#include "returnvals.h"
#include "interp.h"

extern CWorld world;

// paladin 10006 guildmaster - done and checked, Apoc
struct class_skill_defines p_skills[] = { // paladin skills

//   Ability Name            Ability File            Level     Max  Group   Requisites 
//   ------------            ------------            -----     ---  -----   ---------- 
{    "layhands",             SKILL_LAY_HANDS,          1,     100,  0,   INTWIS },
{    "kick",                 SKILL_KICK,               2,      70,  0,   STRCON },
{    "bless",                SPELL_BLESS,              3,      90,  0,   CONWIS },
{    "double",               SKILL_SECOND_ATTACK,      5,      85,  0,   DEXINT },
{    "shield block",         SKILL_SHIELDBLOCK,        7,      90,  0,   CONWIS },
{    "rescue",               SKILL_RESCUE,             8,      85,  0,   CONWIS },
{    "cure light",           SPELL_CURE_LIGHT,         9,      85,  0,   INTWIS },
{    "dual wield",           SKILL_DUAL_WIELD,         10,     80,  0,   STRCON },
{    "detect poison",        SPELL_DETECT_POISON,      11,     85,  0,   STRWIS },
{    "create food",          SPELL_CREATE_FOOD,        12,     85,  0,   INTWIS },
{    "create water",         SPELL_CREATE_WATER,       13,     85,  0,   INTWIS },
{    "cure serious",         SPELL_CURE_SERIOUS,       15,     85,  0,   INTWIS },
{    "detect evil",          SPELL_DETECT_EVIL,        17,    100,  0,   STRWIS },
{    "remove poison",        SPELL_REMOVE_POISON,      18,     85,  0,   CONWIS },
{    "detect invisibility",  SPELL_DETECT_INVISIBLE,   20,     85,  0,   STRWIS },
{    "cure critical",        SPELL_CURE_CRITIC,        22,     85,  0,   INTWIS },
{    "parry",                SKILL_PARRY,              23,     70,  0,   DEXINT },
{    "bash",                 SKILL_BASH,               25,     80,  0,   STRCON },
{    "sense life",           SPELL_SENSE_LIFE,         26,     85,  0,   STRWIS },
{    "strength",             SPELL_STRENGTH,           28,     70,  0,   STRWIS },
{    "earthquake",           SPELL_EARTHQUAKE,         29,     70,  0,   STRCON },
{    "bludgeoning",          SKILL_BLUDGEON_WEAPONS,   30,     85,  0,   DEXINT },
{    "slashing",             SKILL_SLASHING_WEAPONS,   30,     85,  0,   DEXINT },
{    "crushing",             SKILL_CRUSHING_WEAPONS,   30,     85,  0,   DEXINT },
{    "blessed halo",         SPELL_BLESSED_HALO,       31,    100,  0,   STRWIS },
{    "triple",               SKILL_THIRD_ATTACK,       33,     80,  0,   DEXINT },
{    "two handers",          SKILL_TWO_HANDED_WEAPONS, 35,     80,  0,   DEXINT },
{    "heal",                 SPELL_HEAL,               37,     85,  0,   INTWIS },
{    "harm",                 SPELL_HARM,               38,     85,  0,   STRCON },
{    "sanctuary",            SPELL_SANCTUARY,          40,     85,  0,   CONWIS },
{    "holy aegis",           SPELL_AEGIS,              42,    100,  0,   STRWIS },
{    "dispel evil",          SPELL_DISPEL_EVIL,        43,     90,  0,   STRCON },
{    "protection from evil", SPELL_PROTECT_FROM_EVIL,  45,     85,  0,   CONWIS },
{    "resist cold",          SPELL_RESIST_COLD,        46,     70,  0,   STRWIS },
{    "divine fury",          SPELL_DIVINE_FURY,        48,    100,  0,   STRCON },
{    "behead",               SKILL_BEHEAD,             49,    100,  0,   DEXINT },
{    "holy aura",            SPELL_HOLY_AURA,          50,    100,  0,   CONWIS },
{    "heroism",              SPELL_HEROISM,            51,    100,  0,   CONWIS },
{    "consecrate",           SPELL_CONSECRATE,         55,    100,  0,   STRWIS },
//{    "spirit shield",        SPELL_SPIRIT_SHIELD,      57,    100,  0,   INTWIS },
{    "\n",                   0,                        1,      0,   0,   0 }
};

#endif