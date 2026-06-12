#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "view.h"

int convert_Big_Endian(char *data)
{
    for (int i = 0; i < 2; i++)       // Swap first and last bytes to convert Big Endian byte order into Little Endian format
    {
        char temp = data[i];          // Temporarily store the current byte
        data[i] = data[4 - i - 1];    // Move the corresponding last byte to the current position
        data[4 - i - 1] = temp;       // Place the stored byte at the end position
    }

    int *ptr = (int *)data;           // Typecast the 4-byte char array to an integer pointer so it can be interpreted as a 32-bit integer

    return (*ptr);                    // Return the converted integer value from the byte array
}

Status to_view(ToviewInfo *Toview)                
{
    if (open_files(Toview) == failure)      // Call function to open the MP3 file in binary read mode
        return failure;

    if (check_ID3_tag(Toview) == failure)   // Check whether the MP3 file contains an ID3 tag (ID3v1 or ID3v2)
        return failure;

    check_version(Toview);                  // Identify which ID3 version (v1 or v2) is present in the file

    read_size(Toview);                      // Read the ID3 tag size from the header to know how much metadata to process

    mp3_view(Toview);                       // Extract metadata frames (Title, Artist, Album, etc.) and display them

    fclose(Toview->fptr_mp3_file);          // Close the opened MP3 file after reading metadata

    return success;
}

Status open_files(ToviewInfo *Toview)
{
    Toview->fptr_mp3_file = fopen(Toview->mp3_fname, "rb");     // Open the MP3 file in read binary mode and store the file pointer inside the structure

    if (Toview->fptr_mp3_file == NULL)                          // Check if file opening failed
    {
        printf("Unable to open file\n");                        // Display error if file cannot be opened
        return failure;
    }

    return success;                                             // Return success if file opened correctly
}

Status check_ID3_tag(ToviewInfo *Toview)
{
    char Header_Id[3];                                      // Buffer to store tag identifier

    fseek(Toview->fptr_mp3_file, -128, SEEK_END);           // Move file pointer 128 bytes before end of file (used to check ID3v1 tag which is stored in last 128 bytes)
    fread(Header_Id, 1, 3, Toview->fptr_mp3_file);          // Read 3 elements from the file, where each element is 1 byte in size, and store them into the array 'tag' from the file pointer, if it contains "TAG" (ID3v1 identifier)

    printf("========================================================\n");

    if (strncmp(Header_Id, "TAG", 3) == 0)                  // Compare read bytes with "TAG" identifier
    {
        printf("ID3v1 tag found ");                         // ID3v1 metadata exists at the end of file
        return success;
    }    

    fseek(Toview->fptr_mp3_file, 0, SEEK_SET);              // Move file pointer back to beginning of file (start of MP3 file)
    fread(Header_Id, 1, 3, Toview->fptr_mp3_file);          // Read first 3 bytes from beginning to check for "ID3" (ID3v2 identifier)

    if (strncmp(Header_Id, "ID3", 3) == 0)                  // Compare with "ID3" which indicates ID3v2 tag
    {   
        printf("ID3v2 tag found | ");                       // Display that ID3v2 metadata exists
        return success;
    }

    printf("No ID3 tag found\n");                           // If neither ID3v1 nor ID3v2 exists
    return failure;
}

Status check_version(ToviewInfo *Toview)
{
    unsigned char version[2];                                // Array to read version identifier

    fseek(Toview->fptr_mp3_file, 0, SEEK_SET);               // Move file pointer back to beginning of file (start of MP3 file)
    fread(version, 1, 2, Toview->fptr_mp3_file);             // Read first 2 bytes from beginning to check for "ID3" (ID3v2 identifier)

    if (strncmp(version, "ID3", 2) == 0)                     // Compare first 3 bytes with string "ID3"; if matched, file contains ID3v2 tag → return success
        printf("ID3 Version : ID3v2 | ");                    // Print that file uses ID3v2
    else
        printf("ID3 Version : ID3v1 | ");                    // Otherwise assume ID3v1 metadata

    return success;
}

Status read_size(ToviewInfo *Toview)
{
    char size_arr[4];                                        // Array to store 4 bytes representing tag size

    fseek(Toview->fptr_mp3_file, 1, SEEK_CUR);               // Skip flag byte in ID3 header
    fread(size_arr, 1, 4, Toview->fptr_mp3_file);            // Read next 4 bytes containing tag size

    Toview->size = convert_Big_Endian(size_arr);             // Convert Big Endian size into integer format

    return success;
}

Status mp3_view(ToviewInfo *Toview)
{
    char header[10];

    fseek(Toview->fptr_mp3_file, 0, SEEK_SET);               // Again move file pointer to beginning to properly read full ID3v2 header
    fread(header, 1, 10, Toview->fptr_mp3_file);             // Read first 10 bytes (complete ID3v2 header: ID3 + version + flags + size)

    if(strncmp(header, "ID3", 3) != 0)                       // Verify ID3v2 identifier
    {
        printf("No ID3v2 tag found\n");                      // If header does not start with ID3
        return failure;
    }

    printf("ID3v2.%d detected\n", header[3]);                // Print version number (2.3, 2.4 etc.)
    printf("========================================================\n");

    char frame_id[5] = {0};                                  // Create 5-byte array for frame ID (4 characters + null)
    unsigned int frame_size;                                 // Variable to store Frame size
    char frame_size_arr[4];
    char data[1000];                                         // Buffer to store frame data
    unsigned int bytes_read = 0;

    while(bytes_read < Toview->size)                         // Loop through all frames within ID3 tag size
    {
        fread(frame_id, 1, 4, Toview->fptr_mp3_file);        // Read 4 bytes from file to get frame ID (example: "TIT2", "TPE1", "COMM")
        frame_id[4] = '\0';                                  // Null terminate frame ID string
        
        fread(frame_size_arr, 1, 4, Toview->fptr_mp3_file);  // Read next 4 bytes to get frame size (length of frame data; in ID3v2.3 stored as normal integer)
        frame_size = convert_Big_Endian(frame_size_arr);     // Convert big-endian to little-endian
                                                             // Convert the 4-byte integer 'size' from big-endian format (used in ID3v2.3 files)
                                                             // to little-endian format (used by most PCs like x86 architecture) by swapping byte order

        fseek(Toview->fptr_mp3_file, 2, SEEK_CUR);           // Skip 2 bytes of frame flags

        if (frame_size > 0 && frame_size < sizeof(data))     // Ensure frame size is valid and fits buffer
        {
           fread(data, 1, frame_size, Toview->fptr_mp3_file);   // Read frame data
           data[frame_size] = '\0';                              // Add null terminator to treat as string

           if(strcmp(frame_id, "TIT2") == 0)                     // Frame ID for Title
           {
               char *title = data + 1;                           // Skip encoding byte
               char *dash = strchr(title, '-');                  // Search '-' symbol

               if(dash != NULL)
               *dash = '\0';                                     // Remove text after '-'

               printf("Title    : %s\n", title);                 // Display title
           }

           else if(strcmp(frame_id, "TPE1") == 0)                // Frame ID for Artist
           {
               char *artist = data + 1;                          // Skip encoding byte
               char *dash2 = strchr(artist, '-');                // Find '-'

               if(dash2 != NULL)
               *dash2 = '\0';                                    // Remove extra text

               printf("Artist   : %s\n", artist);                // Display artist name
           }

           else if(strcmp(frame_id, "TALB") == 0)
               printf("Album    : %s\n", data+1);                // Album name (skip encoding byte)

           else if(strcmp(frame_id, "TYER") == 0)
               printf("Year     : %s\n", data+1);                // Year metadata

           else if(strcmp(frame_id, "TCON") == 0)
               printf("Genre    : %s\n", data+1);                // Genre metadata

           else if(strcmp(frame_id, "COMM") == 0)
               printf("Comment  : %s\n", data+4);                // Skip encoding + language bytes
        }
        else
        {
            fseek(Toview->fptr_mp3_file, frame_size, SEEK_CUR); // Skip unknown or very large frames
        }

        bytes_read =  bytes_read + 10 + frame_size;             // Update processed bytes count (10 bytes header + data)
    }

    printf("========================================================\n");
    return success;
}

Status skip_header(ToviewInfo *Toview)
{
    return success;                                             // Placeholder function (not implemented yet)
}

Status Read_contents(ToviewInfo *Toview)
{
    return success;                                             // Placeholder function (not implemented yet)
}
