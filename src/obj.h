/***************************************************************************
 *  file: obj.h , Structures                               Part of DIKUMUD *
 *  Usage: Declarations of object data structures                          *
 *  Copyright (C) 1990, 1991 - see 'license.doc' for complete information. *
 *                                                                         *
 *  Copyright (C) 1992, 1993 Michael Chastain, Michael Quan, Mitchell Tse  *
 *  Performance optimization and bug fixes by MERC Industries.             *
 *  You can use our stuff in any way you like whatsoever so long as ths   *
 *  copyright notice remains intact.  If you like it please drop a line    *
 *  to mec@garnet.berkeley.edu.                                            *
 *                                                                         *
 *  This is free software and you are benefitting.  We hope that you       *
 *  share your changes too.  What goes around, comes around.               *
 ***************************************************************************/
/* $Id: obj.h,v 1.36 2015/06/16 04:10:54 pirahna Exp $ */
#ifndef OBJ_H_
#define OBJ_H_
#include "Size.h"
#include "EquipmentLocation.h"
#include "ContainerStatus.h"
#include "LiquidType.h"
#include "UtilityType.h"
#include "ItemMoreFlags.h"
#include "ItemExtra.h"
#include "WearLocation.h"
#include "ItemType.h"
#include "structs.h" // ubyte
#include "character.h"

struct active_object
{
    struct obj_data *obj;
    struct active_object *next;
};

struct extra_descr_data
{
    char *keyword;                 /* Keyword in look/examine          */
    char *description;             /* What to see                      */
    struct extra_descr_data *next; /* Next in list                     */
};

#define OBJ_NOTIMER      -7000000

struct obj_flag_data
{
    int32 value[4];       /* Values of the item (see list)    */
    ubyte type_flag;     /* Type of item                     */
    uint32 wear_flags;     /* Where you can wear it            */
    uint16 size;           /* Race restrictions                */
    uint32 extra_flags;    /* If it hums, glows etc            */
     int16 weight;         /* Weight what else                 */
     int32 cost;           /* Value when sold (gp.)            */
    uint32 more_flags;     /* A second bitvector (extra_flags2)*/
     int16 eq_level;	/* Min level to use it for eq       */
     int16 timer;          /* Timer for object                 */
};

struct obj_affected_type
{
     int32 location;      /* Which ability to change (APPLY_XXX) */
     int32 modifier;     /* How much it changes by              */
};

struct tab_data;
struct table_data;
struct machine_data;
struct wheel_data;

/* ======================== Structure for object ========================= */
struct obj_data {
	int32 item_number;                  /* Where in data-base               */
	int32 in_room;                      /* In what room -1 when conta/carr  */
	int vroom;                          /* for corpse saving */
	obj_flag_data obj_flags;            /* Object information               */
	int16 num_affects;
	obj_affected_type *affected;        /* Which abilities in PC to change  */

	char *name;                         /* Title of object :get etc.        */
	char *description;                  /* When in room                     */
	char *short_description;            /* when worn/carry/in cont.         */
	char *action_description;           /* What to write when used          */
	extra_descr_data *ex_description;   /* extra descriptions     */
	CHAR_DATA *carried_by;              /* Carried by :NULL in room/conta   */
	CHAR_DATA *equipped_by;             /* so I can access the player :)    */

	obj_data *in_obj;                   /* In what object NULL when none    */
	obj_data *contains;                 /* Contains objects                 */

	obj_data *next_content;             /* For 'contains' lists             */
	obj_data *next;                     /* For the object list              */
	obj_data *next_skill;
	table_data *table;
	machine_data *slot;
	wheel_data *wheel;
	time_t save_expiration;
};


/* ***********************************************************************
*  file element for object file. BEWARE: Changing it will ruin the file  *
*********************************************************************** */

#define CURRENT_OBJ_VERSION        1

struct obj_file_elem 
{
    int16 version;
    int32 item_number;
    int16 timer;
    int16 wear_pos;
    int16 container_depth;
    int32 other[5];        // unused
};

// functions from objects.cpp
int eq_max_durability(obj_data * obj);
int damage_eq_once(obj_data * obj);
int eq_current_damage(obj_data * obj);
void eq_remove_damage(obj_data * obj);
void add_obj_affect(obj_data * obj, int loc, int mod);
void remove_obj_affect_by_index(obj_data * obj, int index);
void remove_obj_affect_by_type(obj_data * obj, int loc);
int recheck_height_wears(char_data *ch);
bool fullSave(obj_data *obj);
void heightweight(char_data *ch, bool add);

typedef vector<const char *> item_types_t;

#endif
