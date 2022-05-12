#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include "global.c"

#include "util/color.c"
#include "util/error.c"
#include "util/regex.c"
#include "util/util.c"

#include "socket/receive.c"
#include "socket/send.c"
#include "socket/clientSocket.c"
#include "util/ask.c"

#include "command/router.c"



/**
 * Main receiving loop.
 */
void *readingLoop(){
    while(1){
        // If we are in wait mode, we wait until the second client sends a message.
        char *message = receiveMessage();
        if(strcmp("", message)!=0){
            printf("%s", message);
            setWhiteText();
        }
    }
}


/**
 * Client side.
 */
int main(int argc, char *argv[]) {
    //set default text color
    setWhiteText();
/**
 * Client start.
 */
    if(isDebugMode){
        printf("Début programme\n");
    }

    // Assigning function closeServer() to SIGTERM signal
    signal(SIGTERM, disconnectAction);   // Signal shutdown from ide.
    signal(SIGINT, disconnectAction);    // Signal shutdown from ctr+c in terminal.


/**
 * Check arguments.
 */
    // Error if the number of arguments isn't valid.
    if(argc != 3){
        printf("Erreur: Nombre d'arguments invalide. Utilisation :\n");;
        printf("%s IP PORT\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Erreur si port < 1024 (argv 2)
    if(atoi(argv[2]) < 1024){
        throwError("Erreur: le port doit être supérieur à 1024. \n", 0);
    }

    initCommandList();

/**
 * Connection to the server.
 */
    printf("Waiting for connection to server. \n");
    acceptedSocketDescriptor = connectToServer(argv[1], atoi(argv[2]));


    // Ask user for username.
    // username is a global variable.
    askForUsername();


/**
 * Exchange beginning.
 */

// Lancer le thread de lecture
    pthread_t pthread;
    if (pthread_create(&pthread, NULL, readingLoop, NULL)) {
        throwError("Error:unable to create thread, %d\n", 0);
    }

// Lancer le thread d'écriture
    while(1){ // Sending messages to server
        // If we are in send mode, we have to send a message to the server which will transfer it to the waiting client.
        char *userMessage = askUserForString();

        // Execute the command associated or send the message to the server.
        doCommandAction(userMessage);
    }
}
