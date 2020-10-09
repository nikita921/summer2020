#include <iostream>
#include <string>
#include "WordTree.h"
using namespace std;

//WordNode constructor
WordNode::WordNode(const WordType& myVal, int count)
{
	//initialize data members
	m_count = count;
	m_data = myVal;
	m_left = m_right = nullptr;
}

// copy constructor
WordTree::WordTree(const WordTree& rhs)
{
	//initialize root
	root = nullptr;

	//if empty, return
	if (rhs.root == nullptr)
		return;
		 
	else
		PreOrder(root, rhs.root, nullptr);
}

void WordTree::PreOrder(WordNode* cur, WordNode* rhs, WordNode* prev)
{
	if (rhs == nullptr)            // if empty return
		return;
	
	//allocate memory and initialize
	cur = new WordNode(rhs->m_data, rhs->m_count);

	//set root
	if (root == nullptr)
		root = cur;

	//if there is a node above and the current node is less than it, set left
	if (prev != nullptr && cur->m_data < prev->m_data)
		prev->m_left = cur;

	//if there is a node above and the current node is more than it, set right
	else if (prev != nullptr && cur->m_data > prev->m_data)
		prev->m_right = cur;
	
	PreOrder(cur->m_left, rhs->m_left, cur);			// process nodes in left subtree
	PreOrder(cur->m_right, rhs->m_right, cur);			// process nodes in right subtree
}


// assignment operator
const WordTree& WordTree::operator=(const WordTree& rhs)
{
	//if already equal, return
	if (this == &rhs)
		return *this;

	//make copy of rhs and switch tree roots
	WordTree s = rhs;
	WordNode* temp = root;
	root = s.root;
	s.root = temp;

	return *this;
}


void WordTree::InOrder(ostream& out, WordNode* cur) const
{
	if (cur == NULL)            // if empty return
		return;
	InOrder(out, cur->m_left);        // process nodes in left sub tree
	out << cur->m_data << " " << cur->m_count << endl;        // process current node
	InOrder(out, cur->m_right);        // process nodes in right subtree
}


// prints in alphabetical order
ostream& operator<<(ostream& out, const WordTree& rhs)
{
	rhs.InOrder(out, rhs.root);
	return out;
}

// Inserts v into the WordTree
void WordTree::add(WordType v)
{
	//if tree is empty, set root to new node
	if (root == nullptr)
	{
		root = new WordNode(v);
		return;
	}

	//temp pointer for traversal
	WordNode* cur = root;
	for (;;)
	{
		//if a node already has the value, increment count
		if (v == cur->m_data)
		{
			cur->m_count++;
			return;
		}

		//if the value is less than the node, go left
		if (v < cur->m_data)
		{
			if (cur->m_left != nullptr)
				cur = cur->m_left;
			else
			{
				cur->m_left = new WordNode(v);
				return;
			}
		}

		//if the value is more than the node, go right
		else if (v > cur->m_data)
		{
			if (cur->m_right != nullptr)
				cur = cur->m_right;
			else
			{
				cur->m_right = new WordNode(v);
				return;
			}
		}
	}

}

int WordTree::count(WordNode* cur) const
{
	if (cur == nullptr)
		return 0;

	//add 1 for every node, and remember to add root node
	else 
		return 1 + count(cur->m_left) + count(cur->m_right);	
}

// Returns the number of distinct words / nodes
int WordTree::distinctWords() const
{
	int counter = 0;

	//if root exists, call helper function
	if (root)
		counter = count(root);

	return counter;
}

int WordTree::sum(WordNode* cur, int total) const
{
	if (cur == nullptr)
		return 0;

	//initialize total to current node's count
	total = cur->m_count;

	//add left subtree and right subtree
	total += sum(cur->m_left, total);
	total += sum(cur->m_right, total);

	return total;
}

// Returns the total number of words inserted, including
// duplicate values
int WordTree::totalWords() const
{
	int counter = 0;

	//if root exists, call helper function
	if (root)
		counter = sum(root,0);

	return counter;
}

void WordTree::FreeTree(WordNode* cur)
{
	if (cur == nullptr)
		return;

	//left subtree
	FreeTree(cur->m_left);

	//right subtree
	FreeTree(cur->m_right);

	//delete node
	delete cur;
}

// Destroys all the dynamically allocated memory in the
// tree
WordTree::~WordTree()
{
	//call helper function
	FreeTree(root);
}

