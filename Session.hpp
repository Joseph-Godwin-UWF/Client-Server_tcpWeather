/* Purpose: Class for keeping track of
            active users and their information
 *
 * Developer: Joseph Godwin
 *
 * Last Updated: 17 April 2022
 * 
 * Course: COP 4635
 */

#ifndef SESSION_HPP
#define SESSION_HPP

#include <vector>
#include <string>
#include <algorithm>
using namespace std;

struct activeUser{
	string username;
	int socket_id;
	string messages[10] = {""};
	int messageCount = 0;
	vector<string> locations;
};

class Session{
public:
	bool addToActive(string username, int socket_id);
	bool removeFromActive(int socket_id);
	int getSocket(string username);
	string getUser(int socket);

	bool subToLocation(int socket, string location);
	bool unsubFromLocation(int socket, string location);
	string getSubscriptionList(int socket);
	string getAllActiveUsers();
	int getUserIndex(string username);
	void addMessage(int index, string message);
	std::string getMessages(string username);
	


	vector<activeUser> activeUsers;
};

#endif //SESSION_HPP