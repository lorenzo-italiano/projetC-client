#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <ncurses.h>
#include <form.h>
#include <assert.h>
#include <ctype.h>
#include <signal.h>

#include "global.c"
#include "internationalization/i18n.c"

#include "util/error.c"
#include "util/regex.c"
#include "util/util.c"

#include "ui/driver.c"
#include "ui/colors.c"

#include "socket/send.c"
#include "socket/clientSocket.c"
#include "socket/receive.c"
#include "util/ask.c"

#include "command/router.c"

#include "ui/utils.c"





/**
 * Main receiving loop.
 */
void *readingLoop(){
    while(1){
        char *message = receiveMessage();
        if (isMatch(message, commandList[1]->regex)) {
            // If client is receiving a file from another client.
            pthread_t pthread;
            pthread_create(&pthread, NULL, mpReceiveFileThreaded, message);
        }
        else if (isMatch(message, commandList[0]->regex)) {
            sendMessage(ENDING_MESSAGE);
        }
        else if(strcmp("", message) != 0){

            if(isMatch(message, "^MP ([^ ].*)")){
                char *list[3];
                getRegexGroup(list,message,"^MP ([^ ].*)");     /// TODO : free.
                window_print_blue(list[1]);
            }
            else if(isMatch(message, "^Me: ([^ ].*)")){
                window_print_magenta(message);
            }
            else if(isMatch(message, "->ALL : ([^ ].*)")){
                window_print_yellow(message);
            }
            else{
                window_print_white(message);
            }
            refresh_all();
        }
    }
}


/**
 * Client side.
 */
int main(int argc, char *argv[]) {
/**
 * Client start.
 */
    if(isDebugMode){
        printf(PROGRAM_START);
    }

    // Assigning function closeServer() to SIGTERM signal
    signal(SIGTERM, disconnectAction);   // Signal shutdown from ide.
    signal(SIGINT, disconnectAction);    // Signal shutdown from ctr+c in terminal.
    signal(SIGHUP, disconnectAction);    // Signal close terminal.


/**
 * Check arguments.
 */
    // Error if the number of arguments isn't valid.
    if(argc != 2){
        printf(ERROR_ARGS);
        printf("%s IP\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    IP = argv[1];

    initCommandList();
/**
 * Connection to the server.
 */
    printf(WAITING_SERVER_CONNECTION);
    acceptedSocketDescriptor = connectToServer(IP, CHANNEL_DEFAULT_PORT);


    // Ask user for username.
    // username is a global variable.
    askForUsername();


/**
 * Exchange beginning.
 */

// Lancer le thread de lecture
    pthread_t pthread;
    if (pthread_create(&pthread, NULL, readingLoop, NULL)) {
        throwError(ERROR_THREAD_CREATION, 0);
    }

    int ch;

    interface_init();

    wprintw(win_display, "Hello and welcome on the chat !\n");

    form_initialisation();

    set_field_buffer(fields[1], 0, "Message..");

    set_title("Salon 2");

    doCommandAction("/help\n");
//    doCommandAction("/join 3\n");

    refresh_all();

    // Lancer le thread d'écriture
    while(1){ // Sending messages to server
        ch = getch();
        driver(ch);
    }
}
