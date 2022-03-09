#ifndef CHARACTER_H_
#define CHARACTER_H_
/******************************************************************************
| $Id: character.h,v 1.85 2014/07/26 23:21:23 jhhudso Exp $
| Description: This file contains the header information for the character
|   class implementation.
*/

#define  COMPILE_WITH_CHANGES 1

#include "CharacterStructs.h"
#include "CombatAffects.h"
#include "Actions.h"
#include "Maximums.h"
#include "Progs.h"
#include "Attributes.h"
#include "Position.h"
#include "SexType.h" /* Male, Female, etc. */
#include "Affect.h"  /* MAX_AFFECTS, etc.. */
#include "alias.h"   /* struct char_player_alias, MAX_ALIASES, etc.. */
#include "NumberTypes.h"
#include "timeinfo.h" // time data, etc..
#include "event.h" // eventBrief
#include "isr.h"   // SAVE_TYPE_MAX
#include "utility.h"
#include "mobile.h"
#include "sing.h"
#include "quest.h"
#include "class.h"

extern "C" {
#include <sys/time.h>
#include <stdint.h>
}

#include <queue>
#include <map>
#include <string>
#include <vector>
#include <string>
#include <algorithm>
#include "DC.h"

struct songInfo;

#define START_ROOM        3001 // Where you login
#define CFLAG_HOME        3014 // Where the champion flag normally rests
#define SECOND_START_ROOM 3059 // Where you go if killed in start room
#define FARREACH_START_ROOM 17868
#define THALOS_START_ROOM 5317

#define CHAR_VERSION    -4

/************************************************************************
| max stuff - this is needed almost everywhere
*/
#define CHAMPION_ITEM 45

struct profession {
	std::string name;
	std::string Name;
	uint16_t skillno;
	uint8_t c_class;
};

#endif
