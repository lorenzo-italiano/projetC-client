/**
 * For an accepted socket descriptor in params, send a message of type int.
 * This message is usually used to send the message's size which will be sent after.
 *
 * @param messageSize
 */
void sendMessageInt (int messageSize) {
    if(send(acceptedSocketDescriptor, &messageSize, sizeof(int), 0) == -1){
        throwError("Erreur lors de l'envoi du message. \n", 1);
    }
    else {
        if(isDebugMode){
            printf("Taille du message envoyée. \n");
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
        throwError("Erreur lors de l'envoi du message. \n", 1);
    }
    else {
        setPurpleText();
        printf("%s: %s", username, message);
        setWhiteText();
    }
}

/**
 *
 */
void sendMessage(char *userMessage){
    int messageSize = (int)strlen(userMessage);
    if(isDebugMode){
        printf("Size of sent message: %d \n", messageSize);
    }

    // Send message size.
    sendMessageInt(messageSize);
    // Send message.
    sendMessageString(userMessage, messageSize);
}