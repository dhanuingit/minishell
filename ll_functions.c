#include "shell.h"

int insert_first(Sl **head, int PID, char *cmd)
{
    Sl *new = malloc(sizeof(Sl));
    if (new == NULL)
    {
        return FAILURE;
    }
    new->PID = PID;
    // printf("pid - %d\ncmd - %s\n",new->PID,cmd);
    strcpy(new->cmd, cmd);

    if ((*head) == NULL)
    {
        *head = new;
        new->link = NULL;
        return SUCCESS;
    }

    new->link = (*head);
    (*head) = new;

    // printf("pid - %d\ncmd - %s\n",(*head)->PID,(*head)->cmd);
    return SUCCESS;
}

int delete_first(Sl **head, int *pid, char *buf)
{
    if (*head == NULL)
    {
        printf("List is empty\n");
        return FAILURE;
    }

    *pid = (*head)->PID;
    strcpy(buf, (*head)->cmd);

    Sl *temp = *head;
    (*head) = (*head)->link;
    free(temp);
    return SUCCESS;
}

void print_ll(Sl *head)
{
    // it will print in reverse order
    static int i = 1;
    if (head == NULL)
    {
        return;
    }

    print_ll(head->link);
    printf("[%d]    stopped             %s\n", i++, head->cmd);
}