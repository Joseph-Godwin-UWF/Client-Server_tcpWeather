/* Purpose: Class for handling user credentials
 *
 * Developer: Joseph Godwin
 *
 * Last Updated: 3 April 2022
 * 
 * Course: COP 4635
 */

#ifndef USER_HANDLER_HPP
#define USER_HANDLER_HPP
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdio>
using namespace std;

class UserHandler{
public:
	UserHandler(string);
	bool validateCredentials(string username, string password);
	bool registerUser(string username, string password);
	bool changePassword(string username, string currPass, string newPass);
private:
	int getIndex(string username);
	vector<string> usernames;
	vector<string> passwords;
	string userInfoFile;
};

#endif //USER_HANDLER_HPP