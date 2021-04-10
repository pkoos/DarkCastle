#include <arpa/inet.h>
#include <string.h>
#include <fmt/format.h>
#include <fmt/chrono.h>

#include <QString>
using namespace Qt;

#include "player.h"
#include "levels.h"
#include "structs.h"
#include "character.h"
#include "utility.h"
#include "comm.h"
#include "interp.h"
#include "handler.h"
#include "db.h"
#include "returnvals.h"
#include "spells.h"

struct ban_list_element *ban_list = NULL;


char *ban_types[] = {
  "no",
  "new",
  "select",
  "all",
  "ERROR"
};


void load_banned(void)
{
  FILE *fl;
  int i, date;
  char site_name[BANNED_SITE_LENGTH + 1], ban_type[100];
  char name[100 + 1];
  struct ban_list_element *next_node;

  ban_list = 0;

  if (!(fl = fopen("banned", "r"))) {
    perror("Unable to open banfile");
    return;
  }
  while (fscanf(fl, " %s %s %d %s ", ban_type, site_name, &date, name) == 4) {
    CREATE(next_node, struct ban_list_element, 1);
    next_node->site = site_name;
    strncpy(next_node->name, name, 100);
    next_node->name[99] = '\0';
    next_node->date = date;

    for (i = BAN_NOT; i <= BAN_ALL; i++)
      if (!strcmp(ban_type, ban_types[i]))
	next_node->type = i;

    next_node->next = ban_list;
    ban_list = next_node;
  }

  fclose(fl);
}

void free_ban_list_from_memory()
{
  ban_list_element * next = NULL;

  for( ;ban_list; ban_list = next)
  {
    next = ban_list->next;
    dc_free(ban_list);
  }
}

int isbanned(char *hostname)
{
  int i;
  struct ban_list_element *banned_node;
  char *nextchar;

  if (!hostname || !*hostname)
    return (0);

  i = 0;
  for (nextchar = hostname; *nextchar; nextchar++)
    *nextchar = LOWER(*nextchar);

  for (banned_node = ban_list; banned_node; banned_node = banned_node->next)
    if (strstr(hostname, banned_node->site.c_str()))	/* if hostname is a substring */
      i = MAX(i, banned_node->type);

  return i;
}


void _write_one_node(FILE * fp, struct ban_list_element * node)
{
  if (node) {
    _write_one_node(fp, node->next);
    fprintf(fp, "%s %s %ld %s\n", ban_types[node->type],
	    node->site.c_str(), (long) node->date, node->name);
  }
}



void write_ban_list(void)
{
  FILE *fl;

  if (!(fl = fopen("banned", "w"))) {
    perror("write_ban_list");
    return;
  }
  _write_one_node(fl, ban_list);/* recursively write from end to start */
  fclose(fl);
  return;
}

int do_ban(CHAR_DATA *ch, char *c_args, int cmd)
{
  string argument(c_args);
  string site;
  string buffer;
  int i;
  struct ban_list_element *ban_node;

  if (argument.empty())
  {
    if (!ban_list)
    {
      send_to_char("No sites are banned.\r\n", ch);
      return eSUCCESS;
    }
    
    buffer = fmt::format("{:<15}  {:<8}  {:<19}  {:<16}\r\n",
            "Banned Site Name",
            "Ban Type",
            "Banned On",
            "Banned By");
    send_to_char(buffer, ch);
    buffer = fmt::format("{:<15}  {:<8}  {:<19}  {:<16}\r\n",
            "-----------------------",
            "---------------------------------",
            "-------------------",
            "---------------------------------");
    send_to_char(buffer, ch);

    for (ban_node = ban_list; ban_node; ban_node = ban_node->next)
    {
      if (ban_node->date)
      {
        buffer = fmt::format("{:%Y-%m-%d %H:%M:%S}", *std::localtime(&(ban_node->date)));
      }
      else
      {
        buffer = "Unknown";
      }

      buffer = fmt::format("{:<15}  {:<8}  {:<19}  {:<16}\r\n", ban_node->site, ban_types[ban_node->type], buffer, ban_node->name);
      send_to_char(buffer, ch);
    }
    return eSUCCESS;
  }

  string flag;
  tie(flag, site) = half_chop(argument);
  if (site.empty() || flag.empty())
  {
    send_to_char("Usage: ban {all | select | new} IP address\r\n", ch);
    return eSUCCESS;
  }

  struct sockaddr_in sa;
  if (inet_pton(AF_INET, site.c_str(), &(sa.sin_addr)) == 0)
  {
    csendf(ch, "Invalid IP address.\n\r");
    return eFAILURE;
  }

  QString qflag = QString::fromStdString(flag);

 //QString::compare("aUtO", "AuTo", Qt::CaseInsensitive);  // x == 0
  if (!(!QString::compare(qflag, "all", Qt::CaseInsensitive) || !QString::compare(qflag, "select", Qt::CaseInsensitive) || !QString::compare(qflag, "new", Qt::CaseInsensitive)))
  {
    send_to_char("Flag must be ALL, SELECT, or NEW.\r\n", ch);
    return eSUCCESS;
  }
  for (ban_node = ban_list; ban_node; ban_node = ban_node->next)
  {
    if (ban_node->site == site)
    {
      send_to_char("That site has already been banned -- unban it to change the ban type.\r\n", ch);
      return eSUCCESS;
    }
  }

  CREATE(ban_node, struct ban_list_element, 1);
  for_each(site.begin(), site.end(), [](char &ch) {ch = ::tolower(ch);});
  ban_node->site = site;
  strncpy(ban_node->name, GET_NAME(ch), 100);
  ban_node->name[99] = '\0';
  ban_node->date = time(0);

  for (i = BAN_NEW; i <= BAN_ALL; i++)
    if (flag == ban_types[i])
      ban_node->type = i;

  ban_node->next = ban_list;
  ban_list = ban_node;

  buffer = fmt::format("{} has banned {} for {} players.", GET_NAME(ch), site,
          ban_types[ban_node->type]);
  log(buffer, LOG_GOD, POWER);
  send_to_char("Site banned.\r\n", ch);
  write_ban_list();
  return eSUCCESS;
}

int do_unban(CHAR_DATA *ch, char *argument, int cmd) {
  char site[MAX_INPUT_LENGTH+1];
  struct ban_list_element *ban_node, *temp;
  int found = 0;
  char buf[MAX_STRING_LENGTH];


  one_argument(argument, site);
  if (!*site) {
    send_to_char("A site to unban might help.\r\n", ch);
    return eSUCCESS;
  }
  ban_node = ban_list;
  while (ban_node && !found) {
    if (ban_node->site == site)
      found = 1;
    else
      ban_node = ban_node->next;
  }

  if (!found) {
    send_to_char("That site is not currently banned.\r\n", ch);
    return eSUCCESS;
  }
  REMOVE_FROM_LIST(ban_node, ban_list, next);
  send_to_char("Site unbanned.\r\n", ch);
  sprintf(buf, "%s removed the %s-player ban on %s.",
	  GET_NAME(ch), ban_types[ban_node->type], ban_node->site.c_str());
  log(buf, LOG_GOD, POWER);

  dc_free(ban_node);
  write_ban_list();
  return eSUCCESS;
}
