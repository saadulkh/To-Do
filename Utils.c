/**
 * This file contains utility functions.
*/

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
    for (int pos = 1 + current_pos; pos <= terminal_pos + 1; pos++)
    {
        printf("%c", pos == terminal_pos + 1
                         ? '\n'
                         : (pos == 1 || pos == terminal_pos)
                               ? extreme_char
                               : character);
    }
}
