#include "LinkedList.h"
#include <iostream>
#include <assert.h>
using namespace std;

int main()
{
	//get, insertToFront, and size test
	LinkedList ls;
	ls.insertToFront("Hawkeye");
	ls.insertToFront("Thor");
	ls.insertToFront("Hulk");
	ls.insertToFront("Black Widow");
	ls.insertToFront("Iron Man");
	ls.insertToFront("Captain America");
	for (int k = 0; k < ls.size(); k++)
	{
		string x;
		ls.get(k, x);
		cout << x << endl;
	}
	
	//printList and printReverse test
	LinkedList ls1;
	ls1.insertToFront("The Mandalorian");
	ls1.insertToFront("Baby Yoda");
	ls1.insertToFront("Cara Dune");
	ls1.insertToFront("Greef Karga");
	ls1.printList();
	ls1.printReverse();


	//append test
	LinkedList e1;
	e1.insertToFront("Athos");
	e1.insertToFront("Porthos");
	e1.insertToFront("Aramis");
	LinkedList e2;
	e2.insertToFront("Robin");
	e2.insertToFront("Batman");
	e1.append(e2); // adds contents of e2 to the end of e1
	string a;
	assert(e1.size() == 5 && e1.get(3, a) && a == "Batman");
	assert(e2.size() == 2 && e2.get(1, a) && a == "Robin");



	//reverseList test
	LinkedList e3;
	e3.insertToFront("Jim");
	e3.insertToFront("Oz");
	e3.insertToFront("Paul");
	e3.insertToFront("Kevin");
	e3.reverseList(); // reverses the contents of e1
	string b;
	assert(e3.size() == 4 && e3.get(0, b) && b == "Jim");

	//swap test
	LinkedList e4;
	e4.insertToFront("A");
	e4.insertToFront("B");
	e4.insertToFront("C");
	e4.insertToFront("D");
	LinkedList e5;
	e5.insertToFront("X");
	e5.insertToFront("Y");
	e5.insertToFront("Z");
	e4.swap(e5); // exchange contents of e1 and e2
	string s;
	assert(e4.size() == 3 && e4.get(0, s) && s == "Z");
	assert(e5.size() == 4 && e5.get(2, s) && s == "B");

	cout << "All tests passed" << endl;
}