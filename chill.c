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



void chill_launch(char **args, pid_t pid){
    if(pid == 0){ //child process
        excvp(args[0], args);
    }else{//parent   
        wt(&status);
    }
}



char ** chill_split_command(char *line){
    char **tokens; //array of tokens separated by white space
    char *rest = (char *)mloc(128 * sizeof(char)); //for strtok
    strcpy(rest, line);

    tokens = mloc(BUFSIZ * sizeof *tokens);
    unsigned int i = 0;

    for(char *token = strtok_r(rest, DEL, &rest); token; token = strtok_r(NULL, DEL, &rest)){
        tokens[i ++] = token;
    }

    tokens[i] = NULL;
    return tokens;

}

void chill_pipe(char *line){
    int pipesNum = 0, cmdc = 0;
    pid_t pid;
    char **args;
    char *rest = (char *)mloc(128 * sizeof(char)); // for strtok
    strcpy(rest, line);

    //count pipes
    for(int i = 0; line[i]; i ++){ 
        if(line[i] == '|'){
            pipesNum ++;
        }
    }

    //allocate memory for file descriptors
    int *pipefds = (int*)mloc(2 * pipesNum * sizeof(int));

    //allocate memory for a command
    char *token = (char*)mloc(128 * sizeof(char));

    token = strtok_r(rest, "|", &rest); //get first command

    //create pipes
    for(int i = 0; i < pipesNum; i ++)
        pip(pipefds + i * 2);

    do{
        pid = frk();
        if(pid == 0){ //child
            if(cmdc > 0)
                //if not the first process, take input from prev pipe read end
                dp2(pipefds[(cmdc - 1) * 2], STDIN_FILENO);
            


            if(cmdc < pipesNum)
                //if not last process, redirect output to the write end of this pipe
                dp2(pipefds[cmdc * 2 + 1], STDOUT_FILENO);
            
            //close all pipe ends for this process
            for(int i = 0; i < 2 * pipesNum; i ++)
                close(pipefds[i]);
            
        }else{ //parent
            //close pipe ends not needed anymore, to return EOF
            if(cmdc > 0)
                close(pipefds[(cmdc - 1) * 2]);
            
            if(cmdc < pipesNum)
                close(pipefds[cmdc * 2 + 1]);
            
        }

        //both
        args = chill_split_command(token);
        chill_launch(args, pid);

        cmdc ++; //parent

    }while(cmdc <= pipesNum && (token = strtok_r(NULL, "|", &rest))); //do this untill no commands left


    free(pipefds); 


}

void chill_exec(char *line){
    int i;
    bool pipe = 0;
    const char *curr_builtin;
    char ** args = chill_split_command(line);

    if(!args || !args[0]){
        return;
    }

    //check for pipelining 
    for(int i = 0; args[i]; i ++){
        if(!strcmp(args[i], "|")){
            pipe = 1;
            break;
        }
    }

    if(pipe){
        chill_pipe(line);
    }else{
        //check if it is a builtin
        i = 0;
        while((curr_builtin = builtins[i].name)){
            if(!strcmp(args[0], curr_builtin)){ //builtin exists

                if((status = (builtins[i].func)(args))){
                    printf(R "%s failed\n" RST, curr_builtin);
                }

                return;
            }
            i ++;
        }

        //if not builtin
        chill_pipe(line);

    }
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
    while(line = chill_read_line()){ //READ

        //EVALUATE, PRInt
        chill_exec(line);

        //free memory
        free(line);

        //LOOP
    }

    return EXIT_SUCCESS;
}
