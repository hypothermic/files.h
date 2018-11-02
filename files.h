/*
 * -----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <admin@hypothermic.nl> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * -----------------------------------------------------------------------------
 */

#ifndef LIBFILESHT_H_
#define LIBFILESHT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

// TODO: get rid of <linux/**> imports
#include <linux/limits.h>

#define LIBFILESHT_VERSION ((int) 1)

// --- File object --- //

typedef enum { FALSE, TRUE } boolean;

typedef struct _File
{
    const char* path;
} File;

File *files_new(const char* path)
{
    File *f = (File*)malloc(sizeof(File));
    f->path = path;
    return f;
}

void files_free(File *f)
{
    free(f);
}

// --- Functions --- //

/*! Get the seperator character for
 *  your current platform.
 */
char files_platform_get_seperator()
{

#if defined WIN32 || defined _WIN32 || defined __CYGWIN__
    return '\\';
#else
    return '/';
#endif
}

/*! Does the file exist?
 *
 *  Returns 0 if not exist.
 *  Returns 1 if exist.
 */
boolean files_is_exist(File *f)
{
    if (access(f->path, F_OK) != -1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*! Is the file a directory?
 *
 *  Returns 0 if not exist.
 *  Returns 1 if exist.
 */
boolean files_is_directory(File *f)
{
    boolean result = FALSE;

    DIR* dir = opendir(f->path);

    if (dir)
    {
        result = TRUE;
        closedir(dir);
    }

    return result;
}

/*! Get the name of the file.
 */
char *files_get_name(File *f)
{
    char *buf;
    buf = strdup(f->path);
    return basename(buf);
}

/*! Get the absolute path of the file.
 *  Warning: free the returned char*!
 */
char *files_get_path(File *f)
{
    char *buf = (char *) malloc(PATH_MAX);
    realpath(f->path, buf);
    return buf;
}

/*! Get the absolute path of the parent folder.
 */
File *files_get_parent_file(File *f)
{
    char *buf;
    buf = strdup(files_get_path(f));
    return files_new(dirname(buf));
}

/*! Get the absolute path of the parent folder.
 */
char *files_get_parent_path(File *f)
{
    char *buf;
    buf = strdup(files_get_path(f));
    return dirname(buf);
}

/*! Get the uniform resource identifier (URI) of the file.
 */
char *files_get_uri(File *f)
{
    char* buf = files_get_path(f);

    while (buf[0] == '/')
    {
        //memmove(buf, buf+1, strlen (buf+1) + 1);
        buf++;
    }

    const char* prefix = "file:///";

    char* buf2;

    if (buf2)
    {
        strcpy(buf2, prefix);
        strcat(buf2, buf);
        return buf2;
    }

    return buf;
}

/*! On which partition (device) is the file located?
 *
 *  Returns the Device ID if succeeded
 *  Returns -1 if error.
 */
int files_get_partition_id(File *f)
{
    struct stat buf;

    if (lstat(files_get_path(f), &buf) == 0)
    {
        return buf.st_dev;
    }

    return -1;
}

/*! Is the file a directory?
 *
 *  Returns the Device ID if succeeded
 *  Returns empty char* if error.
 */
/*TODO//char *files_get_partition_name(File *f)
{
    int id = files_get_partition_id(f);

    if (id == -1)
    {
        return "";
    }

    #if defined ( BSD )
        return devname(id);
    #else
        #include <blkid/blkid.h>
        return blkid_devno_to_devname(id);
    #endif

    return -1;
}*/

/*! Get the size of the file
 */
long files_get_size(File *f)
{
    long buf = 0;
    FILE *handler = fopen(files_get_path(f), "r");

    if (handler)
    {
        fseek(handler, 0, SEEK_END);
        buf = ftell(handler);
        fclose(handler);
    }

    return buf;
}

/*! Read the contents of a file to a string.
 */
// source: https://stackoverflow.com/a/3464656/9107324
char *files_contents_read(File *f)
{
    char *buffer = NULL;
    int string_size, read_size;
    FILE *handler = fopen(files_get_path(f), "r");

    if (handler)
    {
       fseek(handler, 0, SEEK_END);
       string_size = ftell(handler);
       rewind(handler);

       buffer = (char*) malloc(sizeof(char) * (string_size + 1) );
       read_size = fread(buffer, sizeof(char), string_size, handler);
       buffer[string_size] = '\0';

       if (string_size != read_size)
       {
           free(buffer);
           buffer = NULL;
       }
       fclose(handler);
    }
    return buffer;
}

//#pragma GCC diagnostic ignored "-Wformat-security"

/*! Write a string to a file.
 *  
 *  A file will be created if it doesn't exist already.
 *  Warning: if file exists already, it will be overwritten!
 */
void files_contents_write(File *f, const char* data)
{
    FILE *ptr;

	ptr = fopen(files_get_path(f), "w");
 
	fprintf(ptr, data);
	
	fclose(ptr);
}

/*! Append a string to the end of the file.
 */
void files_contents_append(File *f, const char* data)
{
    FILE *ptr;
	ptr = fopen(files_get_path(f), "a");
 
	fprintf(ptr, data);
	
	fclose(ptr);
}

/*! Prepend a string to the beginning of the file.
 */
// Note to self: optimize this (or replace with better method)
void files_contents_prepend(File *f, const char* data)
{
    FILE *ptr;

    char *old = files_contents_read(f);

	ptr = fopen(files_get_path(f), "w");
 
    fprintf(ptr, data);
    fprintf(ptr, old);
	
	fclose(ptr);
}

//#pragma GCC diagnostic pop

/*! Make a directory at specified file path
 *  
 *  Argument 2 is to specify the permissions
 *  - for example: 0777
 *
 *  Returns 0 if not successful.
 *  Returns 1 if successful.
 */
boolean files_mkdirp(File *f, int permissions)
{
    boolean result = FALSE;

    if (!files_is_directory(f))
    {
        mkdir(files_get_path(f), 0755);
        result = TRUE;
    }

    return result;
}

/*! Make a directory at specified file path
 *
 *  This function uses the default permission 0755
 *
 *  Returns 0 if not successful.
 *  Returns 1 if successful.
 */
boolean files_mkdir(File *f)
{
    return files_mkdirp(f, 0755);
}

/*! Rename the specified file.
 *
 *  Returns 0 if not successful.
 *  Returns 1 if successful.
 */
boolean files_move(File *src, File* dest)
{
    boolean result = FALSE;

    if (files_is_exist(src))
    {
        if (rename(files_get_path(src), files_get_path(dest)))
        {
            result = TRUE;
        }
    }

    return result;
}

int files_lib_version()
{
    return LIBFILESHT_VERSION;
}

#endif // LIBFILESHT_H_
