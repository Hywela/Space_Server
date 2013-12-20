#include "MySqlHandler.h"

//Constructor
MySqlHandler::MySqlHandler(void){	

}
MySqlHandler::~MySqlHandler(void)
{
	delete res;
	delete stmt;
	delete con;
}
//Methods
string MySqlHandler:: getShip(int userID){
	string ship="";
	try { // Selects ship
		prep_stmt = con->prepareStatement("Select shipData FROM ships WHERE shipOwner=?");
		prep_stmt->setInt(1,userID); // Where with userId
		prep_stmt->execute();		// execute the query
		res = prep_stmt->getResultSet(); // gets the result
		if(res->next()){			// if there is a result
			ship = res->getString(1); // get the data from the result
		}
	} catch (sql::SQLException &e) { // catch all kind of execption
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line "<< __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
	delete stmt; // deletes the statment 
	return ship; // and returns the string with data
}
void MySqlHandler::saveShip(int userID, string ship){
	
	try {  // selects the ships owner 
		prep_stmt = con->prepareStatement("Select shipOwner FROM ships WHERE shipOwner=?");
		prep_stmt->setInt(1,userID);
		prep_stmt->execute();
		res = prep_stmt->getResultSet();
		if(res->next() == 0){ // if the user dosent own a ship stored in the database
			// Insert a new ship into the DB
			prep_stmt = con->prepareStatement("INSERT INTO ships(shipOwner, shipData) VALUES(?,?)");
			prep_stmt->setInt(1,userID);
			prep_stmt->setString(2,ship);
			prep_stmt->execute();
		}else {	 // if it exsists then update it instead
			prep_stmt = con->prepareStatement("UPDATE ships SET shipData=? WHERE shipOwner=?");
			prep_stmt->setInt(2,userID);
			prep_stmt->setString(1,ship);
			prep_stmt->execute();
		} // catch all the execptions 
	} catch (sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line "<< __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
	//delete the prepared statment
	delete prep_stmt;
}
void MySqlHandler::set_database_info(string host, string db, int port, string pwd){
	HOST = host;
	DB = db;
	PORT = port;
	PWD = pwd;
}
void MySqlHandler::init_connection(){
	// Initiating the connector
	try {
		driver = get_driver_instance(); // gets the driver
		con = driver->connect("tcp://"+HOST, DB, PWD); // opens a connection
		con->setSchema("imt3601h13gr01"); // what table should it use
		cout << "\n MySql Connection Established ";
	} catch (sql::SQLException &e){
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line "<< __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
}
void MySqlHandler::handler_test(){
	// A test method to see if everything works
	stmt = con->createStatement();
	res = stmt->executeQuery("SELECT name FROM User");
	while (res->next()) {
		cout << "\n ... MySQL replies: ";
		/* Access column data by alias or column name */
		cout << res->getString("name") << endl;
	}
}
void MySqlHandler::handler_set_news(string input){
	// TODO:: This is a function currently not used
	// ment to set the news that the client would see in mainMenue ect
	prep_stmt = con->prepareStatement("INSERT INTO In_Game_News(news_text) VALUES (?)");
	prep_stmt->setString(1,input);
	prep_stmt->execute();
	delete prep_stmt;
}
void MySqlHandler::handler_set_online(int input){
// Sets the user online and removes user whose not been online in 30 min
	//NOT IN USE TODO:: 
	// yeah it says what it was supposed to be used for up there. 
	prep_stmt = con->prepareStatement("DELETE FROM Online_User WHERE time_start < (NOW() - INTERVAL 30 MINUTE)");
	prep_stmt->execute();
	delete prep_stmt;
	prep_stmt = con->prepareStatement("INSERT INTO Online_User(userID) VALUES(?)");
	prep_stmt->setInt(1,input);
	prep_stmt->execute();
	delete prep_stmt;
}
void MySqlHandler :: handler_update_online_status(int input){
	//NOT IN USE TODO::
	// was meant to be used to update the time so the user dident dc if he was active
	prep_stmt = con->prepareStatement("UPDATE Online_User SET time_start=NOW() WHERE userID=? ");
	prep_stmt->setInt(1,input);
	prep_stmt->execute();
	delete prep_stmt;
}
int MySqlHandler:: handler_getID(string input, string pwd){
	// Method used for getting the ID if 1 exsists from the lofin inputs
	int id=0;
	try {
		prep_stmt = con->prepareStatement("Select id FROM User WHERE name=? and pwd=?");
		prep_stmt->setString(1,input);
		prep_stmt->setString(2,pwd);
		prep_stmt->execute();
		res = prep_stmt->getResultSet();

		if(res->next()){
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
	return id; // returns the id if non found its 0;
}