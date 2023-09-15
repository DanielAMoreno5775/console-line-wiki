/*
This program creates a database of Wikipedia-style articles which can be accessed, modified, deleted, exported, and imported using a menu and a console window.

by Daniel Moreno
*/



#include "Database.h"
using namespace std;

int main() {
	//can technically have multiple article databases
	Database * databasePtr = nullptr;
	databasePtr = new Database;
	databasePtr->accessDatabase();

	return 0;
}