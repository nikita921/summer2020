#ifndef WORDTREE_H
#define WORDTREE_H

#include <iostream>
#include <string>
typedef std::string WordType;

struct WordNode {
	WordType m_data;
	int m_count;
	WordNode* m_left;
	WordNode* m_right;

	//constructor
	WordNode(const WordType& myVal, int count = 1);
};


class WordTree {
private:
	WordNode* root;

	// NAME:		PreOrder
	// INPUT:		three WordNode pointers
	// OUTPUT:		none
	// DESCRIPTION: recursively build tree by processing current node,
	//				left subtree nodes, and then right subtree nodes
	void PreOrder(WordNode* cur, WordNode* rhs, WordNode* prev);

	// NAME:		InOrder
	// INPUT:		ostream, WordNode pointer
	// OUTPUT:		none
	// DESCRIPTION: recursively print nodes out alphabetically
	void InOrder(std::ostream& out, WordNode* cur) const;

	// NAME:		FreeTree
	// INPUT:		WordNode pointer
	// OUTPUT:		none
	// DESCRIPTION:	delete all of the tree nodes
	void FreeTree(WordNode* cur);
	
	// NAME:		count
	// INPUT:		WordNode pointer
	// OUTPUT:		integer
	// DESCRIPTION:	returns the number of nodes in the tree
	int count(WordNode* cur) const;
	
	// NAME:		sum
	// INPUT:		WordNode pointer, integer
	// OUTPUT:		integer
	// DESCRIPTION:	returns the total number of times items have been
	//				added to the tree
	int sum(WordNode* cur, int total) const;
	
public:
	// default constructor
	WordTree() : root(nullptr) { };

	// copy constructor
	WordTree(const WordTree& rhs);

	// assignment operator
	const WordTree& operator=(const WordTree& rhs);

	// destructor
	~WordTree();

	// NAME:		add
	// INPUT:		WordType
	// OUTPUT:		none
	// DESCRIPTION: inserts v into the WordTree
	void add(WordType v);

	// NAME:		distinctWords
	// INPUT:		none
	// OUTPUT:		integer
	// DESCRIPTION:	Returns the number of distinct words / nodes
	//				by calling helper function count
	int distinctWords() const;

	// NAME:		totalWords
	// INPUT:		none
	// OUTPUT:		integer
	// DESCRIPTION: Returns the total number of words inserted, 
	//				including duplicate values by calling helper function sum 
	int totalWords() const;

	// output operator overload
	// prints the tree in alphabetical order (calls InOrder helper function)
	friend std::ostream& operator<<(std::ostream& out, const
		WordTree& rhs);
	
};

#endif