/**
 * Ask the user for a message, in the console.
 * @return
 */
char *askUserForString () {
    char *message = (char *) malloc(sizeof(char) * MAX_SIZE_SENT);

    if(isDebugMode){
        printf(ASK_FOR_STRING);
    }
    if (fgets(message, MAX_SIZE_SENT, stdin) == NULL) {
        throwError(ERROR_FGETS, 1);
    }
    return message;
}

/**
 * Ask user for username.
 */
void askForUsername () {
    int verifiedUsername = 0;

    while(!verifiedUsername){
        printf(ASK_FOR_USERNAME);
        username = askUserForString();
        username[strlen(username) - 1] = '\0';
        sendMessage(username);
        printf("\n");
        int resp = receiveMessageInt();
        if(resp == 201){
            printf(VALID_USERNAME);
            verifiedUsername = 1;
        }
        else {
            printf(INVALID_USERNAME);
        }
    }
}
