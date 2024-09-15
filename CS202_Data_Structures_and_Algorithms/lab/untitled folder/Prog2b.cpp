#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

const string face[] = { "Ace", "2", "3", "4", "5", "6", "7",
                        "8", "9", "10", "Jack", "Queen", "King" }; 
const string suit[] = { "Diamonds", "Hearts", "Clubs", "Spades" };

class list
{
    struct node
    {
        node (int d)
        {
            data = d;
            next = NULL;
        }
        int data;
        node *next;
        node *prev;
    };

    public:
        list();
        ~list();
        void insert(int);
        void clear(); 

        friend ostream & operator<< (ostream  &, const list &);

    private:
        node* head;
        int N;
};

list::list()
{
    head = new node(-1);
    N = 0;
}

list::~list()
{
    node *nextNode = head->next ;
    node *prevNode = head->next;
    while(nextNode != NULL)
    {
        nextNode = nextNode->next;
        delete prevNode;
        prevNode = nextNode;
    }
    delete head;
}

ostream & operator << (ostream &out, const list &l)
{
    list::node *A = l.head->next;
    
    while(A != NULL)
    {
        out << face[A->data] << " ";
        A = A->next;
       
    }
    return out;
}

void list::insert(int index)
{
    node *curr = head->next;
    node *pervi = head;
    while(curr != NULL)
    {
        if(curr->data == index)
        {
            break;
        }
        curr = curr->next;
    }
    if(curr != NULL)
    {
        pervi = curr->next;
        if(pervi == NULL)
        {
            return;
        }
        head->next = pervi;
    }
    else
    {
        curr = new node(index);
    }
    while(pervi->next != NULL)
    {
        pervi = pervi->next;
    }
    pervi->next = curr;
    curr->next = NULL;
}

string random_card(bool verbose=false) 
{
	string card;

    card = face[ rand()%13 ];
	card += " of ";
	card += suit[ rand()%4 ];

	if (verbose)
	  cout << card << "\n";

	return card;
}

int main(int argc, char *argv[])
{
	bool verbose = false;
	int seedvalue = 0;
    //Strings made to help compare the string to the cards drawn.
    string store_suit;
    string store_face;

	for (int i=1; i<argc; i++) 
    {
	    string option = argv[i];
	    if (option.compare(0,6,"-seed=") == 0) 
        {
	        seedvalue = atoi(&argv[i][6]);
	    } 
        else if (option.compare("-verbose") == 0) 
        {
	        verbose = true;
	    } 
        else
        { 
	        cout << "option " << argv[i] << " ignored\n";
        }
	}
    srand(seedvalue);
    

    //Making a table to store the data of the cards. Made a boolean so it can check if any of the suit has finished getting all 13 cards.
    int count[4][13] = {0};
    list deck[4];//each list corresponds to a different suit, where 
    
    int N = 0;
    int M = 0;
    int i;
    int j;
    bool keepDealing = true;

	while (keepDealing) 
    {
	    //Code for storing data in strings for the random cards drawn from the deck.
        string card = random_card(verbose);
	    stringstream storingData(card);
        storingData >> store_face >> store_suit >> store_suit;
        
        //This for loop goes through all 13 cards of each and ever suit.
        for(i = 0; i < 13; i++)
        {
            //This if statement compares the cards drawn and also stores the no. of times it got the same card.
            if(face[i] == store_face)
            {
                N = i;
                break;
            }
        }
        //loop goes 4 times and compares each suit of the card and stores the no. of the same cards drawn.
        for(j = 0; j < 4; j++)
        {
            if(suit[j] == store_suit)
            {
                M = j;
                break;
            }
        }
        
        //Declaring a table and setting the boolean equal to false so it can help while loop stop if needed.
        //Updating the table every time a card is drawn.
        count[M][N]++;
        deck[M].insert(N);
        keepDealing = false;

        //The loop below checks if any of the 13 places in a row has a zero. If so then it keeps running.
        for (i=0; i < 13; i++)
        {
            if(count[M][i] == 0)
            {
                keepDealing = true;
            }
        }
	}

    //Loop made to ouput the data stored in the table.
    for(int suit_index = 0; suit_index < 4; suit_index++)
    {
        cout << setw(8) << suit[suit_index] << " : " << deck[suit_index] << endl;
        if(suit_index == M)
        {
            cout << " **";
        }
    }
   cout << endl;
}
