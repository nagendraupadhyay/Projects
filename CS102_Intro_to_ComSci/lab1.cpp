//Nagendra Upadhyay
//02.04.21

//Lab 1a asks the user for their name and gives them a reply.
#include <iostream>
#include <string>

using namespace  std;

int main()
{
	string name;
	cout << "What is your name? ";
	cin >> name;
	cout << "Hi there, " << name << ".\n";

	//02.12.21
	//lab1 part B starts here. The Lab 1b asks for some user input about their house and gives them some details about the house.

	
	//Declaring all the variables with their necessary data type.
	int streetNumber;
	string streetName;
	string streetType;
	int lastPropertyValue;
	int currentPropertyValue;
	double monthlyMortgage;
	double averageMonthlyMortgage;
	double lastMonthlyMortgage;

	//Asking for user input.
	cout << "Enter the property street number, name, and type: ";
	//Saving the user input to use later for calculations.
	cin >> streetNumber >> streetName >> streetType;
	//Doing the same thing for next few lines which is asking for user input and saving the input to use it for calculations.
	cout << "Last month's property value: $";
	cin >> lastPropertyValue;
	cout << "This month's property value: $";
	cin >> currentPropertyValue;
	cout << '\n';
	
	//Calculating estimated monthly mortgage.
	monthlyMortgage = (currentPropertyValue * 0.050)/12;

	//Printing out some statements with some calcualtions and user inputs.
	cout << "This property is located at " << streetNumber << " " << streetName << " " << streetType << ".\n";
	cout << "Its value has changed by $" << (currentPropertyValue - lastPropertyValue) << " since last month.\n";
	cout << "The estimated monthly mortgage is $" << monthlyMortgage <<".\n";
	
	//02.12.21
	// lab1 part C starts here. The Lab 1c is an extension of part b and gives the user some more information about their house by doing some more calculations.

	
	//Calculating last month's mortgage and average monthly mortgage.
	lastMonthlyMortgage = (lastPropertyValue * 0.050)/12;
	averageMonthlyMortgage = (monthlyMortgage + lastMonthlyMortgage)/2;
	
	//Priting out statements with the calculated numbers.
	cout << "Last month's estimated monthly mortgage was $" << lastMonthlyMortgage << ".\n";
	cout << "The average monthly mortgage is $" << averageMonthlyMortgage  <<".\n";
	
	return 0;
}
