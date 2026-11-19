#include "shell.h"
extern int exit_flag;
extern char *external_cmd[152];
extern int status, pid;
extern Sl *process_list;
// List o/*builtin commands*/
char *builtins[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval",
					"set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
					"exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help", "jobs", "fg", "bg", NULL};

char *get_command(char *input_str)
{
	// printf("%s\n",input_str);
	char *buf = malloc(50);
	if(buf == NULL){
		perror("malloc");
	}
	int i = 0;
	for (i = 0; input_str[i] != '\0' && input_str[i] != ' '; i++)
	{
		// printf("%c\n",buf[i]);
		buf[i] = input_str[i];
	}
	buf[i] = '\0';
	return buf;
}

int check_command_type(char *cmd)
{
	// printf("%s\n",cmd);
	// compare cmd[] with builtin[][];
	for (int i = 0; builtins[i] != NULL; i++)
	{
		if (!strcmp(cmd, builtins[i]))
		{
			return BUILTIN;
		}
	}

	// both are matching
	for (int i = 0; external_cmd[i] != NULL; i++)
	{ // else if(/* compare cmd[] with external_cmd[][];*/1){
		if (!strcmp(cmd, external_cmd[i]))
		{
			return EXTERNAL;
		}
	}

	return NO_COMMAND;
}

void extract_external_commands(char **external_cmd)
{

	FILE *fptr = fopen("external_commands.txt", "r");
	if (fptr == NULL)
	{
		printf("Error Occured while opening External_commands.txt file\n");
	}

	char buf[50];
	int i = 0;
	while (fscanf(fptr, "%[^\n]", buf) != EOF)
	{
		fgetc(fptr);
		int len = strlen(buf);
		external_cmd[i] = malloc(len + 1 * sizeof(char));
		if (external_cmd[i] == NULL)
		{
			printf("Error Occurred due to failure in memory allocation failed\n");
			return;
		}
		strcpy(external_cmd[i++], buf);
	}

}

void execute_internal_commands(char *input_string)
{
	// i) cd
	// ii) pwd
	// iii)exit

	/*  echo $$
		echo $?
		echo $SHELL*/

	/*check the input string contains cd or not


	  if it is
	  syscall - chdir();

	  chdir(input_str + 3);

	  else check if the input_str contains pwd or not
	  if it is pwd - use lib call getcwd() func
	  char buf[50];
	  getcwd(buff,50);
	  print the buf



	  else check if the cmd is exit
	  if it is just call exit func call
	  exit(0);
	 */

	if (strncmp(input_string, "cd", 2) == 0)
	{
		if (strlen(input_string) < 3)
		{
			if(chdir("/home/dhanush")){
				perror("chdir");
			}
		}
		else
		{
		if(chdir(input_string + 3)){
				perror("chdir");
			}
		}
	}
	else if (strncmp(input_string, "pwd", 3) == 0)
	{
		char buf[100];
		if(getcwd(buf, 100) == NULL){
			perror("getcwd");
		}
		printf("%s\n", buf);
	}
	else if (strncmp(input_string, "exit", 4) == 0)
	{
		exit_flag = 1;
		return;
	}
	else if (strncmp(input_string, "echo $$", 7) == 0)
	{
		// check the input_str contains echo $$ or not
		// if it is
		printf("%d\n", getpid());
	}
	else if (strncmp(input_string, "echo $?", 7) == 0)
	{
		// check the input_str contains echo $? or not
		// if it is
		if (WIFEXITED(status))
		{
			printf("%d\n", WEXITSTATUS(status));
		}
		else
		{
			printf("%d", status);
		}
	}
	else if (strncmp(input_string, "echo $SHELL", 11) == 0)
	{
		// check the input_Str contains echo $SHELL or not
		// if it is

		printf("%s\n", getenv("SHELL"));
	}
	else if (strcmp(input_string, "jobs") == 0)
	{
		print_ll(process_list);
	}
	else if (strcmp(input_string, "fg") == 0)
	{
		char buf[20];
		if (delete_first(&process_list, &pid, buf) == SUCCESS)
		{
			if(kill(pid, SIGCONT)){
				perror("kill");
			}
			printf("%s\n", buf);
			if(waitpid(pid, &status, WUNTRACED)== -1){
				perror("waitpid");
			}
		}
	}
	else if (strcmp(input_string, "bg") == 0)
	{
		char buf[20];
		if (delete_first(&process_list, &pid, buf) == SUCCESS)
		{
			if(kill(pid, SIGCONT)){
				perror("kill");
			}
			signal(SIGCHLD, signal_handler);
		}
	}
}

void execute_external_commands(char *input_string)
{
	char *token = strtok(input_string, " ");
	char pipe_flag = 0, len = 0;
	char **mut_str = malloc(sizeof(char *) * 1);
	if(mut_str == NULL){
		perror("malloc");
	}
	int index = 1;

	while (token != NULL)
	{
		len = strlen(token);
		char **ptr = realloc(mut_str, sizeof(char *) * (index + 1));

		if (ptr == NULL)
		{
			printf("Failed to allocate memory\n");
			free(mut_str);
			return;
		}

		mut_str = ptr;

		mut_str[index - 1] = malloc((len * sizeof(char)) + 2);
		if (mut_str[index - 1] == NULL)
		{
			printf("Failed to allocate the memory\n");
			return;
		}
		if (strchr(token, '|') != NULL)
		{
			pipe_flag = 1;
		}
		strcpy(mut_str[index - 1], token);

		index++;

		token = strtok(NULL, " ");
	}

	mut_str[index] = NULL;

	// Access the tokens
	// for (int i = 0; i < index-1; i++)
	// {
	//     printf("%s\n", mut_str[i]);
	// }

	// char mut_str_cmd[row][10];

	// check the pipe is present or not
	// not present
	//  execvp(cmd[0],cmd);

	// present
	//  logic for n_pipe

	if (pipe_flag == 0)
	{
		if(execvp(mut_str[0], mut_str)==-1){
			perror("ececvp");
		}
		// printf("executing the execvp without pipe\n");
	}
	else
	{
		// printf("index = %d\n",index);
		// printf("executing the execvp with pipe\n");
		if (execvp_cmd(index - 1, mut_str))
		{
			printf("Error occured while executing multiple cmd\n");
		}
		else
		{
			// printf("Exectued execvp cmd Successfully\n");
		}
	}

	for (int i = 0; i < index - 1; i++)
	{
		
		free(mut_str[i]);
		// printf("index - %d -- %d\n",index-1,i);
	}
	// printf("done\n");
	free(mut_str);
}

#if 1
int execvp_cmd(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("Insufficient arguments passed\n");
		return 1;
	}

	int cmd_pos[argc];
	cmd_pos[0] = 0;
	int cmd_count = 1;

	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "|") == 0)
		{
			argv[i] = NULL;
			cmd_pos[cmd_count++] = i + 1;
		}
	}

	if (cmd_count < 2)
	{
		printf("No pipe symbol passed\n");
		return 1;
	}

	// reject empty commands such as "ls | | wc" or a trailing "|"
	// for (int i = 0; i < cmd_count; i++) {
	//     if (cmd_pos[i] >= argc || argv[cmd_pos[i]] == NULL) {
	//         printf("Empty command near pipe\n");
	//         return 1;
	//     }
	// }

	int prev_read = -1;
	int launched = 0;

	for (int i = 0; i < cmd_count; i++)
	{
		int fd[2];

		if (i < cmd_count - 1)
		{
			if (pipe(fd) == -1)
			{
				perror("pipe");
				break;
			}
		}

		pid_t pid = fork();
		if (pid < 0)
		{ // error handling
			perror("fork");
			if (i < cmd_count - 1)
			{
				close(fd[0]);
				close(fd[1]);
			}
			break;
		}

		if (pid == 0)
		{ // ---- child ----
			if (prev_read != -1)
			{
				dup2(prev_read, 0);
				close(prev_read);
			}
			if (i < cmd_count - 1)
			{
				close(fd[0]);
				dup2(fd[1], 1);
				close(fd[1]);
			}
			execvp(argv[cmd_pos[i]], argv + (cmd_pos[i]));
			perror("execvp");
			_exit(1);
		}

		launched++; // ---- parent ----
		if (prev_read != -1)
			close(prev_read);
		if (i < cmd_count - 1)
		{
			close(fd[1]);
			prev_read = fd[0];
		}
		else
		{
			prev_read = -1;
		}
	}

	if (prev_read != -1) // cleanup if we broke out early
		close(prev_read);

	for (int i = 0; i < launched; i++) // wait AFTER all are started
		wait(NULL);

	return 0;
}

#endif

#if 0

int execvp_cmd(int argc,char * argv[]){
    if(argc<2){
        printf("Insuffiecient argument passed\n");
        return 1;
    }
    int cmd_pos[argc];
    cmd_pos[0] = 0;
    int cmd_count = 1;
    for(int i =1;i<argc;i++){
        if(strcmp((argv[i]),"|") == 0){
            *(argv+i) = NULL;
            cmd_pos[cmd_count] = i+1;
            cmd_count++;
        }
    }
    if(cmd_count<2){
        printf("No Pipe symbol passed\n");
        return 1;
    }


    for(int i = 0;i<cmd_count;i++){
        int fd[2];
        if(i<cmd_count-1){
            if(pipe(fd)==-1){
                perror("pipe");
                return 1;
            }
        }

        int pid = fork();
        if(pid>0)
        {  
            // printf("Parent process - %d",getpid());
             if(i<cmd_count-1){
            close(fd[1]);
            dup2(fd[0],0);
            close(fd[0]);
        }
            wait(NULL);

        }
        else if(pid == 0)
        {   sleep(1);
            printf("child process - %d",getpid());
            if(i<cmd_count-1)
           {
            close(fd[0]);
            dup2(fd[1],1);
           }
            execvp(argv[cmd_pos[i]],argv+(cmd_pos[i]));
        }
        else{
            printf("Error Occurred\n");
        }
    }

    
}

#endif