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

/**
 * Receive a file from a server.
 *
 * @param serverFileSocket
 * @param fileName
 */
void receiveFile(int serverFileSocket, char *fileName){
    // Get file blocSize.
    long filelen;
    recv(serverFileSocket, &filelen, sizeof(long), 0);

    // Get current path.
    char filePath[200];
    getUploadDirectoryPath(filePath);
    strcat(filePath, fileName);

    // File creation.
    FILE *file;
    file = fopen(filePath, "w+");


    long blocSize = MAX_SIZE_SENT;
    char subBuffer[blocSize];
    for (int i = 0; i < filelen; i += MAX_SIZE_SENT) {
        blocSize = (i + MAX_SIZE_SENT < filelen) ? MAX_SIZE_SENT : filelen - i; // Calculate the bloc's blocSize.
        recv(serverFileSocket, subBuffer, blocSize, 0);
        fwrite(subBuffer, blocSize, 1, file);
        bzero(subBuffer, MAX_SIZE_SENT);    // Clear the buffer.
    }

    fclose(file);
}
