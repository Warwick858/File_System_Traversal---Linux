// ******************************************************************************************************************
//  File System Traversal - traverses directories, files, and subdirectories.
//  Copyright(C) 2018  James LoForti
//  Contact Info: jamesloforti@gmail.com
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.If not, see<https://www.gnu.org/licenses/>.
//									     ____.           .____             _____  _______   
//									    |    |           |    |    ____   /  |  | \   _  \  
//									    |    |   ______  |    |   /  _ \ /   |  |_/  /_\  \ 
//									/\__|    |  /_____/  |    |__(  <_> )    ^   /\  \_/   \
//									\________|           |_______ \____/\____   |  \_____  /
//									                             \/          |__|        \/ 
//
// ******************************************************************************************************************
//

#include <stdlib.h>
#include <stdio.h> // needed for printf()
#include <string.h> // needed for strcpy()
#include <sys/types.h> // needed for dir functions
#include <sys/stat.h> // needed for lstat()
#include <dirent.h> // needed for dirent struct

#define BUFFER_SIZE 500

void traverse_dir(const char* path, int count);

//Declare globals
int totalBytes;

int main(int argc, char* argv[])
{
	//Initialize globals
	totalBytes = 0;

	//Print opening seperator, name
	printf("\n*********************************************** "
			"\nName: James LoForti \n\n");

	//If directory is not specified
	if (argc == 1)
	{
		printf("Dir: %s\n", ".");
		traverse_dir(".", 1);
	}
	else // directory given
	{
		printf("Dir: %s\n", argv[1]);
		traverse_dir(argv[1], 1);
	}

	//Print total bytes and closing seperator
	printf("\nTotal file space used: %d\n", totalBytes);
	printf("\n*********************************************** \n");

	return 0;
} // end main()

void traverse_dir(const char* path, int count)
{
	//Declare vars
	DIR* dirPtr;
	struct dirent* direntPtr;
	struct stat fileStats;
	char* bufferPtr;

	//Open the given directory
	dirPtr = opendir(path);

	//Error check
	if (dirPtr == NULL)
	{
		printf("Open directory failed, exiting... \n");
		return;
	}

	//While directory entries remain
    while ((direntPtr = readdir(dirPtr)) != NULL) 
    {
		//Construct the current absolute path    	
    	char absPath[BUFFER_SIZE];
		if (strcmp(direntPtr->d_name, ".") != 0)
		{
			strcpy(absPath, path);
			strcat(absPath, "/");
    		strcat(absPath, direntPtr->d_name);
		}
		else // if current directory, just save dirent name
		{
			strcpy(absPath, direntPtr->d_name);
		}

    	//If entry is a file
    	if (direntPtr->d_type == DT_REG)
    	{
    		//Save stats for file
    		if ((lstat(absPath, &fileStats)) < 0)
    		{
    			printf("File stats acquisition failed, exiting... \n");
    			return;
    		}

    		//Sum the current file bytes into the total
    		totalBytes += fileStats.st_size;

    		//Print file name and size to console
    		printf("%*s File: %s - %ld\n", (count + 2), "", direntPtr->d_name, fileStats.st_size);
    	} // end if
    } // end while

	//Close and reopen directory for directory traversal    
    closedir(dirPtr);
	dirPtr = opendir(path);

	//Error check
	if (dirPtr == NULL)
	{
		printf("Open directory failed, exiting... \n");
		return;
	}

	//While directory entries remain
    while ((direntPtr = readdir(dirPtr)) != NULL) 
    {
		//Construct the current absolute path    	
    	char absPath[BUFFER_SIZE];
		if (strcmp(direntPtr->d_name, ".") != 0)
		{
			strcpy(absPath, path);
			strcat(absPath, "/");
    		strcat(absPath, direntPtr->d_name);
		}
		else // if current directory, just save dirent name
		{
			strcpy(absPath, direntPtr->d_name);
		}
    	
    	//If entry is a directory
        if (direntPtr->d_type == DT_DIR) 
        {
        	//If directory is NOT current or parent
            if ((strcmp(direntPtr->d_name, ".") != 0) && (strcmp(direntPtr->d_name, "..") != 0))
            {
                //Print the directory path to console
	            printf("%*s Dir: %s\n", count, "", absPath);

	            //Pass this new directory back into the function
	            traverse_dir(absPath, (count + 1));
            }
        }  // end if
    } // end while
    
    closedir(dirPtr);
} // end function traverse_dir()