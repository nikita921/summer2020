#include "BirthdayParty.h"
#include <iostream>
#include <string>
using namespace std;

// Create an empty BirthdayParty list
BirthdayParty::BirthdayParty()
{
	//set head and tail pointer to null
	head = nullptr;
	tail = nullptr;
}

// copy constructor
BirthdayParty::BirthdayParty(const BirthdayParty& rhs)
{
	if (rhs.head == nullptr)
	{
		head = tail = nullptr;		//initialize
	}

	else
	{
		//set values for first node
		head = new Node;
		tail = head;
		head->firstName = rhs.head->firstName;
		head->lastName = rhs.head->lastName;
		head->value = rhs.head->value;
		head->prev = nullptr;
		head->next = nullptr;

		//create temp pointers for traversal
		Node* oldPointer = rhs.head->next;

		while (oldPointer != nullptr)
		{
			//allocate memory
			tail->next = new Node;

			//copy values
			tail->next->firstName = oldPointer->firstName;
			tail->next->lastName = oldPointer->lastName;
			tail->next->value = oldPointer->value;

			//set next and prev
			tail->next->next = nullptr;
			tail->next->prev = tail;

			//increment
			tail = tail->next;
			oldPointer = oldPointer->next;
		}
	}
}

// destructor
BirthdayParty::~BirthdayParty()
{
	//create temporary pointers to help traversal
	Node* p = head;
	Node* temp;
	while (p != nullptr)
	{
		temp = p->next;

		//delete each node
		delete p;
		p = temp;
	}
}

// assignment operator -> algorithm adapted from Prof. Ambrosio's hw1 solution
BirthdayParty& BirthdayParty::operator=(const BirthdayParty& rhs)
{
	//check if already equal
	if (this == &rhs) 
		return *this;

	//make copy of rhs and switch lists
	BirthdayParty temp = rhs;
	changeGuestList(temp);
	return *this;
}

bool BirthdayParty::noInvitees() const
{
	//if head doesn't point to anything, it is empty
	if (head == nullptr)
		return true;

	return false;
}

int BirthdayParty::whosOnTheGuestList() const
{
	Node* p = head;
	int counter = 0;

	while (p != nullptr)
	{
		//count each element
		counter++;
		p = p->next;
	}
	return counter;

}

bool BirthdayParty::addInvitee(const std::string& firstName, const std::string& lastName, const BirthdayType& value)
{
	Node* p = head;
	Node* temp = nullptr;
	bool before = false;
	bool found = false;

	if (head == nullptr)
	{
		//allocate memory for new node
		p = new Node;

		//set value and link
		p->firstName = firstName;
		p->lastName = lastName;
		p->value = value;
		p->next = head;
		p->prev = nullptr;
		head = p;
		tail = p;
		return true;

	}

	//loop
	while (p != nullptr)
	{
		//find node after where node should be inserted
		if (p->lastName > lastName)
		{
			//indicate location of where node should be inserted
			before = true;
			found = true;
		}

		//check if last names are same
		while (p->lastName == lastName && !found)
		{
			//return false if full name is the same 
			if (p->firstName == firstName)
				return false;

			//find node after where node should be inserted
			if (p->firstName > firstName)
			{
				//indicate location of where node should be inserted
				before = true;
				found = true;
			}

			//at last node
			else if (p->next == nullptr)
			{
				//location should be after node stopped at
				before = false;
				found = true;
			}

			//only increment if location has not be found
			if(!found)
				p = p->next;
		}

		//break if location has been found or is at last node
		if (found || p->next == nullptr)
			break;

		p = p->next;
	}

	//allocate memory, copy values, and set pointers
	temp = new Node;
	temp->firstName = firstName;
	temp->lastName = lastName;
	temp->value = value;
	temp->next = nullptr;
	temp->prev = nullptr;

	//case 1: place node after p
	if (!before)
	{
		//set prev pointer first
		temp->prev = p;
		
		//if there is a valid node after   
		if (p->next)
		{
			//set next
			temp->next = p->next;
			//link next previous pointer
			p->next->prev = temp;
		}
		
		//it is the last node
		else
			tail = temp;

		//link last pointer
		p->next = temp;
	}

	//case 2: place node before p
	else if (before)
	{
		//set next pointer first
		temp->next = p;

		//if there is a valid pointer before
		if (p->prev)
		{
			//set previous
			temp->prev = p->prev;
			//link the previous next pointer
			p->prev->next = temp;
		}
		
		//it is the first node
		else
			head = temp;

		//link last pointer
		p->prev = temp;
	}
	
	return true;
}

bool BirthdayParty::modifyInvitee(const std::string& firstName, const std::string& lastName, const BirthdayType& value)
{
	Node* p = head;

	//loop through nodes
	while (p != nullptr)
	{
		//if found
		if (p->firstName == firstName && p->lastName == lastName)
		{
			//change value
			p->value = value;
			return true;
		}

		p = p->next;
	}


	return false;
}

bool BirthdayParty::addOrModify(const std::string& firstName, const std::string& lastName, const BirthdayType& value)
{
	//if on guest list, then modify
	if (personOnGuestList(firstName, lastName))
		modifyInvitee(firstName, lastName, value);

	//if not, then add
	else
		addInvitee(firstName, lastName, value);

	return true;
}

bool BirthdayParty::dropFromGuestList(const std::string& firstName, const std::string& lastName)
{
	//check if list is empty
	if (head == nullptr) 
		return false;

	//check first node
	if (head->firstName == firstName && head->lastName == lastName)
	{
		//create temporary pointer to remember target node location
		Node* killMe = head;
		if (tail == head)
			tail = head = nullptr;

		else 
		{
			head = killMe->next;
			head->prev = nullptr;
		}
		delete killMe;
		return true;
	}

	Node* p = head;

	//loop through nodes
	while (p != nullptr)
	{
		//end on node before target
		if (p->next != nullptr && p->next->firstName == firstName && p->next->lastName == lastName)		
			break;
		p = p->next;
	}

	if (p != nullptr)
	{
		//temp pointer to target node
		Node* killMe = p->next;

		//link previous node to following node then delete
		p->next = killMe->next;
		if (killMe->next != nullptr)
			killMe->next->prev = p;
		else
			tail = p;
		delete killMe;
		return true;
	}

	return false;
}

bool BirthdayParty::personOnGuestList(const std::string& firstName, const std::string& lastName) const
{
	Node* p = head;

	//loop through nodes
	while (p != nullptr)
	{
		//find matching node
		if (p->firstName == firstName && p->lastName == lastName)
			return true;

		p = p->next;
	}

	return false;
}

bool BirthdayParty::checkGuestList(const std::string& firstName, const std::string& lastName, BirthdayType& value) const
{
	Node* p = head;
	
	//loop through nodes
	while (p != nullptr)
	{
		//if matching node is found
		if (p->firstName == firstName && p->lastName == lastName)
		{
			//set value
			value = p->value;
			return true;
		}
		
		p = p->next;
	}
	return false;
}

bool BirthdayParty::selectInvitee(int i, std::string& firstName, std::string& lastName, BirthdayType& value) const
{
	if (i >= 0 && i < whosOnTheGuestList())
	{
		Node* p;
		p = head;
		int counter = 0;

		//loop through nodes
		while (p != nullptr)
		{
			//if land on element i, copy values
			if (counter == i)
			{
				firstName = p->firstName;
				lastName = p->lastName;
				value = p->value;
				return true;
			}

			p = p->next;
			counter++;
		}
	}
	return false;
}

void BirthdayParty::changeGuestList(BirthdayParty& other)
{
	//switch heads
	Node* temp = other.head;
	other.head = head;
	head = temp;
}

bool combineGuestLists(const BirthdayParty& bpOne, const BirthdayParty& bpTwo, BirthdayParty& bpJoined)
{
	BirthdayParty temp = bpJoined;
	int limit = temp.whosOnTheGuestList();

	//empty temp
	if (limit)
	{
		for (int k = 0; k < limit; k++)
		{
			string first = "";
			string last = "";
			BirthdayType val;

			temp.selectInvitee(0, first, last, val);
			//delete node
			temp.dropFromGuestList(first, last);
		}
	}

	//find bpOne and bpTwo sizes, and add temp variables
	int limit1 = bpOne.whosOnTheGuestList();
	int limit2 = bpTwo.whosOnTheGuestList();
	int counter = 0;
	bool add = true;
	bool return1 = true;
	
	for (int n = 0; n < limit2; n++)
	{
		//reset counter
		counter = 0;
		string first = "";
		string last = "";
		BirthdayType val;

		string first1 = "";
		string last1 = "";
		BirthdayType val1;

		//get n node's values from bpTwo
		bpTwo.selectInvitee(n, first, last, val);
		
		//check if same name exists in bpOne
		if (bpOne.personOnGuestList(first, last))
		{
			//loop through bpOne nodes
			while (counter < limit1)
			{
				//check if BirthdayType is different
				bpOne.selectInvitee(counter, first1, last1, val1);
				if (first == first1 && last == last1 && val1 != val)
					{
						add = false;
						return1 = false;
					}

				counter++;
			}
		}
		
		//only add to temp list if add is true and it is not a duplicate
		if(!temp.personOnGuestList(first, last) && add)
			temp.addInvitee(first, last, val);

		//reset add
		add = true;
	}

	for (int i = 0; i < limit1; i++)
	{
		//reset counter
		counter = 0;
		string first = "";
		string last = "";
		BirthdayType val;

		string first1 = "";
		string last1 = "";
		BirthdayType val1;

		//get i node's values from bpTwo
		bpOne.selectInvitee(i, first, last, val);

		//check if same name exists in bpTwo
		if (bpTwo.personOnGuestList(first, last))
		{
			//loop through bpTwo nodes
			while (counter < limit2)
			{
				//check if BirthdayType is different
				bpTwo.selectInvitee(counter, first1, last1, val1);
				if (first == first1 && last == last1 && val1 != val)
				{
					add = false;
					return1 = false;
				}

				counter++;
			}
		}

		//only add to temp list if add is true and it is not a duplicate
		if (!temp.personOnGuestList(first, last) && add)
			temp.addInvitee(first, last, val);

		//reset add
		add = true;
	}

	//set bpJoined
	bpJoined = temp;

	//determine return value using bool
	if(!return1)
		return false;

	return true;
}

void verifyGuestList(const std::string& fsearch, const std::string& lsearch, const BirthdayParty& bpOne, BirthdayParty& bpResult)
{
	
	BirthdayParty temp = bpResult;
	int limit = temp.whosOnTheGuestList();

	//empty temp
	if (limit)
	{
		for (int k = 0; k < limit; k++)
		{
			string first;
			string last;
			BirthdayType val;

			temp.selectInvitee(k, first, last, val);
			//delete node
			temp.dropFromGuestList(first, last);		
		}
	}

	for (int n = 0; n < bpOne.whosOnTheGuestList(); n++)
	{
		string first = "";
		string last = "";
		BirthdayType val;

		bpOne.selectInvitee(n, first, last, val);

		//if full name matches
		if(first == fsearch && last == lsearch)
			temp.addInvitee(first, last, val);

		//if only searching for first name and it matches
		else if (first == fsearch && lsearch == "*")
			temp.addInvitee(first, last, val);

		//if only searching for last name and it matches
		else if (fsearch == "*" && last == lsearch)
			temp.addInvitee(first, last, val);

		//if both search entries are *
		else if (fsearch == "*" && lsearch == "*")
			temp.addInvitee(first, last, val);
	}

	bpResult = temp;
}
