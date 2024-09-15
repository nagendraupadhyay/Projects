//Nagendra Upadhyay
//Part A 03.03.21

//Lab 3 part A asks for userinput and gives them an output of some random numbers.

#include <iostream>
#include <string>
#include <random> 
using namespace std;

int main() 
{
	//Declaring variable for userinput.
	int seed;
	int i;
	int array[3];

	//Asking user for a number input.
	cout << "Input a seed: ";

	//Storing the input in seed.
	cin >> seed;

	//Declaring random number generator using userinput.
	default_random_engine rng(seed);
	//Setting the range of the random numbers.
	uniform_int_distribution<int> userinput_seed(2, 7);

	//This for loop gives user 3 numbers as an output from the random number generator.
	for(i = 0; i < 3; ++i)
	{
		//The array below is storing the numbers that were obtained randomly.
		array[i] = userinput_seed(rng);
		cout << ' ' << array[i];
	}
	cout << endl;
	return 0;
}
