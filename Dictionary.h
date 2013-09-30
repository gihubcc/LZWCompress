/*
A simple dictionary that can add and find strings. It cannot delete entries
*/
#ifndef Dictionary_h
#define Dictionary_h

#include <string>
#include "LList.h"
#include "Hash.h"
using std::string;

class Dictionary
{
private:
	LList<string>* table; 
	size_t size;

public:
	Dictionary() 
	{ 
		table = new LList<string>[65536]; // 65536 is a magic number of the form 2^n
		size = 0; 
	} 
	
	~Dictionary() 
	{
		delete[] table;
	}

	int find(const string& key) const
	{
		unsigned int idx = hash(key);
		if (table[idx].find(key))
			return idx;
		else 
			return -1;
	}

	void add(const string& val)
	{
		table[hash(val)].add(val);
		size++;
	}

	size_t getSize()
	{
		return size;
	}
};

#endif