/**
 * Get the filepath.
 * @param filePath
 */
void getUploadDirectoryPath (char *filePath) {
    getwd(filePath);
    if (isMatch(filePath, "cmake-build-debug")) {
        // Program launched by Cmake project. It means that the current pwd is the directory cmake-build-debug.
        strcat(filePath, "/../uploads/");
    } else {
        // Program launched by the script client.sh.
        strcat(filePath, "/src/uploads/");
    }
}

/**
 * For a filename, return an opened file on binary reading.
 * Can return NULL.
 *
 * @param filename
 * @return
 */
FILE *openFile (char *filename, char *mode) {
    // Get current path.
    char filePath[200];
    getUploadDirectoryPath(filePath);
    strcat(filePath, filename);

    FILE *file;
    file = fopen(filePath, mode);

    return file;
}
