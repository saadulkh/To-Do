/**
 * This file contains utility functions.
*/

#include <stdio.h>
#include <string.h>

#define NULL_INT -1
#define NULL_STR "null"

#define isnull(str) strcmp(str, NULL_STR) == 0

// #define is_smd_type_null(type) strcmp(str, NULL_STR) == 0

/**
 * Writes a line with a character to stdout till the terminal position . 
 * Moves to new line at the end.
 * 
 * @param current_pos Current position on line.
 * @param terminal_pos Terminal position of line.
 * @param character Character that is to be written between first and last position of line.
 * @param extreme_char Character that is to be written at first and last position of line.
*/
void printcharln(int current_pos, int terminal_pos, char character, char extreme_char)
{
    for (current_pos++; current_pos <= terminal_pos + 1; current_pos++)
    {
        printf("%c", current_pos == terminal_pos + 1
                         ? '\n'
                     : (current_pos == 1 || current_pos == terminal_pos)
                         ? extreme_char
                         : character);
    }
}

// char *command_sign(char *command)
// {
//     return strcmp(command, COMMAND_ADD) == 0
//                ? "+"
//            : strcmp(command, COMMAND_EDIT) == 0
//                ? "+/-"
//            : strcmp(command, COMMAND_REMOVE) == 0
//                ? "-"
//                : "~";
// }
