# Mobile Creation Documentation

Here is what an entry in the .mob file looks like: 
```
#mob_num
name~
short_desc~
long_desc~
description~
action_bits1 action_bits2 -1
affect_bits1 affect_bits2 affect_bits3 -1
alignment race level
hitroll armor max_hit dam_dice
gold experience
position default_position sex immune suscept resist
C class_num (optional line)
T str dex con intel wis 0 (optional)
progs
|
Y flag_type
	V index flag_value
S
```
## <ins>`mob_num`
This is the id of the mob you created.
## <ins>`name`
The name used to target the mobile. 'Kill x'
## <ins>`short_desc`
What a player sees when the mob does something. 'x hits you.`
## <ins>`long_desc`
What you see when you look in a room.
## <ins>`description`
What you see when you look at a mob.

## <ins>`action_bits1, action_bits2`</ins> - mobile.h
A bitvector for action skills, broken up into groups of 32.  

action_bits1 -> for actions  1 - 32  
action_bits2 -> for actions 33 - 37  


## <ins> `affect_bits1, affect_bits2, affect_bits3` </ins>  - affect.h

A bitvector of affects, broken up into groups of 32.

affected_bits1 -> for affects  1 - 32  
affected_bits2 -> for affects 33 - 64  
affected_bits3 -> for affects 65 - 73  

	alignment
-----------------------
	the alignment of the mob, from -1000 (evil) to 1000 (good)

	race - race.h
-----------------------
The number denoting race. This is found in race.h under RACE_

	level
-----------------------
The level of the mob

	hitroll
-----------------------
	Use the following formula: (20 - hitroll)

	armor
-----------------------
	Use the following formula: (int)(armor / 10)

	max_hit - hit points of a mob
-----------------------
	The following string: (max_hit)d1+0

	dam_dice
-----------------------
	The following string: (numdice)d(dicesize)+(damroll)
	
	gold
-----------------------
	The amount of gold the mob carries.

	experience
-----------------------
	The amount of experience a mob gives when you kill it.k

	immune
-----------------------
	Bitvector of immunities. Located in isr.h
	
	suscept
-----------------------
	Bitvector of weaknesses. Located in isr.h

	resist
-----------------------
	bitvector of resistances. Located in isr.h