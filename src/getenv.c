#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEFPGQUERYPATH "/etc/libzbxpgsql.d"

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

const char pgquerypath[512] = DEFPGQUERYPATH;

const char * getPGQUERYPATH () {
    const char *envPGQUERYPATH = getenv("PGQUERYPATH");

    if(NULL == &envPGQUERYPATH || '\0' == envPGQUERYPATH) {
        return pgquerypath;
    } else
        return envPGQUERYPATH;
}

int main () {
    const char *foo = NULL;

    foo = getPGQUERYPATH();
    printf("pgquerypath = \"%s\"\n", foo);
    return 0;
}

