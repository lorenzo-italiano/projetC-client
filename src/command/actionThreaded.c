
// Struct used for threaded function.
struct paramFileThreaded {
    char *message;
    char *username;
    char *filename;
};

/**
 * Threaded function called by fileAction(...).
 * Connect to the socket used to send files and send the file.
 *
 * @param filename
 * @return
 */
void *sendFileThreaded (struct paramFileThreaded *param) {
    FILE *file = openFile(param->filename, "rb");

    if (file != NULL) {
        // File exists.
        sendMessage(param->message);

        // Connection to the socket used to send files.
        int clientSocketForFile = connectToServer(IP, PORT_SOCKET_FILE);
        sendFile(clientSocketForFile, file);
        close(clientSocketForFile);
        printf(FILE_SENT);
    }
    else {
        // File doesn't exists.
        printf(FILE_NOT_FOUND);
    }

    free(param->message);
    free(param->username);
    free(param->filename);
    free(param);

    pthread_exit(NULL);
}

/**
 * Request a file to the server.
 * If the file exists, put it in ./uploads/.
 *
 * @param filename
 * @return
 */
void *receiveFileThreaded (struct paramFileThreaded *param) {
    sendMessage(param->message);

    // Connection to the socket used to send files.
    int clientSocketForFile = connectToServer(IP, PORT_SOCKET_FILE);
    int requestStatus;

    recv(clientSocketForFile, &requestStatus, sizeof(int), 0);

    if (requestStatus == 404) {
        // File not found.
        printf(FILE_NOT_FOUND);
    }
    else {
        // Status == 204. File found.
        receiveFile(clientSocketForFile, param->filename);
        printf(FILE_IMPORTED);
        close(clientSocketForFile);
    }

    free(param->message);
    free(param->username);
    free(param->filename);
    free(param);

    pthread_exit(NULL);
}

/**
 * Threaded function called when a user wanna mp a file to another user.
 *
 * @param param
 * @return
 */
void *mpSendFileThreaded (struct paramFileThreaded *param) {
    // Check file exists locally.
    // Send message to serv.
    // Connect to second socket.
    // Server asert that the wanted user is connected.
        // If connected, send the file.

    FILE *file = openFile(param->filename, "rb");

    if (file != NULL) {
        // File exists.

        sendMessage(param->message);

        // Connection to the socket used to send files.
        int clientSocketForFile = connectToServer(IP, PORT_SOCKET_FILE);
        int userTargetStatus;
        recv(clientSocketForFile, &userTargetStatus, sizeof(int), 0);   /// TODO : modif recv msg int avec socket en params.

        if (userTargetStatus == 204) {
            if (isDebugMode) { printf("Send file. \n"); }
            sendFile(clientSocketForFile, file);
        }
        else {
            // userTargetStatus == 404
            printf(USER_NOT_FOUND);
        }
        close(clientSocketForFile);
    }
    else {
        // File doesn't exists.
        printf(FILE_NOT_FOUND);
    }


    free(param->message);
    free(param->username);
    free(param->filename);
    free(param);

    pthread_exit(NULL);
}

/**
 * Threaded function called when a user receive a message saying that he will receive a file from mp.
 *
 * @param message
 * @return
 */
void *mpReceiveFileThreaded (void *message) {
    // Connection to the socket used to send files.
    int clientSocketForFile = connectToServer(IP, PORT_SOCKET_FILE);

    char *regexGroupList[3];
    getRegexGroup(regexGroupList, message, commandList[1]->regex);
    // regexGroupList[0] = message
    // regexGroupList[1] = -send || -get || username
    // regexGroupList[2] = filename

    receiveFile(clientSocketForFile, regexGroupList[2]);

    free(regexGroupList[0]);
    free(regexGroupList[1]);
    free(regexGroupList[2]);

    close(clientSocketForFile);
    pthread_exit(NULL);
}
