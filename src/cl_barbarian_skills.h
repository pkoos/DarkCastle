#ifndef CL_BARBARIAN_SKILLS_H_
#define CL_BARBARIAN_SKILLS_H_

#include "character.h"

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

#endif