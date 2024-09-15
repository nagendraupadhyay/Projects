#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <cstring>
#include <fstream>
#include <algorithm>

using namespace std;

class data {
  public:
    //Made a boolean operator< to campare with the pivot.
    friend bool operator< (const data& A, const data& B ){
        if(A.lastname == B.lastname){
            if(A.firstname==B.firstname)
                return A.phonenum<B.phonenum;
            else
                return A.firstname<B.firstname;
        }
        else 
           return A.lastname < B.lastname;
    }

    friend istream & operator>>(istream &, data &);
    friend ostream & operator<<(ostream &, const data &);

  private:
    string firstname;
    string lastname;
    string phonenum;
};

istream & operator>>(istream &in, data &r) { 
    //extracting firstname, lastname, and phonenum from the input files.
    in >> r.firstname;
    in >> r.lastname;
    in >> r.phonenum;
    
    return in;
}

ostream & operator<<(ostream &out, const data &r) {
    //printing out the output by changing the order to lastname, firstname, and phonenum.
    out << left << r.lastname << ", " << setw(23-r.lastname.length()) <<  r.firstname << " " << setw(12) << right << r.phonenum;
}

template <typename Tdata>
void quicksort(vector<Tdata> &A, int left, int right) { 
    //Checking that the right is always greater than left, the reason is because the pivot will be sent to the right most element of the vector.
    if(left < right){
        int piv = left + rand() % (right-left+1);
        int i = left-1;
        int j = right;
        Tdata pivot = A[piv];
        swap(A[piv], A[right]);
        //Runnig through all the elements using i and j. i goes to the right of the vector and j goes to the left of the vector.
        while(1){
            while(A[++i] < pivot){
            }
            //also adding if statements to check for j to not run over the i. j cannot be an element less than A[0].
            while(pivot < A[--j]){
                if(j<=i){break;}
            }
            if(i>=j){
                break;
            }
            swap(A[i],A[j]);
        }
        //getting i as the result of partition loop.
        piv = i;
        //swaping the elements in order and running the code on recursion.
        swap(A[piv], A[right]);
        quicksort(A, left, piv-1);
        quicksort(A, piv+1, right);
    }
}

template <typename Tdata>
void quickselect(vector<Tdata> &A, int left, int k, int right) {
    //Same process and concept as quicksort..
    while(left<right){
        int piv = left + rand() % (right-left+1);
        int i = left-1;
        int j = right;
        Tdata pivot = A[piv];
        swap(A[piv], A[right]);
 
        while(1){
            while(A[++i] < pivot){
                if(j<=i){break;}
                }
            while(pivot < A[--j]){
                if(j<=i){break;}
                }
            if(i>=j){
                break;
            }
            swap(A[i],A[j]);
        }
        //getting i as the result of partition loop.
        piv = i;
        swap(A[piv], A[right]);
        if(piv == k)
            return;

        if(k < piv) right = piv-1;
        else        left = piv+1;
    }
}

template <typename Tdata>
void printlist(Tdata p0, Tdata p1) { 
    //printing the list out.
    for(;p0 != p1; p0++)
    {
        cout << (*p0) << endl;
    }
}

int main(int argc, char *argv[]) {
    //Command line arguments
    ifstream fin;
    //Strings to store the input_file name and kind of sorting.
    string input_file;
    string sorting;
    //setting if statements to make sure to correctly open the input file because of different argv values.
    if(argc==5)
    {
        input_file = argv[4];
    }
    if(argc==3)
    {
        input_file = argv[2];
    }

    fin.open(input_file);

    //Checking the strings and setting them equal to the specific sort name.
    if(strcmp(argv[1], "-stl") == 0){
        sorting = "-stl";
    }else if(strcmp(argv[1], "-rpivot") == 0){
        sorting = "-rpivot";
    }
        
    vector<data> A;

    data din;
    while (fin >> din)
        A.push_back(din);

    fin.close();
    //if-else statements to make sure to sort data correctly.
    if(sorting ==  "-stl") {
        sort(A.begin(), A.end());
    }else if (sorting == "-rpivot") {
        int N,k0,k1;
        //the statement checks the argc and sorts the input file with quicksort
        if(argc==3){
            N = (int)A.size();
            k0 = 0;
            k1 = N-1;
            quicksort(A, k0, k1);
        }else if(argc==5){
            //the statement extracts the number of input of k0 and k1 and make the code run for the quickselect.
            N = (int)A.size();
            k0 = atoi(argv[2]);
            k1 = atoi(argv[3]);
            if(k0 < 0){
                k0=0;
            }
            if(k1 >= N){
                k1 = N-1;
            }
            quickselect(A,0,k0,N-1);
            quickselect(A,k0,k1,N-1);
            quicksort(A, k0, k1);
        }
    }
    //the print statement that goes through the stored vector and prints.
    printlist(A.begin(), A.end());
}
