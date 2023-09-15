#pragma once
#include <iostream>
#include <new>
#include <fstream>
#include "Article.h"
using namespace std;

class Database {
	private:
		Article * arrayOfArticlesPtr[1000]{ nullptr };
		int numOfArticles = 0;

		string printMenu();
		int getUserInput();
		int addNewArticle(Article *[], int);
		void sortArticlesByTitle(Article *[], int, int);
		int partition(Article *[], int, int);
		void swap(Article* &, Article* &);
		void exportArticlesToFile(Article *[], int);
		void importArticlesFromFile(Article *[], int &);
		int searchArticlesByTitle(Article *[], int);
		int recursiveBinarySearch(Article *[], int, int, string);
		bool doWordsMatch(string, vector <string>, string);
		int runComparisonsReturnLowest(int, int, int);
		int levenshteinDistance(string, string);
		bool isSubString(string &, string &);
		void deleteArticle(Article *[], int &);
		void editArticle(Article *[], int &);
	public:
		//no constructor as variables were initialized in-line
		~Database();

		//this function is the only one that is publicly accessible as it provides the menu of options
		void accessDatabase();
};

