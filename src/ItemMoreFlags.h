#ifndef ITEM_MORE_FLAGS_H_
#define ITEM_MORE_FLAGS_H_

/* Bitvector for 'more_flags' */

#define ITEM_NO_RESTRING        1U
#define ITEM_LIMIT_SACRIFICE    1U<<1
#define ITEM_UNIQUE             1U<<2
#define ITEM_NO_TRADE           1U<<3
#define ITEM_NONOTICE           1U<<4  // Item doesn't show up on 'look' but
                                       // can still be accessed with 'get' etc FUTURE
#define ITEM_NOLOCATE           1U<<5 
#define ITEM_UNIQUE_SAVE	    1U<<6 // for corpse saving, didn't want to affect other unique flag so made a new one

#define ITEM_NPC_CORPSE		    1U<<7
#define ITEM_PC_CORPSE_LOOTED   1U<<8
#define ITEM_NO_SCRAP 		    1U<<9
#define ITEM_CUSTOM             1U<<10
#define ITEM_24H_SAVE		    1U<<11
#define ITEM_NO_DISARM		    1U<<12
#define ITEM_TOGGLE		        1U<<13 // Toggles for certain items.
#define ITEM_NO_CUSTOM          1U<<14

#endif