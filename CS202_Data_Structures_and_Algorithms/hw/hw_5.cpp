#include <iostream>
#include <vector>
using namespace std;

class stats
{
    private:
        int num;
        int N;
        vector<int> data;
        int sum; 

    public:
        void push(T);
        void print();
};

template <typename T>
T add(const  T &v1)
{
    return v1;
}

template<>
string add(const string &v2)
{
    return v2;
}

template<>
float add(const float &v3)
{
    return v3;
}

void stats::push(T)
{
    data.push_back(num);  
}

void stats::print()
{
    
    sum = T();

    for(int i = 0; i < data.size(); i++)
    {
        sum += data[i];
        N = data.size();
        
        cout << "N   = " << N << endl;
        cout << "sum = " << sum << endl;
    }
}


int main()
{
    int num;
    stats s;

 
    while(cin>>num)
    {
        s.push(num);
    }

    s.print();

    return 0;
} 
