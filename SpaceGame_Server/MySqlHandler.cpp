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


void MySqlHandler::set_database_info(string host, string db, int port, string pwd)
{
	HOST = host;
	DB = db;
	PORT = port;
	PWD = pwd;
}


void MySqlHandler::init_connection()
{
	try {
	driver = get_driver_instance();
	con = driver->connect("tcp://"+HOST, DB, PWD);
	con->setSchema("s090376");
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

int MySqlHandler:: handler_getID(string input){
	  stmt = con->createStatement();
	  
res =  stmt->executeQuery("Select id FROM User WHERE name LIKE '"+input+"'");
	
res->next();
	int id = res->getInt(1);
	
	
  delete stmt, res;
  return id;
}