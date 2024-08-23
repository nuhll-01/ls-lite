/*******************************************************************
* Author: Sadia Nowrin
* Date: June 20, 2024

* Description: 
* This C program implements UNIX commands such as ls -l, ls -a and ls -la with the help of lsExtended function.  

Usage:
* Compile it using a C compiler and run the executable from the command line with appropriate arguments.
*    - To list files/directories: ./minils -l[-a][-la] [path]
*    - optional arguments are specified in [ ]
*    - If no path is mentioned explicitly in ls command, it by default reads the path "." which is the current directory.
*******************************************************************/

#include "file.h" 

int main(int argc, char *argv[]) {
    // Check invalid no of arguments. Rest of the checking should be done in the lsExtended function
    if (argc < 1) {
        printf("Usage: %s [optional command: -l or -a] [optinal path]\n", argv[0]);
        return 1;
    }

    lsExtended(argc, argv);

    return 0;
}
