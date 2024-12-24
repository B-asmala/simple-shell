#ifndef CHILL_H
#define CHILL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>

//ANSI Color Codes
#define Y "\033[1;33m"
#define G "\033[1;32m"
#define C "\033[1;36m"
#define R "\033[1;31m"
#define M "\033[35m"
#define BL "\033[30m"
#define RST "\033[0m"

//macros
#define DEL "\n\t \v\f\r"

//built-in commands structure
typedef struct s_builtin {
    const char *name;//name of builtin
    int (*func)(char **av); //pointer to its function

} t_builtin;

int chill_exit(char **args);
int chill_echo(char **args);

void terminateLoading(void);

pid_t frk(void);
pid_t wt(int *status);
void excvp(const char *command, char *const argv[]);
void gcwd(char *buf, size_t size);
void *mloc(size_t size);
void printbanner(void);

#endif

