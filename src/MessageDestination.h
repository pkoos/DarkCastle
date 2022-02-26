#ifndef MESSAGE_DESTINATION_H_
#define MESSAGE_DESTINATION_H_

// These constants need to go in the destination variable
#define TO_ROOM   0   // Everyone in ch's room except ch
#define TO_VICT   1   // Just vict_obj
#define TO_CHAR   2   // Just ch
#define TO_ZONE   3   // Everyone in ch's zone except ch
#define TO_WORLD  4   // Everyone in the world except ch
#define TO_GROUP  5   // Everyone in the ch's group except ch
#define TO_ROOM_NOT_GROUP  6   // Everyone in ch's room except ch's group or ch

#endif