#include <stdio.h>
#include <stdbool.h> // For 'bool' data type.

// Constant for the filename.
#define FILENAME_TASKS "tasks.dat"
#define FILENAME_TASKS_TEMP "tasks.temp"

// Function for adding new tasks.
void add_new_task();

// Function for editing existing tasks.
void edit_task();

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

        // Editting existing task.
        edit_task();

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
    char title[60];

    // Prompting user to add a task.
    printf("Add new task:\n");

    // Getting title from the user.
    printf("Title: ");
    scanf("%59[^\n]", title);

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
 * Edits existing task from the data file.
*/
void edit_task()
{
    // Stores number of the task that is to be editted.
    int task_no;

    // Stores whether any task was edited or not.
    bool is_edited = false;

    // Store old/new titles of the task.
    char old_title[60];
    char new_title[60];

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

    // Prompting user to edit existing task.
    printf("Edit task:\n");

    // Getting task number of the task that is to be editted & storing it.
    printf("Enter the no. of the task you want to edit: ");
    scanf("%d", &task_no);

    // Looping to get data from the file till the end of the file.
    for (int i = 1; fscanf(oldfptr, "%59[^,], ", old_title) != EOF; i++)
    {
        // Checking if the current task is to be edited or not.
        if (i == task_no)
        {
            // Getting new title of the task.
            printf("Enter new title: ");
            scanf("%c"); // For ignoring new-line character from previous scanf call.
            scanf("%59[^\n]", new_title);

            // Saving new task data in the file.
            fprintf(newfptr, "%s, \n", new_title);

            // Storing that task was edited.
            is_edited = true;
        }
        else
        {
            // Saving old task data in the file.
            fprintf(newfptr, "%s, \n", old_title);
        }
    }

    // Closing the data files to release allocated memory.
    fclose(oldfptr);
    fclose(newfptr);

    // Checking whether changes are needed to be applied or not.
    if (is_edited)
    {
        // Applying changes.
        remove(FILENAME_TASKS);
        rename(FILENAME_TASKS_TEMP, FILENAME_TASKS);
    }
    else
    {
        // Removeing temp file.
        remove(FILENAME_TASKS_TEMP);

        // Displaying error msg that no task was found.
        printf("Sorry! No task found.\n");
    }
}

/**
 * Views existing tasks from the data file.
*/
void view_tasks()
{
    // Stores title of the task.
    char title[60];

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

    // Looping to get data from the file till the end of the file.
    while (fscanf(fptr, "%59[^,], ", title) != EOF)
    {
        // Displying tasks to the user.
        printf("%s\n", title);
    }

    // Closing the data file to release allocated memory.
    fclose(fptr);
}
