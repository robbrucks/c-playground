#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXSQLSTMTS 250

// Key/Value storage variables
char  *SQLkey[MAXSQLSTMTS];
char  *SQLstmt[MAXSQLSTMTS];
int   SQLcountStored = 0;

/*
 * Function storeSQLstmt
 *
 * Stores a string key and a string value (SQL statement)
 * into a sorted array, maintaining the sort.
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
    int   x;

    storeSQLstmt("key169","really long sql statement number 169");
    storeSQLstmt("key105","really long sql statement number 105");
    storeSQLstmt("key106","really long sql statement number 106");
    storeSQLstmt("key166","really long sql statement number 166");
    storeSQLstmt("key167","really long sql statement number 167");
    storeSQLstmt("key168","really long sql statement number 168");
    storeSQLstmt("key170","really long sql statement number 170");
    storeSQLstmt("key171","really long sql statement number 171");
    storeSQLstmt("key172","really long sql statement number 172");
    storeSQLstmt("key173","really long sql statement number 173");
    storeSQLstmt("key174","really long sql statement number 174");
    storeSQLstmt("key175","really long sql statement number 175");
    storeSQLstmt("key043","really long sql statement number 043");
    storeSQLstmt("key044","really long sql statement number 044");
    storeSQLstmt("key045","really long sql statement number 045");
    storeSQLstmt("key046","really long sql statement number 046");
    storeSQLstmt("key047","really long sql statement number 047");
    storeSQLstmt("key048","really long sql statement number 048");
    storeSQLstmt("key049","really long sql statement number 049");
    storeSQLstmt("key050","really long sql statement number 050");
    storeSQLstmt("key007","really long sql statement number 007");
    storeSQLstmt("key051","really long sql statement number 051");
    storeSQLstmt("key052","really long sql statement number 052");
    storeSQLstmt("key053","really long sql statement number 053");
    storeSQLstmt("key054","really long sql statement number 054");
    storeSQLstmt("key055","really long sql statement number 055");
    storeSQLstmt("key107","really long sql statement number 107");
    storeSQLstmt("key108","really long sql statement number 108");
    storeSQLstmt("key109","really long sql statement number 109");
    storeSQLstmt("key110","really long sql statement number 110");
    storeSQLstmt("key111","really long sql statement number 111");
    storeSQLstmt("key093","really long sql statement number 093");
    storeSQLstmt("key094","really long sql statement number 094");
    storeSQLstmt("key005","really long sql statement number 005");
    storeSQLstmt("key006","really long sql statement number 006");
    storeSQLstmt("key008","really long sql statement number 008");
    storeSQLstmt("key009","really long sql statement number 009");
    storeSQLstmt("key010","really long sql statement number 010");
    storeSQLstmt("key011","really long sql statement number 011");
    storeSQLstmt("key012","really long sql statement number 012");
    storeSQLstmt("key013","really long sql statement number 013");
    storeSQLstmt("key095","really long sql statement number 095");
    storeSQLstmt("key096","really long sql statement number 096");
    storeSQLstmt("key097","really long sql statement number 097");
    storeSQLstmt("key098","really long sql statement number 098");
    storeSQLstmt("key099","really long sql statement number 099");
    storeSQLstmt("key100","really long sql statement number 100");
    storeSQLstmt("key101","really long sql statement number 101");
    storeSQLstmt("key102","really long sql statement number 102");
    storeSQLstmt("key103","really long sql statement number 103");
    storeSQLstmt("key104","really long sql statement number 104");
    storeSQLstmt("key056","really long sql statement number 056");
    storeSQLstmt("key057","really long sql statement number 057");
    storeSQLstmt("key058","really long sql statement number 058");
    storeSQLstmt("key133","really long sql statement number 133");
    storeSQLstmt("key145","really long sql statement number 145");
    storeSQLstmt("key134","really long sql statement number 134");
    storeSQLstmt("key135","really long sql statement number 135");
    storeSQLstmt("key136","really long sql statement number 136");
    storeSQLstmt("key137","really long sql statement number 137");
    storeSQLstmt("key138","really long sql statement number 138");
    storeSQLstmt("key139","really long sql statement number 139");
    storeSQLstmt("key140","really long sql statement number 140");
    storeSQLstmt("key001","really long sql statement number 001");
    storeSQLstmt("key002","really long sql statement number 002");
    storeSQLstmt("key003","really long sql statement number 003");
    storeSQLstmt("key004","really long sql statement number 004");
    storeSQLstmt("key014","really long sql statement number 014");
    storeSQLstmt("key015","really long sql statement number 015");
    storeSQLstmt("key016","really long sql statement number 016");
    storeSQLstmt("key141","really long sql statement number 141");
    storeSQLstmt("key142","really long sql statement number 142");
    storeSQLstmt("key143","really long sql statement number 143");
    storeSQLstmt("key144","really long sql statement number 144");
    storeSQLstmt("key146","really long sql statement number 146");
    storeSQLstmt("key147","really long sql statement number 147");
    storeSQLstmt("key148","really long sql statement number 148");
    storeSQLstmt("key149","really long sql statement number 149");
    storeSQLstmt("key150","really long sql statement number 150");
    storeSQLstmt("key151","really long sql statement number 151");
    storeSQLstmt("key152","really long sql statement number 152");
    storeSQLstmt("key153","really long sql statement number 153");
    storeSQLstmt("key154","really long sql statement number 154");
    storeSQLstmt("key155","really long sql statement number 155");
    storeSQLstmt("key156","really long sql statement number 156");
    storeSQLstmt("key157","really long sql statement number 157");
    storeSQLstmt("key158","really long sql statement number 158");
    storeSQLstmt("key159","really long sql statement number 159");
    storeSQLstmt("key160","really long sql statement number 160");
    storeSQLstmt("key220","really long sql statement number 220");
    storeSQLstmt("key024","really long sql statement number 024");
    storeSQLstmt("key221","really long sql statement number 221");
    storeSQLstmt("key222","really long sql statement number 222");
    storeSQLstmt("key223","really long sql statement number 223");
    storeSQLstmt("key224","really long sql statement number 224");
    storeSQLstmt("key225","really long sql statement number 225");
    storeSQLstmt("key226","really long sql statement number 226");
    storeSQLstmt("key227","really long sql statement number 227");
    storeSQLstmt("key228","really long sql statement number 228");
    storeSQLstmt("key229","really long sql statement number 229");
    storeSQLstmt("key230","really long sql statement number 230");
    storeSQLstmt("key231","really long sql statement number 231");
    storeSQLstmt("key232","really long sql statement number 232");
    storeSQLstmt("key161","really long sql statement number 161");
    storeSQLstmt("key162","really long sql statement number 162");
    storeSQLstmt("key163","really long sql statement number 163");
    storeSQLstmt("key164","really long sql statement number 164");
    storeSQLstmt("key165","really long sql statement number 165");
    storeSQLstmt("key017","really long sql statement number 017");
    storeSQLstmt("key018","really long sql statement number 018");
    storeSQLstmt("key019","really long sql statement number 019");
    storeSQLstmt("key020","really long sql statement number 020");
    storeSQLstmt("key021","really long sql statement number 021");
    storeSQLstmt("key022","really long sql statement number 022");
    storeSQLstmt("key023","really long sql statement number 023");
    storeSQLstmt("key025","really long sql statement number 025");
    storeSQLstmt("key026","really long sql statement number 026");
    storeSQLstmt("key027","really long sql statement number 027");
    storeSQLstmt("key028","really long sql statement number 028");
    storeSQLstmt("key029","really long sql statement number 029");
    storeSQLstmt("key030","really long sql statement number 030");
    storeSQLstmt("key031","really long sql statement number 031");
    storeSQLstmt("key032","really long sql statement number 032");
    storeSQLstmt("key033","really long sql statement number 033");
    storeSQLstmt("key034","really long sql statement number 034");
    storeSQLstmt("key035","really long sql statement number 035");
    storeSQLstmt("key036","really long sql statement number 036");
    storeSQLstmt("key059","really long sql statement number 059");
    storeSQLstmt("key060","really long sql statement number 060");
    storeSQLstmt("key061","really long sql statement number 061");
    storeSQLstmt("key062","really long sql statement number 062");
    storeSQLstmt("key063","really long sql statement number 063");
    storeSQLstmt("key064","really long sql statement number 064");
    storeSQLstmt("key065","really long sql statement number 065");
    storeSQLstmt("key066","really long sql statement number 066");
    storeSQLstmt("key067","really long sql statement number 067");
    storeSQLstmt("key112","really long sql statement number 112");
    storeSQLstmt("key113","really long sql statement number 113");
    storeSQLstmt("key114","really long sql statement number 114");
    storeSQLstmt("key115","really long sql statement number 115");
    storeSQLstmt("key116","really long sql statement number 116");
    storeSQLstmt("key089","really long sql statement number 089");
    storeSQLstmt("key117","really long sql statement number 117");
    storeSQLstmt("key118","really long sql statement number 118");
    storeSQLstmt("key119","really long sql statement number 119");
    storeSQLstmt("key120","really long sql statement number 120");
    storeSQLstmt("key121","really long sql statement number 121");
    storeSQLstmt("key122","really long sql statement number 122");
    storeSQLstmt("key123","really long sql statement number 123");
    storeSQLstmt("key124","really long sql statement number 124");
    storeSQLstmt("key125","really long sql statement number 125");
    storeSQLstmt("key126","really long sql statement number 126");
    storeSQLstmt("key127","really long sql statement number 127");
    storeSQLstmt("key128","really long sql statement number 128");
    storeSQLstmt("key129","really long sql statement number 129");
    storeSQLstmt("key130","really long sql statement number 130");
    storeSQLstmt("key131","really long sql statement number 131");
    storeSQLstmt("key132","really long sql statement number 132");
    storeSQLstmt("key037","really long sql statement number 037");
    storeSQLstmt("key038","really long sql statement number 038");
    storeSQLstmt("key039","really long sql statement number 039");
    storeSQLstmt("key040","really long sql statement number 040");
    storeSQLstmt("key041","really long sql statement number 041");
    storeSQLstmt("key085","really long sql statement number 085");
    storeSQLstmt("key086","really long sql statement number 086");
    storeSQLstmt("key087","really long sql statement number 087");
    storeSQLstmt("key088","really long sql statement number 088");
    storeSQLstmt("key090","really long sql statement number 090");
    storeSQLstmt("key091","really long sql statement number 091");
    storeSQLstmt("key092","really long sql statement number 092");
    storeSQLstmt("key068","really long sql statement number 068");
    storeSQLstmt("key069","really long sql statement number 069");
    storeSQLstmt("key070","really long sql statement number 070");
    storeSQLstmt("key071","really long sql statement number 071");
    storeSQLstmt("key072","really long sql statement number 072");
    storeSQLstmt("key073","really long sql statement number 073");
    storeSQLstmt("key074","really long sql statement number 074");
    storeSQLstmt("key075","really long sql statement number 075");
    storeSQLstmt("key076","really long sql statement number 076");
    storeSQLstmt("key077","really long sql statement number 077");
    storeSQLstmt("key078","really long sql statement number 078");
    storeSQLstmt("key184","really long sql statement number 184");
    storeSQLstmt("key079","really long sql statement number 079");
    storeSQLstmt("key080","really long sql statement number 080");
    storeSQLstmt("key081","really long sql statement number 081");
    storeSQLstmt("key082","really long sql statement number 082");
    storeSQLstmt("key083","really long sql statement number 083");
    storeSQLstmt("key084","really long sql statement number 084");
    storeSQLstmt("key239","really long sql statement number 239");
    storeSQLstmt("key240","really long sql statement number 240");
    storeSQLstmt("key241","really long sql statement number 241");
    storeSQLstmt("key242","really long sql statement number 242");
    storeSQLstmt("key243","really long sql statement number 243");
    storeSQLstmt("key244","really long sql statement number 244");
    storeSQLstmt("key245","really long sql statement number 245");
    storeSQLstmt("key246","really long sql statement number 246");
    storeSQLstmt("key247","really long sql statement number 247");
    storeSQLstmt("key248","really long sql statement number 248");
    storeSQLstmt("key249","really long sql statement number 249");
    storeSQLstmt("key250","really long sql statement number 250");
    storeSQLstmt("key042","really long sql statement number 042");
    storeSQLstmt("key176","really long sql statement number 176");
    storeSQLstmt("key177","really long sql statement number 177");
    storeSQLstmt("key178","really long sql statement number 178");
    storeSQLstmt("key179","really long sql statement number 179");
    storeSQLstmt("key180","really long sql statement number 180");
    storeSQLstmt("key181","really long sql statement number 181");
    storeSQLstmt("key182","really long sql statement number 182");
    storeSQLstmt("key183","really long sql statement number 183");
    storeSQLstmt("key185","really long sql statement number 185");
    storeSQLstmt("key186","really long sql statement number 186");
    storeSQLstmt("key187","really long sql statement number 187");
    storeSQLstmt("key188","really long sql statement number 188");
    storeSQLstmt("key189","really long sql statement number 189");
    storeSQLstmt("key190","really long sql statement number 190");
    storeSQLstmt("key191","really long sql statement number 191");
    storeSQLstmt("key192","really long sql statement number 192");
    storeSQLstmt("key206","really long sql statement number 206");
    storeSQLstmt("key193","really long sql statement number 193");
    storeSQLstmt("key194","really long sql statement number 194");
    storeSQLstmt("key195","really long sql statement number 195");
    storeSQLstmt("key196","really long sql statement number 196");
    storeSQLstmt("key197","really long sql statement number 197");
    storeSQLstmt("key198","really long sql statement number 198");
    storeSQLstmt("key199","really long sql statement number 199");
    storeSQLstmt("key200","really long sql statement number 200");
    storeSQLstmt("key201","really long sql statement number 201");
    storeSQLstmt("key202","really long sql statement number 202");
    storeSQLstmt("key203","really long sql statement number 203");
    storeSQLstmt("key204","really long sql statement number 204");
    storeSQLstmt("key205","really long sql statement number 205");
    storeSQLstmt("key207","really long sql statement number 207");
    storeSQLstmt("key208","really long sql statement number 208");
    storeSQLstmt("key209","really long sql statement number 209");
    storeSQLstmt("key210","really long sql statement number 210");
    storeSQLstmt("key211","really long sql statement number 211");
    storeSQLstmt("key212","really long sql statement number 212");
    storeSQLstmt("key235","really long sql statement number 235");
    storeSQLstmt("key213","really long sql statement number 213");
    storeSQLstmt("key214","really long sql statement number 214");
    storeSQLstmt("key215","really long sql statement number 215");
    storeSQLstmt("key216","really long sql statement number 216");
    storeSQLstmt("key217","really long sql statement number 217");
    storeSQLstmt("key218","really long sql statement number 218");
    storeSQLstmt("key219","really long sql statement number 219");
    storeSQLstmt("key233","really long sql statement number 233");
    storeSQLstmt("key234","really long sql statement number 234");
    storeSQLstmt("key236","really long sql statement number 236");
    storeSQLstmt("key237","really long sql statement number 237");
    storeSQLstmt("key238","really long sql statement number 238");
    storeSQLstmt("keyfail","too many - fail");

    printf("Stored %i statements\n\n", SQLcountStored);

    for (i=0; i < MAXSQLSTMTS; i++) {
        printf("key=%s  ", SQLkey[i]);
        printf("value=%s\n", SQLstmt[i]);
    }

    printf("\n");
    printf("key115 value =\"%s\"\n", SQLstmt[SQLkeysearch("key115")]);
    printf("cry115 value =\"%s\"\n", SQLstmt[SQLkeysearch("cry115")]);
    printf("key041 value =\"%s\"\n", SQLstmt[SQLkeysearch("key041")]);
    printf("key210 value =\"%s\"\n", SQLstmt[SQLkeysearch("key210")]);
    printf("keyfail value =\"%s\"\n", SQLstmt[SQLkeysearch("keyfail")]);
    printf("key183 value =\"%s\"\n", SQLstmt[SQLkeysearch("key183")]);

    SQLCleanup();

    return 0;
}

