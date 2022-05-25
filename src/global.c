#define MAX_SIZE_SENT 512
#define ENDING_MESSAGE "/disconnect\n\0"
char *IP;
#define NB_COMMAND 4

#define CHANNEL_DEFAULT_PORT 3000
#define CHANNEL_SWITCH_PORT 3334

FORM *form;
FIELD *fields[2];
WINDOW *win_screen;
WINDOW *win_form;
WINDOW *win_display;
WINDOW *sub_win_form;
WINDOW *win_title;

int acceptedSocketDescriptor;
char *username;
int isDebugMode = 0;
