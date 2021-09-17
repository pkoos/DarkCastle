/* $Id: nlog.cpp,v 1.12 2014/07/04 22:00:04 jhhudso Exp $ */

extern "C"
{
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
}
#include "connect.h"
#include "character.h"
#include "utility.h"
#include "terminal.h"
#include "player.h" // PLR_ANSI
#include "utility.h"

void send_to_char(const char *messg, CHAR_DATA *ch);

/*
 * logf, str_hsh, and csendf by Sadus, others by Ysafar.
 */

struct hash_info
{
  char *name;
  struct hash_info *left;
  struct hash_info *right;
};

struct hash_info tree = {"m", 0, 0};
//struct hash_info nulltree = {"", 0, 0};

void kill_hsh_tree_func( struct hash_info * leaf )
{
  if(leaf->left)
    kill_hsh_tree_func(leaf->left);
  if(leaf->right)
    kill_hsh_tree_func(leaf->right);

  dc_free(leaf->name);
  dc_free(leaf);
}

// Since the top of the tree is static, we have to free both sides
void free_hsh_tree_from_memory( )
{
   if(tree.left)
     kill_hsh_tree_func( tree.left );
   if(tree.right)
     kill_hsh_tree_func( tree.right );
}

bool ishashed(char *arg)
{
  struct hash_info *current = &tree;
  for(; current; current = current->right) {
   if (current->name == arg)
     return TRUE;
  }
  return FALSE;
}

char *str_hsh(const char *arg)
{
  int scratch;
  struct hash_info *current = &tree;
  struct hash_info *next;
  struct hash_info *temp;

  // Second spot for "" args so we don't leak them all over the place
  //if(*arg == '\0')
  //  return(nulltree.name);
  if (!arg) return NULL;

  for(; current; current = next) {
     if((scratch = strcmp(arg, current->name)) == 0)
       return(current->name);

     if(scratch < 0)
       next = current->left;
     else
       next = current->right;
     temp = current;
  }

  current = new hash_info;

  current->right = current->left = NULL;
  if(scratch < 0)
    temp->left  = current;
  else
    temp->right = current;
  current->name   = new char[strlen(arg)+1];
  memset(current->name, 0, strlen(arg)+1);
  strncpy(current->name, arg, strlen(arg));

  return(current->name);
}


/* logf(GET_LEVEL(ch), LOG_GOD, "%s restored all!", GET_NAME(ch)); */
void logf(int level, long type, const char *arg, ...)
{ 
  va_list args;
  char s[MAX_STRING_LENGTH];

  va_start(args, arg);
  vsnprintf(s, MAX_STRING_LENGTH, arg, args);
  va_end(args);

  log(s, level, type);
}

int csendf(struct char_data *ch, const char *fmt, ...)
{ 
  va_list args1, args2;

  va_start(args1, fmt);

  // backup the variable argument list to args2
  va_copy(args2, args1);

  auto size = vsnprintf(nullptr, 0, fmt, args1);
  va_end(args1);

  // create vector to hold incoming vsnprintf output
  vector<char> vbuffer(size+1);
  vsnprintf(vbuffer.data(), vbuffer.size(), fmt, args2);
  va_end(args2);

  string buffer = vbuffer.data();;
  send_to_char(buffer, ch);

  return 1;
}


