//Nagendra Upadhyay
//02.19.21

//Lab2 part A is asking for an userinput which is interstate number and replies with some information about the highway as an output.

#include <iostream>
#include <string>
using namespace std;

int main()
{
	//Declaring an integer a variable named interstateNumber.
	int interstateNumber;
	
	//Asking user to input a number.
	cout << "Enter an interstate number: ";
	//Storing the input in interstateNumber.
	cin >> interstateNumber;

	//The first 'if' condition is checking if the interstateNumber is greater than 1 and if it is less than 999, and also if it is divisible by 100.
	//If all the conditions are true in the if statement then it enters the statement.
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
				cout << "I-" << interstateNumber << " is an auxiliary highway, serving I-75, going north/south." << endl;
			}
			else
			{
				//The interstateNumber is even so it will print the message below as an output.
				cout << "I-" << interstateNumber << " is an auxiliary highway, serving I-75, going east/west." << endl;
			}
		}
	}
	//This else statement is printed out as an output when the interstateNumber is 0,100,500,300,1000,1001,etc.
	else
	{
		//This statement is printed out when the interstateNumber value is invalid.
		cout << interstateNumber <<" is not a valid interstate number." << endl;
	}
}
