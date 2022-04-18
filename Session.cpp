/* Purpose: Implementation for the Session class
 *
 * Developer: Joseph Godwin
 *
 * Last Updated: 17 April 2022
 * 
 * Course: COP 4635
 */
#include "Session.hpp"

bool Session::addToActive(string name, int id){

	// CHECK IF USER IS ALREADY LOGGED IN
	for(int i = 0; i < activeUsers.size(); i++){
		if(activeUsers[i].username == name)
			return false;
	}

	/* ADD USER INFO TO CURRENT SESSION */
	activeUsers.push_back(activeUser());
	int lastIndex = activeUsers.size() - 1;
	activeUsers[lastIndex].username = name;
	activeUsers[lastIndex].socket_id = id;

	return true;
}

bool Session::removeFromActive(int id){
	// SEARCH FOR USER AND REMOVE FROM SESSION
	// IF NOT LOGGED IN, RETURN FALSE
	for(int i = 0; i < activeUsers.size(); i++){
		if(activeUsers[i].socket_id == id){
			activeUsers.erase(activeUsers.begin() + i);
			return true;
		}
	}
	return false;
}

int Session::getSocket(string username){
	// SEARCH FOR USER IN CURRENT SESSION
	// IF LOGGED IN, RETURN THE USER'S SOCKET
	for(int i = 0; i < activeUsers.size(); i++){
		if(activeUsers[i].username == username){
			return activeUsers[i].socket_id;
		}
	}

	// IF NOT LOGGED IN, RETURN -1
	return -1;
}

string Session::getUser(int socket){
	// SEARCH FOR SOCKET IN CURRENT SESSION
	// IF SOCKET EXISTS, RETURN THE USERNAME
	for(int i = 0; i < activeUsers.size(); i++){
		if(activeUsers[i].socket_id == socket){
			return activeUsers[i].username;
		}
	}

	// IF NOT LOGGED IN, RETURN -1
	return "USER NOT ONLINE";
}

bool Session::subToLocation(int socket, string location){
	//FIND USER IN ACTIVEUSERS
	int indexOfUser;
	for(int i = 0; i < activeUsers.size(); i++){
		if(activeUsers[i].socket_id == socket){
			indexOfUser = i;
			break;
		}
	}
	//CHECK IF USER IS SUBBED TO LOCATION ALREADY
	for(int i = 0; i < activeUsers[indexOfUser].locations.size(); i++){
		if(activeUsers[indexOfUser].locations[i] == location){
			return false;
		}
	}

	// SUB TO LOCATION
	activeUsers[indexOfUser].locations.push_back(location);
	return true;
}

bool Session::unsubFromLocation(int socket, string location){
	//FIND USER IN ACTIVEUSERS
	int indexOfUser;
	for(int i = 0; i < activeUsers.size(); i++){
		if(activeUsers[i].socket_id == socket){
			indexOfUser = i;
			break;
		}
	}

	//MAKE SURE USER IS SUBBED TO LOCATION
	for(int i = 0; i < activeUsers[indexOfUser].locations.size(); i++){
		if(activeUsers[indexOfUser].locations[i] == location){
			activeUsers[indexOfUser].locations.erase(activeUsers[indexOfUser].locations.begin()+i);
			return true;
		}
	}
	return false;
}

string Session::getSubscriptionList(int socket){

	//FIND USER IN ACTIVEUSERS
	int indexOfUser;
	for(int i = 0; i < activeUsers.size(); i++){
		if(activeUsers[i].socket_id == socket){
			indexOfUser = i;
			break;
		}
	}

	string output;
	if(activeUsers[indexOfUser].locations.size() == 0){
		return "NOT SUBSCRIBED TO ANY LOCATIONS\n";
	}
	output += "\n";
	for(int i = 0; i < activeUsers[indexOfUser].locations.size(); i++){
		output += to_string(i+1);
		output += " - ";
		output += activeUsers[indexOfUser].locations[i];
		output += "\n";
	}
	return output;
}

string Session::getAllActiveUsers(){
	string output;
	for(int i = 0; i < activeUsers.size(); i++){
		output += activeUsers[i].username;
		output += "\n";
	}
	return output;
}

int Session::getUserIndex(string username){
	for(int i = 0; i < activeUsers.size(); i++){
		if(activeUsers[i].username == username){
			return i;
		}
	}
	return -1;
}

void Session::addMessage(int index, string message){
	if(activeUsers[index].messageCount < 10){
		activeUsers[index].messages[activeUsers[index].messageCount] = message;
		activeUsers[index].messageCount++;
	}
	else{
		//shift messages, removing oldest
		for(int i = 0; i < 9; i++){
			activeUsers[index].messages[i] = activeUsers[index].messages[i++];
		}
		//add new message 
		activeUsers[index].messages[9] = message;
	}
	return;
}

string Session::getMessages(string username){
	//get active user associated with username
	int indexOfUser = getUserIndex(username);
	std::string messages = "";
	for(int i = 0; i < 10; i++){
		if(activeUsers[indexOfUser].messages[i] != ""){
			messages += activeUsers[indexOfUser].messages[i];
			messages += "*******************************\n";
		}
		else
			break;
	}
	if(messages == "")
		messages = "No Recent Messages\n";

	return messages;

}