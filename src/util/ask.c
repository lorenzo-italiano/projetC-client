/**
 * Ask the user for a message, in the console.
 * @return
 */
char *askUserForString () {
    char *message = (char *) malloc(sizeof(char) * MAX_STRING_SIZE);

    if(isDebugMode){
        printf("Entrez votre message: \n");
    }
    if (fgets(message, MAX_STRING_SIZE, stdin) == NULL) {
        throwError("Error fgets. \n", 1);
    }
    return message;
}

/**
 * Ask user for username.
 */
char *askForUsername () {
    int verifiedUsername = 0;

    while(!verifiedUsername){
        printf("Entrez votre username: ");
        username = askUserForString();
        username[strlen(username) - 1] = '\0';
        sendMessage(username);
        printf("\n");
        int resp = receiveMessageInt();
        printf("%d \n",resp);
        if(resp == 201){
            verifiedUsername = 1;
        }
    }
}
