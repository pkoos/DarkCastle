#ifndef MOB_PROG_STRUCTS_H_
#define MOB_PROG_STRUCTS_H_

typedef struct char_data CHAR_DATA;
typedef struct obj_data OBJ_DATA;
// * ------- Begin MOBProg stuff ----------- *

typedef struct  mob_prog_data           MPROG_DATA;
typedef struct  mob_prog_act_list       MPROG_ACT_LIST;

struct  mob_prog_act_list
{
    MPROG_ACT_LIST * next;
    char *           buf;
    CHAR_DATA *      ch;
    OBJ_DATA *       obj;
    void *           vo;
};

struct  mob_prog_data
{
    MPROG_DATA *next;
    int         type;
    char *      arglist;
    char *      comlist;
};

// * ------- End MOBProg stuff ----------- *

#endif