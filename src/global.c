#define MAX_SIZE_SENT 512
#define ENDING_MESSAGE "/disconnect\n\0"
#define IP "127.0.0.1"
#define PORT_SOCKET_FILE 3333
#define NB_COMMAND 4
#define CHANNEL_SWITCH_PORT 3000

int acceptedSocketDescriptor;
char *username;
int isDebugMode = 0;