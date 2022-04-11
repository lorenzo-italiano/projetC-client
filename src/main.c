#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]) {

    /*-----------------------------------------
     *
     *          Setting up the client
     *
     *-----------------------------------------
     */

    //erreur si pas assez d'arguments
    if(argc != 3){
        printf("Erreur: Nombre d'arguments invalide. Utilisation :\n");;
        printf("%s IP PORT\n", argv[0]);
        return EXIT_FAILURE;
    }

    //erreur si port != 4 chiffres (argv 2)
    if(strlen(argv[2])!= 4){
        printf("Erreur: Un port doit être composé de 4 chiffres !");
        return EXIT_FAILURE;
    }

    printf("Début programme\n");
    int dS = socket(PF_INET, SOCK_STREAM, 0);
    // verif erreur
    if(dS == -1){
        printf("La création du socket a echoué !");
        return EXIT_FAILURE;
    } else { printf("Socket Créé\n"); }

    struct sockaddr_in aS;
    aS.sin_family = AF_INET;
    int inet = inet_pton(AF_INET,argv[1],&(aS.sin_addr));
    // verif erreur
    if(inet == -1){
        printf("La conversion d'adresse IP a echoué !");
        return EXIT_FAILURE;
    }
    aS.sin_port = htons(atoi(argv[2]));
    socklen_t lgA = sizeof(struct sockaddr_in);
    int connectTest = connect(dS, (struct sockaddr *) &aS, lgA);
    // verif erreur
    if(connectTest == -1){
        printf("Erreur lors de la connection au socket. \n");
        printf("%s", strerror(errno));
        return EXIT_FAILURE;
    } else { printf("Socket Connecté\n"); }

    /*----------------------------------
     *
     *              BEGIN
     *
     *----------------------------------
     */

    // Receiving our current state from server

    char* response = (char*)malloc(sizeof(char)*5);
    int recvReturn = recv(dS, response, sizeof(response), 0);
    // verif erreur
    if(recvReturn == -1){
        printf("La réception de la reponse a echoué !");
        return EXIT_FAILURE;
    } else { printf("Réponse reçue : %s\n", response); }

    // check state and apply actions
    if(strcmp(response,"wait\0")==0){
        // If we are in wait mode, we wait until the second client sends a message
        printf("Waiting for a message\n");

        int size;
        if(recv(dS, &size, sizeof(int), 0)==-1){
            printf("Erreur lors de la reception du message. \n");
            return EXIT_FAILURE;
        } else { printf("Message reçu : %d\n", size); }

        int test = ntohl(size);

        char* msg = (char*)malloc(sizeof(char)*(test+1));

        if(recv(dS, msg, sizeof(char)*test, 0)==-1){
            printf("Erreur lors de la reception du message. \n");
            return EXIT_FAILURE;
        } else { printf("Message reçu : %s\n", msg); }
    }
    else{
        // If we are in send mode, we have to send a message to the server which will tranfer it to the waiting client
        char* str1 = (char*)malloc(sizeof(char)*256);

        printf("Entrez votre message: \n");
        int scanfReturn = scanf("%s",str1);

        // checking if scanf has failed
        if(scanfReturn == -1){
            printf("Erreur lors de la connection au socket. \n");
            printf("%s", strerror(errno));
            return EXIT_FAILURE;
        } else { printf("Vous avez entré le message suivant: %s \n",str1); }

        int taille = htonl(strlen(str1));
        // Sending our message size to server for it to receive the whole message
        int sendSizeReturn = send(dS, &taille, sizeof(int), 0);
        // verif erreur
        if(sendSizeReturn == -1){
            printf("Erreur lors de l'envoi du message. \n");
            printf("%s", strerror(errno));
            return EXIT_FAILURE;
        } else { printf("Taille du message envoyé \n"); }

        // Sending the message to the server
        int sendReturn = send(dS, str1, sizeof(char)*(strlen(str1)+1), 0);
        // verif erreur
        if(sendReturn == -1){
            printf("Erreur lors de l'envoi du message. \n");
            printf("%s", strerror(errno));
            return EXIT_FAILURE;
        } else { printf("Message Envoyé \n"); }
    }

    /*---------------------------------------------
     *
     *          Shutting down the client
     *
     *---------------------------------------------
     */

    shutdown(dS,2);
    printf("Fin du programme. \n");
}