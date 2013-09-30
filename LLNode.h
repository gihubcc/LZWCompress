/*
Defines a node used to make a singly linked list.
This is a pretty bare bones implementation.
*/

#ifndef LLNode_h
#define LLNode_h 

template <class T>
class LLNode
{
public:
	LLNode<T>* next;
	T value;

	LLNode() : next(nullptr), value(NULL) {} // Default constructor should never get used
	LLNode(const T& val) : next(nullptr), value(val) {}
};

#endif