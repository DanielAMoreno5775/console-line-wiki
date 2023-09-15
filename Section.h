#pragma once
#include <string>
#include <vector>
using namespace std;

class Section {
	private:
		string sectionTitle;
		int numOfLines;
		string arrayOfLines[200];

		void validateInput(string);
		void split(const string &, char, vector <string> &);
	public:
		Section();
		
		void createNewSection();
		void printSection();
		void printSpecificLine(int);
		void exportSection(ofstream &);
		void importSectionTitle(ifstream &, string);
		void importSectionLine(ifstream &, string);
		void editSpecificLine(int);
		void editSection();
		int getNumOfLines() const;
};

