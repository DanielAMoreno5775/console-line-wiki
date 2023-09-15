#include "Database.h"

//destructor to handle dynamically allocated memory for each article
Database::~Database (){
	for (int i = 0; i < numOfArticles; i++) {
		delete arrayOfArticlesPtr[i];
		arrayOfArticlesPtr[i] = nullptr;
	}
}

//function to call all of the other private functions and keep accessing the database until the user asks to quit
void Database::accessDatabase() {
	//initialize variables
	int choice = 0;
	int returnedSearchIndex = 0;

	//get user input, print the menu, and return valid choice
	choice = getUserInput();

	//keep asking for input until a 7 is entered, then exit the program
	while (choice != 7) {
		//if 1 was entered, search article database and print result
		if (choice == 1) {
			returnedSearchIndex = searchArticlesByTitle(arrayOfArticlesPtr, numOfArticles);
			if (returnedSearchIndex != -1) {
				arrayOfArticlesPtr[returnedSearchIndex]->printArticle();
			}
			else {
				cout << "Article not found" << endl;
			}
			system("pause");
		}
		//if 2 was entered, create a new article
		else if (choice == 2) {
			numOfArticles = addNewArticle(arrayOfArticlesPtr, numOfArticles);
		}
		//if 3 was entered, search article database and edit an existing article
		else if (choice == 3) {
			editArticle(arrayOfArticlesPtr, numOfArticles);
			system("pause");
		}
		//if 4 was entered, search article database and delete an existing article
		else if (choice == 4) {
			deleteArticle(arrayOfArticlesPtr, numOfArticles);
			system("pause");
		}
		//if 5 was entered, export the article database to a file
		else if (choice == 5) {
			try {
				exportArticlesToFile(arrayOfArticlesPtr, numOfArticles);
				system("pause");
			}
			catch (string) {
				cout << endl << endl << "You chose not to overwrite the previous version which means that you have not exported the article database." << endl;
				system("pause");
			}
		}
		//if 6 was entered, import the article database from a file
		else if (choice == 6) {
			importArticlesFromFile(arrayOfArticlesPtr, numOfArticles);
			system("pause");
		}
		//get the next user input
		choice = getUserInput();
	}
}

//function to display the menu
string Database::printMenu() {
	//initialize variables
	//a string is used here to prevent an infinite loop forming if the input would cause integer overflow
	string choice = "";
	//clear the screen to print the menu
	system("cls");
	//actually print everything
	cout << "******************************************************" << endl;
	cout << "  Console Window Wikipedia" << endl;
	cout << "******************************************************" << endl;
	cout << "  1) Search Articles" << endl;
	cout << "  2) Create Article" << endl;
	cout << "  3) Edit Article" << endl;
	cout << "  4) Delete Article" << endl;
	cout << "  5) Save Database" << endl;
	cout << "  6) Load Database" << endl;
	cout << "  7) Exit Program" << endl;
	cout << "  Please select an option by entering the associated number : ";
	//get the user input
	getline(cin, choice);

	//return the user's choice
	return choice;
}

//function to keep asking the user until valid input
int Database::getUserInput() {
	//initialize variables
	string choice = "";

	//get the user's input
	choice = printMenu();

	//keep getting input until a valid input is provided
	while (choice != "1" && choice != "2" && choice != "3"  && choice != "4"  && choice != "5"  && choice != "6"  && choice != "7") {
		//print error message
		cout << endl << "Invalid Option Entered" << endl;
		//pause the screen
		system("pause");
		//get the user's input
		choice = printMenu();
	}

	return stoi(choice);
}

//function to create the new article when option 2 is selected
int Database::addNewArticle(Article * arrayOfArticlesPtr[], int currentNumOfArticles) {
	if (currentNumOfArticles < 1000) {
		try {
			//initialize the pointer variable
			Article * tempPtr = nullptr;
			//dynamically allocate memory for a new article
			tempPtr = new Article();
			tempPtr->createArticle();
			//store the pointer to the new article in the array and increment the counter
			arrayOfArticlesPtr[currentNumOfArticles] = tempPtr;
			currentNumOfArticles += 1;
		}
		catch (bad_alloc) {
			cout << "The program has run out of virtual memory." << endl;
			system("pause");
		}
		catch (...) {
			cout << "An unexpected error has occurred. Please check later to see if the problem has been resolved." << endl;
			system("pause");
		}
	}
	else {
		cout << "The article database is full. No new articles can be added at this time." << endl;
		system("pause");
	}

	return currentNumOfArticles;
}

//function to sort all articles by their title using the quicksort algorithm
void Database::sortArticlesByTitle(Article * set[], int start, int end) {
	//initialize variable
	int pivotPoint = 0;

	//base case
	if (start >= end) {
		return;
	}
	//recursive case
	else {
		//partition the array into two sections
		pivotPoint = partition(set, start, end);
		//sort the left section
		sortArticlesByTitle(set, start, pivotPoint - 1);
		//sort the right section
		sortArticlesByTitle(set, pivotPoint + 1, end);
	}
}

//function used by sortArticlesByTitle's algorithm
int Database::partition(Article * set[], int start, int end) {
	//initialize variables
	int pivotIndex = 0,
		midIndex = (start + end) / 2;
	string pivotValue = "",
		scanValue = "";

	//swap the first and middle elements
	swap(set[start], set[midIndex]);

	//store the new first element
	pivotIndex = start;
	pivotValue = set[pivotIndex]->getArticleTitle();

	//sort everything around the pivotIndex so everything to the left is smaller than pivotValue and everything to the right is larger
	for (int scan = start + 1; scan <= end; scan += 1) {
		scanValue = set[scan]->getArticleTitle();
		if (scanValue < pivotValue) {
			pivotIndex += 1;
			swap(set[pivotIndex], set[scan]);
		}
	}
	swap(set[start], set[pivotIndex]);

	//return the pivotIndex for use to split the array into sections for quicksort
	return pivotIndex;
}

//function used by the partition function to move around the pointers
void Database::swap(Article* &val1, Article* &val2) {
	Article* temp = val1;
	val1 = val2;
	val2 = temp;
}

//function to export the article database to a file
void Database::exportArticlesToFile(Article * arrayOfArticlesPtr[], int currentNumOfArticles) {
	//initialize variables
	ofstream exportFile;
	ifstream backupFile;
	int result = 1;
	string choice = "";

	//create the backup of the previous export file if it exists
	backupFile.open("articleDatabaseSaveFile.txt", ios::_Nocreate);
	//if the backup file is successfully opened
	if (!backupFile.fail()) {
		backupFile.close();
		remove("articleDatabaseSaveFile_backup.txt");
		result = rename("articleDatabaseSaveFile.txt", "articleDatabaseSaveFile_backup.txt");
		if (result == 0) {
			cout << endl << endl << "Backup made" << endl;
		}
		else {
			cout << endl << endl;
			perror("Error making the backup");
			cout << "Do you still wish to save the article database which will wipe the pre-existing file (YES/NO)? ";
			getline(cin, choice);
			for (int i = 0; i < choice.length(); i += 1) {
				choice[i] = toupper(choice[i]);
			}
			if (choice == "NO") {
				throw choice;
			}
		}
	}
	else {
		cout << endl << endl << "There is not a previous version present for a backup to be made." << endl;
	}
	system("pause");

	//open the file; overwrite any preexisting contents if the file already exists
	exportFile.open("articleDatabaseSaveFile.txt");

	//if the file is successfully opened
	if (!exportFile.fail()) {
		for (int i = 0; i < currentNumOfArticles; i += 1) {
			exportFile << "{" << endl;
			arrayOfArticlesPtr[i]->exportArticle(exportFile);
			exportFile << "}" << endl;
		}
		cout << endl << endl << "Export complete" << endl;
	}
	else {
		cout << endl << endl << "The program was unable to open/create the file it will export the database to." << endl;
	}

	exportFile.close();
}

//function to import the article database from a file
void Database::importArticlesFromFile(Article * arrayOfArticlesPtr[], int &currentNumOfArticles) {
	//initialize variables
	ifstream dataFile;
	string line = "";
	char ch = ' ';
	Article * tempPtr = nullptr;

	//open the file
	dataFile.open("articleDatabaseSaveFile.txt", ios::_Nocreate);

	//if the file is successfully opened
	if (!dataFile.fail()) {
		while (dataFile) {
			//eat up leading whitespace
			dataFile >> ws;
			//look at next character without moving the file pointer forward
			ch = dataFile.peek();
			if (ch == '{') {
				try {
					//reset the pointer variable
					tempPtr = nullptr;
					//dynamically allocate memory for a new article
					tempPtr = new Article();
					tempPtr->importArticle(dataFile);
					//store the pointer to the new article in the array and increment the counter
					arrayOfArticlesPtr[currentNumOfArticles] = tempPtr; //tempPtr isn't being stored for articles after the first
					currentNumOfArticles += 1;
				}
				catch (bad_alloc) {
					cout << "The program has run out of virtual memory." << endl;
					system("pause");
				}
				catch (...) {
					cout << "An unexpected error has occurred. Please check later to see if the problem has been resolved." << endl;
					system("pause");
				}
			}
			//get the next line after the article is done until the file is done
			getline(dataFile, line);
		}

		cout << endl << endl << "File import complete" << endl;
	}
	else {
		cout << endl << endl << "The program was unable to open the file it will import the database from." << endl;
	}

	dataFile.close();
}

//function to locate a specified article
int Database::searchArticlesByTitle(Article * set[], int currentNumOfArticles) {
	string enteredArticleTitle = "";
	int returnedIndex = 0;
	vector<string> doubleMetaphoneCodes{ " " };
	vector<string> articleTitleCodes;
	string fuzzySearchGuessRightWrong = "NO";
	string currentTitle = "";
	int metaphoneBasedGuess = 0;
	bool articleFound = false;
	int difference = INT_MAX;
	int minimalDiffIndex = 0;
	int lD = 0;

	system("cls");
	//sort articles for binary search
	sortArticlesByTitle(set, 0, currentNumOfArticles - 1);

	//get the desired article's title
	cout << "What is the title of the article that you would like to find? ";
	getline(cin, enteredArticleTitle);
	//format title to fit style of Ulllllllll...
	enteredArticleTitle[0] = toupper(enteredArticleTitle[0]);
	for (int i = 1; i < enteredArticleTitle.length(); i += 1) {
		enteredArticleTitle[i] = tolower(enteredArticleTitle[i]);
	}

	//find article via binary search
	try {
		if (currentNumOfArticles > 1) {
			returnedIndex = recursiveBinarySearch(set, 0, currentNumOfArticles - 1, enteredArticleTitle);
			return returnedIndex;
			articleFound = true;
		}
		else {
			cout << "You must create or import articles before you can search for them." << endl;
		}
	}
	//catch block is called if recursiveBinarySearch throws an error because the exact article was not found
	catch (int) {
		//see if user entered a substring of an article title
		for (int i = 0; i < currentNumOfArticles; i += 1) {
			currentTitle = set[i]->getArticleTitle();
			if (isSubString(enteredArticleTitle, currentTitle)) {
				//ask the user for confirmation that guess is correct
				cout << "Did you mean to enter " << currentTitle << " (YES/NO)? ";
				getline(cin, fuzzySearchGuessRightWrong);
				for (int j = 0; j < fuzzySearchGuessRightWrong.length(); j += 1) {
					fuzzySearchGuessRightWrong[j] = toupper(fuzzySearchGuessRightWrong[j]);
				}
				if (fuzzySearchGuessRightWrong == "YES") {
					return i;
					articleFound = true;
				}
				break;
			}
		}

		//see if user's entry matches the metaphone code of a different article
		//a phonetic algorithm is more computationally efficient and does a better job of reflecting a human's intent
		//(smythe and smote have same LD from smith but person entering smith is more likely to want smythe than smote)
		if (!articleFound) {
			DoubleMetaphone(enteredArticleTitle, &doubleMetaphoneCodes);
			for (int i = 0; i < currentNumOfArticles; i += 1) {
				articleTitleCodes.clear();
				set[i]->getTitleCodes(articleTitleCodes);
				//check if it is a match and ask the user for confirmation
				if (doWordsMatch(enteredArticleTitle, articleTitleCodes, "Normal")) {
					cout << "Did you mean to enter " << set[i]->getArticleTitle() << " (YES/NO)? ";
					getline(cin, fuzzySearchGuessRightWrong);
					for (int j = 0; j < fuzzySearchGuessRightWrong.length(); j += 1) {
						fuzzySearchGuessRightWrong[j] = toupper(fuzzySearchGuessRightWrong[j]);
					}
					if (fuzzySearchGuessRightWrong == "YES") {
						return i;
						articleFound = true;
					}
					break;
				}
				//store the edit distance and substring thing for later but just skip over this if article has already been found
				//doing it here saves processing time later
				if (!articleFound) {
					for (int j = 1; j < 3; j += 1) {
						for (int k = 1; k < 3; k += 1) {
							if (isSubString(doubleMetaphoneCodes[j], articleTitleCodes[k])) {
								metaphoneBasedGuess = i;
							}
							lD = levenshteinDistance(doubleMetaphoneCodes[j], articleTitleCodes[k]);
							if (difference > lD) {
								difference = lD;
								minimalDiffIndex = i;
							}
						}
					}
				}
			}
		}

		//if the article is still not found, suggest the article with a metaphone code that contains the code of the user's entry as a substring
		if (!articleFound) {
			cout << "Did you mean to enter " << set[metaphoneBasedGuess]->getArticleTitle() << " (YES/NO)? ";
			getline(cin, fuzzySearchGuessRightWrong);
			for (int i = 0; i < fuzzySearchGuessRightWrong.length(); i += 1) {
				fuzzySearchGuessRightWrong[i] = toupper(fuzzySearchGuessRightWrong[i]);
			}
			if (fuzzySearchGuessRightWrong == "YES") {
				return metaphoneBasedGuess;
				articleFound = true;
			}
		}

		//if the article is still not found, suggest the article with the closest metaphone code
		if (!articleFound) {
			cout << "Did you mean to enter " << set[minimalDiffIndex]->getArticleTitle() << " (YES/NO)? ";
			getline(cin, fuzzySearchGuessRightWrong);
			for (int i = 0; i < fuzzySearchGuessRightWrong.length(); i += 1) {
				fuzzySearchGuessRightWrong[i] = toupper(fuzzySearchGuessRightWrong[i]);
			}
			if (fuzzySearchGuessRightWrong == "YES") {
				return minimalDiffIndex;
				articleFound = true;
			}
		}
	}
	return -1;
}

//function to return the index of the desired search value after recursively going through the array (binary search algorithm)
int Database::recursiveBinarySearch(Article * set[], int first, int last, string desiredValue) {
	//initialize variables
	int midIndex = 0;
	string midTitle = "";
	//item not found so throw an error
	if (first > last) {
		throw (-1);
	}
	//get index and title of pivot point article
	midIndex = (first + last) / 2;
	midTitle = set[midIndex]->getArticleTitle();
	//item found so return index of item
	if (midTitle == desiredValue) {
		return midIndex;
	}
	//if pivot title is less than desiredValue, check second half of array
	if (midTitle < desiredValue) {
		return recursiveBinarySearch(set, midIndex + 1, last, desiredValue);
	}
	//if pivot title is greater than desiredValue, check first half of array
	else {
		return recursiveBinarySearch(set, first, midIndex - 1, desiredValue);
	}
}

//function to match the metaphone codes of a term against a dictionary of codes
//strong: str1 code must match equivalent in the dictionary
//normal (default): primary str1 code must match at least one of the codes in the dictionary
//weak: at least 1 of the str1 codes must match at least one of the codes in the dictionary
bool Database::doWordsMatch(string str1, vector <string> codeDictionary, string matchThreshold = "NORMAL") {
	//initialize variables
	bool wordsMatch = false;
	bool wordsMatchPrimary = false;
	bool wordsMatchSecondary = false;
	for (int i = 0; i < matchThreshold.length(); i += 1) {
		matchThreshold[i] = toupper(matchThreshold[i]);
	}
	vector<string> doubleMetaphoneCodesOfStr{ " " };

	//get codes for str1
	DoubleMetaphone(str1, &doubleMetaphoneCodesOfStr);

	//primary str1 code must match at least one of the codes in the dictionary
	if (matchThreshold == "NORMAL") {
		for (int i = 1; i < codeDictionary.size(); i += 1) {
			if (doubleMetaphoneCodesOfStr[1] == codeDictionary[i]) {
				wordsMatch = true;
			}
		}
	}
	//str1 code must match equivalent in the dictionary
	else if (matchThreshold == "STRONG") {
		if (doubleMetaphoneCodesOfStr[1] == codeDictionary[1]) {
			wordsMatchPrimary = true;
		}
		if (doubleMetaphoneCodesOfStr[2] == codeDictionary[2]) {
			wordsMatchSecondary = true;
		}
		if (wordsMatchPrimary && wordsMatchSecondary) {
			wordsMatch = true;
		}
	}
	//at least 1 of the str1 codes must match at least one of the codes in the dictionary
	else if (matchThreshold == "WEAK") {
		for (int i = 1; i < codeDictionary.size(); i += 1) {
			if (doubleMetaphoneCodesOfStr[1] == codeDictionary[i]) {
				wordsMatch = true;
			}
			if (doubleMetaphoneCodesOfStr[2] == codeDictionary[i]) {
				wordsMatch = true;
			}
		}
	}

	return wordsMatch;
}

//function to run comparisons for the Levenshtein Distance matrix calculations
int Database::runComparisonsReturnLowest(int a, int b, int c) {
	if (a < b) {
		if (a < c) {
			return a;
		}
		else {
			return c;
		}
	}
	else {
		if (b < c) {
			return b;
		}
		else {
			return c;
		}
	}
}

//function to calculate edit distance between two metaphone codes if no exact code matches
int Database::levenshteinDistance(string str1, string str2) {
	int s1len, s2len, x, y, lastdiag, olddiag;
	s1len = str1.length();
	s2len = str2.length();
	//creates a matrix with str1 on the top horizontally and str2 on the side vertically, the distance will end up in the lower right-hand corner
	int* column = new int[s1len + 1];
	for (y = 1; y <= s1len; ++y) {
		column[y] = y;
	}
	for (x = 1; x <= s2len; ++x) {
		column[0] = x;

		for (y = 1, lastdiag = x - 1; y <= s1len; ++y) {
			olddiag = column[y];
			column[y] = runComparisonsReturnLowest(column[y] + 1, column[y - 1] + 1, lastdiag + (str1[y - 1] == str2[x - 1] ? 0 : 1));
			lastdiag = olddiag;
		}
	}

	return(column[s1len]);
}

//function to determine whether str1 is a subsequence of str2 (returns true if so)
bool Database::isSubString(string &str1, string &str2) {
	//initialize variables
	int len1 = str1.length();
	int len2 = str2.length();
	int i = 0;
	int j = 0;

	//if current character on both indexes is same, then increment both pointers; otherwise increment j or pointer for str2
	while ((i < len1) && (j < len2)) {
		if (str1[i] == str2[j]) {
			i++;
		}
		j++;
	}

	//if i reaches the end of str1, all characters in str1 were found in str2
	return (i == len1);
}

//function to delete an article after finding it via search
void Database::deleteArticle(Article * set[], int &currentNumOfArticles) {
	//initialize variables
	int returnedSearchIndex = 0;

	try {
		//get index of search result
		returnedSearchIndex = searchArticlesByTitle(set, currentNumOfArticles);
		if (returnedSearchIndex == -1) {
			throw (-1);
		}

		//swap position of found article and last article
		swap(set[returnedSearchIndex], set[currentNumOfArticles - 1]);

		//deallocate space for found article now in the last space & call destructor for the article
		delete set[currentNumOfArticles - 1];

		//reset the pointer in the array so that it doesn't point to an empty space
		set[currentNumOfArticles - 1] = nullptr;

		//decrement the counter to reflect the new number of articles
		currentNumOfArticles -= 1;

		cout << endl << "The article has been succesfully deleted" << endl;
	}
	catch (int) {
		cout << endl << "The specified article was not found" << endl;
	}
	catch (...) {
		cout << endl << "Error occurred while trying to delete the article" << endl;
	}
}

//function to edit an article after finding it via search
void Database::editArticle(Article * set[], int &currentNumOfArticles) {
	//initialize variables
	int returnedSearchIndex = 0;
	string choice = "";

	//print menu of options
	system("cls");
	cout << "******************************************************" << endl;
	cout << "  Menu of Edit Options" << endl;
	cout << "******************************************************" << endl;
	cout << "  1) Edit Article" << endl;
	cout << "  2) Edit Specific Line Within an Article" << endl;
	cout << "  Please select an option by entering the associated number : ";
	//get the user input
	getline(cin, choice);
	//keep getting input until a valid input is provided
	while (choice != "1" && choice != "2") {
		//print error message
		cout << endl << "Invalid Option Entered" << endl;
		//pause the screen
		system("pause");
		//print the menu and get the user's input
		system("cls");
		cout << "******************************************************" << endl;
		cout << "  Menu of Edit Options" << endl;
		cout << "******************************************************" << endl;
		cout << "  1) Edit Article" << endl;
		cout << "  2) Edit Specific Line Within an Article" << endl;
		cout << "  Please select an option by entering the associated number : ";
		getline(cin, choice);
	}

	//get index of search result
	try {
		returnedSearchIndex = searchArticlesByTitle(set, currentNumOfArticles);
		if (returnedSearchIndex == -1) {
			throw (-1);
		}

		//edit article
		if (choice == "1") {
			set[returnedSearchIndex]->editArticle();
		}
		//edit specific line within an article
		else if (choice == "2") {
			set[returnedSearchIndex]->editLineWithinArticle();
		}
	}
	catch (int) {
		cout << endl << "The specified article was not found" << endl;
	}
}