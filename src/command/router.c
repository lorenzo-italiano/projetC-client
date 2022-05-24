#include "Command.c"
#include "action.c"

/**
 * Find the wanted command in the message in params (with regex).
 * Call the action attached to this command.
 * If there isn't any associated command, juste send the message.
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
    else if (strcmp("files", command->name) == 0) {
        // Files listing.
        filesAction(command, message);
    }
    else if (strcmp("join", command->name) == 0) {
        // Files listing.
        joinAction(message);
    }
    else {
        // Unknown action.
        sendMessage(message);
    }
}
