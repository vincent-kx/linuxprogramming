/*
 * #include <stdlib.h>
 * char *getenv(const char *name);
 * int putenv(const char *string);
 * extern char **environ;
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char **environ;

void show_env()
{
    printf("*******************************************\n");
    char ** env = environ;
    while(*env)
    {
        printf("%s\n",*env);
        env++;
    }
    printf("*******************************************\n");
}

int main(int argc,char * argv[])
{
    if(argc == 2 && 0 == strcmp("show",argv[1]))
    {
        show_env();
    }
    if(argc == 3 && 0 == strcmp("show",argv[1]))
    {
        char * env_value = getenv(argv[2]);
        printf("%s = %s\n",argv[2],env_value);
    }
    if(argc == 4 && 0 == strcmp("set",argv[1]))
    {
        char * env_name = argv[2];
        char * env_value = argv[3];
        char * str_env = (char *)malloc(strlen(env_name) + strlen(env_value) + 2);
        strcpy(str_env,env_name);
        strcat(str_env,"=");
        strcat(str_env,env_value);
        putenv(str_env);
        free(str_env);

        printf("%s = %s\n",env_name,getenv(env_name));
    }
    return 0;
}
