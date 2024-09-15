// Nagendra Upadhyay. CS360 Lab A Bonding. This lab was about the bonding of hydrogen and oxygen atoms to form water molecules. The program uses threads to simulate the atoms, and also 
// mutexes and condition variables to ensure that the threads wait until a bond can be formed before proceeding.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "bonding.h"

// Defines a structure called MolNode
typedef struct MolNode {
    int id, h1_id, h2_id, o_id;
    pthread_cond_t *condition;
    struct MolNode *flink, *blink;
} MolNode;

// Defines a structure called GlobalState
typedef struct GlobalState {
    MolNode *hydrogen_list, *oxygen_list;
    pthread_mutex_t *lock;
    char *verbosity_level;
} GlobalState;

// Function to initialize GlobalState structure
void* initialize_v(char* verbosity_level) {
    // Allocates memory for GlobalState structure and initializes all fields to zero
    GlobalState* state = calloc(1, sizeof(GlobalState));
    if (!state) {
        perror("calloc failed");
        exit(1);
    }
    // Initializes pointers to linked lists to NULL
    state->hydrogen_list = state->oxygen_list = NULL;
    // Allocates memory for mutex
    state->lock = malloc(sizeof(pthread_mutex_t));
    if (!state->lock) {
        perror("malloc failed");
        exit(1);
    }
    // Initializes mutex with default attributes
    int ret = pthread_mutex_init(state->lock, NULL);
    if (ret != 0) {
        printf("ret not 0");
    }
    // Duplicates the string pointed to by verbosity_level
    state->verbosity_level = strdup(verbosity_level);
    return state;
}

// Function to create a node for the linked list
MolNode* create_list_node(struct bonding_arg* arg) {
    MolNode* node = malloc(sizeof(MolNode));
    if (!node) {
        perror("node malloc failed");
        exit(1);
    }
    // Initializes all fields of the node to zero
    memset(node, 0, sizeof(MolNode));
    // Sets the ID of the node to the ID passed as an argument
    node->id = arg->id;
    node->condition = malloc(sizeof(pthread_cond_t));
    // Initializes the condition variable with default attributes
    pthread_cond_init(node->condition, NULL);
    return node;
}

// Function to add a node to the linked list
void add_node_to_list(MolNode** head, MolNode* node) {
    if (!head || !node) {
        perror("add_node_to_list got null");
        exit(1);
    }
    // If the linked list is empty, Set the head pointer to point to the new node
    if (*head == NULL) {
        *head = node;
    } else {
        // If the linked list is not empty. Set a temporary pointer to point to the head of the linked list
        MolNode* current = *head;
        // Traverse the linked list until the last node is reached
        while (current->flink != NULL) {
            // Move to the next node
            current = current->flink;
        }
        current->flink = node;
        node->blink = current;
    }
}

// Function to free memory allocated for a node
void free_list_node(MolNode *node) {
    free(node->condition);
    free(node);
}

// Function to simulate the behavior of a hydrogen atom
void *hydrogen(void *arg) {
    // Retrieve the argument passed to the function, which is a pointer to a bonding_arg struct
    struct bonding_arg *bond_arg = arg;
    if (!bond_arg) {
        perror("Pointer is empty");
    }
    // Get a pointer to the GlobalState struct stored in the bonding_arg struct
    GlobalState *state = bond_arg->v;
    // Create a new MolNode struct and assign it to a pointer called node
    MolNode *node = create_list_node(bond_arg);
    // Lock the mutex that controls access to the shared state data
    pthread_mutex_lock(state->lock);
   
    // If there is both an oxygen atom and a hydrogen atom in the queue 
    if (state->oxygen_list && state->hydrogen_list) {
        // Assign the IDs of the hydrogen atoms and the oxygen atom to the MolNode struct
        node->h1_id = state->hydrogen_list->id;
        node->h2_id = node->id;
        node->o_id = state->oxygen_list->id;

        // Update the IDs of the hydrogen atoms and the oxygen atom in the MolNode struct of the first hydrogen atom in the queue
        state->hydrogen_list->h1_id = node->h1_id;
        state->hydrogen_list->h2_id = node->h2_id;
        state->hydrogen_list->o_id = node->o_id;

        // Update the IDs of the hydrogen atoms and the oxygen atom in the MolNode struct of the oxygen atom in the queue
        state->oxygen_list->h1_id = node->h1_id;
        state->oxygen_list->h2_id = node->h2_id;
        state->oxygen_list->o_id = node->o_id;

        // Signal the threads waiting for a hydrogen atom and an oxygen atom respectively
        pthread_cond_signal(state->hydrogen_list->condition);
        pthread_cond_signal(state->oxygen_list->condition);

        // Remove the hydrogen atom and the oxygen atom from the queue
        state->hydrogen_list = state->hydrogen_list->flink;
        state->oxygen_list = state->oxygen_list->flink;

        // Unlock the mutex that controls access to the shared state data
        pthread_mutex_unlock(state->lock);
    } else {
        // If there is not both an oxygen atom and a hydrogen atom in the queue then
        // add the MolNode struct to the queue of hydrogen atoms or oxygen atoms depending on the thread calling the function
        add_node_to_list(&state->hydrogen_list, node);
        // Wait for a hydrogen atom and an oxygen atom to be in the queue
        pthread_cond_wait(node->condition, state->lock);
        // Unlock the mutex that controls access to the shared state data
        pthread_mutex_unlock(state->lock);
    }

    // Call the Bond() function with the IDs of the hydrogen atoms and the oxygen atom and free the MolNode struct
    void *result = (void *) Bond(node->h1_id, node->h2_id, node->o_id);
    free_list_node(node);
    return result;
}

// This function represents a thread for oxygen atoms
void *oxygen(void *arg) {
    // Doing the same things as done in the hydrogen function.
    struct bonding_arg *bond_arg = arg;
    if (!bond_arg) {
        perror("Pointer is empty");
    }
    GlobalState *state = bond_arg->v;
    MolNode *node = create_list_node(bond_arg);
    pthread_mutex_lock(state->lock);
    
    // If there are two hydrogen atoms available for bonding, retrieve them and bond with the current oxygen atom
    if (state->hydrogen_list && state->hydrogen_list->flink) {
        // Retrieve the first two hydrogen atoms from the list
        MolNode* hydrogen1 = state->hydrogen_list;
        MolNode* hydrogen2 = state->hydrogen_list->flink;
        // Remove the first two hydrogen atoms from the list
        state->hydrogen_list = state->hydrogen_list->flink->flink;

        // Assign the IDs of the hydrogen and oxygen atoms to each other
        node->h1_id = hydrogen1->id;
        node->h2_id = hydrogen2->id;
        node->o_id = node->id;

        hydrogen1->h1_id = node->h1_id;
        hydrogen1->h2_id = node->h2_id;
        hydrogen1->o_id = node->o_id;

        hydrogen2->h1_id = node->h1_id;
        hydrogen2->h2_id = node->h2_id;
        hydrogen2->o_id = node->o_id;

        // Signal the waiting hydrogen atoms to proceed with the bonding process
        pthread_cond_signal(hydrogen1->condition);
        pthread_cond_signal(hydrogen2->condition);
    } else {
        // If there are no hydrogen atoms available for bonding, add the current oxygen atom to the oxygen list and wait
        add_node_to_list(&state->oxygen_list, node);
        pthread_cond_wait(node->condition, state->lock);
    }
    // Unlock the mutex after accessing shared data
    pthread_mutex_unlock(state->lock);

    // Bond the hydrogen and oxygen atoms together and return the result
    void *result = (void *) Bond(node->h1_id, node->h2_id, node->o_id);
    free_list_node(node);
    return result;
}
