#ifndef MESSAGE_DISPLAY_H_
#define MESSAGE_DISPLAY_H_


// These constants go in the flags part (optional -- 0 for none)
#define DEFAULT       0    // "someone" if invisible, sleepers skipped
#define NOTVICT       1<<0 // Sends to destination except victim
#define GODS          1<<1 // Sends to destination, gods only
#define ASLEEP        1<<2 // Will send even to sleepers
#define INVIS_NULL    1<<3 // Invisible messages are skipped completely
#define INVIS_VISIBLE 1<<4 // Invisible messages are shown w/names visible
#define FORCE         1<<5 // Sends regardless of nanny state
#define STAYHIDE      1<<6 // Stayhide flag keeps thieves in hiding.
#define BARDSONG      1<<7 // Bard song so only show it to people in room with BARD_SONG toggle set to verbose

#endif