#include "shell.h"
extern char * prompt;
int main()
{
prompt = malloc(50);

if (prompt == NULL)
{
    printf("Failed to allocate memory\n");
}
strcpy(prompt,"minishell");

    system("clear");

    scan_input(prompt);
}