#include "chill.h"

int chill_exit(char **args){
    (void)args;
    terminateLoading();
    exit(0);
}

int chill_echo(char **args){
    int start = 1;
    bool newline = true; 

    if(!args || !args[0]){
        return 1;
    }

    if(args[1] && !strcmp(args[1], "-n")){
        newline = 0;
        start = 2;
    }

    for(int i = start; args[i]; i ++)
        printf(Y "%s " RST, args[i]);
    
    if(newline)
        printf("\n");
    

    return 0;



}