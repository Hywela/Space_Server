#pragma once
/* Standard C++ includes */
#include <stdlib.h>
#include <iostream>
#include <string>
/*
  Include SQL 
  headers from cppconn/ 
*/
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#pragma comment(lib,"mysqlcppconn.lib") 
#pragma comment(lib,"libmysql.lib")

using namespace std;
class MySqlHandler{
private:
	sql::Driver *driver; //
	sql::Connection *con; // Connection
	sql::Statement *stmt; // Statment
	sql::ResultSet *res;  // result
	sql::PreparedStatement  *prep_stmt; // preperd statment;
	string HOST;
	string DB;				// Db username
	string PWD;				// password to db
	int PORT;
public:
	MySqlHandler(void);
	~MySqlHandler(void);

	// Connection and test 
	void set_database_info(string host, string db, int port, string pwd);
	void init_connection();
	void handler_test();

	// Not in use
	void handler_set_news(string input);
	void handler_set_online(int input);
	void handler_update_online_status(int input);
	
	// Methods used for getting id ship and saving ship
	void saveShip(int userID, string ship);
	string getShip(int userID);
	int handler_getID(string input, string pwd);
};

