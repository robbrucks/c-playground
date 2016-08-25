#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glob.h>

#define DEFPGQUERYPATH "/etc/libzbxpgsql.d"
#define MAXPATHLENGTH 512
#define MAXCONFFILES 100

// globals
const char pgquerypath[MAXPATHLENGTH] = DEFPGQUERYPATH;
char       *configPath[MAXCONFFILES];
int        fileCount = 0;


/*
 * Function getPGQUERYPATH
 *
 * Returns the config directory used for SQL config files
 * for pg_query.* keys.
 * 
 * If the environment variable PGQUERYPATH is set then that is
 * used, otherwise DEFPGQUERYPATH is used.
 *
 * Returns: pointer to const char
 *
 */

const char * getPGQUERYPATH () {
    const char *envPGQUERYPATH = getenv("PGQUERYPATH");

    if(NULL == &envPGQUERYPATH || '\0' == envPGQUERYPATH) {
        printf("Using built-in default path: \"%s\"", DEFPGQUERYPATH);
        return pgquerypath;
    } else
        printf("Using env variable PGQUERYPATH: \"%s\"", envPGQUERYPATH);
        return envPGQUERYPATH;
}


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
    char    path[MAXPATHLENGTH] = "\0";

    if (argc == 2) {                    // was there a shell argument?
        printf("Using command line argument \"%s\"\n", argv[1]);
        strcpy(path,argv[1]);           // then use shell argument
    } else {                            // otherwise
        strcpy(path,getPGQUERYPATH());  // get PGQUERYPATH env var or def directory
    }
    strcat(path,"/*.conf");             // append "/*.conf" wildcard
    printf("Looking for config files \"%s\"\n",path);
    if (globfilelist(path) > 0) {
        for (i = 0; i < fileCount; i++) {
            printf("   Found config file #%i = \"%s\"\n", i, configPath[i]);
            free(configPath[i]);
        }
    }
    return 0;
}


