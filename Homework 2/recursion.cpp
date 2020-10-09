
#include <string>
using namespace std;


// NAME:		product
// INPUT:		positive integer, positive integer
// OUTPUT:		integer
// DESCRIPTION: Returns the product of two non-negative integers, m and n,
//				using only repeated addition.
int product(unsigned int m, unsigned int n)
{
	//base case
	if (n == 0 || m == 0)
		return 0;

	//add number n times
	m += product(m, n - 1);
	return m;
}


// NAME:		digitCount
// INPUT:		integer, integer to find
// OUTPUT:		integer of how many times digit was found
// DESCRIPTION: Returns the number of occurrences of digit in the decimal
//				representation of num. digit is an int between 0 and 9
//				inclusive.

int digitCount(int num, int digit)
{
	int counter = 0;

	//base case
	if (num == 0)
		return 0;

	//isolate last number and count
	else if (num % 10 == digit)
		counter++;

	//pass num without last number
	counter += digitCount(num / 10, digit);

	return counter; 
}


// NAME:		terribleTriplets
// INPUT:		string
// OUTPUT:		updated string
// DESCRIPTION: Returns a string where the same characters next each other in
//				string n are separated by "333"

string terribleTriplets(string n)
{
	int temp = 0;
	string m;

	//base case
	if (n.size() == 0)
		return "";

	//last letter should not be compared
	if (n.size() == 1)
		return n.substr(n.size() - 1);

	//find double letters and insert 333
	if (n.substr(temp, 1) == n.substr(temp + 1, 1))
	{
		m = n.substr(temp, 1) + "333";
	}

	//add single letter if not a double
	else
		m = n.substr(temp, 1);

	//check rest of the string 
	m += terribleTriplets(n.substr(temp + 1));
	
	return m; 
}


// NAME:		hipToBeSquare
// INPUT:		string
// OUTPUT:		string in between []
// DESCRIPTION:	str contains a single pair of square brackets, return a new
//				string made of only the square brackets and whatever those
//				square brackets contain

string hipToBeSquare(string str)
{
	string m = "";

	//base case
	if (str.size() == 0)
		return "";

	//find first bracket
	if (str.substr(0, 1) == "[")
	{
		//find second bracket
		if (str.substr(str.size() - 1) == "]")
		{
			return str;
		}
		
		//continue until you hit second bracket
		else
		{
			m = hipToBeSquare(str.substr(0, str.size()-1));
		}
	}

	//haven't found first bracket, continue looking
	else if(m.size() == 0)
	{
		m = hipToBeSquare(str.substr(1));
	}

	return m;
}


// NAME:		summerSolstice
// INPUT:		integer array, array size, integer
// OUTPUT:		true if a combination of numbers equals target, false otherwise
// DESCRIPTION: Return true if the total of any combination of elements in
//				the array a equals the value of the target.

bool summerSolstice(const int a[], int size, int target)
{
	if (target == 0)
		return true;

	if (size == 0)
		return false;
	
	//if a number is greater than target, exclude it
	if (a[size - 1] > target)
		return summerSolstice(a, size - 1, target);

	//include last element in target and check next element
	if (summerSolstice(a, size - 1, target))
		return true;

	//exclude last element from target and check next element
	if (summerSolstice(a, size - 1, target - a[size - 1]))
		return true;

	return false;
}


// NAME:		isThisTheEnd
// INPUT:		string array, number of rows, number of columns,
//				start row, start col, end row, end col
// OUTPUT:		true if maze can be solved, false otherwise
// DESCRIPTION: Return true if there is a path from (sr,sc) to 
//				(er,ec) through the maze; return false otherwise

bool isThisTheEnd(string maze[], int nRows, int nCols,
	int sr, int sc, int er, int ec)
{
	//base case, found end
	if (sr == er && sc == ec)
		return true;

	//drop crumb
	maze[sr][sc] = '#';

	//check row above
	if (maze[sr - 1][sc] == '.')
	{
		bool temp = isThisTheEnd(maze, nRows, nCols, sr - 1, sc, er, ec);
		if (temp)
			return true;
	}

	//check row below
	if (maze[sr + 1][sc] == '.')
	{
		bool temp = isThisTheEnd(maze, nRows, nCols, sr + 1, sc, er, ec);
		if (temp)
			return true;
	}

	//check left col
	if (maze[sr][sc - 1] == '.')
	{
		bool temp = isThisTheEnd(maze, nRows, nCols, sr, sc - 1, er, ec);
		if (temp)
			return true;
	}

	//check right col
	if (maze[sr][sc + 1] == '.')
	{
		bool temp = isThisTheEnd(maze, nRows, nCols, sr, sc + 1, er, ec);
		if (temp)
			return true;
	}
		
	return false;
}
