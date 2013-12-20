#pragma once
#include <string>
class User
{
private:
	int clientNumber;
	bool inMatch;		// Is user in a match
	bool inQue;			// Is user in the que
	int inMatchWith;	// Whose the user in a match with (clientNumber)
	std::string userName;	// whats the users name
	int dbId;			// the id of the user in the database
public:
	User(int clientNumber);
	~User();
	void setInQueForMatch();	// Sets the user in a que or not
	void setUserInMatch(int inMatchWith); // sets the user in a match and out of the que
	bool inQueForMatch();		// returns inQue
	bool userInMatch();			// returns inQue
	void matchDone();			// is the match done TODO::
	int inFigthWith();			// returns inMatchWith
	void setUserName(std::string name, int id); // sets the username and the database id
	int getDBID();				// returns database id
	int getClientNumber();		// returns clientNumber
};

