#ifndef MAXIMUMS_H_
#define MAXIMUMS_H_

#define MAX_STRING_LENGTH   8192
#define MAX_INPUT_LENGTH     160
#define MAX_MESSAGES         150
#define MAX_OBJ_SDESC_LENGTH 100
#define MAX_RAW_INPUT_LENGTH  512
#define MAX_NOTE_LENGTH 1000      /* arbitrary */
#define MAX_HINTS 100
#define MAX_MESSAGE_LENGTH 	4096
#define MAX_NAME_LENGTH 12
#define MAX_CLAN_DESC_LENGTH 1022
#define MAX_GOLEMS           2 // amount of golems above +1

#define MAX_WEAR    23 // maximum number of items worn

#define MAX_HIDE 10

#define MAX_NAME_LENGTH 12

#define MAX_WORLD_ROOM        50000  // should never get this high...
                                     // it's just to keep builders/imps from 
                                     // doing a 'goto 1831919131928' and 
                                     // creating it

#define MAX_FREAD_INT 64000
#define MAX_ZONE    200
#define MAX_INDEX   6000
#define MAX_RESET  16383

#ifndef LONG_MAX

#define LONG_MAX    2147483467
#define LONG_MIN    -2147483467
#define INT_MAX     32768
#define INT_MIN     -32768

#endif

#define MAX_CHAMP_DEATH_MESSAGE		14

#define MAX_NPC_CORPSE_TIME 7
#define MAX_PC_CORPSE_TIME 7
#define MAX_CLAN_LEN             15
#define MAX_INGREDIENTS    10
#define MAX_DIRS        6
#define MAX_NUM_EVT     1024
#define MAX_GAME_PORTALS 9

#define MAX_HELP_KEYWORD_LENGTH     20
#define MAX_HELP_RELATED_LENGTH     60
#define MAX_HELP_LENGTH             8192

#define MAXIMUM_KI   100

#define MAX_MORTAL 60

#define MAX_HELP  1100

#define MAX_GEM_ASSEMBLER_ITEM   10

#define MAX_PROFESSIONS 2

#define MAX_PC_RACE 	  9

#define MAX_RACE         32

#define MAX_SHOP 70
#define MAX_TRADE 5

#define MAX_BUF_LENGTH               240
#define MAX_SPL_LIST                177
#define MAX_KI_LIST   10

#define MAX_THROW_NAME     60

#define PASSWORD_LEN    20
#define DESC_LENGTH     80

#define QUEST_MAX    1     //max quests at a time
#define QUEST_SHOW   10    //max quests shown at a time
#define QUEST_CANCEL 15    //max quests canceled at a time
#define QUEST_TOTAL  500   //max total quests in file
#endif