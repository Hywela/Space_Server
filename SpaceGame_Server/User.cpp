#include "User.h"


User::User(int clientNumber)
{
	this->clientNumber = clientNumber;
	inQue = false;
	inMatch = false;
	inMatchWith= -1;
}


User::~User()
{
}
// Sets the User to be in que or not 
void User:: setInQueForMatch(){
	inQue = !inQue;
};

void User:: setUserInMatch(int inMatchWith){
	this->inMatchWith = inMatchWith;
	setInQueForMatch();
	inMatch = true;
};

bool User:: inQueForMatch(){
return inQue;
};

bool User:: userInMatch(){
return	inMatch;
};

void User:: matchDone(){
	inMatch=false;
	inMatchWith = -1;
};
int User:: inFigthWith(){
return inMatchWith;
}