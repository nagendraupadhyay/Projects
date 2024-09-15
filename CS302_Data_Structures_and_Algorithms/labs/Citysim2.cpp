//Nagendra Upadhyay. This is a lab about graphs that are being used to calculate the distance and time of the routes being used for shipping.
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <cmath>
#include <cstring>
#include <fstream>
#include <algorithm>
using namespace std;

//Declaring the enum and string
enum type {LOCAL, METRO, REGIONAL, NATIONAL};
string cityPOP[4] = {"LOCAL", "METRO", "REGIONAL", "NATIONAL"};

//The class below takes in all the important information from the inout file
class city{
    public:

        //Below are the declarations of the functions that are going to be very helpful in the later code
        static int w;
        friend istream & operator>>(istream &input, city &A);
        friend ostream & operator<<(ostream &output, city &A);
        friend void read_cityinfo(string input_file, vector<city> &reading);
        friend void write_cityinfo(string input_file, vector<city> &writing);
        
        //Making comparision and getters and setters to access the data
        bool operator< (const city &B){
            return B.population < population;
        }
        float get_latitude(){return latitude * M_PI / 180;}
        float get_longitude(){return longitude * M_PI / 180;}
        int get_population(){return population;}
        string get_cityName(){return cityName;}
        type get_type(){return citySize;}
        void set_type(string cityEType){
            if(cityEType == "LOCAL"){
                citySize = LOCAL;
            }
            if(cityEType == "METRO"){
                citySize = METRO;
            }
            if(cityEType == "REGIONAL"){
                citySize = REGIONAL;
            }
            if(cityEType == "NATIONAL"){
                citySize = NATIONAL;
            }
        }

    private:
        //All the private variables that are being used by the getter and setters
        string cityName;
        type citySize;
        float latitude;
        float longitude;
        int population;
        string state;
        string nameCity;
};

//Below is the smatrix class which sets up functions to be able to setup tables and to store/get data from those indices
class smatrix{
    public:
        float get_variable(int i, int j);
        void set_variable(int i, int j, float distanceB);
        void set_size(int N);

    private:
        vector<vector<float>> city_adjtable;
};

int city::w = 0;

//the function below gets the data that is saves in the indice i j
float smatrix::get_variable(int i, int j){
    //the if statement below checks if there are any indices being filled in the top triangle of the square
    //if they are then they are swapped to be saved in the bottom triangle of the square table
    if(i < j){
        swap(i,j);
    }
    return city_adjtable[i][j];
}

//the function below set a float to the indice i j
void smatrix::set_variable(int i, int j, float distanceB){
    if(i < j){
        swap(i,j);
    }
    city_adjtable[i][j] = distanceB;
}

//the function below makes the table that will be filled with distance and time
void smatrix::set_size(int N){
    city_adjtable.resize(N);
    //the for loop below makes the bottom triangle using vectors of vectors
    for(int i = 0; i < N; i++){
        city_adjtable[i].resize(i+1);
    }
}

//The operator function below takes in all the data from the input file and saves them in their variables
istream &operator>>(istream &input, city &A) {
    string cityType;
    
    //storing all the neccesary data from the input file.
    input >> A.nameCity >> A.state >> cityType >> A.latitude >> A.longitude >> A.population;
    
    //The statement below combines the cityname with its state
    A.cityName = A.nameCity + "_" + A.state;
    
    //setting the enum type
    if(cityType == "LOCAL"){
        A.citySize = LOCAL;
    }
    if(cityType == "METRO"){
        A.citySize = METRO;
    }
    if(cityType == "REGIONAL"){
        A.citySize = REGIONAL;
    }
    if(cityType == "NATIONAL"){
        A.citySize = NATIONAL;
    }
    A.w = max((int)A.cityName.length(), A.w);
    return input;
}

int counter = 0;

//The operator below is used to print out the data from the input file in a formatted way
ostream &operator<<(ostream &output, city &A) {

    output << right << counter << "  " << setw(5) << setw(A.w + 3) << left << setfill('.') << A.cityName << "  " << setfill(' ')
            <<setw(8) << left << cityPOP[A.citySize] << "  " 
            <<setw(8) << right << A.population << "  " 
            <<setw(7) << right << fixed << setprecision(2) << A.latitude << "  " 
            <<setw(7) << right << setprecision(2) << A.longitude;
    return output;
}

//The function below is to read in all the data from the input file
void read_cityinfo(string input_file, vector<city> &reading) {
    ifstream fin;
    //the input file is opened
    fin.open(input_file);
    city data;
    string temp;

    //the getline below helps ignore the first line of the input file
    getline(fin, temp);

    //the while loop below pushes all the data to the reading vector
    while(fin >> data){
        reading.push_back(data);
    }

    //the data is sort by the population
    sort(reading.begin(), reading.end());
}

//The function below makes a file and write outs the contains of the data vector
void write_cityinfo(string input_file, vector<city> &writing) {
    ofstream fout;
    fout.open("city_info2.txt");
    
    //The for loop below goes through the vector and prints with the help of output operator
    for(int i = 0; i < (int)writing.size(); i++){
        fout << writing[i] << endl;
        counter++;
    }
}

//The function below uses the longitude and latitude value to find the distance between two cities
void create_citydist(vector<city> &reading, smatrix &distance){
    float rEarth = 3982;
    float Citydistance;
    
    int N = reading.size();
    //the for loop below goes through all the cities in reading vector
    for(int i = 0; i < N; i++){
        for(int j = 0; j < i; j++){

            //below is the calculation that gives us the distance in miles from one city to another
            Citydistance = 2 * rEarth * asin(sqrt(pow(sin((reading[j].get_latitude() - reading[i].get_latitude())/2),2)
                    + (cos(reading[i].get_latitude()))*(cos(reading[j].get_latitude()))
                    * (pow(sin((reading[j].get_longitude() - reading[i].get_longitude())/2),2))));
            //the distance found between two cities is divide by 5 to get to the nearest value
            Citydistance = 5.0 * round(Citydistance / 5.0);

            //the distance found between each city is inserted to the i j indice of the distance table
            distance.set_variable(i,j,Citydistance);
        }
        distance.set_variable(i, i, 0);
    }
}

//The function below prints out the cities with the distance between them
void write_citydist(vector<city> &reading, smatrix &distance){
    ofstream fout;
    fout.open("city_dist2.txt");
    
    //the for loops below goes through i's and j's and prints all the distances found between the connecting cities
    for(int i = 1; i < (int)reading.size(); i++){
        for(int j = 0; j < i; j++){
            fout << i << "  " << setw(reading[i].w + 3) << left << setfill('.') << reading[i].get_cityName() << setfill(' ');
            fout << " to " << setw(reading[j].w + 3) << left << setfill('.')  << reading[j].get_cityName() << setfill(' ');
            fout << "  " <<  fixed << setprecision(1) << right << setw(6) << distance.get_variable(i,j) << " miles"<< endl;
        }
        fout << endl;
    }
}

//making a struct to simplify the connections between two cities or multiple cities
struct cityWdist{
    float distance;
    city cit;
    int number;
};

//Below are two comparision functions
bool Comparepopu(city a, city b){
    //the if statement below compares the population of two cities and return true of false
    if(a.get_population() > b.get_population())
        return true;
    else
        return false;
}

bool comparedist(cityWdist a, cityWdist b){
    //the if statement below compares the distance of two cities and return true of false
    if(b.distance > a.distance){
        return true;
    }else{
        return false;
    }
}

//The function below find the time it will take to travel to the another city
void create_citytime(vector<city> &reading, smatrix &time, smatrix &distance){
    float timeB;
    
    int N = reading.size();
    time.set_size(N);

    //the for loops below go through each and every i j indice of time table
    for(int i = 0; i < N; i++){
        for(int j = 0; j < i; j++){
            //the if loop below checks if both the cities are national and if they are then it calculates time accordingly
            if(reading[i].get_type() == NATIONAL && reading[j].get_type() == NATIONAL){
                timeB = (distance.get_variable(i, j) / 500);
                time.set_variable(i, j, timeB);
            }else{
                //this else statement calculates time to travel to local or regional cities
                timeB = (distance.get_variable(i, j) / 55);
                time.set_variable(i, j, timeB);
            }
        }
    }
}


//The function below prints out the time it takes to travel to different cities
void write_citytime(vector<city> &reading, smatrix &time){
    ofstream fout;
    fout.open("city_time2.txt");

    //the for loops below go through each and ever indice of time matrix
    for(int i = 1; i < (int)reading.size(); i++){
        for(int j = 0; j < i; j++){
            //it prints out each and every time it takes to go from city 1 to city 2 or city 1 to city 3, etc
            fout << i << "  " << setw(reading[i].w + 3) << left << setfill('.') << reading[i].get_cityName() << setfill(' ')
                << " to " << setw(reading[j].w + 3) << left << setfill('.')  << reading[j].get_cityName() << setfill(' ')
                << "   "  << fixed << setprecision(1) << time.get_variable(i,j) << " hours"<< endl;
        }
        fout << endl;
    }
}

//below is the most important function which gets all the functions together 
void create_citygraph(vector<city> &reading, vector<vector<int>> &adjtable, smatrix &distance){
    //making temp vectors to store and munipulate the data using indices
    vector<city> national;
    vector<city> regional;
    vector<city> local;
    
    //The for loops below goes through all the cities and changes metros to either national or regional type
    for(int i = 0; i < (int)reading.size(); i++){
        if(reading[i].get_type() == METRO){
            if(reading[i].get_population() >= 2000000){
                //changing the enum type to national as the population is greater than 2 million
                reading[i].set_type("NATIONAL");
            }else if(reading[i].get_population() >= 500000){
                //changing the enum type to regional as the population is greater than half a million
                reading[i].set_type("REGIONAL");
            }else if(reading[i].get_population() < 500000){
                //changing any cities that have a population of lower than half a million to local
                reading[i].set_type("LOCAL");
            }
        }
    }

    //The for loop below goes through the changed input data vector
    for(int i = 0; i < (int)reading.size(); i++){
        //the if statement below checks if a city is national
        if(reading[i].get_type() == NATIONAL){
            for(int j = 0; j < (int)reading.size(); j++){
                //the if statement below checks if the distance between two cities is less than 200 which are nationals
                if(distance.get_variable(i,j) < 200 && reading[j].get_type() == NATIONAL){
                    //if it is then it is push to the national vector
                    national.push_back(reading[j]);
                }
            }

            //the vector is sorted
            sort(national.begin(), national.end(), Comparepopu);

            //The for loop starts at one because it found the city with the most population
            for(int m = 1; m < (int)national.size(); m++){
                //the for loop below goes through the whole input vector
                for(int n = 0; n < (int)reading.size(); n++){
                    //the if statement below checks if two national cities are within 200 miles and if they are then the one with less population is demoted to be a regional city
                    if(reading[n].get_cityName() == national[m].get_cityName() && reading[n].get_type() == NATIONAL && distance.get_variable(i,n) <= 200){
                        reading[n].set_type("REGIONAL");
                    }
                }
            }
        }
        national.clear();
    }
    
    //The for loop below goes through the changed input data vector
    for(int i = 0; i < (int)reading.size(); i++){
        //the if statement below checks if a city is regional
        if(reading[i].get_type() == REGIONAL){
            for(int j = 0; j < (int)reading.size(); j++){
                //the if statement below checks if the distance between two cities is less than 100 which are regionals
                if(distance.get_variable(i,j) < 100 && reading[j].get_type() == REGIONAL){
                    //if it is then it is push to the regional vector
                    regional.push_back(reading[j]);
                }
            }

            //the vector is sorted
            sort(regional.begin(), regional.end(), Comparepopu);
            
            //The for loop starts at one because it found the city with the most population
            for(int m = 1; m < (int)regional.size(); m++){
                //the for loop below goes through the whole input vector
                for(int n = 0; n < (int)reading.size(); n++){
                    //the if statement below checks if two regional cities are within 100 miles and if they are then the one with less population is demoted to be a local city
                    if(reading[n].get_cityName() == regional[m].get_cityName() && reading[n].get_type() == REGIONAL && distance.get_variable(i,n) <= 100){
                        reading[n].set_type("LOCAL");
                    }
                }
            }
        }
        regional.clear();
    }

    //the for loop below goes through all the data vector and starts making connections
    //cout << reading.size() <<endl;
    for(int i = 0; i < (int)reading.size(); i++){
        
        for(int j = 0; j < (int)reading.size(); j++){
            //the if statement below checks if both the cities are nationals and if they are then a connection is made
            if(i != j && reading[i].get_type() == NATIONAL && reading[j].get_type() == NATIONAL){
                adjtable[i][j] = 1;
                adjtable[j][i] = 1;
            }
        }
    }

    //the for loop below goes through all the data vector and starts making connections
    for(int i = 0; i < (int)reading.size(); i++){
        vector<cityWdist> regionaldist;
        //regionaldist.resize(); 
        for(int j = 0; j < (int)reading.size(); j++){
            cityWdist C;
            //the if statement below checks for regional cities and nationals just in case
            if(reading[i].get_type() == REGIONAL && j!=i && reading[j].get_type() > LOCAL){
                C.number = j;
                C.distance = distance.get_variable(i, j);
                C.cit = reading[j];
                regionaldist.push_back(C);
            }
        }

        if(reading[i].get_type() == REGIONAL){
            //partial sort is being used to get the 3 cities that are non local
            partial_sort(regionaldist.begin(), regionaldist.begin() + 3, regionaldist.end(), comparedist);
        
            //below are the connections being made with nearest 3 cities
            adjtable[i][regionaldist[0].number] = 1;
            adjtable[i][regionaldist[1].number] = 1;
            adjtable[i][regionaldist[2].number] = 1;
            adjtable[regionaldist[0].number][i] = 1;
            adjtable[regionaldist[1].number][i] = 1;
            adjtable[regionaldist[2].number][i] = 1;
        }
    }
    
    //the for loop below goes through all the data vector and starts making connections
    for(int i = 0; i < (int)reading.size(); i++){
        vector<cityWdist> localdist;
        
        //cout << localdist.size() << endl; 
        for(int j = 0; j < (int)reading.size(); j++){
            //the if statement below checks if the cities are locals and are within 75 mile distance to each other
            if(reading[i].get_type() == LOCAL && reading[j].get_type() == LOCAL && i != j && distance.get_variable(i,j) < 75){
                //the connection is being made
                adjtable[i][j] = 1;
                adjtable[j][i] = 1;
            }
        }
    }
    
    for(int n = 0; n < (int)reading.size(); n++){ 
        vector<cityWdist> localdist;
        cityWdist C;
    
        for(int m = 0; m < (int)reading.size(); m++){
            if(reading[n].get_type() == LOCAL && m!=n && reading[m].get_type() == LOCAL){
                C.number = m;
                C.distance = distance.get_variable(n, m);
                C.cit = reading[m];
                localdist.push_back(C);
            }
        }

            if(reading[n].get_type() == LOCAL){    
                //partial sort is being used to get the 2 cities that are local and near
                partial_sort(localdist.begin(), localdist.begin() + 2, localdist.end(), comparedist);
        
                //below are the connections being made with nearest 2 cities
                adjtable[n][localdist[0].number] = 1;
                adjtable[n][localdist[1].number] = 1;
                adjtable[localdist[0].number][n] = 1;
                adjtable[localdist[1].number][n] = 1;
            }
        
    }
    
}

void write_citygraph(vector<city> &reading, smatrix &distance, smatrix &time, vector<vector<int>> adjtable){
    ofstream fout;
    fout.open("city_graph2.txt");
    for(int i = 0; i < (int)reading.size(); i++){
        
        //printing the first city with connections
        fout << reading[i].get_cityName() << endl;
        for(int j = 0; j < (int)reading.size(); j++){
            //printing all the connections with time
            if(adjtable[i][j] == 1 && i != j){
                fout << i << " " << setw(reading[i].w + 3) << left << setfill('.') << reading[j].get_cityName()
                    << setw(8) << right << setfill(' ') << setprecision(1) << fixed << distance.get_variable(i, j) << " miles "
                    << setw(8) << right << setfill(' ') << setprecision(1) << fixed << time.get_variable(i, j) << " hours " << endl;
            }
        }
        fout << endl;
    }
}

int main(int argc, char *argv[])
{
    string input_file;
    vector<city> inputVec;
    string info = argv[1];

    if(argc == 3){
        input_file = argv[2];
        read_cityinfo(input_file, inputVec);
    
        smatrix distance;
        smatrix time;
        vector<vector<int>> adjtable;
        adjtable.assign(inputVec.size(), vector<int>(inputVec.size()));

        distance.set_size(inputVec.size());

        create_citydist(inputVec, distance);
        create_citygraph(inputVec, adjtable, distance);
        create_citytime(inputVec, time, distance);

        if(argv[1] == info){
            write_citydist(inputVec, distance);
            write_citygraph(inputVec, distance, time, adjtable);
            write_citytime(inputVec, time);
        }

    }else{
        cerr << "usage: incorrect file call" << endl;
        cerr << "file call : ./Citysim -info input_file.txt" << endl;
    }
}
