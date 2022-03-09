#ifndef CL_MAGE_H_
#define CL_MAGE_H_

#include "CombatAffects.h"
#include "Maximums.h"
#include "NumberTypes.h"
#include "player.h"
#include "levels.h"
#include "character.h"
#include "spells.h"
#include "utility.h"
#include "fight.h"
#include "mobile.h"
#include "connect.h"
#include "handler.h"
#include "act.h"
#include "interp.h"
#include "returnvals.h"
#include "room.h"
#include "db.h"

extern CWorld world;
extern struct spell_info_type spell_info[MAX_SPL_LIST];

// mage 3020 guildmaster - done and checked, Apoc
struct class_skill_defines m_skills[] = { // mage skills

//   Ability Name           Ability File           Level     Max  Group   Requisites   
//   ------------           ------------           -----     ---  -----   ----------   
{    "magic missile",       SPELL_MAGIC_MISSILE,     1,     100,  0,   STRINT }, 
{    "ventriloquate",       SPELL_VENTRILOQUATE,     2,     100,  0,   DEXWIS },
{    "clarity",             SPELL_CLARITY,           2,     100,  0,   DEXINT },
{    "detect magic",        SPELL_DETECT_MAGIC,      3,      90,  0,   INTWIS },
{    "detect invisibility", SPELL_DETECT_INVISIBLE,  4,      90,  0,   INTWIS },
{    "invisibility",        SPELL_INVISIBLE,         5,      90,  0,   DEXWIS },
{    "burning hands",       SPELL_BURNING_HANDS,     6,     100,  0,   STRINT },
{    "armor",               SPELL_ARMOR,             7,      85,  0,   CONWIS },
{    "continual light",     SPELL_CONT_LIGHT,        8,      90,  0,   STRCON },
{    "refresh",             SPELL_REFRESH,           9,      90,  0,   DEXWIS },
{    "lightning bolt",      SPELL_LIGHTNING_BOLT,    10,    100,  0,   STRINT },
{    "infravision",         SPELL_INFRAVISION,       11,     90,  0,   INTWIS },
{    "fly",                 SPELL_FLY,               12,    100,  0,   DEXINT },
{    "strength",            SPELL_STRENGTH,          13,     90,  0,   DEXINT },
{    "fear",                SPELL_FEAR,              15,     90,  0,   CONWIS },
{    "identify",            SPELL_IDENTIFY,          16,    100,  0,   INTWIS },
{    "locate object",       SPELL_LOCATE_OBJECT,     17,     90,  0,   INTWIS },
{    "call familiar",       SPELL_SUMMON_FAMILIAR,   18,     85,  0,   STRCON },
{    "dismiss familiar",    SPELL_DISMISS_FAMILIAR,  18,     85,  0,   STRCON },
{    "chill touch",         SPELL_CHILL_TOUCH,       20,     90,  0,   DEXWIS },
{    "shield",              SPELL_SHIELD,            21,    100,  0,   CONWIS },
{    "souldrain",           SPELL_SOULDRAIN,         22,    100,  0,   DEXWIS },
{    "ethereal focus",      SPELL_ETHEREAL_FOCUS,    24,    100,  0,   DEXWIS },
{    "dispel minor",        SPELL_DISPEL_MINOR,      25,     90,  0,   CONWIS },
{    "mass invisibility",   SPELL_MASS_INVISIBILITY, 26,    100,  0,   DEXWIS },
{    "firestorm",           SPELL_FIRESTORM,         27,    100,  0,   STRINT },
{    "portal",              SPELL_PORTAL,            28,     90,  0,   STRCON },
{    "fireball",            SPELL_FIREBALL,          29,    100,  0,   STRINT },
{    "focused repelance",   SKILL_FOCUSED_REPELANCE, 30,    100,  0,   CONWIS },
{    "piercing",            SKILL_PIERCEING_WEAPONS, 30,     50,  0,   DEXINT },
{    "bludgeoning",         SKILL_BLUDGEON_WEAPONS,  30,     50,  0,   DEXINT },
{    "resist magic",        SPELL_RESIST_MAGIC,      31,     90,  0,   CONWIS },
{    "haste",               SPELL_HASTE,             33,    100,  0,   DEXINT },
{    "true sight",          SPELL_TRUE_SIGHT,        34,     90,  0,   INTWIS },
{    "dispel magic",        SPELL_DISPEL_MAGIC,      35,     90,  0,   CONWIS },
{    "resist fire",         SPELL_RESIST_FIRE,       36,     70,  0,   CONWIS },
{    "wizard eye",          SPELL_WIZARD_EYE,        37,    100,  0,   INTWIS },
{    "teleport",            SPELL_TELEPORT,          38,    100,  0,   DEXWIS },
{    "stoneskin",           SPELL_STONE_SKIN,        39,     70,  0,   CONWIS },
{    "meteor swarm",        SPELL_METEOR_SWARM,      40,    100,  0,   STRCON },
{    "life leech",          SPELL_LIFE_LEECH,        41,    100,  0,   CONWIS },
{    "word of recall",      SPELL_WORD_OF_RECALL,    42,     90,  0,   DEXWIS },
{    "create golem",        SPELL_CREATE_GOLEM,      43,    100,  0,   STRCON },
{    "release golem",       SPELL_RELEASE_GOLEM,     43,    100,  0,   STRCON },
{    "mend golem",          SPELL_MEND_GOLEM,        44,    100,  0,   STRCON },
{    "hellstream",          SPELL_HELLSTREAM,        45,    100,  0,   STRINT },
{    "fireshield",          SPELL_FIRESHIELD,        47,    100,  0,   STRINT },
{    "paralyze",            SPELL_PARALYZE,          48,    100,  0,   DEXINT },
{    "solar gate",          SPELL_SOLAR_GATE,        49,    100,  0,   STRINT },
{    "spellcraft",          SKILL_SPELLCRAFT,        50,    100,  0,   INTWIS },
{    "imbue",               SKILL_IMBUE,             51,    100,  0,   INTWIS },
{    "elemental filter",    SKILL_ELEMENTAL_FILTER,  55,    100,  0,   DEXWIS },
{    "\n",                  0,                       1,      0,   0,   0 }
};

#endif