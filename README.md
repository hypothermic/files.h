# files.h

Java-like virtual file objects in C.  
Low-footprint and efficient: just one header file.  
Made for simplicity.

## Features

- Manipulate file and directory paths.
- Read/write to files.
- Create new files/directories.
- Check if files/directories and get their attributes.

## Example

```c
#include "files.h"

int main(int argc, char **argv)
{
    // --- File Tools:
    File *file = files_new("/etc/crontab");
    printf("Name: %s\n", files_get_name(file));
    printf("Path: %s\n", files_get_path(file));
    printf("URI: %s\n", files_get_uri(file));
    printf("Parent: %s\n", files_get_parent_path(file));
    printf("Contents: %s\n", files_contents_read(file));
    printf("Size: %ld\n", files_get_size(file));

    // --- Directory Tools:
    File *dir = files_new("./testdirectory/");

    if (!files_is_directory(dir))
    {
        files_mkdir(dir);
        // or
        files_mkdirp(dir, 0755);
    }

    // --- Write Tools:
    File *sample = files_new("./hello.txt");
    files_contents_write(sample, "Hello!");
    files_contents_append(sample, "Append me!");
    files_contents_prepend(sample, "Prepend me!");
}
```

More examples are in the `tests` folder.

## Versioning

This library is aimed to always be backwards-compatible.
The function signatures will never be changed after the
function has been added to the library.

The predefined `LIBFILESHT_VERSION` integer in `files.h`
will be bumped every git commit which includes changes to
`files.h`