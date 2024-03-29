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
#include <sys/ioctl.h>

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

            if(isMatch(message, "->Me : ")){
                // Message received from mp.
                window_print_yellow(message);
            }
            else if(isMatch(message, "^Me : ") || isMatch(message, "^Me->")){
                // Message sent by the current user.
                window_print_magenta(message);
            }
            else if(isMatch(message, "->ALL : ")){
                // Message received from /all.
                window_print_cyan(message);
            }
            else{
                // Normal message.
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

    printf("\033[0;31m");
    printf(TERMINAL_SIZE_WARNING);
    printf("\033[0;37m");


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

    signal(SIGWINCH, resize_window);

    wprintw(win_display, "Hello and welcome on the chat !\n");

    form_initialisation();

    doCommandAction("/join 3\n");
    doCommandAction("/help\n");

    refresh_all();

    // Lancer le thread d'écriture
    while(1){ // Sending messages to server
        ch = getch();
        driver(ch);
    }
}
