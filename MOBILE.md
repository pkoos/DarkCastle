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

## <ins>`action_bits1, action_bits2`</ins> - mobile.h
A bitvector for action skills, broken up into groups of 32.  

action_bits1 -> for actions  1 - 32  
action_bits2 -> for actions 33 - 37  

## <ins> `affect_bits1, affect_bits2, affect_bits3` </ins>  - affect.h

A bitvector of affects, broken up into groups of 32.

affected_bits1 -> for affects  1 - 32  
affected_bits2 -> for affects 33 - 64  
affected_bits3 -> for affects 65 - 73  

## <ins>`alignment`
the alignment of the mob, from -1000 (evil) to 1000 (good)

## <ins>`armor`
Use the following formula: (int)(armor / 10). Negative is better, and the ac will be translated back to the initial value in the code.

## <ins>`dam_dice`</ins>
The amount of damage the mob does. Here is the formula:  
(numdice)d(dicesize)+(damroll)

## <ins>`description`
What you see when you look at a mob.

## <ins>`experience`
The amount of experience a mob gives when you kill it.

## <ins>`gold`
The amount of gold the mob carries.

## <ins>`hitroll`
Use the following formula: (20 - hitroll)  
For hitroll, the lower the better. Anything above -20 works out to be a negative number.

## <ins>`immune`</ins> - isr.h
Bitvector of immunities.

## <ins>`level`
The level of the mob.

## <ins>`long_desc`
What you see when you look in a room.

## <ins>`max_hit`</ins>
The max hitpoints for the mob. use the following formula:  
(max_hit)d1+0

## <ins>`mob_num`
This is the id of the mob you created.

## <ins>`name`
The name used to target the mobile. 'Kill x'

## <ins>`race`</ins> - race.h
The number denoting race.
	
## <ins>`resist`</ins> - isr.h
Bitvector of resistances.

## <ins>`short_desc`
What a player sees when the mob does something. 'x hits you.`

## <ins>`suscept`</ins> - isr.h
Bitvector of weaknesses.