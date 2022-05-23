/**
 * For an accepted socket descriptor in params, send a message of type int.
 * This message is usually used to send the message's size which will be sent after.
 *
 * @param messageSize
 */
void sendMessageInt (int messageSize) {
    if(send(acceptedSocketDescriptor, &messageSize, sizeof(int), 0) == -1){
        throwError(ERROR_SEND_MESSAGE, 1);
    }
    else {
        if(isDebugMode){
            printf(SEND_MESSAGE_INT);
        }
    }
}

/**
 * For an accepted socket descriptor and a message size, send the message in params.
 *
 * @param message
 * @param size
 */
void sendMessageString (char *message, int size) {
    if(send(acceptedSocketDescriptor, message, sizeof(char) * size, 0) == -1){
        throwError(ERROR_SEND_MESSAGE, 1);
    }
//    else {
//        TODO Set text to magenta
//        window_print_white(strcat(username, message));
//        wattron(win_display, COLOR_PAIR(1));
//        wprintw(win_display, "t");
//        wattroff(win_display, COLOR_PAIR(1));
//        printf("%s: %s", username, message);
//    }
}

/**
 * Send a message to the server.
 * Send the message's size and after the message.
 *
 * @param userMessage
 */
void sendMessage(char *userMessage){
    int messageSize = (int)strlen(userMessage);

    // Send message size.
    sendMessageInt(messageSize);
    // Send message.
    sendMessageString(userMessage, messageSize);
}

/**
 * Send the file in params to the socket also in params.
 * The file is sent by blocs of size MAX_SIZE_SENT.
 *
 * @param socketForFile
 * @param file
 */
void sendFile(int socketForFile, FILE *file){
    long fileSize;
    fseek(file, 0, SEEK_END);          // Jump to the end of the file.
    fileSize = ftell(file);             // Get the current byte offset in the file.
    rewind(file);                      // Jump back to the beginning of the file.

    // Send file blocSize to the server.
    send(socketForFile, &fileSize, sizeof(long), 0);

    // Send the file bloc per bloc.
    long blocSize = MAX_SIZE_SENT;
    char subBuffer[blocSize];
    for (int i = 0; i < fileSize; i += MAX_SIZE_SENT) {
        blocSize = (i + MAX_SIZE_SENT < fileSize) ? MAX_SIZE_SENT : fileSize - i;   // Calcul the bloc's size.
        fread(subBuffer, blocSize, 1, file); // Read in the file.

        if (send(socketForFile, subBuffer, sizeof(subBuffer), 0) == -1) {   // Send bloc of data.
            throwError(ERROR_SEND_FILE, 1);
        }
        bzero(subBuffer, MAX_SIZE_SENT);    // Clear the buffer.
    }

    fclose(file); // Close the file
    printf(FILE_SENT);
}
