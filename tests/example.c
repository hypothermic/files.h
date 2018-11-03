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
    // --- File Tools:                                         // ------ EXAMPLE OUTPUTS ------ //
    File *file = files_new("./example.c");                     // 
    printf("Exists?: %d\n", files_is_exist(file));             // Exists?: 1
    printf("Is directory?: %d\n", files_is_directory(file));   // Is directory?: 0
    printf("Name: %s\n", files_get_name(file));                // Name: example.c
    printf("Path: %s\n", files_get_path(file));                // Path: /home/hypothermic/c/files.h/tests/example.c
    printf("URI: %s\n", files_get_uri(file));                  // URI: file:///home/hypothermic/c/files.h/tests/example.c
    printf("Parent path: %s\n", files_get_parent_path(file));  // Parent path: /home/hypothermic/c/files.h/tests
    printf("Contents: %s\n", files_contents_read(file));       // Contents: *prints the contents*
    printf("Size: %ld\n", files_get_size(file));               // Get file size as long: 1854

    // --- Write Tools:                                        // -------- INFORMATION -------- //
    File *sample = files_new("./hello.txt");                   // Creates a new file object to the relative file hello.txt
    files_contents_write(sample, "Hello!");                    // Creates the file (or overrides if it exists already) and writes "Hello!"
    files_contents_append(sample, "Append me!");               // Appends a string to the end of the file.
    files_contents_prepend(sample, "Prepend me!");             // Prepends a string to the beginning of the file

    // --- Create tools:
    File *directory = files_new("./directory/");
    if (!files_is_directory(directory))                        // if the file doesn't exist and isn't a directory
    {
        files_mkdir(directory);                                // create the directory with default permissions (0755)
        // or
        files_mkdirp(directory, 0777);                         // create the directory with specefied permissions
    }
    
    // --- Move a file:
    File *source = files_new("./wrongname.txt");
    if (!files_is_exist(source))
    {
        files_contents_write(source, "sample contents");       // write to file if it doesn't exist yet
    }
    File *destination = files_new("./correctname.txt");        // create a virtual file to the desired destination
    printf("Device id=%d name=%s\n", files_get_partition_id(source), files_get_partition_name(source));
    printf("Move: %d\n", files_move(source, destination));     // execute the move from source to destination

}