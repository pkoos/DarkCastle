#ifndef CL_MONK_H_
#define CL_MONK_H_

#include "player.h"
#include "levels.h"
#include "fight.h"
#include "utility.h"
#include "character.h"
#include "spells.h"
#include "handler.h"
#include "connect.h"
#include "mobile.h"
#include "act.h"
#include "returnvals.h"
#include "db.h"
#include "room.h"
#include "interp.h"

// monk 10008 guildmaster - done and checked, Apoc
struct class_skill_defines k_skills[] = { // monk skills

//   Ability Name       Ability File          Level    Max   Group   Requisites
//   ------------       ------------          -----    ---   -----   ----------
{    "kick",            SKILL_KICK,             1,      90,  0,   STRDEX },
{    "dodge",           SKILL_DODGE,            2,      80,  0,   DEXWIS },
{    "redirect",        SKILL_REDIRECT,         3,      85,  0,   DEXWIS },
{    "trip",            SKILL_TRIP,             5,      70,  0,   DEXWIS },
{    "purify",          KI_PURIFY+KI_OFFSET,    8,     100,  0,   CONWIS },
{    "martial defense", SKILL_DEFENSE,          10,    100,  0,   DEXWIS },
{    "rescue",          SKILL_RESCUE,           12,     80,  0,   DEXWIS },
{    "punch",           KI_PUNCH+KI_OFFSET,     15,    100,  0,   STRDEX },
{    "eagleclaw",       SKILL_EAGLE_CLAW,       17,    100,  0,   CONWIS },
{    "dual wield",      SKILL_DUAL_WIELD,       20,     50,  0,   DEXWIS },
{    "sense",           KI_SENSE+KI_OFFSET,     21,    100,  0,   CONWIS },
{    "stance",          KI_STANCE+KI_OFFSET,    24,    100,  0,   CONWIS },
{    "speed",           KI_SPEED+KI_OFFSET,     27,    100,  0,   STRDEX },
{    "whipping",        SKILL_WHIPPING_WEAPONS, 30,     60,  0,   STRDEX },
{    "hand to hand",    SKILL_HAND_TO_HAND,     30,    100,  0,   STRDEX },
{    "agility",         KI_AGILITY+KI_OFFSET,   31,    100,  0,   STRDEX },
{    "stun",            SKILL_STUN,             33,     90,  0,   CONINT },
{    "storm",           KI_STORM+KI_OFFSET,     35,    100,  0,   CONINT },
{    "blindfighting",   SKILL_BLINDFIGHTING,    38,     90,  0,   CONWIS },
{    "quiver",          SKILL_QUIVERING_PALM,   40,    100,  0,   CONINT },
{    "blast",           KI_BLAST+KI_OFFSET,     45,    100,  0,   CONINT },
{    "disrupt",         KI_DISRUPT+KI_OFFSET,   47,    100,  0,   CONINT },
{    "meditation",      KI_MEDITATION+KI_OFFSET,50,    100,  0,   CONWIS },
{    "transfer",        KI_TRANSFER+KI_OFFSET,  51,    100,  0,   CONWIS },
{    "counter strike",  SKILL_COUNTER_STRIKE,   55,    100,  0,   STRDEX },
{    "\n",              0,                      1,      0,   0,   0 }
};

#endif