
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
void *sendFileThreaded (void *paramVoid) {
    struct paramFileThreaded *param = (struct paramFileThreaded *)paramVoid;

    FILE *file = openFile(param->filename, "rb");

    if (file != NULL) {
        // File exists.
        sendMessage(param->message);

        // Receive the port of the socket used to transfer files, by the switch socket.
        int portSocketForFile = receiveNewPort();

        // Connection to the socket used to send files.
        int clientSocketForFile = connectToServer(IP, portSocketForFile);
        sendFile(clientSocketForFile, file);
        close(clientSocketForFile);
        window_print_green(FILE_SENT);
    }
    else {
        // File doesn't exists.
        window_print_red(FILE_NOT_FOUND);
    }

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
void *receiveFileThreaded (void *paramVoid) {
    struct paramFileThreaded *param = (struct paramFileThreaded *)paramVoid;

    sendMessage(param->message);

    // Receive the port of the socket used to transfer files, by the switch socket.
    int portSocketForFile = receiveNewPort();

    // Connection to the socket used to send files.
    int clientSocketForFile = connectToServer(IP, portSocketForFile);
    int requestStatus;

    recv(clientSocketForFile, &requestStatus, sizeof(int), 0);

    if (requestStatus == 404) {
        // File not found.
        window_print_red(FILE_NOT_FOUND);
    }
    else {
        // Status == 204. File found.
        receiveFile(clientSocketForFile, param->filename);
        window_print_green(FILE_IMPORTED);
        close(clientSocketForFile);
    }

    free(param);

    pthread_exit(NULL);
}

/**
 * Threaded function called when a user wanna mp a file to another user.
 *
 * @param param
 * @return
 */
void *mpSendFileThreaded (void *paramVoid) {
    struct paramFileThreaded *param = (struct paramFileThreaded *)paramVoid;

    // Check file exists locally.
    // Send message to serv.
    // Connect to second socket.
    // Server asert that the wanted user is connected.
        // If connected, send the file.

    window_print_white(FILE_SENDING);
    FILE *file = openFile(param->filename, "rb");

    if (file != NULL) {
        // File exists.

        sendMessage(param->message);

        // Receive the port of the socket used to transfer files, by the switch socket.
        int portSocketForFile = receiveNewPort();

        // Connection to the socket used to send files.
        int clientSocketForFile = connectToServer(IP, portSocketForFile);
        int userTargetStatus;
        recv(clientSocketForFile, &userTargetStatus, sizeof(int), 0);   /// TODO : modif recv msg int avec socket en params.

        if (userTargetStatus == 204) {
            if (isDebugMode) { printf("Send file. \n"); }
            sendFile(clientSocketForFile, file);
            window_print_white(FILE_SENT);
        }
        else {
            // userTargetStatus == 404
            window_print_red(USER_NOT_FOUND);
        }
        close(clientSocketForFile);
    }
    else {
        // File doesn't exists.
        window_print_red(FILE_NOT_FOUND);
    }

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
    message = (char *)message;

    window_print_white(FILE_RECEIVING);
    // Receive the port of the socket used to transfer files, by the switch socket.
    int portSocketForFile = receiveNewPort();

    // Connection to the socket used to send files.
    int clientSocketForFile = connectToServer(IP, portSocketForFile);

    char *regexGroupList[3];
    getRegexGroup(regexGroupList, message, commandList[1]->regex);
    // regexGroupList[0] == message
    // regexGroupList[1] == -send || -get || username
    // regexGroupList[2] == filename

    receiveFile(clientSocketForFile, regexGroupList[2]);

    free(regexGroupList[0]);
    free(regexGroupList[1]);
    free(regexGroupList[2]);

    close(clientSocketForFile);
    window_print_white(FILE_RECEIVED);
    pthread_exit(NULL);
}
