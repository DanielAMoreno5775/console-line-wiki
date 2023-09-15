#include "Section.h"
#include <fstream>
#include <iostream>

using namespace std;

//default constructor
Section::Section() {
	sectionTitle = "";
	numOfLines = 0;
	for (int i = 0; i < 200; i++) {
		arrayOfLines[i] = "";
	}
}

void Section::createNewSection() {
	//initialize variables
	int i = 1;
	string nextLine = "";
	string tempLine = "";

	try {
		//get the title
		cout << "What title would you like to give this section? ";
		getline(cin, sectionTitle);
		//format title to fit style of Ulllllllll...
		sectionTitle[0] = toupper(sectionTitle[0]);
		for (int j = 1; j < sectionTitle.length(); j += 1) {
			sectionTitle[j] = tolower(sectionTitle[j]);
		}
		validateInput(sectionTitle);

		//get the lines until STOP is entered
		while (tempLine != "STOP" && (numOfLines + 1) < 200) {
			//get the user's input
			cout << "Please enter line #" << i << "(Enter STOP to exit section): ";
			getline(cin, nextLine);
			validateInput(nextLine);
			//store the line in a temporary variable which can be safely altered so that user doesn't need to type STOP in all capitals but line won't be all capitals
			tempLine = nextLine;
			for (int j = 0; j < tempLine.length(); j += 1) {
				tempLine[j] = toupper(tempLine[j]);
			}
			//ensure that the line isn't saved if it is STOP
			if (tempLine != "STOP") {
				//save the input as a line
				numOfLines += 1;
				arrayOfLines[i - 1] = nextLine;
				//increment the line counter
				i += 1;
			}
		}
	}
	catch (bool) {
		cout << "You cannot use « or » anywhere in your text." << endl;
		system("pause");
	}
	
}

void Section::printSection() {
	//print title and an empty line
	cout << sectionTitle << endl;
	//print lines in section
	for (int i = 0; i < numOfLines; i += 1) {
		cout << arrayOfLines[i] << endl;
	}
	cout << endl;
}

void Section::printSpecificLine(int lineNum) {
	//initialize variables
	string temp;

	//check whether parameter is valid and print line
	if ((lineNum > 0)) {
		cout << arrayOfLines[lineNum - 1] << endl;
	}
	//if the parameter is invalid, keep asking for new input until a valid one is provided
	else {
		while (lineNum <= 0) {
			cout << "Please enter a line number that is 1 or greater" << endl;
			getline(cin, temp);
			lineNum = stoi(temp);
		}
		cout << arrayOfLines[lineNum - 1] << endl;
	}
}

void Section::editSpecificLine(int lineNum) {
	//initialize variables
	string temp = "";
	string newLine = "";
	
	try {
		//check whether parameter is valid and print line
		if ((lineNum > 0)) {
			cout << arrayOfLines[lineNum - 1] << endl;
			cout << "Please enter the new line: ";
			getline(cin, newLine);
			validateInput(newLine);
			arrayOfLines[lineNum - 1] = newLine;
		}
		//if the parameter is invalid, keep asking for new input until a valid one is provided
		else {
			while (lineNum <= 0) {
				cout << "Please enter a line number that is 1 or greater" << endl;
				getline(cin, temp);
				lineNum = stoi(temp);
			}
			cout << arrayOfLines[lineNum - 1] << endl;
			cout << "Please enter the new line: ";
			getline(cin, newLine);
			validateInput(newLine);
			arrayOfLines[lineNum - 1] = newLine;
		}
	}
	catch (bool) {
		cout << "You cannot use « or » anywhere in your text." << endl;
		system("pause");
	}
}

void Section::editSection() {
	string doAgain = "NO";
	string nextLine = "";
	string tempLine = "";

	try {
		//get new title
		cout << "Current title: " << sectionTitle << endl;
		cout << "What new title would you like to give this section? ";
		getline(cin, sectionTitle);
		sectionTitle[0] = tolower(sectionTitle[0]);
		for (int i = 1; i < sectionTitle.length(); i += 1) {
			sectionTitle[i] = tolower(sectionTitle[i]);
		}
		validateInput(sectionTitle);

		//offer a chance to edit each line
		for (int i = 1; i <= numOfLines; i += 1) {
			cout << "Current line #" << i << ": " << arrayOfLines[i - 1] << endl;
			cout << "Would you like to alter this line (YES/NO)? ";
			getline(cin, doAgain);
			for (int j = 0; j < doAgain.length(); j += 1) {
				doAgain[j] = toupper(doAgain[j]);
			}

			//if the the user entered a YES
			if (doAgain == "YES") {
				editSpecificLine(i);
			}
		}

		//check whether they want to enter a new line
		if (numOfLines < 200) {
			doAgain = "";
			cout << "Would you like to add new lines (YES/NO)? ";
			getline(cin, doAgain);
			for (int j = 0; j < doAgain.length(); j += 1) {
				doAgain[j] = toupper(doAgain[j]);
			}
			if (doAgain == "YES") {
				//ensure that the array does not overflow
				while (tempLine != "STOP" && (numOfLines + 1) < 200) {
					//get the user's input
					cout << "Please enter line #" << numOfLines + 1 << "(Enter STOP to exit section): ";
					getline(cin, nextLine);
					validateInput(nextLine);
					//store the line in a temporary variable which can be safely altered so that user doesn't need to type STOP in all capitals but line won't be all capitals
					tempLine = nextLine;
					for (int j = 0; j < tempLine.length(); j += 1) {
						tempLine[j] = toupper(tempLine[j]);
					}
					//ensure that the line isn't saved if it is STOP
					if (tempLine != "STOP") {
						//save the input as a line
						arrayOfLines[numOfLines] = nextLine;
						numOfLines += 1;
					}
				}
			}
			system("pause");
		}
	}
	catch (bool) {
		cout << "You cannot use « or » anywhere in your text." << endl;
		system("pause");
	}
}

void Section::exportSection(ofstream &file) {
	//uses ASCII 174 and 175 to indicate start of certain thing

	//save article title
	file << "\t»SectionTitle: «" << sectionTitle << endl;

	for (int i = 0; i < numOfLines; i += 1) {
		//save each line
		file << "\tLine «#" << i + 1 << ":«" << arrayOfLines[i] << endl;
	}
}

void Section::importSectionTitle(ifstream &file, string title) {
	sectionTitle = title;
}

void Section::importSectionLine(ifstream &file, string line) {
	//initialize variables
	int lineNum = 0;
	vector<string> componentsOfString;
	split(line, '«', componentsOfString);
	//convert character number to an integer (not ASCII code)
	lineNum = (componentsOfString[1][1]) - '0';
	arrayOfLines[lineNum - 1] = componentsOfString[2];
	numOfLines += 1;
}

void Section::split(const string &s, char delim, vector <string> &tokens) {
	int tokenStart = 0;
	int delimPosition;
	string tok;

	delimPosition = static_cast<int>(s.find(delim, 0));

	while (delimPosition != string::npos) {
		//extract token
		tok = s.substr(tokenStart, (delimPosition - tokenStart));
		//put token into vector
		tokens.push_back(tok);
		//move delimPosition ahead
		delimPosition += 1;
		//move tokenStart to delimPosition
		tokenStart = delimPosition;
		//find next occurrence of delimiter in s
		delimPosition = static_cast<int>(s.find(delim, delimPosition));
		//if not more delimiters, extract last token
		if (delimPosition == string::npos) {
			tok = s.substr(tokenStart, (delimPosition - tokenStart));
			tokens.push_back(tok);
		}
	}
}

int Section::getNumOfLines() const {
	return numOfLines;
}

//This function will not work due to the fact that the console and Visual Studio save and encode the text in different formats
//I don't know how to fix this in a reliable manner that will work if I were to hand this cpp file to someone else
//The code itself should work if it was any other character
void Section::validateInput(string test) {
	//ensure that the user does not input ASCII 174 or 175
	for (int i = 0; i < test.length(); i += 1) {
		if ((test[i] == '«') || (test[i] == '»')) {
			throw false;
		}
	}
}