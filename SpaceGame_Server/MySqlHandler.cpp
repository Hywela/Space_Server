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
	  con->setSchema("test");

  stmt = con->createStatement();
  res = stmt->executeQuery("SELECT id FROM test");
  while (res->next()) {
    cout << "\t... MySQL replies: ";
    /* Access column data by alias or column name */
    cout << res->getInt("id") << endl;
    cout << "\t... MySQL says it again: ";
    /* Access column fata by numeric offset, 1 is the first column */
    cout << res->getString(1) << endl;
}
}
