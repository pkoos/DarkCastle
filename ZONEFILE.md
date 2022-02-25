# Zone File Creation

Here is what the information in a zone file means: 

```
V2
#zone_num
zone_name
top lifespan reset_mode zone_flags continent
M if_flag mob_num proc_value room_num comment
O if_flag obj_num proc_value room_num comment
P if_flag obj_num1 proc_value obj_num2 comment
G if_flag obj_num proc_value mob_num comment
E if_flag obj_num proc_value wear_location comment
D 
% if_flag base_chance max_chance
S
$~
```

## <ins>`base_chance`
The base chance for something to happen.

## <ins>`command`
M: Spawn Mob  
O: Spawn Object  
P: Put object in another object  
G: Give object to previously-spawned mob  
E: Equip object to previously-spawned mob  
D: Set door state  
%: Percent chance next entry will happen  

## <ins>`comment`</ins> - optional
A comment explaining what the line does

## <ins>`continent` - db.h
The number of the continent for this zone. 

## <ins>`if_flag`
The if_flag determines when it's line will occur.

0:		(always)  
1:		(ontrue)  
2:		(onfalse)  
3:		(onboot)  
4:		(if-last-mob-true)  
5:		(if-last-mob-false)  
6:		(if-last-obj-true)  
7:		(if-last-obj-false)  
8:		(if-last-%%-true)  
9:		(if-last-%%-false)  

## <ins>`lifespan`
The amount of time between resets, in minutes.

## <ins>`max_chance`
The max chance for something to happen.

## <ins>`mob_num`
The id of the mob.

## <ins>`proc_value`
If there is a proc related to the line, this defines what it is.

-1:		do nothing  
 1: 	act_prog  
 2: 	speech_prog  
 3: 	rand_prog  
 4:		fight_prog  
 5:		death_prog  
 6:		hitprcnt_prog  
 7:		entry_prog  
 8:		greet_prog  
 9:		all_greet_prog  
10:		give_prog  
11:		bribe_prog  
12:		catch_prog  
13:		attack_prog  
14:		arand_prog  
15:		load_prog  
16:		can_see_prog  
17:		damage_prog  

## <ins>`reset_mode` - db.h
What happens when the area is reset. 

## <ins>`room_num`
The id of the room.

## <ins>`top`
The highest numbered room in the zone.

## <ins>`wear_location`</ins> - obj.h
Numerical value related to wearing an item.

## <ins>`zone_flags` - db.h
A bitvector of the flags that apply to the entire zone.

## <ins>`zone_name` 
The name of the zone

## <ins>`zone_num`
Not sure what this number is, doesn't seem to be used anywhere.