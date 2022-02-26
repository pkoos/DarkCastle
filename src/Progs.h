#ifndef PROGS_H_
#define PROGS_H_

#define ERROR_PROG        -1
#define IN_FILE_PROG       0
#define ACT_PROG           1
#define SPEECH_PROG        2
#define RAND_PROG          4
#define FIGHT_PROG         8
#define DEATH_PROG        16
#define HITPRCNT_PROG     32
#define ENTRY_PROG        64
#define GREET_PROG       128
#define ALL_GREET_PROG   256
#define GIVE_PROG        512
#define BRIBE_PROG      1024
#define CATCH_PROG      2048
#define ATTACK_PROG     4096
#define ARAND_PROG      8192
#define LOAD_PROG      16384
#define COMMAND_PROG   16384 <<1
#define WEAPON_PROG    16384 <<2
#define ARMOUR_PROG    16384 <<3
#define CAN_SEE_PROG   16384 <<4
#define DAMAGE_PROG    16384 <<5
#define MPROG_MAX_TYPE_VALUE (16384 << 6)

#endif