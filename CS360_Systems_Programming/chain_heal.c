//Nagendra Upadhyay CS360 Lab 1.
//The lab is about a game and its main healing character Urgosa. Urgosa is to keep her companions alive with her healing spell. Basically our task was to "heal"
//the players with the help of distance, graphs, and DFS.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>   

#define MAX_NAME_LENGTH 101

//The struct below declares the variables to store the info of all the players
typedef struct node
{
    char name[100];
    int x, y;
    int cur_PP, max_PP;
    struct node *prev;
    int adj_size;
    struct node **adj;
    int visited;
    int healing;
}Node;

//The struct below declares the variables to store the game info and the "damage" and "healling" done to each player.
typedef struct data
{
    int initial_power, num_jumps, initial_range, jump_range;
    double power_reduction;
    int total_healing;
    int best_healing;
    int best_path_length;
    Node **best_path;
    int *healing;
}data;

//The function below calculates the distance between two players using their X and Y coordinates.
double distance(Node *a, Node *b) {
    return sqrt(((b->x - a->x) * (b->x - a->x)) + ((b->y - a->y) * (b->y - a->y)));
}

//Below is the most important DFS function as it keeps track and finds the best path to heal a player.
void DFS(Node *Pdata, data *D, int hopNum, int total_healing, double initialPow, Node *from){
    Pdata->visited = 1;
    Pdata->prev = from;
    double initialPower;
    
    //The if statements below check if hopNum is 1, which refers to Orgosa's position to players
    if(hopNum == 1){
        initialPower = initialPow;
    }else{
        //Reducing the heal ever time a player is healed.
        initialPower = initialPow * (1 - D->power_reduction);
    }

    int healingDone = rint(initialPower);
    //checking if the player does not get over healed
    if(healingDone > Pdata->max_PP - Pdata->cur_PP){
        healingDone = Pdata->max_PP - Pdata->cur_PP;
    }
    Pdata->healing = healingDone;
    
    //The for loop below goes through all the connections between the players
    for(int i = 0; i < Pdata->adj_size; i++){
        //checks again if healing is not over done
        if(healingDone > Pdata->max_PP - Pdata->cur_PP){
            healingDone = Pdata->max_PP - Pdata->cur_PP;
        }
        //checks if the player is visited, if not then it calls DFS on that player
        if(Pdata->adj[i]->visited == 0){
            if(hopNum < D->num_jumps){
                DFS(Pdata->adj[i] , D, hopNum + 1, total_healing + healingDone, initialPower, Pdata);
            }
        }
    }
    Pdata->visited = 0;
    
    //the if statement below looks for the best path and best heal done
    if (total_healing + healingDone > D->best_healing){
        D->best_healing = total_healing + healingDone;
        Node *temp = Pdata;
        D->best_path_length = 0;
        //The while loop goes through all the data and looks for the length of the paths to players
        while (temp != NULL){
            D->best_path_length++;
            temp = temp->prev;
        }
        D->best_path = (Node **)malloc(sizeof(Node *) * D->best_path_length);
        temp = Pdata;
        //The for loops below go through all the players and finds the best path and heals them.
        for (int i = D->best_path_length - 1; i >= 0; i--){
            D->best_path[i] = temp;
            temp = temp->prev;
        }
        D->healing = (int *)malloc(sizeof(int) * D->best_path_length);
        temp = Pdata;
        for (int i = D->best_path_length - 1; i >= 0; i--){
            D->healing[i] = temp->healing;
            temp = temp->prev;
        }
    }
    return;
}

int main(int argc, char *argv[])
{
    //Command line arguments
    data *D = (data *)malloc(sizeof(data));
    D->initial_range = atoi(argv[1]);
    D->jump_range = atoi(argv[2]);
    D->num_jumps = atoi(argv[3]);
    D->initial_power = atoi(argv[4]);
    D->power_reduction = atof(argv[5]);
    
    int x, y, cur_PP, max_PP;
    char name[100];

    int playerCount = 1;
    
    //The scanf below stores Orgosa's info as she is the first player in the input
    scanf("%d%d%d%d%s", &x, &y, &cur_PP, &max_PP, name);
    Node *mainP = (Node *)malloc(sizeof(Node));
    //mainP->name = (char *)malloc(sizeof(char)*100);
    strcpy(mainP->name, name);
    mainP->x = x;
    mainP->y = y;
    mainP->cur_PP = cur_PP;
    mainP->max_PP = max_PP;
    mainP->prev = NULL;
    mainP->visited = 0;
    Node *prev = mainP;
    
    //The while loop below goes through all the player after Orgosa and stores all their information
    while (scanf("%d%d%d%d%s", &x, &y, &cur_PP, &max_PP, name) != EOF) {
        Node *player = (Node *)malloc(sizeof(Node));
        strcpy(player->name, name);
        player->x = x;
        player->y = y;
        player->cur_PP = cur_PP;
        player->max_PP = max_PP;
        player->prev = prev;
        player->visited = 0;
        prev = player;
        //The counter below keeps track of number of players
        playerCount++;
    }
    
    //Below enough memory is made to store all the data of the players to Pdata
    int counter = playerCount - 1;
    Node **Pdata = malloc(sizeof(Node*) * (playerCount));
    while(counter >= 0){
        Pdata[counter] = prev;
        prev = prev->prev;
        counter--;
    }

    //The three for loops below are to find connections between players and also set up enough space to store them
    for(int i = 0; i < playerCount; i++){
        for(int j = 0; j < playerCount; j++){
            if(i == j){
                continue;
            }
            //The if statement below checks if the distance between players is in the jump range to move to the nest player
            if(distance(Pdata[i],Pdata[j]) <= D->jump_range){
                //It also calculates the adj size
                Pdata[i]->adj_size++;
            }
        }
    }
    
    //The for loop below makes new memory to store the connections one after another
    for(int i = 0; i < playerCount; i++){
        Pdata[i]->adj = malloc(Pdata[i]->adj_size * sizeof(struct node*));
    }
    
    for(int i = 0; i < playerCount; i++){
        int friends = 0;
        for(int j = 0; j < playerCount; j++){
            if(i == j){
                continue;
            }
            //The if sttaement below checks for the possible distance in a range to another player
            if(distance(Pdata[i],Pdata[j]) <= D->jump_range){
                Pdata[i]->adj[friends++] = Pdata[j];
            }
        }
    }
    
    int hop_num = 0;
    hop_num++;
    
    //The for loop below goes through all the players and call DfS to find the paths
    for(int i = 0; i < playerCount; i++){
        if(distance(Pdata[i],Pdata[0]) <= D->initial_range){
            DFS(Pdata[i], D, hop_num, 0, D->initial_power, NULL);
        }
    }
    
    //The for loop below prints out all the names with the amount they were healed
    for(int i = 0; i < D->best_path_length; i++){
        printf("%s %d\n", D->best_path[i]->name, D->healing[i]);
    }

    //The statement below prints the total amount of healing that happened to each player
    printf("Total_Healing %d\n", D->best_healing);
    return 0;
}
