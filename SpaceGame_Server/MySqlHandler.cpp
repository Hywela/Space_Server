#include "MySqlHandler.h"


MySqlHandler::MySqlHandler(void)
{
	
}


MySqlHandler::~MySqlHandler(void)
{
  delete res;
  delete stmt;
  delete con;
}

string MySqlHandler:: getShip(int userID){
	

		string ship="";
try {

prep_stmt = con->prepareStatement("Select shipData FROM ships WHERE shipOwner=?");

		prep_stmt->setInt(1,userID);
		prep_stmt->execute();
		res = prep_stmt->getResultSet();

	if(	res->next()){
		ship = res->getString(1);
	}



	} catch (sql::SQLException &e) {
  cout << "# ERR: SQLException in " << __FILE__;
  cout << "(" << __FUNCTION__ << ") on line "<< __LINE__ << endl;
  cout << "# ERR: " << e.what();
  cout << " (MySQL error code: " << e.getErrorCode();
  cout << ", SQLState: " << e.getSQLState() << " )" << endl;

}

  delete stmt;

	return ship;
}
void MySqlHandler::saveShip(int userID, string ship){
try {

	prep_stmt = con->prepareStatement("Select shipOwner FROM ships WHERE shipOwner=?");

		prep_stmt->setInt(1,userID);
		prep_stmt->execute();
		res = prep_stmt->getResultSet();
	if(	res->next() == 0){
		prep_stmt = con->prepareStatement("INSERT INTO ships(shipOwner, shipData) VALUES(?,?)");

		prep_stmt->setInt(1,userID);
		prep_stmt->setString(2,ship);
		prep_stmt->execute();
	}else {
	
		prep_stmt = con->prepareStatement("UPDATE ships SET shipData=? WHERE shipOwner=?");
		prep_stmt->setInt(2,userID);
		prep_stmt->setString(1,ship);
		prep_stmt->execute();
	}

	} catch (sql::SQLException &e) {
  cout << "# ERR: SQLException in " << __FILE__;
  cout << "(" << __FUNCTION__ << ") on line "<< __LINE__ << endl;
  cout << "# ERR: " << e.what();
  cout << " (MySQL error code: " << e.getErrorCode();
  cout << ", SQLState: " << e.getSQLState() << " )" << endl;

}
  
  delete prep_stmt;
}
void MySqlHandler::set_database_info(string host, string db, int port, string pwd)
{
	HOST = host;
	DB = db;
	PORT = port;
	PWD = pwd;
}
void MySqlHandler::handler_set_ship(int userID , string shipName, string ship){
	prep_stmt = con->prepareStatement("INSERT INTO Ship(userID, shipName, ship) VALUES (?,?,?)");

	prep_stmt->setInt(1,userID);
	prep_stmt->setString(1,shipName);
	prep_stmt->setString(1,ship);
		prep_stmt->execute();


  
  delete prep_stmt;
}

void MySqlHandler::init_connection()
{
	try {
	driver = get_driver_instance();
	con = driver->connect("tcp://"+HOST, DB, PWD);
	con->setSchema("imt3601h13gr01");
	cout << "\n MySql Connection Established ";
	} catch (sql::SQLException &e) {
  cout << "# ERR: SQLException in " << __FILE__;
  cout << "(" << __FUNCTION__ << ") on line "<< __LINE__ << endl;
  cout << "# ERR: " << e.what();
  cout << " (MySQL error code: " << e.getErrorCode();
  cout << ", SQLState: " << e.getSQLState() << " )" << endl;
}





}


void MySqlHandler::handler_test(void)
{
	  

  stmt = con->createStatement();
  res = stmt->executeQuery("SELECT name FROM User");
  while (res->next()) {
    cout << "\n ... MySQL replies: ";
    /* Access column data by alias or column name */
    cout << res->getString("name") << endl;

}
}


void MySqlHandler::handler_set_news(string input)
{


prep_stmt = con->prepareStatement("INSERT INTO In_Game_News(news_text) VALUES (?)");

		prep_stmt->setString(1,input);
		prep_stmt->execute();


  
  delete prep_stmt;

}


// Sets the user online and removes user whose not been online in 30 min
void MySqlHandler::handler_set_online(int input)
{
	prep_stmt = con->prepareStatement("DELETE FROM Online_User WHERE time_start < (NOW() - INTERVAL 30 MINUTE)");
	
		prep_stmt->execute();
  delete prep_stmt;

prep_stmt = con->prepareStatement("INSERT INTO Online_User(userID) VALUES(?)");
	
		prep_stmt->setInt(1,input);
		prep_stmt->execute();

  delete prep_stmt;

}

void MySqlHandler :: handler_update_online_status(int input){
prep_stmt = con->prepareStatement("UPDATE Online_User SET time_start=NOW() WHERE userID=? ");
	
		prep_stmt->setInt(1,input);
		prep_stmt->execute();
	
  delete prep_stmt;

}

int MySqlHandler:: handler_getID(string input, string pwd){

	
	  int id=0;

	
try {

prep_stmt = con->prepareStatement("Select id FROM User WHERE name=? and pwd=?");
	
		prep_stmt->setString(1,input);
		prep_stmt->setString(2,pwd);
		prep_stmt->execute();
		res = prep_stmt->getResultSet();

	if(	res->next()){
		id = res->getInt(1);
	}



	} catch (sql::SQLException &e) {
  cout << "# ERR: SQLException in " << __FILE__;
  cout << "(" << __FUNCTION__ << ") on line "<< __LINE__ << endl;
  cout << "# ERR: " << e.what();
  cout << " (MySQL error code: " << e.getErrorCode();
  cout << ", SQLState: " << e.getSQLState() << " )" << endl;

}

	
	
  delete stmt;
  return id;
}