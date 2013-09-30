#ifndef Hash_h
#define Hash_h

#include <string> 

// This is a simple, optimized multiplicative hash function to
// hash a string into an unsigned integer between 0 and 65535

unsigned int hash(const std::string& s) 
{
	unsigned int hash_val = 5381;
	size_t len = s.length();

	for (size_t i = 0; i < len; ++i) 
		hash_val = hash_val * 33 + s[i];

	return hash_val & 65535; // same as hash_val % 65536 but faster on many machines
}

#endif