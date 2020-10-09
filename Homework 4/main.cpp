#include <fstream>
#include <iostream>
#include <cctype>
#include <string>
#include "WordTree.h"
#include <cassert>
using namespace std;

int main() 
{
	WordTree w;
	w.add("Harry");
	w.add("Niall");
	w.add("niall");
	w.add("Liam");
	w.add("Louis");
	w.add("Harry");
	w.add("Niall");
	w.add("Zayn");
	cout << "\noriginal:" << endl;
	cout << w << endl;

	WordTree y = w;
	cout << "copy:" << endl;
	cout << y;

	WordTree k;
	k.add("Kim");
	k.add("Kanye");
	k.add("Kanye");
	k.add("Kanye");

	cout << "\noriginal:" << endl;
	cout << k << endl;

	cout << "assignment:" << endl;
	y = k;
	cout << y;

	WordTree x;
	x = w;
	cout << "\nempty assignment:\n" << x;

	assert(k.distinctWords() == 2);
	assert(k.totalWords() == 4);
	cout << "\nAll tests passed" << endl;
	
	return 0;
}