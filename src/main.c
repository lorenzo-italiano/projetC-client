#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]) {
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


    char* str1 = (char*)malloc(sizeof(char)*256);
    printf("Entrez str1 \n");
    int scanfReturn = scanf("%s",str1);
    if(scanfReturn == -1){
        printf("Erreur lors de la connection au socket. \n");
        printf("%s", strerror(errno));
        return EXIT_FAILURE;
    } else { printf("Vous avez entré le message suivant: %s \n",str1); }

    int taille = htonl(strlen(str1));


    int sendSizeReturn = send(dS, &taille, sizeof(int), 0);
    // verif erreur
    if(sendSizeReturn == -1){
        printf("Erreur lors de l'envoi du message. \n");
        printf("%s", strerror(errno));
        return EXIT_FAILURE;
    } else { printf("Taille du message envoyé \n"); }


    int sendReturn = send(dS, str1, sizeof(char)*(strlen(str1)+1), 0);
    // verif erreur
    if(sendReturn == -1){
        printf("Erreur lors de l'envoi du message. \n");
        printf("%s", strerror(errno));
        return EXIT_FAILURE;
    } else { printf("Message Envoyé \n"); }

    int r;
    int recvReturn = recv(dS, &r, sizeof(int), 0);
    // verif erreur
    if(recvReturn == -1){
        printf("La réception de la reponse a echoué !");
        return EXIT_FAILURE;
    } else { printf("Réponse reçue : %d\n", r); }

    shutdown(dS,2);
    printf("Fin du programme. \n");
}