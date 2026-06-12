#ifndef TYPES_H          // Header guard start: prevents multiple inclusion of this file
#define TYPES_H          // Defines TYPES_H if not already defined

/* User defined types */
typedef unsigned int uint;   // Creates alias 'uint' for 'unsigned int'

/* Status will be used in fn. return type */
typedef enum               // Define enumeration for function return status
{
    success,               // success = 0 (default first enum value)
    failure                // failure = 1 (next enum value)
} Status;                  // Name of enum type is 'Status'

typedef enum               // Define enumeration for operation types
{
    view_mp3,              // Represents view operation
    edit_mp3,              // Represents edit operation
    helpmenu,              // Represents help menu operation
    Invalid_operator       // Represents invalid operation
} OperationType;           // Name of enum type is 'OperationType'

#endif                     // End of header guard
