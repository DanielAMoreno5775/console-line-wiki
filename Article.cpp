#include "Article.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cctype>
using namespace std;

Article::Article() {
	articleTitle = "";
	numOfSections = 0;
	for (int i = 0; i < 200; i++) {
		arrayOfSectionsPtr[i] = nullptr;
	}
}

Article::~Article() {
	titleMetaphoneCodes.clear();
	for (int i = 0; i < numOfSections; i++) {
		delete arrayOfSectionsPtr[i];
		arrayOfSectionsPtr[i] = nullptr;
	}
}

void Article::createArticle() {
	Section * tempPtr = nullptr;
	string doAgain = "NO";


	try {
		//get the title
		system("cls");
		cout << "What title would you like to give this article? ";
		getline(cin, articleTitle);
		validateInput(articleTitle);
		//format title to fit style of Ulllllllll...
		articleTitle[0] = toupper(articleTitle[0]);
		for (int i = 1; i < articleTitle.length(); i += 1) {
			articleTitle[i] = tolower(articleTitle[i]);
		}
		DoubleMetaphone(articleTitle, &titleMetaphoneCodes);

		//create the first section
		tempPtr = new Section();
		tempPtr->createNewSection();
		arrayOfSectionsPtr[numOfSections] = tempPtr;
		numOfSections += 1;
		cout << "Do you wish to create another section (YES/NO)? ";
		getline(cin, doAgain);
		for (int i = 0; i < doAgain.length(); i += 1) {
			doAgain[i] = toupper(doAgain[i]);
		}

		//continue creating sections
		while ((numOfSections < 200) && (doAgain[0] == 'Y')) {
			//reset tempPtr's address
			tempPtr = nullptr;

			//get next section's contents
			tempPtr = new Section();
			tempPtr->createNewSection();
			arrayOfSectionsPtr[numOfSections] = tempPtr;
			numOfSections += 1;
			cout << "Do you wish to create another section (YES/NO)? ";
			getline(cin, doAgain);
			for (int i = 0; i < doAgain.length(); i += 1) {
				doAgain[i] = toupper(doAgain[i]);
			}
		}
	}
	catch (bool) {
		cout << "You cannot use « or » anywhere in your text." << endl;
		system("pause");
	}
	catch (bad_alloc) {
		cout << "The program has run out of virtual memory." << endl;
		system("pause");
	}
	
}

void Article::printArticle() {
	//print title and 2 empty lines
	cout << articleTitle << endl << endl << endl;
	//print lines in section
	for (int i = 0; i < numOfSections; i += 1) {
		arrayOfSectionsPtr[i]->printSection();
		//must pause after each section as the console window will remove text if too much is currently being displayed
		system("pause");
	}
}

void Article::printSpecificLineWithinArticle() {
	string temp = "";
	int sectionNum = 0;
	int lineNum = 0;

	//get the section number
	cout << "Please enter the number of the section that you wish to view starting at 1: ";
	getline(cin, temp);
	sectionNum = stoi(temp);
	while (sectionNum > numOfSections) {
		cout << "That is an invalid section number." << endl;
		cout << "Please enter the number of the section that you wish to view starting at 1: ";
		getline(cin, temp);
		sectionNum = stoi(temp);
	}

	//send instruction to print line
	cout << "Please enter the number of the line that you wish to print starting at 1: ";
	getline(cin, temp);
	lineNum = stoi(temp);
	arrayOfSectionsPtr[sectionNum - 1]->printSpecificLine(lineNum);
}

void Article::editLineWithinArticle() {
	string temp = "";
	int sectionNum = 0;
	int lineNum = 0;
	int numOfLines = 0;

	//get the section number
	cout << "Please enter the number of the section that you wish to alter starting at 1 and ending with " << numOfSections <<": ";
	getline(cin, temp);
	sectionNum = stoi(temp);
	while (sectionNum > numOfSections || sectionNum < 1) {
		cout << "That is an invalid section number." << endl;
		cout << "Please enter the number of the section that you wish to alter starting at 1 and ending with " << numOfSections << ": ";
		getline(cin, temp);
		sectionNum = stoi(temp);
	}

	//get number of lines in section to save time in memory lookups
	numOfLines = arrayOfSectionsPtr[sectionNum - 1]->getNumOfLines();

	//send instruction to change section
	cout << "Please enter the number of the line that you wish to alter starting at 1 and ending with " << numOfLines << ": ";
	getline(cin, temp);
	lineNum = stoi(temp);
	while (lineNum > numOfLines || lineNum < 1) {
		cout << "That is an invalid line number." << endl;
		cout << "Please enter the number of the line that you wish to alter starting at 1 and ending with " << numOfLines << ": ";
		getline(cin, temp);
		lineNum = stoi(temp);
	}
	arrayOfSectionsPtr[sectionNum - 1]->editSpecificLine(lineNum);
}

void Article::editArticle() {
	string doAgain = "NO";
	string nextLine = "STOP";

	try {
		//get new title
		system("cls");
		cout << "Current title: " << articleTitle << endl;
		cout << "What new title would you like to give this article? ";
		getline(cin, articleTitle);
		articleTitle[0] = toupper(articleTitle[0]);
		for (int i = 1; i < articleTitle.length(); i += 1) {
			articleTitle[i] = tolower(articleTitle[i]);
		}
		validateInput(articleTitle);
		DoubleMetaphone(articleTitle, &titleMetaphoneCodes);

		for (int i = 0; i < numOfSections; i += 1) {
			arrayOfSectionsPtr[i]->editSection();
			system("cls");
		}
	}
	catch (bool) {
		cout << "You cannot use « or » anywhere in your text." << endl;
		system("pause");
	}
}

string Article::getArticleTitle() const {
	return articleTitle;
}

void Article::exportArticle(ofstream &file) {
	//uses ASCII 174 and 175 to indicate start of certain things

	//save article title
	file << "\t»ArticleTitle: «" << articleTitle << endl;
	//save article title codes
	file << "\t»Code 1: «" << titleMetaphoneCodes[1] << endl;
	file << "\t»Code 2: «" << titleMetaphoneCodes[2] << endl;
	//save sections
	for (int i = 0; i < numOfSections; i += 1) {
		arrayOfSectionsPtr[i]->exportSection(file);
	}
}

//this function will not work due to the fact that the console and Visual Studio save and encode the text in different formats
//I don't know how to fix this in a reliable manner that will work if I were to hand this cpp file to someone else
//The code itself should work if it was any other character
void Article::validateInput(string test) {
	//ensure that the user does not input ASCII 174 or 175
	for (int i = 0; i < test.length(); i += 1) {
		if ((test[i] == '«') || (test[i] == '»')) {
			throw false;
		}
	}
}

void Article::importArticle(ifstream &file) {
	//initialize variables
	string line = "";
	vector<string> componentsOfString{" "};
	vector<string> titleComponents{ " " };
	Section * tempPtr = nullptr;

	//get the next line until it sees }
	while (file.peek() != '}' && componentsOfString[0] != "}") {
		getline(file, line);
		//ensures that line is not solely whitespace characters
		if (line != "\t" && line != " "&& line != "\n" && line != "\v" && line != "\f" && line != "\r" && line != "{" && line != "}" && line != "«" && line != "»") {
			//reset the vector and then fill it
			componentsOfString.clear();
			titleComponents.clear();
			split(line, ' ', componentsOfString);
			split(line, '«', titleComponents);
			if (componentsOfString[0] == "\t»ArticleTitle:") {
				articleTitle = titleComponents[1];
			}
			else if (titleComponents[0] == "\t»Code 1: ") {
				titleMetaphoneCodes.push_back(titleComponents[1]);
			}
			else if (titleComponents[0] == "\t»Code 2: ") {
				titleMetaphoneCodes.push_back(titleComponents[1]);
			}
			else if (componentsOfString[0] == "\t»SectionTitle:") {
				tempPtr = new Section();
				tempPtr->importSectionTitle(file, titleComponents[1]);
				arrayOfSectionsPtr[numOfSections] = tempPtr;
				numOfSections += 1;
			}
			else if (componentsOfString[0] == "\tLine") {
				arrayOfSectionsPtr[numOfSections-1]->importSectionLine(file, line);
			}
		}
	}
}

void Article::split(const string &s, char delim, vector <string> &tokens) {
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

void Article::getTitleCodes(vector <string> &codes) {
	codes = titleMetaphoneCodes;
}