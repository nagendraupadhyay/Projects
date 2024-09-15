#include<iostream>

class list
{
    struct node{
        node (int data=0);
        int data;
        node *next;
    }
    public:
        void push_front(const int &);
        void pop_front();
        const int &front();

    private:
        int N;
        node *head;
}


void list::push_front(const int & din)
{
    node *p = new node(din);
    p->next = head->next;
    head->next = p;
    N++;
}

void list::pop_front()
{
    if (head->next = NULL)
    {
        return;
    }
    node *p;
    p = head->next;
    head->next = p->next;
    delete p;
    N--;
}

const int & list::front()
{
    node *p;
    return p->head;
}
