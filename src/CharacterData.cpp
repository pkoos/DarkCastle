#include "CharacterData.h"

class CharacterData {
    public:
        struct mob_data * mobdata;
        struct pc_data *pcdata;
        struct obj_data *objdata;

        struct descriptor_data *desc;

        char *name;
        char *short_desc;
        char *long_desc;
        char *description;
        char *title;

        sbyte sex;
        sbyte c_class;
        sbyte race;
        sbyte level;
        sbyte position;

        sbyte str;
        sbyte intel;
        sbyte wis;
        sbyte dex;
        sbyte con;

        sbyte conditions[3]; // what is this magic number?

        ubyte weight;
        ubyte height;

        int16 hometown;
        int64 gold;
        uint32 plat;
        int64 exp;

        int32 in_room;

        uint32 immune;
        uint32 resist;
        uint32 suscept;
        int16 saves[SAVE_TYPE_MAX+1];

        int32 mana;
        int32 max_mana;
        int32 raw_mana;
        int32 hit;
        int32 max_hit;
        int32 raw_hit;
        int32 move;
        int32 raw_move;
        int32 max_move;
        int32 ki;
        int32 max_ki;
        int32 raw_ki;

        int16 alignment;

        uint32 hpmetas;             // total number of times meta'd hps
        uint32 manametas;           // total number of times meta'd mana
        uint32 movemetas;           // total number of times meta'd moves
        uint32 acmetas;             // total number of times meta'd ac
        int32 agemetas;            // number of years age has been meta'd

        int16 hit_regen;           // modifier to hp regen
        int16 mana_regen;          // modifier to mana regen
        int16 move_regen;          // modifier to move regen
        int16 ki_regen;            // modifier to ki regen

        int16 melee_mitigation;    // modifies melee damage
        int16 spell_mitigation;    // modified spell damage
        int16 song_mitigation;     // modifies song damage
        int16 spell_reflect;

        intptr_t clan;                       /* Clan the char is in */

        int16 armor;                 // Armor class
        int16 hitroll;               // Any bonus or penalty to the hit roll
        int16 damroll;               // Any bonus or penalty to the damage roll

        int16 glow_factor;           // Amount that the character glows

        obj_data * beacon;       /* pointer to my beacon */

        std::vector<songInfo> songs;          // Song list
//     int16 song_timer;       /* status for songs being sung */
//     int16 song_number;      /* number of song being sung */
//     char * song_data;        /* args for the songs */

    struct obj_data *equipment[MAX_WEAR]; // Equipment List

    struct char_skill_data * skills;   // Skills List
    struct affected_type *affected;    // Affected by list
    struct obj_data *carrying;         // Inventory List

     int16 poison_amount;              // How much poison damage I'm taking every few seconds

     int16 carry_weight;               // Carried weight
     int16 carry_items;                // Number of items carried                

    char *hunting;                     // Name of "track" target
    char *ambush;                      // Name of "ambush" target

    CharacterData * guarding;              // Pointer to who I am guarding
    follow_type * guarded_by;          // List of people guarding me

    uint32 affected_by[AFF_MAX/ASIZE+1]; // Quick reference bitvector for spell affects
    uint32 combat;                     // Bitvector for combat related flags (bash, stun, shock)
    uint32 misc;                       // Bitvector for IS_MOB/logs/channels.  So possessed mobs can channel

    CHAR_DATA *fighting;                 /* Opponent     */
    CHAR_DATA *next;                     /* Next anywhere in game */
    CHAR_DATA *next_in_room;             /* Next in room */
    CHAR_DATA *next_fighting;            /* Next fighting */
    OBJ_DATA *altar;
    struct follow_type *followers;  /* List of followers */
    CHAR_DATA *master;              /* Who is char following? */
    char *group_name;                /* Name of group */
    
    int32 timer;                         // Timer for update
    int32 shotsthisround;                // Arrows fired this round
    int32 spellcraftglyph;               // Used for spellcraft glyphs
    bool  changeLeadBonus;
    int32 curLeadBonus;
    int   cRooms;			// number of rooms consecrated/desecrated

// TODO - see if we can move the "wait" timer from desc to char
// since we need something to lag mobs too

    int32 deaths;                   /* deaths is reused for mobs as a
                                       timer to check for WAIT_STATE */

    int cID; // character ID

    struct timer_data *timerAttached;
    struct tempvariable *tempVariable;
    int spelldamage;
#ifdef USE_SQL
    int player_id;
#endif
    int spec;

    struct room_direction_data *brace_at, *brace_exit; //exits affected by brace
    void tell_history(std::string message);
    time_t first_damage;
    uint64_t damage_done;
    uint64_t damages;
    time_t last_damage;
    uint64_t damage_per_second;
    void setPOSFighting();
    int32 getHP(void);
    void setHP(int hp, CharacterData* causer = nullptr);
    void addHP(int hp, CharacterData* causer = nullptr);
    void removeHP(int dam, CharacterData* causer = nullptr);
    void fillHP(void);
    void fillHPLimit(void);
    void send(std::string);
};


//      intptr_t clan;                       /* Clan the char is in */

//      int16 armor;                 // Armor class
//      int16 hitroll;               // Any bonus or penalty to the hit roll
//      int16 damroll;               // Any bonus or penalty to the damage roll

//      int16 glow_factor;           // Amount that the character glows

//     obj_data * beacon;       /* pointer to my beacon */

//      std::vector<songInfo> songs;          // Song list
// //     int16 song_timer;       /* status for songs being sung */
// //     int16 song_number;      /* number of song being sung */
// //     char * song_data;        /* args for the songs */

//     struct obj_data *equipment[MAX_WEAR]; // Equipment List

//     struct char_skill_data * skills;   // Skills List
//     struct affected_type *affected;    // Affected by list
//     struct obj_data *carrying;         // Inventory List

//      int16 poison_amount;              // How much poison damage I'm taking every few seconds

//      int16 carry_weight;               // Carried weight
//      int16 carry_items;                // Number of items carried                

//     char *hunting;                     // Name of "track" target
//     char *ambush;                      // Name of "ambush" target

//     char_data * guarding;              // Pointer to who I am guarding
//     follow_type * guarded_by;          // List of people guarding me

//     uint32 affected_by[AFF_MAX/ASIZE+1]; // Quick reference bitvector for spell affects
//     uint32 combat;                     // Bitvector for combat related flags (bash, stun, shock)
//     uint32 misc;                       // Bitvector for IS_MOB/logs/channels.  So possessed mobs can channel

//     CHAR_DATA *fighting;                 /* Opponent     */
//     CHAR_DATA *next;                     /* Next anywhere in game */
//     CHAR_DATA *next_in_room;             /* Next in room */
//     CHAR_DATA *next_fighting;            /* Next fighting */
//     OBJ_DATA *altar;
//     struct follow_type *followers;  /* List of followers */
//     CHAR_DATA *master;              /* Who is char following? */
//     char *group_name;                /* Name of group */
    
//     int32 timer;                         // Timer for update
//     int32 shotsthisround;                // Arrows fired this round
//     int32 spellcraftglyph;               // Used for spellcraft glyphs
//     bool  changeLeadBonus;
//     int32 curLeadBonus;
//     int   cRooms;			// number of rooms consecrated/desecrated

// // TODO - see if we can move the "wait" timer from desc to char
// // since we need something to lag mobs too

//     int32 deaths;                   /* deaths is reused for mobs as a
//                                        timer to check for WAIT_STATE */

//     int cID; // character ID

//     struct timer_data *timerAttached;
//     struct tempvariable *tempVariable;
//     int spelldamage;
// #ifdef USE_SQL
//     int player_id;
// #endif
//     int spec;

//     struct room_direction_data *brace_at, *brace_exit; //exits affected by brace
//     void tell_history(string message);
//     time_t first_damage;
//     uint64_t damage_done;
//     uint64_t damages;
//     time_t last_damage;
//     uint64_t damage_per_second;
//     void setPOSFighting();
//     int32 getHP(void);
//     void setHP(int hp, char_data* causer = nullptr);
//     void addHP(int hp, char_data* causer = nullptr);
//     void removeHP(int dam, char_data* causer = nullptr);
//     void fillHP(void);
//     void fillHPLimit(void);
//     void send(string);
// }

// struct char_data
// {
//     struct mob_data * mobdata;
//     struct pc_data * pcdata;
//     struct obj_data * objdata;

//     struct descriptor_data *desc;       // NULL normally for mobs 

//     char *name;         // Keyword 'kill X'
//     char *short_desc;   // Action 'X hits you.'
//     char *long_desc;    // For 'look room'
//     char *description;  // For 'look mob'
//     char *title;

//     sbyte sex;
//     sbyte c_class;
//     sbyte race;
//     sbyte level;
//     sbyte position;      // Standing, sitting, fighting

//     sbyte str;         sbyte raw_str;        sbyte str_bonus;
//     sbyte intel;       sbyte raw_intel;      sbyte intel_bonus;
//     sbyte wis;         sbyte raw_wis;        sbyte wis_bonus;
//     sbyte dex;         sbyte raw_dex;        sbyte dex_bonus;
//     sbyte con;         sbyte raw_con;        sbyte con_bonus;

//     sbyte conditions[3];      // Drunk full etc.                       

//     ubyte weight;       /* PC/NPC's weight */
//     ubyte height;       /* PC/NPC's height */

//     int16 hometown;      /* PC/NPC home town */
//     int64 gold;           /* Money carried                           */
//     uint32 plat;           /* Platinum                                */
//      int64 exp;            /* The experience of the player            */
// 			   /* Changed to a long long */
//      int32 in_room;

//     uint32 immune;         // Bitvector of damage types I'm immune to
//     uint32 resist;         // Bitvector of damage types I'm resistant to
//     uint32 suscept;        // Bitvector of damage types I'm susceptible to
//      int16 saves[SAVE_TYPE_MAX+1];  // Saving throw bonuses

//      int32 mana;         
//      int32 max_mana;     /* Not useable                             */
//      int32 raw_mana;     /* before int bonus                        */
//      int32 hit;   
//      int32 max_hit;      /* Max hit for NPC                         */
//      int32 raw_hit;      /* before con bonus                        */
//      int32 move;  
//      int32 raw_move;
//      int32 max_move;     /* Max move for NPC                        */
//      int32 ki;
//      int32 max_ki;
//      int32 raw_ki;

//      int16 alignment;          // +-1000 for alignments                 

//     uint32 hpmetas;             // total number of times meta'd hps
//     uint32 manametas;           // total number of times meta'd mana
//     uint32 movemetas;           // total number of times meta'd moves
//     uint32 acmetas;             // total number of times meta'd ac
//      int32 agemetas;            // number of years age has been meta'd

//      int16 hit_regen;           // modifier to hp regen
//      int16 mana_regen;          // modifier to mana regen
//      int16 move_regen;          // modifier to move regen
//      int16 ki_regen;            // modifier to ki regen

//      int16 melee_mitigation;    // modifies melee damage
//      int16 spell_mitigation;    // modified spell damage
//      int16 song_mitigation;     // modifies song damage
//      int16 spell_reflect;