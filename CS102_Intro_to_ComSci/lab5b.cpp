//Nagendra Upadhyay
//Part A 04.08.21
//Part B 04.16.21

//Lab 5 part A asks user for input for their shopping list and helps them to checkout. 
//Lab 5 part B is a Shopping Cart program which gives the user multiple choices to the user to add/edit thing from their shopping list.

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
    //Making public mutator and accessors so the functions below can be used in the code later.
    public:
        //Declaring and defining functions below to use later.
        Item ();
        void SetName (string n);
        void SetPrice (double p);
        void SetQuantity (int i);
        string GetName ()const;
        double GetPrice ();
        int GetQuantity ();
        void Print ();
        
        //Making/declaring some functions to use later in the code.
        //The function below stores the description of the item in d.
        void SetDescription(string d)
        {
            description = d;
        }
        //The function below returns the description.
        string GetDescription()
        {
            return description;
        }
        //The function below prints out the item's name and its description.
        void PrintDescription()
        {
            cout << GetName() << ": " << GetDescription() << endl;
        }
        
        Item (string, string, double, int);
    
    //Making private variables to use later.
    private:
        string name;
        string description;
        double price;
        int quantity;
};

//Making a class below named ShoppingCart.
class ShoppingCart
{
    //Making public mutator and accessors so the functions below can be used in the code later.
    public:
        
        //The function below returns the name.
        string GetCustomerName()const
        {
            return name;
        }
        
        //The function below returns the date.
        string GetDate()const
        {
            return date;
        }
        
        //The function below is calculating the total number of items in the cart.
        //The function also returns the total number of things present in the cart.
        int GetTotalQuantity()
        {
            int totalQuantity = 0;
            for (int x = 0; x < items.size(); ++x)
            {
                totalQuantity += items.at(x).GetQuantity();
            }
            return totalQuantity;
        }
        
        //The function below is calculating the total cost of the things present in the cart.
        double GetTotalCost()
        {
            int totalCost = 0;
            for (int x = 0; x < items.size(); ++x)
            {
                //Calculating total cost of the things present in the cart by multiplying the quantity and price.
                totalCost += items.at(x).GetPrice() * items.at(x).GetQuantity();
            }
            return totalCost;
        }
        
        //The function below is printing the total cost of all the things in the cart.
        void PrintTotal ()
        {
            double totalAmount = 0;
            cout << endl;
            //Printing out user's name and date that they entered.
            cout << GetCustomerName() << "'s Shopping Cart - " << GetDate() << endl;
            //Printing out number of things in the cart.
            cout << "Number of Items: " << GetTotalQuantity() << endl;

            //The if statement below checks if the number of things are not equal to zero.
            if(items.size() != 0)
            {
                //This for loop calculates the total cost of the items.
                for (int x = 0; x < items.size(); ++x)
                {
                    items.at(x).Print();
                    totalAmount += items.at(x).GetPrice() * items.at(x).GetQuantity();
                }
            }
            //This else statement is entered when number of items in cart = 0.
            else
            {
                cout << endl << "Shopping cart is empty." << endl;
            }
            //The statement below prints out the total amount.
            cout << endl << "Total: $" << totalAmount << endl;
        }
    
        //The function below gives the user descriptions that they wrote for the items present in teh cart.
        void PrintDescriptions()
        {
            cout << endl << GetCustomerName() << "'s Shopping Cart - " << GetDate() << endl;
            cout << endl <<"Item Descriptions" << endl;
            
            //The if statement below checks that there are some items present in the cart.
            if(items.size() != 0)
            {
                for (int x = 0; x < items.size(); ++x)
                {
                    items.at(x).PrintDescription();
                }
            }
            //If the cart has zero items then the else statement below is entered.
            else
            {
                cout << "Shopping cart is empty." << endl;
            }
        }
        
        //This function adds new items to the vector.
        void AddItem(const Item& currItem)
        {
            items.push_back(currItem);
        }
        
        //The function below finds the item that user entered and removes it.
        void RemoveItem(const string& name)
        {
            int i = FindItemIndex(name);
            //If the item is not in the cart then the message below is printed.
            if(i == -1)
            {
                cout << "Item not found in cart." << endl;
            }
            //If the item is found then it is erased from the vector.
            else
            {
                items.erase(items.begin() + i);
            }
        }
        
        //The function below changes the quantity of an item.
        void ChangeQuantity(const string& name, int quantity)
        {
            int i = FindItemIndex(name);
            if(i == -1)
            {
                cout << "Item not found in cart." << endl;
            }
            else
            {
                items[i].SetQuantity(quantity);
                if (quantity == 0) {
                    RemoveItem(name);
                }
            }
            
        }
        
        //Making a constructer below.
        ShoppingCart(string n = "none", string d = "April 1, 2020") : name(n), date(d) {}; 
    
    //Making private variables to use in the functions.
    private:
        string name;
        string date;
        vector<Item> items;
        int FindItemIndex(const string& name)const;
};

//The menu below is printed and asks the user for an input.
char PrintMenu()
{
    char choice;

    cout << endl << "MENU" << endl;
    cout << "a - Add item to cart" << endl;
    cout << "d - Remove item from cart" << endl;
    cout << "c - Change item quantity" << endl;
    cout << "i - Output item descriptions" << endl;
    cout << "o - Output shopping cart" << endl;
    cout << "q - Quit" << endl;
    cout << endl;
    cout << "Choose an option: ";
    //The input is stored in choice.
    cin >> choice;
    return choice;
}


int main ()
{
    //Declaring all the variables used in the code below.
    double totalAmount = 0;
    string n;
    string customerDate;
    string customerName;
    double p;
    int i;
    double price;
    int quantity;
    char choice;
    Item ItemNow;

    //The lines below are printed for the user and some inputs are being stored.
    cout << "Enter customer name: ";
    getline(cin, customerName);
    cout << "Enter today's date: ";
    getline(cin, customerDate);
    cout << endl;
    cout << "Customer name: "<< customerName << endl;
    cout << "Today's date: " << customerDate << endl;

    ShoppingCart cart(customerName, customerDate);

    //The while loop below checks if q is not entered and cntrl D is not pressed.
    while (!cin.eof () && choice != 'q')
    {
        //Declaring variables
        choice = PrintMenu();
        string ItemName;
        string ItemDes;
        double ItemPrice = 0;
        int ItemQuantity = 0;
        
        //The switch below controls all the options that are printed for the user.
        switch(choice)
        {
            //If the user chose option a then this case is entered.
            case 'a':
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << endl << "ADD ITEM TO CART" << endl;
                cout << "Enter the item name: ";
                getline(cin, ItemName);
                
                //Checking if cntrl D is pressed.
                if (cin.eof()) { break; };
                //Asking user for an input.
                cout << "Enter the item description: ";
                //Storing the user input in ItemDes
                getline(cin,ItemDes);
                if (cin.eof()) { break; };
                
                //Asking for user input
                cout << "Enter the item price: $";
                //The while loop below checks if the value entered for price is a double or not.
                while (!(cin >> ItemPrice)) 
                {
                    if (cin.eof()) { break; };
                    //Invalid value is cleared.
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    //If the input is something other than an interger the message is printed again.
                    cout << "Enter the item price: ";
                }
                
                if (cin.eof()) { break; };  
                
                cout << "Enter the item quantity: ";
                //The while loop below checks if the value entered for quantity is an integer or not.
                while (!(cin >> ItemQuantity)) 
                {
                    if (cin.eof()) { break; };
                    //Invalid value is cleared.
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    //If the input is something other than an interger the message is printed again.
                    cout << "Enter the item quantity: ";
                }
                
                if (cin.eof()) { break; };
                //Storing all the inputs of the user in the vectors below.
                ItemNow.SetName(ItemName);
                ItemNow.SetDescription(ItemDes);
                ItemNow.SetPrice(ItemPrice);
                ItemNow.SetQuantity(ItemQuantity);
                
                cart.AddItem(ItemNow);
                break;
            }
            
            //If the user chose option d then this case is entered.
            //The case below asks the user for the name of the things that they want to delete.
            case 'd':
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "REMOVE ITEM FROM CART" << endl;
                cout << "Enter the item name to remove: ";
                getline(cin, ItemName);                
                if (cin.eof()) { break; };
                cart.RemoveItem(ItemName);
                break;
            }
            
            //The case below gives the user an option if they want to change the quantity of any item.
            case 'c':
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "CHANGE ITEM QUANTITY" << endl;
                cout << "Enter the item name: ";
                getline(cin, ItemName);
                cout << "Enter the new quantity: ";
                while (!(cin >> ItemQuantity)) 
                {
                    if (cin.eof()) { break; };
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Enter the new quantity: ";
                }
                
                if (cin.eof()) { break; };
                
                cart.ChangeQuantity(ItemName, ItemQuantity);
                break;
            }
            
            //The case below call for a function.
            case 'i':
            {
                cart.PrintDescriptions();
                break;
            }
            //The case below call for a function.
            case 'o':
            {
                cart.PrintTotal();
                break;
            }
            //The case below breaks if q is entered.
            case 'q':
                break;
        }
    }
    return 0;
}

//Creating a default constructor below.
Item::Item ()
{
    name = "none";
    price = 0;
    quantity = 0;
    description = "none";
}

//Making a constructor below for the variables.
Item::Item (string name, string description, double price, int quantity)
{
    name = name;
    price = price;
    quantity = quantity;
    description = description;
}


//Making all the functions that were used in the code above.
//The function below stores the name of the item in n and the other functions are also doing something similar to that.
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

string Item :: GetName ()const
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

void Item :: Print ()
{
    cout << name << ":  " << quantity << " at $" << setprecision (2) << fixed <<  price << " = $" <<  quantity * price << endl;
}

int ShoppingCart::FindItemIndex(const string& name)const
{
    for(int i=0; i < items.size(); ++i)
    {
        if(items[i].GetName() == name)
        {
            return i;
        }
    } 
    return -1;
}