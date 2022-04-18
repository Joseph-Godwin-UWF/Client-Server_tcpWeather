/* Purpose: Class for serving client-side menus
 *			and formatting commands
 *
 * Developer: Joseph Godwin
 *
 * Last Updated: 17 April 2022
 * 
 * Course: COP 4635
 */

#ifndef CLIENT_MENUS
#define CLIENT_MENUS

#include <string>
#include <iostream>

class ClientMenus{
public:
	ClientMenus();
	std::string welcomeMenu();
	std::string goodbye();
	std::string formatLoginCommand();
	std::string formatRegisterCommand();
	std::string mainMenu();
};

#endif //CLIENT_MENUS