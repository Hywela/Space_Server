#include "User.h"
//Constructor
User::User(int clientNumber){
	this->clientNumber = clientNumber;
	inQue = false;
	inMatch = false;
	inMatchWith= -1; // -1 = no one
}
User::~User(){
}
//Methods
void User:: setInQueForMatch(){ 
	inQue = !inQue;
}// Sets the User to be in que or not	
void User:: setUserInMatch(int inMatchWith){
	this->inMatchWith = inMatchWith;
	setInQueForMatch();
	inMatch = true;
}
bool User:: inQueForMatch(){
	return inQue;
}
bool User:: userInMatch(){
	return	inMatch;
}
void User:: matchDone(){
	inMatch=false;
	inMatchWith = -1;
}
int User:: inFigthWith(){
	return inMatchWith;
}
void User::setUserName(std::string name, int id){
	userName = name;
	dbId = id;
}
int User::getDBID(){
	return dbId;
}
int User::getClientNumber(){
	return clientNumber;
}