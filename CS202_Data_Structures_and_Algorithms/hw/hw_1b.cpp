#include<iostream>
#include<cstdio>

using namespace std;

int strlen(char *str)
{
    int size = 0;
    while(*str != '\0')
    {
        size += 1 ;
        str += 1;
    }
    return size;
}

int main(int argc, char *argv[])
{
    int N = argc;
    int X[N];
    
    cout << "Num args = " << argc << endl;

    for (int i=0; i < N; i++)
    cout << "argv" << i << "] = " << &X[i]  << " " << argv[i] << " (strlen=" << strlen(argv[i]) << ")" <<  "\n";
}
