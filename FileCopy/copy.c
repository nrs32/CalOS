#include <stdio.h>
#include <errno.h>
#include <unistd.h>   // For access
#include <sys/stat.h> // For stat
/**
 * copy.c main waits for a correctly formatted copy command from a user ( copy <srcFile> <destFile> )
 * and then copies the contents of the srcFile to the destFile.
 * 
 * @author Nikita Sietsema
 * @date 11 February 2020
 * Sources: I used the following C library for finding functions and C syntax (unless otherwise stated)
 * https://www.tutorialspoint.com/c_standard_library/
 * */
extern int errno;

int main(int argCount, char **args)
{
    char *srcFile, *destFile;
    FILE *srcFileStream, *destFileStream;

    // Make sure command has 3 values (copy, src, dest)
    if (argCount != 3) {
        fprintf(stderr, "\nInvalid command. We only support the following command:\ncopy <srcFile> <destFile>\n");
        return -1;
    }

    // Extract file name args
    srcFile = args[1]; 
    destFile = args[2];

    // See if srcFile exists and is a regular file
    // https://stackoverflow.com/questions/4553012/checking-if-a-file-is-a-directory-or-just-a-file (Frédéric Hamidi) -- for stat
    struct stat path_stat;
    stat(srcFile, &path_stat);

    if (access(srcFile, F_OK|R_OK) == -1 || !S_ISREG(path_stat.st_mode)) { 
        perror("\nThe provided source file could not be found or is invalid.\nBe sure to include the file extension and use a regular file.\n\n");
        return -1;
    }

    // Make sure destFile does not exist (causing errno 2)
    int destAccessible = access(destFile, F_OK); // <-- call before if-statement to set errno
    if (!(errno == 2 && destAccessible == -1)) { 
        errno == 36 
            ? fprintf(stderr, "\nThe provided destination file name is too long.\n")
            : fprintf(stderr, "\nThe provided destination file already exists.\nPlease provide a new file name.\n");
        return -1;
    }

    //////////////////////////////////////////////////////////////////////
    // We have passed all tests for valid arguments and can copy the file

    srcFileStream = fopen(srcFile, "r");
    destFileStream = fopen(destFile, "w");

    char fileContent;
    char putResult;
    // Loop over srcFile and copy contents over to destFile
    while((fileContent = fgetc(srcFileStream)) != EOF) {
        putResult = fputc(fileContent, destFileStream);

        // If fputc fails it will return EOF
        if (putResult == EOF) {
            perror("\nThe contents of the source file could not be written to the destination file");
            return -1;
        }
    }

    fclose(srcFileStream);
    fclose(destFileStream);

    return 0;
}