#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "edit.h"

/* Same conversion logic used in view.c */
int convert_Big_Endian_edit(char *data)
{
    for (int i = 0; i < 2; i++)                     // Loop runs 2 times to swap first byte with last and second byte with third (total 4 bytes reversal)
    {
        char temp = data[i];                        // Store current byte temporarily
        data[i] = data[4 - i - 1];                  // Replace current byte into opposit positions
        data[4 - i - 1] = temp;                     // Place stored byte into opposite position 
    }

    int *ptr = (int *)data;                         // Typecast byte array to integer pointer so we can read it as a 32-bit integer
    return (*ptr);                                  // Return the converted integer value
}

Status to_edit(ToeditInfo *Toedit, char *option, char *value)
{
    if(open_files_for_edit(Toedit) == failure)      // Open MP3 file in read/write mode so we can modify tags
        return failure;

    if(check_ID3_tag_for_edit(Toedit) == failure)   // Verify file contains ID3 tag
        return failure;

    if(mp3_edit(Toedit, option, value) == failure)  // Call main function for edits the tag
        return failure;

    fclose(Toedit->fptr_edit_mp3_file);             // Close the MP3 file after editing is complete

    return success;                                 // Return success to caller
}

Status open_files_for_edit(ToeditInfo *Toedit)
{
    Toedit->fptr_edit_mp3_file = fopen(Toedit->edit_mp3_fname, "rb+");  
    // Open MP3 file in read + write binary mode
    // "rb+" means existing file will be opened for both reading and writing without deleting content

    if(Toedit->fptr_edit_mp3_file == NULL)          // If file pointer is NULL → file opening failed
    {
        printf("Unable to open file\n");            // Print error message
        return failure;
    }

    return success;                                 // File opened successfully
}

Status check_ID3_tag_for_edit(ToeditInfo *Toedit)
{
    char tag[3];                                    // Buffer to store first 3 bytes of file

    fseek(Toedit->fptr_edit_mp3_file, 0, SEEK_SET); // Move file pointer to beginning of file
    fread(tag, 1, 3, Toedit->fptr_edit_mp3_file);   // Read first 3 bytes of MP3 file

    if(strncmp(tag,"ID3",3) == 0)                   // Compare first 3 bytes with string "ID3"
        return success;                             // If equal → file contains ID3v2 tag

    printf("ID3 tag not found\n");                  // If not matched → file does not contain ID3 tag
    return failure;
}

Status mp3_edit(ToeditInfo *Toedit, char *option, char *value)
{
    char frame_id[5];                               // Buffer to store frame identifier (example TIT2, TPE1 etc)

    if(strcmp(option,"-t")==0)
        strcpy(frame_id,"TIT2");                    // If user selects title → frame ID = TIT2
    else if(strcmp(option,"-a")==0)
        strcpy(frame_id,"TPE1");                    // Artist frame
    else if(strcmp(option,"-A")==0)
        strcpy(frame_id,"TALB");                    // Album frame
    else if(strcmp(option,"-y")==0)
        strcpy(frame_id,"TYER");                    // Year frame
    else if(strcmp(option,"-g")==0)
        strcpy(frame_id,"TCON");                    // Genre frame
    else if(strcmp(option,"-c")==0)
        strcpy(frame_id,"COMM");                    // Comment frame
    else
    {
        printf("Invalid option\n");                 // If option is invalid
        return failure;
    }

    char header[10];                                                  // Buffer to store ID3 header (first 10 bytes)

    fseek(Toedit->fptr_edit_mp3_file,0,SEEK_SET);                     // Move file pointer to beginning of file
    fread(header,1,10,Toedit->fptr_edit_mp3_file);                    // Read first 10 bytes which contain ID3 tag header

    unsigned int bytes_read = 0;                                      // Variable used to track how many bytes of tag we have processed
    unsigned int tag_size;                                            // Total size of ID3 tag

    char size_arr[4];
    memcpy(size_arr, header+6,4);                                     // Copy 4 bytes from header (byte 6-9 contain tag size)

    tag_size = convert_Big_Endian_edit(size_arr);                     // Convert big-endian tag size to system integer

    while(bytes_read < tag_size)                                      // Loop through all frames inside ID3 tag
    {
        char id[5]={0};                                               // Buffer to store frame ID
        char frame_size_arr[4];                                       // Buffer to store frame size bytes
        unsigned int frame_size;                                      // Variable to hold actual frame size

        fread(id,1,4,Toedit->fptr_edit_mp3_file);                     // Read frame ID (example TIT2, TPE1 etc)
        id[4] = '\0';                                                 // Add null terminator so it becomes valid string

        fread(frame_size_arr,1,4,Toedit->fptr_edit_mp3_file);         // Read frame size (4 bytes)

        frame_size = convert_Big_Endian_edit(frame_size_arr);         // Convert frame size from big-endian to integer

        fseek(Toedit->fptr_edit_mp3_file,2,SEEK_CUR);                 // Skip frame flags (2 bytes not needed)

        char data[1000];                                              // Buffer to store frame data

        if(frame_size < sizeof(data))                                 // Safety check to avoid buffer overflow
        {
            fread(data,1,frame_size,Toedit->fptr_edit_mp3_file);      // Read frame data into buffer

            if(strcmp(id,frame_id)==0)                                // Check whether this frame matches user selected tag
            {
               /* move file pointer to beginning of frame data */
                fseek(Toedit->fptr_edit_mp3_file, -(long)frame_size, SEEK_CUR);          // Move pointer backward to start of frame data so we can overwrite it

                data[0] = 0;                                                             // First byte defines text encoding (0 = ISO-8859-1)

                if(strcmp(frame_id,"COMM")==0)                                           // Special handling for comment frame
                {
                    memcpy(data+1,"eng",3);                                              // Set language code "eng"
                    strncpy(data+4,value,frame_size-4);                                  // Copy new comment value
                }
                else
                {
                    strncpy(data+1,value,frame_size-1);                                  // Copy new tag value (skip encoding byte)
                }

                fwrite(data,1,frame_size,Toedit->fptr_edit_mp3_file);                    // Write modified data back to the same frame location
                
                fflush(Toedit->fptr_edit_mp3_file);                                      // Force buffered data to be written immediately to file

                printf("\nTag updated successfully\n");                                  // Print success message
                return success;                                                          // Exit after updating required frame
            }
        }
        else
        {
            fseek(Toedit->fptr_edit_mp3_file,frame_size,SEEK_CUR);                       // Skip frames larger than buffer
        }

        bytes_read = bytes_read + 10 + frame_size;                                       // Update processed bytes (10 bytes header + frame data)
    }

    printf("Frame not found\n");                                                         // If requested frame does not exist
    return failure;
}
