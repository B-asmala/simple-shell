# Basic Shell Implementation with Pipelining Support
This project is a custom implementation of a Linux shell that supports basic command execution along with pipelining. The aim of this project is to deepen my understanding of core operating system concepts, such as process execution, forking, and inter-process communication through pipes.

## Features
- Supports command execution with or without pipelines.
- Handles multiple commands connected by pipes "|".
- Built-in commands such as echo and exit.

## Clone
```
$ git clone https://github.com/B-asmala/simple-shell
```
## Compile
on a UNIX based system
```
$ gcc -o chill chill.c utils.c builtins.c
```
## Run
```
$ ./chill
```
