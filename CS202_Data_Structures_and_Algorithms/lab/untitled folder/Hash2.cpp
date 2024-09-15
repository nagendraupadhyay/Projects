#include <cstring>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <utility>
#include <iomanip>
#include <string>
#include <cmath>

using namespace std;

typedef unsigned int uint;
typedef unordered_multimap<string, int> hashtable;

template <typename T>
void replace_punctuation(T p, T p_end)
{
    for(;p != p_end; p++)
    {
        if((ispunct(*p) && *p != '_'))
        {
             *p = ' ';
        }
    }
}

int main(int argc, char *argv[])
{
    bool verbose = false;
	char *fname = NULL;
    
    fname = argv[2];
     
    if(argc < 2 || argc > 4)
    {
        cout  << "usage: ./HashX [-verbose] -f textfile" << endl;
    }
    for (int i=1; i<argc; i++)
    {
        string option = argv[i];
	    if (option.compare("-verbose") == 0)
        {
	        verbose = true;
	    }
        if (option.compare("-f") == 0)
        {
	        i++;
	    }
	}

    hashtable H;

    if(verbose == true)
    {
        cout << "** N = " << setw(8) << H.bucket_count();
        cout << " load = " << fixed << setprecision(2) << H.load_factor() << endl;
    }

	ifstream fin(fname);

	vector<string> cache;
	string text, word;
    int lines = 0;

    while(getline(fin,text))
    {
        cache.push_back(text);
       
        replace_punctuation(text.begin(), text.end());
       
        stringstream ss(text);
        while(ss>>word)
        {
            H.insert(make_pair(word, lines));
        }
        lines++;
    }
    fin.close();

    if(verbose == true)
    {
        cout << "** N = " << setw(8) << H.bucket_count();
        cout << " load = " << fixed << setprecision(2) << H.load_factor() << endl;
    }
    
    pair<hashtable::iterator, hashtable::iterator> range;

    hashtable::iterator start_it;
	hashtable::iterator end_it;

    while(true)
    {
        cout << "find>";
        cin >> word;
        
        range = H.equal_range(word);
        start_it = range.first;
        end_it = range.second;
        
        while(start_it != end_it)
        {
            cout << start_it->second << ":   " << cache.at(start_it->second) << endl;
            start_it++;
        }

        if(cin.eof())
        {break;}
    }
	return 0;
}
