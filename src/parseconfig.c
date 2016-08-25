#include <stdio.h>
#include <stdlib.h>
#include <libconfig.h>

int main()
{
    const char        *cfgfile = "test.cfg";
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
