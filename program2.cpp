/*
     Title:    Project2
     Author:   Xavier Mathews 
     Date:      11/01/23
     Purpose:   Traveling Salesman - find the lowest cost
                tour when traveling from US to 8 other countries
                and then back to US.
*/

#include <iostream>
#include <fstream>
#include <cmath>
#include "GraphMatrix.h"
using namespace std;

const int SIZE = 10;
const string COUNTRY_CODES[SIZE]={"AU", "BR", "CA", "CN", "GL", "IT", "NA", "RU", "US", "US"};

/*
	Use the following structure to store each tour & its cost
	You will create an array of Tour variables in your program
*/
struct Tour
{
	string tour[SIZE];
	int cost;
};

//Function Prototypes
int searchCountryCode(string);
GraphMatrix* readFileMakeMatrix();
void printStringArray(string* arr, int size);
void lexicographic(string *,int, Tour*, GraphMatrix*);
void saveTour(Tour*, GraphMatrix*, int, string*);
void findLowest(Tour*);

int main()
{	
	Tour *tourOptions = new Tour[40320];
	//read in the flight information from the file and then create the weight matrix
	GraphMatrix * matrix = readFileMakeMatrix();
	string * countries = new string[SIZE-2];
	
	cout << "\n\n*************************COUNTRIES*******************\n";
	for(int x=0; x < SIZE-2; x++)
	{
		countries[x] = COUNTRY_CODES[x];
		cout << countries[x] << endl;
	}
	
	
	//generate all possible tours (starting & ending with "US") using lexicographic permute algorithm
	//you will need to call your lexicographic function, sending the modified countries array with the 8 country codes
	lexicographic(countries, 8 , tourOptions, matrix); //calls the lexicographic function
	
	cout << "\n\n*************************SOLUTION*******************\n";
	//find the lowest cost tour and print it out (including the cost)
	findLowest(tourOptions);
	
	cout << "\nHappy Traveling!\n";
	
	//don't forget to release anything that was dynamically allocated!
	delete[] tourOptions;
	delete matrix;
	delete[] countries;

	return 0;
}
/*
	Function: searchCountryCode
	Parameters: a string with the country code like "BR"
	Returns: an integer representing this country's index in the GraphMatrix.
	For example, if the parameter is "BR", it should return index 1.  If the parameter is "CA", it should return index 3
	It is returning the index value in the COUNTRY_CODES array.	
	Implement this search function with the binary search algorithm!
*/
int searchCountryCode(string code)
{
	int i=0;
	while(COUNTRY_CODES[i]!=code){ //while loop is a brute force search COUNTRY_CODES until code is found
		i++;
	}
	return i; //once found return int index of code on the COUNTRY_CODES ARRAY
}

/*
	Title: readFileMakeMatrix
	Purpose:  this function will read the data in flights.txt and create an
	adjacency matrix with the couuntry codes.  The matrix values will be the
	cost of the flights.
*/
GraphMatrix* readFileMakeMatrix()
{
	ifstream inFile;
	GraphMatrix * matrix = new GraphMatrix(SIZE-1);
	cout << "\nCreated the matrix.";
	string country1, country2;
	int price;
	inFile.open("flights.txt");
	cout << "\nReading from flights.txt\n";
	if(inFile)
	{
		while(inFile >> country1)
		{
			inFile >> country2;
			inFile >> price;
			//add price to graph matrix
			matrix->addEdge(searchCountryCode(country1), searchCountryCode(country2), price);
			cout << "\nAdded edge from " << country1 << " to " << country2 << " with cost of $" << price;
		}
	}
	else
		cout << "\nSorry, I am unable to open the file.\n";
	inFile.close();
	cout << "\n\nFLIGHT PRICE GRAPH MATRIX\n";
	matrix->printGraph();
	cout << endl;
	return matrix;
}

/*
	Title: printStringArray
	Purpose:  this function will print the array of strings with a space between each string
*/
void printStringArray(string* arr, int size)
{
	for(int x=0; x<size; x++)
	{
		cout << arr[x] << " ";
	}

}

/*
	Title: lexicographic
	Purpose:  this function will use the lexicographic permute algorithm to generate all possibles tours and save them to tourOptions array.
	Args: string* countries: an array of string contain all countries in tour execpt US
		  int size: size of countries array
		  Tour* tourOptions: a list of tours containing it permutation and total weight
		  GraphMatrix* matrix: A weighted graphed matrix that contains all flights and the cost to fly to the country
*/
void lexicographic(string* countries, int size, Tour* tourOptions, GraphMatrix* matrix)
{
	string tempTour[size]; //create a string array called tempTour used to create all possible permutations.	
	for (int i = 0; i < size; i++){ //for loop used to assign all elements in countries to tempTour
		tempTour[i] = countries[i];
	}
	int index = 0; //indicates where to save tempTour in tourOptions array, when calling saveTour function
	bool isDone = false;
	int iterationCount = 1;
	int lowBound = 0;		//The "i" value
	int uppBound = 0;		//The "j" value
	
	//Temporary variables to store values during a swap
	string tempCountry;
	string swapCountries[size];
	
	cout << "\nLEXICOGRAPHIC ALGORITHM";
	
	while(!isDone)
	{
		/*
			---PRINT Current Set of Tour---
		*/

		cout << "\n" << iterationCount << ":\t";
		printStringArray(tempTour,size); //called the printStringArray to print out tempTour array

		saveTour(tourOptions, matrix, index, tempTour); //calls saveTour to save current permutation of tour to tourOptions with its total weight
		index++; //increment by 1 so we can save next permutation in a empty spot in tourOptions
		iterationCount++;

		/*
			---FIND lowBound OR DETERMINE IF DONE---
		*/
		isDone = true;
		for(int i = 0; i < size-1; i++)
		{
			if(tempTour[i]<tempTour[i+1])
			{
				isDone = false;
				lowBound = i;
			}
		}
		if(isDone)
			continue;
		/*
			---FIND uppBound---
		*/
		for(int j = size-1; j>0; j--)
		{
			if(tempTour[j] > tempTour[lowBound])
			{
				uppBound = j;
				break;
			}
		}
		/*
			---SWAP ELEMENTS---
		*/
		tempCountry = tempTour[lowBound];
		tempTour[lowBound] = tempTour[uppBound];
		tempTour[uppBound] = tempCountry;

		for(int i = 0; i < size; i++){ //for loop used to set all swapCountries element the same as tempTour
			swapCountries[i] = tempTour[i]; //so swapCountries can be used to create to the next lexicographic premutation
		}
		
		/*
			---REVERSE ELEMENTS FROM lowBound+1 TO l-1---
		*/
		for(int i = 1;i < size-lowBound;i++)
		{
			swapCountries[lowBound+i] = tempTour[size-i];
		}
		for(int i = 1;i < size-lowBound;i++)
		{
			tempTour[lowBound+i] = swapCountries[lowBound+i];
		}
	}
}
/*
	Title: saveTour
	Purpose:  this function saves the current permutation of tour, and total cost of the tour to tourOptions, and prints the weight/flight cost between countries and total tour cost.
	Args: Tour* tourOptions: a list of tours containing it permutation and total weight
		  GraphMatrix* matrix: A weighted graphed matrix that contains all flights and the cost to fly to the country
		  int index: index to identify where you want to save Tour to on tourOptions
		  string* tempTour: the Tour you want to save
*/
void saveTour(Tour* tourOptions, GraphMatrix* matrix, int index, string* tempTour){
	int vertice1, vertice2; //vertice variable will be used to to represent countries index in Country_Codes
							// once found will be used in getWeight function in matrix to weight of flight between country
	int weight;
	int total = 0; //total weight/cost of tour

	tourOptions[index].tour[0] = "US";
	tourOptions[index].tour[9] = "US"; //the first and last country visted will be US in all permutations.

	for(int i=0; i<8;i++){ //for loop saves current permutation (tempTour) in tourOptions
		tourOptions[index].tour[i+1] = tempTour[i];
	}

	for(int i = 1; i <= 9; i++){//for loop get the weight(flight cost) between countries in tour array then adds it to total
	 	vertice1 = searchCountryCode(tourOptions[index].tour[i-1]); //calls searchCountryCode to get country code of country its departing from
		vertice2 = searchCountryCode(tourOptions[index].tour[i]);//calls searchCountryCode to get country code of country its arriving to
		weight = matrix->getWeight(vertice1,vertice2); //calls getweight function in matrix to gets the weight/flight cost between the two countries 
		cout << " " << weight << " "; //print weight for user to see
		total += weight; //add weight to total
	}

	tourOptions[index].cost = total; //save total cost of tour in tour Options
	cout << " total weight: " << total; //print total weight for user to see
}


/*
	Title: findLowest
	Purpose: This function searchest tourOptions and prints the tour that would cost the lowest
	Args: Tour* tourOptions: a list of tours containing it permutation and total weight
*/
void findLowest(Tour* tourOptions){
	int min = 0; //set the lowest to first index in tourOptions
	for(int i=1; i<40320;i++){ //for loop will iterate through entirety of tourOptions
		if(tourOptions[min].cost > tourOptions[i].cost){ //if current tour cost lower than current min tour
			min =i ; //replace min index with current index (i)
			cout << "New Lowest is " << tourOptions[min].cost << " with tour "; //notify user that new lowest is found then print tour
			printStringArray(tourOptions[min].tour,10);
			cout << "\n";
		}
	}
	cout << "\n\n********************************************************\n";
	cout << "The tour with the lowest cost of $" << tourOptions[min].cost << " is "; //once the for loop has been iterated entirely print the tour that cost the lowest with it's cost
	printStringArray(tourOptions[min].tour,10);
}
