#include <iostream>
#include <fstream>
#include <istream>
#include <string>
using namespace std;

const int MAXRESULTS = 20;    // Max matches that can be found
const int MAXDICTWORDS = 30000; // Max words that can be read in

// NAME:		createDictHelp
// INPUT:		dictionary file, string array, integer
// OUTPUT:		number of elements in dict array
// DESCRIPTION: Puts each string in dictfile into the array dict. Returns the number of 
//              words read into dict, which should not be larger than MAXDICTWORDS

int createDictHelp(istream& dictfile1, string dict[], int counter)
{
    string word;

    //base case
    if (!(getline(dictfile1, word)))
        return counter;
    //copy to dictionary and increment counter
    dict[counter] = word;
    counter++;

    //if counter is max, return 
    if (counter == MAXDICTWORDS)
        return counter;
    
    //recurse
    counter = createDictHelp(dictfile1, dict, counter);

    return counter;
}

// NAME:		createDict
// INPUT:		dictionary file, string array
// OUTPUT:		number of elements in array
// DESCRIPTION: calls helper function to put each string in dictfile into array
			
int createDict(istream& dictfile, string dict[])
{
    int i = 0;
    //call helper
    return createDictHelp(dictfile, dict, i);
}

int mixer(string prefix, string rest, const string dict[], int size, string results[], int count);

// NAME:		characterMixer
// INPUT:		string, const string array, integer, string array
// OUTPUT:		number of matched words
// DESCRIPTION: calls helper function to returns the number of matched words found in dict array.

int characterMixer(string word, const string dict[], int size, string results[])
{
    int count = 0;
    string prefix = "";

    //check constraints for dictionary and call helper
    if (size > MAXDICTWORDS)
        size = MAXDICTWORDS;
   
    return mixer(prefix, word, dict, size, results, count);	
}

// NAME:		check
// INPUT:		string (reference), const string array, integer)
// OUTPUT:		true if string is in array, false otherwise
// DESCRIPTION: loops through array and checks if the string is in the array
//			
bool check(string &word, const string arr[], int size)
{
    //base case
    if (size == 0)
        return false;

    //if there is a match return true
    if (word == arr[size - 1])
        return true;

    //check next word
    else
        return check(word, arr, size - 1);

    return false;
}

// NAME:		Loop
// INPUT:		integer, integer, string, string, const string array, integer, string array, integer (reference)
// OUTPUT:		none
// DESCRIPTION: loops through all of the letters of rest and scrambles the letters 
//			
void Loop(int i, int max, string prefix, string rest, const string dict[], int size, string results[], int &count)
{
    //base case
    if (i >= max)
        return;

    //store original variables
    string temp = prefix;
    string temp1 = rest;

    //add one char of rest to prefix and remove char from rest
    prefix += rest.substr(i, 1);
    rest = rest.substr(0, i) + rest.substr(i + 1);

    //call these vars in mixer
    count = mixer(prefix, rest, dict, size, results, count);
  
    //reset values
    prefix = temp;
    rest = temp1;

    //loop
    Loop(i + 1, rest.size(), prefix, rest, dict, size, results, count);
}

// NAME:		mixer
// INPUT:		string, string, const string array, integer, string array, integer
// OUTPUT:		number of matched words
// DESCRIPTION: Puts all the possibilities of word which are found in dict into results. Returns
//              the number of matched words found.This number should not be larger than
//              MAXRESULTS since that is the size of the array. The size is the number of words
//              inside the dict array.

int mixer(string prefix, string rest, const string dict[], int size, string results[], int count)
{
    //base case
    if (count == MAXRESULTS)
        return count;

    // check scrambled word
    if (rest.size() == 0)
    {
        //match in dict and not duplicate in results
        if (check(prefix, dict, size) && !check(prefix, results, count))
        {
            results[count] = prefix;
            count++;
            return count;
        }    
    }

    //for loop --> recursion
    else
        Loop(0, rest.size(), prefix, rest, dict, size, results, count);

    return count;
}

// NAME:		viewAnswers
// INPUT:		const string array, integer
// OUTPUT:		none
// DESCRIPTION: Displays size number of strings from results. The results can be 
//              printed in any order.

void viewAnswers(const string results[], int size)
{
    //base case
    if (size == 0)
        return;

    if (size > MAXRESULTS)
        size = MAXRESULTS;

    //recurse
    viewAnswers(results, size - 1);

    //if empty element, return
    if (results[size - 1] == "")
        cout << results[size - 1] << endl;

    //print
    else
        cout << "Matching word " << results[size-1] << endl;
}

int main()
{

}