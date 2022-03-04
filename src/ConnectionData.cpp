
#include "ConnectionData.h"

class ConnectionData {
    public:
        int descriptor; /* file descriptor for socket	*/
        int desc_num;
        char *name; /* Copy of the player name	*/
        char host[80]; /* hostname			*/
        conn connected; /* mode of 'connectedness'	*/
        int web_connected;
        int wait; /* wait for how many loops	*/
        char *showstr_head; /* for paging through texts	*/
        const char **showstr_vector;
        int showstr_count;
        int showstr_page;
        bool new_newline; /* prepend newline in output	*/
        //  char	**str;			/* for the modify-str system	*/
        char **hashstr;
        char *astr;
        int max_str; /*	-			*/
        char buf[10 * MAX_INPUT_LENGTH]; /* buffer for raw input	*/
        char last_input[MAX_INPUT_LENGTH]; /* the last input	*/
        char *output; /* queue of strings to send	*/
        char inbuf[MAX_RAW_INPUT_LENGTH];
        TXT_Q input; /* queue of unprocessed input	*/
        CHAR_DATA *character; /* linked to char		*/
        CHAR_DATA *original; /* for switch / return		*/
        ConnectionData *snooping; /* Who is this char snooping       */
        ConnectionData *snoop_by; /* And who is snooping this char   */
        ConnectionData *next; /* link to next descriptor	*/
        int tick_wait; /* # ticks desired to wait	*/
        int reallythere; /* Goddamm #&@$*% sig 13 (hack) */
        int prompt_mode;
        txt_block *large_outbuf;
        int bufptr;
        int bufspace;
        char small_outbuf[SMALL_BUFSIZE];
        ubyte idle_tics;
        time_t login_time;
        stat_data *stats;            // for rolling up a char

        char **strnew; /* for the modify-str system	*/
        char *backstr;
        int idle_time; // How long the descriptor has been idle, overall.
        bool color;
};