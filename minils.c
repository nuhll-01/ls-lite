/*************************************************************************************************************
*                                                                                                            *
* Author: Erick Chavez                                                                                       *
* Created: June 12, 2024                                                                                     *
*                                                                                                            *
* A simple program which emulates the "ls" UNIX command with the flags [-a], [-l], [-la]                     *
*                                                                                                            *
* References/Citations:                                                                                      *
*   - https://pubs.opengroup.org/onlinepubs/000095399/functions/stat.html                                    *
*   - https://pubs.opengroup.org/onlinepubs/000095399/basedefs/sys/stat.h.html                               *
*   - https://codeforwin.org/c-programming/c-program-find-file-properties-using-stat-function                *
*   - https://www.man7.org/linux/man-pages/man1/ls.1.html                                                    *
*   - https://pubs.opengroup.org/onlinepubs/7908799/xsh/sysstat.h.html                                       *
*   - https://stackoverflow.com/questions/36252328/printout-st-uid-in-c-programming                          *
*   - https://pubs.opengroup.org/onlinepubs/009604499/functions/getpwuid.html                                *
*   - https://cboard.cprogramming.com/c-programming/91931-difference-between-st_atime-st_mtime-st_ctime.html *
*   - https://www.geeksforgeeks.org/strftime-function-in-c/                                                  *
*   - https://cboard.cprogramming.com/c-programming/72671-displaying-file-permissions.html                   *
*   - https://www.javatpoint.com/snprintf-function-in-c                                                      *
*   - https://cplusplus.com/reference/cstdio/snprintf/                                                       *
*                                                                                                            *
**************************************************************************************************************/ 


#include "file.h"
#define HYPHEN "-"
#define FCMD "-a"
#define SCMD "-l"
#define TCMD "-la"
#define CRDIR "."


/**
 * @brief Perform a countdown from a given value.
 * 
 * @param value The value to start the countdown from.
 */
void checkCmdErr(char *arg[]) {
   // check if the argument is JUST a hyphen
   if (strcmp(*arg, HYPHEN) == 0) {
      printf("%s\n", "Error: Invalid command flag. \nValid options: [-a] [-l] [-la].");
      exit(-1);
   }
   
   // if the argument does include a hyphen (with a specified flag)
   if (strstr(*arg, HYPHEN)) {
      // check if the flag is valid
      if ((strcmp(*arg, FCMD) != 0) && (strcmp(*arg, SCMD) != 0) && (strcmp(*arg, TCMD) != 0)) {
         printf("%s\n", "Error: Invalid command flag. \nValid options: [-a] [-l] [-la].");
         exit(-1);
      } 
   }
}

/*

Check whether or not an entry starts with a '.' character.

entry[] - the given entry to check 


returns 1 if it does start with a '.', otherwise return 0.
*/ 
int checkDot(char entry[]) {
   if (entry[0] == '.') {
      return 1;
   }

   return 0;
}

/*

Check if the command-line argument is a path or not.

*arg[] - the argument passed from the command-line

returns 1 if it is a path, otherwise return 0.

*/
int isFlag(char *arg[]) {
   if (strstr(*arg, HYPHEN)) {
      return 1;
   }
   return 0;
}

/*

Check if the command-line argument is a directory or not.

*arg[] - the argument passed from the command-line

returns 1 if it is a directory, otherwise return 0.

*/
int isDirectory(char *arg[]) {
   if (!(strstr(*arg, HYPHEN))) {
      return 1;
   }
   return 0;
}

/*

The primary function which executes the "ls" command.

argc - the number of arguments given from the command-line.
*argv[] - A string representation of the arguments given from the command-line.

*/
void lsExtended(int argc, char *argv[]) {
   if (argc > 1) {
      checkCmdErr(&argv[1]);
   }

   // handle the case were no flag or path is provided
   if (argc == 1) {
      struct dirent *entry;
      char mainPath[1024];
      const char *defPth = CRDIR;
      DIR *dir = opendir(defPth);

      if (dir == NULL) {
         perror(defPth);
         closedir(dir);
         exit(-1);
      }

      // traverse the directory
      while ((entry = readdir(dir)) != NULL) {
         // format a string to include the absolute and relative path
         snprintf(mainPath, sizeof(mainPath), "%s/%s", defPth, entry->d_name);
         printf("%s\t\t", mainPath);
      }

      puts("");
      closedir(dir);
   }

   // handle the case where there's more than 1 argument in the command-line
   if (argc > 1) {
      
      // check if it's a directory
      if (isDirectory(&argv[1]) == 1) {
         struct dirent *entry;
         const char *basepath = argv[1];
         char mainPath[1024];
         DIR *dir = opendir(basepath);

         if (dir == NULL) {
            perror(basepath);
            closedir(dir);
            exit(-1);
         }

         // traverse the directory
         while ((entry = readdir(dir)) != NULL) {
            // if the directory entry starts with a '.' character then ignore it 
            if (checkDot(entry->d_name) == 1) {
               continue;
            }

            // format a string to include the absolute and relative path
            snprintf(mainPath, sizeof(mainPath), "%s/%s", basepath, entry->d_name);

            printf("%s\t\t", entry->d_name);
         }

         puts("");
         closedir(dir);
      }

      // check if it's a command flag
      if (isFlag(&argv[1]) == 1) {

         // check if its the first flag
         if (strcmp(argv[1], FCMD) == 0) {
            struct dirent *entry;
            const char *basepath;

            if (argc > 2) {
               basepath = argv[2];
            } else {
               basepath = CRDIR;
            }
            
            DIR *dir = opendir(basepath);

            if (dir == NULL) {
               perror(basepath);
               closedir(dir);
               exit(-1);
            }

            // traverse through the directory
            while ((entry = readdir(dir)) != NULL) {
               printf("%s\t\t", entry->d_name);
            }

            puts("");
            closedir(dir);
         }

         // check if its the second or third flag
         if ((strcmp(argv[1], SCMD) == 0) || (strcmp(argv[1], TCMD) == 0)) {
            const char *basepath;
            char date[256];
            char mainPath[1024];
            struct stat buf;
            struct dirent *entry;
            struct passwd *pwd;
            struct group  *grp;
            struct tm *tm;

            // if there are more than 2 arguments, we assume a given path
            // otherwise, we assume the current path
            if (argc > 2) {
               basepath = argv[2];
            } else {
               basepath = CRDIR;
            }

            DIR *dir = opendir(basepath);

            if (dir == NULL) {
               perror(basepath);
               closedir(dir);
               exit(-1);
            }

            // traverse through the directory
            while ((entry = readdir(dir)) != NULL) {

               // if the user chose the second command
               // then ignore any directory entry that starts with a '.' character
               if (strcmp(argv[1], SCMD) == 0) {
                  if (checkDot(entry->d_name) == 1) {
                     continue;
                  }
               }

               // format a string to include the absolute and relative path
               snprintf(mainPath, sizeof(mainPath), "%s/%s", basepath, entry->d_name);

               // obtain information about the named file and write it to the area pointed to by the 'buf' argument
               if (stat(mainPath, &buf) == -1) {
                  continue;
               }
               
               pwd = getpwuid(buf.st_uid);
               grp = getgrgid(buf.st_gid);
               tm = localtime(&buf.st_mtime);

               if (pwd == NULL) {
                  perror("getpwuid");
                  exit(-1);
               }

               if (grp == NULL) {
                  perror("getgrgid");
                  exit(-1);
               }

               // search through the data and check if the type is a regular file 
               if (S_ISREG(buf.st_mode)) {
                  printf("%s", "-");
               }

               // search through the data and check if the type is a directory 
               if (S_ISDIR(buf.st_mode)) {
                  printf("%s", "dr");
               }

               // if its a regular file, determine its permissions
               if (S_ISREG(buf.st_mode)) {
                  if (buf.st_mode & S_IRUSR) printf("%s", "r");
                  else printf("%s", "-");
                  if (buf.st_mode & S_IWUSR) printf("%s", "w");
                  else printf("%s", "-");
                  if (buf.st_mode & S_IXUSR) printf("%s", "x");
                  else printf("%s", "-");
                     
                  if (buf.st_mode & S_IRGRP) printf("%s", "r");
                  else printf("%s", "-");
                  if (buf.st_mode & S_IWGRP) printf("%s", "w");
                  else printf("%s", "-");
                  if (buf.st_mode & S_IXGRP) printf("%s", "x"); 
                  else printf("%s", "-");

                  if (buf.st_mode & S_IROTH) printf("%s", "r");                     
                  else printf("%s", "-");
                  if (buf.st_mode & S_IWOTH) printf("%s", "w");
                  else printf("%s", "-");
                  if (buf.st_mode & S_IXOTH) printf("%s", "x");
                  else printf("%s", "-");
               }

               // if its a directory, determine its permissions
               if (S_ISDIR(buf.st_mode)) {
                  // determine the permissions of the "owner"
                  if (buf.st_mode & S_IWUSR) printf ("%s", "w");
                  else printf("%s", "-");
                  if (buf.st_mode & S_IXUSR) printf ("%s", "x");
                  else printf("%s", "-");
                  if (buf.st_mode & S_IRUSR) printf ("%s", "r");
                  else printf("%s", "-");

                  // determine the permissions of the "group"
                  if (buf.st_mode & S_IWGRP) printf ("%s", "w");
                  else printf("%s", "-");
                  if (buf.st_mode & S_IXGRP) printf ("%s", "x");
                  else printf("%s", "-");
                  if (buf.st_mode & S_IRGRP) printf ("%s", "r");
                  else printf("%s", "-");

                  // determine the permissions of "other"
                  if (buf.st_mode & S_IWOTH) printf ("%s", "w");
                  else printf("%s", "-");
                  if (buf.st_mode & S_IXOTH) printf ("%s", "x");
                  else printf("%s", "-");
                  if (buf.st_mode & S_IROTH) printf ("%s", "r");
                  else printf("%s", "-");
               }

               strftime(date, sizeof(date), "%b %d %H:%M", tm);
               printf("\t%d %2s %2s %10lld\t %2s %2s\n", buf.st_nlink, pwd->pw_name, grp->gr_name, buf.st_size, date, entry->d_name);
            }
            
            puts("");
            closedir(dir);
         }
      }
   }
}
