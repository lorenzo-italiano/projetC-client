/**
 * Shutdown the client.
 */
void disconnectAction () {
    shutdownClient();
}

/**
 * Threaded function called by fileAction(...).
 * Connect to the socket used to send files and send the file.
 *
 * @param file
 * @return
 */
void *fileActionThreaded (void *file) {
    int clientSocketForFile = connectToServer(IP, PORT_SOCKET_FILE);

    sendFile(clientSocketForFile, file);

    close(clientSocketForFile);
    pthread_exit(NULL);
}

/**
 * Action used to send a file to the server.
 * Create a thread which use fileActionThreaded(...).
 *
 * @param command
 * @param message
 */
void fileAction (Command *command, char *message) {
    // Get current path.
    char filePath[200];
    getPwd(filePath);

    // Get filename and add it to the file path.
    char *regexGroupList[3];
    getRegexGroup(regexGroupList, message, command->regex);
    strcat(filePath, regexGroupList[2]);

    FILE *file;
    file = fopen(filePath, "rb");
    if (file != NULL) {
        // File exists.
        sendMessage(message);

        // Create thread which send the file.
        pthread_t fileThread;
        pthread_create(&fileThread, NULL, fileActionThreaded, file);
    }
    else {
        // File doesn't exists.
        printf("Fichier inconnu. \n");
    }

    free(regexGroupList[0]);
    free(regexGroupList[1]);
    free(regexGroupList[2]);
}

/**
 * Find the wanted command in the message in params (with regex).
 * Call the action attached to this command.
 * If there isn't any associated command, juste sebd the message.
 *
 * @param message
 */
void doCommandAction (char *message) {
    Command *command = getCommand(message);

    if (command == NULL) {
        // Unknown action.
        sendMessage(message);
    }
    else if (strcmp("disconnect", command->name) == 0) {
        // Disconnection.
        disconnectAction();
    }
    else if (strcmp("file", command->name) == 0) {
        // File sending.
        fileAction(command, message);
    }
    else {
        // Unknown action.
        sendMessage(message);
    }
}
