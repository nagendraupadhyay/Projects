//Nagendra Upadhyay. 02/26/2023. The code below is a "makefile" which complies the .c files and makes exe files. 
//The code limits itself to making one executable, and assumes that you are using gcc to do your compilation.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "fields.h"
#include "dllist.h"

int main(int argc, char *argv[]){
    // Declare and initialize variables
    IS is;
    Dllist CList = new_dllist();
    Dllist HList = new_dllist();
    Dllist FList = new_dllist();
    Dllist LList = new_dllist();
    int i = 0;
    char *Estring = NULL;
    struct stat sp;
    
    // Check command line arguments
    if(argc == 2){
        is = new_inputstruct(argv[1]);
    }else {
        is = new_inputstruct("fmakefile");
    }
    
    // Read the input file line by line
    while(get_line(is) >= 0){
        // Skip empty lines
        if(is->NF == 0) continue;
        
        // Process each line based on its first field
        if(strcmp(is->fields[0], "C") == 0){// C source files
            for(i = 1; i < is->NF; i++){
	            dll_append(CList, new_jval_s(strdup(is->fields[i])));
            }
        }else if(strcmp(is->fields[0], "H") == 0){// Header files
            for(i = 1; i < is->NF; i++){
	            dll_append(HList, new_jval_s(strdup(is->fields[i])));
            }
        }else if(strcmp(is->fields[0], "F") == 0){// Flags
            for(i = 1; i < is->NF; i++){
	            dll_append(FList, new_jval_s(strdup(is->fields[i])));
            }
        }else if(strcmp(is->fields[0], "L") == 0){// Libraries
            for(i = 1; i < is->NF; i++){
                dll_append(LList, new_jval_s(strdup(is->fields[i])));
            }
        }else if(strcmp(is->fields[0], "E") == 0){// Executable name
            if (Estring != NULL){
                fprintf(stderr, "fmakefile (%i) cannot have more than one E line\n", is->line);
                exit(1);
            }
            Estring = strdup(is->text1 + 2);
            Estring[strcspn(Estring, "\n")] = '\0';
        }else{
            exit(1);
        }
    }

    // Check if an executable name is provided
    if (Estring == NULL) {
        fprintf(stderr, "No executable specified\n");
        exit(1);
    }
    
    // Compute the maximum modification time of header files
    Dllist tmp;
    time_t mtime = 0;
    time_t maxHeaderFile = 0;
    dll_traverse(tmp, HList){
        char *f = tmp->val.s;
        int ret = stat(f,&sp);
        if(ret == 0){
            mtime = sp.st_mtime;
            if(maxHeaderFile < mtime){
                maxHeaderFile = mtime;
            }
        }
    }

    // Compute the maximum modification time of object files and
    // the list of C files that need to be compiled
    time_t maxObjFile = 0;
    // create a new linked list for object files
    Dllist OList = new_dllist();
    // create a new linked list for C files to be compiled
    Dllist CompileFiles = new_dllist();

    // iterate through the list of C files
    dll_traverse(tmp, CList){
        // get the name of the C file
        char *c = tmp->val.s;
        // allocate memory for the object file name
        char *o = malloc(strlen(c) + 2);
        // copy the name of the C file to the object file name
        strcpy(o, c);
        // find the last occurrence of '.' in the object file name
        char *dot = strrchr(o, '.');
        // replace the extension with ".o"
        strcpy(dot, ".o");

        // add the object file name to the object file list
        dll_append(OList, new_jval_s(strdup(o)));

        // create a stat struct
        struct stat sp;
        // get the status of the C file
        int ret = stat(c, &sp);
        if(ret != 0){
            fprintf(stderr, "fmakefile: %s: No such file or directory\n", jval_s(tmp->val));
            exit(1);
        }
        
        // get the modified time of the C file
        time_t mtimeC = sp.st_mtime;
        int ret1 = stat(o, &sp);

        if(ret1 != 0){
            // add the C file to the list of files to be compiled
            dll_append(CompileFiles, tmp->val);
        }else{
            time_t mtimeO = sp.st_mtime;
            if(mtimeO < mtimeC || mtimeO < maxHeaderFile){
                dll_append(CompileFiles, tmp->val);
            }else{
                // if the modified time of the object file is greater than the current maximum object file modified time
                if(maxObjFile < mtimeO){
                    // update the maximum object file modified time
                    maxObjFile = mtimeO;
                }
            }
        }
        free(o);
    }

    // Allocate memory for two strings, one for the command and one for the compiled files
    char *cmd = (char*)malloc(sizeof(char) * 100);
    char *Cprint = (char*)malloc(sizeof(char) * 100);
    // Set the initial command to "gcc -c"
    sprintf(cmd, "gcc -c");

    // Traverse a doubly linked list and append each element to the command string
    Dllist it;
    dll_traverse(it, FList){
        char* Flag = jval_s(it->val);
        size_t len = strlen(cmd);
        sprintf(cmd, "%s %s", cmd, Flag);
    }
    // Traverse another doubly linked list of files to be compiled
    dll_traverse(it, CompileFiles){
        char* CFile = it->val.s;
        // Append the file name to the command string
        sprintf(Cprint, "%s %s", cmd, CFile);
        printf("%s\n", Cprint);

        // Compile the file using the system() function and check for errors
        if(system(Cprint) != 0){
            fprintf(stderr, "Command failed.  Exiting\n");
            exit(1);
        }
    }
    // Traverse a third doubly linked list of C files and check if each file exists
    dll_traverse(it, CList){
        char* cFile = jval_s(it->val);
        if(stat(cFile, &sp) != 0) {
            fprintf(stderr, "fmakefile: %s: No such file or directory\n", cFile);
            exit(1);
        }
    }

    // Pointer to the command for compiling executable
    char* CompileExe = NULL;
    // Flag for whether executable needs to be compiled
    int PExe = 0;
    if(dll_empty(CompileFiles) == 0){
        // If there are source files to compile, set the flag to 1
        PExe = 1;
	}
    
    // Check if object files need to be updated
    if (PExe == 0  && !dll_empty(OList)) {
        // Traverse the object files list
        dll_traverse(tmp, OList) {
            // Get the object file's status
            if (stat(jval_s(tmp->val), &sp) == 0) {
                // Get the object file's modification time
                mtime = sp.st_mtime;
                // Update the max modification time if necessary
                if (mtime > maxObjFile) {
                    maxObjFile = mtime;
                }
            }
        }
    }

    // Check if the executable needs to be updated
    if(stat(Estring, &sp) == 0){
        mtime = sp.st_mtime;
        // If the executable is older than the max object file modification time, set the flag to 1
        if (mtime < maxObjFile){
            PExe = 1;
        }
    }else{
        // If the executable does not exist, set the flag to 1
        PExe = 1;
    }

    // Compile the executable if necessary
    if(PExe){
        // Allocate memory for the command
        CompileExe = (char*)malloc(sizeof(char)*100);
        // Create the basic command for compiling the executable
        sprintf(CompileExe, "gcc -o %s", Estring);

        // Traverse the lists of flags, object files, and libraries, and add them to the command string
        Dllist tmp1;
        dll_traverse(tmp1, FList){
            char* flag = jval_s(tmp1->val);
            sprintf(CompileExe + strlen(CompileExe), " %s", flag);
        }
        dll_traverse(tmp1, OList){
            char* oFile = jval_s(tmp1->val);
            sprintf(CompileExe + strlen(CompileExe), " %s", oFile);
        }
        dll_traverse(tmp1, LList){
            char* lib = jval_s(tmp1->val);
            sprintf(CompileExe + strlen(CompileExe), " %s", lib);
        }
        
        // Print the command and execute it using system()
        printf("%s\n", CompileExe);
        if (system(CompileExe) != 0) {
            fprintf(stderr, "Command failed.  Fakemake exiting\n");
            exit(1);
        }
    // If the executable is up to date, print a message and exit
    } else {
        printf("%s up to date\n", Estring);
        exit(0);
    }
    
    // Free the memory allocated for the various lists and strings and exit
    free_dllist(CList);
    free_dllist(HList);
    free_dllist(FList);
    free_dllist(LList);
    free_dllist(OList);
    free_dllist(CompileFiles);
    free(Estring);
    free(Cprint);
    free(cmd);
    free(CompileExe);
    jettison_inputstruct(is);
    return 0;
}
