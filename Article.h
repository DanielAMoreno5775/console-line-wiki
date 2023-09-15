#pragma once
#include "Section.h"
#include "DoubleMetaphone.h"
#include <vector>

class Article {
	private:
		Section * arrayOfSectionsPtr[200];
		string articleTitle;
		int numOfSections;
		vector<string> titleMetaphoneCodes{" "};

		void validateInput(string);
		void split(const string &, char, vector <string> &);
	public:
		Article();
		~Article();

		void createArticle();
		void printArticle();
		void printSpecificLineWithinArticle();
		void exportArticle(ofstream &);
		void importArticle(ifstream &);
		void editArticle();
		void editLineWithinArticle();
		string getArticleTitle() const;
		void getTitleCodes(vector <string> &);
};

