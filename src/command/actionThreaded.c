
/**
 * Threaded function called by fileAction(...).
 * Connect to the socket used to send files and send the file.
 *
 * @param filename
 * @return
 */
void *sendFileThreaded (void *filename) {
    // Get current path.
    char filePath[200];
    getUploadDirectoryPath(filePath);
    strcat(filePath, filename);

    // Create initial user message. Can't get it in params because it's a threaded function.
    char message[200] = "/file -send ";
    strcat(message, filename);
    strcat(message, "\n");

    FILE *file;
    file = fopen(filePath, "rb");
    if (file != NULL) {
        // File exists.
        sendMessage(message);

        // Connection to the socket used to send files.
        int clientSocketForFile = connectToServer(IP, PORT_SOCKET_FILE);
        sendFile(clientSocketForFile, file);
        close(clientSocketForFile);
        printf("Le fichier a bien été envoyé. \n");
    }
    else {
        // File doesn't exists.
        printf("Fichier inconnu. \n");
    }
    pthread_exit(NULL);
}

/**
 * Request a file to the server.
 * If the file exists, put it in ./uploads/.
 *
 * @param filename
 * @return
 */
void *receiveFileThreaded (char *filename) {
    // Connection to the socket used to send files.
    int clientSocketForFile = connectToServer(IP, PORT_SOCKET_FILE);
    int requestStatus;

    recv(clientSocketForFile, &requestStatus, sizeof(int), 0);

    if (requestStatus == 404) {
        // File not found.
        printf("Le fichier n'existe pas. \n");
    }
    else {
        // Status == 204. File found.
        receiveFile(clientSocketForFile, filename);
        printf("Le fichier a bien été importé. \n");
    }

    pthread_exit(NULL);
}