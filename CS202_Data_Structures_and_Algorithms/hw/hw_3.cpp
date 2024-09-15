#include <iostream>
#include <iomanip>

using namespace std;

class list 
{
    struct node 
    {
        node (int data =0);

        int data;
        node *next;
        node *prev;
    };

    public:
        list();
        ~list();

    private:
        int N;
        node *head;
        node *findnode(int);
};

list::node *list::findnode(int i)
{
    if(i<(N/2))
    {
        if(i == -1)
            return head;
            
        node *p = head->next;
        while(i--)
            p = p->next;
            
        return p;
    }
    else
    {
        if(i == -1)
            return head;

        node *p = head->prev;
        while(i++)
            p = p->prev;

        return p;
    }
}
