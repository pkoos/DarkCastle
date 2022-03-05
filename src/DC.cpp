/*
 * DC.cpp
 *
 *  Created on: Dec 24, 2017
 *      Author: jhhudso
 *      Based on http://www.cplusplus.com/forum/beginner/152735/#msg792909
 */

#include <unistd.h>

#include "DC.h"
#include "room.h" // NOWHERE
#include "db.h"

string DC::version = VERSION;

DC::DC() {

}

DC& DC::instance()
{
	static DC DC_instance { /* ... */};
	return DC_instance;
}

void DC::removeDead(void) {

	for (auto& node : death_list)
	{
		character_list.erase(node.first);
		shooting_list.erase(node.first);
		Trace& t = node.second;
		t.addTrack("DC::removeDeath");
		free_char(node.first, t);
	}
	death_list.clear();

	while (!obj_free_list.empty()) {
	  obj_data *obj = *(obj_free_list.cbegin());
	  active_obj_list.erase(obj);
	  obj_free_list.erase(obj);
    delete obj;
	}
}

void DC::handleShooting(void) {
	unordered_set<char_data *> remove_list;

	for (auto &ch : shooting_list) {
		// ignore the dead
		if (ch->in_room == NOWHERE) {
			continue;
		}

		if (ch->shotsthisround) {
			ch->shotsthisround--;
		} else {
			remove_list.insert(ch);
		}
	}

	for (auto &ch : remove_list) {
		shooting_list.erase(ch);
	}
}

string DC::getVersion(void)
{
  return version;
}

string DC::getBuildTime(void)
{
  return string(BUILD_TIME);
}

void DC::init_game(void)
{

#ifdef LEAK_CHECK
  void remove_all_mobs_from_world();
  void remove_all_objs_from_world();
  void clean_socials_from_memory();
  void free_clans_from_memory();
  void free_world_from_memory();
  void free_mobs_from_memory();
  void free_objs_from_memory();
  void free_messages_from_memory();
  void free_hsh_tree_from_memory();
  void free_wizlist_from_memory();
  void free_game_portals_from_memory();
  void free_help_from_memory();
  void free_zones_from_memory();
  void free_shops_from_memory();
  void free_emoting_objects_from_memory();
  void free_command_radix_nodes(struct cmd_hash_info * curr);
  void free_ban_list_from_memory();
  void free_buff_pool_from_memory();
  extern cmd_hash_info *cmd_radix;
#endif

  FILE *fp;
  // create boot'ing lockfile
  if ((fp = fopen("died_in_bootup", "w")))
  {
    fclose(fp);
  }

  log("Attempting to load hotboot file.", 0, LOG_MISC);

  if (load_hotboot_descs())
  {
    log("Hotboot Loading complete.", 0, LOG_MISC);
    was_hotboot = 1;
  }
  else
  {
    log("Hotboot failed.  Starting regular sockets.", 0, LOG_MISC);
    log("Opening mother connections.", 0, LOG_MISC);

    for_each(cf.ports.begin(), cf.ports.end(), [this](in_port_t &port)
             {
               logf(0, LOG_MISC, "Opening port %d.", port);
               int listen_fd = init_socket(port);
               if (listen_fd >= 0)
               {
                 server_descriptor_list.insert(listen_fd);
               }
               else
               {
                 logf(0, LOG_MISC, "Error opening port %d.", port);
               }
             });
  }

  start_time = time(0);
  boot_db();

  if (was_hotboot)
  {
    log("Connecting hotboot characters to their descriptiors", 0, LOG_MISC);
    finish_hotboot();
  }

  log("Signal trapping.", 0, LOG_MISC);
  signal_setup();

  // we got all the way through, let's turn auto-hotboot back on
  try_to_hotboot_on_crash = 1;

  log("Entering game loop.", 0, LOG_MISC);

  unlink("died_in_bootup");

  if (DC::instance().cf.bport == false)
  {
    do_not_save_corpses = 0;
  }
  game_loop();
  do_not_save_corpses = 1;

  log("Closing all sockets.", 0, LOG_MISC);
  while (descriptor_list)
  {
    close_socket(descriptor_list);
  }

  for_each(server_descriptor_list.begin(), server_descriptor_list.end(), [](const int &fd)
           {
             logf(0, LOG_MISC, "Closing fd %d.", fd);
             CLOSE_SOCKET(fd);
           });
#ifdef LEAK_CHECK

  log("Freeing all mobs in world.", 0, LOG_MISC);
  remove_all_mobs_from_world();
  log("Freeing all objs in world.", 0, LOG_MISC);
  remove_all_objs_from_world();
  log("Freeing socials from memory.", 0, LOG_MISC);
  clean_socials_from_memory();
  log("Freeing zones data.", 0, LOG_MISC);
  free_zones_from_memory();
  log("Freeing clan data.", 0, LOG_MISC);
  free_clans_from_memory();
  log("Freeing the world.", 0, LOG_MISC);
  free_world_from_memory();
  log("Freeing mobs from memory.", 0, LOG_MISC);
  free_mobs_from_memory();
  log("Freeing objs from memory.", 0, LOG_MISC);
  free_objs_from_memory();
  log("Freeing messages from memory.", 0, LOG_MISC);
  free_messages_from_memory();
  log("Freeing hash tree from memory.", 0, LOG_MISC);
  free_hsh_tree_from_memory();
  log("Freeing wizlist from memory.", 0, LOG_MISC);
  free_wizlist_from_memory();
  log("Freeing help index.", 0, LOG_MISC);
  free_help_from_memory();
  log("Freeing shops from memory.", 0, LOG_MISC);
  free_shops_from_memory();
  log("Freeing emoting objects from memory.", 0, LOG_MISC);
  free_emoting_objects_from_memory();
  log("Freeing game portals from memory.", 0, LOG_MISC);
  free_game_portals_from_memory();
  log("Freeing command radix from memory.", 0, LOG_MISC);
  free_command_radix_nodes(cmd_radix);
  log("Freeing ban list from memory.", 0, LOG_MISC);
  free_ban_list_from_memory();
  log("Freeing the bufpool.", 0, LOG_MISC);
  free_buff_pool_from_memory();
  DC::instance().removeDead();
#endif

  log("Goodbye.", 0, LOG_MISC);
  log("Normal termination of game.", 0, LOG_MISC);
}
