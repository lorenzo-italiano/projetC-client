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
        throwError("La connexion avec le serveur distant a été fermée. \n", 0);
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
        throwError("La connexion avec le serveur distant a été fermée. \n", 0);
    }

    if(isMatch(message, "^MP ([^ ].*)")){
        setBlueText();
        char *list[3];
        getRegexGroup(list,message,"^MP ([^ ].*)");
        return list[1];
        //printf("%s \n", list[1]);
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