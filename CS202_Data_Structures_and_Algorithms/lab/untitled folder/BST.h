#ifndef BST2_H
#define BST2_H

#include <iomanip>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

template <typename TKey>
class bst {
  
    struct node {
	    node(int n_ID=0); //node constructor (see below)

		void print(); //print function (TKey version partially written for you, will need to write special string version)

		TKey key; //key of a bst node
		int ID; //id of a bst node

		node *parent; //parent of bst node
		node *link[2]; //you must use link array! link[0] = left child, link[1] = right child
	};

    public:
        class iterator {
            public:
                iterator(){p = NULL;}
                iterator &operator++();
                TKey &operator*() {return p->key;}
                bool operator==(const iterator &rhs) const{return p == rhs.p;}

            private:
                friend class bst<TKey>;
                iterator(node *new_p)
                {
                    p = new_p;
                }
      //  update and set node ID

                node *p;
        };

    iterator begin();
    iterator end();

    public:
        bst() { Troot=NULL; nodeID = 0;}
        ~bst() { clear(Troot); }

        bool empty() { return Troot==NULL; }

        void insert(const TKey &);

        iterator lower_bound(const TKey &);
        iterator upper_bound(const TKey &);

        void print_bylevel();

    private:
        void clear(node *);
        node *insert(node *, const TKey &);

        int nodeID;
        node *Troot;
};

template <typename TKey>
bst<TKey>::node::node(int n_ID)
{
    ID = n_ID;
    parent = NULL;
    link[0] = NULL;
    link[1] = NULL;
}

template <class TKey>
void bst<TKey>::node::print()
{
    cout << setw(3) << ID << " ";
    cout << setw(3) << key << " :";
    
    if(parent)
    {
        cout << " P=" << setw(3) << parent->ID  << " ";
    }
    else{cout << setw(3) << " ROOT  ";}
    
    if (link[0]) cout << " L=" << setw(3) << link[0]->ID;
    else         cout << "      ";
    if (link[1]) cout << " R=" << setw(3) << link[1]->ID;
    else         cout << "      ";

    cout << "\n";
}

template <>
void bst<string>::node::print()
{
    cout << setw(20) << ID << " ";
    cout << setw(20) << key << " :";
    if(parent)
    {
        cout << " P=" << setw(20) << parent->ID << " ";
    }
    else{cout << setw(3) << " ROOT  ";}

    if (link[0]) cout << " L=" << setw(3) << link[0]->ID;
    else         cout << "      ";
    if (link[1]) cout << " R=" << setw(3) << link[1]->ID;
    else         cout << "      ";   
    cout << "\n";
}

template <typename TKey>
class bst<TKey>::iterator bst<TKey>::begin(){
    node *T = Troot;
    
    while(T->link[0] != NULL)
    {
            T = T->link[0];
    }
    return iterator(T);
}

template <typename TKey>
class bst<TKey>::iterator bst<TKey>::end(){
    return iterator(NULL);
}

template <typename TKey>
class bst<TKey>::iterator & bst<TKey>::iterator::operator++()
{
    //STEP 1: go to minimum key in right subtree
	// check p's right link exists
	// set p to its right link
	// loop appropriately to get leftmost element of p
    if(p->link[1])
    {
        p = p->link[1];
        while(p->link[0] != NULL)
        {
            p = p->link[0];
        }
    }
    else if(p->parent && p->parent->link[0] == p)
    {
        p = p->parent;
    }
    else if(p->parent == NULL)
    {
        p = NULL;
    }
    else
    {
        while(p->parent && p->parent->link[1] == p)
        {
            p = p->parent;
        }
        if(p->parent != NULL)
        {
            p = p->parent;
        }
        else
        {
            p = NULL;
        }
    }
	//STEP 2: if leaving left subtree go to parent
	// if p's parent does not exist, set p to NULL
	// otherwise set p to p's parent
	

	//STEP 3: return to (great) grandparent
	// undo sequence of right moves (hint: involves loop)
	// undo sequence of left moves unless done (i.e. p->parent == NULL)

    return *this;
}

template <class TKey>
void bst<TKey>::clear(node *T)
{
    if (T) {
        clear(T->link[0]);
        clear(T->link[1]);
        delete T;
        T = NULL;
    }
}

template <class TKey>
void bst<TKey>::insert(const TKey &key)
{ 
    Troot = insert(Troot, key);
    if(Troot)
        Troot->parent = NULL;
}

template <class TKey>
struct bst<TKey>::node *bst<TKey>::insert(node *T,const TKey &key)
{
    if (T == NULL) {
        T = new node;
        T->key = key;
        T->ID = ++nodeID;
    } else if (T->key == key) {
        ;
    } else if (key < T->key) {
        T->link[0] = insert(T->link[0], key);
        T->link[0]->parent = T;
    } else {
        T->link[1] = insert(T->link[1], key);
        T->link[1]->parent = T;
    }

    return T;
}

template <typename TKey>
class bst<TKey>::iterator bst<TKey>::lower_bound(const TKey &key)
{
    node *tmp = Troot;
    node *save = NULL;
	while(tmp)
    {
        if(key <= tmp->key)
        {
            save = tmp;
            tmp = tmp->link[0];
        }
        else
        {
            tmp = tmp->link[1];
        }
    }
    return iterator(save);
}

template <typename TKey>
class bst<TKey>::iterator bst<TKey>::upper_bound(const TKey &key)
{
    node *tmp = Troot;
    node *save = NULL;
 
	while(tmp)
    {
        if(key < tmp->key)
        {
            save = tmp;
            tmp = tmp->link[0];
        }
        else
        {
            tmp = tmp->link[1];
        }
    }
    return iterator(save);
}

template <class TKey>
void bst<TKey>::print_bylevel()
{
  if (Troot == NULL)
    return;

  queue<node *> Q;
  node *T;

  Q.push(Troot);
  while (!Q.empty()) {
    T = Q.front();
    Q.pop();

    T->print();
    if (T->link[0]) Q.push(T->link[0]);
    if (T->link[1]) Q.push(T->link[1]);
  }
}
#endif
