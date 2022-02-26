# <ins>Room Documentation
This is the format for an individual room.


```
#room_num
name~
room_desc
~
zone_num room_flags sector_type
Ddir_num
dir_desc
~
keyword~
exit_info key_onum dest_room
E
extra_keyword~
extra_desc
~
S
```
Mark the end of the file with this: 
```
$~
```
## <ins>`dest_room`
Where the exit leads to.

## <ins>`dir_desc`
A description seen when you look 'direction.' Add the tilde on the line below the description.

## <ins>`dir_keyword`
A keyword used to see something in the given direction. Most likely 'door' but could be anything.

## <ins>`dir_num`
A number designating the direction of the exit from the room you are in.  
0: North  
1: East  
2: South  
3: West  
4: Up  
5: Down  

## <ins>`exit_info`</ins> - ExitInfo.h
A bitvector of exit information.

## <ins>`extra_desc`
The description from looking at the keyword. Add the tilde to the line below the description.

## <ins>`extra_keyword`
This is for extra objects in the room you can look at, such as signs.

## <ins>`key_onum`
The object number of the key that unlocks the direction, or -1 if unlocked.

## <ins>`name`
The title of the room when you are standing in it.

## <ins>`room_desc`
A description of the room you are in. This section is ended with a tilde. Make sure the tilde is BELOW the description, as this is what adds the newline.

## <ins>`room_flags`</ins> - RoomFlags.h
A bitvector of room flags

## <ins>`room_num`
The id of the room. Must be unique, and in ascending order.

## <ins>`sector_type`</ins> - SectorType.h
An id for the sector type.

## <ins>`zone_num`
This number links to a zonefile, and is used for reset information.