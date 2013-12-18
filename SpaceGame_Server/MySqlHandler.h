#pragma once

/* Standard C++ includes */

#include <stdlib.h>
#include <iostream>
#include <string>
/*
  Include directly the different
  headers from cppconn/ and mysql_driver.h + mysql_util.h
  (and mysql_connection.h). This will reduce your build time!
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
class MySqlHandler
{
private:
		sql::Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;
		sql::ResultSet *res;
		sql::PreparedStatement  *prep_stmt;
		string HOST;
		string DB;
		string PWD;
		int PORT;
public:
	MySqlHandler(void);
	~MySqlHandler(void);
private:
	
public:
	void set_database_info(string host, string db, int port, string pwd);
	void init_connection();
	void handler_test(void);
	void saveShip(int userID, string ship);
	string getShip(int userID);
	void handler_set_news(string input);
	void handler_set_ship(int userID, string shipName, string ship);

	

	//online
	void handler_set_online(int input);
	void handler_update_online_status(int input);
	void handler_return_users_online();
	void handler_in_game();
	int handler_getID(string input, string pwd);
};

