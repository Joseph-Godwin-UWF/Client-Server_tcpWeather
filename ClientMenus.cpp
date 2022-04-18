/* Purpose: Implementation file for ClientMenus class
 *
 * Developer: Joseph Godwin
 *
 * Last Updated: 17 April 2022
 * 
 * Course: COP 4635
 */
#include "ClientMenus.hpp"

ClientMenus::ClientMenus(){

}

std::string ClientMenus::welcomeMenu(){
	std::string menu;
	menu += "Welcome!\n";
	menu += "Press 1 to Login\n";
	menu += "Press 2 to Register\n";
	menu += "Type \"quit\" to Quit\n\n";
	return menu;
}

std::string ClientMenus::goodbye(){
	return "Goodbye!\n\n";
}

std::string ClientMenus::formatLoginCommand(){
	std::cout << "ENTER LOGIN DETAILS\n";
	std::string username, password;
	std::cout << "Username: ";
	
	std::cin >> username;
	std::cout << "Password: ";
	
	std::cin >> password;
	return "/login " + username + " " + password;
}

std::string ClientMenus::formatRegisterCommand(){
	std::cout << "ENTER REGISTRATION DETAILS\n";
	std::string username, password;
	std::cout << "Username: ";
	
	std::cin >> username;
	std::cout << "Password: ";
	
	std::cin >> password;
	return "/register " + username + " " + password;
}

std::string ClientMenus::mainMenu(){
	std::string message = "================COMMANDS=============================\n";
			   message += "/subscribe      - Subscribe to a location\n";
	           message += "/unsub          - Unsubscribe from a location\n";
	           message += "/sublist        - See locations subscribed to\n";
	           message += "/chgpss         - Change password\n";
	           message += "/userlist       - show list of active users\n";
	           message += "/groupMsg       - Send a group message to location\n";
	           message += "/message        - Send a private message to user\n";
	           message += "/recentMessages - See last 10 messages received\n";
	           message += "/logout         - Logout and end program\n";
	           message += "=====================================================\n";
	return message;
}