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

extern void debugpoint();

#endif
