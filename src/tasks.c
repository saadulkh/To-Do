/**
 * This file handles tasks.
*/

#include <stdio.h>
#include <stdbool.h> // For 'bool' data type.

#include "utils.c" // Contains utility functions.

// Constant for the filename.
#define FILENAME_TASKS "tasks.dat"
#define FILENAME_TASKS_TEMP "tasks.tmp"

#define COMMAND_ADD "add"
#define COMMAND_EDIT "edit"
#define COMMAND_REMOVE "remove"
#define COMMAND_LIST "list"
#define COMMAND_EXIT "exit"

#define TAG_ERROR "error"

// Constants for sizes in characters.
#define LENGTH_TITLE_MAX 60
#define WIDTH_LIST (LENGTH_TITLE_MAX + 4 + 4)

typedef struct cmd_args
{
    char name[10];
    bool is_all;
} CMD;

typedef struct task_args
{
    int id;
    char title[LENGTH_TITLE_MAX + 2]; // +1 for '\n' from fgets() & +1 for '\0' at the end of string.
} Task;

void init_task(Task *task)
{
    task->id = NULL_INT;
    strcpy(task->title, NULL_STR);
}

/**
 * Views existing tasks from the data file.
*/
void list_tasks()
{
    Task task;

    // Opening the data file in read mode & storing its pointer.
    FILE *fptr = fopen(FILENAME_TASKS, "r");

    // Checking for any error while opening file.
    if (fptr == NULL)
    {
        // Displaying error.
        printf("%s: error while accessing data.\n", TAG_ERROR);
    }

    // Displaying top horizontal line of the list.
    printcharln(0, WIDTH_LIST, '_', ' ');

    // Iterating throught tasks in file.
    for (task.id = 1; fscanf(fptr, "%[^,], ", task.title) != EOF; task.id++)
    {
        // Stores length of the title.
        int length_title = strlen(task.title);

        // Ignore task when title length is greater than equal to the max title length possible.
        if (LENGTH_TITLE_MAX < length_title)
        {
            continue;
        }

        // Displaying empty line for some space between tasks.
        printcharln(0, WIDTH_LIST, ' ', '|');

        // Displaying task no. and the task title.
        printf("| #%02d %s", task.id, task.title);
        printcharln(6 + length_title, WIDTH_LIST, ' ', '|');

        // Displaying bottom horizontal line of the list.
        printcharln(0, WIDTH_LIST, '_', '|');
    }

    if (task.id == 1)
        printf(" NOTHING HERE\n\n");

    // Closing the data file to release allocated memory.
    fclose(fptr);
}

void get_id(char *command, Task *task)
{
    printf("%s: [%s] Enter task no: ", command, !isnull(task->title) ? task->title : "~");
    scanf("%d", &task->id);
}

void get_title(char *command, Task *task)
{
    printf("%s: [%d] Enter new title: ", command, task->id);
    fgets(task->title, LENGTH_TITLE_MAX + 2, stdin); // +1 for '\n' & +1 for '\0'.
    int title_length = strcspn(task->title, "\n");

    if (title_length > LENGTH_TITLE_MAX)
    {
        while (getchar() != '\n')
            /* do nothing */;
        printf("%s: title length exceeds %d characters\n", TAG_ERROR, LENGTH_TITLE_MAX);
        strcpy(task->title, NULL_STR);
        return;
    }

    task->title[title_length] = '\0';
}

bool validate_task(CMD *cmd, Task *task)
{
    while (task->id <= 0)
    {
        if (strcmp(cmd->name, COMMAND_ADD) == 0)
        {
            if (cmd->is_all)
            {
                printf("%s: '--all' option cannot be used with '%s' command.\n", TAG_ERROR, cmd->name);
                cmd->is_all = false;
            }

            task->id = 0;
            break;
        }
        else
        {
            if (cmd->is_all)
            {
                break;
            }

            get_id(cmd->name, task);

            char exit_cmd[5];
            scanf("%5[^\n]", exit_cmd);
            while (getchar() != '\n')
                /* do nothing */;

            if (strcmp(exit_cmd, COMMAND_EXIT) == 0)
                return false;
        }
    }

    while (isnull(task->title) &&
           (strcmp(cmd->name, COMMAND_ADD) == 0 ||
            strcmp(cmd->name, COMMAND_EDIT) == 0))
    {
        get_title(cmd->name, task);

        if (strcmp(task->title, COMMAND_EXIT) == 0)
            return false;
    }

    return true;
}

/**
 * Updates i.e. edits or deletes the existing task from the data file.
*/
void update_tasks(CMD *cmd, Task *task)
{
    if (!validate_task(cmd, task))
        return;

    Task oldtask;

    FILE *oldfptr;
    FILE *newfptr;

    if (cmd->is_all)
    {
        oldfptr = fopen(FILENAME_TASKS, "r");
        newfptr = fopen(FILENAME_TASKS_TEMP, "w");

        // Checking for any error while opening files.
        if (oldfptr == NULL || newfptr == NULL)
        {
            // Displaying error.
            printf("%s: error while accessing data.\n", TAG_ERROR);
        }

        for (oldtask.id = 1; fscanf(oldfptr, "%60[^,], ", oldtask.title) != EOF; oldtask.id++)
        {
            if (strcmp(cmd->name, COMMAND_EDIT) == 0)
            {
                fprintf(newfptr, "%s, \n", task->title);

                printf("[+/-] Edited task \"%s\" successfully at #%02d.\n",
                       oldtask.title, oldtask.id);
            }
            else if (strcmp(cmd->name, COMMAND_REMOVE) == 0)
            {
                printf("[-] Removed task \"%s\" successfully from #%02d.\n",
                       oldtask.title, oldtask.id);
            }
        }

        // Closing the data files to release allocated memory.
        fclose(oldfptr);
        fclose(newfptr);

        // Applying changes.
        remove(FILENAME_TASKS);
        rename(FILENAME_TASKS_TEMP, FILENAME_TASKS);

        cmd->is_all = false;
    }
    else
    {
        // Stores whether any task was updated or not.
        bool is_updated = false;

        if (task->id == 0 && strcmp(cmd->name, COMMAND_ADD) == 0)
        {
            oldfptr = fopen(FILENAME_TASKS, "a");

            // Checking for any error while opening files.
            if (oldfptr == NULL)
            {
                // Displaying error.
                printf("%s: error while accessing data.\n", TAG_ERROR);
            }

            // Saving task data in the file.
            fprintf(oldfptr, "%s, \n", task->title);

            printf("[+] Added task \"%s\" successfully at the end of the list.\n", task->title);

            // Storing that the task was updated.
            is_updated = true;

            // Closing the data files to release allocated memory.
            fclose(oldfptr);
        }
        else
        {
            oldfptr = fopen(FILENAME_TASKS, "r");
            newfptr = fopen(FILENAME_TASKS_TEMP, "w");

            // Checking for any error while opening files.
            if (oldfptr == NULL || newfptr == NULL)
            {
                // Displaying error.
                printf("%s: error while accessing data.\n", TAG_ERROR);
            }

            // Looping to get data from the file till the end of the file.
            for (oldtask.id = 1; fscanf(oldfptr, "%60[^,], ", oldtask.title) != EOF; oldtask.id++)
            {
                // Checking whether the current task is to be updated or not.
                if (oldtask.id == task->id)
                {
                    if (strcmp(cmd->name, COMMAND_ADD) == 0)
                    {
                        fprintf(newfptr, "%s, \n", task->title);
                        fprintf(newfptr, "%s, \n", oldtask.title);

                        printf("[+] Added task \"%s\" successfully at #%02d.\n",
                               task->title, task->id);
                    }
                    else if (strcmp(cmd->name, COMMAND_EDIT) == 0)
                    {
                        fprintf(newfptr, "%s, \n", task->title);

                        printf("[+/-] Edited task \"%s\" successfully at #%02d.\n",
                               task->title, task->id);
                    }
                    else if (strcmp(cmd->name, COMMAND_REMOVE) == 0)
                    {
                        printf("[-] Removed task \"%s\" successfully from #%02d.\n",
                               oldtask.title, oldtask.id);
                    }

                    // Storing that the task was updated.
                    is_updated = true;
                }
                else
                {
                    // Saving current task data in new file.
                    fprintf(newfptr, "%s, \n", oldtask.title);
                }
            }

            // Closing the data files to release allocated memory.
            fclose(oldfptr);
            fclose(newfptr);

            // Applying changes.
            remove(FILENAME_TASKS);
            rename(FILENAME_TASKS_TEMP, FILENAME_TASKS);
        }

        // Checking whether the task was updated or not.
        if (!is_updated)
        {
            if (strcmp(cmd->name, COMMAND_ADD) == 0)
            {
                printf("%s: couldn't add task.\n", cmd->name);
            }
            else
            {
                // Displaying error msg that no task was found.
                printf("%s: no task found.\n", cmd->name);
            }
        }
    }
}
