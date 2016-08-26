#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXSQLSTMTS 10

// global vars
char  *SQLkeyStore[MAXSQLSTMTS];
char  *SQLstmtStore[MAXSQLSTMTS];
int   SQLcountStored = 0;

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

int  storeSQLstmt(char *key, char *stmt) {

    if (SQLcountStored >= MAXSQLSTMTS) {
        printf("Keystore full: %i statements stored already\n", SQLcountStored);
        return 1;
    }
    SQLkeyStore[SQLcountStored] = malloc(sizeof(char) * (strlen(key)+1));
    if (SQLkeyStore[SQLcountStored] == NULL) {
        printf("Error from malloc");
        return 1;
    }
    SQLstmtStore[SQLcountStored] = malloc(sizeof(char) * (strlen(stmt)+1));
    if (SQLstmtStore[SQLcountStored] == NULL) {
        printf("Error from malloc");
        return 1;
    }
    strcpy(SQLkeyStore[SQLcountStored],key);
    strcpy(SQLstmtStore[SQLcountStored],stmt);
    SQLcountStored++;
    return 0;
}

int  sortSQLkeys() {
    int   i;
    int   j;
    char  *tempkey;

    // simple bubble sort
    for ( i = 1; i < SQLcountStored; i++ ) {
        for ( j = 1; j < SQLcountStored; j++ ) {
            if (strcmp(SQLkeyStore[j-1],SQLkeyStore[j]) > 0) {
                // swap keys
                tempkey = SQLkeyStore[j-1];
                SQLkeyStore[j-1] = SQLkeyStore[j];
                SQLkeyStore[j] = tempkey;
                // swap stmts
                tempkey = SQLstmtStore[j-1];
                SQLstmtStore[j-1] = SQLstmtStore[j];
                SQLstmtStore[j] = tempkey;
            }
        }
    }
}

int  SQLkeysearch(char *key) {
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
        if (strcmp(SQLkeyStore[mid], key) == 0) {
            return mid;
        } else if (strcmp(SQLkeyStore[mid], key) > 0) {
            top    = mid - 1;
        } else if (strcmp(SQLkeyStore[mid], key) < 0) {
            bottom = mid + 1;
        }
    }
    return -1;
}

int  SQLCleanup() {
    int   i;

    for (i = 0; i < SQLcountStored; i++) {
        free(SQLkeyStore[i]);
        free(SQLstmtStore[i]);
    }
    SQLcountStored = 0;
}

int main () {
    int   i;
    int   x;

    storeSQLstmt("key4","really long sql statement number 4");
    storeSQLstmt("key6","really long sql statement number 6");
    storeSQLstmt("key9","really long sql statement number 9");
    storeSQLstmt("key7","really long sql statement number 7");
    storeSQLstmt("key1","really long sql statement number 1");
    storeSQLstmt("key3","really long sql statement number 3");
    storeSQLstmt("key10","really long sql statement number 10");
    storeSQLstmt("key8","really long sql statement number 8");
    storeSQLstmt("key5","really long sql statement number 5");
    storeSQLstmt("key2","really long sql statement number 2");
    storeSQLstmt("key11","really long sql statement number 11");

    printf("\nUnsorted values:\n");
    for (i=0; i < 10; i++) {
        printf("key=%s\n", SQLkeyStore[i]);
        printf("value=%s\n", SQLstmtStore[i]);
    }

    sortSQLkeys();

    printf("\nSorted values:\n");
    for (i=0; i < 10; i++) {
        printf("key=%s\n", SQLkeyStore[i]);
        printf("value=%s\n", SQLstmtStore[i]);
    }

    printf("\n");
    x=SQLkeysearch("key5");  printf("key5 value =\"%s\"\n", SQLstmtStore[x]);
    x=SQLkeysearch("cry5");  printf("cry5 value =\"%s\"\n", SQLstmtStore[x]);
    x=SQLkeysearch("key1");  printf("key1 value =\"%s\"\n", SQLstmtStore[x]);
    x=SQLkeysearch("key10");  printf("key10 value =\"%s\"\n", SQLstmtStore[x]);
    x=SQLkeysearch("key3");  printf("key3 value =\"%s\"\n", SQLstmtStore[x]);

    SQLCleanup();

    return 0;
}

