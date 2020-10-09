#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <string>

typedef std::string ItemType;
struct Node 
{
	ItemType value;
	Node* next;
};

class LinkedList 
{
  private:
	Node* head;

  public:
	// default constructor
	LinkedList();

	// copy constructor
	LinkedList(const LinkedList& rhs);

	// destructor
	~LinkedList();

	// assignment operator
	const LinkedList& operator=(const LinkedList& rhs);


	////////////////////////////////////////////////////////////
	//accessor functions
	////////////////////////////////////////////////////////////

	// NAME: printList
	// INPUT: none
	// OUTPUT: none
	// DESCRIPTION: prints the LinkedList
	void printList() const;

	// NAME: get
	// INPUT: integer for index in array, reference to ItemType to be changed
	// OUTPUT: true if element exists, false otherwise
	// DESCRIPTION: sets item to the value at position i in this
	// LinkedList and return true, returns false if there is no element i
	bool get(int i, ItemType& item) const;

	// NAME: printReverse
	// INPUT: none
	// OUTPUT: none
	// DESCRIPTION: prints the LinkedList in reverse order
	void printReverse() const;

	// NAME: size
	// INPUT: none
	// OUTPUT: integer for number of elements
	// DESCRIPTION: returns the number of items in the Linked List.
	int size() const;


	////////////////////////////////////////////////////////////
	// mutator functions 
	////////////////////////////////////////////////////////////

	// NAME: insertToFront
	// INPUT: constant reference to an ItemType value
	// OUTPUT: none
	// DESCRIPTION: inserts val at the front of the list
	void insertToFront(const ItemType& val);

	// NAME: reverseList
	// INPUT: none
	// OUTPUT: none
	// DESCRIPTION: reverses the LinkedList
	void reverseList();

	// NAME: append
	// INPUT: constant reference to a LinkedList
	// OUTPUT: none
	// DESCRIPTION: appends the values of other onto the end of this LinkedList.
	void append(const LinkedList& other);

	// NAME: swap
	// INPUT: reference to a LinkedList
	// OUTPUT: none
	// DESCRIPTION: exchange the contents of this LinkedList with the other one.
	void swap(LinkedList& other);

};

#endif
