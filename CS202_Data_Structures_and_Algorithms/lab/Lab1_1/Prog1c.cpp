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
        int max;
        int min;     

    public:
        void push(int);
        void print();
};

void stats::push(int num)
{
    data.push_back(num);  
}

void stats::print()
{
    
    int max = data[0];
    int min = data[0];
    sum = 0;

    for(int i = 0; i < data.size(); i++)
    {
        sum += data[i];

        if(max < data[i])
        {
            max = data[i];
        }
        if(min > data[i])
        {
            min = data[i];
        }
    }

    N = data.size();
        
    cout << "N   = " << N << endl;
    cout << "sum = " << sum << endl;
    cout << "min = " << min << endl;
    cout << "max = " << max << endl;
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
