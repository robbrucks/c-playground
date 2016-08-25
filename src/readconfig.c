#include <stdio.h>
#include <stdlib.h>
#include <libconfig.h>
#include <unistd.h>
#include <glob.h>
#include <string.h>

#define MAXCONFFILES 100

char   *configPath[MAXCONFFILES];
int    fileCount = 0;

int getconffiles(const char *pattern)
{
    glob_t   paths;
    int      ret, strsize;

    paths.gl_pathc = 0;
    paths.gl_pathv = NULL;
    paths.gl_offs = 0;
    ret = glob( pattern, GLOB_NOCHECK, NULL, &paths );
    if( ret == 0 ) {
        for( fileCount = 0; fileCount < paths.gl_pathc; fileCount++ ) {
            if(fileCount >= MAXCONFFILES) {
                printf("Conf File limit hit\n");
                break;
            }
            printf("%s\n", paths.gl_pathv[fileCount]);
            strsize = strlen(paths.gl_pathv[fileCount]) + 1;
            configPath[fileCount] = malloc(strsize * sizeof(char));
            strcpy(configPath[fileCount],paths.gl_pathv[fileCount]);
        }
        globfree( &paths );
    } else {
        printf("glob() failed rc=%i", ret );
    }
    return fileCount;
}

int readconfig(const char *cfgfile)
{
    config_t          cfg;
    config_setting_t  *root = NULL, *element = NULL;
    int               parmct, i;
    const char        *setting_value = NULL, *setting_name = NULL;

    config_init(&cfg);
    if(! config_read_file(&cfg, cfgfile)) {
        printf("%s for file \"%s\"\n", config_error_text(&cfg),
            (config_error_file(&cfg) == NULL ? cfgfile : config_error_file(&cfg)));
        if(CONFIG_ERR_PARSE == config_error_type(&cfg)) 
            printf("Parsing error on or near line %i\n", config_error_line(&cfg));
        goto out;
    }
    root = config_root_setting(&cfg);
    parmct = config_setting_length(root);
    for (i = 0; i < parmct; i++) {
        element = config_setting_get_elem(root, i);
        setting_name = config_setting_name(element);
        if(CONFIG_TYPE_STRING == config_setting_type(element)) {
            setting_value = config_setting_get_string_elem(root, i);
            printf("Config Element \"%s\" = \"%s\"\n", setting_name, setting_value);
        }
        else
            printf("Config Element \"%s\" on line %i in file \"%s\" is not a string - ignoring it\n",
                   setting_name, config_setting_source_line(element), config_setting_source_file(element));
    }

out:
    config_destroy(&cfg);
    return 0;
}

int main() {
    int     i;

    if(0 == getconffiles("/home/rob.brucks/playground/configs.d/*.conf")) {
        printf("no files found\n");
        goto out;
    }
    printf("made it here\n");
    for(i = 0; i < fileCount; i++) {
        printf("config file %i = %s\n", i, configPath[i]);
        readconfig(configPath[i]);
    }
out:
    return 0;
}
