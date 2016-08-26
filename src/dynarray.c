#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXSQLSTMTS 10

// Key/Value storage variables
char  *SQLkey[MAXSQLSTMTS];
char  *SQLstmt[MAXSQLSTMTS];
int   SQLcountStored = 0;

/*
 * Function storeSQLstmt
 *
 * Stores a string key and a string value (SQL statement)
 * into a sorted array, maintaining sort order.
 *
 * Returns: 
 *   0 = successfully inserted
 *   1 = insert failed
 */

int  storeSQLstmt(char *key, char *stmt) {
    int  i;

    if (SQLcountStored >= MAXSQLSTMTS) {
        printf("Keystore full: %i statements stored already\n", SQLcountStored);
        return EXIT_FAILURE;
    }

    if (SQLkeysearch(key) != -1) {
        printf("Duplicate key \"%s\", discarding it\n", key);
        return EXIT_FAILURE;
    }

    i = SQLcountStored - 1;
    while (i >= 0 && strcmp(key,SQLkey[i]) < 0) {
        SQLkey[i+1]  = SQLkey[i];
        SQLstmt[i+1] = SQLstmt[i];
        i--;
    }

    SQLkey[i+1] = malloc(sizeof(char) * (strlen(key)+1));
    if (SQLkey[SQLcountStored] == NULL) {
        printf("Error from malloc");
        return EXIT_FAILURE;
    }

    SQLstmt[i+1] = malloc(sizeof(char) * (strlen(stmt)+1));
    if (SQLstmt[SQLcountStored] == NULL) {
        printf("Error from malloc");
        return EXIT_FAILURE;
    }

    strcpy(SQLkey[i+1],key);
    strcpy(SQLstmt[i+1],stmt);
    SQLcountStored++;

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

    top = SQLcountStored - 1;
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

    for (i = 0; i < SQLcountStored; i++) {
        free(SQLkey[i]);
        free(SQLstmt[i]);
    }
    SQLcountStored = 0;
    return EXIT_SUCCESS;
}


// Main test

int main () {
    int   i;

    storeSQLstmt("key01","really long sql statement number 01");
    storeSQLstmt("key07","really long sql statement number 07");
    storeSQLstmt("key03","really long sql statement number 03");
    storeSQLstmt("key10","really long sql statement number 10");
    storeSQLstmt("key04","really long sql statement number 04");
    storeSQLstmt("key07","really long sql statement number ???????");  // dupe check
    storeSQLstmt("key06","really long sql statement number 06");
    storeSQLstmt("key02","really long sql statement number 02");
    storeSQLstmt("key08","really long sql statement number 08");
    storeSQLstmt("key09","really long sql statement number 09");
    storeSQLstmt("key05","really long sql statement number 05");
    storeSQLstmt("keyfail","too many - fail ************************");

    printf("Stored %i statements\n\n", SQLcountStored);

    for (i=0; i < MAXSQLSTMTS; i++) {
        printf("key=%s  ", SQLkey[i]);
        printf("value=%s\n", SQLstmt[i]);
    }

    printf("\n");
    printf("key05 value =\"%s\"\n", SQLstmt[SQLkeysearch("key05")]);
    printf("cry11 value =\"%s\"\n", SQLstmt[SQLkeysearch("cry11")]);
    printf("key04 value =\"%s\"\n", SQLstmt[SQLkeysearch("key04")]);
    printf("key09 value =\"%s\"\n", SQLstmt[SQLkeysearch("key09")]);
    printf("key07 value =\"%s\"\n", SQLstmt[SQLkeysearch("key07")]);
    printf("keyfail value =\"%s\"\n", SQLstmt[SQLkeysearch("keyfail")]);
    printf("key08 value =\"%s\"\n", SQLstmt[SQLkeysearch("key08")]);
    printf("key09 value =\"%s\"\n", SQLstmt[SQLkeysearch("key09")]);

    SQLCleanup();

    return 0;
}

