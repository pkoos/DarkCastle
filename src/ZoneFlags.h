#ifndef ZONE_FLAGS_H_
#define ZONE_FLAGS_H_

/* Zone Flag Bits */

#define ZONE_NO_TELEPORT   1
#define ZONE_IS_TOWN       1<<1  // Keep out the really bad baddies that are STAY_NO_TOWN
#define ZONE_MODIFIED      1<<2
#define ZONE_UNUSED        1<<3
#define ZONE_BPORT	       1<<4
#define ZONE_NOCLAIM       1<<5 // cannot claim this area
#define ZONE_NOHUNT        1<<6
// Remember to update const.C  zone_bits[] if you change this

#endif