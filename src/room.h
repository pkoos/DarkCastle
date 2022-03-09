#ifndef __ROOM_H__
#define __ROOM_H__
/************************************************************************
| $Id: room.h,v 1.21 2015/06/14 02:38:12 pirahna Exp $
| room.h
| Description:  This file contains all of the room header file/constant
|   information.  It also contains information about the 'world' structs.
*/
#include "RoomStructs.h"
#include "NumberTypes.h"
#include "SectorTypes.h"
#include "ExitInfo.h"
#include "Direction.h"
#include "RoomFlags.h"
#include "obj.h" // ubyte
#include "MobActivity.h" // struct path_data
#include "player.h" // CLASS_MAX

/* Bitvector For 'temp_room_flags' 
   temp_room_flags are NOT in the world file and cannot be saved or added at boot time
   These are for runtime flags, such as ETHEREAL_FOCUS
*/

#define ROOM_ETHEREAL_FOCUS   1
#define TEMP_ROOM_FLAG_AVAILABLE 1<<1

/* Internal flags */
#define iNO_TRACK   1
#define iNO_MAGIC   1<<1

#endif // __ROOM_H__
