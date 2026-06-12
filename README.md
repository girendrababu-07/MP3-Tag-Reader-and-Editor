# MP3 Tag Reader & Editor

## Overview

MP3 Tag Reader & Editor is a C-based command-line application that allows users to read and modify ID3 metadata tags stored in MP3 files.

The project demonstrates low-level file handling, binary data processing, structure usage, and metadata manipulation in C.

## Features

### View MP3 Tags

* Title
* Artist
* Album
* Year
* Genre
* Comment

### Edit MP3 Tags

* Edit Title
* Edit Artist
* Edit Album
* Edit Year
* Edit Genre
* Edit Comment

## Technologies Used

* C Programming
* File Handling
* Structures
* Binary File Processing
* ID3v2 Metadata Parsing
* GCC Compiler

## Project Structure

```text
.
├── main.c
├── view.c
├── view.h
├── edit.c
├── edit.h
├── types.h
└── README.md
```

## Compilation

Compile the project using GCC:

```bash
gcc main.c view.c edit.c -o mp3_tag_reader
```

## Execution

Run the executable:

```bash
./mp3_tag_reader
```

## View Operation

Select option:

```text
1
```

Enter MP3 file name:

```text
test.mp3
```

Example Output:

```text
========================================================
ID3v2 tag found | ID3 Version : ID3v2 |
ID3v2.3 detected
========================================================

Title    : Sunnysunny
Artist   : Yo Yo Honey Singh
Album    : Yaariyan
Year     : 2021
Genre    : Bollywood Music
Comment  : eng

========================================================
```

## Edit Operation

Select option:

```text
2
```

Choose a tag to modify:

```text
-t : Title
-a : Artist
-A : Album
-y : Year
-g : Genre
-c : Comment
```

Example:

```text
Enter option : -t
Enter new value : New Song Title
```

Output:

```text
Tag updated successfully
```

## Key Concepts Implemented

* Binary File Operations
* Metadata Parsing
* ID3v1 and ID3v2 Tag Detection
* Big Endian to Little Endian Conversion
* Frame-Based Data Extraction
* File Pointer Manipulation
* Data Modification in Binary Files

## Challenges Faced

* Understanding the ID3v2 frame structure and metadata storage format.
* Handling binary file operations and endian conversion while preserving MP3 file integrity.

## Learning Outcomes

* Deep understanding of file handling in C.
* Practical experience with binary data processing.
* Knowledge of metadata structures used in MP3 files.
* Improved debugging and modular programming skills.

## Author

Girendra Babu
