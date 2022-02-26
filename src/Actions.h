#ifndef ACTIONS_H_
#define ACTIONS_H_

#define BASE_STAT      0

#define ACT_SPEC       1
#define ACT_SENTINEL   2
#define ACT_SCAVENGER  3
#define ACT_NOTRACK    4
#define ACT_NICE_THIEF 5
#define ACT_AGGRESSIVE 6
#define ACT_STAY_ZONE  7
#define ACT_WIMPY      8
			        /* aggressive only attack sleeping players */
#define ACT_2ND_ATTACK 9
#define ACT_3RD_ATTACK 10
#define ACT_4TH_ATTACK 11
			         /* Each attack bit must be set to get up */
			         /* 4 attacks                             */
/*
 * For ACT_AGGRESSIVE_XXX, you must also set ACT_AGGRESSIVE
 * These switches can be combined, if none are selected, then
 * the mobile will attack any alignment (same as if all 3 were set)
 */
#define ACT_AGGR_EVIL       12
#define ACT_AGGR_GOOD       13
#define ACT_AGGR_NEUT       14
#define ACT_UNDEAD          15
#define ACT_STUPID          16
#define ACT_CHARM           17
#define ACT_HUGE            18
#define ACT_DODGE           19
#define ACT_PARRY           20
#define ACT_RACIST          21
#define ACT_FRIENDLY        22
#define ACT_STAY_NO_TOWN    23
#define ACT_NOMAGIC         24
#define ACT_DRAINY          25
#define ACT_BARDCHARM       26
#define ACT_NOKI            27
#define ACT_NOMATRIX        28
#define ACT_BOSS            29
#define ACT_NOHEADBUTT      30
#define ACT_NOATTACK        31
#define ACT_SWARM           33
#define ACT_TINY            34
#define ACT_NODISPEL        35
#define ACT_POISONOUS       36
#define ACT_NO_GOLD_BONUS   37
#define ACT_MAX             37
//#define CHECKTHISACT      64 //Do not chance unless ASIZE changes

#endif