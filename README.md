m.

Motivation
The primitive C/C++ array is a simple and efficient data structure, but it has many problems, such as requiring a fixed size to be known at creation time. If we do not know this and hence are forced to create an unnecessarily large array "just in case", it wastes a lot of space. Similar problems happen if there are some range of indices that are only very sparingly used (but used nonetheless). For example, an application may need an array where most indices used are between 0 and 10, but occasionally has some "outliers" with large indices such as 99, 147, 2046 and 9999. Then creating an array of size 10000 to cater for them wastes a lot of memory.

There are various ways to create an "adaptive" kind of array that has the speed and convenience of direct access of an array, for the range of indices that are "densely populated", while allowing those "outliers" to be stored as well without wasting too much space (possibly at the expense of slower access). All these should happen transparently without the end user's involvement. In this assignment you will attempt a naive way of doing this, where the outliers are stored in a linked list. (In reality no one will use a linked list for this purpose and more sophisticated data structures are used, but, you know, linked list is pretty much the only other thing you know right now.) We call the data structure "HybridTable" here. (I might tell you why after the assignment finishes...)

Overview of the data structure
From an object-oriented point of view, I should not prescribe how you should implement the internal workings of the data structure as long as your implementation complies with the specified external interface; nevertheless, this is an assignment, so I am going to prescribe it.

To the end user, the "HybridTable" data structure allows the storage and retrieval of integer values given an integer index, just like a normal array. (For the purpose of this assignment, we only consider the integer data type as values stored.) Internally, the data structure consists of two parts: an array part and a list part. The array part stores a consecutive range of indexed values, starting from 0. This range is determined dynamically in a way to be specified later. All "outliers", namely those with indices falling outside of this range, goes into the list part. We also support negative indices (because, why not?) and they are always in the list part. The list part is a linked list of (index, value) pairs, in increasing order of index.

Here is a picture showing (conceptually) how the (index, value) pairs (1,-1), (2, 123), (-1, 2046), (7, 97), (99, -2) are stored in a HybridTable where the array part has size 4 (i.e., it handles the index range 0 to 3). In primitive array syntax this is like a[1] = -1, a[2] = 123, a[7] = 97, etc.


To faciliate the storage of these (index, value) pairs in a linked list, a Node class is defined as follows:

class Node {
  int index_;  // index of this node
  int value_;  // value corresponding to this index
  Node* next_; // pointer to next Node in the list
};
The data structure is encapsulated in the HybridTable class, defined as follows:

class HybridTable {
  int* array_; // pointer to array part
  Node* list_; // pointer to head of list part
  ...
};
The array_ pointer points to an (dynamically allocated) array of integers for the array part. (As you will see below, this array will be resized from time to time, so dynamic allocation is necessary instead of using a fixed-size array.) The list_ pointer points to the head of a linked list of Nodes which is the list part.
Note that the array part is an array of integers (the indices are not stored explicitly) whereas the list part is a list of nodes (pairs of indices and values).

The HybridTable class is made a "friend" of the Node class. We will explain what this means later, but all you need to know now is that HybridTable can access Node as if everything there is public.

Supported operations
You should implement the Node and HybridTable classes so that HybridTable supports the following functions:
HybridTable(): Default constructor. It constucts a HybridTable with an array part of size INITIAL_ARRAY_SIZE (a constant defined in HybridTable.h); in other words, the array handles the index range from 0 to INITIAL_ARRAY_SIZE - 1. Each entry in the array part should be initialised to 0. It also initialises an empty list part (by setting list_ to a null pointer).

HybridTable(const int* p, int n): Parameterised constructor, where p is supposed to point to the beginning of an integer array and n is supposed to be the number of elements in that array. This constructor constructs a HybridTable with an array part of size n, populated with the values from the array p (i.e., index i of the array part should be initialised with the value p[i]). It also initialises an empty list part.
Destructor, that releases all memory used.

Copy constructor and copy assignment functions, that perform a deep copy of another existing HybridTable. They should really be exact copies, i.e., not just the set of values stored are identical but also which values go into the array part or the list part should be the same as well. The copy assignment operator should release all memory of the existing HybridTable that is being overwritten.

int get(int index): returns the value stored in the HybridTable corresponding to the given index. If there is no value stored corresponding to this index (i.e., it is outside the range of the array part and also does not appear in the list part), return 0.
void set(int index, int val): stores the value val in the HybridTable at the given index. If the index is within the range of the array part, or is already in the list part, the new value is simply written to the corresponding location, replacing the existing value. Otherwise, a new node is created with the given index and value, and inserted to the list part. The linked list should always be in increasing order of the indices.

In the case where a new node is to be added, it should check whether resizing should happen to the array part; this is described separately below.

(Don't you think using these get() and set() is very cumbersome? If only we could simply use the array indexing syntax, i.e., A[100] = 1 will set the value with index 100 in a HybridTable called A, irrespective of whether the index 100 is in the array part or list part? Don't worry, you will do that later in Assignment 2A. Or maybe a lab exercise. Possibly...)

string tostring(): returns a string that would give a "visual" representation of the contents of the HybridTable. First, there is a row of each entry in the array part, in the form "index : value". Then, if the list part is not empty, there should first be a line with three dashes "---"; then the (index, value) pairs in the listed part are listed, again in the "index : value" format, with " --> " between them. Both the entries in the array part and the list part should (separately) be in increasing index order. Note the single space character before and after the ":" symbol and before and after the "-->". For example, the HybridTable in the picture above is printed like this:
0 : 0
1 : -1
2 : 123
3 : 0
---
-1 : 2046 --> 7 : 97 --> 99 : -2
The whole printout is stored in one string (which will contain multiple lines, separated by the '\n' character) and returned to the caller; the function itself does not print anything to the screen. You must have the exact whitespace as indicated, and be careful not to include any extra invisible whitespaces (in particular there should be no '\n' after the final line), otherwise you will not pass the test cases.

int getArraySize(): returns the size of the array part, i.e. how many entries can be stored there. Note that this is not about how many entries in the array part have been "used". Even if they have never been written into and still contain the original initialised value of 0, they still count.

int getTotalSize(): returns the total size (number of entries) of the array part and the list part. Again, all entries in the array part count.
Resizing the array part
(This part is meant to be difficult. You may want to leave this to the end. Only test cases t to y involving resizing; you can still pass all other test cases if you skip this whole part.)

As more and more indices outside the array range are being accessed, the list part gets longer and longer and the data structure becomes increasingly inefficient because it has to traverse a long list. Thus it would be beneficial to "grow" the array part every now and then, allowing some entries of the list part to be stored there instead. At the same time the array part should still be reasonably "dense" (most indices are actually used) to avoid wasting memory. In our HybridTable data structure, the criteria is that the new size of the array part, which must be a power of 2, is such that 75% or more of its entries are "used". During the set() operation, if it accesses an index not in the current array part or list part (i.e. it is about to create a new node), it should first check whether the array part should be resized, and what the new size should be.
As a simple example, suppose the array part originally has size 4 (index range 0 to 3). Also there is an index 4 in the list part. All these five indices count as "used". If now index 5 is written into, then instead of leaving it in the list part, we should increase the size of the array part to 8 (index 0 to 7) as 75% of it (6 entries out of 8) are "used". The new value for index 5 is therefore written to the array part. The value for index 4 is also moved to the list part.

In contrast, if index 8 is being written to instead of index 5, then an array size 8 (index 0 to 7) is not 75% used (only 5 out of 8 used), and array size 16 is not 75% used either (only 6 out of 16 used), and the same is true for all larger array sizes. So in this case the array part is not resized and the entry with index 8 is added to the list part. If subsequently more values are written, for example at indices 9,10,11,12,13,14, in this order, then by the time of index 14 the array part should be resized to 16 (index 0 to 15) because 75% of it (12 out of 16) are used.

To be precise, the new size s of the array part should be the largest power of 2 such that 75% or more of the indices in the range [0..s-1] were either in the old array range or in the list part, including the index currently being written into. Note that even if some of the array part entries were never written into, they still count as "used". The index range always start at 0, and negative indices never get into the array part. The array part never shrinks, and its size is always a power of 2 after resize has happened at least once.

The following table gives more examples:

Old array part index range	List part indices (including the one being set)	New array part index range	Explanation / notes
[0..3]	4	[0..3]	[0..7] would be only 5/8 used
[0..3]	5,7	[0..7]	[0..7] is 6/8 used
[0..3]	6,8,9,10,11,12,13	[0..3]	[0..7] would be only 5/8 used, and [0..15] would be only 11/16 used
[0..3]	4,5,8,9,10,11,12	[0..7]	[0..7] is 6/8 used, and [0..15] would be only 11/16 used
[0..3]	7,8,9,10,11,12,13,14	[0..15]	[0..7] would be only 5/8 used, and [0..15] is 12/16 used
[0..7]	8,9,10,11,16,17,18,19,20,21,22,23,24,25,26,27	[0..31]	both [0..15] or [0..31] would be 75% used so the larger range is chosen
[0..3]	-4,-3,-2,-1	[0..3]	negative indices would never be part of the array part
[0..4]	8	[0..4]	An original non-power-of-2 size, such as 5 in this case, is only possible following parameterized constructor. Since [0..7] would only be 5/8 used, the size stays at 5.
[0..9]	10,12,14,16,18,20,22,24	[0..15]	Again this follows from parameterised constructor. [0..15] is 13/16 used but [0..31] would only be 18/32 used.
[0..11]	-1	[0..15]	Again this follows from a parameterised constructor. Since the original range [0..11] would make [0..15] 75% used already, any insertion would trigger a resize to [0..15], even from a negative index as in this case. But note that the resize is triggered only when an insertion to the list is about to be made.
Once the new size is identified and the array part resized, those values in the list part where the corresponding index now fall into the new array index range should be moved to the array part. All other new entries in the array part are initialised to 0. For example, if set(5,50) is applied to the HybridTable in the picture earlier, the result would be (as produced by toString()):

0 : 0
1 : -1
2 : 123
3 : 0
4 : 0
5 : 50
6 : 0
7 : 97
---
-1 : 2046 --> 99 : -2
As explained earlier, index 0 and 3 count as "used" even though their values are 0; and after this resize, indices 4 and 6 also count as used for the next resize.
Any old array or deleted list nodes as a result of this procedure should have their memory released properly.

What you can and cannot do
You must not change the existing public interface of the classes, and you must not change the existing private data members of the classes. You are allowed to add public or private member functions, or private data members, should you want to (and you may need to).

Do NOT move or remove existing code given to you, except those parts labelled as dummy code. NEVER change anything in the test suite (see below). If you somehow believe you "need" to change something (in order for the program to compile, for example), you almost certainly did something wrong and it would not compile when I test it. Past examples include removing header guards, moving constants defined in .h out to .cpp, removing function definitions in .h that you decide to not attempt, etc.

You must not use containers from the Standard Template Library (STL), nor smart pointers. (Both of these will be covered later in the module.) You also must not use any external libraries.

Files Provided
HybridTable.h
HybridTable.cpp
These are the only two files you need to modify/submit. All code written by you should be in there.
main.cpp
This is just an example that illustrates how the functions can be called.
HybridTableTester.h
HybridTableTester.cpp
HybridTableTesterMain.cpp
They are used for the execution testing part (see the next section).
makefile
This is a makefile that will compile the main executable and the test suite executable.
Marking Criteria and Test Suite
See this separate page for the marking criteria.

To use the test suite (which is used in the execution testing part of the marking), simply type "make" in a linux terminal (with all the above files in the same folder). It will (assuming you did not break HybridTable.h or HybridTable.cpp) produce a HybridTableTesterMain executable file. Run the program by typing (for example)

./HybridTableTesterMain a
or
./HybridTableTesterMain a b c d
which runs a single test case or multiple test cases respectively. We will also demonstrate its use in class.
The given HybridTable.h and HybridTable.cpp files (without your contributions) are already compilable with the test suite. It might even pass a few test cases. So, whatever you do, please don't break them...

The test suite may not cover all corner cases, so passing all test cases does not guarantee your program is 100% correct. Also, unfortunately we cannot isolate the testing of the various functions. For example to pass those test cases meant for toString() you need to also implement set() at least partially correctly.

Submission Instructions
Submit your completed work on Blackboard ("Assessment and Feedback" on the left, then "Assignment 1").

Submit only the files HybridTable.h and HybridTable.cpp. Just upload them as two separate files with those exact names. DO NOT change their names including upper/lowercase, DO NOT change the extension to .txt or .cpp.txt or some such, DO NOT put them in a Word or pdf file (yes, someone actually did that), DO NOT put them in a zip archive, and DO NOT upload the entire project folder of whatever IDE you are using. Any such problems will incur a penalty.

While you may want to change the main.cpp file for your own testing, it is not part of the submission. The test suites and the makefiles also should not be submitted. If you submit them, they will be ignored and I will use my own version for testing.

This is an individual assignment, and collaboration is not permitted. Plagiarism will be treated strictly according to standard university and departmental procedures. Your submissions will be sent to a plagiarism detection service (MOSS).

In line with university policy, marking will be done anonymously. Only the Blackboard-supplied userid / student number will be visible in marking.

For the above two reasons, do not include your name, userid, student number, or any other personally identifiable information in your programs.
