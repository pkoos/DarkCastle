#ifndef SPELL_IDS_H_
#define SPELL_IDS_H_

/*
 * Spell numbers are well known.
 * They appear in tinyworld files attached to magical items.
 * Spells and skills share the same address space (for practicing)
 *   and also for special damage messages as attack types.
 */
#define TYPE_UNDEFINED               -1
#define SPELL_RESERVED_DBC            0
#define SPELL_ARMOR                   1
#define SPELL_TELEPORT                2
#define SPELL_BLESS                   3
#define SPELL_BLINDNESS               4
#define SPELL_BURNING_HANDS           5
#define SPELL_IRIDESCENT_AURA         6
#define SPELL_CHARM_PERSON            7
#define SPELL_CHILL_TOUCH             8
#define SPELL_CLONE                   9
#define SPELL_COLOUR_SPRAY           10
#define SPELL_CONTROL_WEATHER        11
#define SPELL_CREATE_FOOD            12
#define SPELL_CREATE_WATER           13
#define SPELL_REMOVE_BLIND           14
#define SPELL_CURE_CRITIC            15
#define SPELL_CURE_LIGHT             16
#define SPELL_CURSE                  17
#define SPELL_DETECT_EVIL            18
#define SPELL_DETECT_INVISIBLE       19
#define SPELL_DETECT_MAGIC           20
#define SPELL_DETECT_POISON          21
#define SPELL_DISPEL_EVIL            22
#define SPELL_EARTHQUAKE             23
#define SPELL_ENCHANT_WEAPON         24
#define SPELL_ENERGY_DRAIN           25
#define SPELL_FIREBALL               26
#define SPELL_HARM                   27
#define SPELL_HEAL                   28
#define SPELL_INVISIBLE              29
#define SPELL_LIGHTNING_BOLT         30
#define SPELL_LOCATE_OBJECT          31
#define SPELL_MAGIC_MISSILE          32
#define SPELL_POISON                 33
#define SPELL_PROTECT_FROM_EVIL      34
#define SPELL_REMOVE_CURSE           35
#define SPELL_SANCTUARY              36
#define SPELL_SHOCKING_GRASP         37
#define SPELL_SLEEP                  38
#define SPELL_STRENGTH               39
#define SPELL_SUMMON                 40
#define SPELL_VENTRILOQUATE          41
#define SPELL_WORD_OF_RECALL         42
#define SPELL_REMOVE_POISON          43
#define SPELL_SENSE_LIFE             44
#define SPELL_SUMMON_FAMILIAR        45
#define SPELL_LIGHTED_PATH           46
#define SPELL_RESIST_ACID            47
#define SPELL_SUN_RAY                48
#define SPELL_RAPID_MEND             49
#define SPELL_ACID_SHIELD            50
#define SPELL_WATER_BREATHING        51
#define SPELL_GLOBE_OF_DARKNESS      52
#define SPELL_IDENTIFY               53
#define SPELL_ANIMATE_DEAD           54
#define SPELL_FEAR                   55
#define SPELL_FLY                    56
#define SPELL_CONT_LIGHT             57
#define SPELL_KNOW_ALIGNMENT         58
#define SPELL_DISPEL_MAGIC           59
#define SPELL_CONJURE_ELEMENTAL      60
#define SPELL_CURE_SERIOUS           61
#define SPELL_CAUSE_LIGHT            62
#define SPELL_CAUSE_CRITICAL         63
#define SPELL_CAUSE_SERIOUS          64
#define SPELL_FLAMESTRIKE            65
#define SPELL_STONE_SKIN             66
#define SPELL_SHIELD                 67
#define SPELL_WEAKEN                 68
#define SPELL_MASS_INVISIBILITY      69
#define SPELL_ACID_BLAST             70
#define SPELL_PORTAL                 71
#define SPELL_INFRAVISION            72
#define SPELL_REFRESH                73
#define SPELL_HASTE		     74
#define SPELL_DISPEL_GOOD            75
#define SPELL_HELLSTREAM             76
#define SPELL_POWER_HEAL             77
#define SPELL_FULL_HEAL              78
#define SPELL_FIRESTORM              79
#define SPELL_POWER_HARM             80
#define SPELL_DETECT_GOOD            81
#define SPELL_VAMPIRIC_TOUCH         82
#define SPELL_LIFE_LEECH             83
#define SPELL_PARALYZE               84
#define SPELL_REMOVE_PARALYSIS       85
#define SPELL_FIRESHIELD             86
#define SPELL_METEOR_SWARM           87
#define SPELL_WIZARD_EYE             88
#define SPELL_TRUE_SIGHT             89
#define SPELL_MANA                   90
#define SPELL_SOLAR_GATE             91
#define SPELL_HEROES_FEAST           92
#define SPELL_HEAL_SPRAY             93
#define SPELL_GROUP_SANC             94
#define SPELL_GROUP_RECALL           95
#define SPELL_GROUP_FLY              96
#define SPELL_ENCHANT_ARMOR          97
#define SPELL_RESIST_FIRE            98
#define SPELL_RESIST_COLD            99
#define SPELL_BEE_STING	            100
#define SPELL_BEE_SWARM	            101
#define SPELL_CREEPING_DEATH	    102
#define SPELL_BARKSKIN	            103
#define SPELL_HERB_LORE	            104
#define SPELL_CALL_FOLLOWER	    105
#define SPELL_ENTANGLE	            106
#define SPELL_EYES_OF_THE_OWL	    107
#define SPELL_FELINE_AGILITY        108
#define SPELL_FOREST_MELD	    109
#define SPELL_COMPANION	            110
#define SPELL_DROWN	            111
#define SPELL_HOWL	            112
#define SPELL_SOULDRAIN	            113
#define SPELL_SPARKS	            114
#define SPELL_CAMOUFLAGE            115
#define SPELL_FARSIGHT              116
#define SPELL_FREEFLOAT             117
#define SPELL_INSOMNIA              118
#define SPELL_SHADOWSLIP            119
#define SPELL_RESIST_ENERGY         120
#define SPELL_STAUNCHBLOOD          121
#define SPELL_CREATE_GOLEM	    122
#define SPELL_REFLECT               123
#define SPELL_DISPEL_MINOR          124
#define SPELL_RELEASE_GOLEM         125
#define SPELL_BEACON                126
#define SPELL_STONE_SHIELD          127
#define SPELL_GREATER_STONE_SHIELD  128
#define SPELL_IRON_ROOTS            129
#define SPELL_EYES_OF_THE_EAGLE     130
#define SPELL_MISANRA_QUIVER        131
#define SPELL_ICESTORM              132
#define SPELL_LIGHTNING_SHIELD      133
#define SPELL_BLUE_BIRD             134
#define SPELL_DEBILITY              135
#define SPELL_ATTRITION             136
#define SPELL_VAMPIRIC_AURA         137
#define SPELL_HOLY_AURA             138
#define SPELL_DISMISS_FAMILIAR      139
#define SPELL_DISMISS_CORPSE        140
#define SPELL_BLESSED_HALO          141
#define SPELL_VISAGE_OF_HATE        142
#define SPELL_PROTECT_FROM_GOOD     143
#define SPELL_OAKEN_FORTITUDE	    144
#define SPELL_FROSTSHIELD	    145
#define SPELL_STABILITY		    146
#define SPELL_KILLER 		    147
#define SPELL_CANTQUIT 		    148
#define SPELL_SOLIDITY 		    149
#define SPELL_EAS 		    150
#define SPELL_ALIGN_GOOD 	    151 // uriel's fire of redemption
#define SPELL_ALIGN_EVIL 	    152 // Moruk's heart
#define SPELL_AEGIS 		    153
#define SPELL_U_AEGIS               154
#define SPELL_RESIST_MAGIC          155
#define SPELL_EAGLE_EYE             156
#define SPELL_CALL_LIGHTNING	    157
#define SPELL_DIVINE_FURY           158
#define SPELL_GHOSTWALK		    159
#define SPELL_MEND_GOLEM            160
#define SPELL_CLARITY               161
#define SPELL_DIVINE_INTER	    162
#define SPELL_WRATH_OF_GOD	    163
#define SPELL_ATONEMENT             164
#define SPELL_SILENCE               165
#define SPELL_IMMUNITY              166
#define SPELL_BONESHIELD            167
#define SPELL_CHANNEL               168
#define SPELL_RELEASE_ELEMENTAL     169
#define SPELL_WILD_MAGIC	    170
#define SPELL_SPIRIT_SHIELD	    171
#define SPELL_VILLAINY		    172
#define SPELL_HEROISM		    173
#define SPELL_CONSECRATE	    174
#define SPELL_DESECRATE		    175
#define SPELL_ELEMENTAL_WALL        176
#define SPELL_ETHEREAL_FOCUS        177

// if you add a spell, make sure you update "spells[]" in spells.C

/*
 * 150 to 249 reserved for more spells.
 */

#endif