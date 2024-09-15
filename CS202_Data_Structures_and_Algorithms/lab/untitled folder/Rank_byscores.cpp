#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <set>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <map>
#include <numeric>
using namespace std;

class name_t {
    public:
        name_t(string, string);
  
        bool operator<(name_t rhs) const;
        void print();
  
    private:
        static int w;
        string name;
};
  
    int name_t::w = 0;
 
class scores_t {
    public:
        scores_t();
  
        void insert(int);
        void insert_done();
        void print();
 
    private:
        vector<int> scores;
        float mean;
};

class namescores_t {
  public:
    namescores_t();

    bool operator<(namescores_t rhs) const;
    
    void print_name();
    void print_scores();

  private:
    name_t name;
    scores_t scores;
};

name_t::name_t(string first, string last)
{
    name = last + ", " + first;
    if(name.size() > w)
    {
        w = name.size();
    }
}
 
bool name_t::operator<(name_t rsh) const
{
    if(name < rsh.name)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void name_t::print()
{
    cout.fill('.');
    cout <<  setw(w+3) << left << name;
    cout.fill(' ');
}
 
scores_t::scores_t()
{
    mean = 0;
}
 
void scores_t::insert(int num)
{
    scores.push_back(num);
}
 
void scores_t::insert_done()
{
    mean = std::accumulate(scores.begin(), scores.end(), 0);
    mean = mean/scores.size();
}

void scores_t::print()
{
    for(int i = 0; i < scores.size(); i++)
    {
        cout << " " << scores[i] << setw(2);
    }
        cout << ": " << setprecision(1) << fixed << mean << endl;
}

namescores_t::namescores_t(name_t n, scores_t s)
{
    name = n;
    scores = s;
}

bool scores_t::operator<(scores_t rhs) const
{
    if(mean < rhs.mean)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool namescores_t::operator<(namescores_t rhs) const
{
    if(scores < rhs.scores)
    {
        return true;
    }
    else if(rhs.scores < scores)
    {
        return false;
    }
    else
    {
        if(name < rhs.name)
        {
            return true;
        }
    }
}

void namescores_t::print_name()
{
    name.print();
}

void namescores_t::print_scores()
{
    scores.print();
}


int main(int argc, char* argv[])
{
    ifstream ifs(argv[2]);
    //argv[1]: K (number of records to print)

    string line;
    string first;
    string last;
    int num;
  
    vector<namescores_t> NS;

    while (getline(ifs,line)) 
    {
    //add first and lastname to name_t object
        stringstream ss;
        ss.str(line);
        ss >> first >> last;
        name_t name(first, last);
        scores_t nums;
        
        while (ss >> num) 
        {
            nums.insert(num);
        }   
        
        nums.insert_done();
        namescores_t nscores(name, nums);
        NS.push_back(nscores);
    }
///
    make_heap(NS.begin(), NS.end());

    //for (K data elements && heap not empty)
    {
        if(NS.empty())
        {
            return 0;
        }
        
        namescores_t extract = NS.front();
        extract.print_name();
        extract.print_scores();

        pop_heap(NS.begin(), NS.end());
        NS.pop_back();
    }
    ifs.close();
}
