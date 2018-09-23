# files.h

Java-like virtual file objects in C.  
Low-footprint and efficient.

## Features

- Manipulate file paths.
- Write to files.
- Read files.

## Example

```c
#include "files.h"

int main(int argc, char **argv)
{
    // --- File Tools:
    File *file = files_new("/etc/crontab");
    printf("Name: %s\n", files_get_name(file));
    printf("Path: %s\n", files_get_path(file));
    printf("Parent: %s\n", files_get_parent_path(file));
    printf("Contents: %s\n", files_contents_read(file));

    // --- Write Tools:
    File *sample = files_new("./hello.txt");
    files_contents_write(sample, "Hello!");
    files_contents_append(sample, "Append me!");
    files_contents_prepend(sample, "Prepend me!");
}
```

More examples are in the `tests` folder.