#include <stdio.h>

// Constant for the filename.
#define FILENAME_TASKS "tasks.dat"

// Function for adding new tasks.
void add_new_task();

// Function for viewing existing tasks.
void view_tasks();

int main()
{
    // Adding a new task.
    add_new_task();

    // Viewing existing tasks.
    view_tasks();

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

    // Checking for any error during opening file.
    if (fptr == NULL)
    {
        // Displaying error.
        printf("Error while accessing data.");
    }

    // Saving task data in the file.
    fprintf(fptr, "%s, \n", title);

    // Closing the data file to release allocated memory.
    fclose(fptr);
}

/**
 * Views existing tasks in the data file.
*/
void view_tasks()
{
    // Stores title of the task.
    char title[60];

    // Opening the data file in read mode & storing its pointer.
    FILE *fptr = fopen(FILENAME_TASKS, "r");

    // Checking for any error during opening file.
    if (fptr == NULL)
    {
        // Displaying error.
        printf("Error while accessing data.");
    }

    // Prompting user to view existing tasks.
    printf("\nCurrent tasks:\n");

    // Looping to get data from the file till the end of the file.
    while (fscanf(fptr, "%59[^,], ", title) != EOF)
    {
        // Displying tasks to the user.
        printf("%s\n", title);
    }

    // Closing the data file to release allocated memory.
    fclose(fptr);
}
