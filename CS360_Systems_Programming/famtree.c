//Nagendra Upadhyay. 02/12/23. Lab2 A family tree lab were you have to take in data and make a JBR tree
//the tree holds connections between people and prints a detailed output of connections.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fields.h"
#include "jrb.h"
#include "dllist.h"
#include "jval.h"

//Define the Person struct to store information about a person
typedef struct Person
{
    char* Name;
    char* Sex;
    struct Person *Father;
    struct Person *Mother;
    Dllist Children;
    Dllist tmpD;
    int visited; 
    int printed; 
    
}Person;

// Check if a person is a descendant of another person
int is_descendant(Person *p, JRB tree)
{
    // If the person has already been visited, return false
    if (p->visited == 1) return 0;
    // If the person is currently being visited, there is a cycle in the input
    if (p->visited == 2){ fprintf(stderr, "Bad input -- cycle in specification\n"); exit(0);}

    // Mark the person as currently being visited
    p->visited = 2;
    
    // Check if any of the person's children are descendants
    dll_traverse(p->tmpD, p->Children){
        Person* child = (Person*) p->tmpD->val.v;
        if (is_descendant(child, tree)) return 1;
    }

    // Mark the person as visited
    p->visited = 1;
    return 0;
}

// Retrieve a person from the tree based on their name
Person* getPersonName(JRB tree, char* Name)
{
    // Try to find the person in the tree
    JRB node = jrb_find_str(tree, Name);
    
    // If the person is not found, create a new person with the given name
    if (node == NULL)
    {
        Person *p = malloc(sizeof(Person));
        if (p == NULL)
        {
            exit(1);
        }
        
        // Initialize the person's information
        p->Name = Name;
        p->Sex = "";
        p->Father = NULL;
        p->Mother = NULL;
        p->Children = new_dllist();
        p->visited = 0;
        p->printed = 0;
        
        // Add the person to the tree
        jrb_insert_str(tree, Name, new_jval_v(p));
        return p;
    }else
    {
        // If the person is found, return the person's information
        return node->val.v;
    }
}

// Concatenate a full name from multiple fields
char* FullName(IS is){
    int nsize;
    int i;
    char *TN;
    nsize = strlen(is->fields[1]);
    for (i = 2; i < is->NF; i++){
        nsize += (strlen(is->fields[i])+1);
    }
    
    // Allocate memory for the concatenated name string
    TN = (char *) malloc(sizeof(char)*(nsize+1));
    if(TN == NULL)
    {
        exit(1);
    }

    // Copy the first field to the concatenated string
    strcpy(TN, is->fields[1]);
    nsize = strlen(is->fields[1]);
    
    // Concatenate the rest of the fields to the concatenated string
    for (i = 2; i < is->NF; i++) {
        TN[nsize] = ' ';
        strcpy(TN+nsize+1, is->fields[i]);
        nsize += strlen(TN+nsize);
    }
    return TN;
}

int main(){
    // Declare pointers, JRB trees, input struct, and necessary variables 
    Person *p; 
    Person *pp;
    JRB tmpTree;
    JRB t;
    IS is = new_inputstruct(NULL);
    t = make_jrb();  
    int FPrint;
    int MPrint;
    
    // Loop through input lines
    while (get_line(is) >= 0) {
        // Skip line if it has no fields
        if (is->NF == 0) continue;
        
        // Check if first field is "PERSON"
        if(strcmp(is->fields[0], "PERSON") == 0) {
            char* Name = FullName(is);
            p = getPersonName(t, Name);
        }else{
            // Check if first field is "SEX"
            if(strcmp(is->fields[0], "SEX") == 0){
                // Check if second field is "F"
                if(strcmp(is->fields[1], "F") == 0){
                    // Check if the person's sex has already been set
                    if(strcmp(p->Sex, "") != 0){
                        // Check if the person's sex was previously set to "M"
                        if(strcmp(p->Sex, "M") == 0){
                            // Print an error message
                            fprintf(stderr, "Bad input - sex mismatch on line %d\n", is->line);
                        }
                    }
                    // Set the person's sex to "Female"
                    p->Sex = strdup("Female");
                }
                if(strcmp(is->fields[1], "M") == 0){
                    if(strcmp(p->Sex, "") != 0){
                        if(strcmp(p->Sex, "F") == 0){    
                            fprintf(stderr, "Bad input - sex mismatch on line %d\n", is->line);
                        }
                    }
                    p->Sex = strdup("Male");
                }
            }
            // Check if first field is "FATHER_OF"
            if(strcmp(is->fields[0], "FATHER_OF") == 0){
                //Getting the child's full name
                char* ChildName = FullName(is);
                //Getting the child's person structure
                pp = getPersonName(t, ChildName);
               
                //If the parent's sex is not defined yet
                if(p->Sex == NULL){
                    p->Sex = strdup("Male");
                }else if(strcmp(p->Sex, "Female") == 0){
                    //If the parent's sex is already defined as "Female", then it is wrong
                    fprintf(stderr, "Bad input - sex mismatch on line %d\n", is->line);
                    exit(1);
                }else{
                    //Assign "Male" as the parent's sex
                    p->Sex = strdup("Male");
                }
                
                //Checking if the parent's sex is not "Male"
                if(strcmp(p->Sex, "Male") != 0){
                    //Print error message and exit
                    fprintf(stderr, "Bad input - sex mismatch on line %d\n", is->line);
                }
                //Checking if the child already has a father assigned
                if(pp->Father != NULL){
                    if(pp->Father != p){
                        fprintf(stderr, "Bad input - sex mismatch on line %d\n", is->line);
                    }
                }else{
                    //If the child does not have a father assigned yet
                    //Assign the current parent as the child's father
                    pp->Father = p;
                    //Append the child to the parent's Children list
                    dll_append(p->Children, new_jval_v(pp));
                }
            }
            //MOTHER_OF
            if(strcmp(is->fields[0], "MOTHER_OF") == 0){
                char* ChildName = FullName(is);
                pp = getPersonName(t, ChildName);
                
                if(p->Sex == NULL){
                    p->Sex = strdup("Female");
                }else if(strcmp(p->Sex, "Male") == 0){
                    fprintf(stderr, "Bad input - sex mismatch on line %d\n", is->line);
                    exit(0);
                }else{
                    p->Sex = strdup("Female");
                }

                if(strcmp(p->Sex, "Female") != 0){
                    fprintf(stderr, "Bad input - sex mismatch on line %d\n", is->line);
                }
                if(pp->Mother != NULL){
                    if(pp->Mother != p){
                        fprintf(stderr, "Bad input - sex mismatch on line %d\n", is->line);
                    }
                }else{
                    pp->Mother = p;
                    dll_append(p->Children, new_jval_v(pp));
                }
            }
            //FATHER
            if(strcmp(is->fields[0], "FATHER") == 0){
                char* Name = FullName(is);
                int check = 1;
                pp = getPersonName(t, Name);
                if(p->Father != NULL && strcmp(p->Father->Name, pp->Name) != 0){
                    fprintf(stderr, "Bad input -- child with two fathers on line %i\n", is->line);
                    exit(0);
                }
                p->Father = pp;
                if(pp == NULL){
                    printf("No Father Found.\n");
                }else{
                    if(pp->Sex == NULL){
                        pp->Sex = strdup("Male");
                    }else if(strcmp(pp->Sex, "Female") == 0){
                        fprintf(stderr, "Bad input - sex mismatch on line %i\n", is->line);
                        exit(0);
                    }else{
                        pp->Sex = strdup("Male");
                    }
                    dll_traverse(pp->tmpD, pp->Children){
                        Person *child = (Person*) pp->tmpD->val.v;
                        if(strcmp(child->Name,p->Name) == 0){
                            check = 0;
                            break;
                        }
                    }
                    if(check){
                        dll_append(pp->Children, new_jval_v(p));  
                    }
                }
            }
            //MOTHER
            if(strcmp(is->fields[0], "MOTHER") == 0){
                char* Name = FullName(is);
                int check = 1;
                pp = getPersonName(t, Name);
                if(p->Mother != NULL && strcmp(p->Mother->Name, pp->Name) != 0){
                    fprintf(stderr, "Bad input -- child with two mothers on line %i\n", is->line);
                    exit(0);  
                }
                p->Mother = pp;
                if(pp == NULL){
                    //printf("No Mother Found.\n");
                }else{
                    if(pp->Sex == NULL){
                        pp->Sex = strdup("Female");
                    }else if(strcmp(pp->Sex, "Male") == 0){
                        fprintf(stderr, "Bad input - sex mismatch on line %d\n", is->line);
                        exit(1);
                    }else{
                        pp->Sex = strdup("Female");
                    }
                    dll_traverse(pp->tmpD, pp->Children){
                        Person *child = (Person*) pp->tmpD->val.v;
                        if(strcmp(child->Name,p->Name) == 0){
                            check = 0;
                            break;
                        }
                    }
                    if(check){
                        //Adding the current person to the children
                        dll_append(pp->Children, new_jval_v(p));
                    }
                }
            }
            //Clearing the first field in input fields after processing
            is->fields[0] = "";
        }
    }
    
    // Free the memory used by the inputstruct 
    jettison_inputstruct(is);

    // Initialize a JRB tree for DFS checking
    JRB DFSCheck;

    // Traverse the tree and call the is_descendant function for each person
    jrb_traverse(DFSCheck, t){
        p = ((Person*) DFSCheck->val.v);
        is_descendant(p, t);
    } 
    
    // Create a new doubly linked list for checking the sort order
    Dllist checkSort = new_dllist();
    jrb_traverse(tmpTree, t){
        p = (Person*) tmpTree->val.v;
        if(p->Father == NULL && p->Mother == NULL){
            dll_append(checkSort, new_jval_v(p));
        }
    }
    
    // While there are still people in the checkSort list
    while(!dll_empty(checkSort)){
        // Get the first node in the checkSort list
        Dllist node = dll_first(checkSort);
        // Remove the node from the list
        dll_delete_node(dll_first(checkSort));
        // Get the person stored in the node
        Person *p2 = (Person*) node->val.v;
        
        // If the person has not been printed yet
        if(p2->printed == 0){
            // Check if the father and mother have been printed
            if(p2->Father != NULL){
                FPrint = p2->Father->printed;
            }else{
                FPrint = 0;
            }
            if(p2->Mother != NULL){
                MPrint = p2->Mother->printed;
            }else{
                MPrint = 0;
            }

            // If the person has no parents or both parents have been printed
            if((p2->Father == NULL && p2->Mother == NULL) 
                    || (FPrint == 1 && p2->Mother == NULL) 
                    || (p2->Father == NULL && MPrint == 1) 
                    || (FPrint == 1 && MPrint == 1)){
                
                // Get the person from the node
                p = (Person*) node->val.v;
                // Print the person's name
                printf("%s\n", p->Name);
                // Mark the person as printed
                p->printed = 1;
                
                // Print the person's sex
                if(p->Sex != NULL && strcmp(p->Sex, "") != 0){
                    printf("  Sex: %s\n", p->Sex);
                }else{
                    printf("  Sex: Unknown\n");
                }
 
                // Print the person's father's name
                if(p->Father != NULL){
                    printf("  Father: %s\n", p->Father->Name);
                }else{
                    printf("  Father: Unknown\n");
                }

                // Print the person's mother's name
                if(p->Mother != NULL){
                    printf("  Mother: %s\n", p->Mother->Name);
                }else{
                    printf("  Mother: Unknown\n");
                }

                // Print the person's children's name
                if(dll_empty(p->Children)){
                    printf("  Children: None\n\n");
                }else{
                    // If person has multiple children go through the tree and print
                    printf("  Children:\n");
                    dll_traverse(p->tmpD, p->Children){
                        Person *child = (Person*) p->tmpD->val.v;
                        printf("    %s\n", child->Name);
                        dll_append(checkSort, new_jval_v(child));
                    }
                    printf("\n");
                }
            }
        }
    }
    return 0;
}
