#include <cstring>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <string>
#include <cmath>

using namespace std;

typedef unsigned int uint;

class hash_table 
{

	struct key_line 
    {
		bool inuse()
        {
            if(key.length() == 0) {return false;} 
            else {return true;}
        }
		bool operator==(const string &target) //return whether the key == target (one line)
        {
            return key==target;
        }

		string key; //key string
		vector <int> line; //vector of line #s where key is found
	};

	public:
		hash_table(); //given below
		~hash_table(){;}

		void insert(const string &, int); //detailed below
		const vector<int> & find(const string &); //detailed below
		void show_load(); //go over after spring break
		void show_stats(); //go over after spring break

	private:
		int hash(const string &);
		int nextprime(int);
		int qprobe(const string &, bool);//still gotta fetch this from handout.
		void resize();

		bool show_load_active; //for printing load
		int num_inuse; //number of keys in use
		int max_inuse; //maximum number of keys that can be in use
		int num_collisions; //number of times we have had to handle a collision

		vector<key_line> table; //the has table
};

hash_table::hash_table()
{
	int N = nextprime(101);
	table.assign(N, key_line());

	show_load_active = false; //this is for after spring break

	num_inuse = 0;
	max_inuse = N/2;

	num_collisions = 0;
}

int hash_table::hash(const string &key)
{
    uint index = 0;
    const char *c = key.c_str();
    while (*c)
    index = ((index << 5) | (index >> 27)) + *c++;
    return index % table.size();
}

int hash_table::qprobe(const string &key, bool check)
{
    int index = hash(key);
    int k = 0;

    
    while (table[index].inuse() && !(table[index] == key))
    {
        if(check == true)
        {
            num_collisions++;
        }
        
        index += 2*(++k) - 1;
        index = index % table.size();
    }
    return index;
}

void hash_table::resize() 
{
    vector<key_line> tmp_table;
    for (int i=0; i<(int)table.size(); i++)
    {
        if (table[i].inuse())
        {
            tmp_table.push_back(table[i]);
        }
    }
    int N = nextprime(table.size());
    table.assign(N, key_line());
    num_inuse = 0;
    max_inuse = N/2;
    for (int i=0; i<(int)tmp_table.size(); i++)
    {
        table[qprobe(tmp_table[i].key, false)] = tmp_table[i];
        num_inuse++;
    }
}

int hash_table::nextprime(int N)
{ 
    N = (2*N) + 1;
    int i = 3;
    while(i*i <= N)
    {
        if(N%i==0)
        {
            N += 2;
            i = 1;
        }
        i += 2;
    }
    return max(3, N);
}


void hash_table::insert(const string &key, int line)
{
    int index = qprobe(key, true);

    if(!table[index].inuse())
    {
        table[index].key = key;
        num_inuse++;
    }

	vector<int> &L = table[index].line; //grab the lines vector at index
	
    if(!binary_search(L.begin(), L.end(), line))
    {
        table[index].line.push_back(line);
    }
    
    if(num_inuse >= max_inuse)
    {  
        if(show_load_active == true)
        {
            show_load();
        }
        resize();
        index = qprobe(key, false);
    }
}
const vector<int> &hash_table::find(const string &key)
{
    int index = qprobe(key, false);
    return table[index].line;
}

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

void hash_table::show_stats()
{
    cout << endl;
    cout << "Num slots used: " << num_inuse << setw(10) << endl;
    cout << "Num usable slots: " << max_inuse << setw(10) << endl;
    cout << "Num collisions: " << num_collisions << setw(10) << endl;
    cout << " " <<endl;
}

void hash_table::show_load()
{
    if(show_load_active == false)
    {
        show_load_active = true;
        return;
    }
    
    cout << "** N =" << setw(8) << table.size() << " load = " << fixed  << setprecision(2) << (double)num_inuse/(double)table.size() << endl;
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

	hash_table H;

    if(verbose == true)
    {
        H.show_load();
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
            H.insert(word, lines);
        }
        lines++;
        
    }
    fin.close();

    if(verbose == true)
    {
        H.show_load();
        H.show_stats();
    }
    while(true)
    {
        cout << "find>";
        cin >> word;

        if(cin.eof())
        {break;}
        
        vector<int> lineNums = H.find(word);
        for(int i=0; i < (int)lineNums.size(); i++)
        {
            cout << lineNums[i] << ":   " << cache[lineNums[i]] << endl;
        }
    }
	return 0;
}
