# <ins>Object Creation</ins>

The following is documentation on how to create custom objects for Dark Castle.

First off, here is the format for an object: 

```
#onum
name~
short_desc~
desc~
action_description~
type_flag extra_flags wear_flags size
v1 v2 v3 v4 eq_level
weight cost more_flags
E
currkeyword~
currdesc
~
A
location modifier
S
```

This is the end of file marker: 

```
$~
```

## <ins>`act_desc`
What displays when the item is used. Optional, with 'Fixed.' appearing when it is not included.

## <ins>`cost`
The cost of the item

## <ins>`curr_desc`
This is extra description, and is an optional section. This only appears if there is extra description is completed.

## <ins>`curr_keyword`
This is extra description, and is an optional section. This only appears if there is extra description, and is the keyword to see the extra description.

These two sections are not unique, you can have many different locations and modifiers, each under its own A from above.

## <ins>`desc`
What you see when the item is in a room.

## <ins>`eq_level`
The level of the item.

## <ins>`extra_flags` - obj.h
A bitvector for item extras. 

## <ins>`location`</ins> - player.h
This is an apply type, and can be searched with APPLY_ in the above file.

## <ins>`modifier`
The amount to give to the apply. 

## <ins>`more_flags`</ins> - obj.h
A bitvector of other optional flags.

## <ins>`name`
The name used to pick up the item. Can be multiple words, separated by spaces.

## <ins>`onum`</ins>
the id given in the .obj file

## <ins>`short_desc`
What appears when the item is worn, in your inventory, etc.

## <ins>`size`</ins> - obj.h
The bitvector for the item size.

## <ins>`type_flag` - obj.h
An integer based on the item type.

## <ins>`v1 - v4`
What these do depends on the item type.

### <ins>ITEM_LIGHT       1
v1 - Colour  
v2 - Type  
v3 - hours  
v4 - unused
### <ins>ITEM_SCROLL      2
v1 - level  
v2 - Spell  
v3 - Spell  
v4 - Spell  
### <ins>ITEM_WAND        3
### <ins>ITEM_STAFF       4
v1 - Level  
v2 - Total Charges  
v3 - Current Charges  
v4 - Spell  
### <ins>ITEM_WEAPON      5
v1 - Unused  
v2 - dicenum  
v3 - dicesize  
v4 - Weapon type  
### <ins>ITEM_FIREWEAPON  6
v1 - hitroll  
v2 - dicenum  
v3 - dicesize  
v4 - weapon type  
### <ins>ITEM_MISSILE     7
v1 - dicenum  
v2 - dicesize  
v3 - hitroll  
v4 - damroll  
### <ins>ITEM_ARMOR       9
v1 - ac apply  
v2 - unused  
v3 - unused  
v4 - unused  
### <ins>ITEM_POTION     10
v1 - level  
v2 - spell  
v3 - spell  
v4 - spell  
### <ins>ITEM_TRAP       14 
v1 - spell  
v2 - hitpoints  
v3 - unused  
v4 - unused  
### <ins>ITEM_CONTAINER  15
v1 - max contains  
v2 - lock type  
v3 - key  
v4 - corpse  
### <ins>ITEM_NOTE       16
v1 - Tounge  
v2 - unused  
v3 - unused  
v4 - unused  
### <ins>ITEM_DRINKCON   17
v1 - max contains  
v2 - contains  
v3 - liquid  
v4 - poisoned  
### <ins>ITEM_KEY        18
v1 - keytype  
v2 - unused  
v3 - unused  
v4 - unused  
### <ins>ITEM_FOOD       19
v1 - makes full  
v2 - unused  
v3 - unused  
v4 - poisoned  
### <ins>ITEM_PORTAL     24
v1 - to room  
v2 - type  
v3 - zone  
v4 - flags   
### <ins>ITEM_INSTRUMENT 26
v1 - non-combat effect  
v2 - combat effect  
v3 - unused  
v4 - unused  
### <ins>ITEM_UTILITY    27
v1 - utility type  
#### <ins>Catstink  
v2 - sector  
v3 - sector  
v4 - how much lag  
#### <ins>Mortar
v2 - numdice  
v3 - dicesize  
v4 - how much lag  
### <ins>ITEM_MEGAPHONE  31
v1 - interval  
v2 - interval, again  
v3 - unused  
v4 - unused  

## <ins>`wear_flags` - obj.h
A bitvector for the item wear location.

## <ins>`weight`
The weight of the item