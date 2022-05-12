#include <dirent.h>

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
    getUploadDirectoryPath(filePath);

    // Get filename and add it to the file path.
    char *regexGroupList[3];
    getRegexGroup(regexGroupList, message, command->regex);
    strcat(filePath, regexGroupList[1]);

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
//    free(regexGroupList[2]);
}

/**
 * Print directory's names within uploads/.
 * Depending on the option in message, print the client or server directory.
 *
 * @param command
 * @param message
 */
void filesAction (Command *command, char *message) {
    // Get the option. -c || -s.
    char *regexGroupList[3];
    getRegexGroup(regexGroupList, message, command->regex);

    if (strcmp(regexGroupList[1], "c") == 0) {
        // Client files listing.

        printf("Liste des fichiers disponibles dans le dossier uploads : \n");

        // Get uploads path.
        char uploadDirectoryPath[200];
        getUploadDirectoryPath(uploadDirectoryPath);

        // Get directory.
        DIR *directory;
        struct dirent *file;
        directory = opendir(uploadDirectoryPath);
        if (directory == NULL) {
            throwError("Unable to open the directory. \n", 0);
        }

        while ((file = readdir(directory)) != NULL) {
            if (strcmp(file->d_name, ".") != 0 && strcmp(file->d_name, "..") != 0) {  // Don't take files : "." et "..".
//                printf("File %d : %s\n", n, file->d_name);
                printf("%s\n", file->d_name);
            }
        }
        printf("\n");
        closedir(directory);
    }
    else if (strcmp(regexGroupList[1], "s") == 0) {
        // Server files listing.
        printf("Liste des fichiers disponibles sur le serveur : \n");
        sendMessage(message);
    }

    free(regexGroupList[0]);
    free(regexGroupList[1]);
    free(regexGroupList[2]);
}
