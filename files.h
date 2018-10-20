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

#include <linux/limits.h>

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

char *files_get_uri(File *f)
{
    char* buf = files_get_path(f);

    if (buf[0] == '/')
    {
        buf++;
    }

    const char* prefix = "file:///";
    const unsigned int RESULT_SIZE = sizeof(buf) + sizeof(prefix) + 2 * sizeof('\0');

    char* buf2 = malloc(RESULT_SIZE);

    if (buf2)
    {
        strcpy(buf2, prefix);
        strcat(buf2, buf);
        return buf2;
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

#pragma GCC diagnostic ignored "-Wformat-security"

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

char files_platform_get_seperator()
{
//#ifdef _WIN32
#if defined WIN32 || defined _WIN32 || defined __CYGWIN__
    return '\\';
#else
    return '/';
#endif
}

#pragma GCC diagnostic pop

#endif // LIBFILESHT_H_
