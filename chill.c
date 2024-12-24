#include "chill.h"

/*
                         .              
                        -%%. =**.       
                     ..:+%%--%%#.       
        .::--=++*##%%%%%%%%%#*#.        
       +@@@@@@%%%%%%%@@%%@@%%%#:        
       %@@@@@@%%%%%%%%%%%%%%%%%+        
       %@@@@@@%%%%%%%%%%%%%%%%%+        
       -%@@@@@%%%%%%%%%%%%%%%%%=        
         -*@@%%%%%%%%%%%%%%%%%%:        
            :-===%%%%%%%%%%%%%%.        
                 *%%%%%%%%%%%%%=        
                +%%%%%%%%%%%%%%#+       
               .%%%%%%%%%%%%%%%%#+      
               .%%%%%%%%%%%%%%%%%*      
                =@%%%%%%%%%%%%%%#.      
                 =%%%%%%%%%%%%%*        
                  %%%%%%%%%%%%%+        
                  -%%%%%%%%%%%%-        
                   =###%%%#####:        
                                        
*/




int status = 0;

//chill builtins
t_builtin builtins[] = {
    {.name = "echo", .func = chill_echo},
    {.name = "exit", .func = chill_exit},
    {.name = NULL},

};

void chill_launch(char **args){

    if(frk() == 0){ //child process
        excvp(args[0], args);
    }else{
        wt(&status); //wait for child termination
    }
}

void chill_exec(char **args){
    int i;
    const char *curr_builtin;

    if(!args || !args[0]){
        return;
    }

    i = 0;

    //check if it is a builtin
    while((curr_builtin = builtins[i].name)){
        if(!strcmp(args[0], curr_builtin)){ //builtin exists

            if((status = (builtins[i].func)(args))){
                printf(R "%s failed\n" RST, curr_builtin);
            }

            return;
        }
        i ++;
    }

    //if not builtin, fork and launch
    chill_launch(args);



}

char **chill_split_line(char *line){
    char **tokens;
    tokens = mloc(BUFSIZ * sizeof *tokens);
    unsigned int i = 0;

    for(char *token = strtok(line, DEL); token; token = strtok(NULL, DEL)){
        tokens[i ++] = token;
    }

    tokens[i] = NULL;
    return tokens;

}

char *chill_read_line(void){
    char *buf;
    size_t bufsize;
    char cwd[BUFSIZ];
    buf = NULL;

    gcwd(cwd,sizeof(cwd));
    
    printf(C "%s>> " RST, cwd);

    if(getline(&buf, &bufsize, stdin) == -1){
        free(buf);
        buf = NULL;
        if(feof(stdin))
            printf(R "EOF\n" RST);
        else
            printf(R "Getline FAIL\n" RST);
    }
    return buf;
}

int main(int argc, char **argv){

    printbanner();

    //SIMPLE SHELL
    // (void)argc; //tell the compiler it is intentionally not used
    // int status;

    // if(fork() == 0) //child process
    //     //execute, vector, path
    //     //takes an array of arguments and uses PATH to find executables
    //     // execvp("ls", argv);
    //     execvp(argv[1], argv + 1); 
    
    // wait(&status);

    //REPL
    char *line;
    char **args;
    while(line = chill_read_line()){ //1- get line
        //2- get tokens
        args = chill_split_line(line);

        //3- execute 
        chill_exec(args);

        //free memory
        free(line);
        free(args);

    }

    return EXIT_SUCCESS;
}
