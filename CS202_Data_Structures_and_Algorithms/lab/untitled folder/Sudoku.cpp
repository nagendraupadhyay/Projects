#include <sys/time.h>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
using namespace std;

class omega {
    public:
        omega();
        float get_sec();

    private:
        long Tinit;
        long Tmark;
};

omega::omega() {
    struct timeval T;
    gettimeofday(&T, NULL);

    Tinit = 1000000*T.tv_sec + T.tv_usec;
}

float omega::get_sec() {
    struct timeval T;
    gettimeofday(&T, NULL);

    Tmark = 1000000*T.tv_sec + T.tv_usec;

    return (float)(Tmark-Tinit)/1000000.0;
}

string elapsed(int elasped_time, int k = 0) 
{
    char time[40];

    if(elasped_time > 0.1)
    {
        if(k == 0)
        {
            sprintf(time, "%d seconds", elasped_time);
            return time;
        }
        if(k == 1)
        {
            sprintf(time, "%d miliseconds", elasped_time);
            return time;
        }
        if(k == 2)
        {
            sprintf(time, "%d microseconds", elasped_time);
            return time;
        }
    }
    return elapsed(elasped_time * 1000, k + 1);
}

#define ROW_INUSE    0x1
#define COLUMN_INUSE 0x2
#define SQUARE_INUSE 0x4

class sudoku {
    struct cell {
        cell(int _i = 0, int _j = 0){
            i = _i;
            j = _j;
        }
        int i;
        int j;
    };

    public:
        sudoku();

        void solve();
        void test_error_check();

        void read(const char *);
        void write(const char *);
        void write(const char *, const char *);

        bool error_check(int);//////////////just for testing

    private:
        bool solve(int);

        void set_inuse(int, int, int);
        void unset_inuse(int, int, int);

        void get_values(int, int, vector<int> &);

        void display();
        //bool error_check(int);

        int game[9][9];//This is the actual board, containing the values of the cells.
        int inuse[9][10];//This will be set by your set_inuse and unset_inuse functions.
};

sudoku::sudoku() { 
    for (int i=0; i<9; i++) {
        for (int j=0; j<9; j++)
            game[i][j] = 0;

        for (int j=0; j<10; j++)
            inuse[i][j] = 0;
    }
}

void sudoku::solve()
{
    error_check(0);
    cout << "SOLVE\n";
    
    if(error_check(0))
    {
        cout << " ";
        exit(0);
    }
    
    solve();
    
    if(error_check(1))
    {
        cout << " ";
        exit(0);
    }
    display();
}

bool sudoku::solve(int k) 
{
    vector<int> tempList;
    vector<int> minValue;
    int size = 10;
    int ii;
    int jj;
    int minK = -1;
    
    if(k == 81)
    {
        return true;
    }
    
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            get_values(i, j, tempList);

            if(size > tempList.size())
            {
                size = tempList.size();
                ii = i;
                ii = j;
                minK = k;
                minValue = tempList;
            }
            else if(tempList.size() == size)
            {
                if(tempList.at(0) < minValue.at(0))
                {
                    ii = i;
                    jj = j;
                    minK = k;
                    minValue = tempList;
                }
            }
        }
        if(minK == -1)
        {
            return false;
        }
    }   
    for(int i = 0; i < minValue.size(); i++)
    {
        set_inuse(ii, jj, tempList.at(i));
        if(solve(k+1))
        {
            return true;
        }
        unset_inuse(ii, jj, tempList.at(i));
    }
    return false;
}

void sudoku::read(const char *fname) 
{
    cout << "READ\n";

    ifstream fin(fname);

    int i, j, value;

    int line = 1;
    bool error = false;

    while(fin >> i >> j >> value) 
    {
        if((i < 0 || i >= 9) || (j < 0 || j >= 9) || (value < 0 || value > 9))
        {
            cout << "line " << line << ": " << i << " " << j << " " << value  << " invalid index" << endl;
            error = true;
        }
        else
        {
            set_inuse(i, j, value);
        }
        line++;
    }
    fin.close();

    if (error == true)
    {
        exit(0);
    }

    display();
}

void sudoku::write(const char *fname) {
    ofstream fout(fname);

    for (int i=0; i<9; i++) 
    {
        for (int j=0; j<9; j++) 
        {
            if (0 < game[i][j]) 
            {
                fout << i << " "
                    << j << " "
                    << game[i][j] << "\n";
            }
        }
    }

    fout.close();
}

void sudoku::write(const char *fname, const char *addon) {
    int N1 = strlen(fname);
    int N2 = strlen(addon);

    char *n_fname = new char[N1+N2+2];

    // strip .txt suffix, then concatenate _addon.txt
    strncpy(n_fname, fname, N1-4);
    strcpy(n_fname+N1-4, "_");
    strcpy(n_fname+N1-3, addon);
    strcpy(n_fname+N1-3+N2, ".txt");

    write(n_fname);

    delete [] n_fname;
}

int square_index(int i,int j)
{
    return 3*(i/3)+(j/3);
}

void sudoku::set_inuse(int i, int j, int value) {
    game[i][j] = value;
    int k = square_index(i, j);
    
    inuse[i][value] |= ROW_INUSE;
    inuse[j][value] |= COLUMN_INUSE;
    inuse[k][value] |= SQUARE_INUSE;
}

void sudoku::unset_inuse(int i, int j, int value) {
    game[i][j] = 0;
    int k = square_index(i, j);
    
    inuse[i][value] &= ~ROW_INUSE;
    inuse[j][value] &= ~COLUMN_INUSE;
    inuse[k][value] &= ~SQUARE_INUSE;
}

void sudoku::get_values(int i, int j, vector<int> &value_list) 
{
    int k = square_index(i, j);
    value_list.clear();
    for(int value = 1; value < 9; value++)
    {
        if (!(inuse[i][value] & ROW_INUSE || inuse[j][value] & COLUMN_INUSE || inuse[k][value] & SQUARE_INUSE)) 
        {
            value_list.push_back(value);
        }
    }
}

void sudoku::test_error_check() 
{
    error_check(1);
}

bool sudoku::error_check(int mode) 
{
    int value;

    vector<int> freqCount(10,0);

    if(mode == 0)
    {
        for(int row = 0; row < 9; row++)
        {
            for(int col = 0; col < 9; col++ )
            {
                value = game[row][col];
                freqCount[value]++;
            }
            for(int col = 1; col < 10; col++)
            {
                if(freqCount[col] > 1)
                {
                    cout << "row[" << row  << "]: " << col  << " seen " << freqCount[col]  << " times" << endl;
                }
            }
            freqCount.assign(10,0);
        }
        for(int col = 0; col < 9; col++)
        {
            for(int row = 0; row < 9; row++ )
            {
                value = game[row][col];
                freqCount[value]++;
            }
            for(int row = 1; row < 10; row++)
            { 
                if(freqCount[row] > 1)
                {
                    cout << "column[" << col  << "]: " << row  << " seen " << freqCount[row]  << " times" << endl;
                }
            }
            freqCount.assign(10,0);
        }

        for(int row = 0; row < 9; row+=3)
        {
            for(int col = 0; col < 9; col+=3)
            {
                for(int m = 0; m < 3; m++)
                {
                    for(int n = 0; n < 3; n++)
                    {
                        value = game[row+m][col+n];
                        freqCount[value]++;
                    }
                }
                
                for(int value = 1; value < 10; value++)
                {
                    if(freqCount[value] > 1)
                    {   
                        cout << "square [" << square_index(row,col)  << "]: " << value  << " seen " << freqCount[value] << " times" << endl;
                    }
                }
                freqCount.assign(10,0);
            }
        }
    }

    if(mode == 1)
    {
        for(int row = 0; row < 9; row++)
        {
            for(int col = 0; col < 9; col++ )
            {
                value = game[row][col];
                freqCount[value]++;
            }
            if(freqCount.at(0) > 0)
            {
                return true;
            }
            for(int col = 1; col < 10; col++)
            {
                if(freqCount[col] > 1)
                {
                    cout << "row[" << row  << "]: " << col  << " seen " << freqCount[col]  << " times" << endl;
                }
            }
            freqCount.assign(10,0);
        }
        
        for(int col = 0; col < 9; col++)
        {
            for(int row = 0; row < 9; row++ )
            {
                value = game[row][col];
                freqCount[value]++;
            }
            if(freqCount.at(0) > 0)
            {
                return true;
            }
            for(int row = 1; row < 10; row++)
            { 
                if(freqCount[row] > 1)
                {
                    cout << "column[" << col  << "]: " << row  << " seen " << freqCount[row]  << " times" << endl;
                }
            }
            freqCount.assign(10,0);
        }

        for(int row = 0; row < 9; row+=3)
        {
            for(int col = 0; col < 9; col+=3) 
            {
                for(int m = 0; m < 3; m++) 
                {
                    for(int n = 0; n < 3; n++)
                    {
                        value = game[row+m][col+n];
                        freqCount[value]++;
                    }
                }
                for(int value = 1; value < 10; value++)
                {
                    if(freqCount[value] > 1)
                    {
                        cout << "square [" << square_index(row,col)  << "]: " << value  << " seen " << freqCount[row]  << " times" << endl;
                    }
                }     
                if(freqCount.at(0) > 0)
                {
                    return true;
                }
            }
        }
    }
}
    void sudoku::display() {
        cout << "| --------------------------- |\n";
        for (int i=0; i<9; i++) {
            for (int j=0; j<9; j++) {
                if (j == 0) 
                    cout << "| " << game[i][j];
                else if (j%3 == 0) 
                    cout << " | " << game[i][j];
                else
                    cout << "  " << game[i][j];
            }
            cout << " |\n";
            if (i%3 == 2)
                cout << "| --------------------------- |\n";
        }
    }

    int main(int argc, char *argv[]) {
        srand(time(NULL));

        omega timer;
        sudoku sudoku_game;

        sudoku_game.read(argv[2]);

        float T0 = timer.get_sec();
        sudoku_game.solve();
        float T1 = timer.get_sec();

        sudoku_game.write(argv[1], "solved");

        string elapsed_time = elapsed(T1-T0, 0);
        cout << "Time: " << elapsed_time << " " << fixed << setprecision(8) << "(" << T1-T0 << " sec)" << endl;
    }
