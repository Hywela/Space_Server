#pragma once
#include <string>
class User
{
private:
	int clientNumber;
	bool inMatch;
	bool inQue;
	int inMatchWith;
	std::string userName;
	int dbId;
public:
	User(int clientNumber);
	~User();
	void setInQueForMatch();
	void setUserInMatch(int inMatchWith);
	bool inQueForMatch();
	bool userInMatch();
	void matchDone();
	int inFigthWith();
	void setUserName(std::string name, int id);
	int getDBID();
};

