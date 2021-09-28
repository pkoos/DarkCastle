#include "connect.h"

descriptor_data::descriptor_data()
{
    descriptor = 0; /* file descriptor for socket	*/
    desc_num = 0;
    name = nullptr; /* Copy of the player name	*/
    memset(host, 0, 80);
    ;              /* hostname			*/
    connected = 0; /* mode of 'connectedness'	*/
    web_connected = 0;
    wait = 0;               /* wait for how many loops	*/
    showstr_head = nullptr; /* for paging through texts	*/
    showstr_vector = nullptr;
    showstr_count = 0;
    showstr_page = 0;
    new_newline = false; /* prepend newline in output	*/
    hashstr = nullptr;
    astr = nullptr;
    max_str = 0;                             /*	-			*/
    memset(buf, 0, 10 * MAX_INPUT_LENGTH);   /* buffer for raw input	*/
    memset(last_input, 0, MAX_INPUT_LENGTH); /* the last input	*/
    output = QByteArray();                      /* queue of strings to send	*/
    memset(inbuf, 0, MAX_RAW_INPUT_LENGTH);
    input = TXT_Q();     /* queue of unprocessed input	*/
    character = nullptr; /* linked to char		*/
    original = nullptr;  /* for switch / return		*/
    snooping = nullptr;  /* Who is this char snooping       */
    snoop_by = nullptr;  /* And who is snooping this char   */
    next = nullptr;      /* link to next descriptor	*/
    tick_wait = 0;       /* # ticks desired to wait	*/
    reallythere = 0;     /* Goddamm #&@$*% sig 13 (hack) */
    prompt_mode = 0;
    large_outbuf = nullptr;
    idle_tics = 0;
    login_time = time_t();
    stats = nullptr; // for rolling up a char

    strnew = nullptr; /* for the modify-str system	*/
    backstr = nullptr;
    idle_time = 0; // How long the descriptor has been idle, overall.
}