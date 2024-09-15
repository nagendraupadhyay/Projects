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


int main(int argc, char* argv[]) 
{
    ifstream ifs(argv[1]);

    map<name_t,scores_t> NS;
    string line;
    string first; 
    string last;
    int num;
    while (getline(ifs, line))
    {
        //add first and lastname to name_t obje
        stringstream ss;
        ss.str(line);
        ss >> first >> last;
        name_t name(first, last);
        scores_t nums;
        while(ss >> num) 
        {
            nums.insert(num);
        }
        nums.insert_done();
        NS.insert(make_pair(name, nums));
    }
    map<name_t, scores_t>::iterator it = NS.begin();
    while(it != NS.end()) 
    {
        auto n = it -> first;
        n.print();
	    it -> second.print();
        it++;
    }
    ifs.close();
}

