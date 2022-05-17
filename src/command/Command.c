struct Command {
    char *name;
    char *regex;
    /// TODO : Add action function associated.
};
typedef struct Command Command;

/**
 * Malloc a struct Command and set attributes.
 *
 * @param name
 * @param regex
 * @return
 */
Command *createCommand (char *name, char *regex) {
    Command *command = (Command *)malloc(sizeof(Command));

    command->name = name;
    command->regex = regex;

    return command;
}

Command *commandList[NB_COMMAND];
/**
 * Initialize the global list of available commands, commandList.
 */
void initCommandList () {
    commandList[0] = createCommand("disconnect", "^/disconnect *\n$");  // Déconnecte l'utilisateur.
    commandList[1] = createCommand("file", "^/file +([^ ]+) +([^ ]+) *\n$");     // Envoi un fichier.
    commandList[2] = createCommand("files", "^/files +-(c|s) *\n$");    // Liste des fichier disponibles dans le répertoire client ou serveur. /files -c || -s
    // Don't forger to update NB_COMMAND.
}

/**
 * Return true if the message in params contains a command (starts with a "/").
 * @param message
 * @return
 */
int isCommand (char *message) {
    return isMatch(message, "^/");
}

/**
 * Return the Command associated to the message in params.
 * If there is no matching command, then return NULL.
 *
 * @param message
 * @return
 */
Command *getCommand (char *message) {
    if (isCommand(message)) {
        for (int i = 0; i < NB_COMMAND; i++) {
            if (isMatch(message, commandList[i]->regex)) {
                if (isDebugMode) { printf("Command : %s. \n", commandList[i]->name); }
                return commandList[i];
            }
        }
    }
    return NULL;
}

