#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    //erreur si pas assez d'arguments
    if(argc != 3){
        perror("Pas assez d'arguments (./fonction IP PORT)");
        exit(1);
    }

    //erreur si port != 4 chiffres (argv 2)
    if(strlen(argv[2])!= 4){
        perror("Un port doit être composé de 4 chiffres !");
        exit(1);
    }

    //verif argv 1 correspond a une ip

    printf("Début programme\n");
    int dS = socket(PF_INET, SOCK_STREAM, 0);
    // verif erreur
    if(dS == -1){
        perror("La création du socket a echoué !");
        exit(1);
    }
    printf("Socket Créé\n");

    struct sockaddr_in aS;
    aS.sin_family = AF_INET;
    int inet = inet_pton(AF_INET,argv[1],&(aS.sin_addr));
    // verif erreur
    if(inet == -1){
        perror("La conversion d'adresse IP a echoué !");
        exit(1);
    }
    aS.sin_port = htons(atoi(argv[2]));
    socklen_t lgA = sizeof(struct sockaddr_in);
    int connectTest = connect(dS, (struct sockaddr *) &aS, lgA);
    // verif erreur
    if(connectTest == -1){
        perror("La conversion d'adresse IP a echoué !");
        exit(1);
    }
    printf("Socket Connecté\n");

    char* str1 = (char*)malloc(sizeof(char)*256);
    printf("Entrez str1 \n");
    int scanfReturn = scanf("%s",str1);
    if(scanfReturn == -1){
        exit(1);
    }
    printf("%s \n",str1);

    int taille = htonl(strlen(str1));

    //ssize_t taille = 2;


    int sendSizeReturn = send(dS, &taille, sizeof(int), 0);
    // verif erreur
    if(sendSizeReturn == -1){
        perror("L'envoie du message a echoué !");
        exit(1);
    }


    int sendReturn = send(dS, str1, sizeof(char)*(strlen(str1)+1), 0);
    // verif erreur
    if(sendReturn == -1){
        perror("L'envoie du message a echoué !");
        exit(1);
    }
    printf("Message Envoyé \n");

    int r;
    int recvReturn = recv(dS, &r, sizeof(int), 0);
    // verif erreur
    if(recvReturn == -1){
        perror("La réception de la reponse a echoué !");
        exit(1);
    }
    printf("Réponse reçue : %d\n", r);

    int shutdownReturn = shutdown(dS,2);
    // verif erreur
    if(shutdownReturn == -1){
        perror("La fonction shutdown a echoué !");
        exit(1);
    }
    printf("Fin du programme \n");
}