#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <stdlib.h> 

using namespace std;

class isprime {
  public:
    isprime() {plist.push_back(2);}
    bool operator()(int);

  private:
    void expand_plist(int);
    vector<int> plist;
};

bool isprime::operator()(int number)
{
    //vector<int>::iterator pnumber;
    bool pnumber = true;//

    if(number <= plist[plist.size() - 1])
    {
        pnumber = binary_search(plist.begin(), plist.end(), number);
    }

    if(number > plist[plist.size() - 1])
    {
        expand_plist(number);
        pnumber = binary_search(plist.begin(), plist.end(), number);
    }

    if(pnumber == false)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void isprime::expand_plist(int number)
{
    int E = plist[plist.size() - 1] + 1;
    bool primeN = true;

    while(plist.back()<number)
    {
        for(int j = 2; j <= sqrt(E); j++)// < sqrt(E)
        {
            if(E%j == 0)
            {
                primeN = false;
            }
        }
        if(primeN == true)
        {
            plist.push_back(E);
        }
        primeN = true;
        E++;
    }
}

int random_number()
{
    return  1 + (rand() % 202);
}

int main(int argc, char *argv[])
{
    int N;
    if(argc>1)
    {
        N = atoi(argv[1]);
    }
    else
    {
        N = 202;
    }

    srand(N);

    vector<int> generateNumbers;
    vector<bool> transformNumbers;
    
    generateNumbers.resize(N);
    transformNumbers.resize(N);

    generate(generateNumbers.begin(), generateNumbers.end(), random_number);
    transform(generateNumbers.begin(), generateNumbers.end(), transformNumbers.begin(), isprime());
    int c = count(transformNumbers.begin(), transformNumbers.end(), true);
    
    cout <<  "Sequence contains " << c << " prime numbers." << endl;
}
