#include <stdlib.h>
#include <stdio.h>
#include <glob.h>
#include <string.h>
#include <libconfig.h>

#define DEFPGQUERYPATH "/etc/libzbxpgsql.d"   // not to exceed MAXGLOBPATH length
#define MAXGLOBPATH 512
#define MAXCONFFILES 100
#define MAXSQLSTMTS 500

// Global Variables
const char pgquerypath[MAXGLOBPATH] = DEFPGQUERYPATH;
char      *configPath[MAXCONFFILES];
int        fileCount = 0;
char      *SQLkey[MAXSQLSTMTS];
char      *SQLstmt[MAXSQLSTMTS];
int        SQLcount = 0;



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
        return pgquerypath;
    } else {
        if (strlen(envPGQUERYPATH) > MAXGLOBPATH) {
            printf("ERROR: Env variable \"PGQUERYPATH\" value too long.\n");
            printf("ERROR: Length %i exceeds max length of %i\n", strlen(pgquerypath), MAXGLOBPATH);
            return NULL;
        }
        return envPGQUERYPATH;
    }
}



/*
 * Function globerror
 *
 * Error handler for globbing.
 *
 */
int globerror(const char *filename, int errorcode) {
    printf("ERROR: globbing error: %s: %s\n", filename, strerror(errorcode));
    return EXIT_FAILURE;
}



/*
 * Function globfilelist
 *
 * Takes a file glob pattern and stores matching
 * filenames in configPath array.
 *
 * Returns:
 *    int number of files matched
 *    -1 if error encountered
 */
int globfilelist(const char *pattern) {
    glob_t   filenames;
    int      strsize;

    switch(glob(pattern, GLOB_ERR, globerror, &filenames)) {
        case 0 :
            for (fileCount = 0; fileCount < filenames.gl_pathc; fileCount++) {
                if (fileCount >= MAXCONFFILES) {
                    printf("ERROR: Conf file count limit hit\n");
                    break;
                }
                strsize = strlen(filenames.gl_pathv[fileCount]) + 1;
                configPath[fileCount] = malloc(strsize * sizeof(char));
                strcpy(configPath[fileCount],filenames.gl_pathv[fileCount]);
            }
            globfree(&filenames);
            break;
        case GLOB_NOMATCH :
            printf("INFO: No conf file matches found\n");
            fileCount = 0;
            break;
        default :
            fileCount = -1;
    }
    return fileCount;
}



/*
 * Function storeSQLstmt
 *
 * A simple key/value store using arrays of pointers.
 *
 * Stores a string key and a string value (SQL statement)
 * into a sorted array, maintaining sort order.
 *
 * Since it's just moving pointers it should perform well
 * even with large numbers of keys.
 *
 * Returns: 
 *   EXIT_SUCCESS = successfully inserted
 *   EXIT_FAILURE = insert failed
 *   -1           = duplicate key discarded
 */
int  storeSQLstmt(const char *key, const char *stmt) {
    int  i;

    // make sure we have space
    if (SQLcount >= MAXSQLSTMTS) {
        printf("WARNING: Keystore full: %i statements stored already\n", SQLcount);
        return EXIT_FAILURE;
    }
    // exclude dupes
    if (SQLkeysearch(key) != -1) {
        printf("WARNING: Duplicate key: \"%s\", discarding it\n", key);
//        return EXIT_FAILURE;
        return -1;
    }
    // start at the end of the index and push out
    // entries to the next spot until you find the
    // right spot to insert the new key/value pair
    i = SQLcount - 1;  // indexes start at zero, so -1
    while (i >= 0 && strcmp(key,SQLkey[i]) < 0) {
        SQLkey[i+1]  = SQLkey[i];
        SQLstmt[i+1] = SQLstmt[i];
        i--;
    }
    // allocate memory for the new key
    SQLkey[i+1] = malloc(sizeof(char) * (strlen(key)+1));
    if (SQLkey[SQLcount] == NULL) {
        printf("ERROR: malloc did not allocate memory");
        return EXIT_FAILURE;
    }
    // allocate memory for the new value
    SQLstmt[i+1] = malloc(sizeof(char) * (strlen(stmt)+1));
    if (SQLstmt[SQLcount] == NULL) {
        printf("ERROR: malloc did not allocate memory");
        return EXIT_FAILURE;
    }
    // store the key and value
    strcpy(SQLkey[i+1],key);
    strcpy(SQLstmt[i+1],stmt);
    SQLcount++;

    return EXIT_SUCCESS;
}



/*
 * Function SQLkeysearch
 *
 * Searches the key array to find the
 * corresponding SQL stmt using binary
 * search.
 *
 * Returns: 
 *    If Key Found: index to key
 *    If Not Found: -1
 */
int SQLkeysearch(char *key) {
    int  top;
    int  mid;
    int  bottom;
    int  ctr;

    top = SQLcount - 1;
    bottom = 0;
    ctr = 0;
    while (bottom <= top) {
        ctr++;
        mid = (bottom + top)/2;
        if (strcmp(SQLkey[mid], key) == 0) {
            return mid;
        } else if (strcmp(SQLkey[mid], key) > 0) {
            top    = mid - 1;
        } else if (strcmp(SQLkey[mid], key) < 0) {
            bottom = mid + 1;
        }
    }
    return -1;
}



/*
 * Function SQLcleanup
 *
 * Free up array memory when done.
 *
 * Returns: 
 */
int  SQLCleanup() {
    int   i;

    for (i = 0; i < SQLcount; i++) {
        free(SQLkey[i]);
        free(SQLstmt[i]);
    }
    SQLcount = 0;
    return EXIT_SUCCESS;
}



/*
 * Function readconfig
 *
 * Reads the contents of a config file using libconfig.
 * 
 * Note: although libconfig supports complex parameter
 * files (such as nested parms), this function's
 * implementation only supports simple
 *    key = value
 * parameters.
 *
 * Returns:
 *   EXIT_SUCCESS = successfully parsed
 *   EXIT_FAILURE = parse failed
 */
int readconfig(const char *cfgfile) {
    config_t          cfg;
    config_setting_t  *root, *element;
    int               i;
    const char        *key, *value;

    config_init(&cfg);
    // call libconfig to parse config file into memory
    if(! config_read_file(&cfg, cfgfile)) {
        printf("ERROR: %s for file \"%s\"\n", config_error_text(&cfg),
            (config_error_file(&cfg) == NULL ? cfgfile : config_error_file(&cfg)));
        if(CONFIG_ERR_PARSE == config_error_type(&cfg)) 
            printf("ERROR: Parsing error on or near line %i\n", config_error_line(&cfg));
        config_destroy(&cfg);
        return EXIT_FAILURE;
    }
    // start retrieving key/value pairs
    root = config_root_setting(&cfg);
    for (i = 0; i < config_setting_length(root); i++) {
        element = config_setting_get_elem(root, i);
        key = config_setting_name(element);
        // we only want strings
        if(CONFIG_TYPE_STRING == config_setting_type(element)) {
            value = config_setting_get_string_elem(root, i);
            // store it in our key/value store
            if (storeSQLstmt(key, value) == EXIT_FAILURE) {
                config_destroy(&cfg);
                return EXIT_FAILURE;
            }
        }
        else
            printf("WARNING: Element \"%s\" in \"%s\" on line %i is not a string - discarding\n",
                   key, config_setting_source_file(element), config_setting_source_line(element));
    }

    config_destroy(&cfg);
    return EXIT_SUCCESS;
}



//  init test
int main(int argc, char **argv) {
    int    i, numfiles;
    char   confdir[MAXGLOBPATH];

// called during zabbix agent init
    if (argc == 2) {
        strcpy(confdir,argv[1]);          // read cmd line arg
    } else {
        strcpy(confdir,getPGQUERYPATH()); // normal method
    }
    // append either "/*.conf" or "*.conf"
    if ('/' == confdir[strlen(confdir)-1]) {  
        strcat(confdir,"*.conf");
    } else {
        strcat(confdir,"/*.conf");
    }
    numfiles = globfilelist(confdir);
    if (numfiles < 0) {
        printf("ERROR: Error returned from globfilelist\n");
        return EXIT_FAILURE;
    }
    if (numfiles == 0) {
        printf("No files found\n");
        return EXIT_SUCCESS;
    }

    for (i = 0; i < numfiles; i++) {
        printf("INFO: Parsing config file \"%s\"\n", configPath[i]);
        readconfig(configPath[i]);
        free(configPath[i]);
    }

    for (i=0; i < SQLcount; i++) {
        printf("%-12s", SQLkey[i]);
        printf("=\"%s\"\n", SQLstmt[i]);
    }

// called when zabbix agent shuts down
    SQLCleanup();
    return EXIT_SUCCESS;

}

