/* Purpose: implementation for UserHandler class
 *
 * Developer: Joseph Godwin
 *
 * Last Updated: 3 April 2022
 * 
 * Course: COP 4635
 */

#include "UserHandler.hpp"

UserHandler::UserHandler(string filename){
	ifstream fin;
	fin.open(filename.c_str());
	string word;
	int counter = 1;
	while(fin >> word){
		if(counter % 2 == 0){ //password
			passwords.push_back(word);
		}
		else{ //username
			usernames.push_back(word);
		}
		counter++;
	}
	userInfoFile = filename;

	//if last run was a username (no matching password)
	// throw error
	if(counter % 2 == 0){
		printf("[-] Error copying file to vectors in UserHandler\n");
	}
	fin.clear();                 // clear fail and eof bits
	fin.seekg(0, ios::beg); // back to the start!
	fin.close();

}

bool UserHandler::validateCredentials(string username, string password){
	// Search for index of username
	int indexOfUsername = getIndex(username);
	if(indexOfUsername == -1) // username doesnt exist
		return false;
	// Check if password at index matches
	if(passwords.at(indexOfUsername) == password){
		return true;
	}
	return false;
}

int UserHandler::getIndex(string username){
	for(int i = 0; i < usernames.size(); i++){
		if(usernames.at(i) == username)
			return i;
	}
	return -1;
}

bool UserHandler::registerUser(string username, string password){
	//check if user already exists
	if(!(getIndex(username) == -1))
		return false;
	//add username and password to vectors
	usernames.push_back(username);
	passwords.push_back(password);

	//add to file
	ofstream file;
	file.open(userInfoFile.c_str(), ios_base::app); //append to the file
	file << username << " " << password << endl;
	file.close();
	return true;
}

bool UserHandler::changePassword(string username, string currPass, string newPass){
	if(!validateCredentials(username, currPass)){
		return false;
	}
	ifstream userInfos("userInfo.txt");
	ofstream updatedFile("temp.txt");

	if(!userInfos || !updatedFile) return false;

	string testUser, testPass;
	while(userInfos >> testUser >> testPass){
		updatedFile << testUser << " ";
		if(testUser == username){
			updatedFile << newPass << "\n";
		}
		else
			updatedFile << testPass << "\n";
	}

	userInfos.close();
	updatedFile.close();
	remove("userInfo.txt");
	rename("temp.txt", "userInfo.txt");
	return true;
}