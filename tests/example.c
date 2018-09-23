/*
 * -----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <admin@hypothermic.nl> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * -----------------------------------------------------------------------------
 */

#include "../files.h"

int main()
{
    // --- File Tools:                                    // ------ EXAMPLE OUTPUTS ------ //
    File *file = files_new("./example.c");                // 
    printf("Name: %s\n", files_get_name(file));           // Name: example.c
    printf("Path: %s\n", files_get_path(file));           // Path: /home/hypothermic/c/files.h/tests/example.c
    printf("Parent: %s\n", files_get_parent_path(file));  // Parent: /home/hypothermic/c/files.h/tests
    printf("Contents: %s\n", files_contents_read(file));  // Contents: *prints the contents*

    // --- Write Tools:                                   // -------- INFORMATION -------- //
    File *sample = files_new("./hello.txt");              // Creates a new file object to the relative file hello.txt
    files_contents_write(sample, "Hello!");               // Creates the file (or overrides if it exists already) and writes "Hello!"
    files_contents_append(sample, "Append me!");          // Appends a string to the end of the file.
    files_contents_prepend(sample, "Prepend me!");        // Prepends a string to the beginning of the file
}