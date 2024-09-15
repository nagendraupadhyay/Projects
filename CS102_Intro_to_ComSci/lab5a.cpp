//Nagendra Upadhyay
//Part A 04.08.21

//Lab 5 part A asks user for input for their shopping list and helps them to checkout. 

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <limits>
using namespace std;

//Making a class below named Item.
class Item
{
    //Making public mutator so the functions below can be used in the code later.
    public:
    //Declaring and definitng functions below to use later.
    Item ();
    void SetName (string n);
    void SetPrice (double p);
    void SetQuantity (int i);
    string GetName ();
    double GetPrice ();
    int GetQuantity ();
    void Print ();
    //Making private variables to use later.
    private:
    string name;
    double price;
    int quantity;
    };

 
int main ()
{
    //Declaring all the variables used in the code below.
    int z = 0;
    int numItem = 0;
    double totalAmount = 0;
    vector<Item> items;
    string n;
    double p;
    int i;
    double price;
    int quantity;
    int f;
 
    //The while loop below check if Ctrl-D is pressed or not.
    while (!cin.eof ())
    {
        //Declaring a variable with the help of the class above.
        Item currItem;
        //Increasing the count for number of items the user is shopping for.
        numItem++;
 
        //The if statement below checks if Ctrl-D is pressed. If pressed, then the code from while loop will exit.
        if(cin.eof())
        {
           break;
        }

        //Printing out the number of things the user bought.
        cout << "Item " << numItem << endl;

        //Printing out a message for user.
        cout << "Enter the item name: ";
        //Getting the user input and storing it in n.
        getline (cin, n);
        //Storing the data in the vectors below.
        currItem.SetName(n);
        items.push_back (currItem);

        //The if statement below checks if Ctrl-D is pressed. If pressed, then the code from while loop will exit.
        if(cin.eof())
        {
            break;
        }
        //Printing out a message for user.
        cout << "Enter the item price: $";
        //The while loop below checks if correct kind of data type is entered or not.
        while (!(cin >> p))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            //Printing out a message for user asking for a new input as the the privous input was invalid.
            cout << "Enter the item price: ";
        }
 
            //Storing the data in the vector below.
            items.at(z).SetPrice(p);
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
 
            //The if statement below checks if Ctrl-D is pressed. If pressed, then the code from while loop will exit.
            if(cin.eof())
            {
                break;
            }

            //Printing out a message for user.
            cout << "Enter the item quantity: ";
            
            //The while loop below checks if correct kind of data type is entered or not.
            while (!(cin >> i))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                //Printing out a message for user asking for a new input as the the privous input was invalid.
                cout << "Enter the item quantity: ";
            }
            
            //Storing the data in the vector below.
            items.at(z).SetQuantity(i);
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            //Updating z below and also ignoring the previous invalid inputs.
            ++z;
            cout << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        //Priting out a heading for the user.
        cout << endl;
        cout << endl << "Totals" << endl;

        //The for loop below calculates the total amount the user has to pay and also shows the the things the user bought..
        for (int x = 0; x < items.size() - 1; ++x)
        {
            items.at(x).Print();
            totalAmount += items.at(x).GetPrice() * items.at(x).GetQuantity();
        }

        //The statement below prints the total amount.
        cout << endl << "Total: $ " << totalAmount << endl;

        return 0;
    }

    //Creating a default constructor below.
    Item::Item ()
    {
        name = "none";
        price = 0;
        quantity = 0;
    }

    //Making all the functions that were used in the code above.
    //The function below stores the name of the iteam in n and the other functions are also doing something similar..
    void Item :: SetName (string n) 
    {
        name = n;
    }

    void Item :: SetPrice (double p)
    {
        price = p;
    }
    void Item :: SetQuantity (int i)
    {
        quantity = i;
    }
    string Item :: GetName ()
    {
        return name;
    }
    double Item :: GetPrice ()
    {
        return price;
    }
    int Item :: GetQuantity ()
    {
        return quantity;
    }
    //The function below prints out all the data stored in vectors and gives the user a list of things they got.
    void Item :: Print ()
    {
        cout << name << ":  " << quantity << " at $" << setprecision (2) << fixed <<  price << " = $" <<  quantity * price << endl;
    }