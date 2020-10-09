#include "LinkedList.h"
#include <iostream>
using namespace std;

//constructor
LinkedList::LinkedList()
{
	//set first pointer to null
	head = nullptr;
}

//copy constructor
LinkedList::LinkedList(const LinkedList& rhs)
{
	//create temp pointers for traversal
	Node* oldPointer = rhs.head;
	Node* temp = nullptr;
	Node* newPointer;

	//loop through each rhs node
	while (oldPointer != nullptr)
	{
		//allocate memory for new linked list and initialize head
		newPointer = new Node;
		if(head == nullptr)
			head = newPointer;

		//copy value
		newPointer->value = oldPointer->value;
		newPointer->next = nullptr;

		//set next
		if (temp != nullptr)
			temp->next = newPointer;

		temp = newPointer;
		
		//increment
		oldPointer = oldPointer->next; 

	}
}

//destructor
LinkedList::~LinkedList()
{
	Node* p;
	p = head;
	Node* temp;
	while (p != nullptr)
	{
		temp = p->next;

		//delete each node
		delete p;
		p = temp;
	}
}

//assignment operator
const LinkedList& LinkedList::operator=(const LinkedList& rhs)
{
	//create temp pointer for rhs traversal
	Node* oldPointer = rhs.head;

	//if equal, don't do anything
	if(this == &rhs)
		return *this;
	
	//free memory 
	Node* newPointer;
	newPointer = head;
	Node* temp = nullptr;
	while (newPointer != nullptr)
	{
		temp = newPointer->next;
		delete newPointer;
		newPointer = temp;
	}

	//loop through each rhs node
	while (oldPointer != nullptr)
	{
		//allocate memory for new linked list and initialize head
		newPointer = new Node;
		if (head == nullptr)
			head = newPointer;

		//copy value and assign next
		newPointer->value = oldPointer->value;
		newPointer->next = nullptr;
		if (temp != nullptr)
			temp->next = newPointer;

		temp = newPointer;

		oldPointer = oldPointer->next;

	}

	return *this;
}

void LinkedList::insertToFront(const ItemType& val)
{
	//allocate memory for new node
	Node* p;
	p = new Node;

	//set value and link to old top node
	p->value = val;
	p->next = head;

	//assign head to new top node
	head = p;
}

void LinkedList::printList() const
{
	if (head == nullptr)
		return;

	//assign pointer to head
	Node* temp;
	temp = head;

	//loop through and print
	while (temp != nullptr)
	{
		cout << temp->value << " ";
		temp = temp->next;
	}

	cout << endl;
}

bool LinkedList::get(int i, ItemType& item) const
{
	Node* p;
	p = head;
	int counter = 0;

	//loop through nodes
	while (p != nullptr)
	{
		//if land on element i, change item to value
		if (counter == i)
		{
			item = p->value;
			return true;
		}

		p = p->next;
		counter++;
	}

	return false;
}

void LinkedList::reverseList()
{
	Node *curr = head;
	Node *prev = nullptr;
	Node *next = nullptr;

	while (curr != nullptr)
	{
		//store next node
		next = curr->next;

		//reverse node's next pointer
		curr->next = prev;

		//move prev pointer to current node
		prev = curr;

		//move current to next node
		curr = next;
	}

	if (head != nullptr)
		head = prev;
}

void LinkedList::printReverse() const
{
	if (head == nullptr)
		return;

	//run through list backwards and print
	for (int i = size()-1; i >= 0; i--)
	{
		string x;
		get(i, x);
		cout << x << " ";
	}

	cout << endl;
}

void LinkedList::append(const LinkedList& other)
{
	Node* temp = head;
	Node* otherTemp = other.head;
	Node* hold = nullptr;
	int counter = other.size();

	//stop at end, temp will point to null
	while (temp != nullptr)
	{
		if (temp->next == nullptr)
			break;
		temp = temp->next;
	}

	//copy list, adding after last node
	while (counter--)
	{
		//allocate memory
		hold = new Node;

		//copy value
		hold->value = otherTemp->value;

		//set next
		hold->next = nullptr;

		//check if head is null
		if (head == nullptr) 
			head = hold;

		//link nodes
		if (temp != nullptr) 
			temp->next = hold;
		
		temp = hold;

		otherTemp = otherTemp->next;
		
	}

}

void LinkedList::swap(LinkedList& other)
{
	//switch heads
	Node* temp = other.head;
	other.head = head;
	head = temp;
	
}

int LinkedList::size() const
{
	Node* p;
	p = head;
	int counter = 0;

	while (p != nullptr)
	{
		//count each element
		counter++;
		p = p->next;
	}
	return counter;
}