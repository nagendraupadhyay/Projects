//03.19.23. Nagendra Upadhyay. Lab 5 is about making tarc code which takes single argument on the command line which is the name of a directory. 
//It can be an absolute pathname, or a relative pathname. Tarc will then create a tarfile, which has enough information to recreate the directory and all of its contents. 
//It prints this tarfile on standard output.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "jrb.h"
#include "dllist.h"
#include "jval.h"

void traverse_directories(JRB tree, Dllist data, int shift);

void DirInfo(JRB tree, const char *path, int shift){
    // open the directory
    DIR *dir = opendir(path);
    struct dirent *entry;
    struct stat st;
    // create a new doubly linked list to store the subdirectories
    Dllist data = new_dllist();

    // loop through the entries in the directory
    while((entry = readdir(dir)) != NULL){
        // skip the current directory and the parent directory
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        // create a full path to the entry
        char full_path[1000];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        // get information about the entry
        if(lstat(full_path, &st) != 0) continue;

        // calculate the file path relative to the root directory
        char *file_path = full_path + shift;
        int file_path_len = strlen(file_path);

        // write the file path, inode number, mode, and modification time to stdout
        fwrite(&file_path_len, sizeof(int), 1, stdout);
        printf("%s", file_path);
        fwrite(&st.st_ino, sizeof(st.st_ino), 1, stdout);

        // check if the inode number is already in the tree
        JRB node = jrb_find_int(tree, st.st_ino);
        if(node == NULL){
            // if not, insert it and write the mode and modification time to stdout
            jrb_insert_int(tree, st.st_ino, new_jval_i(0));
            fwrite(&st.st_mode, sizeof(st.st_mode), 1, stdout);
            fwrite(&st.st_mtime, sizeof(st.st_mtime), 1, stdout);

            // if it's a directory, add it to the list of subdirectories
            if(S_ISDIR(st.st_mode)){
                dll_append(data, new_jval_s(strdup(full_path)));
            }else{
                // if it's a file, open it and write its size and contents to stdout
                FILE *file = fopen(full_path, "r");
                if(file != NULL){
                    fwrite(&st.st_size, sizeof(st.st_size), 1, stdout);
                    char buf[100];
                    size_t bytes_read;
                    while((bytes_read = fread(buf, 1, sizeof(buf), file)) > 0){
                        fwrite(buf, 1, bytes_read, stdout);
                    }
                    fclose(file);
                }
            }
        }
    }
    closedir(dir);
    // traverse the subdirectories recursively
    traverse_directories(tree, data, shift);
}

void traverse_directories(JRB tree, Dllist data, int shift){
    // loop through the list of subdirectories
    Dllist tmp;
    dll_traverse(tmp, data){
        // traverse the subdirectory recursively and free the memory used by the path string
        DirInfo(tree, tmp->val.s, shift);
        free(tmp->val.s);   
    }
    // free the memory used by the list of subdirectories
    free_dllist(data);
}

// Function writes the file name length, file name, and file information to standard output.
void write_file_info(int NameLen, char *TmpShift, struct stat buf){
    // write the file name length
    fwrite(&NameLen, sizeof(int), 1, stdout);  
    // write the file name
    printf("%s", TmpShift);
    // write the inode number
    fwrite(&buf.st_ino, sizeof(buf.st_ino), 1, stdout);
    // write the file mode
    fwrite(&buf.st_mode, sizeof(buf.st_mode), 1, stdout);
    // write the last modification time
    fwrite(&buf.st_mtime, sizeof(buf.st_mtime), 1, stdout);
}

//  Function frees memory allocated for a binary search tree and a file name.
void FreeMem(JRB tree, char *FileName){
    JRB tmp;
    jrb_traverse(tmp, tree){
        free(tmp->val.v);
    }
    jrb_free_tree(tree);
    free(FileName);
}

int main(int argc, char *argv[]){
    struct stat buf;
    struct dirent *de;
    char *DirName = NULL, *TmpName = NULL, *FileName = NULL;
    DIR *dir = NULL;
    JRB tree = NULL;
    int exists = 0, shift = 0, NameLen = 0;
    
    // Allocate memory for strings to hold the directory name, temporary directory name, and file name.
    DirName = malloc(strlen(argv[1]) + 1);
    TmpName = malloc(strlen(argv[1]) + 1);
    FileName = strdup(argv[1]);
    
    // Check if memory allocation was successful
    if(!DirName || !TmpName || !FileName){
        fprintf(stderr, "Error: unable to allocate memory.\n");
        exit(1);
    }
    
    // Copy the directory name to the DirName and TmpName strings, and obtain file information for the input directory.
    strcpy(DirName, argv[1]);
    strcpy(TmpName, argv[1]);
    exists = lstat(argv[1], &buf);

    // Find the position of the last '/' character in the directory name, which will be used to determine the length of the file name
    char *EF = strrchr(DirName, '/');
    
    if(EF != NULL){
        shift = (int)(EF - DirName + 1);
    }
    
    // Obtain a pointer to the start of the file name string, and calculate the length of the file name. Then write the file information to standard output.
    char *TmpShift = TmpName + shift;
    NameLen = (int)strlen(TmpShift);
    write_file_info(NameLen, TmpShift, buf);
    dir = opendir(".");
    
    if(dir == NULL){
        fprintf(stderr, "Error: unable to open directory.\n");
        exit(1);
    }
    
    // Create a binary search tree to store information about the directory's files, and fill the tree by calling the DirInfo function.
    tree = make_jrb();
    DirInfo(tree, FileName, shift);
    closedir(dir);
    
    // Free the memory when done
    FreeMem(tree, FileName);
    free(DirName);
    free(TmpName);
    
    return 0;
}
