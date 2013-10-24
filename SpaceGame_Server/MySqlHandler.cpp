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
	con->setSchema("");
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
