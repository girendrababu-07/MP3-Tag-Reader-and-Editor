#ifndef ENCODE_H                 // Prevent multiple inclusion of this header file
#define ENCODE_H                 // Defines ENCODE macro so this file is included only once

#include "types.h"               // Include user-defined data types like Status, uint, etc.

typedef struct _Toview           // Define structure named ToviewInfo for storing MP3 viewing related data
{
    /* mp3 file */
    char mp3_fname[100];         // Character array to store the name/path of the MP3 file
    FILE *fptr_mp3_file;         // File pointer used to access and read the MP3 file
    unsigned int size;           // Variable to store ID3 tag size read from the MP3 header
    uint loop;                   // Loop counter used while parsing frames in the MP3 metadata

} ToviewInfo;                    // Structure alias name used throughout the program



/* Check operation type */
//OperationType check_operation_type(char *argv[]);

/* read and validation with extension */
//Status read_and_validate(char *argv[], ToviewInfo *Toview );

int convert_Big_Endian(char *data);

/* Perform to view */
Status to_view(ToviewInfo *Toview);

/* Get File pointers */
Status open_files(ToviewInfo *Toview);

/* check ID3 tag */
Status check_ID3_tag(ToviewInfo *Toview);

/* check version  */
Status check_version(ToviewInfo *Toview);

/* call function for view */
Status mp3_view(ToviewInfo *Toview);

/* skip header */
Status skip_header(ToviewInfo *Toview);

/* read the tag */
Status read_size(ToviewInfo *Toview);

/* read the contents */
Status Read_contents(ToviewInfo *Toview);


#endif                           // End of header file guard


