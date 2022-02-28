#ifndef INVENTORY_H_
#define INVENTORY_H_

void get(struct char_data *ch, struct obj_data *obj_object, struct obj_data *sub_object, bool has_consent, int cmd);
void log_sacrifice(CHAR_DATA *ch, OBJ_DATA *obj, bool decay);
int search_char_for_item_count(char_data * ch, int16 item_number, bool wearonly);
struct obj_data * search_char_for_item(char_data * ch, int16 item_number, bool wearonly);

#endif