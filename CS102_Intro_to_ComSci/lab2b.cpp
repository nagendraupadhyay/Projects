//Nagendra Upadhyay
//Part A 02.19.21
//Part B 02.26.21
//Part C 02.26.21

//Lab2 part A is asking for an userinput which is interstate number and replies with some information about the highway as an output.
//Lab2 part B is going to ask for userinput again and again until 0 is enter as an input. When 0 will be entered the program will end.
//Lab2 part C will print out a message if userinput is anything other than an interger.

#include <iostream>
#include <string>
#include <limits>
using namespace std;

int main()
{
	//Declaring an integer a variable named interstateNumber.
	int interstateNumber = -1;

	//Asking user to input a number.
	cout << "Enter an interstate number: ";

	//This while loop is checking if userinput is not 0. If it is not 0, then it will enter the loop. If it is 0, then it will not enter the loop. 
	while(!(interstateNumber == 0))
	{
		//This while loop is checking if the userinput is anything other than an integer.
		while (!(cin >> interstateNumber))
		{
			//clears the invalid userinput
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			//prints out a message becuase of the invalid input.
			cout << "Invalid input, try again: ";
		}

		//Checks for any errors if userinput is  0.
		if(interstateNumber== 0)	
		{
			return 0;
		}

		//The first 'if' condition is checking if the interstateNumber is greater than 1 and if it is less than 999, and also if it is divisible by 100.
		//If all the conditions are true in the if statement then it enters the loop.
		if((interstateNumber >= 1) && (interstateNumber <= 999) && (interstateNumber % 100))
		{
			//It checks if the interstateNumber is less than 99. If it is then it will enter the statement.
			if(interstateNumber <= 99)
			{
				//This if statement is for primary interstates.
				//The condition below checks if interstateNumber is odd or even.
				if(interstateNumber % 2)
				{
					//The interstateNumber is odd so it will print the message below as an output.
					cout << "I-" << interstateNumber << " is a primary interstate, going north/south." << endl;//odd
				}
				else
				{
					//The interstateNumber is even so it will print the message below as an output.
					cout << "I-" << interstateNumber << " is a primary interstate, going east/west." << endl;//even
				}
			}
			//The statement below checks if the interstateNumber is greater than 101. If it is then it enters the statement.
			else if((interstateNumber >= 101))
			{
				//This else if statement is for auxiliary highway
				//The condition below checks if interstateNumber is odd or even.
				if(interstateNumber % 2)
				{
					//The interstateNumber is odd so it will print the message below as an output.
					cout << "I-" << interstateNumber << " is an auxiliary highway, serving I-"<< interstateNumber % 100 <<", going north/south." << endl;
				}
				else
				{
					//The interstateNumber is even so it will print the message below as an output.
					cout << "I-" << interstateNumber << " is an auxiliary highway, serving I-"<< interstateNumber % 100 <<", going east/west." << endl;
				}
			}
		}
		//This else statement is printed out as an output when the interstateNumber is 0,100,500,300,1000,1001,etc.
		else
		{
			//This statement is printed out when the interstateNumber value is invalid.
			cout << interstateNumber <<" is not a valid interstate number." << endl;
		}

		//The statement below is asking for userinput again after giving one output.
		cout << "Enter an interstate number: ";
	}
}
