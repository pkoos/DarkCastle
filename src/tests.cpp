#include <iostream>
using namespace std;

#include "character.h"
#include "tests.h"
#define ASSERT_OFFSET(var) assert((size_t)offsetof(char_file_u, var) == (size_t)offsetof(char_file_u4, var));

bool test_char_file_u()
{
    typedef signed char sbyte;
    typedef unsigned char ubyte;
    typedef signed char byte;
    typedef signed short int int16;
    typedef unsigned short int uint16;
    typedef signed int int32;
    typedef unsigned int uint32;
    typedef signed long long int64;
    typedef unsigned long long uint64;

    struct char_file_u
    {
        sbyte sex;     /* Sex */
        sbyte c_class; /* Class */
        sbyte race;    /* Race */
        sbyte level;   /* Level */

        sbyte raw_str;
        sbyte raw_intel;
        sbyte raw_wis;
        sbyte raw_dex;
        sbyte raw_con;
        sbyte conditions[3];

        ubyte weight;
        ubyte height;

        int16 hometown;
        uint32 gold;
        uint32 plat;
        int64 exp;
        uint32 immune;
        uint32 resist;
        uint32 suscept;

        int32 mana;     // current
        int32 raw_mana; // max without eq/stat bonuses
        int32 hit;
        int32 raw_hit;
        int32 move;
        int32 raw_move;
        int32 ki;
        int32 raw_ki;

        int16 alignment;
        uint32 hpmetas; // Used by familiars too... why not.
        uint32 manametas;
        uint32 movemetas;

        int16 armor; // have to save these since mobs have different bases
        int16 hitroll;
        int16 damroll;
        int32 afected_by;
        int32 afected_by2;
        uint32 misc; // channel flags

        int16 clan;
        int32 load_room; // Which room to place char in

        uint32 acmetas;
        int32 agemetas;
        int32 extra_ints[3]; // available just in case
    };

    char_file_u uc;
    char_file_u4 uc4;

    ASSERT_OFFSET(sex);     /* Sex */
    ASSERT_OFFSET(c_class); /* Class */
    ASSERT_OFFSET(race);    /* Race */
    ASSERT_OFFSET(level);   /* Level */

    ASSERT_OFFSET(raw_str);
    ASSERT_OFFSET(raw_intel);
    ASSERT_OFFSET(raw_wis);
    ASSERT_OFFSET(raw_dex);

    ASSERT_OFFSET(raw_con);
    ASSERT_OFFSET(conditions[0]);
    ASSERT_OFFSET(conditions[1]);
    ASSERT_OFFSET(conditions[2]);
    ASSERT_OFFSET(conditions[3]);

    ASSERT_OFFSET(weight);
    ASSERT_OFFSET(height);
    ASSERT_OFFSET(hometown);

    ASSERT_OFFSET(gold);
    ASSERT_OFFSET(plat);
    ASSERT_OFFSET(exp);
    ASSERT_OFFSET(immune);
    ASSERT_OFFSET(resist);
    ASSERT_OFFSET(suscept);

    ASSERT_OFFSET(mana);     // current
    ASSERT_OFFSET(raw_mana); // max without eq/stat bonuses
    ASSERT_OFFSET(hit);
    ASSERT_OFFSET(raw_hit);
    ASSERT_OFFSET(move);
    ASSERT_OFFSET(raw_move);
    ASSERT_OFFSET(ki);
    ASSERT_OFFSET(raw_ki);

    ASSERT_OFFSET(hpmetas); // Used by familiars too... why not.
    ASSERT_OFFSET(manametas);
    ASSERT_OFFSET(movemetas);

    ASSERT_OFFSET(alignment)
    ASSERT_OFFSET(armor)
    ASSERT_OFFSET(hitroll)
    ASSERT_OFFSET(damroll)
    ASSERT_OFFSET(afected_by)
    ASSERT_OFFSET(afected_by2)
    ASSERT_OFFSET(misc)
    ASSERT_OFFSET(clan)
    ASSERT_OFFSET(load_room)
    ASSERT_OFFSET(acmetas)
    ASSERT_OFFSET(agemetas)
    ASSERT_OFFSET(extra_ints[0])
    ASSERT_OFFSET(extra_ints[1])
    ASSERT_OFFSET(extra_ints[2])
    ASSERT_OFFSET(extra_ints[3])

    cerr << offsetof(char_file_u, extra_ints[3]) << endl;
    cerr << offsetof(char_file_u4, extra_ints[3]) << endl;
    assert(140 == sizeof(char_file_u4));
}

bool test_code(void)
{
    test_char_file_u();
    

    return true;
}