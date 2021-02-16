/**
 * This file handles tasks.
*/

#include <stdio.h>
#include <stdbool.h> // For 'bool' data type.

#include "utils.c" // Contains utility functions.

// Constant for the filename.
#define FILENAME_TODO "todo.dat"
#define FILENAME_TASKS_TEMP "tasks.tmp"
#define FILENAME_DONE "done.dat"

#define COMMAND_ADD "add"
#define COMMAND_EDIT "edit"
#define COMMAND_REMOVE "remove"
#define COMMAND_LIST "list"
#define COMMAND_MARK "mark"

#define MARK_DONE "done"
#define MARK_TODO "todo"

#define COMMAND_EXIT "exit"

#define TAG_ERROR "error"

// Constants for sizes in characters.
#define LENGTH_TITLE_MAX 60
#define WIDTH_LIST (LENGTH_TITLE_MAX + 4 + 4)

typedef struct cmd_args
{
    char name[10];
    bool is_all;
    char mark_option[6];
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
void list_tasks(CMD *cmd)
{
    Task task;

    FILE *fptr;

    // Opening the data file in read mode & storing its pointer.
    if (strcmp(cmd->mark_option, MARK_DONE) == 0)
    {
        fptr = fopen(FILENAME_DONE, "r");
        
        printf("List of done tasks:\n");
    }
    else
    {
        fptr = fopen(FILENAME_TODO, "r");

        printf("List of todo tasks:\n");
    }

    // Checking for any error while opening file.
    if (fptr == NULL)
    {
        // Displaying error.
        printf("%s: error while accessing data.\n", TAG_ERROR);
        exit(EXIT_FAILURE);
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

bool validate_cmd(CMD *cmd)
{
    if (strcmp(cmd->name, COMMAND_MARK) == 0 && strcmp(cmd->mark_option, NULL_STR) == 0)
    {
        printf("%s: mandatory options are missing.\nUse help command to see available options.\n",
               cmd->name);
        return false;
    }

    return true;
}

/**
 * Updates i.e. edits or deletes the existing task from the data file.
*/
void update_tasks(CMD *cmd, Task *task)
{
    if (!validate_cmd(cmd))
        return;

    if (!validate_task(cmd, task))
        return;

    Task oldtask;

    FILE *srcfptr;
    FILE *tempfptr;
    FILE *destfptr;

    // Stores whether any task was updated or not.
    bool is_successful = false;

    if (cmd->is_all)
    {
        tempfptr = fopen(FILENAME_TASKS_TEMP, "w");

        if (strcmp(cmd->mark_option, MARK_DONE) == 0)
        {
            srcfptr = fopen(FILENAME_TODO, "r");
            destfptr = fopen(FILENAME_DONE, "a");

            // Checking for any error while opening files.
            if (destfptr == NULL)
            {
                printf("%s: error while accessing data.\n", TAG_ERROR);
                exit(EXIT_FAILURE);
            }
        }
        else if (strcmp(cmd->mark_option, MARK_TODO) == 0)
        {
            srcfptr = fopen(FILENAME_DONE, "r");
            destfptr = fopen(FILENAME_TODO, "a");

            // Checking for any error while opening files.
            if (destfptr == NULL)
            {
                printf("%s: error while accessing data.\n", TAG_ERROR);
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            srcfptr = fopen(FILENAME_TODO, "r");
        }

        // Checking for any error while opening files.
        if (srcfptr == NULL || tempfptr == NULL)
        {
            // Displaying error.
            printf("%s: error while accessing data.\n", TAG_ERROR);
            exit(EXIT_FAILURE);
        }

        for (oldtask.id = 1; fscanf(srcfptr, "%60[^,], ", oldtask.title) != EOF; oldtask.id++)
        {
            if (strcmp(cmd->name, COMMAND_EDIT) == 0)
            {
                fprintf(tempfptr, "%s, \n", task->title);

                printf("[+/-] Edited task \"%s\" successfully at #%02d.\n",
                       oldtask.title, oldtask.id);
            }
            else if (strcmp(cmd->name, COMMAND_REMOVE) == 0)
            {
                printf("[-] Removed task \"%s\" successfully from #%02d.\n",
                       oldtask.title, oldtask.id);
            }
            else if (strcmp(cmd->name, COMMAND_MARK) == 0)
            {
                fprintf(destfptr, "%s, \n", oldtask.title);

                printf("[%s] Marked task \"%s\" as %s successfully.\n",
                       strcmp(cmd->mark_option, MARK_DONE) == 0
                           ? "\u2713"
                           : "~",
                       oldtask.title,
                       strcmp(cmd->mark_option, MARK_DONE) == 0
                           ? "done"
                           : "todo");
            }
        }

        // Storing that the task was updated.
        is_successful = true;

        // Closing the data files to release allocated memory and applying changes.
        fclose(tempfptr);
        if (strcmp(cmd->mark_option, MARK_DONE) == 0)
        {
            fclose(srcfptr);
            fclose(destfptr);

            remove(FILENAME_TODO);
            rename(FILENAME_TASKS_TEMP, FILENAME_TODO);
        }
        else if (strcmp(cmd->mark_option, MARK_TODO) == 0)
        {
            fclose(srcfptr);
            fclose(destfptr);

            remove(FILENAME_DONE);
            rename(FILENAME_TASKS_TEMP, FILENAME_DONE);
        }
        else
        {
            fclose(srcfptr);

            remove(FILENAME_TODO);
            rename(FILENAME_TASKS_TEMP, FILENAME_TODO);
        }

        cmd->is_all = false;
    }
    else if (task->id == 0 && strcmp(cmd->name, COMMAND_ADD) == 0)
    {
        srcfptr = fopen(FILENAME_TODO, "a");

        // Checking for any error while opening files.
        if (srcfptr == NULL)
        {
            // Displaying error.
            printf("%s: error while accessing data.\n", TAG_ERROR);
            exit(EXIT_FAILURE);
        }

        // Saving task data in the file.
        fprintf(srcfptr, "%s, \n", task->title);

        printf("[+] Added task \"%s\" successfully at the end of the list.\n", task->title);

        // Storing that the task was updated.
        is_successful = true;

        // Closing the data files to release allocated memory.
        fclose(srcfptr);
    }
    else
    {
        tempfptr = fopen(FILENAME_TASKS_TEMP, "w");

        if (strcmp(cmd->mark_option, MARK_DONE) == 0)
        {
            srcfptr = fopen(FILENAME_TODO, "r");
            destfptr = fopen(FILENAME_DONE, "a");

            // Checking for any error while opening files.
            if (destfptr == NULL)
            {
                printf("%s: error while accessing data.\n", TAG_ERROR);
                exit(EXIT_FAILURE);
            }
        }
        else if (strcmp(cmd->mark_option, MARK_TODO) == 0)
        {
            srcfptr = fopen(FILENAME_DONE, "r");
            destfptr = fopen(FILENAME_TODO, "a");

            // Checking for any error while opening files.
            if (destfptr == NULL)
            {
                printf("%s: error while accessing data.\n", TAG_ERROR);
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            srcfptr = fopen(FILENAME_TODO, "r");
        }

        // Checking for any error while opening files.
        if (srcfptr == NULL || tempfptr == NULL)
        {
            // Displaying error.
            printf("%s: error while accessing data.\n", TAG_ERROR);
            exit(EXIT_FAILURE);
        }

        // Looping to get data from the file till the end of the file.
        for (oldtask.id = 1; fscanf(srcfptr, "%60[^,], ", oldtask.title) != EOF; oldtask.id++)
        {
            // Checking whether the current task is to be updated or not.
            if (oldtask.id == task->id)
            {
                if (strcmp(cmd->name, COMMAND_ADD) == 0)
                {
                    fprintf(tempfptr, "%s, \n", task->title);
                    fprintf(tempfptr, "%s, \n", oldtask.title);

                    printf("[+] Added task \"%s\" successfully at #%02d.\n",
                           task->title, task->id);
                }
                else if (strcmp(cmd->name, COMMAND_EDIT) == 0)
                {
                    fprintf(tempfptr, "%s, \n", task->title);

                    printf("[+/-] Edited task \"%s\" successfully at #%02d.\n",
                           task->title, task->id);
                }
                else if (strcmp(cmd->name, COMMAND_REMOVE) == 0)
                {
                    printf("[-] Removed task \"%s\" successfully from #%02d.\n",
                           oldtask.title, oldtask.id);
                }
                else if (strcmp(cmd->name, COMMAND_MARK) == 0)
                {
                    fprintf(destfptr, "%s, \n", oldtask.title);

                    printf("[%s] Marked task \"%s\" as %s successfully.\n",
                           strcmp(cmd->mark_option, MARK_DONE) == 0
                               ? "\u2713"
                               : "~",
                           oldtask.title,
                           strcmp(cmd->mark_option, MARK_DONE) == 0
                               ? "done"
                               : "todo");
                }

                // Storing that the task was updated.
                is_successful = true;
            }
            else
            {
                fprintf(tempfptr, "%s, \n", oldtask.title);
            }
        }

        // Closing the data files to release allocated memory and applying changes.
        fclose(tempfptr);
        if (strcmp(cmd->mark_option, MARK_DONE) == 0)
        {
            fclose(srcfptr);
            fclose(destfptr);

            remove(FILENAME_TODO);
            rename(FILENAME_TASKS_TEMP, FILENAME_TODO);
        }
        else if (strcmp(cmd->mark_option, MARK_TODO) == 0)
        {
            fclose(srcfptr);
            fclose(destfptr);

            remove(FILENAME_DONE);
            rename(FILENAME_TASKS_TEMP, FILENAME_DONE);
        }
        else
        {
            fclose(srcfptr);

            remove(FILENAME_TODO);
            rename(FILENAME_TASKS_TEMP, FILENAME_TODO);
        }
    }

    // Checking whether the task was updated or not.
    if (!is_successful)
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
