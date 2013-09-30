#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <hash_map>
#include "dictionary.h"
using namespace std;

void displayHelp(void);
void decodeFile(const string& inFile, const string& outFile);
void encodeFile(const string& inFile, string& outFile);
int findStr(const string& value, hash_map<string, int>& dictionary);
void writeKey(int key, size_t dictionary_size, size_t& starting_bit, vector<bool>& output);
void writeToFile(string& outputFile, const vector<bool>& output, const size_t ending_bit);
void showData(vector<bool>& output, size_t end_idx);

int main(int nArgs, const char** cmdLine)
{
	
	string args[4];
	for (int i = 0; i < 4; i++)
		args[i] = cmdLine[i];

	if (args[1] == "-e" || args[1] == "/e")
	{
		if (args[3] != "")
			encodeFile(args[2], args[3]);
		else 
			cout << "Error: you must specify an output file name!\n" << endl;
	}
	else if (args[1] == "-d" || args[1] == "/d")
		decodeFile(args[2], args[3]);
	else 
		displayHelp();

	return 0;
}

void displayHelp(void) 
{
	cout << "This program sucks!" << endl;
}

void decodeFile(const string& inFile, const string& outFile)
{

}

void encodeFile(const string& inputFile, string& outputFile)
{
	// Populate data with file contents
	ifstream file(inputFile, ios::in | ios::binary);

	if (file.is_open())
	{
		vector<char> data;
		char c;
		while(file.good())
		{
			file.read(&c, 1);
			data.push_back(c);
		}
		file.close();

		Dictionary dictionary;
		int dict_idx = 0;
		for (unsigned char c = 0; dict_idx < 256; c++, dict_idx++) 
		{
			string key;
			key = c;
			dictionary[key] = c;
		}
		
		// Create an empty output bool vector with size = 16 * data.size
		// eg 8 bit data encoded at up to 16 bits per code
		vector<bool> output(data.size()*16, false);
	
		// Populate output using lzw encoding scheme!
		string sub_str = "";
		string str_next = "";
		int old_key, new_key;
		size_t starting_bit = 0;

		// Black Magic Below!!!
		// Tommy wrote this shit on 10/27 @ 10:55 pm
		// Modified slightly by Stew on 11/3 @ 1:45 pm
		for (vector<char>::iterator i = data.begin(); i+1 != data.end();)
		{
			sub_str = *i;
			str_next = sub_str + *(++i);
			old_key = findStr(sub_str, dictionary);

			while((new_key = findStr(str_next, dictionary)) >= 0 && i+1 != data.end())
			{
				old_key = new_key;
				sub_str = str_next;
				str_next = sub_str + *(++i);
			}

			// Don't swap these two
			writeKey(old_key, dictionary.size(), starting_bit, output);
			dictionary[str_next] = dictionary.size();
		}

		cout << "Dictionary size: " << dictionary.size();

		writeToFile(outputFile, output, starting_bit); // starting_bit is actually being used as ending_bit
	}
	else 
	{
		cout << "Error, could not open file: " << inputFile << endl;
		return;
	}
}

int findStr(const string& value, hash_map<string, int>& dictionary)
{
	auto it = dictionary.find(value);

	if (it == dictionary.end())
		return -1;
	else
		return it->second;
}

void writeKey(int key, size_t dictionary_size, size_t& starting_bit, vector<bool>& output)
{
	unsigned int u_key = (unsigned int) key;
	// Hacky log base 2 TROLOLO
	// input 31 gives 5, 32 gives 6
	unsigned int key_bit_length = 1;
	while (dictionary_size >>= 1)
		key_bit_length++;

	// Write key to output!
	for (unsigned int i = 0; i < key_bit_length; i++)
	{
		if ((u_key >> (key_bit_length - i - 1)) & 1)
		{
			output[starting_bit + i].flip();
		}
	}
	starting_bit += key_bit_length;
}

void writeToFile(string& outputFile, const vector<bool>& output, const size_t ending_bit)
{
	// Add file extension
	outputFile += ".lzw";

	// Open file in binary mode
	ofstream file(outputFile, ios::out | ios::binary);
	if (file.is_open())
	{
		// Convert chunks of the output vector into chars, and then write those to file
		for (size_t i = 0; i < ending_bit; i += 8)
		{
			unsigned char outByte = 0;
			for (size_t j = 0; j < 8; j++) 
			{
				if (i + j < ending_bit) // output[i+j] could "theoretically" fail
				{
					unsigned char temp = output[i+j];
					temp <<= 7-j; // 8-j-1
					outByte |= temp;
				}
			}
			file.write((char*)&outByte,1);
		}
		// Be a good boy and close the file
		file.close();
	}
	else 
		cout << "Could not write compressed data to file: " << outputFile << endl;

	return;
}

void showData(vector<bool>& output, size_t end_idx)
{
	for (vector<bool>::iterator i = output.begin(); i < end_idx+output.begin(); i++)
	{
		if (*i)
			cout << 1;
		else
			cout << 0;
	}
	cout << endl;
}