#ifndef ATTACK_TYPES_H_
#define ATTACK_TYPES_H_

/*
 * Types of attacks.
 * Must be non-overlapping with spell/skill types,
 * but may be arbitrary beyond that.
 * If you change this, update strs_damage_types[] in const.cpp
 */
#define TYPE_HIT                     1000
#define TYPE_BLUDGEON                (TYPE_HIT +  1)
#define TYPE_PIERCE                  (TYPE_HIT +  2)
#define TYPE_SLASH                   (TYPE_HIT +  3)
#define TYPE_WHIP                    (TYPE_HIT +  4)
#define TYPE_CLAW                    (TYPE_HIT +  5)
#define TYPE_BITE                    (TYPE_HIT +  6)
#define TYPE_STING                   (TYPE_HIT +  7)
#define TYPE_CRUSH                   (TYPE_HIT +  8)
#define TYPE_SUFFERING               (TYPE_HIT +  9)
#define TYPE_MAGIC                   (TYPE_HIT + 10)
#define TYPE_CHARM                   (TYPE_HIT + 11)
#define TYPE_FIRE                    (TYPE_HIT + 12)
#define TYPE_ENERGY                  (TYPE_HIT + 13)
#define TYPE_ACID                    (TYPE_HIT + 14)
#define TYPE_POISON                  (TYPE_HIT + 15)
#define TYPE_SLEEP                   (TYPE_HIT + 16)
#define TYPE_COLD                    (TYPE_HIT + 17)
#define TYPE_PARA                    (TYPE_HIT + 18)
#define TYPE_KI                      (TYPE_HIT + 19)
#define TYPE_SONG                    (TYPE_HIT + 20)
#define TYPE_PHYSICAL_MAGIC          (TYPE_HIT + 21)
#define TYPE_WATER                   (TYPE_HIT + 22)
// If you change this, update strs_damage_types[] in const.cpp
////////////////////////////////////////////////////////////////

#endif