#ifndef __ROOM_H__
#define __ROOM_H__
/************************************************************************
| $Id: room.h,v 1.21 2015/06/14 02:38:12 pirahna Exp $
| room.h
| Description:  This file contains all of the room header file/constant
|   information.  It also contains information about the 'world' structs.
*/
#include "ExitInfo.h"
#include "Direction.h"
#include "RoomFlags.h"
#include "structs.h" // ubyte
#include "obj.h" // ubyte
#include "MobActivity.h" // struct path_data
#include "player.h" // CLASS_MAX

// The following defs are for room_data

const unsigned int NOWHERE = 0;

/* Bitvector For 'temp_room_flags' 
   temp_room_flags are NOT in the world file and cannot be saved or added at boot time
   These are for runtime flags, such as ETHEREAL_FOCUS
*/

#define ROOM_ETHEREAL_FOCUS   1
#define TEMP_ROOM_FLAG_AVAILABLE 1<<1

/* Internal flags */
#define iNO_TRACK   1
#define iNO_MAGIC   1<<1


struct room_direction_data
{
    char *general_description;      /* When look DIR.                  */ 
    char *keyword;                  /* for open/close                  */  
    int16 exit_info;                /* Exit info                       */
    CHAR_DATA *bracee;		    /* This is who is bracing the door */
    int16 key;                      /* Key's number (-1 for no key)    */
    int16 to_room;                  /* Where direction leeds (NOWHERE) */
};

struct room_track_data
{
    int weight;
    int race;
    int direction; 
    int sex;
    int condition;
    char *trackee;
    
    room_track_data * next;
    room_track_data * previous;
}; 

struct deny_data
{
  struct deny_data *next;
  int vnum;
};

// ========================= Structure for room ==========================
struct room_data
{
    int16 number;                       // Rooms number
    int16 zone;                         // Room zone (for resetting)
    int sector_type;                     // sector type (move/hide)
    struct deny_data *denied;
    char * name;                         // Rooms name 'You are ...'
    char * description;                  // Shown when entered
    extra_descr_data * ex_description;   // for examine/look
    room_direction_data * dir_option[MAX_DIRS]; // Directions
    uint32 room_flags;                     // DEATH, DARK ... etc
    uint32 temp_room_flags;             // A second bitvector for flags that do NOT get saved.  These are temporary runtime flags.
    int16 light;                        // Light factor of room
    
    int (*funct)(CHAR_DATA*, int, char*);  // special procedure
	 
    struct obj_data *contents;   // List of items in room
    CHAR_DATA *people;           // List of NPC / PC in room
    
    int              nTracks;    // number of tracks in the room
    room_track_data* tracks;     // beginning of the list of scents
    room_track_data* last_track; // last in the scent list
    
    void              AddTrackItem(room_track_data * newTrack);
    room_track_data * TrackItem(int nIndex);
    void              FreeTracks();
    int 	      iFlags; // Internal flags. These do NOT save.
    struct path_data *paths;
    bool              allow_class[CLASS_MAX];
};

#endif // __ROOM_H__
