/*
Optimized singly linked list. It only contains a find and add method since these are
the only methods that dictionary will use.
*/
#ifndef LList_h
#define LList_h

#include "LLNode.h"

template <class T>
class LList
{
private:
	LLNode<T>* head;

public:
	LList() : head(nullptr) {} // Make an empty list by default+
	~LList() 
	{
		LLNode<T>* it = head;
		LLNode<T>* it2;
		while (it != nullptr)
		{
			it2 = it->next;
			delete it;
			it = it2;
		}
	}
		
	void add(const T& val)
	{
		// If we have an empty list, add item to head. This is the common case.
		if (head == nullptr) 
			head = new LLNode<T>(val);
		else {
			// Else find the end of the list and add a new node with val.
			LLNode<T>* it = head;
			while (it->next != nullptr)
				it = it->next;

			it->next = new LLNode<T>(val);
		}
	}

	bool find(const T& key) const
	{
		LLNode<T>* it = head;
		while (it != nullptr)
		{
			if (it->value == key)
				return true;
			else
				it = it->next;
		}

		return false;
	}
};

#endif