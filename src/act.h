#ifndef ACT_H_
#define ACT_H_
/************************************************************************
| act.h
| Written by: Morcallen
| Date: 27 July 1996
| Description:  This file contains the header information for the new
|   act() function
*/
#include "NumberTypes.h"
#include "MessageDisplay.h"
#include "MessageDestination.h"
#include <string>
#include "character.h"
#include "obj.h"
#include "token.h"
//--
// Function interface
//--

struct act_return
{
  string str;
  int retval;
};

act_return act(const char *str, CHAR_DATA *ch, OBJ_DATA *obj, void *vict_obj,
          int16 destination, int16 flags);

act_return act(const std::string &str, CHAR_DATA *ch, OBJ_DATA *obj, void *vict_obj,
	 int16 destination, int16 flags);

struct send_tokens_return
{
  string str;
  int retval;
};

send_tokens_return send_tokens(TokenList * tokens, CHAR_DATA *ch, OBJ_DATA * obj, void * vch, int flags, CHAR_DATA *to);

void send_message(const char *str, CHAR_DATA *to);

#endif /* ACT_H_ */
