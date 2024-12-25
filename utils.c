#include "chill.h"

//fork wrapper
pid_t frk(void){
    pid_t pid = fork();

    if(pid < 0){
        perror(R "fork FAIL" RST);
        exit(1);
    }

    return pid; //0 for child, pid of child for parent

}

//execvp wrapper
void excvp(const char *command, char *const argv[]){
    if(!command || !argv){
        fprintf(stderr, R "execvp invalid arguments\n" RST);
        exit(1);
    }

    if(execvp(command, argv) == -1){
        perror(R "execvp FAIL" RST);
        exit(1);
    }
}

//wait wrapper
pid_t wt(int *status){
    pid_t res;

    if(!status){
        fprintf(stderr, R "wait invalid argument");
        return -1;
    }

    res = wait(status);
    if(res == -1){
        perror(R "wait FAIL" RST);
    }

    if(WIFEXITED(*status)){
        *status = WEXITSTATUS(*status);
    }

    return res;
}

//pipe wrapper
void pip(int *pds){

    if(pipe(pds) < 0){
        perror(R "pipe creation FAIL" RST);
        exit(1);
    }
}

//dup2 wrapper
void dp2(int fd1, int fd2){
    if(dup2(fd1, fd2) < 0){
        perror(R "dup FAIL" RST);
        exit(1);
    }
}



//getcwd wrapper
void gcwd(char *buf, size_t size){
    if(getcwd(buf, size) == NULL)
        perror(R "getcwd FAIL" RST);
}

//malloc wrapper
void *mloc(size_t size){
    void *ptr;
    if(size == 0){
        return (NULL);
    }
    ptr = malloc(size);
    if(!ptr){
        perror(R "malloc FAIL" RST);
        exit(1);
    }

    return (ptr);

}



void printbanner(void){
    printf(M
" ██████╗██╗  ██╗██╗██╗     ██╗         ██╗   ██╗ ██╗\n"
"██╔════╝██║  ██║██║██║     ██║         ██║   ██║███║\n"
"██║     ███████║██║██║     ██║         ██║   ██║╚██║\n"
"██║     ██╔══██║██║██║     ██║         ╚██╗ ██╔╝ ██║\n"
"╚██████╗██║  ██║██║███████╗███████╗     ╚████╔╝  ██║\n"
" ╚═════╝╚═╝  ╚═╝╚═╝╚══════╝╚══════╝      ╚═══╝   ╚═╝\n" RST);
}


void terminateLoading(void){
    const char *frames[] = {
        "/ terminating \\",
        "- terminating -",
        "\\ terminating /",
        "| terminating |",
    };

    for(int i = 0; i < 5; i ++){
        for(int j = 0; j < 4; j ++){
            printf("\r" Y "%s" RST, frames[j]);
            fflush(stdout);
            usleep(100000);
        }


    }

    printf(C "\rTERMINATED       \n" RST);


}



