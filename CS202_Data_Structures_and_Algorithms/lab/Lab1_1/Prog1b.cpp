#include <iostream>
#include <vector>
using namespace std;

int main()
{
    int num;
    vector<int> data;
    int N;
    int sum = 0;
    
    while(cin>>num)
    {
        data.push_back(num);
    }
    
    int max = data[0];
    int min = data[0];

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
    
    return 0;
}
