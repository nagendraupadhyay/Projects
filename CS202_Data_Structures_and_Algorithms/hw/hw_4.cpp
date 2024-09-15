#include <iostream>

class stack
{
    public:
        bool empty() const {};
        int size() const {};

        stack();
        ~stack();
        void empty();
        void size();
        void push(const data_t &din);
        void pop();
        const data_type &top();

    private:
        list v;
        int data[] = 10;
};

stack::stack()
{

}

stack::~stack()
{

}

void stack::empty()
{
    return v.empty();
}

void stack::size()
{
    return v.size();
}

void stack::push()
{
    v.push_front(din);
}

void stack::pop()
{
    v.pop_front();
}

void stack::top()
{
    return v.front();
}
