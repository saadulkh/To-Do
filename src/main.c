#include <stdio.h>
#include <stdlib.h>  // General purpose standard library
#include <stdbool.h> // For 'bool' data type.
#include <string.h>  // For working with strings.
#include <ctype.h>   // For type checks.

#include "tasks.c" // For hadling tasks.

#define COMMAND_VERSION "version"
#define COMMAND_HELP "help"

const char OPTION_DONE[2][7] = {"-d", "--done"};
const char OPTION_TODO[2][7] = {"-t", "--todo"};
const char OPTION_ALL[2][6] = {"-a", "--all"};
const char OPTION_VERSION[2][10] = {"-v", "--version"};
const char OPTION_HELP[2][7] = {"-h", "--help"};

#define NAME "todo"
#define VERSION NAME " 1.0.0"

#define USAGE_COMMAND_ADD "add             Add new task to the list.\n\
                    Multiple titles in batches are allowed."
#define USAGE_COMMAND_EDIT "edit            Edit existing task."
#define USAGE_COMMAND_REMOVE "remove          Remove task from the list."
#define USAGE_COMMAND_LIST "list            Show tasks list."
#define USAGE_COMMAND_MARK "mark            Mark a task as done or todo.\n\
                    Multiple task numbers in batches are allowed."
#define USAGE_COMMAND_VERSION "version         See version of " NAME "."
#define USAGE_COMMAND_HELP "help            See this help."

#define USAGE_OPTION_NUMBER "{NUMBER}        Task no/position of the task in the list."
#define USAGE_OPTION_TITLE "{TITLE}         Title of the task.\n\
                    Number/digit at the start is not allowed."
#define USAGE_OPTION_DONE "-d --done        Mark tasks as done or list done tasks."
#define USAGE_OPTION_TODO "-t --todo        Mark tasks as todo or list todo tasks."
#define USAGE_OPTION_ALL "-a --all        Perform operation on all tasks."
#define USAGE_OPTION_VERSION "-v --version    See version of " NAME "."
#define USAGE_OPTION_HELP "-h --help       See this help."

#define USAGE_ADD "Usage: ./" NAME " " COMMAND_ADD " [options...]\n\
\n\
Command\n\
   " USAGE_COMMAND_ADD "\n\
\n\
Options\n\
    " USAGE_OPTION_NUMBER "\n\
    " USAGE_OPTION_TITLE "\n\
    " USAGE_OPTION_HELP "\n"

#define USAGE_EDIT "Usage: ./" NAME " " COMMAND_EDIT " [options...]\n\
\n\
Command\n\
   " USAGE_COMMAND_EDIT "\n\
\n\
Options\n\
    " USAGE_OPTION_NUMBER "\n\
    " USAGE_OPTION_TITLE "\n\
    " USAGE_OPTION_ALL "\n\
    " USAGE_OPTION_HELP "\n"

#define USAGE_REMOVE "Usage: ./" NAME " " COMMAND_REMOVE " [options...]\n\
\n\
Command\n\
   " USAGE_COMMAND_REMOVE "\n\
\n\
Options\n\
    " USAGE_OPTION_NUMBER "\n\
    " USAGE_OPTION_ALL "\n\
    " USAGE_OPTION_HELP "\n"

#define USAGE_LIST "Usage: ./" NAME " " COMMAND_LIST " [options...]\n\
\n\
Command\n\
   " USAGE_COMMAND_LIST "\n\
\n\
Options\n\
    " USAGE_OPTION_DONE "\n\
    " USAGE_OPTION_TODO "\n\
    " USAGE_OPTION_HELP "\n"

#define USAGE_MARK "Usage: ./" NAME " " COMMAND_MARK " [options...]\n\
\n\
Command\n\
   " USAGE_COMMAND_MARK "\n\
\n\
Options\n\
    " USAGE_OPTION_NUMBER "\n\
    " USAGE_OPTION_DONE "\n\
    " USAGE_OPTION_TODO "\n\
    " USAGE_OPTION_ALL "\n\
    " USAGE_OPTION_HELP "\n"

#define USAGE "Usage: ./" NAME " [--help], [-h],\n\
            [--version], [-v],\n\
            <command> [options...]\n\
\n\
Enter 'exit', when prompted anything, to ignore command.\n\
\n\
Commands\n\
    " USAGE_COMMAND_ADD "\n\
    " USAGE_COMMAND_EDIT "\n\
    " USAGE_COMMAND_REMOVE "\n\
    " USAGE_COMMAND_LIST "\n\
    " USAGE_COMMAND_MARK "\n\
    " USAGE_COMMAND_HELP "\n\
\n\
Options\n\
    " USAGE_OPTION_NUMBER "\n\
    " USAGE_OPTION_TITLE "\n\
    " USAGE_OPTION_DONE "\n\
    " USAGE_OPTION_TODO "\n\
    " USAGE_OPTION_ALL "\n\
    " USAGE_OPTION_VERSION "\n\
    " USAGE_OPTION_HELP "\n\
\n\
Use './" NAME " help <command>' to see help for any command.\n"

void handle_args(int argc, char const *argv[])
{
    CMD cmd;
    strcpy(cmd.name, NULL_STR);
    cmd.is_all = false;
    strcpy(cmd.mark_option, NULL_STR);

    if (argc >= 2)
    {
        Task task;

        init_task(&task);

        for (int i = 1; i < argc; i++)
        {
            if (strcmp(cmd.name, COMMAND_HELP) == 0)
            {
                if (strcmp(argv[i], COMMAND_ADD) == 0)
                {
                    printf("%s\n", USAGE_ADD);
                }
                else if (strcmp(argv[i], COMMAND_EDIT) == 0)
                {
                    printf("%s\n", USAGE_EDIT);
                }
                else if (strcmp(argv[i], COMMAND_REMOVE) == 0)
                {
                    printf("%s\n", USAGE_REMOVE);
                }
                else if (strcmp(argv[i], COMMAND_LIST) == 0)
                {
                    printf("%s\n", USAGE_LIST);
                }
                else if (strcmp(argv[i], COMMAND_MARK) == 0)
                {
                    printf("%s\n", USAGE_MARK);
                }
                else
                {
                    printf("%s: command not found\nUse './%s help' to see available commands.\n",
                           argv[i], NAME);
                }

                exit(EXIT_SUCCESS);
            }
            else if (strcmp(argv[i], COMMAND_ADD) == 0 ||
                     strcmp(argv[i], COMMAND_EDIT) == 0 ||
                     strcmp(argv[i], COMMAND_REMOVE) == 0)
            {
                if (!isnull(cmd.name))
                {
                    update_tasks(&cmd, &task);
                    init_task(&task);
                }

                strcpy(cmd.name, argv[i]);
            }
            else if (strcmp(argv[i], COMMAND_MARK) == 0)
            {
                if (!isnull(cmd.name))
                {
                    update_tasks(&cmd, &task);
                    init_task(&task);
                }

                strcpy(cmd.name, argv[i]);
            }
            else if (isdigit(argv[i][0]))
            {
                if (task.id != NULL_INT)
                {
                    update_tasks(&cmd, &task);
                    int id_task = task.id; // For 'done' command.
                    init_task(&task);

                    if (id_task != 0)
                        task.id = ++id_task;
                }

                task.id = atoi(argv[i]);
            }
            else if (argv[i][0] == '-')
            {
                bool is_option = false;

                for (int j = 0; j < 2; j++)
                {
                    if (strcmp(argv[i], OPTION_DONE[j]) == 0 &&
                        (strcmp(cmd.name, COMMAND_MARK) == 0 ||
                         strcmp(cmd.name, COMMAND_LIST) == 0))
                    {
                        strcpy(cmd.mark_option, MARK_DONE);
                        is_option = true;
                        break;
                    }
                    else if (strcmp(argv[i], OPTION_TODO[j]) == 0 &&
                             (strcmp(cmd.name, COMMAND_MARK) == 0 ||
                              strcmp(cmd.name, COMMAND_LIST) == 0))
                    {
                        strcpy(cmd.mark_option, MARK_TODO);
                        is_option = true;
                        break;
                    }
                    else if (strcmp(argv[i], OPTION_ALL[j]) == 0 &&
                             !strcmp(cmd.name, COMMAND_ADD) == 0)
                    {
                        char yes_no_question = 'n';
                        printf("%s: do you want to perform this operation on all tasks? [y/N] ",
                               cmd.name);
                        scanf("%c", &yes_no_question);
                        if ('y' != tolower(yes_no_question))
                        {
                            printf("Abort.\n");
                            exit(EXIT_SUCCESS);
                        }

                        cmd.is_all = true;
                        is_option = true;
                        break;
                    }
                    else if (isnull(cmd.name) && strcmp(argv[i], OPTION_VERSION[j]) == 0)
                    {
                        printf("%s\n", VERSION);
                        exit(EXIT_SUCCESS);
                    }
                    else if (strcmp(argv[i], OPTION_HELP[j]) == 0)
                    {
                        if (isnull(cmd.name))
                        {
                            strcpy(cmd.name, COMMAND_HELP);
                            is_option = true;
                            break;
                        }
                        else if (strcmp(cmd.name, COMMAND_ADD) == 0)
                        {
                            printf("%s\n", USAGE_ADD);
                            exit(EXIT_SUCCESS);
                        }
                        else if (strcmp(cmd.name, COMMAND_EDIT) == 0)
                        {
                            printf("%s\n", USAGE_EDIT);
                            exit(EXIT_SUCCESS);
                        }
                        else if (strcmp(cmd.name, COMMAND_REMOVE) == 0)
                        {
                            printf("%s\n", USAGE_REMOVE);
                            exit(EXIT_SUCCESS);
                        }
                        else if (strcmp(cmd.name, COMMAND_LIST) == 0)
                        {
                            printf("%s\n", USAGE_LIST);
                            exit(EXIT_SUCCESS);
                        }
                        else if (strcmp(cmd.name, COMMAND_MARK) == 0)
                        {
                            printf("%s\n", USAGE_MARK);
                            exit(EXIT_SUCCESS);
                        }
                    }
                }

                if (!is_option)
                {
                    printf("%s: option not available\nUse './%s help %s' to see available options.\n",
                           argv[i], NAME, !isnull(cmd.name) ? cmd.name : "\b");

                    exit(EXIT_FAILURE);
                }
            }
            else if (strcmp(cmd.name, COMMAND_ADD) == 0)
            {
                if (!isnull(task.title))
                {
                    update_tasks(&cmd, &task);
                    int id_task = task.id;
                    init_task(&task);

                    if (id_task != 0)
                        task.id = ++id_task;
                }

                strcpy(task.title, argv[i]);
            }
            else if (strcmp(cmd.name, COMMAND_EDIT) == 0)
            {
                if (!isnull(task.title))
                {
                    update_tasks(&cmd, &task);
                    init_task(&task);
                }

                strcpy(task.title, argv[i]);
            }
            else if (strcmp(argv[i], COMMAND_LIST) == 0 ||
                     strcmp(argv[i], COMMAND_HELP) == 0)
            {
                strcpy(cmd.name, argv[i]);
            }
            else if (strcmp(argv[i], COMMAND_VERSION) == 0)
            {
                printf("%s\n", VERSION);
                exit(EXIT_SUCCESS);
            }
            else
            {
                printf("%s: command not found\nUse './%s help' to see available commands.\n",
                       argv[i], NAME);
                exit(EXIT_FAILURE);
            }
        }

        if (strcmp(cmd.name, COMMAND_LIST) == 0)
        {
            list_tasks(&cmd);
        }
        else if (strcmp(cmd.name, COMMAND_HELP) == 0)
        {
            printf("%s\n", USAGE);
        }
        else if (!isnull(cmd.name))
        {
            update_tasks(&cmd, &task);
        }
    }
    else
    {
        list_tasks(&cmd);
    }
}

void init()
{
    FILE *todofptr = fopen(FILENAME_TODO, "a");
    FILE *donefptr = fopen(FILENAME_DONE, "a");

    if (todofptr == NULL || donefptr == NULL)
    {
        printf("%s: error while accessing data.\n", TAG_ERROR);
        exit(EXIT_FAILURE);
    }

    fclose(todofptr);
    fclose(donefptr);
}

int main(int argc, char const *argv[])
{
    init();

    handle_args(argc, argv);

    return 0;
}
