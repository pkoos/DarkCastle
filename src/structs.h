/************************************************************************
| $Id: structs.h,v 1.19 2009/01/24 19:21:29 kkoons Exp $
| structs.h
| Description:  This file should go away someday - it's stuff that I
|   wasn't sure how to break up.  --Morc XXX
*/
#ifndef STRUCTS_H_
#define STRUCTS_H_

extern "C" {
    #include <sys/types.h>
    #include <stdio.h> // FILE
}

#include "CVoteData.h"

#include <string>
#include <vector>
#include <map>



typedef	struct char_data	CHAR_DATA;
typedef	struct obj_data		OBJ_DATA;

/* ======================================================================== */
struct txt_block
{
    char *text;
    struct txt_block *next;
    int aliased;
};

typedef struct txt_q
{
    struct txt_block *head;
    struct txt_block *tail;
} TXT_Q;

struct msg_type 
{
    char *attacker_msg;  /* message to attacker */
    char *victim_msg;    /* message to victim   */
    char *room_msg;      /* message to room     */
};

struct message_type
{
    struct msg_type die_msg;      /* messages when death            */
    struct msg_type miss_msg;     /* messages when miss             */
    struct msg_type hit_msg;      /* messages when hit              */
    struct msg_type sanctuary_msg;/* messages when hit on sanctuary */
    struct msg_type god_msg;      /* messages when hit on god       */
    struct message_type *next;/* to next messages of ths kind.*/
};

struct message_list
{
    int a_type;               /* Attack type				*/
    int number_of_attacks;    /* # messages to chose from		*/
    struct message_type *msg; /* List of messages			*/
    struct message_type *msg2; /* List of messages with toggle damage ON */
};

extern void debugpoint();

#endif
