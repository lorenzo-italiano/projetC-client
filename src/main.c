#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#define MAX_STRING_SIZE 256


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
        printf("Socket Créé. \n");
    }
    return socketDescriptor;
}


/**
 * Find the socket a the adress in params and establish a connection with it.
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
        printf("Socket Connecté. \n");
    }
    return socketDescriptor;
}

/**
 * For an accepted socket descriptor in params, wait until receiving an int message.
 * This message is usually the size of the message which will be sent just after.
 *
 * @param acceptedSocketDescriptor
 * @return the size of the message which will be sent after.
 */
int receiveMessageInt (int acceptedSocketDescriptor) {
    int size;
    if(recv(acceptedSocketDescriptor, &size, sizeof(int), 0) == -1){
        throwError("Erreur lors de la reception du message. \n", 0);
    }
    return size;
}

/**
 * For an accepted socket descriptor in params, wait until receiving a message of the messageSize in params.
 *
 * @param acceptedSocketDescriptor
 * @param messageSize
 * @return the gotten message.
 */
char *receiveMessageString (int acceptedSocketDescriptor, int messageSize) {
    char *message = (char*)malloc(sizeof(char) * (messageSize + 1));

    if(recv(acceptedSocketDescriptor, message, sizeof(char) * messageSize, 0) == -1){
        throwError("Erreur lors de la reception du message. \n", 0);
    }
    return message;
}

/**
 *
 * @param acceptedSocketDescriptor
 */
char *receiveMessage(int acceptedSocketDescriptor){
    printf("Waiting for a message\n");
    int messageSize = receiveMessageInt(acceptedSocketDescriptor);
    return receiveMessageString(acceptedSocketDescriptor, messageSize);
}

char *askUserForString () {
    char *message = (char *) malloc(sizeof(char) * MAX_STRING_SIZE);

    printf("Entrez votre message: \n");
    if (fgets(message, MAX_STRING_SIZE, stdin) == NULL) {
        throwError("Error fgets. \n", 1);
    }
    return message;
}

/**
 * For an accepted socket descriptor in params, send a message of type int.
 * This message is usually used to send the message's size which will be sent after.
 *
 * @param acceptedSocketDescriptor
 * @param messageSize
 */
void sendMessageInt (int acceptedSocketDescriptor, int messageSize) {
    if(send(acceptedSocketDescriptor, &messageSize, sizeof(int), 0) == -1){
        throwError("Erreur lors de l'envoi du message. \n", 1);
    }
    else {
        printf("Taille du message envoyée. \n");
    }
}

/**
 * For an accepted socket descriptor and a message size, send the message in params.
 *
 * @param acceptedSocketDescriptor
 * @param message
 * @param size
 */
void sendMessageString (int acceptedSocketDescriptor, char *message, int size) {
    if(send(acceptedSocketDescriptor, message, sizeof(char) * size, 0) == -1){
        throwError("Erreur lors de l'envoi du message. \n", 1);
    }
    else {
        printf("Message envoyé. \n");
    }
}

/**
 *
 * @param acceptedSocketDescriptor
 */
void sendMessage(int acceptedSocketDescriptor, char *userMessage){
    int messageSize = (int)strlen(userMessage);
    printf("Size of sent message: %d \n", messageSize);

    // Send message size.
    sendMessageInt(acceptedSocketDescriptor, messageSize);
    // Send message.
    sendMessageString(acceptedSocketDescriptor, userMessage, messageSize);
}

/**
 * @param socket
 * @return current client status
 */
char* receiveStatus(int socket){
    char *response = (char*)malloc(sizeof(char)*5);
    int recvReturn = recv(socket, response, sizeof(response), 0);
    // verif erreur
    if(recvReturn == -1){
        printf("La réception de la reponse a echoué !");
        exit(EXIT_FAILURE);
    }
    else {
        printf("Réponse reçue : %s\n", response);
    }
    return response;
}

void readingLoop(int acceptedSocketDescriptor){
    while(1){
        // If we are in wait mode, we wait until the second client sends a message.
        char *message = receiveMessage(acceptedSocketDescriptor);
        printf("Message reçu : %s\n", message);
    }
}

/**
 * Client side.
 */
int main(int argc, char *argv[]) {
/**
 * Client start.
 */
    printf("Début programme\n");


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
    int socketDescriptor = connectToServer(argv[1], atoi(argv[2]));


/**
 * Exchange beginning.
 */

// Lancer le thread de lecture

pthread_t pthread;

int testThread = pthread_create(&pthread,NULL,readingLoop,socketDescriptor);

if (testThread) {
    throwError("Error:unable to create thread, %d\n", 0);
}

// Lancer le thread d'écriture
while(1){ // Sending messages to server
    // If we are in send mode, we have to send a message to the server which will transfer it to the waiting client.
    char* userMessage = askUserForString();
    sendMessage(socketDescriptor, userMessage);
}


/**
 * Shutdown the client.
 */
    shutdown(socketDescriptor, 2);
    printf("Fin du programme. \n");
}