#ifndef ENCODE                // Prevents multiple inclusion of this header file
#define ENCODE                // Defines ENCODE macro so this file is included only once

#include "types.h"            // Includes user-defined types like Status, uint, etc.

typedef struct _Toedit        // Define structure to store MP3 edit related information
{
    /* mp3 file */
    char edit_mp3_fname[100]; // Stores the name of the MP3 file that user wants to edit
    FILE *fptr_edit_mp3_file; // File pointer used to open and modify the original MP3 file
    unsigned int e_size;      // Stores size of the ID3 tag or frame during editing process
    char tag[5];              // Stores frame identifier like TIT2, TPE1, TALB etc
    int flag;                 // General flag variable used for control/condition handling

    /* temp mp3 file */
    char temp_mp3_fname[100]; // Stores temporary MP3 file name used while rewriting edited data
    FILE *fptr_temp_mp3_file; // File pointer used to handle temporary MP3 file operations
    uint argv_size;           // Stores number of command line arguments passed to the program

} ToeditInfo;                 // Structure alias used to access editing related data easily




/* read and validation with extension_for_edit*/

//Status Read_and_Validation_for_edit(char *argv[], ToeditInfo *Toedit );

int convert_Big_Endian_edit(char *data);

/* Perform to edit*/
Status to_edit(ToeditInfo *Toedit, char *option, char *value);

/* Get File pointers for edit */
Status open_files_for_edit(ToeditInfo *Toedit);


/* check ID3 tag for edit */
Status check_ID3_tag_for_edit(ToeditInfo *Toedit);

/* check version for edit */
Status check_version_for_edit(ToeditInfo *Toedit);

/* call function for edit */
Status mp3_edit(ToeditInfo *Toedit, char *option, char *value);

/* skip header for edit */
Status skip_header_for_edit(ToeditInfo *Toedit);

/* read tag for edit */
Status read_tag_for_edit(ToeditInfo *Toedit);

/* read the size */
Status read_size_for_edit(ToeditInfo *Toedit,char *argv[]);

/* read and replace */
Status read_and_replace(ToeditInfo *Toedit,char *argv[]);

/* copy data */
Status copy_data(ToeditInfo *Toedit,char *argv[]);

/*copy remain */
Status copy_remaining_data(ToeditInfo *Toedit);

/* read size for edit without modify */
Status read_size_for_edit_without_modify(ToeditInfo *Toedit);

/* read and replace without modify */
Status read_and_replace_without_modify(ToeditInfo *Toedit);

/* set flag */
Status set_flag(ToeditInfo *Toedit,char *argv[]);

Status copy_temp_to_org(ToeditInfo *Toedit);


int year_validation(char *argv);
	

#endif                        // Ends header guard to avoid multiple inclusion

