#ifndef RACE_DATA_H_
#define RACE_DATA_H_

#include <string>
#include "Affect.h"

class RaceData {
    public:
        char *singular_name;   /* Dwarf, Elf, etc.     */
        std::string lowercase_name; /* dwarf, elf, etc.     */
        char *plural_name;     /* dwarves, elves, etc. */
        bool playable;    /* Can a player play as this race? */
        long body_parts;  /* bitvector for body parts       */
        long immune;      /* bitvector for immunities       */
        long resist;      /* bitvector for resistances      */
        long suscept;     /* bitvector for susceptibilities */
        long hate_fear;   /* bitvector for hate/fear        */
        long friendly;    /* bitvector for friendliness     */
        int  min_weight;      /* min weight */
        int  max_weight;
        
        int  min_height;      
        int  max_height;

        unsigned min_str;
        unsigned max_str;
        int mod_str;

        unsigned min_dex;
        unsigned max_dex;
        int mod_dex;

        unsigned min_con;
        unsigned max_con;
        int mod_con;

        unsigned min_int;
        unsigned max_int;
        int mod_int;

        unsigned min_wis;
        unsigned max_wis;
        int mod_wis;
        
        int affects;      /* automatically added affects   */
        const char *unarmed;    // unarmed attack message
};

#endif