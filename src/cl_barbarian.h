#ifndef CL_BARBARIAN_H_
#define CL_BARBARIAN_H_

#include "MaxLength.h"
#include "player.h"
#include "levels.h"
#include "character.h"
#include "spells.h"
#include "utility.h"
#include "fight.h"
#include "mobile.h"
#include "magic.h"
#include "connect.h"
#include "handler.h"
#include "act.h"
#include "interp.h"
#include "returnvals.h"
#include "room.h"
#include "db.h"
#include "clan.h"
#include "utility.h"
#include "const.h"

extern struct index_data *obj_index;
extern int rev_dir[];
extern bool str_prefix(const char *astr, const char *bstr);
extern CWorld world;
int attempt_move(CHAR_DATA *ch, int cmd, int is_retreat = 0);
int find_door(CHAR_DATA *ch, char *type, char *dir);

int do_batter(CHAR_DATA *ch, char *argument, int cmd);
int do_brace(CHAR_DATA *ch, char *argument, int cmd);
int do_rage(CHAR_DATA *ch, char *argument, int cmd);
int do_battlecry(CHAR_DATA *ch, char *argument, int cmd);
int do_berserk(CHAR_DATA *ch, char *argument, int cmd);
int do_headbutt(CHAR_DATA *ch, char *argument, int cmd);
int do_bloodfury(CHAR_DATA *ch, char *argument, int cmd);
int do_crazedassault(CHAR_DATA *ch, char *argument, int cmd);
void rush_reset(void *arg1, void *arg2, void *arg3);
int do_bullrush(CHAR_DATA *ch, char *argument, int cmd);
int do_ferocity(CHAR_DATA *ch, char *argument, int cmd);
void barb_magic_resist(char_data *ch, int old, int nw);
int do_knockback(CHAR_DATA *ch, char *argument, int cmd);
int do_primalfury(CHAR_DATA *ch, char *argument, int cmd);
int do_pursue(CHAR_DATA *ch, char *argument, int cmd);

// barbarian 10007 guildmaster - done and checked, Apoc
struct class_skill_defines b_skills[] = { // barbarian skills

//   Ability Name       Ability File            Level  Max  Group  Requisites
//   ------------       ------------            -----  ---  -----  ----------
{    "dual wield",      SKILL_DUAL_WIELD,         1,    85,  0,  DEXCON },
{    "bash",            SKILL_BASH,               2,    90,  0,  STRINT },
{    "kick",            SKILL_KICK,               3,    80,  0,  STRINT },
{    "parry",           SKILL_PARRY,              5,    70,  0,  DEXCON },
{    "double",          SKILL_SECOND_ATTACK,      8,    85,  0,  DEXWIS },
{    "dodge",           SKILL_DODGE,  	          10,   70,  0,  DEXCON },
{    "blood fury",      SKILL_BLOOD_FURY,         12,  100,  0,  STRCON },
{    "crazed assault",  SKILL_CRAZED_ASSAULT,     15,  100,  0,  STRCON },
{    "frenzy",          SKILL_FRENZY,             18,   90,  0,  DEXCON },
{    "rage",            SKILL_RAGE,               20,  100,  0,  STRCON },
{"enhanced regeneration",SKILL_ENHANCED_REGEN,    22,   90,  0,  DEXCON },
{    "triple",          SKILL_THIRD_ATTACK,       25,   85,  0,  DEXWIS },
{    "battlecry",       SKILL_BATTLECRY,          27,  100,  0,  STRINT },
{    "blindfighting",   SKILL_BLINDFIGHTING,      28,   60,  0,  DEXCON },
{    "whipping",        SKILL_WHIPPING_WEAPONS,   30,   85,  0,  DEXWIS },
{    "piercing",        SKILL_PIERCEING_WEAPONS,  30,   85,  0,  DEXWIS },
{    "slashing",        SKILL_SLASHING_WEAPONS,   30,   85,  0,  DEXWIS },
{    "bludgeoning",     SKILL_BLUDGEON_WEAPONS,   30,   85,  0,  DEXWIS },
{    "crushing",        SKILL_CRUSHING_WEAPONS,   30,   85,  0,  DEXWIS },
{    "ferocity",        SKILL_FEROCITY,           31,  100,  0,  STRINT },
{    "headbutt",        SKILL_HEADBUTT,           33,   90,  0,  STRINT },
{    "two handers",     SKILL_TWO_HANDED_WEAPONS, 35,   90,  0,  DEXWIS },
{    "archery",         SKILL_ARCHERY,            38,   80,  0,  DEXWIS },
{    "berserk",         SKILL_BERSERK,            40,  100,  0,  STRCON },
{    "hitall",          SKILL_HITALL,             45,   90,  0,  STRCON },
{    "magic resistance", SKILL_MAGIC_RESIST,      47,  100,  0,  DEXCON },
{    "knockback",       SKILL_KNOCKBACK,          48,  100,  0,  STRINT },
{    "bullrush",        SKILL_BULLRUSH,           50,  100,  0,  STRCON },
{    "batterbrace",     SKILL_BATTERBRACE,        51,  100,  0,  DEXWIS },
{    "vigor",           SKILL_VIGOR,              55,  100,  0,  DEXCON },
{    "pursuit",         SKILL_PURSUIT,            55,  100,  1,  DEXCON },
{    "\n",              0,                        1,    0,   0,  0 }
};

#endif