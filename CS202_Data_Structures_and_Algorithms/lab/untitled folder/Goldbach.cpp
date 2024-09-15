#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <set>
using namespace std;

class prime_partition {
    public:
        prime_partition(){create_pset(20000);}
        void operator()(int number);

    private:
        void create_pset(int i);
        bool find_partition(int, int);

        set<int> pset;
        int pset_min = 2;
        int pset_max = 2;
        int max_terms;
        vector<int> partition;
};

void prime_partition::create_pset(int n)
{
    bool primeN = true;
    int i;

    for(i = 3; i < 20000; i++)
    {    
        for(int j = 2; j <= sqrt(i); j++)
        {
            if(i%j == 0)
            {
                primeN = false;
            }
        }
    
        if(primeN == true)
        {
            pset.insert(i);
            pset_max = i;
        }
        primeN = true;
    }
}


void prime_partition::operator()(int n)
{
    if(n <= pset_min || n >= pset_max) 
    {
        return;
    }

    if(n%2 == 0)
    {
        max_terms = 2;
    }
    else
    {
        max_terms = 3;
    }
    partition.clear();
    find_partition(n, 0);

    for(uint j = 0; j < partition.size(); j++)
    {
        cout << partition[j]  << " ";
    }
    cout << endl; 
}

bool prime_partition::find_partition(int n, int terms)
{
    if(terms > max_terms)
    {
        return false;
    }
    if(!(is_sorted(partition.begin(), partition.end())))
    {
        return false;
    }
    if(n == 0)
    {
        return true;
    }

    set<int>::iterator start_it = pset.lower_bound((int)(sqrt(n)));
    set<int>::iterator end_it = pset.upper_bound(n);
    
    for(auto i  = start_it; i != end_it; i++)
    {
        partition.push_back(*i);
        
        if(find_partition(n - *i, partition.size()) == true)
        {
            return true;
        }
        else
        {
            partition.pop_back();
        }
    }

    return false;
}

int main(int argc, char *argv[])
{
    prime_partition goldbach;

    int number;
    while (1) {
	    cout << "number = ";
        cin >> number;
	    if (cin.eof())
	    break;
	
	    goldbach(number);
    }

    cout << "\n";
}
