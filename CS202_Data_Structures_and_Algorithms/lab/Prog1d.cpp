#include<iostream>
#include<string>
#include<stdio.h>
#include<ctype.h>

using namespace std;

void encode(string& encoding,int shift)
{
    for(int i = 0; i < encoding.size(); i++)
    {
        if(isalpha(encoding[i]))
        {
            int IntVal;
            if(isupper(encoding[i]))
            {
                IntVal = encoding[i] - 'A';
                IntVal = (IntVal + shift) % 26;
                encoding[i] = IntVal + 'A';
            }
            else//islower
            {
                IntVal = encoding[i] - 'a';
                IntVal = (IntVal + shift) % 26;
                encoding[i] = IntVal + 'a';
            }
        }
    }
        
}
void decode(string& decoding,int shift)
{
    for(int i = 0; i < decoding.size(); i++)
    {
        if(isalpha(decoding[i]))
        {
            int IntVal;
            if(isupper(decoding[i]))
            {
                IntVal = decoding[i] - 'A';
                IntVal = (IntVal - shift) % 26;
                if(IntVal < 0)
                {
                    IntVal = IntVal + 26;
                }
                decoding[i] = IntVal + 'A';
            }
            else//islower
            {
                IntVal = decoding[i] - 'a'; 
                IntVal = (IntVal - shift) % 26;
                if(IntVal < 0)
                {
                    IntVal = IntVal + 26;
                }
                decoding[i] = IntVal + 'a';
            }
        }
    }
}

int main(int argc, char *argv[])
{
    if(argc < 3)
    {
        return 0;
    }
    
    string data = argv[1];
    
    if(!(data=="-encode" || data=="-decode"))
    {
        return 0;
    }

    if(atoi(argv[2]) < 0 || atoi(argv[2]) > 9)
    {
        return 0;
    }
    
    int shift = atoi(argv[2]);

    if(data=="-encode")
    {
        while(getline(cin,data))
        {
            encode(data, shift);
            cout << data << endl;
        }
    }
    else if(data=="-decode")
    {
        while(getline(cin,data))
        {
            decode(data, shift);
            cout << data << endl;
        }
    }
    else
    {
       cerr << "Error Detected" << endl;
       return -1;
    }
    return 0;
}
