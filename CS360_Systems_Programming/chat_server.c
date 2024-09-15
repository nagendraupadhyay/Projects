// Nagendra Upadhyay. CS360 Lab9. This lab is about creating a chat server 
// using pthreads that allows clients to chat with each other using nc (or jtelnet)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "sockettome.h"
#include "dllist.h"
#include "jrb.h"

typedef struct chatroom {
    char *roomName;  
    Dllist clientList;                               
    Dllist chatList;                           
    pthread_mutex_t *lockRoom;
    pthread_cond_t *roomCond;
} *ChatRoom;

typedef struct client {
    char *clientName;
    int fd;                                                    
    int roomNum;                                               
    FILE *fileOut;                                  
    pthread_mutex_t *clientLock;                            
    ChatRoom rooms;
} *Client;

void *roomThread(void *arg)
{
    // Cast the argument to ChatRoom type and assign it to cRoom variable
    ChatRoom cRoom = (ChatRoom)arg;
    while (1) {
        // Acquire a lock on the chat room
        pthread_mutex_lock(cRoom->lockRoom);
        // Check if the chat list is empty and wait until there is chat in the chat list
        while (dll_empty(cRoom->chatList))
            pthread_cond_wait(cRoom->roomCond, cRoom->lockRoom);
        // Assign the chat list to cPtr variable
        Dllist cPtr = cRoom->chatList;
        // Iterate over the chat list until it's empty
        while (!dll_empty(cPtr)) {
            // Get the chat message from the chat list
            char *chat = (char *)cPtr->flink->val.s;
            Dllist clientPtr = cRoom->clientList;
            // Iterate over the client list until reaching the end
            while (clientPtr->flink != cRoom->clientList) {
                // Get the client from the client list
                Client chatter = (Client)clientPtr->flink->val.v;
                pthread_mutex_lock(chatter->clientLock);
                if (fputs(chat, chatter->fileOut) == EOF || fflush(chatter->fileOut) == EOF) {
                    // Write the chat message to the client's output file and check for errors
                    // If an error occurs, close the file and delete the client from the list
                    fclose(chatter->fileOut);
                    dll_delete_node(clientPtr->flink);
                } else {// Move to the next client in the list
                    clientPtr = clientPtr->flink;
                }
                pthread_mutex_unlock(chatter->clientLock);
            }
            // Delete the processed chat message from the chat list
            dll_delete_node(cPtr->flink);
        }
        // Release the lock on the chat room
        pthread_mutex_unlock(cRoom->lockRoom);
    }
    return NULL;
}

void close_files_and_exit(FILE *fin, FILE *fout) {
    fclose(fin);
    fclose(fout);
    pthread_exit(NULL);
}

// This function is executed in a separate thread for each client connecting to the server.
// It handles the communication between the client and the server.
void *clientThread(void *arg)
{
    Client client = (Client) arg;
    Client tmpClient;
    ChatRoom room;
    FILE *fin = fdopen(client->fd, "r");
    FILE *fout = fdopen(client->fd, "w");
    Dllist clientPtr;
    int i, isRoomValid = 0;
    char buf[1000], chatLine[2000], *tmp;
    client->fileOut = fout;

    // Flush the output stream and send the list of available chat rooms to the client
    if (fflush(fout) == EOF) close_files_and_exit(fin, fout);
    sprintf(buf, "Chat Rooms:\n\n");
    if (fputs(buf, fout) == EOF || fflush(fout) == EOF) close_files_and_exit(fin, fout);

    // Iterate over each chat room and send the list of clients in each room to the client
    for (i = 0; i < client->roomNum; i++) {
        pthread_mutex_lock(client->rooms[i].lockRoom);
        memset(buf, 0, sizeof(buf));
        tmp = buf;
        tmp += sprintf(tmp, "%s:", client->rooms[i].roomName);

        // Traverse the client list of the current room and append the client names to the buffer
        dll_traverse(clientPtr, client->rooms[i].clientList) {
            tmpClient = clientPtr->val.v;
            tmp += sprintf(tmp, " %s", tmpClient->clientName);
        }
        tmp += sprintf(tmp, "\n");

        // Send the buffer containing the chat room information to the client
        if (fputs(buf, fout) == EOF || fflush(fout) == EOF) close_files_and_exit(fin, fout);
        pthread_mutex_unlock(client->rooms[i].lockRoom);
    }

    // Prompt the client to enter their chat name
    fprintf(fout, "\nEnter your chat name (no spaces):\n");
    if (fflush(fout) == EOF) close_files_and_exit(fin, fout);
    // Read the client's chat name from the input stream
    if (fgets(buf, sizeof(buf), fin) == NULL) close_files_and_exit(fin, fout);
    buf[strcspn(buf, "\r\n")] = '\0';  // remove newline characters
    client->clientName = strdup(buf);

    // Loop until a valid chat room is selected by the client
    while (!isRoomValid) {
        fprintf(fout, "Enter chat room:\n");
        if (fflush(fout) == EOF) close_files_and_exit(fin, fout);
        // Read the client's selected chat room from the input stream
        if (fgets(buf, sizeof(buf), fin) == NULL) close_files_and_exit(fin, fout);
        buf[strcspn(buf, "\r\n")] = '\0';  // remove newline characters
        tmp = strdup(buf);

        // Check if the selected chat room is valid and join the room if it exists
        for (i = 0; i < client->roomNum; i++) {
            // Check if the current room matches the selected room
            if (strcmp(client->rooms[i].roomName, buf) == 0) {
                // Mark the room as valid
                isRoomValid = 1;
                // Set the room pointer to the current room
                room = &(client->rooms[i]);

                pthread_mutex_lock(room->lockRoom);
                // Add the client to the room's client list
                dll_append(room->clientList, new_jval_v((void *) client));
                // Create a chat message indicating the client has joined
                sprintf(chatLine, "%s has joined\n", client->clientName);
                // Append the chat message to the room's chat list
                dll_append(room->chatList, new_jval_s(chatLine));

                // Signal the condition variable to wake up waiting threads
                pthread_cond_signal(room->roomCond);
                pthread_mutex_unlock(room->lockRoom);
                break;
            }
        }
        // If no valid room is found, notify the client and prompt for another room selection
        if (!isRoomValid) {
            sprintf(chatLine, "No chat room %s. \n", tmp);
            if (fputs(chatLine, fout) == EOF || fflush(fout) == EOF) close_files_and_exit(fin, fout);
        }
    }

    // Continuously read chat messages from the client until EOF is encountered
    while (fgets(buf, 1000, fin) != NULL) {
        // Format the chat message with the client's name
        sprintf(chatLine, "%s: %s", client->clientName, buf);
        pthread_mutex_lock(room->lockRoom);
        // Append the chat message to the room's chat list
        dll_append(room->chatList, new_jval_s(strdup(chatLine)));
        pthread_cond_signal(room->roomCond);
        pthread_mutex_unlock(room->lockRoom);
    }
    pthread_mutex_lock(client->clientLock);
    fclose(fin);    
    pthread_mutex_unlock(client->clientLock);

    pthread_mutex_lock(room->lockRoom); 
    dll_traverse(clientPtr, room->clientList) {
        tmpClient = (Client) clientPtr->val.v;
        if (strcmp(tmpClient->clientName, client->clientName) == 0) {
            fclose(fout);
            // Remove the client from the room's client list
            dll_delete_node(clientPtr);
            break;
        }
    }
    // Create a chat message indicating the client has left
    sprintf(buf, "%s has left\n", client->clientName);
    // Append the chat message to the room's chat list
    dll_append(room->chatList, new_jval_s(strdup(buf)));
    pthread_cond_signal(room->roomCond);
    pthread_mutex_unlock(room->lockRoom);
    pthread_exit(NULL);
    return NULL;
}

int main(int argc, char **argv) {
    int portUse = atoi(argv[1]);
    int numRoom = argc - 2;

    // Creating a JRB to sort room names
    JRB sortRoomNames = make_jrb();
    for (int i = 0; i < numRoom; i++) {
        jrb_insert_str(sortRoomNames, argv[i + 2], new_jval_v(NULL));
    }

    // Allocating memory for chat rooms and chatroomThreads
    ChatRoom rooms = calloc(numRoom, sizeof(struct chatroom));
    pthread_t *chatroomThreads = calloc(numRoom, sizeof(pthread_t));

    // Creating chat rooms and their associated threads
    JRB roomNode;
    int i = 0;
    jrb_traverse(roomNode, sortRoomNames) {
        rooms[i].roomName = strdup(roomNode->key.s);
        rooms[i].clientList = new_dllist();
        rooms[i].chatList = new_dllist();
        rooms[i].lockRoom = calloc(1, sizeof(pthread_mutex_t));
        rooms[i].roomCond = calloc(1, sizeof(pthread_cond_t));
        pthread_mutex_init(rooms[i].lockRoom, NULL);
        pthread_cond_init(rooms[i].roomCond, NULL);
        if (pthread_create(&chatroomThreads[i], NULL, roomThread, &rooms[i]) != 0) {
            perror("pthread_create_chatroom");
            exit(1);
        }
        i++;
    }

    // Setting up socket and accepting client connections
    int sockets = serve_socket(portUse);
    i = 0;
    while (1) {
        int clientFD = accept_connection(sockets);
        Client chatter = calloc(1, sizeof(struct client));
        chatter->fd = clientFD;
        chatter->roomNum = numRoom;
        chatter->rooms = rooms;
        chatter->clientLock = calloc(1, sizeof(pthread_mutex_t));
        pthread_mutex_init(chatter->clientLock, NULL);
        if (pthread_create(&chatroomThreads[i % numRoom], NULL, clientThread, chatter) != 0) {
            perror("pthread_create_client");
            exit(1);
        }
        i++;
    }
    // Cleanup and exit
    return 0;
}
