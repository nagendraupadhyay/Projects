#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <cstring>
#include <fstream>
#include <algorithm>
#include <map>
using namespace std;

enum fuel_type { REGULAR, MIDGRADE, PREMIUM, DIESEL };
string monthselection[12] = {"Jan", "Feb", "Mar", "Arp", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
string fuelSelection[4] = {"Regular", "Midgrade", "Premium", "Diesel"};
struct sale {
    int month;
    int day;
    int year;
    fuel_type type;
    float quantity;
    float unitPrice;
    float subTotal;
};

struct sale_summary { 
    sale_summary();
    sale_summary(const sale &);
    void operator+=(const sale &);
    double quantity;
    double subtotal;
};

sale_summary::sale_summary(const sale &n){
    quantity = n.quantity;
    subtotal = n.subTotal;
}

void sale_summary::operator+=(const sale &n){
    quantity += n.quantity;
    subtotal += n.quantity;
}

istream &operator>>(istream & input, sale & money) {
    string fuelType;
    string date;
    input >> date;

    sscanf(date.c_str(), "%02d-%02d-%4d", &money.month, &money.day, &money.year);
    
    input >> fuelType;
    
    if(!input.eof())
    {

        if(fuelType == "Regular")
        {
            money.type = REGULAR;
        }else if(fuelType == "Midgrade")
        {
            money.type = MIDGRADE;
        }else if(fuelType == "Premium" )
        {
            money.type = PREMIUM;
        }else
        {
            money.type = DIESEL;
        }
    
        input >> money.quantity;
        input >> money.unitPrice;
    
        money.subTotal = money.unitPrice * money.quantity;
    }
    return input;
}

ostream &operator<<(ostream &output, const sale &money) {
    output << setw(3) << monthselection[money.month - 1] << right << "  " << setw(2) << money.day << left << ", " << setw(4) << money.year << left 
            << " " << setw(8) << fuelSelection[money.type] << right << " " << setw(8) << money.quantity << left << " x " << setw(4) << money.unitPrice << left 
            << " = " << setw(7) << right << setprecision(2) << fixed << money.subTotal;
    return output;    
}

ostream &operator<<(ostream &output, const sale_summary &money) {
     output << " "<<setw(8)<< right <<money.quantity << left <<" x " << setw(4)
         << money.subtotal/money.quantity <<left <<  " = " << setw(7) << right << setprecision(2) << fixed 
         << money.subtotal;
}

bool cmp_bydate(const sale & sale1, const sale &sale2) {
    int dateNumber1;
    int dateNumber2;
    
    dateNumber1 = sale1.year*10000 + sale1.month*100 + sale1.day;
    dateNumber2 = sale2.year*10000 + sale2.month*100 + sale2.day;
    
    if(dateNumber1 > dateNumber2)
    {
        return false;
    }
}

bool cmp_bytype(const sale & type1, const sale & type2) { 
    return type1.type < type2.type;
}

template <typename Iterator>
void print_details(Iterator p0, Iterator p1) {
    double total = 0.0;
    for(;p0 != p1; p0++)
    {
        cout << (*p0)<<" :   ";
        total += p0 -> subTotal;
        cout <<  setprecision(2) << fixed << total << left << endl;
    }
}

template <typename Iterator>
void print_summary_bydate(Iterator p0, Iterator p1) {
    map<int ,sale_summary> sales;
    for(;p0 != p1; p0++)
    {
        int SumDate = (*p0).year*10000 + (*p0).month*100 + (*p0).day;   
        auto [index,succeeded] = sales.insert(make_pair(SumDate, sale_summary(*p0)));
        if(succeeded == false){
            sales.at(index->first) += *p0;
        }
    }

    double total = 0.0;
    
    map<int , sale_summary>::iterator it0 = sales.begin();
    map<int , sale_summary>::iterator it1 = sales.end();

    for(;it0 != it1; it0++)
    {
        int year;
        int month;
        int day;

        year = it0->first/10000;
        month = it0->first/100;
        day = it0->first;

        month = month - year*100;
        day =  day - month*100 - year*10000;

        cout << setw(3) << monthselection[month-1] << right << "  " << setw(2) << day << left << ", " << setw(4) << year << left << " " << setw(8);
        cout << it0->second;
        
        total += it0->second.subtotal;
        cout << " :   " << setprecision(2) << fixed << total << left << endl;
    }
}

template <typename Iterator>
void print_summary_bytype(Iterator p0, Iterator p1) {
    map<fuel_type,sale_summary> TypeOfFuel;
     
    
}

int main(int argc, char *argv[]) {
    ifstream fin;
    string sort_mode;
    string input_file;
    string summary_mode;
    input_file = argv[3];

    if (strcmp(argv[1], "-inorder") == 0){
        sort_mode = "-inorder";
    }else if(strcmp(argv[1], "-bydate") == 0){
        sort_mode = "-bydate";
    }else if(strcmp(argv[1], "-bytype") == 0){
        sort_mode = "-bytype";
    }else{
        cout << "Error : ./Fuel -inorder/-bydate/-bytype .txt";
    }
    
    if(strcmp(argv[2], "-details") == 0){
        summary_mode = "-details";
    }else if(strcmp(argv[2], "-summary") == 0){
        summary_mode = "-summary";
    }else{
        cout << "Error : ./Fuel -inorder/-bydate/-bytype -summary/-details .txt";
    }
        
    vector<sale> DB;

    fin.open(input_file);
    sale F;
    while(fin>>F)
        DB.push_back(F);

    if(sort_mode == "-bydate"){
        sort(DB.begin(), DB.end(), cmp_bydate);
    } 
    if(sort_mode == "-bytype"){
        sort(DB.begin(), DB.end(), cmp_bytype);
    }

    if(summary_mode == "-details"){
        print_details(DB.begin(), DB.end());
    }else if(summary_mode == "-summary"){
        if(sort_mode == "-bydate"){
            print_summary_bydate(DB.begin(), DB.end());
        }
        else if(sort_mode == "-bytype"){
            print_summary_bytype(DB.begin(),DB.end());
        }
    }

    fin.close();
}
