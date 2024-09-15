//Nagendra Upadhyay
//Part A 03.19.21
//Part B 03.26.21

//Lab 4 part A asks the user for a string input and gives them the total number of characters in the string and also prints out the string without whitespaces.
//Lab 4 part B asks the user for a string and gives them a list of multiple things that the user could do with their input.

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

//Defining all the functions used.
int NumWords(const string&);
int NumNonWSCharacters(const string&);
void CharReplace(string&, char, char);
char PrintMenu();


int main()
{
	//Declaring variables for the user input and other functions.
	string input;
	char find;
	char replace;
	char choice;

	//Asking the user for an input of a string.
	cout << "Enter a line of text: ";

	//Storing the whole string in input
	getline(cin, input);
	cout << endl;

	//Printing out the user input string again.
	cout << "You entered:  " << input;
	cout << endl;
	cout << endl;

	//The function statement below makes sure that the Menu will at least print once.
	choice = PrintMenu();

	//The while loop below checks if choice is 'q' and also checks if choice is a valid input.
	while(choice != 'q' && (cin))
	{
		//The switch loop below takes the user input saved in choice and checks it with all the cases.
		switch(choice)
		{
			//If the user enters 'w' as the choice, then the code below will be executed.
			case 'w':
				{
					cout << endl;
					//The statement below outputs a message and also calls the function to get a value.
					cout << "Number of words: " << NumWords(input) << endl;
					cout << endl;
					break;
				}
				//If the user enters 'c' as the choice, then the code below will be executed.
			case 'c':
				{
					cout << endl;
					//The statement below outputs a message and also calls the function to get a value.
					cout << "Number of non-whitespace characters: " << NumNonWSCharacters(input) << endl;
					cout << endl;
					break;
				}
				//If the user enters 'r' as the choice, then the code below will be executed.
				//Case 'r' is a little different as it replaces a character with a new character of user's choice.
			case 'r':
				{
					//Asking user for the character that needs to be changed.
					cout << "Enter a character to find: ";
					//Storing the character user entered in 'find'.
					cin >> find;
					//Asking user for the new character that will take places of the old characters.
					cout << "Enter a character to replace: ";
					//Storing Storing the character user entered in 'replace'.
					cin >> replace;

					//Calling for the function to swap the characters.
					CharReplace(input, find, replace);
					cout << endl;
					//Printing out the new string with changed/swapped characters.
					cout << "New string: " << input << endl;
					cout << endl;
					break;
				}
			default: break; 
					 cout << endl;
		}
		//Calling the function again, but this time it is inside the while loop.
		//The function below will print out the Menu every time the function will get out of the switch.
		choice = PrintMenu();
	}   
	return 0;
}

//Making the function for Menu below.
char PrintMenu()
{
	char choice;
	//Printing out the Menu that user will see.
	cout << "Options" << endl;
	cout << "w - Number of words" << endl;
	cout << "c - Number of non-whitespace characters" << endl;
	cout << "r - Replace a character" << endl;
	cout << "q - Quit" << endl;
	cout << endl;

	//Asking the user for a character or the option that they would want to go with.
	cout << "Choose an option: ";
	//Storing the input in choice.
	cin >> choice;
	//Returning the value stored in choice.
	return choice;
}

//Making the function that will swap/replace the old character with the new one that the user wants.
void CharReplace(string& input, char OLD, char NEW)
{
	int i;
	//The for loop below scans for the characters in the string.
	for (i = 0; i < input.length(); ++i)
	{
		//The if statement below swaps the old character will the new one with the help of referencing the variable..
		if(input[i] == OLD)
		{
			input[i] = NEW;
		}
	}
}

//Making the function that will count the characters without a whitespace.
int NumNonWSCharacters(const string& input)
{
	int NW = 0;
	int i;
	//The for loop below checks all the characters in the string and counts the ones without whitespaces.
	for(i = 0; i < input.length(); ++i)
	{
		//The if statement below is basically adding 1 to NW when a character is found.
		if(input[i] != ' ')
		{
			NW++;
		}
	}
	//Returning the value to the place were function is called.
	return NW;
}

int NumWords(const string& input)
{
	//Making a stringstream named ss.
	stringstream ss;
	//Storing the user input in ss.
	ss << input;
	string temp;
	int NumOfWords = 0;
	int i;
	//The while loop below counts all the words present in the user input(string).
	while(ss >> temp)
	{
		NumOfWords++;
	}
	//Returning the value to the place were function is called.
	return NumOfWords;
}
