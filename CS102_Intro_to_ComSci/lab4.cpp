//Nagendra Upadhyay
//Part A 03.19.21

//Lab 4 part A asks the user for a string input and gives them the total number of characters in the string and also prints out the string without whitespaces.

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

int main()
{
	//Declaring variables for the user input and other functions.
	string input;
	stringstream checkString;
	char inputchar;

	//Asking the user for an input of a string.
	cout << "Enter a line of text: ";
	//Storing the whole string in input
	getline(cin, input);
	cout << endl;

	//Printing out the user input string again.
	cout << "Original: " << input << endl;
	//Telling the user how many characters are in the string that user typed in.
	cout << "Characters: " << input.length() << endl;

	//Storing the user input in checkString.
	checkString << input;

	//Printing out a message.
	cout << "Without whitespace: ";

	//The while loop below checks if the string stored in checkString has only data type char.
	//If the string has anything other than a char it will ignore it.
	while (checkString >> inputchar)
	{
		//The statement below prints out every single character in the string other than the whitespaces.
		cout << inputchar;
	}
	cout << endl;

	return 0;
}
