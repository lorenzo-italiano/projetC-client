#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include "color.c"
#define MAX_STRING_SIZE 256
#define ENDING_MESSAGE "Fin\n\0"

int acceptedSocketDescriptor;
char* pseudo;
int isDebugMode = 0;

/**
 * Perror the message in params and exit the programme.
 * If hasErrno, print it.
 * @param errorMessage
 * @param hasErrno
 */
void throwError (char *errorMessage, int hasErrno) {
    perror(errorMessage);
    if (hasErrno) {
        printf("%s", strerror(errno));
    }
    exit(EXIT_FAILURE);
}

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
        throwError("Erreur lors de la connection au socketDescriptor. \n", 1);
    }
    else {
        if(isDebugMode){
            printf("Socket Connecté. \n");
        }
    }
    return socketDescriptor;
}

/**
 * Ask the user for a message, in the console.
 * @return
 */
char *askUserForString () {
    char *message = (char *) malloc(sizeof(char) * MAX_STRING_SIZE);

    if(isDebugMode){
        printf("Entrez votre message: \n");
    }
    if (fgets(message, MAX_STRING_SIZE, stdin) == NULL) {
        throwError("Error fgets. \n", 1);
    }
    return message;
}

/**
 * For an accepted socket descriptor in params, send a message of type int.
 * This message is usually used to send the message's size which will be sent after.
 *
 * @param messageSize
 */
void sendMessageInt (int messageSize) {
    if(send(acceptedSocketDescriptor, &messageSize, sizeof(int), 0) == -1){
        throwError("Erreur lors de l'envoi du message. \n", 1);
    }
    else {
        if(isDebugMode){
            printf("Taille du message envoyée. \n");
        }
    }
}

/**
 * For an accepted socket descriptor and a message size, send the message in params.
 *
 * @param message
 * @param size
 */
void sendMessageString (char *message, int size) {
    if(send(acceptedSocketDescriptor, message, sizeof(char) * size, 0) == -1){
        throwError("Erreur lors de l'envoi du message. \n", 1);
    }
    else {
        setPurpleText();
        printf("%s: %s", pseudo, message);
        setWhiteText();
    }
}

/**
 *
 */
void sendMessage(char *userMessage){
    int messageSize = (int)strlen(userMessage);
    if(isDebugMode){
        printf("Size of sent message: %d \n", messageSize);
    }

    // Send message size.
    sendMessageInt(messageSize);
    // Send message.
    sendMessageString(userMessage, messageSize);
}

/**
* Shutdown the client.
*/
void shutdownClient () {
    sendMessage(ENDING_MESSAGE);
    shutdown(acceptedSocketDescriptor, 2);
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

/**
 * For an accepted socket descriptor, wait until receiving an int message.
 * This message is usually the size of the message which will be sent just after.
 *
 * @return the size of the message which will be sent after.
 */
int receiveMessageInt () {
    int size;
    int recvInt = recv(acceptedSocketDescriptor, &size, sizeof(int), 0);
    if(recvInt == -1){
        throwError("Erreur lors de la reception du message. \n", 0);
    }
    else if(recvInt == 0){
        throwError("La connexion avec le serveur distant a été fermée \n", 0);
    }
    return size;
}

/**
 * For an accepted socket descriptor, wait until receiving a message of the messageSize in params.
 *
 * @param messageSize
 * @return the gotten message.
 */
char *receiveMessageString (int messageSize) {
    char *message = (char*)malloc(sizeof(char) * (messageSize + 1));

    int recvInt = recv(acceptedSocketDescriptor,message, sizeof(char) * messageSize, 0);
    if(recvInt == -1){
        throwError("Erreur lors de la reception du message. \n", 0);
    }
    else if(recvInt == 0){
        throwError("La connexion avec le serveur distant a été fermée \n", 0);
    }
    return message;
}

/**
 * Wait until receiving a message.
 */
char *receiveMessage(){
    int messageSize = receiveMessageInt();
    return receiveMessageString(messageSize);
}

/**
 * Main receiving loop.
 */
void readingLoop(){
    while(1){
        // If we are in wait mode, we wait until the second client sends a message.
        char *message = receiveMessage();
        if(strcmp("", message)!=0){
            printf("%s", message);
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
    signal(SIGTERM, shutdownClient);   // Signal shutdown from ide.
    signal(SIGINT, shutdownClient);    // Signal shutdown from ctr+c in terminal.


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


/**
 * Connection to the server.
 */
    acceptedSocketDescriptor = connectToServer(argv[1], atoi(argv[2]));


/**
 * Exchange beginning.
 */

// Lancer le thread de lecture
    pthread_t pthread;
    if (pthread_create(&pthread, NULL, readingLoop, acceptedSocketDescriptor)) {
        throwError("Error:unable to create thread, %d\n", 0);
    }

/**
 * Ask user for pseudo
 */

    printf("Entrez votre pseudo: ");
    pseudo = askUserForString();
    pseudo[strlen(pseudo)-1] = '\0';
    sendMessage(pseudo);
    printf("\n");

// Lancer le thread d'écriture
    while(1){ // Sending messages to server
        // If we are in send mode, we have to send a message to the server which will transfer it to the waiting client.
        char *userMessage = askUserForString();
        sendMessage(userMessage);
        if (strcmp(ENDING_MESSAGE, userMessage) == 0) {
            // Shutdown the client.
            shutdownClient();
        }
    }
}
