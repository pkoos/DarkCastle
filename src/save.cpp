/***************************************************************************
 *  file: save.c, Database module.                        Part of DIKUMUD  *
 *  Usage: Saving and loading of characters                                *
 *  Copyright (C) 1990, 1991 - see 'license.doc' for complete information. *
 *                                                                         *
 *  Copyright (C) 1992, 1993 Michael Chastain, Michael Quan, Mitchell Tse  *
 *  Rewritten by MERC Industries, based on crash.c by prometheus           *
 *  (Taquin Ho) and abaddon (Jeff Stile).                                  *
 *  You can use our stuff in any way you like whatsoever so long as ths   *
 *  copyright notice remains intact.  If you like it please drop a line    *
 *  to mec\@garnet.berkeley.edu.                                            *
 *                                                                         *
 *  This is free software and you are benefitting.  We hope that you       *
 *  share your changes too.  What goes around, comes around.               *
 ***************************************************************************/
/* $Id: save.cpp,v 1.76 2015/06/15 01:06:10 pirahna Exp $ */

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
//#include <sys/stat.h>
//#include <unistd.h>
}

#include <QFile>

#include "obj.h"
#include "room.h"
#include "character.h"
#include "mobile.h"
#include "utility.h"
#include "spells.h"
#include "fileinfo.h" // SAVE_DIR
#include "player.h"
#include "levels.h"
#include "db.h"
#include "connect.h"
#include "handler.h"
#include "race.h"
#include "vault.h"
#include "const.h"
#include "guild.h"

#ifdef USE_SQL
#include <iostream>
#include <libpq-fe.h>
#include "Backend/Database.h"

extern Database db;
#endif

using namespace std;

extern struct index_data *obj_index;
extern CWorld world;

struct obj_data * obj_store_to_char( CHAR_DATA *ch, FILE *fpsave, struct obj_data * last_cont );
bool put_obj_in_store( struct obj_data *obj, CHAR_DATA *ch, FILE *fpsave, int wear_pos);
bool put_obj_in_store( struct obj_data *obj, CHAR_DATA *ch, QDataStream &out, int wear_pos);
void restore_weight(struct obj_data *obj);
void store_to_char(struct char_file_u4 *st, CHAR_DATA *ch);
char *fread_alias_string(FILE *fpsave);

// return 1 on success
// return 0 on failure
// donno where it would fail off hand though unless we ran out of HD space
// or had a failure.  I'm just not willing to code that much fault protection in
// -pir
int save_char_aliases(char_player_alias * alias, FILE * fpsave)
{
  uint32 tmp_size = 0;
  char_player_alias * curr;

  // count up the number of aliases
  for(curr = alias; curr; curr = curr->next)
    tmp_size++;

  // write out how many
  fwrite(&tmp_size, sizeof(tmp_size), 1, fpsave);

  // write the aliases out
  for(curr = alias; curr; curr = curr->next)
  {
    // note that we save the number of characters in tmp_size
    tmp_size = strlen(curr->keyword);
    if(tmp_size < 1)  // minimal error checking:)
      continue;
    fwrite (&tmp_size, sizeof(tmp_size), 1, fpsave);
    // but we actually write tmp_size +1 to get the trailing \0
    fwrite (curr->keyword, sizeof(char), (tmp_size+1), fpsave);

    tmp_size = strlen(curr->command);
    fwrite (&tmp_size, sizeof(tmp_size), 1, fpsave);
    fwrite (curr->command, sizeof(char), (tmp_size+1), fpsave);
  }
  return 1; 
}

// return 1 on success
// return 0 on failure
// donno where it would fail off hand though unless we ran out of HD space
// or had a failure.  I'm just not willing to code that much fault protection in
// -pir
int save_char_aliases(char_player_alias * alias, QDataStream &out)
{
  uint32 tmp_size = 0;
  char_player_alias * curr;

  // count up the number of aliases
  for(curr = alias; curr; curr = curr->next)
    tmp_size++;

  // write out how many
  out.writeRawData((const char *)&tmp_size, sizeof(tmp_size));

  // write the aliases out
  for(curr = alias; curr; curr = curr->next)
  {
    // note that we save the number of characters in tmp_size
    tmp_size = strlen(curr->keyword);
    if(tmp_size < 1)  // minimal error checking:)
      continue;
    out.writeRawData((const char *)&tmp_size, sizeof(tmp_size));
    // but we actually write tmp_size +1 to get the trailing \0
    out.writeRawData(curr->keyword, tmp_size+1);

    tmp_size = strlen(curr->command);
    out.writeRawData((const char *)&tmp_size, sizeof(tmp_size));
    out.writeRawData(curr->command, tmp_size+1);
  }
  return 1; 
}

// return pointer to aliases or NULL
struct char_player_alias * read_char_aliases(FILE * fpsave)
{
  uint32 total, x;  
  struct char_player_alias * top = NULL;  
  struct char_player_alias * curr = NULL;  

  fread(&total, sizeof(total), 1, fpsave);

  if(total < 1)
    return NULL;

  for(x = 0; x < total; x++) 
  {
    curr = new char_player_alias;

    curr->keyword = fread_alias_string(fpsave);
    curr->command = fread_alias_string(fpsave);
    if (curr->keyword == NULL || curr->command == NULL) {
       if (curr->keyword == NULL && curr->command)
          dc_free(curr->command);
       if (curr->command == NULL && curr->keyword)
          dc_free(curr->keyword);
       dc_free(curr);
    } else {
       curr->next = top;
       top = curr;
    }
  }
  return top;
}

char *fread_alias_string(FILE *fpsave)
{
    uint32 tmp_size;
    char *buf = NULL;

    fread (&tmp_size, sizeof tmp_size, 1, fpsave);
    if(tmp_size > 0) {
       if (tmp_size > MAX_INPUT_LENGTH) {
          /* flush this string and continue on with the next */
          while (fgetc(fpsave))
             ;
       } else {
          buf = new char[tmp_size + 1];
          fread(buf, sizeof(char), (tmp_size+1), fpsave);
       }
    }
    return(buf);
}

void fwrite_var_string(const char * string, FILE * fpsave)
{
   uint16 tmp_size;

   if(string) {
     tmp_size = strlen(string);
     tmp_size++; // count the null terminator
     fwrite(&tmp_size, sizeof(tmp_size), 1, fpsave);
     fwrite(string, sizeof(char), (tmp_size), fpsave);
   }
   else {
     tmp_size = 0;
     fwrite(&tmp_size, sizeof(tmp_size), 1, fpsave);
   }
}

void fwrite_var_string(char * str, QDataStream &out)
{
   quint16 tmp_size = 0;

   if(str) {
     tmp_size = strlen(str);
     tmp_size++; // count the null terminator
     out.writeRawData((const char *)&tmp_size, sizeof(tmp_size));
     out.writeRawData(str, tmp_size);
   }
   else {
     out.writeRawData((const char *)&tmp_size, sizeof(tmp_size));
   }
}


char * fread_var_string(FILE * fpsave)
{
   quint16 tmp_size = 0;
   char * tmp_str = nullptr;

   size_t readItems = fread(&tmp_size, sizeof (tmp_size), 1, fpsave);  
   if(readItems == 1 && tmp_size > 0) {
     tmp_str = new char[tmp_size];
     fread(tmp_str, sizeof(char), tmp_size, fpsave);

     return tmp_str;
   }
   else return nullptr;
}

void save_mob_data(struct mob_data * i, FILE * fpsave)
{
  fwrite(&(i->nr),          sizeof(i->nr),          1, fpsave);
  fwrite(&(i->default_pos), sizeof(i->default_pos), 1, fpsave);
  fwrite(&(i->attack_type), sizeof(i->attack_type), 1, fpsave); 
  fwrite(&(i->actflags),    sizeof(i->actflags),    1, fpsave);
  fwrite(&(i->damnodice),   sizeof(i->damnodice),   1, fpsave);
  fwrite(&(i->damsizedice), sizeof(i->damsizedice), 1, fpsave);

  // Any future additions to this save file will need to be placed LAST here with a 3 letter code
  // and appropriate strcmp statement in the read_mob_data object

  fwrite("STP", sizeof(char), 3, fpsave);
}

void save_mob_data(mob_data *i, QDataStream &out)
{
  out.writeRawData((const char *)&(i->nr),          sizeof(i->nr));
  out.writeRawData((const char *)&(i->default_pos), sizeof(i->default_pos));
  out.writeRawData((const char *)&(i->attack_type), sizeof(i->attack_type)); 
  out.writeRawData((const char *)&(i->actflags),    sizeof(i->actflags));
  out.writeRawData((const char *)&(i->damnodice),   sizeof(i->damnodice));
  out.writeRawData((const char *)&(i->damsizedice), sizeof(i->damsizedice));

  // Any future additions to this save file will need to be placed LAST here with a 3 letter code
  // and appropriate strcmp statement in the read_mob_data object

  out.writeRawData("STP", 3);
}

void read_mob_data(struct mob_data * i, FILE * fpsave)
{
  char typeflag[4];

  fread(&(i->nr),          sizeof(i->nr),          1, fpsave);
  fread(&(i->default_pos), sizeof(i->default_pos), 1, fpsave);
  fread(&(i->attack_type), sizeof(i->attack_type), 1, fpsave); 
  fread(&(i->actflags),    sizeof(i->actflags),    1, fpsave);
  fread(&(i->damnodice),   sizeof(i->damnodice),   1, fpsave);
  fread(&(i->damsizedice), sizeof(i->damsizedice), 1, fpsave);

  typeflag[3] = '\0';
  fread(&typeflag, sizeof(char), 3, fpsave);

  // Add new items in this format
//  if(!strcmp(typeflag, "XXX"))
//    do_something

  // Any future additions to this read file will need to be placed LAST

  // at this point, typeflag should = "STP", and we're done reading mob data
}

// TODO - make sure I go back and update the time_data structs everywhere when 
// we lose link, or logout, etc so that the 'played' variable is correct

void fwrite_string_tilde(FILE *fpsave)
{
  char buf[40] = {0};
  strncpy(buf, "Bugfixbugfixbugfixbugfixbugfixbugfix~", sizeof(buf));
  fwrite(&buf, 37, 1, fpsave); 
}

void fwrite_string_tilde(QDataStream &out)
{
  char buf[40] = {0};
  strncpy(buf, "Bugfixbugfixbugfixbugfixbugfixbugfix~", sizeof(buf));
  out.writeRawData((const char *)&buf, 37);
}


void save_pc_data(struct pc_data * i, FILE * fpsave, struct time_data tmpage)
{
  fwrite(i->pwd,            sizeof(char),        PASSWORD_LEN+1, fpsave);
  save_char_aliases(i->alias, fpsave);

  fwrite_string_tilde(fpsave);
  fwrite(&(i->rdeaths),     sizeof(i->rdeaths),  1, fpsave);
  fwrite(&(i->pdeaths),     sizeof(i->pdeaths),  1, fpsave);
  fwrite(&(i->pkills),      sizeof(i->pkills),   1, fpsave);
  fwrite(&(i->pklvl),       sizeof(i->pklvl),    1, fpsave);
  // we save tmpage cause it was calculated when all eq was off
  fwrite(&(tmpage),         sizeof(time_data),   1, fpsave);
  fwrite(&(i->bad_pw_tries),sizeof(i->bad_pw_tries), 1, fpsave);
  fwrite(&(i->practices),   sizeof(i->practices), 1, fpsave);
  fwrite(&(i->bank),        sizeof(i->bank),     1, fpsave);
  fwrite(&(i->toggles),     sizeof(i->toggles),  1, fpsave);
  fwrite(&(i->punish),      sizeof(i->punish),   1, fpsave);
  fwrite_var_string(i->last_site, fpsave);
  fwrite_var_string(i->poofin, fpsave);
  fwrite_var_string(i->poofout, fpsave);
  fwrite_var_string(i->prompt, fpsave);
  fwrite_var_string("NewSaveType",fpsave);

  // Quest bitvector one
  if(i->quest_bv1) {
    fwrite("QS1", sizeof(char), 3, fpsave);
    fwrite(&(i->quest_bv1), sizeof(i->quest_bv1), 1, fpsave);
  }

  // Saving throw mods
  fwrite("SVM", sizeof(char), 3, fpsave);
  fwrite(&(i->saves_mods), sizeof(i->saves_mods[0]), SAVE_TYPE_MAX+1, fpsave);  // Write the whole array

  // Specializations
  fwrite("SPC", sizeof(char), 3, fpsave);
  fwrite(&(i->specializations), sizeof(i->specializations), 1, fpsave);

  // Stat metas
  if(i->statmetas) {
    fwrite("STM", sizeof(char), 3, fpsave);
    fwrite(&(i->statmetas), sizeof(i->statmetas), 1, fpsave);
  }

  // Ki metas
  if(i->kimetas) {
    fwrite("KIM", sizeof(char), 3, fpsave);
    fwrite(&(i->kimetas), sizeof(i->kimetas), 1, fpsave);
  }
  // autojoinin'
  if(i->joining) {
    fwrite("JIN", sizeof(char), 3, fpsave);
    fwrite_var_string(i->joining, fpsave);
  }

  fwrite("QST", sizeof(char), 3, fpsave);
  fwrite(&(i->quest_points), sizeof(i->quest_points), 1, fpsave);
  for(int j=0;j<QUEST_CANCEL;j++)
    fwrite(&(i->quest_cancel[j]), sizeof(i->quest_cancel[j]), 1, fpsave);
  for(int j=0;j<=QUEST_TOTAL/ASIZE;j++)
    fwrite(&(i->quest_complete[j]), sizeof(i->quest_complete[j]), 1, fpsave);
  if (i->buildLowVnum) {
    fwrite("BLO", sizeof(char), 3, fpsave);
    fwrite(&(i->buildLowVnum), sizeof(i->buildLowVnum), 1, fpsave);
  }
  if (i->buildHighVnum) {
    fwrite("BHI", sizeof(char), 3, fpsave);
    fwrite(&(i->buildHighVnum), sizeof(i->buildHighVnum), 1, fpsave);
  }
  if (i->buildMLowVnum) {
    fwrite("BMO", sizeof(char), 3, fpsave);
    fwrite(&(i->buildMLowVnum), sizeof(i->buildMLowVnum), 1, fpsave);
  }
  if (i->buildMHighVnum) {
    fwrite("BMI", sizeof(char), 3, fpsave);
    fwrite(&(i->buildMHighVnum), sizeof(i->buildMHighVnum), 1, fpsave);
  }
  if (i->buildOLowVnum) {
    fwrite("BOO", sizeof(char), 3, fpsave);
    fwrite(&(i->buildOLowVnum), sizeof(i->buildOLowVnum), 1, fpsave);
  }
  if (i->buildOHighVnum) {
    fwrite("BOI", sizeof(char), 3, fpsave);
    fwrite(&(i->buildOHighVnum), sizeof(i->buildOHighVnum), 1, fpsave);
  }
  if (i->profession) {
    fwrite("PRO", sizeof(char), 3, fpsave);
    fwrite(&(i->profession), sizeof(i->profession), 1, fpsave);
  }
  if (i->wizinvis) {
	fwrite("WIZ", sizeof(char), 3, fpsave);
	fwrite(&(i->wizinvis), sizeof(i->wizinvis), 1, fpsave);
  }
  if (i->options && i->options->empty() == false)
  {
    for (auto &opt : *i->options)
    {
      if (opt.first == "color.good" ||
          opt.first == "color.bad")
      {
        fwrite("OPT", sizeof(char),3, fpsave);
        fwrite_var_string(opt.first.c_str(),fpsave);
        fwrite_var_string(opt.second.c_str(),fpsave);
      }
    }
  }

  // Any future additions to this save file will need to be placed LAST here with a 3 letter code
  // and appropriate strcmp statement in the read_mob_data object

  fwrite("STP", sizeof(char), 3, fpsave);
}

void save_pc_data(pc_data *i, QDataStream &out, time_data tmpage)
{
  out.writeRawData(i->pwd, PASSWORD_LEN + 1);
  save_char_aliases(i->alias, out);

  fwrite_string_tilde(out);
  out.writeRawData((const char *)&(i->rdeaths), sizeof(i->rdeaths));
  out.writeRawData((const char *)&(i->pdeaths), sizeof(i->pdeaths));
  out.writeRawData((const char *)&(i->pkills), sizeof(i->pkills));
  out.writeRawData((const char *)&(i->pklvl), sizeof(i->pklvl));
  // we save tmpage cause it was calculated when all eq was off
  out.writeRawData((const char *)&(tmpage), sizeof(time_data));
  out.writeRawData((const char *)&(i->bad_pw_tries), sizeof(i->bad_pw_tries));
  out.writeRawData((const char *)&(i->practices), sizeof(i->practices));
  out.writeRawData((const char *)&(i->bank), sizeof(i->bank));
  out.writeRawData((const char *)&(i->toggles), sizeof(i->toggles));
  out.writeRawData((const char *)&(i->punish), sizeof(i->punish));
  fwrite_var_string(i->last_site, out);
  fwrite_var_string(i->poofin, out);
  fwrite_var_string(i->poofout, out);
  fwrite_var_string(i->prompt, out);
  fwrite_var_string("NewSaveType", out);

  // Quest bitvector one
  if (i->quest_bv1)
  {
    out.writeRawData("QS1", 3);
    out.writeRawData((const char *)&(i->quest_bv1), sizeof(i->quest_bv1));
  }

  // Saving throw mods
  out.writeRawData("SVM", 3);
  out.writeRawData((const char *)&(i->saves_mods), sizeof(i->saves_mods[0])*(SAVE_TYPE_MAX + 1)); // Write the whole array

  // Specializations
  out.writeRawData("SPC", 3);
  out.writeRawData((const char *)&(i->specializations), sizeof(i->specializations));

  // Stat metas
  if (i->statmetas)
  {
    out.writeRawData("STM", 3);
    out.writeRawData((const char *)&(i->statmetas), sizeof(i->statmetas));
  }

  // Ki metas
  if (i->kimetas)
  {
    out.writeRawData("KIM", 3);
    out.writeRawData((const char *)&(i->kimetas), sizeof(i->kimetas));
  }
  // autojoinin'
  if (i->joining)
  {
    out.writeRawData("JIN", 3);
    fwrite_var_string(i->joining, out);
  }

  out.writeRawData("QST", 3);
  out.writeRawData((const char *)&(i->quest_points), sizeof(i->quest_points));
  for (int j = 0; j < QUEST_CANCEL; j++)
  {
    out.writeRawData((const char *)&(i->quest_cancel[j]), sizeof(i->quest_cancel[j]));
  }
  for (int j = 0; j <= QUEST_TOTAL / ASIZE; j++)
  {
    out.writeRawData((const char *)&(i->quest_complete[j]), sizeof(i->quest_complete[j]));
  }
  if (i->buildLowVnum)
  {
    out.writeRawData("BLO", 3);
    out.writeRawData((const char *)&(i->buildLowVnum), sizeof(i->buildLowVnum));
  }
  if (i->buildHighVnum)
  {
    out.writeRawData("BHI", 3);
    out.writeRawData((const char *)&(i->buildHighVnum), sizeof(i->buildHighVnum));
  }
  if (i->buildMLowVnum)
  {
    out.writeRawData("BMO", 3);
    out.writeRawData((const char *)&(i->buildMLowVnum), sizeof(i->buildMLowVnum));
  }
  if (i->buildMHighVnum)
  {
    out.writeRawData("BMI", 3);
    out.writeRawData((const char *)&(i->buildMHighVnum), sizeof(i->buildMHighVnum));
  }
  if (i->buildOLowVnum)
  {
    out.writeRawData("BOO", 3);
    out.writeRawData((const char *)&(i->buildOLowVnum), sizeof(i->buildOLowVnum));
  }
  if (i->buildOHighVnum)
  {
    out.writeRawData("BOI", 3);
    out.writeRawData((const char *)&(i->buildOHighVnum), sizeof(i->buildOHighVnum));
  }
  if (i->profession)
  {
    out.writeRawData("PRO", 3);
    out.writeRawData((const char *)&(i->profession), sizeof(i->profession));
  }
  if (i->wizinvis)
  {
    out.writeRawData("WIZ", 3);
    out.writeRawData((const char *)&(i->wizinvis), sizeof(i->wizinvis));
  }
  // Any future additions to this save file will need to be placed LAST here with a 3 letter code
  // and appropriate strcmp statement in the read_mob_data object

  out.writeRawData("STP", 3);
}

void fread_to_tilde(FILE *fpsave)
{
  char a;
  while (TRUE)
  {
    fread(&a, 1, 1, fpsave);
    if (a == '~') break; 
  }
}

void read_pc_data(struct char_data *ch, FILE* fpsave)
{
  char typeflag[4] = { 0 };
  struct pc_data * i = ch->pcdata;

  i->golem = 0;
  i->quest_points = 0;
  for(int j=0;j<QUEST_CANCEL;j++)
	i->quest_cancel[j] = 0;
  for(int j=0;j<=QUEST_TOTAL/ASIZE;j++)
	i->quest_complete[j] = 0;

  fread(i->pwd,            sizeof(char),       PASSWORD_LEN+1, fpsave);
  i->alias = read_char_aliases(fpsave);
  if (has_skill(ch, NEW_SAVE))
     fread_to_tilde(fpsave);
  fread(&(i->rdeaths),     sizeof(i->rdeaths),  1, fpsave);
  fread(&(i->pdeaths),     sizeof(i->pdeaths),  1, fpsave);
  fread(&(i->pkills),      sizeof(i->pkills),   1, fpsave);
  fread(&(i->pklvl),       sizeof(i->pklvl),    1, fpsave);
  fread(&(i->time),        sizeof(time_data),   1, fpsave);
  fread(&(i->bad_pw_tries),sizeof(i->bad_pw_tries), 1, fpsave);
  fread(&(i->practices),   sizeof(i->practices), 1, fpsave);
  fread(&(i->bank),        sizeof(i->bank),     1, fpsave);
  fread(&(i->toggles),     sizeof(i->toggles),  1, fpsave);
  fread(&(i->punish),      sizeof(i->punish),   1, fpsave);
  i->last_site = fread_var_string(fpsave);
  i->poofin    = fread_var_string(fpsave);
  i->poofout   = fread_var_string(fpsave);
  i->prompt    = fread_var_string(fpsave);

  char *tmp = fread_var_string(fpsave);
  if (!tmp || str_cmp(tmp,"NewSaveType"))
  {
    tmp = fread_var_string(fpsave);
    tmp = fread_var_string(fpsave);
  }
  i->skillchange = 0;
  fread(&typeflag, sizeof(char), 3, fpsave);

  if(!strcmp("QS1", typeflag))
  {
    fread(&i->quest_bv1, sizeof(i->quest_bv1), 1, fpsave);
    fread(&typeflag, sizeof(char), 3, fpsave);
  }

  if(!strcmp("SVM", typeflag))
  {
    fread(&(i->saves_mods), sizeof(i->saves_mods[0]), SAVE_TYPE_MAX+1, fpsave); // read the whole array
    fread(&typeflag, sizeof(char), 3, fpsave);
  }

  if(!strcmp("SPC", typeflag))
  {
    fread(&(i->specializations), sizeof(i->specializations), 1, fpsave);
    fread(&typeflag, sizeof(char), 3, fpsave);
  }

  if(!strcmp("STM", typeflag))
  {
    fread(&i->statmetas, sizeof(i->statmetas), 1, fpsave);
    fread(&typeflag, sizeof(char), 3, fpsave);
  }

  if(!strcmp("KIM", typeflag))
  {
    fread(&i->kimetas, sizeof(i->kimetas), 1, fpsave);
    fread(&typeflag, sizeof(char), 3, fpsave);
  }
  i->joining = 0;
  if (!strcmp("JIN", typeflag))
  {
    i->joining = fread_var_string(fpsave);
    fread(&typeflag, sizeof(char), 3, fpsave);
  }
  if (!strcmp("QST", typeflag))
  {
    fread(&(i->quest_points), sizeof(i->quest_points), 1, fpsave);
    for(int j = 0;j<QUEST_CANCEL;j++)
      fread(&(i->quest_cancel[j]), sizeof(i->quest_cancel[j]), 1, fpsave);
    for(int j=0;j<=QUEST_TOTAL/ASIZE;j++)
      fread(&(i->quest_complete[j]), sizeof(i->quest_complete[j]), 1, fpsave);
   fread(&typeflag, sizeof(char), 3, fpsave);
  }
  if (!strcmp("BLO", typeflag))
  {
    fread(&i->buildLowVnum, sizeof(i->buildLowVnum), 1, fpsave);
    fread(&typeflag, sizeof(char), 3, fpsave);
  }
  if (!strcmp("BHI", typeflag))
  {
    fread(&i->buildHighVnum, sizeof(i->buildHighVnum), 1, fpsave);
    fread(&typeflag, sizeof(char), 3, fpsave);
  }
  if (!strcmp("BMO", typeflag))
  {
    fread(&i->buildMLowVnum, sizeof(i->buildMLowVnum), 1, fpsave);
    fread(&typeflag, sizeof(char), 3, fpsave);
  }
  if (!strcmp("BMI", typeflag))
  {
    fread(&i->buildMHighVnum, sizeof(i->buildMHighVnum), 1, fpsave);
    fread(&typeflag, sizeof(char), 3, fpsave);
  }
  if (!strcmp("BOO", typeflag))
  {
    fread(&i->buildOLowVnum, sizeof(i->buildOLowVnum), 1, fpsave);
    fread(&typeflag, sizeof(char), 3, fpsave);
  }
  if (!strcmp("BOI", typeflag))
  {
    fread(&i->buildOHighVnum, sizeof(i->buildOHighVnum), 1, fpsave);
    fread(&typeflag, sizeof(char), 3, fpsave);
  }
  if (!strcmp("PRO", typeflag))
  {
    fread(&i->profession, sizeof(i->profession), 1, fpsave);
    fread(&typeflag, sizeof(char), 3, fpsave);
  }
  if (!strcmp("WIZ", typeflag))
  {
	  fread(&i->wizinvis, sizeof(i->wizinvis), 1, fpsave);
	  fread(&typeflag, sizeof(char), 3, fpsave);
  }
  while (!strcmp("OPT", typeflag))
  {
    if (i->options == nullptr)
    {
      i->options = new map<string,string>();
    }

    string key = fread_var_string(fpsave);
    string value = fread_var_string(fpsave);
    if (key.empty() == false && (key == "color.good" || key == "color.bad"))
    {
      (*i->options)[key] = value;
    }    

    fread(&typeflag, sizeof(char), 3, fpsave);
  }

  i->skillchange = 0;
  // Add new items in this format
//  if(!strcmp(typeflag, "XXX"))
//    do_something

  // Any future additions to this read file will need to be placed LAST

  // at this point, typeflag should = "STP", and we're done reading mob data
}

int save_pc_or_mob_data(CHAR_DATA *ch, FILE * fpsave, struct time_data tmpage)
{
  if(IS_MOB(ch))
    save_mob_data(ch->mobdata, fpsave);
  else
    save_pc_data(ch->pcdata, fpsave, tmpage);

  return 1;
}

int save_pc_or_mob_data(CHAR_DATA *ch, QDataStream &out, struct time_data tmpage)
{
  if(IS_MOB(ch))
    save_mob_data(ch->mobdata, out);
  else
    save_pc_data(ch->pcdata, out, tmpage);

  return 1;
}

int read_pc_or_mob_data(CHAR_DATA *ch, FILE *fpsave)
{
  if(IS_MOB(ch)) {
    ch->pcdata = NULL;
    ch->mobdata = new mob_data;
    read_mob_data(ch->mobdata, fpsave);
  }
  else {
    ch->mobdata = nullptr;
    ch->pcdata = new pc_data;
    read_pc_data(ch, fpsave);
  }
  return 1;
}

// return 1 on success
// return 0 on failure
int store_worn_eq(char_data * ch, FILE * fpsave)
{
  int wear_pos = -1;
  int iWear = 0;

  for (iWear = 0; iWear < MAX_WEAR; iWear++) 
  {
    wear_pos = iWear;
    if (ch->equipment[iWear]) 
    {
      if (!obj_to_store( ch->equipment[iWear], ch, fpsave, wear_pos))
        return 0;
    }
  }
  return 1;
}

int store_worn_eq(char_data * ch, QDataStream &out)
{
  int wear_pos = -1;
  int iWear = 0;

  for (iWear = 0; iWear < MAX_WEAR; iWear++) 
  {
    wear_pos = iWear;
    if (ch->equipment[iWear]) 
    {
      if (!obj_to_store( ch->equipment[iWear], ch, out, wear_pos))
        return 0;
    }
  }
  return 1;
}

int char_to_store_variable_data(CHAR_DATA * ch, QDataStream &out)
{

  fwrite_var_string(ch->name, out);
  fwrite_var_string(ch->short_desc, out);
  fwrite_var_string(ch->long_desc, out);
  fwrite_var_string(ch->description, out);
  fwrite_var_string(ch->title, out);

  if (!has_skill(ch, NEW_SAVE)) // New save.
     learn_skill(ch, NEW_SAVE, 1, 100);    

  map<int16, char_skill_data> skills = ch->skills;
  queue<int16> order = ch->skillsSaveLoadOrder;
  queue<int16> neworder;

  map<int16_t,bool> seen;
  while (order.size() > 0)
  {
    int16 skillnum = order.front();
    char_skill_data skill = skills[skillnum];

    map<int16_t,bool>::iterator i = seen.find(skillnum);
    if (i == seen.end())
    {
      neworder.push(order.front());
      seen[skillnum] = true;
      qDebug() << "Saving skill " << skillnum << " " << skill.skillnum << " " << skill.learned;
      out.writeRawData("SKL", 3);
      out.writeRawData((const char *)&(skill.skillnum), sizeof(skill.skillnum));
      out.writeRawData((const char *)&(skill.learned), sizeof(skill.learned));
      out.writeRawData((const char *)&(skill.unused), sizeof(skill.unused));
    } else {
      qDebug() << "Not saving duplicate skill " << skillnum << " " << skill.skillnum << " " << skill.learned;
    }

    order.pop();    
  }
  out.writeRawData("END", 3);
  if (neworder != ch->skillsSaveLoadOrder)
  {
    qDebug() << "neworder different thant old order";
    ch->skillsSaveLoadOrder = neworder;
  } else {
    qDebug() << "No change to order";
  }


  struct affected_type *af;
  int16 aff_count = 0; // do not change from int16

  for(af = ch->affected; af; af = af->next)
    aff_count++;

  if(aff_count)
  {
    out.writeRawData("AFS", 3);
    out.writeRawData((const char *)&aff_count, sizeof(aff_count));
    for(af = ch->affected; af; af = af->next)
    {
       out.writeRawData((const char *)&(af->type),      sizeof(af->type));
       out.writeRawData((const char *)&(af->duration),  sizeof(af->duration));
       out.writeRawData((const char *)&(af->modifier),  sizeof(af->modifier));
       out.writeRawData((const char *)&(af->location),  sizeof(af->location));
       out.writeRawData((const char *)&(af->bitvector), sizeof(af->bitvector));
    }
  }

  struct tempvariable *mpv;
  for (mpv = ch->tempVariable;mpv;mpv = mpv->next)
  {
    if (!mpv->save) continue;
    out.writeRawData("MPV", 3);
    fwrite_var_string(mpv->name, out);
    fwrite_var_string(mpv->data, out);
  }

  out.writeRawData("GLD", 3);
  out.writeRawData((const char *)&ch->gold, sizeof(ch->gold));

  // Any future additions to this save file will need to be placed LAST here with a 3 letter code
  // and appropriate strcmp statement in the read_mob_data object

  out.writeRawData("STP", 3);

  return 1;
}

void read_skill(CHAR_DATA * ch, FILE * fpsave)
{
  char_skill_data curr;

  fread(&(curr.skillnum), sizeof(curr.skillnum), 1, fpsave);
  fread(&(curr.learned), sizeof(curr.learned), 1, fpsave);
  fread(&(curr.unused), sizeof(curr.unused[0]), 5, fpsave);

//  The above line takes care of these four.  They are here for future use
//  fread(&(curr->unused[1]), sizeof(curr->unused[1]), 1, fpsave);
//  fread(&(curr->unused[2]), sizeof(curr->unused[2]), 1, fpsave);
//  fread(&(curr->unused[3]), sizeof(curr->unused[3]), 1, fpsave);
//  fread(&(curr->unused[4]), sizeof(curr->unused[4]), 1, fpsave);

  ch->skills[curr.skillnum] = curr;
  ch->skillsSaveLoadOrder.push(curr.skillnum);
}

int store_to_char_variable_data(CHAR_DATA * ch, FILE * fpsave)
{
  char typeflag[4];
  memset(typeflag, 0, 4);

  ch->name = fread_var_string(fpsave);
  ch->short_desc = fread_var_string(fpsave);
  ch->long_desc = fread_var_string(fpsave);
  ch->description = fread_var_string(fpsave);
  ch->title = fread_var_string(fpsave);

  fread(&typeflag, sizeof(char), 3, fpsave);

  while(strcmp(typeflag, "END")) {
    read_skill(ch, fpsave);  
    fread(&typeflag, sizeof(char), 3, fpsave);
  }

  fread(&typeflag, sizeof(char), 3, fpsave);

  if(!strncmp(typeflag, "AFS", 3)) // affects
  {
    int16 aff_count; // do not change form int16
    fread(&aff_count, sizeof(aff_count), 1, fpsave);
    ch->affected = NULL;
    for(int16 i = 0; i < aff_count; i++)
    {
       affected_type * af = new (nothrow) affected_type;
       af->duration_type = 0;
       af->next = ch->affected;
       ch->affected = af;

       fread(&(af->type),      sizeof(af->type),      1, fpsave);
       fread(&(af->duration),  sizeof(af->duration),  1, fpsave);
       fread(&(af->modifier),  sizeof(af->modifier),  1, fpsave);
       fread(&(af->location),  sizeof(af->location),  1, fpsave);
       fread(&(af->bitvector), sizeof(af->bitvector), 1, fpsave);

       affect_modify(ch, af->location, af->modifier, af->bitvector, TRUE); // re-affect the char
    }
    fread(&typeflag, sizeof(char), 3, fpsave);
  }

  while (!strcmp(typeflag, "MPV"))
  {  // MobProgVars
     struct tempvariable *mpv;
     mpv = new tempvariable;
     mpv->name = fread_var_string(fpsave);
     mpv->data = fread_var_string(fpsave);
     mpv->save = 1;
     mpv->next = ch->tempVariable;
     ch->tempVariable = mpv;
     fread(&typeflag, sizeof(char), 3, fpsave);
  }
  if (!strcmp(typeflag, "GLD"))
  {
      fread(&(ch->gold), sizeof(ch->gold), 1, fpsave);
      fread(&typeflag, sizeof(char), 3, fpsave);
  }
  // Add new items in this format
//  if(!strcmp(typeflag, "XXX"))
//    do_something

  // Any future additions to this read file will need to be placed LAST

  // at this point, typeflag should = "STP", and we're done reading mob data

  return 1;
}


#ifdef USE_SQL
void save_char_obj_db(CHAR_DATA *ch)
{
  if (ch == 0)
    return;

  if(IS_NPC(ch) || GET_LEVEL(ch) < 2)
    return;

  // so weapons stop falling off
  SETBIT(ch->affected_by, AFF_IGNORE_WEAPON_WEIGHT); 
  
  char_file_u4 uchar;
  time_data tmpage;
  memset(&uchar, 0, sizeof(uchar));
  memset(&tmpage, 0, sizeof(tmpage));

  char_to_store(ch, &uchar, tmpage);

  // if they're in a safe room, save them there.
  // if they're a god, send 'em home
  // otherwise save them in tavern
  if(IS_SET(world[ch->in_room].room_flags, SAFE))
    uchar.load_room = world[ch->in_room].number;
  else
    uchar.load_room = real_room(GET_HOME(ch));

  timeval start, finish;

  gettimeofday(&start, NULL);
  db.save(ch, &uchar);
  gettimeofday(&finish, NULL);

  int msec = finish.tv_sec*1000 + finish.tv_usec/1000;
  msec -= start.tv_sec*1000 + start.tv_usec/1000;
  csendf(ch, "Save took %dms\n\r", msec);


  /*
  if((fwrite(&uchar, sizeof(uchar), 1, fpsave))               &&
     (char_to_store_variable_data(ch, fpsave))                &&
     (save_pc_or_mob_data(ch, fpsave, tmpage))                &&
     (obj_to_store (ch->carrying, ch, fpsave, -1))            &&
     (store_worn_eq(ch, fpsave))
    )
  {
    if(fpsave != NULL)
      dc_fclose(fpsave);
    sprintf(log_buf, "mv %s %s", strsave, name); 
    system(log_buf);
  }
  else
  {
    if(fpsave != NULL)
      dc_fclose(fpsave);
    sprintf(log_buf, "Save_char_obj: %s", strsave);
    send_to_char ("WARNING: file problem. You did not save!", ch);
    perror(log_buf);
    log(log_buf, ANGEL, LOG_BUG);
  }

  REMBIT(ch->affected_by, AFF_IGNORE_WEAPON_WEIGHT);
  struct vault_data *vault;
  if ((vault = has_vault(GET_NAME(ch))))
    save_vault(vault->owner);
  */

}
#endif

// save a character and inventory.
// maybe modify it to save mobs for quest purposes too
void save_char_obj (CHAR_DATA *ch)
{
  char_file_u4 uchar = char_file_u4();
  time_data tmpage = time_data();
  FILE * fpsave  = 0;
  char strsave[MAX_INPUT_LENGTH] = {0};
  char name[200] = {0};

  memset(&tmpage, 0, sizeof(tmpage));

  if(IS_NPC(ch) || GET_LEVEL(ch) < 1 || GET_NAME(ch) == nullptr)
  {
    return;
  }

  // TODO - figure out a way for mob's to save...maybe <mastername>.pet ?
  if (DC::instance().cf.bport)
  {
    sprintf (name, "%s/%c/%s", BSAVE_DIR, ch->name[0], ch->name);
  } else
  {
    sprintf (name, "%s/%c/%s", SAVE_DIR, ch->name[0], ch->name);
  }

  sprintf (strsave, "%s.back", name);

  QFile pFile(strsave);
  if (!(pFile.open(QIODeviceBase::ReadWrite))) {
    send_to_char("Warning!  Did not save.  Could not open file.  Contact a god, do not logoff.\n\r", ch);
    sprintf(log_buf, "Could not open file in save_char_obj. '%s'", strsave);
    perror(log_buf);
    log(log_buf, ANGEL, LOG_BUG);
    return;
  }
  QDataStream pFileOut(&pFile);
  
  SETBIT(ch->affected_by, AFF_IGNORE_WEAPON_WEIGHT); // so weapons stop falling off

  char_to_store (ch, &uchar, tmpage);

  // if they're in a safe room, save them there.
  // if they're a god, send 'em home
  // otherwise save them in tavern

  if (ch->in_room < 1)
  {
    uchar.load_room = START_ROOM;
  }
  else
  {
    if(IS_SET(world[ch->in_room].room_flags, SAFE))
      uchar.load_room = world[ch->in_room].number;
    else
      uchar.load_room = real_room(GET_HOME(ch));
  }

  pFileOut.writeRawData((const char *)&uchar.sex,sizeof(uchar.sex));
  pFileOut.writeRawData((const char *)&uchar.c_class, sizeof(uchar.c_class));
  pFileOut.writeRawData((const char *)&uchar.race, sizeof(uchar.race));
  pFileOut.writeRawData((const char *)&uchar.level, sizeof(uchar.level));
  pFileOut.writeRawData((const char *)&uchar.raw_str, sizeof(uchar.raw_str));
  pFileOut.writeRawData((const char *)&uchar.raw_intel, sizeof(uchar.raw_intel));
  pFileOut.writeRawData((const char *)&uchar.raw_wis, sizeof(uchar.raw_wis));
  pFileOut.writeRawData((const char *)&uchar.raw_dex, sizeof(uchar.raw_dex));

  pFileOut.writeRawData((const char *)&uchar.raw_con, sizeof(uchar.raw_con));
  pFileOut.writeRawData((const char *)&uchar.conditions, 3*sizeof(uchar.conditions[0]));

  pFileOut.writeRawData((const char *)&uchar.weight, sizeof(uchar.weight));
  pFileOut.writeRawData((const char *)&uchar.height, sizeof(uchar.height));
  pFileOut.writeRawData((const char *)&uchar.hometown, sizeof(uchar.hometown));

  pFileOut.writeRawData((const char *)&uchar.gold, sizeof(uchar.gold));
  pFileOut.writeRawData((const char *)&uchar.plat, sizeof(uchar.plat));
  pFileOut.writeRawData((const char *)&uchar.exp, sizeof(uchar.exp));
  pFileOut.writeRawData((const char *)&uchar.immune, sizeof(uchar.immune));
  pFileOut.writeRawData((const char *)&uchar.resist, sizeof(uchar.resist));
  pFileOut.writeRawData((const char *)&uchar.suscept, sizeof(uchar.suscept));

  pFileOut.writeRawData((const char *)&uchar.mana, sizeof(uchar.mana));
  pFileOut.writeRawData((const char *)&uchar.raw_mana, sizeof(uchar.raw_mana));
  pFileOut.writeRawData((const char *)&uchar.hit, sizeof(uchar.hit));
  pFileOut.writeRawData((const char *)&uchar.raw_hit, sizeof(uchar.raw_hit));
  pFileOut.writeRawData((const char *)&uchar.move, sizeof(uchar.move));
  pFileOut.writeRawData((const char *)&uchar.raw_move, sizeof(uchar.raw_move));
  pFileOut.writeRawData((const char *)&uchar.ki, sizeof(uchar.ki));
  pFileOut.writeRawData((const char *)&uchar.raw_ki, sizeof(uchar.raw_ki));

  pFileOut.writeRawData((const char *)&uchar.alignment, sizeof(uchar.alignment));
  pFileOut.writeRawData((const char *)&uchar.unused1, sizeof(uchar.unused1));

  pFileOut.writeRawData((const char *)&uchar.hpmetas, sizeof(uchar.hpmetas));
  pFileOut.writeRawData((const char *)&uchar.manametas, sizeof(uchar.manametas));
  pFileOut.writeRawData((const char *)&uchar.movemetas, sizeof(uchar.movemetas));

  pFileOut.writeRawData((const char *)&uchar.armor, sizeof(uchar.armor));
  pFileOut.writeRawData((const char *)&uchar.hitroll, sizeof(uchar.hitroll));

  pFileOut.writeRawData((const char *)&uchar.damroll, sizeof(uchar.damroll));
  pFileOut.writeRawData((const char *)&uchar.unused2, sizeof(uchar.unused2));

  pFileOut.writeRawData((const char *)&uchar.afected_by, sizeof(uchar.afected_by));
  pFileOut.writeRawData((const char *)&uchar.afected_by2, sizeof(uchar.afected_by2));
  pFileOut.writeRawData((const char *)&uchar.misc, sizeof(uchar.misc));

  pFileOut.writeRawData((const char *)&uchar.clan, sizeof(uchar.clan));
  pFileOut.writeRawData((const char *)&uchar.unused3, sizeof(uchar.unused3));
  pFileOut.writeRawData((const char *)&uchar.load_room, sizeof(uchar.load_room));

  pFileOut.writeRawData((const char *)&uchar.acmetas, sizeof(uchar.acmetas));
  pFileOut.writeRawData((const char *)&uchar.agemetas, sizeof(uchar.agemetas));
  pFileOut.writeRawData((const char *)&uchar.extra_ints, 3*sizeof(uchar.extra_ints[0]));

  bool vardataWrote = char_to_store_variable_data(ch, pFileOut);
  bool pcdataWrote = save_pc_or_mob_data(ch, pFileOut, tmpage);
  bool objdataWrote = obj_to_store(ch->carrying, ch, pFileOut, -1);
  bool eqWrote = store_worn_eq(ch, pFileOut);

  if (vardataWrote && pcdataWrote && objdataWrote && eqWrote)
  {
    pFile.close();
    sprintf(log_buf, "mv %s %s", strsave, name);
    system(log_buf);
  }
  else
  {
    pFile.close();
    sprintf(log_buf, "Save_char_obj: %s", strsave);
    send_to_char("WARNING: file problem. You did not save!", ch);
    perror(log_buf);
    log(log_buf, ANGEL, LOG_BUG);
  }

  REMBIT(ch->affected_by, AFF_IGNORE_WEAPON_WEIGHT);
  struct vault_data *vault;
  if ((vault = has_vault(GET_NAME(ch))))
    save_vault(vault->owner);
}

// just error crap to avoid using "goto" like we were
void load_char_obj_error(FILE * fpsave, char strsave[MAX_INPUT_LENGTH])
{
  sprintf(log_buf, "Load_char_obj: %s", strsave);
  perror(log_buf);
  log(log_buf, ANGEL, LOG_BUG);
  if(fpsave != NULL)
    dc_fclose(fpsave);
}

// Load a char and inventory into a new_new ch structure.
bool load_char_obj( struct descriptor_data *d, const char *name )
{
  char_file_u4 uchar;
  char strsave[MAX_INPUT_LENGTH] = { 0 };
  FILE *fpsave  = nullptr;   
  obj_data *last_cont = nullptr;
  char_data *ch = nullptr;

  if(!name || !strcmp(name, ""))
    return FALSE;

  ch = new char_data;

	if (d->character) {
		delete d->character;
	}

  d->character    = ch;
  clear_char(ch);
  ch->desc        = d;

  if (DC::instance().cf.bport)
  {
    sprintf(strsave, "%s/%c/%s", BSAVE_DIR, UPPER(name[0]), name);
  } else
  {
    sprintf(strsave, "%s/%c/%s", SAVE_DIR, UPPER(name[0]), name);
  }

//  struct stat mystats;
//  stat(strsave, &mystats);
//  TODO - Eventually, i'm going to just slurp in the whole file
//  then parse the memory instead of reading each item from file seperately
//  Should be much faster and save our HD from turning itself to mush -pir

  if ((fpsave = dc_fopen(strsave, "rb" )) == NULL)
    return FALSE;

  if (fread(&uchar, 1, 140, fpsave) != 140)
  {
    load_char_obj_error(fpsave, strsave);
    return FALSE;
  }

  reset_char(ch);

  store_to_char(&uchar, ch);
  store_to_char_variable_data(ch, fpsave);
  read_pc_or_mob_data(ch, fpsave);
 if (!IS_NPC(ch) && ch->pcdata->time.logon < 1117527906)
  {
    extern int do_clearaff(struct char_data *ch, char *argument, int cmd);
    do_clearaff(ch,"",9);
    ch->affected_by[0] = ch->affected_by[1] = 0;
  }
  
  // stored names only matter for mobs
  if(!IS_MOB(ch)) {
    delete[] GET_NAME(ch);
    GET_NAME(ch) = str_dup(name);
  }
  
  while(!feof(fpsave)) {
    last_cont = obj_store_to_char( ch, fpsave, last_cont );
  }

  if(fpsave != NULL)
    dc_fclose(fpsave);
  return TRUE;
}

// read data from file for an item.
struct obj_data *  obj_store_to_char(CHAR_DATA *ch, FILE *fpsave, struct obj_data * last_cont )
{
  struct obj_data *obj;
//  struct extra_descr_data *new_new_descr;
//  struct extra_descr_data *ed, *next_ed;

  int j;
  int nr;
  uint16 length;  // do not change this type
  int wear_pos;
  char mod_type[4];
  char buf[MAX_STRING_LENGTH];

  // read in the standard file data
  struct obj_file_elem object;
  fread(&object, sizeof(object), 1, fpsave);

  if(feof(fpsave))
    return NULL;

  // if it's a current object, clone it and continue
  // if it's not, then we need to remove it from the pfile so clone obj 1

  if ( ( nr = real_object(object.item_number) ) > -1 ) 
    obj = clone_object(nr);
  else obj = clone_object(1);

  obj->obj_flags.timer       = object.timer;
  wear_pos                   = object.wear_pos;

  // begin sequence find any modifications to the item the person has
  // what happens, is the mods are written in a particular order to the pfile
  // so I only need to go through this once instead of looping through for each
  // one each time.  If we later decide to want to add something else, we just 
  // put it at the end of the sequence and all is good.  We keep reading until
  // we hit a STP flag.  If we aren't on STP by the end of the sequence, then
  // something very bad has happened. -pir
  mod_type[3] = 0;
  fread(&mod_type, sizeof(char), 3, fpsave);

  if(!strcmp("EQL", mod_type))
  {
    fread(&obj->obj_flags.eq_level, sizeof(obj->obj_flags.eq_level), 1, fpsave);
    fread(&mod_type, sizeof(char), 3, fpsave);
  }
  if(!strcmp("VA0", mod_type))
  {
    fread(&obj->obj_flags.value[0], sizeof(obj->obj_flags.value[0]), 1, fpsave);
    fread(&mod_type, sizeof(char), 3, fpsave);
  }
  if(!strcmp("VA1", mod_type))
  {
    fread(&obj->obj_flags.value[1], sizeof(obj->obj_flags.value[1]), 1, fpsave);
    fread(&mod_type, sizeof(char), 3, fpsave);
  }
  if(!strcmp("VA2", mod_type))
  {
    fread(&obj->obj_flags.value[2], sizeof(obj->obj_flags.value[2]), 1, fpsave);
    fread(&mod_type, sizeof(char), 3, fpsave);
  }
  if(!strcmp("VA3", mod_type))
  {
    fread(&obj->obj_flags.value[3], sizeof(obj->obj_flags.value[3]), 1, fpsave);
    fread(&mod_type, sizeof(char), 3, fpsave);
  }
  if(!strcmp("EXF", mod_type))
  {
    fread(&obj->obj_flags.extra_flags, sizeof(obj->obj_flags.extra_flags), 1, fpsave);
    fread(&mod_type, sizeof(char), 3, fpsave);
  }
  if(!strcmp("MOF", mod_type))
  {
    fread(&obj->obj_flags.more_flags, sizeof(obj->obj_flags.more_flags), 1, fpsave);
    fread(&mod_type, sizeof(char), 3, fpsave);
  }
  if(!strcmp("TYF", mod_type))
  {
    fread(&obj->obj_flags.type_flag, sizeof(obj->obj_flags.type_flag), 1, fpsave);
    fread(&mod_type, sizeof(char), 3, fpsave);
  }
  if(!strcmp("WEA", mod_type))
  {
    fread(&obj->obj_flags.wear_flags, sizeof(obj->obj_flags.wear_flags), 1, fpsave);
    fread(&mod_type, sizeof(char), 3, fpsave);
  }
  if(!strcmp("SZE", mod_type))
  {
    fread(&obj->obj_flags.size, sizeof(obj->obj_flags.size), 1, fpsave);
    fread(&mod_type, sizeof(char), 3, fpsave);
  }
  if(!strcmp("WEI", mod_type))
  {
    fread(&obj->obj_flags.weight, sizeof(obj->obj_flags.weight), 1, fpsave);
    fread(&mod_type, sizeof(char), 3, fpsave);
  }
  if(!strcmp("AFF", mod_type))
  {
    fread(&obj->num_affects, sizeof(obj->num_affects), 1, fpsave);
    if(obj->affected)
      dc_free(obj->affected);
    obj->affected = new obj_affected_type[obj->num_affects];

    for(j = 0; j < obj->num_affects; j++)
    {
      fread(&obj->affected[j].location, sizeof(obj->affected[j].location), 1, fpsave);
      fread(&obj->affected[j].modifier, sizeof(obj->affected[j].modifier), 1, fpsave);
    }

    fread(&mod_type, sizeof(char), 3, fpsave);
  }
  if (!strcmp("RPR",mod_type))
  {
    struct obj_affected_type *a;
    a = new obj_affected_type[obj->num_affects+1];
    int i;
    for (i = 0; i < obj->num_affects;i++)
    {
        a[i].location = obj->affected[i].location;
        a[i].modifier = obj->affected[i].modifier;
    }
    if(obj->affected)
      dc_free(obj->affected);
    a[i].location = APPLY_DAMAGED;
    fread(&a[i].modifier, sizeof(a[i].modifier), 1, fpsave);
    obj->affected = a;
    obj->num_affects++;
    fread(&mod_type, sizeof(char), 3, fpsave);
  }
  if(!strcmp("NAM", mod_type))
  {
    fread(&length, sizeof(length), 1, fpsave);
    fread(&buf, sizeof(char), length, fpsave);
    buf[length] = '\0';
    obj->name = str_hsh(buf);
    fread(&mod_type, sizeof(char), 3, fpsave);
  }
  if(!strcmp("DES", mod_type))
  {
    fread(&length, sizeof(length), 1, fpsave);
    fread(&buf, sizeof(char), length, fpsave);
    buf[length] = '\0';
    obj->description = str_hsh(buf);
    fread(&mod_type, sizeof(char), 3, fpsave);
  }
  if(!strcmp("SDE", mod_type))
  {
    fread(&length, sizeof(length), 1, fpsave);
    fread(&buf, sizeof(char), length, fpsave);
    buf[length] = '\0';
    obj->short_description = str_hsh(buf);
    fread(&mod_type, sizeof(char), 3, fpsave);
  }
  if(!strcmp("ADE", mod_type))
  {
    fread(&length, sizeof(length), 1, fpsave);
    fread(&buf, sizeof(char), length, fpsave);
    buf[length] = '\0';
    obj->action_description = str_hsh(buf);
    fread(&mod_type, sizeof(char), 3, fpsave);
  }
  if(!strcmp("COS", mod_type))
  {
    fread(&obj->obj_flags.cost, sizeof(obj->obj_flags.cost), 1, fpsave);
    fread(&mod_type, sizeof(char), 3, fpsave);
  }
  if(!strcmp("SAV", mod_type))
  {
    fread(&obj->save_expiration, sizeof(time_t), 1, fpsave);
    fread(&mod_type, sizeof(char), 3, fpsave);
  }


  // TODO - put extra desc support here
  // NEW READS GO HERE

  if(nr == -1) {
    extract_obj(obj);
    return last_cont;
  }
  // Handle worn EQ
  if ( (wear_pos > -1) && (wear_pos < MAX_WEAR) && (!ch->equipment[wear_pos])
	&& CAN_WEAR(obj, wear_corr[wear_pos]))
  {
    equip_char (ch, obj, wear_pos, 1);
    return obj;
  }
  else if ( (wear_pos > -1) && (wear_pos < MAX_WEAR) && (!ch->equipment[wear_pos+1])
      && CAN_WEAR(obj, wear_corr[wear_pos+1]))
  {
     equip_char(ch, obj, wear_pos+1, 1);
	return obj;
  }
  else if(object.container_depth == 1 && last_cont)
  {
    // put the eq in a container
    // this code does not currently support containers in containers
    if(ARE_CONTAINERS(last_cont))
    {
      obj_to_obj(obj, last_cont);
      // we don't add weight to the character for containers that are worn
      if(!last_cont->equipped_by && obj_index[last_cont->item_number].virt != 536)
          IS_CARRYING_W(ch) += GET_OBJ_WEIGHT(obj);
    }
    else {
      obj_to_char(obj, ch); // just in case
      return last_cont;
    }
  }
  // screw it, just put it in their inventory
  else {
    obj_to_char(obj, ch);
    if (wear_pos > -1 && wear_pos < MAX_WEAR)
    {
	SETBIT(ch->affected_by, AFF_ITEM_REMOVE);
    }
   return obj;
  }


  return last_cont;
}



bool obj_to_store (struct obj_data *obj, CHAR_DATA *ch, FILE *fpsave, int wear_pos)
{
 // struct obj_data *tmp;

  if (obj == NULL)
    return TRUE;

  // recurse down next item in list
  if (!obj_to_store (obj->next_content, ch, fpsave, -1))
    return FALSE;

  // store myself
  if (!put_obj_in_store (obj, ch, fpsave, wear_pos ))
    return FALSE;

  // store anything IN myself.  That way they get put back in on read
  if (!obj_to_store (obj->contains, ch, fpsave, -1))
    return FALSE;

  return TRUE;
}

bool obj_to_store (struct obj_data *obj, CHAR_DATA *ch, QDataStream &out, int wear_pos)
{
 // struct obj_data *tmp;

  if (obj == NULL)
    return TRUE;

  // recurse down next item in list
  if (!obj_to_store (obj->next_content, ch, out, -1))
    return FALSE;

  // store myself
  if (!put_obj_in_store (obj, ch, out, wear_pos ))
    return FALSE;

  // store anything IN myself.  That way they get put back in on read
  if (!obj_to_store (obj->contains, ch, out, -1))
    return FALSE;

  return TRUE;
}


// return true on success
// return false on error
// write one object to file
bool put_obj_in_store (struct obj_data *obj, CHAR_DATA *ch, FILE *fpsave, int wear_pos)
{
  obj_file_elem object;
  obj_data *standard_obj = 0;
  uint16 length = 0;  // do not change this type

  memset(&object, 0, sizeof(object));

  if (GET_ITEM_TYPE(obj) == ITEM_NOTE)
    return TRUE;

  if(IS_SET(obj->obj_flags.extra_flags, ITEM_NOSAVE))
    return TRUE;

  if (IS_SET(obj->obj_flags.more_flags, ITEM_24H_SAVE)) {
	  // First time we try to save this object we set the
	  // expiration to 24 hours from this point
	  if (obj->save_expiration == 0) {
		  obj->save_expiration = time(NULL) + (60 * 60 * 24);
	  } else if (time(NULL) > obj->save_expiration){
		  // If the object's window for saving has expired then
		  // we don't save it as-if it had ITEM_NOSAVE
		  return TRUE;
	  }
    }

  if (obj->item_number < 0)
    return TRUE;

  // Set up items saved for all items
  object.version      = CURRENT_OBJ_VERSION;
  object.item_number  = obj_index[obj->item_number].virt;
  object.timer        = obj->obj_flags.timer;
  object.wear_pos     = wear_pos;
  if(obj->in_obj) // I'm in a container
     object.container_depth = 1;
  else object.container_depth = 0;

  // write basic item format to file
  if(!(fwrite(&object, sizeof(object), 1, fpsave )))
    return FALSE;

  // get a pointer to the standard version of this item
  standard_obj = ((struct obj_data *)obj_index[obj->item_number].item);

  // Begin checking if this item has been modified in any way from the standard
  // If it has, we need to save that particular modification to the file
  // THESE MUST REMAIN IN PROPER ORDER
  // IF YOU HAVE ANYMORE TO ADD, ADD THEM BEFORE THE "STP" FLAG AT END
/*  if(obj->obj_flags.eq_level    != standard_obj->obj_flags.eq_level) 
  {
    fwrite("EQL", sizeof(char), 3, fpsave);
    fwrite(&obj->obj_flags.eq_level, sizeof(obj->obj_flags.eq_level), 1, fpsave);
  }
  if(obj->obj_flags.value[0]    != standard_obj->obj_flags.value[0])
  {
    fwrite("VA0", sizeof(char), 3, fpsave);
    fwrite(&obj->obj_flags.value[0], sizeof(obj->obj_flags.value[0]), 1, fpsave);
  }*/

  if (IS_SET(obj->obj_flags.more_flags, ITEM_CUSTOM)
		  && obj->obj_flags.value[0] != standard_obj->obj_flags.value[0])
  {
    fwrite("VA0", sizeof(char), 3, fpsave);
    fwrite(&obj->obj_flags.value[0], sizeof(obj->obj_flags.value[0]), 1, fpsave);
  }

  if ((obj->obj_flags.type_flag == ITEM_CONTAINER || obj->obj_flags.type_flag == ITEM_DRINKCON || IS_SET(obj->obj_flags.more_flags, ITEM_CUSTOM))
		  && obj->obj_flags.value[1] != standard_obj->obj_flags.value[1])
  {
    fwrite("VA1", sizeof(char), 3, fpsave);
    fwrite(&obj->obj_flags.value[1], sizeof(obj->obj_flags.value[1]), 1, fpsave);
  }

  if ((obj->obj_flags.type_flag == ITEM_DRINKCON || obj->obj_flags.type_flag == ITEM_STAFF || obj->obj_flags.type_flag == ITEM_WAND || IS_SET(obj->obj_flags.more_flags, ITEM_CUSTOM))
		  && obj->obj_flags.value[2] != standard_obj->obj_flags.value[2])
  {
    fwrite("VA2", sizeof(char), 3, fpsave);
    fwrite(&obj->obj_flags.value[2], sizeof(obj->obj_flags.value[2]), 1, fpsave);
  }

  if (IS_SET(obj->obj_flags.more_flags, ITEM_CUSTOM)
		  && obj->obj_flags.value[3] != standard_obj->obj_flags.value[3])
  {
	  fwrite("VA3", sizeof(char), 3, fpsave);
	  fwrite(&obj->obj_flags.value[3], sizeof(obj->obj_flags.value[3]), 1, fpsave);
  }

  if(obj->obj_flags.extra_flags != standard_obj->obj_flags.extra_flags)
  {
    fwrite("EXF", sizeof(char), 3, fpsave);
    fwrite(&obj->obj_flags.extra_flags, sizeof(obj->obj_flags.extra_flags), 1, fpsave);
  }

  if (IS_SET(obj->obj_flags.more_flags, ITEM_CUSTOM)
		  && obj->obj_flags.more_flags != standard_obj->obj_flags.more_flags)
  {
    fwrite("MOF", sizeof(char), 3, fpsave);
    fwrite(&obj->obj_flags.more_flags, sizeof(obj->obj_flags.more_flags), 1, fpsave);   
  }


/*
  if(obj->obj_flags.more_flags != standard_obj->obj_flags.more_flags)
  {
    fwrite("MOF", sizeof(char), 3, fpsave);
    fwrite(&obj->obj_flags.more_flags, sizeof(obj->obj_flags.more_flags), 1, fpsave);
  }
  if(obj->obj_flags.type_flag != standard_obj->obj_flags.type_flag)
  {
    fwrite("TYF", sizeof(char), 3, fpsave);
    fwrite(&obj->obj_flags.type_flag, sizeof(obj->obj_flags.type_flag), 1, fpsave);
  }
  if(obj->obj_flags.wear_flags != standard_obj->obj_flags.wear_flags)
  {
    fwrite("WEA", sizeof(char), 3, fpsave);
    fwrite(&obj->obj_flags.wear_flags, sizeof(obj->obj_flags.wear_flags), 1, fpsave);
  }
  if(obj->obj_flags.size != standard_obj->obj_flags.size)
  {
    fwrite("SZE", sizeof(char), 3, fpsave);
    fwrite(&obj->obj_flags.size, sizeof(obj->obj_flags.size), 1, fpsave);
  }

  if(obj->obj_flags.weight != standard_obj->obj_flags.weight)
    {
      fwrite("WEI", sizeof(char), 3, fpsave);
      fwrite(&obj->obj_flags.weight, sizeof(obj->obj_flags.weight), 1, fpsave);
    }


  tmp_weight = obj->obj_flags.weight;
  if(GET_ITEM_TYPE(obj) == ITEM_CONTAINER && (loop_obj = obj->contains)
	&& obj_index[obj->item->number].virt != 536)
    for (; loop_obj; loop_obj = loop_obj->next_content)
      tmp_weight -= GET_OBJ_WEIGHT(loop_obj);
  if(tmp_weight      != standard_obj->obj_flags.weight)
  {
    fwrite("WEI", sizeof(char), 3, fpsave);
    fwrite(&tmp_weight, sizeof(tmp_weight), 1, fpsave);
  }
  change = (obj->num_affects != standard_obj->num_affects);
  // since they aren't always in the same order (builder might have swapped them in an
  // rsave or something) we have to search through for each one to see if they are there,
  // just in a different spot
  for (iAffect = 0; (iAffect < obj->num_affects) && !change; iAffect++) 
  {
    // set it to changed, and if we find it, set it back to unchanged, then continue prior loop
    change = 1;
    for(iAff2 = 0; (iAff2 < obj->num_affects) && change; iAff2++)
      if( (obj->affected[iAffect].location == standard_obj->affected[iAff2].location) ||
          (obj->affected[iAffect].modifier == standard_obj->affected[iAff2].modifier))
        change = 0;
  }
  */
  // Custom objects get all of their affects copied
  if (IS_SET(obj->obj_flags.more_flags, ITEM_CUSTOM)) {
	  fwrite("AFF", sizeof(char), 3, fpsave);
	  fwrite(&obj->num_affects, sizeof(obj->num_affects), 1, fpsave);
	  for (int iAffect = 0; iAffect < obj->num_affects; iAffect++)
	  {
		fwrite(&obj->affected[iAffect].location, sizeof(obj->affected[iAffect].location), 1, fpsave);
		fwrite(&obj->affected[iAffect].modifier, sizeof(obj->affected[iAffect].modifier), 1, fpsave);
	  }
  } else { // non-custom objects only get the damaged affect copied by way of RPR
	  int i;
	  for (i = 0; i < obj->num_affects; i++)
	  {
		if (obj->affected[i].location == APPLY_DAMAGED)
		{
		  fwrite("RPR", sizeof(char), 3, fpsave);
		  fwrite(&obj->affected[i].modifier,sizeof(obj->affected[i].modifier),1,fpsave);
		  break; // Fixed!
		}
	  }
  }

  if(strcmp(obj->name, standard_obj->name))
  {
    fwrite("NAM", sizeof(char), 3, fpsave);
    length = strlen(obj->name);
    fwrite(&length, sizeof(length), 1, fpsave);
    fwrite(obj->name, sizeof(char), length, fpsave);
  }
  if(strcmp(obj->description, standard_obj->description))
  {
    fwrite("DES", sizeof(char), 3, fpsave);
    length = strlen(obj->description);
    fwrite(&length, sizeof(length), 1, fpsave);
    fwrite(obj->description, sizeof(char), length, fpsave);
  }
  if(strcmp(obj->short_description, standard_obj->short_description))
  {
    fwrite("SDE", sizeof(char), 3, fpsave);
    length = strlen(obj->short_description);
    fwrite(&length, sizeof(length), 1, fpsave);
    fwrite(obj->short_description, sizeof(char), length, fpsave);
  }
  if(strcmp(obj->action_description, standard_obj->action_description))
  {
    fwrite("ADE", sizeof(char), 3, fpsave);
    length = strlen(obj->action_description);
    fwrite(&length, sizeof(length), 1, fpsave);
    fwrite(obj->action_description, sizeof(char), length, fpsave);
  }

  if(obj->obj_flags.cost != standard_obj->obj_flags.cost)
  {
    fwrite("COS", sizeof(char), 3, fpsave);
    fwrite(&obj->obj_flags.cost, sizeof(obj->obj_flags.cost), 1, fpsave);
  }

  if (IS_SET(obj->obj_flags.more_flags, ITEM_24H_SAVE)) {
	  fwrite("SAV", sizeof(char), 3, fpsave);
	  fwrite(&obj->save_expiration, sizeof(time_t), 1, fpsave);
  }

  // extra descs are a little strange...it's a pointer to a list of them
  // I don't really want to handle this right now, so I'm going to just ignore them now
  // TODO - figure out a way to save extra descs later.  I'll just make them impossible
  // to restring for now

  // THIS IS WHERE YOU SHOULD PUT ANY ADDITIONS TO THE OBJ PFILE THAT NEED TO BE SAVED
  // A CORRESPONDING ENTRY SHOULD BE MADE IN THE READ FUNCTION
  // MAKE SURE YOUR FLAG ISN'T ALREADY USED

  // Stop flag.  This means we are done with this object on the read
  fwrite("STP", sizeof(char), 3, fpsave);

  return TRUE;
}

// return true on success
// return false on error
// write one object to file
bool put_obj_in_store (struct obj_data *obj, CHAR_DATA *ch, QDataStream &out, int wear_pos)
{
  obj_file_elem object;
  obj_data *standard_obj = 0;
  uint16 length = 0;  // do not change this type

  memset(&object, 0, sizeof(object));

  if (GET_ITEM_TYPE(obj) == ITEM_KEY)
    return TRUE;

  if (GET_ITEM_TYPE(obj) == ITEM_NOTE)
    return TRUE;

  if(IS_SET(obj->obj_flags.extra_flags, ITEM_NOSAVE))
    return TRUE;

  if (IS_SET(obj->obj_flags.more_flags, ITEM_24H_SAVE)) {
	  // First time we try to save this object we set the
	  // expiration to 24 hours from this point
	  if (obj->save_expiration == 0) {
		  obj->save_expiration = time(NULL) + (60 * 60 * 24);
	  } else if (time(NULL) > obj->save_expiration){
		  // If the object's window for saving has expired then
		  // we don't save it as-if it had ITEM_NOSAVE
		  return TRUE;
	  }
    }

  if (obj->item_number < 0)
    return TRUE;

  // Set up items saved for all items
  object.version      = CURRENT_OBJ_VERSION;
  object.item_number  = obj_index[obj->item_number].virt;
  object.timer        = obj->obj_flags.timer;
  object.wear_pos     = wear_pos;
  if(obj->in_obj) // I'm in a container
     object.container_depth = 1;
  else object.container_depth = 0;

  // write basic item format to file
  if(!(out.writeRawData((const char *)&object, sizeof(object))));
    return FALSE;

  // get a pointer to the standard version of this item
  standard_obj = ((struct obj_data *)obj_index[obj->item_number].item);

  // Begin checking if this item has been modified in any way from the standard
  // If it has, we need to save that particular modification to the file
  // THESE MUST REMAIN IN PROPER ORDER
  // IF YOU HAVE ANYMORE TO ADD, ADD THEM BEFORE THE "STP" FLAG AT END

  if (IS_SET(obj->obj_flags.more_flags, ITEM_CUSTOM)
		  && obj->obj_flags.value[0] != standard_obj->obj_flags.value[0])
  {
    out.writeRawData("VA0", 3);
    out.writeRawData((const char *)&obj->obj_flags.value[0], sizeof(obj->obj_flags.value[0]));
  }

  if ((obj->obj_flags.type_flag == ITEM_CONTAINER || obj->obj_flags.type_flag == ITEM_DRINKCON || IS_SET(obj->obj_flags.more_flags, ITEM_CUSTOM))
		  && obj->obj_flags.value[1] != standard_obj->obj_flags.value[1])
  {
    out.writeRawData("VA1", 3);
    out.writeRawData((const char *)&obj->obj_flags.value[1], sizeof(obj->obj_flags.value[1]));
  }

  if ((obj->obj_flags.type_flag == ITEM_DRINKCON || obj->obj_flags.type_flag == ITEM_STAFF || obj->obj_flags.type_flag == ITEM_WAND || IS_SET(obj->obj_flags.more_flags, ITEM_CUSTOM))
		  && obj->obj_flags.value[2] != standard_obj->obj_flags.value[2])
  {
    out.writeRawData("VA2", 3);
    out.writeRawData((const char *)&obj->obj_flags.value[2], sizeof(obj->obj_flags.value[2]));
  }

  if (IS_SET(obj->obj_flags.more_flags, ITEM_CUSTOM)
		  && obj->obj_flags.value[3] != standard_obj->obj_flags.value[3])
  {
	  out.writeRawData("VA3", 3);
	  out.writeRawData((const char *)&obj->obj_flags.value[3], sizeof(obj->obj_flags.value[3]));
  }

  if(obj->obj_flags.extra_flags != standard_obj->obj_flags.extra_flags)
  {
    out.writeRawData("EXF", 3);
    out.writeRawData((const char *)&obj->obj_flags.extra_flags, sizeof(obj->obj_flags.extra_flags));
  }

  if (IS_SET(obj->obj_flags.more_flags, ITEM_CUSTOM)
		  && obj->obj_flags.more_flags != standard_obj->obj_flags.more_flags)
  {
    out.writeRawData("MOF", 3);
    out.writeRawData((const char *)&obj->obj_flags.more_flags, sizeof(obj->obj_flags.more_flags));   
  }

  // Custom objects get all of their affects copied
  if (IS_SET(obj->obj_flags.more_flags, ITEM_CUSTOM)) {
	  out.writeRawData("AFF", 3);
	  out.writeRawData((const char *)&obj->num_affects, sizeof(obj->num_affects));
	  for (int iAffect = 0; iAffect < obj->num_affects; iAffect++)
	  {
		out.writeRawData((const char *)&obj->affected[iAffect].location, sizeof(obj->affected[iAffect].location));
		out.writeRawData((const char *)&obj->affected[iAffect].modifier, sizeof(obj->affected[iAffect].modifier));
	  }
  } else { // non-custom objects only get the damaged affect copied by way of RPR
	  int i;
    for (i = 0; i < obj->num_affects; i++)
    {
      if (obj->affected[i].location == APPLY_DAMAGED)
      {
        out.writeRawData("RPR", 3);
        out.writeRawData((const char *)&obj->affected[i].modifier, sizeof(obj->affected[i].modifier));
        break; // Fixed!
      }
    }
  }

  if(strcmp(obj->name, standard_obj->name))
  {
    out.writeRawData("NAM", 3);
    length = strlen(obj->name);
    out.writeRawData((const char *)&length, sizeof(length));
    out.writeRawData((const char *)obj->name, length);
  }
  if(strcmp(obj->description, standard_obj->description))
  {
    out.writeRawData("DES", 3);
    length = strlen(obj->description);
    out.writeRawData((const char *)&length, sizeof(length));
    out.writeRawData((const char *)obj->description, length);
  }
  if(strcmp(obj->short_description, standard_obj->short_description))
  {
    out.writeRawData("SDE", 3);
    length = strlen(obj->short_description);
    out.writeRawData((const char *)&length, sizeof(length));
    out.writeRawData((const char *)obj->short_description, length);
  }
  if(strcmp(obj->action_description, standard_obj->action_description))
  {
    out.writeRawData("ADE", 3);
    length = strlen(obj->action_description);
    out.writeRawData((const char *)&length, sizeof(length));
    out.writeRawData((const char *)obj->action_description, length);
  }

  if(obj->obj_flags.cost != standard_obj->obj_flags.cost)
  {
    out.writeRawData("COS", 3);
    out.writeRawData((const char *)&obj->obj_flags.cost, sizeof(obj->obj_flags.cost));
  }

  if (IS_SET(obj->obj_flags.more_flags, ITEM_24H_SAVE)) {
	  out.writeRawData("SAV", 3);
	  out.writeRawData((const char *)&obj->save_expiration, sizeof(time_t));
  }

  // extra descs are a little strange...it's a pointer to a list of them
  // I don't really want to handle this right now, so I'm going to just ignore them now
  // TODO - figure out a way to save extra descs later.  I'll just make them impossible
  // to restring for now

  // THIS IS WHERE YOU SHOULD PUT ANY ADDITIONS TO THE OBJ PFILE THAT NEED TO BE SAVED
  // A CORRESPONDING ENTRY SHOULD BE MADE IN THE READ FUNCTION
  // MAKE SURE YOUR FLAG ISN'T ALREADY USED

  // Stop flag.  This means we are done with this object on the read
  out.writeRawData("STP", 3);

  return TRUE;
}




/*
 * Restore container weights after a save.
 */
void restore_weight(struct obj_data *obj)
{
    struct obj_data *tmp;

    if ( obj == NULL )
      return;
    if (obj_index[obj->item_number].virt == 536) return;
    restore_weight( obj->contains );
    restore_weight( obj->next_content );
    for ( tmp = obj->in_obj; tmp; tmp = tmp->in_obj )
       GET_OBJ_WEIGHT( tmp ) += GET_OBJ_WEIGHT( obj );
}

void donothin() {}
// Read shared data from pfile
void store_to_char(struct char_file_u4 *st, CHAR_DATA *ch)
{
    int i;

    ch->clan = st->clan;

    GET_SEX(ch) = st->sex;
    GET_CLASS(ch) = st->c_class;
    GET_RACE(ch) = st->race;
    GET_LEVEL(ch) = st->level;

    ch->hometown = st->hometown;
    if(GET_LEVEL(ch) < 11)
      ch->hometown = START_ROOM;

    GET_STR(ch)       = GET_RAW_STR(ch)   = st->raw_str;
    GET_INT(ch)       = GET_RAW_INT(ch) = st->raw_intel;
    GET_WIS(ch)       = GET_RAW_WIS(ch)   = st->raw_wis;
    GET_DEX(ch)       = GET_RAW_DEX(ch)   = st->raw_dex;
    GET_CON(ch)       = GET_RAW_CON(ch)   = st->raw_con;

    ch->weight  = st->weight;
    ch->height  = st->height;
    ch->gold    = st->gold;
    ch->plat    = st->plat;
    ch->exp     = st->exp;
    ch->immune  = st->immune;
    ch->resist  = st->resist;
    ch->suscept = st->suscept;

    GET_HIT(ch)      = st->hit;
    GET_RAW_HIT(ch)  = st->raw_hit;
    GET_MANA(ch)     = st->mana;
    GET_RAW_MANA(ch) = st->raw_mana;

    // since move and ki don't get "redone" with stat bonuses we need to set the max here
    GET_MOVE(ch)     = st->move;
    ch->max_move     = GET_RAW_MOVE(ch) = st->raw_move;
    GET_KI(ch)       = st->ki;
    GET_RAW_KI(ch)   = st->raw_ki;

    ch->alignment    = st->alignment;
    ch->misc         = st->misc;

    GET_HP_METAS(ch)   = st->hpmetas;
    GET_MANA_METAS(ch) = st->manametas;
    GET_MOVE_METAS(ch) = st->movemetas;
    GET_AC_METAS(ch)   = st->acmetas;
    GET_AGE_METAS(ch)  = st->agemetas;

    ch->armor          = st->armor;
    ch->hitroll        = st->hitroll;
    ch->damroll        = st->damroll;
    donothin();

    ch->affected_by[0] = st->afected_by;
    ch->affected_by[1] = st->afected_by2;
   
/*    i = 0;
    while(st->afected_by[i] != -1) {
       ch->affected_by[i] = st->afected_by[i];
       i++;
    }
    st->afected_by[i] = -1;
*/
    for(i = 0; i <= 2; i++)
      GET_COND(ch, i) = st->conditions[i];

    // it's ok assigning the in_room directly since do_on_login_stuff() will
    // make the actual call to "char_to_room" using this data later
    ch->in_room = real_room(st->load_room);

    if(ch->in_room == (-1)) {
      if(GET_LEVEL(ch) >= IMMORTAL)
        ch->in_room = real_room(17);
      else
        ch->in_room = real_room(START_ROOM);
    }
}



// copy vital data from a players char-structure to the file structure 
// return 'age' of character unmodified
void char_to_store(CHAR_DATA *ch, struct char_file_u4 *st, struct time_data & tmpage)
{
  int i;
  int x;
  struct affected_type *af;
  struct obj_data *char_eq[MAX_WEAR];

  // Remove all the eq and store it in temp storage
  for(i=0; i<MAX_WEAR; i++) {
    if (ch->equipment[i])
      char_eq[i] = unequip_char(ch, i,1);
    else
      char_eq[i] = 0;
  }

  // Unaffect everything a character can be affected by spell-wise
  for(af = ch->affected; af; af = af->next) 
  {
    affect_modify( ch, af->location, af->modifier, af->bitvector, FALSE);
  }

  st->sex      = GET_SEX(ch);
  st->c_class  = GET_CLASS(ch);
  st->race     = GET_RACE(ch);
  st->level    = GET_LEVEL(ch);

  st->raw_str   = GET_RAW_STR(ch);
  st->raw_intel = GET_RAW_INT(ch);
  st->raw_wis   = GET_RAW_WIS(ch);
  st->raw_dex   = GET_RAW_DEX(ch);
  st->raw_con   = GET_RAW_CON(ch);

  st->mana      = GET_MANA(ch);
  st->raw_mana  = GET_RAW_MANA(ch);
  st->hit       = GET_HIT(ch);
  st->raw_hit   = GET_RAW_HIT(ch);
  st->move      = GET_MOVE(ch);
  st->raw_move  = GET_RAW_MOVE(ch);
  st->ki        = GET_KI(ch);
  st->raw_ki    = GET_RAW_KI(ch);

  st->weight    = GET_WEIGHT(ch);
  st->height    = GET_HEIGHT(ch);
  for(i = 0; i < 3; i++)
    st->conditions[i] = GET_COND(ch, i);

  st->hometown = ch->hometown;

//  gets set outside
//  st->load_room = world[ch->in_room].number;

//  st->gold      = GET_GOLD(ch);
  st->gold      = 0; // Moved
  st->plat      = GET_PLATINUM(ch);
  st->exp       = GET_EXP(ch);
  st->immune    = ch->immune;
  st->resist    = ch->resist;
  st->suscept   = ch->suscept;
  st->alignment = ch->alignment;
  st->misc      = ch->misc;

  st->hpmetas   = GET_HP_METAS(ch);
  st->manametas = GET_MANA_METAS(ch);
  st->movemetas = GET_MOVE_METAS(ch);
  st->clan      = ch->clan;

  // make sure rest of unused are set to 0
  for(x = 0; x < 3; x++)
    st->extra_ints[x] = 0;

  if(IS_MOB(ch)) {
    st->armor = ch->armor;
    st->hitroll =  ch->hitroll;
    st->damroll =  ch->damroll;
    st->afected_by = ch->affected_by[0];
    st->afected_by2 = ch->affected_by[1];
//  x=0;
//  while(ch->afected_by[x] != -1) {
//     st->afected_by[x] = ch->affected_by[x];
//     x++;
//  }
//  st->afected_by[x] = -1;
  } else { 
    switch(GET_CLASS(ch)) {
      case CLASS_MAGE: st->armor = 150; break;
      case CLASS_DRUID: st->armor = 140; break;
      case CLASS_CLERIC: st->armor = 130; break;
      case CLASS_ANTI_PAL: st->armor = 120; break;
      case CLASS_THIEF: st->armor = 110; break;
      case CLASS_BARD: st->armor = 100; break;
      case CLASS_BARBARIAN: st->armor = 80; break;
      case CLASS_RANGER: st->armor = 60; break;
      case CLASS_PALADIN: st->armor = 40; break;
      case CLASS_WARRIOR: st->armor = 20; break;
      case CLASS_MONK: st->armor = 0; break;
      default: st->armor   = 100; break;
    }
    st->hitroll =  0;
    st->damroll =  0;
    st->afected_by = 0;
    st->afected_by2 = 0;
    st->acmetas = GET_AC_METAS(ch);
    st->agemetas = GET_AGE_METAS(ch);
    tmpage = ch->pcdata->time;
  }

  // re-affect the character with spells
  for(af = ch->affected; af; af = af->next) {
      affect_modify( ch, af->location, af->modifier, af->bitvector, TRUE);
  }

  // re-equip the character with his eq
  for(i=0; i<MAX_WEAR; i++) {
    if (char_eq[i])
      equip_char(ch, char_eq[i], i,1);
  }
}

