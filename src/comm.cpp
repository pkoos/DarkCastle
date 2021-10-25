/* 
************************************************************************
*   File: comm.c                                        Part of CircleMUD *
*  Usage: Communication, socket handling, main(), central game loop       *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/telnet.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/types.h>
#include <fmt/format.h>
#include <fmt/chrono.h>
#include <signal.h>
#include <ctype.h>
#include <cassert>



#include "terminal.h"
#include "fileinfo.h"
#include "act.h"
#include "player.h"
#include "levels.h"
#include "room.h"
#include "structs.h"
#include "utility.h"
#include "connect.h"
#include "interp.h"
#include "handler.h"
#include "db.h"
#include "comm.h"
#include "returnvals.h"
#include "quest.h"
#include "shop.h"
#include "Leaderboard.h"
#include "Timer.h"
#ifdef USE_SQL
#include "Backend/Database.h"
#endif

#include <sstream>
#include <iostream>
#include <list>
#include "Timer.h"
#include "DC.h"
#include "CommandStack.h"
#include <QStringTokenizer>
#include <QDebug>
struct multiplayer {
  char *host;
  char *name1;
  char *name2;
};

using namespace std;

#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif

extern bool MOBtrigger;

// This is turned on right before we call game_loop
int do_not_save_corpses = 1;
int try_to_hotboot_on_crash = 0;
int was_hotboot = 0;
int died_from_sigsegv = 0;

char* const* orig_argv;
/* these are here for the eventual addition of ban */
int num_invalid = 0;
int restrict = 0;

/* externs */
extern int restrict;
//extern int mini_mud;
//extern int no_rent_check;

extern CWorld world;	/* In db.c */
extern char * sector_types[];
extern char *time_look[];
extern char *sky_look[];
extern struct room_data ** world_array;
extern char last_char_name[MAX_INPUT_LENGTH];
extern char last_processed_cmd[MAX_INPUT_LENGTH];

void check_champion_and_website_who_list(void);
void save_slot_machines(void);
void check_silence_beacons(void);

/* local globals */
struct descriptor_data *descriptor_list = NULL;		/* master desc list */
struct txt_block *bufpool = 0;	/* pool of large output buffers */
int buf_largecount = 0;		/* # of large buffers which exist */
int buf_overflows = 0;		/* # of overflows of output */
int buf_switches = 0;		/* # of switches from small to large buf */
int _shutdown = 0;		/* clean shutdown */
int tics = 0;			/* for extern checkpointing */
//int nameserver_is_slow = 0;	/* see config.c */
//extern int auto_save;		/* see config.c */
//extern int autosave_time;	/* see config.c */
struct timeval null_time;	/* zero-valued time structure */
time_t start_time;

// heartbeat globals
int pulse_timer;
int pulse_mobile;
int pulse_bard;
int pulse_violence;
int pulse_tensec;
int pulse_weather;
int pulse_regen;
int pulse_time;
int pulse_short; // short timer, for archery

#ifdef USE_SQL
Database db;
#endif

/* functions in this file */
void update_mprog_throws(void);
void update_bard_singing(void);
void update_characters(void);
void short_activity();
void skip_spaces(char **string);
char *any_one_arg(char *argument, char *first_arg);
char * calc_color(int hit, int max_hit);
QString generate_prompt(CHAR_DATA *ch);
int get_from_q(struct txt_q *queue, char *dest, int *aliased);
void signal_setup(void);
int new_descriptor(int s);
int process_output(struct descriptor_data *t);
int process_input(struct descriptor_data *t);
void flush_queues(struct descriptor_data *d);
int perform_subst(struct descriptor_data *t, char *orig, char *subst);
int perform_alias(struct descriptor_data *d, char *orig);
void check_idle_passwords(void);
void init_heartbeat();
void heartbeat();
void report_debug_logging();

/* extern fcnts */
void pulse_takeover(void);
void boot_db(void);
void zone_update(void);
void affect_update(int32 duration_type);	/* In spells.c */
void point_update(void);	/* In limits.c */
void food_update(void);		/* In limits.c */
void mobile_activity(void);
void object_activity(uint64_t pulse_type);
void update_corpses_and_portals(void);
void string_hash_add(struct descriptor_data *d, char *str);
void perform_violence(void);
void show_string(struct descriptor_data *d, char *input);
int isbanned(char *hostname);
void time_update();
void weather_update();
void send_hint();
extern void pulse_command_lag();
void checkConsecrate(int);

//extern char greetings1[MAX_STRING_LENGTH];
//extern char greetings2[MAX_STRING_LENGTH];
//extern char greetings3[MAX_STRING_LENGTH];
//extern char greetings4[MAX_STRING_LENGTH];

#ifdef WIN32
void gettimeofday(struct timeval *t, struct timezone *dummy)
{
  DWORD millisec = GetTickCount();
  t->tv_sec = (int) (millisec / 1000);
  t->tv_usec = (millisec % 1000) * 1000;
}
#endif

// writes all the descriptors to file so we can open them back up after
// a reboot
int write_hotboot_file(char **new_argv) 
{
  FILE *fp;
  struct descriptor_data *d;
  struct descriptor_data *sd;
  /* Azrack -- do these need to be here?
  extern int mother_desc;
  extern int other_desc;
  extern int third_desc;
  */
  //  extern char ** ext_argv;

  if ((fp=fopen("hotboot","w"))==NULL) {   
    log("Hotboot failed, unable to open hotboot file.", 0, LOG_MISC);
    return 0; 
  }

  DC &dc = DC::instance();
  //for_each(dc.server_descriptor_list.begin(), dc.server_descriptor_list.end(), [fp](server_descriptor_list_i i)
  for_each(dc.server_descriptor_list.begin(), dc.server_descriptor_list.end(), [&fp](const int &fd)
  {
    fprintf(fp, "%d\n", fd);
  });

  for (d = descriptor_list; d; d = sd)
  {
    sd = d->next;
    if (STATE(d) != CON_PLAYING || !d->character || GET_LEVEL(d->character) < 1)
    {
      // Kick out anyone not currently playing in the game.
      write_to_descriptor(d->descriptor, "We are rebooting, come back in a minute.");
      close_socket(d);
    }
    else
    {
      STATE(d) = CON_PLAYING; // if editors.
      if (d->original)
      {
        fprintf(fp, "%d\n%s\n%s\n", d->descriptor, GET_NAME(d->original), d->host);
        if (d->original->pcdata)
        {
          if (d->original->pcdata->last_site)
          {
            delete[] d->original->pcdata->last_site;
          }

          d->original->pcdata->last_site = new char[(strlen(d->host) + 1)];
          strcpy(d->original->pcdata->last_site, d->original->desc->host);
          d->original->pcdata->time.logon = time(0);
        }
        save_char_obj(d->original);
      }
      else
      {
        fprintf(fp, "%d\n%s\n%s\n", d->descriptor, GET_NAME(d->character), d->host);
        if (d->character->pcdata)
        {
          if (d->character->pcdata->last_site)
          {
            delete[] d->character->pcdata->last_site;
          }
          
          d->character->pcdata->last_site = new char[strlen(d->host) + 1];
          strcpy(d->character->pcdata->last_site, d->character->desc->host);
          d->character->pcdata->time.logon = time(0);
        }
        save_char_obj(d->character);
      }
      write_to_descriptor(d->descriptor, "Attempting to maintain your link during reboot.\r\nPlease wait..");
    }
  }
  fclose(fp);
  log("Hotboot descriptor file successfully written.", 0, LOG_MISC);
  
  chdir("../bin/");

  char* const* argv;
  if (new_argv) {
    argv = new_argv;
  } else {
    argv = orig_argv;
  }
  
  // convert array to string
  int index = 0;
  stringstream argv_string;
  while(argv[index] != 0) {
    argv_string << argv[index++];
    argv_string << " ";
  }
  char *buffer = new char[100];
  getcwd(buffer, 99);
  logf(108, LOG_GOD, "Hotbooting %s at [%s]", argv_string.str().c_str(), buffer);

  if(-1 == execv(argv[0], argv)) {
    log("Hotboot execv call failed.", 0, LOG_MISC);
    perror(argv[0]);
    chdir(DFLT_DIR);
    unlink("hotboot"); // wipe the file since we can't use it anyway
    return 0;
  }

  return 1;
}

// attempts to read in the descs written to file, and reconnect their
// links to the mud.
int load_hotboot_descs() 
{
  FILE *fp;
  char chr[MAX_INPUT_LENGTH] = { 0 }, host[MAX_INPUT_LENGTH] = { 0 } ,buf[MAX_STRING_LENGTH*3] = { 0 };
  int desc;
  struct descriptor_data *d;
  /* Azrack - do these need to be here
  extern int mother_desc;
  extern int other_desc;
  extern int third_desc;
*/
  if ((fp=fopen("hotboot","r"))==NULL) { // Checks if it actually *is* a hotboot
    log("Hotboot file missing/unopenable.", 0, LOG_MISC);
    return 0;
  }
  log("Hotboot, reloading characters.", 0, LOG_MISC);
  unlink("hotboot"); // remove the file, it's in memory for reading anyways

  DC &dc = DC::instance();
  for_each(dc.cf.ports.begin(), dc.cf.ports.end(), [&dc, fp](in_port_t &port)
  {
    int fd;
    fscanf(fp, "%d\n", &fd);
    dc.server_descriptor_list.insert(fd);
  });



  while(!feof(fp)) {
    desc =0;
    *host = '\0';
    fscanf(fp, "%d\n%s\n%s\n", &desc, chr, host);
    d = new descriptor_data;
    memset((char *) d, 0, sizeof(struct descriptor_data));
    d->idle_time = 0;
    d->idle_tics               = 0;
    d->wait                    = 1;
    d->prompt_mode             = 1;
    d->input.head              = 0;
    d->login_time              = time(0);
    d->output                  = chr;

    if ( write_to_descriptor( desc, "Recovering...\r\n" ) == -1) {
      sprintf(buf,"Host %s Char %s Desc %d FAILED to recover from hotboot.",host,chr,desc);
      log(buf, 0, LOG_MISC);
      CLOSE_SOCKET(desc);
      dc_free(d);
      d = NULL;
      continue;
    }

    strcpy(d->host, host);
    d->descriptor              = desc;

    // we need a second to be sure
    if (-1 == write_to_descriptor(d->descriptor,"Link recovery successful.\n\rPlease wait while mud finishes rebooting...\n\r")) {
      sprintf(buf,"Host %s Char %s Desc %d failed to recover from hotboot.",host,chr,desc);
      log(buf, 0, LOG_MISC);
      CLOSE_SOCKET(desc);
      dc_free(d);
      d = NULL;
      continue; 
    }

    d->next                    = descriptor_list; 
    descriptor_list            = d;

  }
  fclose(fp);

  unlink("hotboot"); // if the above unlink failed somehow(?), 
                     // remove the hotboot file so that it dosen't think 
                     // next reboot is another hotboot
  log("Successful hotboot file read.", 0, LOG_MISC);
  return 1;
}

void finish_hotboot() 
{
  struct descriptor_data *d;
  char buf[MAX_STRING_LENGTH];

  void do_on_login_stuff(char_data * ch);

  for (d=descriptor_list;d;d=d->next)
  {
    write_to_descriptor( d->descriptor, "Reconnecting your link to your character...\r\n" );

    if (!load_char_obj(d, d->output.toStdString().c_str())) {
      sprintf(buf, "Could not load char '%s' in hotboot.",  d->output.toStdString().c_str());
      log(buf, 0, LOG_MISC);
      write_to_descriptor( d->descriptor, "Link Failed!  Tell an Immortal when you can.\n\r" );
      close_socket(d);
      continue;
    }

    write_to_descriptor( d->descriptor, "Success...May your visit continue to suck...\n\r" );

    d->output.clear();

    auto &character_list = DC::instance().character_list;
    character_list.insert(d->character);

    do_on_login_stuff(d->character);

    STATE(d) = CON_PLAYING;
  }

  for (d=descriptor_list;d;d=d->next)
 {   do_look(d->character, "", 8);
    do_save(d->character,"",666);
  }
}

/* Init sockets, run game, and cleanup sockets */
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
	extern cmd_hash_info * cmd_radix;
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
  } else
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
      } else
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



/*
 * init_socket sets up the mother descriptor - creates the socket, sets
 * its options up, binds it, and listens.
 */
int DC::init_socket(in_port_t port)
{
  int fd, opt;
  struct sockaddr_in sa;

  if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Error creating socket");
    exit(1);
  }

  opt = LARGE_BUFSIZE + GARBAGE_SPACE;
  if (setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (char *) &opt, sizeof(opt)) < 0) {
    perror("setsockopt SNDBUF");
    exit(1);
  }

  opt = 1;
  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof(opt)) < 0) {
    perror("setsockopt REUSEADDR");
    exit(1);
  }

  struct linger ld;
  ld.l_onoff = 0;
  ld.l_linger = 0;
  if (setsockopt(fd, SOL_SOCKET, SO_LINGER, (char *) &ld, sizeof(ld)) < 0) {
     perror("setsockopt LINGER");
     exit(1);
  }

  sa.sin_family = AF_INET;
  sa.sin_port = htons(port);
  sa.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(fd, (struct sockaddr *) &sa, sizeof(sa)) < 0) {
    perror("bind");
    CLOSE_SOCKET(fd);
    exit(1);
  }

  if(fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {
    perror("init_socket : fcntl : nonblock");
    exit(1);
  }

  if(listen(fd, 5) < 0) {
    perror("init_socket : listen");
    exit(1);
  }
  return fd;
}


// This is set as a global...it is the increment variable for the descriptor list
// used in game_loop.  It has to be global so that "close_socket" can increment
// it if we are closing the socket that is next to be processed.
struct descriptor_data * next_d;
stringstream timingDebugStr;
unsigned long long pulseavg = 0;

/*
 * game_loop contains the main loop which drives the entire MUD.  It
 * cycles once every 0.10 seconds and is responsible for accepting new
 * new connections, polling existing connections for input, dequeueing
 * output and sending it out to players, and calling "heartbeat" function
 */
void DC::game_loop(void)
{

	fd_set input_set, output_set, exc_set, null_set;
	struct timeval last_time, delay_time, now_time;
	long secDelta, usecDelta;	

	// comm must be much longer than MAX_INPUT_LENGTH since we allow aliases in-game
	// otherwise an alias'd command could easily overrun the buffer
	char comm[MAX_STRING_LENGTH];
	char buf[128];
	struct descriptor_data *d;
	int maxdesc;
	int aliased;

	null_time.tv_sec = 0;
	null_time.tv_usec = 0;
	FD_ZERO(&null_set);
	init_heartbeat();

	gettimeofday(&last_time, NULL);

	/* The Main Loop.  The Big Cheese.  The Top Dog.  The Head Honcho.  The.. */
	while (!_shutdown) {
		PerfTimers["gameloop"].start();
		// Set CommandStack to track a command stack with max queue depth of 20
		CommandStack cstack(0, 5);

		extern bool selfpurge;
		selfpurge = false;
		// Set up the input, output, and exception sets for select().
		FD_ZERO(&input_set);
		FD_ZERO(&output_set);
		FD_ZERO(&exc_set);

		maxdesc = 0;
    for_each(server_descriptor_list.begin(), server_descriptor_list.end(), [&input_set, &maxdesc](const int &fd)
    {
      FD_SET(fd, &input_set);
      if (fd > maxdesc) {
        maxdesc = fd;
      }
    });

		for (d = descriptor_list; d; d = d->next) {
			if(d->descriptor > maxdesc)
				maxdesc = d->descriptor;
			FD_SET(d->descriptor, &input_set);
			FD_SET(d->descriptor, &output_set);
			FD_SET(d->descriptor, &exc_set);
		}

		// poll (without blocking) for new input, output, and exceptions
		if (select(maxdesc + 1, &input_set, &output_set, &exc_set, &null_time) < 0) {
			int save_errno = errno;
			perror("game_loop : select : poll");
			if (save_errno != EINTR) {
				return;
			}
		}

    // If new connection waiting, accept it
    for_each(server_descriptor_list.begin(), server_descriptor_list.end(), [&input_set](const int &fd)
    {
      if (FD_ISSET(fd, &input_set))
      {
        new_descriptor(fd);
      }
    });

		// close the weird descriptors in the exception set
		for (d = descriptor_list; d; d = next_d) {
			next_d = d->next;
			if (FD_ISSET(d->descriptor, &exc_set)) {
				FD_CLR(d->descriptor, &input_set);
				FD_CLR(d->descriptor, &output_set);
				close_socket(d);
			}
		}

		/* process descriptors with input pending */
		for (d = descriptor_list; d; d = next_d) {
			next_d = d->next;
			if (FD_ISSET(d->descriptor, &input_set)) {
				if (process_input(d) < 0) {
					if (strcmp(d->host, "127.0.0.1")) {
						sprintf(buf, "Connection attempt bailed from %s", d->host);
						printf(buf);
						log(buf, OVERSEER, LOG_SOCKET);
					}
					close_socket(d);
				}
			}
		}

		/* process commands we just read from process_input */
		for (d = descriptor_list; d; d = next_d) {
			next_d = d->next;
			d->wait = MAX(d->wait, 1);
			if (d->connected == CON_CLOSE)
			{  close_socket(d); // So they don't have to type a command.
			continue; }
			//debugpoint();
			if ((--(d->wait) <= 0) && get_from_q(&d->input, comm, &aliased)) {
				/* reset the idle timer & pull char back from void if necessary */
				d->wait = 1;
				d->prompt_mode = 1;

				if (d->showstr_count)	/* reading something w/ pager     */
					show_string(d, comm);
				//	else if (d->str)		/* writing boards, mail, etc.     */
				//	  string_add(d, comm);
				else if (d->strnew && STATE(d) == CON_EXDSCR)
					new_string_add(d, comm);
				else if(d->hashstr)
					string_hash_add(d, comm);
				else if(d->strnew && (IS_MOB(d->character) || !IS_SET(d->character->pcdata->toggles, PLR_EDITOR_WEB)))
					new_string_add(d, comm);
				else if (d->connected != CON_PLAYING)	/* in menus, etc. */
					nanny(d, comm);
				else {			/* else: we're playing normally */
					if (aliased)		/* to prevent recursive aliases */
						d->prompt_mode = 0;
					else {
						if (perform_alias(d, comm))
							get_from_q(&d->input, comm, &aliased);
					}
					PerfTimers["command"].start();
					// Azrack's a chode.  Don't forget to check
					// ->snooping before you check snooping->char:P
					if(*comm == '%' && d->snooping && d->snooping->character) {
						command_interpreter(d->snooping->character, comm + 1);
					} else {
						command_interpreter(d->character, comm);	/* send it to interpreter */
					}
          PerfTimers["command"].stop();

				} // else if input
			} // if input
			// the below two if-statements are used to allow the mud to detect and respond
			// to web-browsers attempting to connect to the game on port 80
			// this line processes a "get" or "post" if available.  Otherwise it prints the
			// entrance screen.  If a player has already entered their name, it processes
			// that too.
			else if (d->connected == CON_DISPLAY_ENTRANCE)
				nanny(d, "");
			// this line allows the mud to skip this descriptor until next pulse
			else if (d->connected == CON_PRE_DISPLAY_ENTRANCE)
				d->connected = CON_DISPLAY_ENTRANCE;
			else d->idle_time++;
		} // for

		// do what needs to be done.  violence, repoping, regen, etc.
		PerfTimers["heartbeat"].start();
		heartbeat();
		PerfTimers["heartbeat"].stop();
	
#ifdef USE_SQL
		PerfTimers["db"].start();
		db.processqueue();
    PerfTimers["db"].stop();
#endif
    PerfTimers["output"].start();
		
		/* send queued output out to the operating system (ultimately to user) */
		for (d = descriptor_list; d; d = next_d) {
			next_d = d->next;
			if ((FD_ISSET(d->descriptor, &output_set) && d->output.count() > 0) || d->prompt_mode)
				if (process_output(d) < 0)
					close_socket(d);
			// else
			// d->prompt_mode = 1;
		}
		PerfTimers["output"].stop();
		// we're done with this pulse.  Now calculate the time until the next pulse and sleep until then
		// we want to pulse PASSES_PER_SEC times a second (duh).  This is currently 4.

		gettimeofday( &now_time, NULL );
		usecDelta   = ((int) last_time.tv_usec) - ((int) now_time.tv_usec);
		secDelta    = ((int) last_time.tv_sec ) - ((int) now_time.tv_sec );

    /*
		if (now_time.tv_sec-last_time.tv_sec > 0 || now_time.tv_usec-last_time.tv_usec > 20000) {
			timingDebugStr <<  "Time since last pulse is "
					<< (((int) now_time.tv_sec) - ((int) last_time.tv_sec)) << "sec "
					<< (((int) now_time.tv_usec) - ((int) last_time.tv_usec)) << "usec.\r\n";
			//logf(110, LOG_BUG, timingDebugStr.str().c_str());
		}
    */

		usecDelta   += (1000000 / PASSES_PER_SEC);
		while ( usecDelta < 0 )
		{
			usecDelta += 1000000;
			secDelta  -= 1;
		}
		while ( usecDelta >= 1000000 )
		{
			usecDelta -= 1000000;
			secDelta  += 1;
		}
		//logf(110, LOG_BUG, "secD : %d  usecD: %d", secDelta, usecDelta);

		if ( secDelta > 0 || ( secDelta == 0 && usecDelta > 0 ) )
		{
			delay_time.tv_usec = usecDelta;
			delay_time.tv_sec  = secDelta;
			//logf(110, LOG_BUG, "Pausing for  %dsec %dusec.", secDelta, usecDelta);
			if ( select( 0, NULL, NULL, NULL, &delay_time ) < 0 && errno != EINTR)
			{
				perror( "game_loop: select: delay" );
				exit( 1 );
			}
		}
		// temp removing this since it's spamming the crap out of us
		//else logf(110, LOG_BUG, "0 delay on pulse");
		gettimeofday(&last_time, NULL);
    PerfTimers["gameloop"].stop();
	}
}




extern void pulse_hunts(); 
extern void auction_expire();
void init_heartbeat()
{
  pulse_mobile    = PULSE_MOBILE;
  pulse_timer	  = PULSE_TIMER;
  pulse_bard      = PULSE_BARD;   
  pulse_violence  = PULSE_VIOLENCE;
  pulse_tensec    = PULSE_TENSEC;
  pulse_weather   = PULSE_WEATHER;
  pulse_regen     = PULSE_REGEN;
  pulse_time      = PULSE_TIME;
  pulse_short     = PULSE_SHORT;
}

void heartbeat() {
	if (--pulse_mobile < 1) {
		pulse_mobile = PULSE_MOBILE;

		PerfTimers["mobile"].start();
		mobile_activity();
    PerfTimers["mobile"].stop();

    PerfTimers["object"].start();
		object_activity(PULSE_MOBILE);
    PerfTimers["object"].stop();
	}
	if (--pulse_timer < 1) {
		pulse_timer = PULSE_TIMER;
		check_timer();

    PerfTimers["affect"].start();
		affect_update(PULSE_TIMER);
    PerfTimers["affect"].stop();
	}
	if (--pulse_short < 1) {
		pulse_short = PULSE_SHORT;
    PerfTimers["short"].start();
		short_activity();
    PerfTimers["short"].stop();
		pulse_command_lag();

	}
	// TODO - need to eventually modify this so it works for casters too so I can delay certain
	if (--pulse_bard < 1) {
		pulse_bard = PULSE_BARD;

    PerfTimers["bard"].start();
		update_bard_singing();
    PerfTimers["bard"].stop();

    PerfTimers["mprogthrows"].start();
		update_mprog_throws();  // convienant place to put it
    PerfTimers["mprogthrows"].stop();

    PerfTimers["camp"].start();
		update_make_camp_and_leadership();     // and this, too
    PerfTimers["camp"].stop();
	}

	if (--pulse_violence < 1) {
		pulse_violence = PULSE_VIOLENCE;

    PerfTimers["violence"].start();
		perform_violence();
		update_characters();
		affect_update(PULSE_VIOLENCE);
		check_silence_beacons();
    PerfTimers["violence"].stop();
	}

	if (--pulse_tensec < 1) {
		pulse_tensec = PULSE_TENSEC;
    PerfTimers["consecrate"].start();
		checkConsecrate(PULSE_TENSEC);
    PerfTimers["consecrate"].stop();
	}

	if (--pulse_weather < 1) {
		pulse_weather = PULSE_WEATHER;
    PerfTimers["weather"].start();
		weather_update();
    PerfTimers["weather"].stop();

    PerfTimers["auctionexp"].start();
		auction_expire();
    PerfTimers["auctionexp"].stop();
	}

	if (--pulse_regen < 1) {
    PerfTimers["pulse_regen"].start();
		// random pulse timer for regen to make tick sleeping impossible
		pulse_regen = number(PULSE_REGEN - 8 * PASSES_PER_SEC, PULSE_REGEN + 5 * PASSES_PER_SEC);    
		point_update();
		pulse_takeover();
		affect_update(PULSE_REGEN);
		checkConsecrate(PULSE_REGEN);
		if (!number(0, 2))
    {
			send_hint();
    }
    PerfTimers["pulse_regen"].stop();
	}

	if (--pulse_time < 1) {
		pulse_time = PULSE_TIME;
    PerfTimers["pulse_time"].start();

    PerfTimers["zone_update"].start();
		zone_update();
    PerfTimers["zone_update"].stop();

    PerfTimers["time_update"].start();
		time_update();
    PerfTimers["time_update"].stop();

    PerfTimers["food_update"].start();
		food_update();
    PerfTimers["food_update"].stop();

    PerfTimers["affect_update"].start();
		affect_update(PULSE_TIME);
    PerfTimers["affect_update"].stop();

    PerfTimers["update_corpses"].start();
		update_corpses_and_portals();
    PerfTimers["update_corpses"].stop();

    PerfTimers["check_idle"].start();
		check_idle_passwords();
    PerfTimers["check_idle"].stop();

    PerfTimers["quest_update"].start();
		quest_update();
    PerfTimers["quest_update"].stop();

    PerfTimers["leaderboard"].start();
		leaderboard.check(); //good place to put this
    PerfTimers["leaderboard"].stop();

		if (DC::instance().cf.bport == false) {
      PerfTimers["check_champ"].start();
			check_champion_and_website_who_list();
      PerfTimers["check_champ"].stop();
		}

    PerfTimers["save_slot"].start();
		save_slot_machines();
    PerfTimers["save_slot"].stop();

    PerfTimers["pulse_hunts"].start();
		pulse_hunts();
    PerfTimers["pulse_hunts"].stop();

    PerfTimers["redo_shop"].start();
		if (!number(0, 47))
    {
			redo_shop_profit();
    }
    PerfTimers["redo_shop"].stop();


    PerfTimers["pulse_time"].stop();
	}
}


/* ******************************************************************
*  general utility stuff (for local use)                            *
****************************************************************** */

/*
 * Turn off echoing (specific to telnet client)
 */
void echo_off(struct descriptor_data *d)
{
  char off_string[] =
  {
    (char) IAC,
    (char) WILL,
    (char) TELOPT_ECHO,
    (char) 0,
  };

  SEND_TO_Q(off_string, d);
}


/*
 * Turn on echoing (specific to telnet client)
 */
void echo_on(struct descriptor_data *d)
{
  char on_string[] =
  {
    (char) IAC,
    (char) WONT,
    (char) TELOPT_ECHO,
    (char) TELOPT_NAOFFD,
    (char) TELOPT_NAOCRD,
    (char) 0,
  };

  SEND_TO_Q(on_string, d);
}

void telnet_ga(descriptor_data *d)
{
  char go_ahead[] = {(char)IAC, (char)GA, (char)0};
  if (d != nullptr)
  {
    d->output += QByteArray(go_ahead);
  }
}

int do_lastprompt(CHAR_DATA *ch, char *arg, int cmd) 
{
  if (GET_LAST_PROMPT(ch))
    csendf(ch, "Last prompt: %s\n\r", GET_LAST_PROMPT(ch));
  else
    send_to_char("Last prompt: unset\n\r", ch);

  return eSUCCESS;
}

int do_prompt(CHAR_DATA *ch, char *arg, int cmd) 
{
  while(*arg == ' ')
    arg++;

  if(IS_MOB(ch)) {
    send_to_char("You're a mob!  You can't set your prompt.\r\n", ch);
    return eFAILURE;
  }

  if(!*arg) {
    send_to_char("Set your prompt to what? Try 'help prompt'.\n\r", ch);
    if(GET_PROMPT(ch)) {
       send_to_char("Current prompt:  ", ch);
       send_to_char(GET_PROMPT(ch), ch);
       send_to_char("\n\r", ch);
       send_to_char("Last prompt: ", ch);
       if (GET_LAST_PROMPT(ch)) {
	 send_to_char(GET_LAST_PROMPT(ch), ch);
       } else {
	 send_to_char("unset", ch);
       }
       send_to_char("\n\r", ch);
    }
    return eSUCCESS;
  }

  // we only have 80 chars of storage in the pfile!
  if(strlen(arg) > 78) {
    send_to_char("Prompts have a maximum of 78 characters.\n\r", ch);
    return eFAILURE;
  }

  if (GET_LAST_PROMPT(ch))
    dc_free(GET_LAST_PROMPT(ch));

  if(GET_PROMPT(ch)) {
    GET_LAST_PROMPT(ch) = str_dup(GET_PROMPT(ch));
    dc_free(GET_PROMPT(ch));
  }

  GET_PROMPT(ch) = str_dup(arg);
  send_to_char("Ok.\n\r", ch);
  return eSUCCESS;
}

char * calc_color_align(int align)
{
  if(align <= -351)
    return BOLD RED;
  if (align <= -300) 
     return BOLD YELLOW;
  if (align <= 299)
     return BOLD GREY;
  if (align <= 349)
     return BOLD YELLOW;
  return BOLD GREEN;
}

char * calc_color(int hit, int max_hit)
{
/* damn whiney players
  int percentage = hit * 100 / max_hit;

  if(percentage >= 100)
    return BOLD GREEN;
  else if(percentage >= 90)
    return GREEN;
  else if(percentage >= 75)
    return BOLD YELLOW;
  else if(percentage >= 50)
    return YELLOW;
  else if(percentage >= 30)
    return RED;
  else if(percentage >= 15)
    return BOLD RED;
  else return BOLD GREY;
*/

  if(hit <= (max_hit / 3))
    return BOLD RED;

  if(hit <= (max_hit / 3) * 2)
    return BOLD YELLOW; 

  return GREEN;

}

char * cond_txtz[] = {
  "excellent condition",
  "a few scratches",
  "slightly hurt",
  "fairly fucked up",
  "bleeding freely",
  "covered in blood",
  "near death",
  "dead as a doornail"
};

QString make_prompt(struct descriptor_data *d)
{
  if (d->character == nullptr)
  {
    return QString();
  }

  QString prompt;
  if (d->showstr_count > 0)
  {
    prompt = QString::fromStdString(fmt::format("\r\n[ Return to continue, (q)uit, (r)efresh, (b)ack, or page number ({} {}) ]", d->showstr_page, d->showstr_count));
  }
  else if (d->strnew)
  {
    if (d->character->pcdata && IS_SET(d->character->pcdata->toggles, PLR_EDITOR_WEB))
    {
      prompt = "Web Editor] ";
    }
    else
    {
      prompt = "*] ";
    }
  }
  else if (d->hashstr)
  {
    prompt = "] ";
  }
  else if (STATE(d) != CON_PLAYING)
  {
    return QString();
  }
  else if (IS_MOB(d->character))
  {
    prompt = generate_prompt(d->character);
  }
  else if (GET_LEVEL(d->character) < IMMORTAL)
  {
    if (!IS_SET(GET_TOGGLES(d->character), PLR_COMPACT))
    {
      prompt = "\n\r";
    }

    if (!GET_PROMPT(d->character))
    {
      prompt += "type 'help prompt'> ";
    } else {
      prompt += generate_prompt(d->character);
    }
  }
  else
  {
    if (!IS_SET(GET_TOGGLES(d->character), PLR_COMPACT))
      prompt = "\n\r";

    struct room_data *rm = &world[d->character->in_room];
    if (IS_SET(GET_TOGGLES(d->character), PLR_ANSI))
    {
      prompt += QString::fromStdString(fmt::format("Z:" RED "{} " NTEXT "R:" GREEN "{} " NTEXT "I:" YELLOW "{}" NTEXT "> ", rm->zone, rm->number, d->character->pcdata->wizinvis));
    }
    else
    {
      prompt += QString::fromStdString(fmt::format("Z:{} R:{} I:{}> ", rm->zone, rm->number, d->character->pcdata->wizinvis));
    }
  }

  return prompt;
}

CHAR_DATA* get_charmie(CHAR_DATA *ch)
{
  if(!ch) return 0;

  struct follow_type *k;
  if(ch->followers)
    for(k = ch->followers; k && k != (follow_type *)0x95959595; k = k->next)
      if(IS_AFFECTED(k->follower, AFF_CHARM))
        return k->follower;

  return NULL;
}

QString generate_prompt(CHAR_DATA *ch)
{
  QString prompt;
  if (ch == nullptr)
  {
    return prompt;
  }

  char_data *charmie = get_charmie(ch);

  if (IS_NPC(ch))
  {
    prompt = "HP: %i/%H %f >";
  }
  else
  {
    prompt = GET_PROMPT(ch);
  }

  QString result;
  bool errorsDetected = false;
  for (auto t : QStringTokenizer{prompt, QString("%")})
  {
    if (t.size() > 0)
    {
      QString token = t.toString();
      QChar tokenType = token[0];
      QString value = " ";
      switch (tokenType.toLatin1())
      {
      case 'p': // %p - Name of current tank
        if (ch->fighting && ch->fighting->fighting)
        {
          value = ch->fighting->fighting->getName();
        }
        break;
      case 'P': // %P - Name of tank in "condition" colors
        if (ch->fighting && ch->fighting->fighting)
        {
          value = ch->fighting->fighting->getColorName();
        }
        break;
      case 'q': // %q - Name of current opponent
        if (ch->fighting)
        {
          value = ch->fighting->getName();
        }
        break;
      case 'Q': // %Q - Name of opponent with color
        if (ch->fighting)
        {
          value = ch->fighting->getColorName();
        }
        break;
      case 'd': // Indicate time of day
        value = time_look[weather_info.sunlight];
        break;
      case 'D': // Indicate weather conditions
        if (OUTSIDE(ch))
          value = sky_look[weather_info.sky];
        else
          value = "indoors";
        break;
      case 'g': // Current gold
        value = QString::number(ch->getGold());
        break;
      case 'G': // Current gold/20000
        value = QString::number((int64)(ch->getGold() / 20000));
        break;
      case '$': // %$ - Current platinum
        value = QString::number(ch->getPlatinum());
        break;
      case 'h': // Current hitpoints
        value = QString::number(ch->getHP());
        break;
      case 'H': // %H - Max hitpoints
        value = QString::number(ch->getMaxHP());
        break;
      case 'm': // %m - Current mana
        value = QString::number(ch->getMana());
        break;
      case 'M': // %M - Max mana
        value = QString::number(ch->getMaxMana());
        break;
      case 'v': // %v - Current moves
        value = QString::number(ch->getMove());
        break;
      case 'V': // %V - Max moves
        value = QString::number(ch->getMaxMove());
        break;
      case 'k': // %k - Current ki
        value = QString::number(ch->getKi());
        break;
      case 'K': // %K - Max ki
        value = QString::number(ch->getMaxKi());
        break;
      case 'l': // %l - Current ki with color
        value = calc_color(ch->getKi(), ch->getMaxKi()) + QString::number(ch->getKi()) + NTEXT;
        break;
      case 'i': // %i - Current hp with color
        value = calc_color(ch->getHP(), ch->getMaxHP()) + QString::number(ch->getHP()) + NTEXT;
        break;
      case 'n': // %n - Current mana with color
        value = calc_color(ch->getMana(), ch->getMaxMana()) + QString::number(ch->getMana()) + NTEXT;
        break;
      case 'w': // %w - Current moves with color
        value = calc_color(ch->getMove(), ch->getMaxMove()) + QString::number(ch->getMove()) + NTEXT;
        break;
      case 'y': // %y - Follower's condition
        if (charmie != nullptr)
        {
          value = QString::number(charmie->getHPPercent());
        }
        break;
      case 'Y': // %Y - Follower's condition with color
        if (charmie != nullptr)
        {
          value = calc_color(charmie->getHP(), charmie->getMaxHP()) + QString::number(charmie->getHPPercent()) + NTEXT;
        }
        break;
      case 'I': // %I - Current hp as a percent 
        value = QString::number(ch->getHPPercent());
        break;
      case 'N':
        value = QString::number(ch->getManaPercent());
        break;
      case 'W':
        value = QString::number(ch->getMovePercent());
        break;
      case 'L':
        value = QString::number(ch->getKiPercent());
        break;
      case 'x':
        value = QString::number(ch->getXP());
        break;
      case 'X':
        value = QString::number(ch->getXPtoLevel());
        break;
      case '%':
        value = "%";
        break;
      case 'a':
        value = QString::number(ch->getAlignment());
        break;
      case 'A':
        value = calc_color_align(ch->getAlignment()) + QString::number(ch->getAlignment()) + NTEXT;
        break;
      case 'c':
        if (ch->fighting)
          value = "<" + ch->getCondition() + ">";
        break;
      case 'C':
        if (ch->fighting)
           value = "<" + ch->getConditionColor() + ">";
        break;
      case 'f':
        if (ch->fighting)
          value = "<" + ch->fighting->getCondition() + ">";
        break;
      case 'F':
        if (ch->fighting)
          value = "<" + ch->fighting->getConditionColor() + ">";
        break;
      case 't':
        if (ch->fighting && ch->fighting->fighting)
          value = "[" + ch->fighting->fighting->getCondition() + "]";
        break;
      case 'T':
        if (ch->fighting && ch->fighting->fighting)
          value = "[" + ch->fighting->fighting->getConditionColor() + "]";
        break;
      case 's':
        if (ch->in_room >= 0 && world_array[ch->in_room])
          value = sector_types[world[ch->in_room].sector_type];
        break;
      case '0':
        value = NTEXT;
        break;
      case '1':
        value = RED;
        break;
      case '2':
        value = GREEN;
        break;
      case '3':
        value = YELLOW;
        break;
      case '4':
        value = BLUE;
        break;
      case '5':
        value = PURPLE;
        break;
      case '6':
        value = CYAN;
        break;
      case '7':
        value = GREY;
        break;
      case '8':
        value = BOLD;
        break;
      case 'r':
        value = "\n\r";
        break;
      default:
        value = "?";
        errorsDetected = true;
        break;
      }
      token.replace(0, 1, value);
      result += token;
    }
  }

  if (errorsDetected == true)
  {
    result += "\n\rYour prompt has invalid prompt codes in it> ";
  }

  return prompt;
}

void write_to_q(char *txt, struct txt_q *queue, int aliased)
{
  struct txt_block *new_block;

  new_block = new txt_block;
  new_block->text = new char[strlen(txt) + 1];

  strcpy(new_block->text, txt);
  new_block->aliased = aliased;

  /* queue empty? */
  if (!queue->head) {
    new_block->next = nullptr;
    queue->head = queue->tail = new_block;
  } else {
    queue->tail->next = new_block;
    queue->tail = new_block;
    new_block->next = nullptr;
  }
}



int get_from_q(struct txt_q *queue, char *dest, int *aliased)
{
  struct txt_block *tmp;

  /* queue empty? */
  if (!queue->head)
    return 0;

  tmp = queue->head;
  strcpy(dest, queue->head->text);
  *aliased = queue->head->aliased;
  queue->head = queue->head->next;

  delete[] tmp->text;
  delete tmp;
  return 1;
}



/* Empty the queues before closing connection */
void flush_queues(struct descriptor_data *d)
{
  int dummy;
  char buf2[8096];

  if (d->large_outbuf) {
    d->large_outbuf->next = bufpool;
    bufpool = d->large_outbuf;
  }
  while (get_from_q(&d->input, buf2, &dummy));
  if(d->output.length() > 0) 
  {
    write_to_descriptor(d->descriptor, d->output);
  }
}

void free_buff_pool_from_memory()
{
  struct txt_block * curr = NULL;

  while(bufpool)
  {
    curr = bufpool->next;
    dc_free(bufpool->text);
    dc_free(bufpool);
    bufpool = curr;
  }
}

QString scramble_text( QString curr )
{
  for (auto &ch : curr)
  {
    // only scramble letters, but not 'm' cause 'm' is used in ansi codes
    if(number(1, 5) == 5 && ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
       && ch != 'm')
    {
      ch = number(0, 1) ? (char) number('a', 'z') : (char) number('A', 'Z');
    }
  }

    return curr;
}

char * handle_ansi_old(char * s, char_data * ch)
{
  char * t;
  char* tp, *sp, *i;

  char nullstring[] = "";
  char dollarstring[] = "$";

  // Worse case scenario is a string of color codes that are all $R's.  These take up
  // 11 characters each.  So to handle that, we'll count the number of $'s and multiply
  // that by 11 for the amount of extra space we need.

  int numdollars = 0;

  t = s;
  while((t = strstr(t, "$"))) {
    numdollars++;
    t++;
  }
  
  size_t size = (strlen(s) + (numdollars*11) + 1) * sizeof(char);
  t = new char[size];
  memset(t, 0, size);

  i = nullstring;
  tp = t;
  sp = s;
  while(*sp) {
    if(*sp != '$') {
      *tp++ = *sp++;
    } else {
       if(IS_MOB(ch) || IS_SET(ch->pcdata->toggles, PLR_ANSI)) {
          switch(*++sp) {
//             case 'B':  i = BLACK; break;
//             case 'R':  i = RED; break;
//             case 'g':  i = GREEN; break;
//             case 'Y':  i = YELLOW; break;
//             case 'b':  i = BLUE; break;
//             case 'P':  i = PURPLE; break;
//             case 'C':  i = CYAN; break;
//             case 'G':  i = GREY; break;
//             case '!':  i = BOLD; break;
//             case 'N':  i = NTEXT; break;

             case '0':  i = BLACK; break;
             case '1':  i = BLUE; break;
             case '2':  i = GREEN; break;
             case '3':  i = CYAN; break;
             case '4':  i = RED; break;
             case '5':  i = YELLOW; break;
             case '6':  i = PURPLE; break;
             case '7':  i = GREY; break;
             case 'B':  i = BOLD; break;
             case 'R':  i = NTEXT; break;
	     case 'L':  i = FLASH;break;
	     case 'K':  i = BLINK;break;
             case 'I':  i = INVERSE; break;
             case '$':  i = dollarstring; break;
             case '\0': // this happens if we end a line with $
                        sp--; // back up to the $ char so we don't go past our \0
                        // no break here so the default catchs it and uses a nullstring
             default: i = nullstring; break;
          }
       } else {
         sp++;
         if(*sp == '$')
           i = dollarstring;
         else i = nullstring;
       }
       while((*tp++ = *i++));
       tp--;
       sp++;
    }
  }
  *tp = '\0';

  return t;
}

QString handle_ansi(QString buffer, char_data *ch)
{
  size_t bufferSize = buffer.size();
  char *copyOfBuffer = new char [bufferSize+1];
  strncpy(copyOfBuffer, buffer.toStdString().c_str(), bufferSize);
  copyOfBuffer[bufferSize] = '\0';

  char *oldResult = handle_ansi_old(copyOfBuffer, ch);
  QString result = QString(oldResult);

  // DO NOT CHECKIN
  assert(result == QString(oldResult));
  
  /*
  char nullstring[] = "";
  char dollarstring[] = "$";

  for (string::iterator i = buffer.begin(); i != buffer.end(); i++)
  {
    if (*i != '$')
    {
      result += *i;
    }
    else
    {
      if (IS_MOB(ch) || IS_SET(ch->pcdata->toggles, PLR_ANSI))
      {
        switch (*i)
        {
        case '0':
          i = BLACK;
          break;
        case '1':
          i = BLUE;
          break;
        case '2':
          i = GREEN;
          break;
        case '3':
          i = CYAN;
          break;
        case '4':
          i = RED;
          break;
        case '5':
          i = YELLOW;
          break;
        case '6':
          i = PURPLE;
          break;
        case '7':
          i = GREY;
          break;
        case 'B':
          i = BOLD;
          break;
        case 'R':
          i = NTEXT;
          break;
        case 'L':
          i = FLASH;
          break;
        case 'K':
          i = BLINK;
          break;
        case 'I':
          i = INVERSE;
          break;
        case '$':
          i = dollarstring;
          break;
        case '\0': // this happens if we end a line with $
          sp--;    // back up to the $ char so we don't go past our \0
                   // no break here so the default catchs it and uses a nullstring
        default:
          i = nullstring;
          break;
        }
      }
      else
      {
        sp++;
        if (*sp == '$')
          i = dollarstring;
        else
          i = nullstring;
      }
    }
  }
*/

  return result;
}

/*
       if(IS_MOB(ch) || IS_SET(ch->pcdata->toggles, PLR_ANSI)) {
          switch(*++sp) {
//             case 'B':  i = BLACK; break;
//             case 'R':  i = RED; break;
//             case 'g':  i = GREEN; break;
//             case 'Y':  i = YELLOW; break;
//             case 'b':  i = BLUE; break;
//             case 'P':  i = PURPLE; break;
//             case 'C':  i = CYAN; break;
//             case 'G':  i = GREY; break;
//             case '!':  i = BOLD; break;
//             case 'N':  i = NTEXT; break;

             case '0':  i = BLACK; break;
             case '1':  i = BLUE; break;
             case '2':  i = GREEN; break;
             case '3':  i = CYAN; break;
             case '4':  i = RED; break;
             case '5':  i = YELLOW; break;
             case '6':  i = PURPLE; break;
             case '7':  i = GREY; break;
             case 'B':  i = BOLD; break;
             case 'R':  i = NTEXT; break;
	     case 'L':  i = FLASH;break;
	     case 'K':  i = BLINK;break;
             case 'I':  i = INVERSE; break;
             case '$':  i = dollarstring; break;
             case '\0': // this happens if we end a line with $
                        sp--; // back up to the $ char so we don't go past our \0
                        // no break here so the default catchs it and uses a nullstring
             default: i = nullstring; break;
          }
       } else {
         sp++;
         if(*sp == '$')
           i = dollarstring;
         else i = nullstring;
       }
       while((*tp++ = *i++));
       tp--;
       sp++;
    }
  }
  *tp = '\0';

*/

/* Add a new string to a player's output queue */
void write_to_output(QString buffer, descriptor_data *t)
{
  /* if there's no descriptor, don't worry about output */
  if (t->descriptor == 0)
    return;

  if (t->connected != CON_EDITING && t->connected != CON_WRITE_BOARD && t->connected != CON_EDIT_MPROG) {
    buffer = handle_ansi(buffer, t->character);
  }

  if(t->character && IS_AFFECTED(t->character, AFF_INSANE) && t->connected == CON_PLAYING)
  {
    buffer = scramble_text(buffer);
  }

  t->output += buffer.toUtf8();
}



/* ******************************************************************
*  socket handling                                                  *
****************************************************************** */


int new_descriptor(int s)
{
  socket_t desc;
#ifndef WIN32
  socklen_t i;
#else
  int i;
#endif
  static int last_desc = 0;	/* last descriptor number */
  struct descriptor_data *newd;
  struct sockaddr_in peer;
  char buf[MAX_STRING_LENGTH];
  
/* accept the new connection */
  i = sizeof(peer);
  getsockname(s, (struct sockaddr *) &peer, &i);
  if ((desc = accept(s, (struct sockaddr *) &peer, &i)) < 0) {
    perror("accept");
    return -1;
  }

  // keep it from blocking 
#ifndef WIN32
  if(fcntl(desc, F_SETFL, O_NONBLOCK) < 0) {
    perror("init_socket : fcntl : nonblock");
    exit(1);
  }
#else
  unsigned long int nb = 1;
  if(ioctlsocket(desc, FIONBIO, &nb) < 0)
  {
	perror("init_socket : ioctl : nonblock");
	exit(1);
  }
#endif

  /* create a new descriptor */
  newd = new descriptor_data;
  memset((char *) newd, 0, sizeof(struct descriptor_data));
  strcpy(newd->host, inet_ntoa(peer.sin_addr));

  /* determine if the site is banned */
  if (isbanned(newd->host) == BAN_ALL) {
    write_to_descriptor(desc, 
              "Your site has been banned from Dark Castle. If you have any\n\r"
              "Questions, please email us at:\n\r"
              "imps@dcastle.org\n\r");
                                                  
    CLOSE_SOCKET(desc);
    sprintf(buf, "Connection attempt denied from [%s]", newd->host);
    log(buf, OVERSEER, LOG_SOCKET);
   // dc_free(newd->host);
    dc_free(newd);
    return 0;
  }

  /* initialize descriptor data */
  newd->descriptor = desc;
  newd->idle_tics = 0;
  newd->idle_time = 0;
  newd->wait = 1;
  newd->next = descriptor_list;
  newd->login_time = time(0);
  newd->astr = 0;
  if (++last_desc == 1000)
    last_desc = 1;
  newd->desc_num = last_desc;

  /* prepend to list */
  descriptor_list = newd;

  newd->connected = CON_PRE_DISPLAY_ENTRANCE;
  return 0;
}



int process_output(struct descriptor_data *t)
{
  static int result;
  if (t->character && t->connected == CON_PLAYING)
  {
    t->output += blackjack_prompt(t->character, t->character->pcdata && !IS_SET(t->character->pcdata->toggles, PLR_ASCII)).toUtf8();
  }
  
  t->output += make_prompt(t).toUtf8();
  telnet_ga(t);
  

  /*
   * now, send the output.  If this is an 'interruption', use the prepended
   * CRLF, otherwise send the straight output sans CRLF.
   */
  if (!t->prompt_mode) {		/* && !t->connected) */
     t->output = "\r\n" + t->output;
    result = write_to_descriptor(t->descriptor, t->output);
    t->prompt_mode = 0;
  } else {
    result = write_to_descriptor(t->descriptor, t->output);
    t->prompt_mode = 0;
  }
  /* handle snooping: prepend "% " and send to snooper */
  if (t->snoop_by) {
    SEND_TO_Q("% ", t->snoop_by);
    SEND_TO_Q(t->output, t->snoop_by);
    SEND_TO_Q("%%", t->snoop_by);
  }
  t->output.clear();

  return result;
}

int write_to_descriptor(socket_t desc, QByteArray txt)
{
  int total, bytes_written;

  total = txt.size();

  do
  {
    if ((bytes_written = write(desc, txt.toStdString().c_str(), total)) < 0)
    {
#ifdef EWOULDBLOCK
      if (errno == EWOULDBLOCK)
        errno = EAGAIN;
#endif /* EWOULDBLOCK */
      if (errno == EAGAIN)
      {
        log("process_output: socket write would block", 0, LOG_MISC);
      }
      else
      {
        perror("Write to socket");
        return (-1);
      }
      return (0);
    }
    else
    {
      txt = txt.remove(0, bytes_written);
      total -= bytes_written;
    }
  } while (total > 0);

  return 0;
}

/*
 * ASSUMPTION: There will be no newlines in the raw input buffer when this
 * function is called.  We must maintain that before returning.
 */
int process_input(struct descriptor_data *t)
{
  int buf_length, bytes_read, space_left, failed_subst;
  char *ptr = NULL;
  char *read_point = NULL;
  char *write_point = NULL;
  char *nl_pos = NULL;
  char *tmp_ptr = NULL;
  char tmp[MAX_INPUT_LENGTH + 8];

  t->idle_time = 0;
  /* first, find the point where we left off reading data */
  buf_length = strlen(t->inbuf);
  read_point = t->inbuf + buf_length;
  space_left = MAX_RAW_INPUT_LENGTH - buf_length - 1;

  do
  {
    if (space_left <= 0)
    {
//      log("process_input: input overflow", ANGEL,LOG_SOCKET);
          
      break;
    }
    if ((bytes_read = read(t->descriptor, read_point, space_left)) < 0)
    {

#ifdef EWOULDBLOCK
      if (errno == EWOULDBLOCK)
        errno = EAGAIN;
#endif /* EWOULDBLOCK */
      if (errno != EAGAIN)
      {
        perror("process_input: about to lose connection");
        return -1; /* some error condition was encountered on
				 * read */
      }
      else
        return 0; /* the read would have blocked: just means no
				 * data there but everything's okay */
    }
    else if (bytes_read == 0)
    {
      log("EOF on socket read (connection broken by peer)", ANGEL, LOG_SOCKET);
      return -1;
    }
    /* at this point, we know we got some data from the read */

    *(read_point + bytes_read) = '\0'; /* terminate the string */

    /* search for a newline in the data we just read */
    for (ptr = read_point; *ptr && !nl_pos; ptr++)
      if (ISNEWL(*ptr) || (t->connected != CON_WRITE_BOARD && t->connected != CON_EDITING && t->connected != CON_EDIT_MPROG && *ptr == '|'))
        nl_pos = ptr;

    read_point += bytes_read;
    space_left -= bytes_read;

    /*
 * on some systems such as AIX, POSIX-standard nonblocking I/O is broken,
 * causing the MUD to hang when it encounters input not terminated by a
 * newline.  This was causing hangs at the Password: prompt, for example.
 * I attempt to compensate by always returning after the _first_ read, instead
 * of looping forever until a read returns -1.  This simulates non-blocking
 * I/O because the result is we never call read unless we know from select()
 * that data is ready (process_input is only called if select indicates that
 * this descriptor is in the read set).  JE 2/23/95.
 */
  } while (nl_pos == NULL);
  /*
   * okay, at this point we have at least one newline in the string; now we
   * can copy the formatted data to a new array for further processing.
   */

  read_point = t->inbuf;

  while (nl_pos != NULL)
  {
    write_point = tmp;
    space_left = MAX_INPUT_LENGTH - 1;

    for (ptr = read_point; (space_left > 0) && (ptr < nl_pos); ptr++)
    {
      if (*ptr == '\b')
      { // handle backspacing
        if (write_point > tmp)
        {
          if ((*(--write_point) == '$') && (write_point > tmp) &&    // if backup to $ AND room left AND
              (!t->character || GET_LEVEL(t->character) < IMMORTAL)) //    (no char OR mortal)
          {
            // need to backspace twice if it's a $ to keep morts from the $codes
            // need the write_point > tmp check to make sure we don't backspace past beginning
            write_point--;
            space_left += 2;
          }
          else
            space_left++;
        }
        // BEGIN NEW HERE - replacing how $'s are handled to make it more intelligent
        // and to stop it from overwriting our buffer
      }
      else if ((*ptr == '$'))
      {
        if (!t->character || (GET_LEVEL(t->character) < IMMORTAL))
        {
          // if it's a $, and I'm a mortal, or have no character yet, handle it.
          // if there is a $, double it if there is room, and keep going
          if (space_left > 2)
          {
            *write_point++ = *ptr;
            *write_point++ = '$';
            space_left -= 2;
          }
          else
            space_left = 0; // so it truncates properly
          // do nothing, which junks the $
        }
        else
        {
          // gods can use $codes but only ones for color UNLESS inside a MOBProg editor
          // I have to let them use $codes inside the editor or they can't write MOBProgs
          // tmp_ptr is just so I don't have to put ptr+1 7 times....
          tmp_ptr = (ptr + 1);
          if (isdigit(*tmp_ptr) || *tmp_ptr == 'I' || *tmp_ptr == 'L' ||
              *tmp_ptr == '*' || *tmp_ptr == 'R' ||
              *tmp_ptr == 'B' || t->connected == CON_EDIT_MPROG ||
              t->connected == CON_EDITING)
          { // write it like normal
            *write_point++ = *ptr;
            space_left--;
          }
          else if (space_left > 2)
          { // any other code, double up the $
            *write_point++ = *ptr;
            *write_point++ = '$';
            space_left -= 2;
          }
          else
            space_left = 0; // if no space left, so it truncates properly
                            // do nothing, which junks the $
        }
      }
      else if (isascii(*ptr) && isprint(*ptr))
      {
        *write_point++ = *ptr;
        space_left--;
      }
      // END NEW HERE

      /* BEGIN OLD HERE
      } else if (isascii(*ptr) && isprint(*ptr)) {
	if (((*(write_point++) = *ptr) == '$') && 
             ((*(ptr+1) != '$') || doublesign))
        {
          if(doublesign) // last one was a '$'
          {
            doublesign = 0;
            space_left--;
          }
          else
          {
	    *(write_point++) = '$';	// double the $ 
	    space_left -= 2;
          }
	} 
        else
        {
          if(*(ptr+1) == '$' && *ptr == '$')
            doublesign = 1;
	  space_left--;
        }
      }
END OLD HERE */
    }

    *write_point = '\0';

    if ((space_left <= 0) && (ptr < nl_pos))
    {
      char buffer[MAX_INPUT_LENGTH + 64];

      sprintf(buffer, "Line too long.  Truncated to:\r\n%s (%d)\r\n", tmp, strlen(tmp));
      if (write_to_descriptor(t->descriptor, QString(buffer).toUtf8()) < 0)
        return -1;
    }
    if (t->snoop_by)
    {
      SEND_TO_Q("% ", t->snoop_by);
      SEND_TO_Q(tmp, t->snoop_by);
      SEND_TO_Q("\r\n", t->snoop_by);
    }
    failed_subst = 0;

    if (*tmp == '!')
      strcpy(tmp, t->last_input);
    else if (*tmp == '^')
    {
      if (!(failed_subst = perform_subst(t, t->last_input, tmp)))
        strcpy(t->last_input, tmp);
    }
    else
      strcpy(t->last_input, tmp);

    if (!failed_subst)
      write_to_q(tmp, &t->input, 0);

    /* find the end of this line */
    //while (ISNEWL(*nl_pos))
    while (ISNEWL(*nl_pos) || (t->connected != CON_WRITE_BOARD && t->connected != CON_EDITING && t->connected != CON_EDIT_MPROG && *nl_pos == '|'))
      nl_pos++;

    /* see if there's another newline in the input buffer */
    read_point = ptr = nl_pos;
    for (nl_pos = NULL; *ptr && !nl_pos; ptr++)
      if (ISNEWL(*ptr) || (t->connected != CON_WRITE_BOARD && t->connected != CON_EDITING && t->connected != CON_EDIT_MPROG && *ptr == '|'))
        nl_pos = ptr;
    //if (ISNEWL(*ptr))
  }

  /* now move the rest of the buffer up to the beginning for the next pass */
  write_point = t->inbuf;
  while (*read_point)
    *(write_point++) = *(read_point++);
  *write_point = '\0';

  return 1;
}

/*
 * perform substitution for the '^..^' csh-esque syntax
 * orig is the orig string (i.e. the one being modified.
 * subst contains the substition string, i.e. "^telm^tell"
 */
int perform_subst(struct descriptor_data *t, char *orig, char *subst)
{
  char new_subst[MAX_INPUT_LENGTH + 5];

  char *first, *second, *strpos;

  /*
   * first is the position of the beginning of the first string (the one
   * to be replaced
   */
  first = subst + 1;

  /* now find the second '^' */
  if (!(second = strchr(first, '^'))) {
    SEND_TO_Q("Invalid substitution.\r\n", t);
    return 1;
  }
  /* terminate "first" at the position of the '^' and make 'second' point
   * to the beginning of the second string */
  *(second++) = '\0';

  /* now, see if the contents of the first string appear in the original */
  if (!(strpos = strstr(orig, first))) {
    SEND_TO_Q("Invalid substitution.\r\n", t);
    return 1;
  }
  /* now, we construct the new string for output. */

  /* first, everything in the original, up to the string to be replaced */
  strncpy(new_subst, orig, (strpos - orig));
  new_subst[(strpos - orig)] = '\0';

  /* now, the replacement string */
  strncat(new_subst, second, (MAX_INPUT_LENGTH - strlen(new_subst) - 1));

  /* now, if there's anything left in the original after the string to
   * replaced, copy that too. */
  if (((strpos - orig) + strlen(first)) < strlen(orig))
    strncat(new_subst, strpos + strlen(first), 
           (MAX_INPUT_LENGTH - strlen(new_subst) - 1));

  /* terminate the string in case of an overflow from strncat */
  new_subst[MAX_INPUT_LENGTH - 1] = '\0';
  strcpy(subst, new_subst);

  return 0;
}

// return 1 on success
// return 0 if we quit everyone out at the bottom
int close_socket(struct descriptor_data *d)
{
  char buf[128], idiotbuf[128];
  struct descriptor_data *temp;
  // long target_idnum = -1;
  if (!d) return 0;
  flush_queues(d);
  CLOSE_SOCKET(d->descriptor);

  /* Forget snooping */
  if (d->snooping)
    d->snooping->snoop_by = NULL;

  if (d->snoop_by) {
    SEND_TO_Q("Your victim is no longer among us.\r\n", d->snoop_by);
    d->snoop_by->snooping = NULL;
  }
  if(d->hashstr) {
    strcpy(idiotbuf, "\n\r~\n\r");
    strcat(idiotbuf, "\0");
    string_hash_add(d, idiotbuf);
  }
  if(d->strnew && (IS_MOB(d->character) || !IS_SET(d->character->pcdata->toggles, PLR_EDITOR_WEB))) {
    strcpy(idiotbuf, "/s\n\r");
    strcat(idiotbuf, "\0");
    new_string_add(d, idiotbuf);
  }
  if (d->character) {
    // target_idnum = GET_IDNUM(d->character);
    if (d->connected == CON_PLAYING || d->connected == CON_WRITE_BOARD ||
	d->connected == CON_EDITING || d->connected == CON_EDIT_MPROG) {
      save_char_obj(d->character);
	// clan area stuff
      extern void check_quitter(CHAR_DATA *ch);
      check_quitter(d->character);
	
      // end any performances
      if(IS_SINGING(d->character))
         do_sing(d->character, "stop", 9);

      act("$n has lost $s link.", d->character, 0, 0, TO_ROOM, 0);
      sprintf(buf, "Closing link to: %s at %d.", GET_NAME(d->character),
              world[d->character->in_room].number);
      if(IS_AFFECTED(d->character, AFF_CANTQUIT))
              sprintf(buf, "%s with CQ.", buf);
      log(buf, GET_LEVEL(d->character) > SERAPH ? GET_LEVEL(d->character) : SERAPH, LOG_SOCKET);
      d->character->desc = NULL;
    } else {
      sprintf(buf, "Losing player: %s.",
	      GET_NAME(d->character) ? GET_NAME(d->character) : "<null>");
      log(buf, ANGEL, LOG_SOCKET);
      if (d->connected == CON_WRITE_BOARD || d->connected ==
		CON_EDITING || d->connected == CON_EDIT_MPROG)
	{
//		sprintf(buf, "Suspicious: %s.", 
//			GET_NAME(d->character));
//		log(buf, 110, LOG_HMM);
	}
			delete d->character;
    }
  }
//   Removed this log caues it's so fricken annoying
//   else
//    log("Losing descriptor without char.", ANGEL, LOG_SOCKET);

  /* JE 2/22/95 -- part of my unending quest to make switch stable */
  if (d->original && d->original->desc)
    d->original->desc = NULL;

  // if we're closing the socket that is next to be processed, we want to
  // go ahead and move on to the next one
  if(d == next_d)
    next_d = d->next;

  REMOVE_FROM_LIST(d, descriptor_list, next);

  if (d->showstr_head)
    dc_free(d->showstr_head);
  if (d->showstr_count)
    dc_free(d->showstr_vector);

  dc_free(d);
  d = NULL;

/*  if(descriptor_list == NULL) 
  {
    // if there is NOONE on (everyone got disconnected) loop through and
    // boot all of the linkdeads.  That way if the mud's link is cut, the
    // first person back on can't RK everyone
    char_data * next_i;
    for(char_data * i = character_list; i; i = next_i) {
       next_i = i->next;
       if(IS_NPC(i))
         continue;
       do_quit(i, "", 666);
    }
    return 0;
  }*/
  return 1;
}



void check_idle_passwords(void)
{
  struct descriptor_data *d, *next_d;

  for (d = descriptor_list; d; d = next_d) {
    next_d = d->next;
    if (STATE(d) != CON_GET_OLD_PASSWORD && STATE(d) != CON_GET_NAME)
      continue;
    if (!d->idle_tics) {
      d->idle_tics++;
      continue;
    } else {
      echo_on(d);
      SEND_TO_Q("\r\nTimed out... goodbye.\r\n", d);
      STATE(d) = CON_CLOSE;
    }
  }
}

/* ******************************************************************
*  signal-handling functions (formerly signals.c)                   *
****************************************************************** */


void checkpointing(int sig)
{
  if (!tics) {
    log("SYSERR: CHECKPOINT shutdown: tics not updated", ANGEL, LOG_BUG);
    abort();
  } else
    tics = 0;
}

void report_debug_logging()
{
  extern int  last_char_room;

  log("Last cmd:", ANGEL, LOG_BUG);
  log(last_processed_cmd, ANGEL, LOG_BUG);
  log("Owner's Name:", ANGEL, LOG_BUG);
  log(last_char_name, ANGEL, LOG_BUG);
  logf(ANGEL, LOG_BUG, "Last room: %d", last_char_room);
}

void crash_hotboot()
{
  struct descriptor_data * d = NULL;
  extern int try_to_hotboot_on_crash;
  extern int died_from_sigsegv;

  // This can be dangerous, because if we had a SIGSEGV due to a descriptor being
  // invalid, we're going to do it again.  That's why we put in extern int died_from_sigsegv
  // sigsegv = # of times we've crashed from SIGSEGV

  for (d=descriptor_list;d && died_from_sigsegv < 2;d=d->next) {
    write_to_descriptor(d->descriptor,"Mud crash detected.\n\r");
  }

  // attempt to hotboot
  if(try_to_hotboot_on_crash) {
    for (d=descriptor_list;d && died_from_sigsegv < 2;d=d->next) {
      write_to_descriptor(d->descriptor,"Attempting to recover with a hotboot.\n\r");
    }
    log("Attempting to hotboot from the crash.", ANGEL, LOG_BUG);
    write_hotboot_file(0);
    // we shouldn't return from there unless we failed
    log("Hotboot crash recovery failed.  Exiting.", ANGEL, LOG_BUG);
    for (d=descriptor_list;d && died_from_sigsegv < 2;d=d->next) {
      write_to_descriptor(d->descriptor,"Hotboot failed giving up.\n\r");
    }
  }

  for (d=descriptor_list;d && died_from_sigsegv < 2;d=d->next) {
    write_to_descriptor(d->descriptor,"Giving up, goodbye.\n\r");
  }
}

void crashill(int sig)
{
  report_debug_logging();
  log("Recieved SIGFPE (Illegal Instruction)", ANGEL, LOG_BUG);
  crash_hotboot();
  log("Mud exiting from SIGFPE.", ANGEL, LOG_BUG);
  exit(0);
}

void crashfpe(int sig)
{
  report_debug_logging();
  log("Recieved SIGFPE (Arithmetic Error)", ANGEL, LOG_BUG);
  crash_hotboot();
  log("Mud exiting from SIGFPE.", ANGEL, LOG_BUG);
  exit(0);
}

void crashsig(int sig)
{
  extern int died_from_sigsegv;
  died_from_sigsegv++;
  if(died_from_sigsegv > 3) { // panic! error is in log...lovely  just give up
    exit(0);
  }
  if(died_from_sigsegv > 2) { // panic! try to log and get out
    log("Hit 'died_from_sigsegv > 2'", ANGEL, LOG_BUG);
    exit(0);
  }
  report_debug_logging();
  log("Recieved SIGSEGV (Segmentation fault)", ANGEL, LOG_BUG);
  crash_hotboot();
  log("Mud exiting from SIGSEGV.", ANGEL, LOG_BUG);
  exit(0);
}

void unrestrict_game(int sig)
{
  extern struct ban_list_element *ban_list;
  extern int num_invalid;

  log("Received SIGUSR2 - completely unrestricting game (emergent)",
       ANGEL, LOG_GOD);
  ban_list = NULL;
  restrict = 0;
  num_invalid = 0;
}


void hupsig(int sig)
{
  log("Received SIGHUP, SIGINT, or SIGTERM.  Shutting down...", 0, LOG_MISC);
  abort();			/* perhaps something more elegant should
				 * substituted */
}

#ifndef WIN32
void sigchld(int sig)
{
  struct rusage ru;
  wait3(NULL, WNOHANG, &ru);
}
#endif
/*
 * This is an implementation of signal() using sigaction() for portability.
 * (sigaction() is POSIX; signal() is not.)  Taken from Stevens' _Advanced
 * Programming in the UNIX Environment_.  We are specifying that all system
 * calls _not_ be automatically restarted for uniformity, because BSD systems
 * do not restart select(), even if SA_RESTART is used.
 *
 * Note that NeXT 2.x is not POSIX and does not have sigaction; therefore,
 * I just define it to be the old signal.  If your system doesn't have
 * sigaction either, you can use the same fix.
 *
 * SunOS Release 4.0.2 (sun386) needs this too, according to Tim Aldric.
 */

#define my_signal(signo, func) signal(signo, func)

void signal_handler(int signal, siginfo_t *si, void *) {
	logf(IMMORTAL, LOG_BUG, "signal_handler: signo=%d errno=%d code=%d "
			"pid=%d uid=%d status=%d utime=%lu stime=%lu value=%d "
			"int=%d ptr=%p overrun=%d timerid=%d addr=%p band=%ld "
			"fd=%d", si->si_signo, si->si_errno, si->si_code,
			si->si_pid, si->si_uid, si->si_status, si->si_utime, si->si_stime, si->si_value.sival_int,
			si->si_int, si->si_ptr, si->si_overrun, si->si_timerid, si->si_addr, si->si_band,
			si->si_fd);

	if (signal == SIGINT || signal == SIGTERM) {
		abort();
	}
}

void signal_setup(void)
{
#ifndef WIN32
  // struct timeval interval;


  /*
   * user signal 2: unrestrict game.  Used for emergencies if you lock
   * yourself out of the MUD somehow.  (Duh...)
   */
  my_signal(SIGUSR2, unrestrict_game);

  /*
   * set up the deadlock-protection so that the MUD aborts itself if it gets
   * caught in an infinite loop for more than 3 minutes.  Doesn't work with
   * OS/2.
   */
  /* just to be on the safe side: */
  struct sigaction sahup;
  memset(&sahup, 0, sizeof(sahup));
  sahup.sa_sigaction = signal_handler;
  sigemptyset(&sahup.sa_mask);
  sahup.sa_flags = SA_SIGINFO;
  sahup.sa_restorer = NULL;
  sigaction(SIGHUP, &sahup, NULL);

  my_signal(SIGTERM, hupsig);
  my_signal(SIGPIPE, SIG_IGN);
  my_signal(SIGALRM, SIG_IGN);
  signal(SIGCHLD, sigchld); // hopefully kill zombies

//  my_signal(SIGSEGV, crashsig);  // catch null->blah
  my_signal(SIGFPE,  crashfpe);  // catch x / 0
  my_signal(SIGILL,  crashill);  // catch illegal instruction
#endif
}


/* ****************************************************************
*       Public routines for system-to-player-communication        *
**************************************************************** */

void send_to_char_regardless(char *messg, CHAR_DATA *ch) {
  if(ch->desc && messg) {
    SEND_TO_Q(messg, ch->desc);
  }
}

void send_to_char_nosp(const char *messg, struct char_data *ch)
{
  char *tmp = str_nospace(messg);
  send_to_char(tmp, ch);
  dc_free(tmp);
}

void record_msg(char *messg, struct char_data *ch)
{
  if (!messg || IS_NPC(ch) || GET_LEVEL(ch) < IMMORTAL)
    return;

  if (ch->pcdata->away_msgs == 0) {
    ch->pcdata->away_msgs = new std::queue<char *>();
  }

  if (ch->pcdata->away_msgs->size() < 1000) {
    char *our_copy = str_dup(messg);
    ch->pcdata->away_msgs->push(our_copy);
  }
}

int do_awaymsgs(CHAR_DATA *ch, char *argument, int cmd)
{
  int lines = 0;
  char *tmp;
  
  if (IS_NPC(ch))
    return eFAILURE;
  
  if ((ch->pcdata->away_msgs == 0) || ch->pcdata->away_msgs->empty()) {
    SEND_TO_Q("No messages have been recorded.\n\r", ch->desc);
    return eSUCCESS;
  }

  // Show 23 lines of text, then stop
  while (! ch->pcdata->away_msgs->empty()) {
    tmp = ch->pcdata->away_msgs->front();
    SEND_TO_Q(tmp, ch->desc);
    dc_free(tmp);
    ch->pcdata->away_msgs->pop();
    
    if (++lines == 23) {
      SEND_TO_Q("\n\rMore msgs available. Type awaymsgs to see them\n\r",
		ch->desc);
      break;
    }
  }

  return eSUCCESS;
}

void check_for_awaymsgs(struct char_data *ch)
{
  if (!ch)
    return;

  if (IS_NPC(ch))
    return;

  if ((ch->pcdata->away_msgs == 0) || ch->pcdata->away_msgs->empty()) {
    return;
  }

  send_to_char("You have unviewed away messages. ", ch);
  send_to_char("Type awaymsgs to view them.\n\r", ch);
}

void send_to_char(string messg, struct char_data *ch)
{
  send_to_char(messg.c_str(), ch);
}

void send_to_char(const char *messg, struct char_data *ch)
{
  extern bool selfpurge;
  if (IS_NPC(ch) && !ch->desc && MOBtrigger && messg)
    mprog_act_trigger( messg, ch, 0, 0, 0 );
  if (IS_NPC(ch) && !ch->desc && !selfpurge && MOBtrigger && messg)
    oprog_act_trigger( messg, ch);

  if (!selfpurge && (ch->desc && messg) && (!is_busy(ch))) {
    SEND_TO_Q(messg, ch->desc);
  }
}

void send_to_all(char *messg)
{
  struct descriptor_data *i;

  if (messg)
    for (i = descriptor_list; i; i = i->next)
      if (!i->connected)
	SEND_TO_Q(messg, i);
}

void ansi_color( char *txt, CHAR_DATA *ch )
{
    // mobs don't have toggles, so they automatically get ansi on
    if ( txt != NULL && ch->desc != NULL ) {
       if ( !IS_MOB(ch) && 
            !IS_SET(GET_TOGGLES(ch), PLR_ANSI) &&
            !IS_SET(GET_TOGGLES(ch), PLR_VT100) )
          return;
       else if (!IS_MOB(ch) && 
                 IS_SET(GET_TOGGLES(ch), PLR_VT100) &&
                !IS_SET(GET_TOGGLES(ch), PLR_ANSI ) )
      {
         if ( (!strcmp(txt,GREEN))
           || (!strcmp(txt,RED))
           || (!strcmp(txt,BLUE))
           || (!strcmp(txt,BLACK))
           || (!strcmp(txt,CYAN))
           || (!strcmp(txt,GREY))
           || (!strcmp(txt,EEEE))
           || (!strcmp(txt,YELLOW))
           || (!strcmp(txt,PURPLE)))
	   return;
      }
     send_to_char( txt, ch );
     return;
    }
}

void send_info(string messg)
{
  send_info(messg.c_str());
}


void send_info(const char *messg)
{
    struct descriptor_data *i;

    if(messg)
      for(i = descriptor_list; i; i = i->next) {
         if(!(i->character) ||
            !IS_SET(i->character->misc, CHANNEL_INFO) )
           continue;
         if((!i->connected) && !is_busy(i->character))
           SEND_TO_Q(messg, i);
      }
}


void send_to_outdoor(char *messg) {
    struct descriptor_data *i;

    if (messg)
        for (i = descriptor_list; i; i = i->next)
            if (!i->connected)
                if (OUTSIDE(i->character) && !is_busy(i->character))
                    SEND_TO_Q(messg, i);
}

void send_to_zone(char *messg, int zone)
{
   struct descriptor_data *i = NULL; 
   if(messg) 
   {
      for(i = descriptor_list; i; i = i->next) 
      {
         if(!i->connected && !is_busy(i->character) && i->character->in_room != NOWHERE
                          && world[i->character->in_room].zone == zone) 
         {
            SEND_TO_Q(messg, i);
         }
      }
   }
}

void send_to_room(const char *messg, int room, bool awakeonly, CHAR_DATA *nta)
{
    CHAR_DATA *i = NULL;

    //If a megaphone goes off when in someone's inventory this happens
    if (room == -1)
      return;

    if(!world_array[room] || !world[room].people) {
        return;
    }
    if (messg)
        for (i = world[room].people; i; i = i->next_in_room)
            if (i->desc && !is_busy(i) && nta != i)
               if(!awakeonly || GET_POS(i) > POSITION_SLEEPING)
                  SEND_TO_Q(messg, i->desc);
}

int is_busy(CHAR_DATA *ch)
{

  if(ch->desc &&
     ((ch->desc->connected == CON_WRITE_BOARD) ||
      (ch->desc->connected == CON_SEND_MAIL) ||
      (ch->desc->connected == CON_EDITING) ||
      (ch->desc->connected == CON_EDIT_MPROG)
     )
    )
    return 1;

  return(0);
}

int perform_alias(struct descriptor_data *d, char *orig) {
     char first_arg[MAX_INPUT_LENGTH], new_buf[MAX_INPUT_LENGTH], *ptr;
     ptr = any_one_arg(orig, first_arg);
     struct char_player_alias * x;
     int lengthpre;
     int lengthpost;
     
     if(!*first_arg) {
         return(0);
     }
     if(IS_MOB(d->character) || !d->character->pcdata->alias)
         return(0);

     for (x=d->character->pcdata->alias; x ; x = x->next) {
         if (x->keyword)
            if (!strcmp(x->keyword, first_arg)) {
               strcpy(new_buf, x->command);

               // make sure the new command still fits in our buffers
               lengthpre = strlen(new_buf);
               lengthpost = strlen(ptr);
               if(lengthpre + lengthpost > MAX_INPUT_LENGTH - 1)
               {
                  ptr[MAX_INPUT_LENGTH - lengthpre - 1] = '\0'; // truncate it to fit
                  strcat(new_buf, ptr);
                  strcat(new_buf, "\0");
                  strcpy(orig, new_buf);
                  SEND_TO_Q("Line too long.  Truncated to:\r\n", d);
                  SEND_TO_Q(orig, d);
                  SEND_TO_Q("\r\n", d);
               }
               else 
               {
                  strcat(new_buf, ptr);
                  strcat(new_buf, "\0");
                  strcpy(orig, new_buf);
               }
            }
        }
    return(0);
}

void skip_spaces(char **string)
{
  for (; **string && isspace(**string); (*string)++);
}


char *any_one_arg(char *argument, char *first_arg)
{
  skip_spaces(&argument);

  while (*argument && !isspace(*argument)) {
    *(first_arg++) = LOWER(*argument);
    argument++;
  }

  *first_arg = '\0';
  return argument;
}

bool is_multi(char_data *ch)
{
  for (descriptor_data *d = descriptor_list; d; d = d->next)
  {
    if (d->character &&
        strcmp(GET_NAME(ch), GET_NAME(d->character)) &&
        !strcmp(d->host, ch->desc->host))
    {
      return true;
    }
  }

  return false;
}

void warn_if_duplicate_ip(char_data *ch)
{
  char buf[256];
  int highlev = 51;

  list<multiplayer> multi_list;

  for (descriptor_data *d = descriptor_list; d; d = d->next)
  {
    if (d->character &&
        strcmp(GET_NAME(ch), GET_NAME(d->character)) &&
        !strcmp(d->host, ch->desc->host))
    {
      multiplayer m;
      m.host = d->host;
      m.name1 = GET_NAME(ch);
      m.name2 = GET_NAME(d->character);

      multi_list.push_back(m);

      highlev = MAX(GET_LEVEL(d->character), GET_LEVEL(ch));
      highlev = MAX(highlev, OVERSEER);

      // Mark both characters as multi-playing until they log out
      // This will be used elsewhere to enable automatic logging
      if (ch->pcdata)
      {
        ch->pcdata->multi = true;
      }

      if (d->character->pcdata)
      {
        d->character->pcdata->multi = true;
      }
    }
  }

  for (list<multiplayer>::iterator i = multi_list.begin(); i != multi_list.end(); ++i)
  {
    logf(108, LOG_WARNINGS, "MultipleIP: %s -> %s / %s ", (*i).host, (*i).name1, (*i).name2);
  }
}

  int do_editor(CHAR_DATA * ch, char *argument, int cmd)
  {
    char arg1[MAX_INPUT_LENGTH];
    if (argument == 0)
      return eFAILURE;

    if (IS_MOB(ch))
      return eFAILURE;

    csendf(ch, "Current editor: %s\n\r\n\r", IS_SET(ch->pcdata->toggles, PLR_EDITOR_WEB) ? "web" : "game");

    one_argument(argument, arg1);

    if (*arg1)
    {
      if (!strcmp(arg1, "web")) {
	  SET_BIT(ch->pcdata->toggles, PLR_EDITOR_WEB);
	  send_to_char("Changing to web editor.\n\r", ch);
	  send_to_char("Ok.\n\r", ch);
	  return eSUCCESS;
      } else if (!strcmp(arg1, "game")) {
	  REMOVE_BIT(ch->pcdata->toggles, PLR_EDITOR_WEB);
	  send_to_char("Changing to in game line editor.\n\r", ch);
	  send_to_char("Ok.\n\r", ch);
	  return eSUCCESS;
      }
  }

  send_to_char("Usage: editor <type>\n\r", ch);
  send_to_char("Where type can be:\n\r", ch);
  send_to_char("web    - use online web editor\n\r", ch);
  send_to_char("game   - use in game line editor\n\r", ch);

  return eSUCCESS;
}
