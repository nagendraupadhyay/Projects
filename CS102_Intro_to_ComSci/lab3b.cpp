//Nagendra Upadhyay
//Part A 03.03.21
//Part B 03.10.21
//Lab 3 part A asks for userinput and gives them an output of some random numbers.
//Lab 3 part B uses the seed value that userinputs and starts a slot machine game that includes bets and coins.

#include <iostream>
#include <string>
#include <random>
#include <limits>
using namespace std;

int main()
{
	//Declaring variable for userinput and other calculations.
	int seed;
	int i;
	int array[3];

	//The user starts the game with 100 coins.
	int coins = 100;
	int bet;

	//Asking user for a number input.
	cout << "Input a seed: ";

	//Storing the input in seed and checking if it is an integer or not. If it is other than an integer it will ask for user seed input again.
	while(!(cin >> seed))
	{
		cout << "Input a seed: ";
		//Clears the bad userinput.
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	//Declaring random number generator using userinput.
	default_random_engine rng(seed);
	//Setting the range of the random numbers.
	uniform_int_distribution<int> userinput_seed(2, 7);

	//This massive do-while loop checks for condition and repeats itself again and again.
	do{
		//Telling the user the number of coins that they posses after betting and earning coins.
		cout << '\n' << "You currently possess " << coins << " coins." << endl;
		//Asking user for a bet.
		cout << "Bet how many coins? ";

		//The while loop below error checks. This loop check if the bet input is not negative, bet is not greater than coins, and also that the bet should be an integer.
		while(!(cin >> bet) || coins < bet || bet < 0)
		{
			//It will ask for userinput of bet again and agin until the correct amount of bet is entered.
			cout << "Bet how many coins? ";
			//Clears the bad userinput.
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}

		//This statement below subtracts the amount of bet from the amount of coins the user posses.
		coins = coins - bet;

		//The if statement below will exit the loop if a 0 is entered for a bet.
		if(bet == 0)
		{
			break;
		}

		//This for loop gives user 3 numbers as an output from the random number generator.
		for(i = 0; i < 3; ++i)
		{	
			//The array below is storing the numbers that were obtained randomly.
			array[i] = userinput_seed(rng);
			cout << ' ' << array[i];
		}
		cout << endl;

		//This is the heart of the slot machine code.
		//The if statement below checks if any two elements of the array are the same. If they are, then it enter the winnings loop.
		if(array[0] == array[1] || array[1] == array[2] || array[0] == array[2])
		{
			//The if statement below checks if all the elements in the number generator array are the same except they are not 777 or array[0] is not equal to 7.
			if(array[0] == array[1] && array[1] == array[2] && array[0] != 7)
			{
				//The statement below tells the user how any coins they won by multiplying the bet by 5.
				cout<<"You won "<< bet * 5 << " coins!" <<endl;
				//The statement below updates the coins user posses.
				coins = coins + (bet * 5);
			}
			else if(array[0] == 7 && array[1] == 7 && array[2] == 7)
			{
				//This else if loop checks if all the elements of the array are 7.
				//If all the elements are 7, then the user bet is multiplied by 10.
				cout<<"You won "<< bet * 10 << " coins!" <<endl;
				//The statement below updates the coins user posses.
				coins = coins + (bet * 10);
			}
			else
			{
				//This else statement is the basic winning statement. Which basically means that if two of the three elements match, then the user gets their bet multiplied by 2.
				cout<<"You won "<< bet * 2 << " coins!" << endl;
				//The statement below updates the coins user posses.
				coins = coins + (bet * 2);
			}
		}
		else
		{
			//This else statement is the part of the if statement at the very beginning.
			//This else statement is entered when none of the elements of the array match. In this case the user just loses the bet amount and gains nothing.
			cout<<"You did not win." << endl;
		}
	}while(coins !=0 && coins >= 1);//This while condition checks if the coins are not zero and are greater than or equal to 1. If they are, then the loop comes to an end.
	return 0;
}
