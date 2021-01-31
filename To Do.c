#include <stdio.h>
#include <stdlib.h>  // General purpose standard library
#include <stdbool.h> // For 'bool' data type.
#include <string.h>  // For working with strings.

#include "Utils.c" // Contains utility functions.

// Constant for the filename.
#define FILENAME_TASKS "tasks.dat"
#define FILENAME_TASKS_TEMP "tasks.temp"

// Constants for the types of the update operation
#define EDIT 0
#define DELETE 1

// Constants for sizes in characters.
#define LENGTH_TITLE_MAX 60
#define WIDTH_LIST (LENGTH_TITLE_MAX + 4 + 4)

// Function for adding new tasks.
void add_new_task();

// Function for updateing existing tasks.
void update_task(int);

// Function for viewing existing tasks.
void view_tasks();

int main()
{
    // Temporary interface
    {
        // Viewing existing tasks.
        view_tasks();

        printf("\n");

        // Adding a new task.
        add_new_task();

        printf("\n");

        // Viewing existing tasks.
        view_tasks();

        printf("\n");

        // updateting existing task.
        update_task(EDIT);

        printf("\n");

        // Viewing existing tasks.
        view_tasks();

        printf("\n");

        // updateting existing task.
        update_task(DELETE);

        printf("\n");

        // Viewing existing tasks.
        view_tasks();
    }

    system("pause");

    return 0;
}

/**
 * Adds a new task in the data file.
*/
void add_new_task()
{
    // Stores title of the task.
    char title[LENGTH_TITLE_MAX + 1]; // Increment of '1' because the last index of string always contains '\0'.

    // Prompting user to add a task.
    printf("Add new task:\n");

    // Getting title from the user.
    printf("Title: ");
    scanf("%60[^\n]", title);

    // Opening the data file in append mode & storing its pointer.
    FILE *fptr = fopen(FILENAME_TASKS, "a");

    // Checking for any error while opening file.
    if (fptr == NULL)
    {
        // Displaying error.
        printf("Error while accessing data.\n");
    }

    // Saving task data in the file.
    fprintf(fptr, "%s, \n", title);

    // Closing the data file to release allocated memory.
    fclose(fptr);
}

/**
 * Updates i.e. edits or deletes the existing task from the data file.
*/
void update_task(int type)
{
    // Stores number of the task that is to be updated.
    int task_no;

    // Stores whether any task was updated or not.
    bool is_updated = false;

    // Stores current title of the task.
    char current_title[LENGTH_TITLE_MAX + 1];

    // Opening the old data file in read mode &
    // the new data file in write mode & storing their pointers.
    FILE *oldfptr = fopen(FILENAME_TASKS, "r");
    FILE *newfptr = fopen(FILENAME_TASKS_TEMP, "w");

    // Checking for any error while opening files.
    if (oldfptr == NULL || newfptr == NULL)
    {
        // Displaying error.
        printf("Error while accessing data.\n");
    }

    // Checking which type of operation is needed to be performed.
    if (EDIT == type)
    {
        // Prompting user to edit existing task.
        printf("Edit task:\n");

        // Getting task number of the task that is to be edited & storing it.
        printf("Enter the no. of the task you want to edit: ");
        scanf("%d", &task_no);
    }
    else if (DELETE == type)
    {
        // Prompting user to delete existing task.
        printf("Delete task:\n");

        // Getting task number of the task that is to be deleted & storing it.
        printf("Enter the no. of the task you want to delete: ");
        scanf("%d", &task_no);
    }

    // Looping to get data from the file till the end of the file.
    for (int i = 1; fscanf(oldfptr, "%59[^,], ", current_title) != EOF; i++)
    {
        // Checking whether the current task is to be updated or not.
        if (i == task_no)
        {
            // Checking which type of operation is needed to be performed.
            if (EDIT == type)
            {
                // Stores new title of the task.
                char new_title[LENGTH_TITLE_MAX + 1];

                // Getting new title of the task.
                printf("Enter new title: ");
                scanf("%c"); // For ignoring new-line character from previous scanf call.
                scanf("%60[^\n]", new_title);

                // Saving new task data in the file.
                fprintf(newfptr, "%s, \n", new_title);
            }

            // Storing that the task was updated.
            is_updated = true;
        }
        else
        {
            // Saving current task data in new file.
            fprintf(newfptr, "%s, \n", current_title);
        }
    }

    // Closing the data files to release allocated memory.
    fclose(oldfptr);
    fclose(newfptr);

    // Checking whether the task was updated or not.
    if (!is_updated)
    {
        // Displaying error msg that no task was found.
        printf("Sorry! No task found.\n");
    }

    // Applying changes.
    remove(FILENAME_TASKS);
    rename(FILENAME_TASKS_TEMP, FILENAME_TASKS);
}

/**
 * Views existing tasks from the data file.
*/
void view_tasks()
{
    // Clearing screen.
    system("cls || clear");

    // Stores title of the task.
    char title[LENGTH_TITLE_MAX + 1];

    // Opening the data file in read mode & storing its pointer.
    FILE *fptr = fopen(FILENAME_TASKS, "r");

    // Checking for any error while opening file.
    if (fptr == NULL)
    {
        // Displaying error.
        printf("Error while accessing data.\n");
    }

    // Prompting user to view existing tasks.
    printf("Current tasks:\n");

    // Displaying top horizontal line of the list.
    printcharln(0, WIDTH_LIST, '_', ' ');

    // Iterating throught tasks in file.
    for (int task_no = 1; fscanf(fptr, "%[^,], ", title) != EOF; task_no++)
    {
        // Stores length of the title.
        int length_title = strlen(title);

        // Ignore task when title length is greater than equal to the max title length possible.
        if (LENGTH_TITLE_MAX < length_title)
        {
            continue;
        }

        // Displaying empty line for some space between tasks.
        printcharln(0, WIDTH_LIST, ' ', '|');

        // Displaying task no. and the task title.
        printf("| #%02d %s", task_no, title);
        printcharln(6 + length_title, WIDTH_LIST, ' ', '|');

        // Displaying bottom horizontal line of the list.
        printcharln(0, WIDTH_LIST, '_', '|');
    }

    // Closing the data file to release allocated memory.
    fclose(fptr);
}
