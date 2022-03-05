/***************************************************************************
| comm.h: New comm stuff to make the rewrite of comm.C work properly
*/
#ifndef __COMM_H__
#define __COMM_H__

#include "BanAction.h"
#include "Pulses.h"
#include <string>

using namespace std;
typedef int socket_t;

#define SEND_TO_Q(messg, desc)  write_to_output((messg), desc)
#ifndef WIN32
#define CLOSE_SOCKET(sock) close(sock)
#else
#define CLOSE_SOCKET(sock) closesocket(sock)
#endif

#define SMALL_BUFSIZE           1024
#define LARGE_BUFSIZE           (24 * 2048)
#define GARBAGE_SPACE           32
#define NUM_RESERVED_DESCS      8

#define HOST_LENGTH             30

struct pulse_info {
	pulse_type pulse;
	uint64_t duration;
	char name[];
};

struct txt_block
{
    char *text;
    struct txt_block *next;
    int aliased;
};

typedef struct txt_q
{
    struct txt_block *head;
    struct txt_block *tail;
} TXT_Q;

#include "character.h"

//void     write_to_output(const char *txt, struct descriptor_data *d);
void     write_to_output(const char *txt, struct descriptor_data *d);
void     scramble_text(char * txt);
void     warn_if_duplicate_ip(char_data * ch);
void     record_msg(char *messg, char_data *ch);
int      write_hotboot_file(char **argv);
void  send_info(string messg);
void  send_info(const char *messg);
bool is_multi(char_data *ch);
void new_string_add(struct descriptor_data *d, char *str);
void telnet_ga(descriptor_data *d);
int load_hotboot_descs(void);
void finish_hotboot(void);
void signal_setup(void);

#endif
