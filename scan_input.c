#include "shell.h"
int exit_flag;
int pid, status;
char *prompt;
char *input_str;
char *external_cmd[152];
Sl *process_list = NULL;

void signal_handler(int signum)
{

    if (signum == SIGINT)
    {
        if (pid == 0)
        {
            printf("\n");
            print_cwd();
        }
    }
    else if (signum == SIGTSTP)
    {
        if (pid == 0)
        {
            printf("\n");
            print_cwd();
        }
        else
        {
            insert_first(&process_list, pid, input_str);
        }
    }
    else if (signum == SIGCHLD)
    {
        if (waitpid(pid, &status, WNOHANG) == -1)
        {
            perror("waitpid");
        }
    }
    fflush(stdout);
}

void scan_input(char *prompt_a)
{
    input_str = malloc(50);

    signal(SIGINT, signal_handler); // registering the signalq
    signal(SIGTSTP, signal_handler);
    // int i = 0;
    extract_external_commands(external_cmd);
    while (1)
    {
        pid = 0;
        // if(i++>20)
        // {
        //     return;
        // }

        print_cwd();
        scanf(" %[^\n]", input_str);

        if (!strncmp(input_str, "PS1=", 4))
        {
            char *ptr = NULL;
            if (ptr = strchr(input_str, ' '))
            { // check the input_str[4] is space or not
                printf("%s: command not found\n", ptr + 1);
                continue;
            }
            // if no space
            strcpy(prompt_a, input_str + 4);
        }
        else
        {

            char *cmd = get_command(input_str);
            int ret = check_command_type(cmd);
            free(cmd);
            if (ret == BUILTIN)
            {
                // printf("Executing internal command\n");
                execute_internal_commands(input_str);
                if (exit_flag)
                {
                    free(input_str);
                    exit(0);
                }
            }
            else if (ret == EXTERNAL)
            {
                // printf("Execute external cmd\n");
                pid = fork();
                if (pid > 0) // parent process
                {
                    waitpid(pid, &status, WUNTRACED);
                }
                else if (pid == 0)
                {

                    signal(SIGINT, SIG_DFL);
                    signal(SIGTSTP, SIG_DFL);
                    execute_external_commands(input_str);
                }
                else
                {
                    printf("Error occurred while creating child process\n");
                }
            }
            else
            {
                printf("INVALID COMMAND\n");
            }
        }
    }
}

void print_cwd(void)
{

    char buf[100];
    getcwd(buf, 100);
    printf(ANSI_COLOR_GREEN "%s" ANSI_COLOR_RESET ":", prompt);
    printf(ANSI_COLOR_BLUE "%s" ANSI_COLOR_RESET, buf);
    printf("$ ");
    fflush(stdout);
}
