#pragma once
class User
{
private:
	int clientNumber;
	bool inMatch;
	bool inQue;
	int inMatchWith;

public:
	User(int clientNumber);
	~User();
	void setInQueForMatch();
	void setUserInMatch(int inMatchWith);
	bool inQueForMatch();
	bool userInMatch();
	void matchDone();
	int inFigthWith();
};

