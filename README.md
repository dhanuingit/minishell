# Minishell

A minimal Unix shell written in **C** that reads commands, creates child processes and executes them, with signal handling and pipe support.

## Features
- Custom prompt (changeable through `PS1=`)
- Execution of external commands using `fork` and `execvp`
- Built-ins: `cd`, `pwd`, `exit`, `echo $?`, `echo $$`, `echo $SHELL`
- Pipes (`cmd1 | cmd2 | cmd3`)
- Signal handling: `Ctrl+C` (SIGINT), `Ctrl+Z` (SIGTSTP)
- Exit status tracking

## Concepts used
Process creation (`fork`), program execution (`execvp`), `wait`/`waitpid`, signals, pipes and IPC (`pipe`, `dup2`), string manipulation, dynamic memory, 2D arrays.

## Project structure
```
minishell/
├── main.c
├── shell.h
├── commands.c
├── scan_input.c
├── ll_functions.c
├── external_commands.txt
├── Makefile         # optional
└── README.md
```

## Build
```bash
gcc *.c -o minishell
```

## Run
```bash
./minishell
```

## Example
```
minishell$ ls -l | grep .c | wc -l
minishell$ echo $?
minishell$ exit
```

## Author
DHANUSH - [GitHub](https://github.com/dhanuingit)
