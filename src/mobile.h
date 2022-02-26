#ifndef MOBILE_H_
#define MOBILE_H_
/************************************************************************
| $Id: mobile.h,v 1.32 2010/01/01 03:03:14 jhhudso Exp $
| mobile.h
| Description:  This file contains the header information for mobile
|   control.
*/

#include "Actions.h"
#include <string>

using namespace std;


void rebuild_rnum_references(int startAt, int type);
void mprog_driver(char * com_list, 
  CHAR_DATA * mob, CHAR_DATA * actor, OBJ_DATA * obj, 
  void* vo, struct mprog_throw_type * thrw, CHAR_DATA * rndm);
char * mprog_next_command(char * clist);


bool charExists(CHAR_DATA *ch);

char *getTemp(CHAR_DATA *ch, char *name);

struct race_data
{
  char *singular_name;   /* Dwarf, Elf, etc.     */
  string lowercase_name; /* dwarf, elf, etc.     */
  char *plural_name;     /* dwarves, elves, etc. */
  bool playable;    /* Can a player play as this race? */
  long body_parts;  /* bitvector for body parts       */
  long immune;      /* bitvector for immunities       */
  long resist;      /* bitvector for resistances      */
  long suscept;     /* bitvector for susceptibilities */
  long hate_fear;   /* bitvector for hate/fear        */
  long friendly;    /* bitvector for friendliness     */
  int  min_weight;      /* min weight */
  int  max_weight;
  
  int  min_height;      
  int  max_height;

  unsigned min_str;
  unsigned max_str;
  int mod_str;

  unsigned min_dex;
  unsigned max_dex;
  int mod_dex;

  unsigned min_con;
  unsigned max_con;
  int mod_con;

  unsigned min_int;
  unsigned max_int;
  int mod_int;

  unsigned min_wis;
  unsigned max_wis;
  int mod_wis;
  
  int affects;      /* automatically added affects   */
  const char *unarmed;    // unarmed attack message
};

struct mob_matrix_data
{
  long long experience;
  int hitpoints;
  int tohit;
  int todam;
  int armor;
  int gold;
};


void translate_value(char *leftptr, char *rightptr, int16 **vali, uint32 **valui,
                char ***valstr, int64 **vali64, sbyte **valb, CHAR_DATA *mob, CHAR_DATA *actor,
                OBJ_DATA *obj, void *vo, CHAR_DATA *rndm);

void save_golem_data(CHAR_DATA *ch);
void save_charmie_data(CHAR_DATA *ch);

#endif
