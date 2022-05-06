#include <arpa/inet.h>

/**
 * Create a socket and return his descriptor.
 *
 * @return the descriptor of the socket created.
 */
int createSocket () {
    int socketDescriptor = socket(PF_INET, SOCK_STREAM, 0);
    if (socketDescriptor == -1) {
        throwError("Erreur lors de la création du socket. \n", 0);
    }
    else {
        if(isDebugMode){
            printf("Socket Créé. \n");
        }
    }
    return socketDescriptor;
}


/**
 * Find the socket a the address in params and establish a connection with it.
 *
 * @param ip
 * @param port
 * @return
 */
int connectToServer(char *ip, int port){
    int socketDescriptor = createSocket();

    struct sockaddr_in aS;
    aS.sin_family = AF_INET;
    aS.sin_port = htons(port);

    int inet = inet_pton(AF_INET, ip, &(aS.sin_addr));
    if (inet == -1){
        throwError("La conversion d'adresse IP a echouée. \n", 0);
    }

    socklen_t lgA = sizeof(struct sockaddr_in);

    if(connect(socketDescriptor, (struct sockaddr *) &aS, lgA) == -1){
        throwError("Erreur lors de la connexion au socketDescriptor. \n", 1);
    }
    else {
        if(isDebugMode){
            printf("Socket Connecté. \n");
        }
    }
    return socketDescriptor;
}

/**
* Shutdown the client.
*/
void shutdownClient () {
    sendMessage(ENDING_MESSAGE);
    printf("Fin du programme. \n");
    exit(EXIT_SUCCESS);
}

/**
* Shutdown the thread.
*/
void shutdownThread () {
    //sendMessage(ENDING_MESSAGE);
    shutdown(acceptedSocketDescriptor, 2);
    printf("Fin du thread de lecture. \n");
    pthread_exit(NULL);
}
