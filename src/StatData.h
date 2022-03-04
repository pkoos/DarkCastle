#ifndef STAT_DATA_H_
#define STAT_DATA_H_

#include "RaceData.h"
#include "NumberTypes.h"
#include "utility.h"

extern RaceData oRaces[];

class StatData {
    public:
        StatData();
        void setMin(void);
        ubyte getMin(ubyte cur, sbyte mod, ubyte min);
        int str[5], tel[5], wis[5], dex[5], con[5];
        int min_str, min_int, min_wis, min_dex, min_con;
        unsigned int points;
        unsigned int selection;
        ubyte race;
        ubyte clss;
};

#endif
