#ifndef HYBRIDTABLE_H_
#define HYBRIDTABLE_H_

#include<iostream>
#include<cstring>
#include <string>
#include <cmath>
using std::string;
using namespace std;
class Node {

	int index_;  // index of this node
	int val_;    // value corresponding to this index
	Node* next_; // pointer to next node in the list

	// even constructors are private!
	// so only HybridTable can access them

	// Constructor initialising the node with an index and its
	// corresponding value, with the next_ pointer set to nullptr
	Node(int index, int val);

	// Constructor initialising the node with an index, its
	// corresponding value, and the next node pointer
	Node(int index, int val, Node* next);

	// Destructor
	~Node();

friend class HybridTable; // allow HybridTable to access private members
};

class HybridTable {

public:
	// Constructor. Constructs an empty HybridTable with array part of
	// size INITIAL_ARRAY_SIZE, and empty list part.
	// All entries in the array part initialised to 0.
	HybridTable();

	// Parameterised constructor. Constructs a HybridTable with array
	// part of size n, and an empty list part.
	// arr is assumed to have size n, and the entries in the array part
	// are initialised with arr[0] to arr[n-1].
	HybridTable(const int* arr, int n);

	// Destructor. It should release all memory used by this HybridTable.
	~HybridTable();

	// Copy constructor.
	HybridTable(const HybridTable& other);

	// Copy assignment operator.
	HybridTable& operator=(const HybridTable& other);

	// Returns the value corresponding to index i.
	// If index i is not present in the HybridTable, return 0.
	int get(int i) const;

	// Sets the value corresponding to index i to val.
	// Resizing of the array part, if required, should also happen here.
	void set(int i, int val);

	// Returns a string representation of the HybridTable, as described
	// in the assignment webpage.
	// Note that it does not actually print anything to the screen.
	// To pass the test cases you must make sure that even the
	// white spaces are correct.
	string toString() const;

	// Returns the number of entries of the array part. In other words,
	// the array part indices are [0..getArraySize()-1].
	int getArraySize() const;

	// Returns the total number of elements in the array part and
	// the list part.
	int getTotalSize() const;

	// We didn't explain what static and constexpr are, but you can just
	// use them in HybridTable.cpp just like normal constants
	// DO NOT CHANGE, MOVE OR REMOVE IT
	static constexpr int INITIAL_ARRAY_SIZE = 4; // default array part size

private:

	int* array_; // pointer to array part
	Node* list_; // pointer to head of list part
	int npositiveElement = 0;

	// add other member variables if required
	// Maintain Reference Counter for array_ size
	int ncntr_array = INITIAL_ARRAY_SIZE;
	// add other member functions if required
	Node* getInsertPosition(int nindex,bool findIndex) const;
	Node* deepClone(Node* phead_node) const;
	void setList(int i ,int val);
	void setArray(int i, int val);
	int getNearestPower(int i);
	Node* getPostiveStartNode(int nPower,int i);
	bool checkifArrayCanResize(int i, int val);
	void deleteList(Node * StartNode);
	void deleteNode(int index);
	void setResizedArray(int i, int val, Node* Startpos, int nNewSize);
	
	
//	void setResizedArray(int i, int val Node* pPos_start i


};

#endif /* HYBRIDTABLE_H_ */
