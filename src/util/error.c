/**
 * Perror the message in params and exit the programme.
 * If hasErrno, print it.
 * @param errorMessage
 * @param hasErrno
 */
void throwError (char *errorMessage, int hasErrno) {
    if (hasErrno) {
        perror(errorMessage);
    }
    else{
        printf("%s \n", errorMessage);
    }
    exit(EXIT_FAILURE);
}
