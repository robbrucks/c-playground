#include <stdlib.h>
#include <stdio.h>
#include <glob.h>
#include <string.h>

#define MAXCONFFILES 100

// globals
char   *configPath[MAXCONFFILES];
int    fileCount = 0;

int globerror(const char *filename, int errorcode) {
    printf("%s: %s\n", filename, strerror(errorcode));
    return 1;
}

int globfilelist(const char *pattern) {
    glob_t   filenames;
    int      strsize;

    switch(glob(pattern, GLOB_ERR, globerror, &filenames)) {
        case 0 :
            for (fileCount = 0; fileCount < filenames.gl_pathc; fileCount++) {
                if (fileCount >= MAXCONFFILES) {
                    printf("Conf File limit hit\n");
                    break;
                }
                strsize = strlen(filenames.gl_pathv[fileCount]) + 1;
                configPath[fileCount] = malloc(strsize * sizeof(char));
                strcpy(configPath[fileCount],filenames.gl_pathv[fileCount]);
            }
            globfree(&filenames);
            break;
        case GLOB_NOMATCH :
            printf("No matches found\n");
            fileCount = 0;
            break;
        default :
            fileCount = -1;
    }
    return fileCount;
}

int main(int argc, char **argv) {
    int     i;

    if (argc == 1) {
        printf("usage: %s '<wildcard_path>'\n", argv[0]);
        return 1;
    }
    if (globfilelist(argv[1]) > 0) {
        for (i = 0; i < fileCount; i++) {
            printf("config file %i = %s\n", i, configPath[i]);
            free(configPath[i]);
        }
    }
    return 0;
}

