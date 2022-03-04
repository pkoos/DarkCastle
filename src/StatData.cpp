#include "StatData.h"

ubyte StatData::getMin(ubyte cur, sbyte race_mod, ubyte min)
{
   if (min > cur + race_mod)
   {
      ubyte points_needed = min - (cur + race_mod);
      if (points > points_needed)
      {
         points -= points_needed;
         cur += points_needed;
      }
   }

   return cur;
}

void StatData::setMin(void)
{
   str[0] = getMin(str[0], oRaces[race].mod_str, MAX(races[race].min_str, classes[clss].min_str));
   dex[0] = getMin(dex[0], oRaces[race].mod_dex, MAX(races[race].min_dex, classes[clss].min_dex));
   con[0] = getMin(con[0], oRaces[race].mod_con, MAX(races[race].min_con, classes[clss].min_con));
   tel[0] = getMin(tel[0], oRaces[race].mod_int, MAX(races[race].min_int, classes[clss].min_int));
   wis[0] = getMin(wis[0], oRaces[race].mod_wis, MAX(races[race].min_wis, classes[clss].min_wis));
}