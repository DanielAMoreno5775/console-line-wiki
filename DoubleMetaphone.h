#pragma once

/*
This class is a tweaked version of the Double Metaphone Phonetic Algorithm by Stephen Lacy.
https://github.com/slacy/double-metaphone
*/

#include <string>
#include <vector>
using namespace std;

class DoubleMetaphone {
	private:
		const int MAX_LENGTH = 32;

		void MakeUpper(string &);
		int IsVowel(string &, int);
		int SlavoGermanic(string &);
		char GetAt(string &, int);
		void SetAt(string &, int, char);
		int StringAt(string &, int, int, ...);
	public:
		DoubleMetaphone(const string &, vector<string> *);
};

