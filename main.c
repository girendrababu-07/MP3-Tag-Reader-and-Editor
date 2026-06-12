#include <stdio.h>      
#include <stdlib.h>     // for Standard library functions
#include <string.h>    
#include "view.h"       // Structure and function declarations for viewing MP3 tags
#include "edit.h"       // Structure and function declarations for editing MP3 tags

int main()
{
    ToviewInfo Toview;                          // Structure variable used for storing view operation data
    ToeditInfo Toedit;                          // Structure variable used for storing edit operation data

    int choice;                                 // Variable to store menu choice

    printf("1.View\n");     
    printf("2.Edit\n");     
    printf("3.Help\n");     
    printf("Enter the operation : ");
    scanf("%d", &choice);                       // Read user choice

    switch(choice)                              // Perform operation based on user input
    {
        case 1: // View operation
        
            printf("Enter MP3 file name : "); 
            scanf("%s", Toview.mp3_fname);      // Store MP3 file name inside structure variable

            to_view(&Toview);                   // Call function to read and display MP3 metadata

            break;

        case 2: // Edit operation

            char option[3];                     // Stores edit option like -t, -a, -A, etc
            char value[100];                    // Stores new value for selected tag

            printf("Enter MP3 file name : ");
            scanf("%s", Toedit.edit_mp3_fname);       // Store MP3 file name inside edit structure

            printf("\nSelect option to edit\n");      
            printf("-t : Title\n");
            printf("-a : Artist\n");
            printf("-A : Album\n");
            printf("-y : Year\n");
            printf("-g : Genre\n");
            printf("-c : Comment\n");

            printf("Enter option : ");
            scanf("%s", option);                // Read tag selection option

            printf("Enter new value : ");
            scanf(" %[^\n]", value);            // Read full line including spaces for new tag value

            to_edit(&Toedit, option, value);    // Call edit function to modify MP3 tag

            break;

        case 3: // Help section

            printf("\n");
            printf("=============== MP3 TAG READER HELP ===============\n");  // Display help header

            printf("    Example Flow:\n");
            printf("    Usage : ./mp3_tag_reader\n");                         // Shows how to run the program
            printf("    1.View -> View MP3 tags\n");    
            printf("    2.Edit -> Edit MP3 tags\n");    
            printf("    3.Help -> Display Help\n\n");   

            printf("    Enter the operation : 1\n");                          // Example for viewing tags
            printf("    Enter MP3 file name : test.mp3\n\n");

            printf("    Enter the operation : 2\n");                          // Example for editing tags
            printf("    Enter MP3 file name : test.mp3\n\n");

            printf("    Select option to edit:\n");                           // Shows available edit fields
            printf("    -t : Edit Title\n");
            printf("    -a : Edit Artist\n");
            printf("    -A : Edit Album\n");
            printf("    -y : Edit Year\n");
            printf("    -g : Edit Genre\n");
            printf("    -c : Edit Comment\n\n");
            
            printf("    Enter option : -t\n");                                // Example edit option
            printf("    Enter new value : shivam\n");                         // Example new tag value

            printf("===================================================\n");

            break;

        default:
            printf("Invalid Input\n");                                        // Executes if user enters invalid menu option
    }

    return 0;                                                                 // Program terminates successfully
}
