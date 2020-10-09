#ifndef BIRTHDAY_H
#define BIRTHDAY_H
#include <string>

typedef std::string BirthdayType;
class BirthdayParty
{
  public:
	// default constructor : create an empty BirthdayParty list
	BirthdayParty();

	// copy constructor
	BirthdayParty(const BirthdayParty& rhs);

	// destructor
	~BirthdayParty();

	// assignment operator
	BirthdayParty& operator=(const BirthdayParty& rhs);

	// NAME: noInvitees
	// INPUT: none
	// OUTPUT: true if empty, false if not
	// DESCRIPTION: Return true if the BirthdayParty list
	// is empty, otherwise false.
	bool noInvitees() const;

	// NAME: whosOnTheGuestList
	// INPUT: none
	// OUTPUT: integer for number of elements
	// DESCRIPTION: Return the number of players in
	// the BirthdayParty list.
	int whosOnTheGuestList() const;

	// NAME: addInvitee
	// INPUT: const reference string, const reference string, const reference BirthdayType
	// OUTPUT: true if added, false if it is a duplicate
	// DESCRIPTION: If the full name (both the first and last name) is not equal
	// to any full name currently in the list then add it and return
	// true. Elements should be added according to their last name.
	// Elements with the same last name should be added according to
	// their first names. Otherwise, make no change to the list and
	// return false (indicating that the name is already in the list).
	bool addInvitee(const std::string& firstName, const std::string&
		lastName, const BirthdayType& value);
	
	// NAME: modifyInvitee
	// INPUT: const reference string, const reference string, const reference BirthdayType
	// OUTPUT: true if modified, false if not
	// DESCRIPTION: If the full name is equal to a full name currently in the
	// list, then make that full name no longer map to the value it
	// currently maps to, but instead map to the value of the third
	// parameter; return true in this case. Otherwise, make no
	// change to the list and return false.
	bool modifyInvitee(const std::string& firstName, const
	std::string& lastName, const BirthdayType& value);
	
	// NAME: addOrModify
	// INPUT: const reference string, const reference string, const reference BirthdayType
	// OUTPUT: always true
	// DESCRIPTION: If full name is equal to a name currently in the list, then
	// make that full name no longer map to the value it currently
	// maps to, but instead map to the value of the third parameter;
	// return true in this case. If the full name is not equal to
	// any full name currently in the list then add it and return
	// true. In fact, this function always returns true.
	bool addOrModify(const std::string & firstName, const std::string &
		lastName, const BirthdayType & value);
	
	// NAME: dropFromGuestList
	// INPUT: const reference string, const reference string
	// OUTPUT: true if deleted, false if not
	// DESCRIPTION: If the full name is equal to a full name currently in the
	// list, remove the full name and value from the list and return
	// true. Otherwise, make no change to the list and return false.
	bool dropFromGuestList(const std::string& firstName, const
	std::string& lastName);
	
	// NAME: personOnGuestList
	// INPUT: const reference string, const reference string
	// OUTPUT: true if full name exists, false if not
	// DESCRIPTION: Return true if the full name is equal to a full name
	// currently in the list, otherwise false.
	bool personOnGuestList(const std::string& firstName, const
	std::string& lastName) const;

	// NAME: checkGuestList
	// INPUT: const reference string, const reference string, const reference BirthdayType
	// OUTPUT: true if value parameter was changed, false otherwise
	// DESCRIPTION: If the full name is equal to a full name currently in the
	// list, set value to the value in the list that that full name
	// maps to, and return true. Otherwise, make no change to the
	// value parameter of this function and return false.
	bool checkGuestList(const std::string& firstName, const
	std::string& lastName, BirthdayType& value) const;
	
	// NAME: selectInvitee
	// INPUT: integer, const reference string, const reference string, const reference BirthdayType
	// OUTPUT: true if parameters hold information, false otherwise
	// DESCRIPTION: If 0 <= i < size(), copy into firstName, lastName and value
	// parameters the corresponding information of the element at
	// position i in the list and return true. Otherwise, leave the
	// parameters unchanged and return false. (See below for details
	// about this function.)
	bool selectInvitee(int i, std::string& firstName, std::string&
	lastName, BirthdayType& value) const;
	
	// NAME: changeGuestList
	// INPUT: reference BirthdayParty
	// OUTPUT: none
	// DESCRIPTION: Exchange the contents of this list with the other one.
	void changeGuestList(BirthdayParty& other);
	

  private:
	struct Node
	{
		std::string firstName;
		std::string lastName;
		BirthdayType value;
		Node* next;
		Node* prev;
	};

	Node* head;
	Node* tail;
};

// NAME: combineGuestList
// INPUT: const reference BirthdayParty, const reference BirthdayParty, reference BirthdayParty
// OUTPUT: false if there was a duplicate name w/ different BirthdayType, true otherwise
// DESCRIPTION: bpJoined should contain only the combination of lists bpOne and bpTwo
// and should not contain any duplicates. If a full name appears in both bpOne and
// bpTwo while having different BirthdayType values, then function should return false and the 
// element should not be added to bpJoined, but bpJoined should still contain the combination
// of the two lists
bool combineGuestLists(const BirthdayParty& bpOne,
	const BirthdayParty& bpTwo,
	BirthdayParty& bpJoined);

// NAME: verifyGuestList
// INPUT: const reference string, const reference string, const reference BirthdayParty, reference BirthdayParty
// OUTPUT: none
// DESCRIPTION: bpResult should contain all elements in bpOne that match fsearch (first name)
// and lsearch (last name). If "*" is inputted as the fsearch element, then all first names
// should match and the code should continue to search for matching last names. This applies if lsearch
// is "*", where all last names should match and the code should search for a matching first name,
// and also if both entries are "*", where every element matches.
void verifyGuestList(const std::string& fsearch,
	const std::string& lsearch,
	const BirthdayParty& bpOne,
	BirthdayParty& bpResult);

#endif
