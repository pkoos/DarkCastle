#ifndef ITEM_MORE_FLAGS_H_
#define ITEM_MORE_FLAGS_H_

/* Bitvector for 'extra_flags' */

#define ITEM_GLOW               1U
#define ITEM_HUM                1U<<1
#define ITEM_DARK               1U<<2
#define ITEM_LOCK               1U<<3
#define ITEM_ANY_CLASS          1U<<4     // Any class can use
#define ITEM_INVISIBLE          1U<<5
#define ITEM_MAGIC              1U<<6
#define ITEM_NODROP             1U<<7
#define ITEM_BLESS              1U<<8
#define ITEM_ANTI_GOOD          1U<<9
#define ITEM_ANTI_EVIL          1U<<10
#define ITEM_ANTI_NEUTRAL       1U<<11
#define ITEM_WARRIOR            1U<<12
#define ITEM_MAGE               1U<<13
#define ITEM_THIEF              1U<<14
#define ITEM_CLERIC             1U<<15
#define ITEM_PAL                1U<<16
#define ITEM_ANTI               1U<<17
#define ITEM_BARB               1U<<18
#define ITEM_MONK               1U<<19
#define ITEM_RANGER             1U<<20
#define ITEM_DRUID              1U<<21
#define ITEM_BARD               1U<<22
#define ITEM_TWO_HANDED         1U<<23
#define ITEM_ENCHANTED          1U<<24
#define ITEM_SPECIAL            1U<<25
#define ITEM_NOSAVE             1U<<26
#define ITEM_NOSEE		        1U<<27
#define ITEM_NOREPAIR           1U<<28
#define ITEM_NEWBIE             1U<<29 // Newbie flagged.
#define ITEM_PC_CORPSE		    1U<<30
#define ITEM_QUEST              1U<<31

#endif