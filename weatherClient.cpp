/* Purpose: Top-Level file for the Weather Client
            that connects tp the Weather Server
 *
 * Developer: Joseph Godwin
 *
 * Last Updated: 17 April 2022
 * 
 * Course: COP 4635
 */
#include "tcpClient.hpp"
#include "PortHandler.hpp"
#include "ClientMenus.hpp"
#include <iostream>
#include <string>
#include <thread>
#include <algorithm>
#include <string.h>

bool reprintMenu = false;
bool freeToPrint = true;

void message_handler(int socket);
int getCommand(std::string arg);

int main(int argc, char* argv[]){

	PortHandler portHandler(60000, 60099);
	const int PORT_NUM = portHandler.validatePort(argc, argv);
	
	if(PORT_NUM < 0)
		exit(1);

	tcpClient client(PORT_NUM, "127.0.0.1");
	ClientMenus menu;

	/* DISPLAY THE WELCOME MENU AND PROMPT USER TO EITHER:
	 * LOGIN, REGISTER, or EXIT
     * REPROMPT UNTIL VALID SELECTION IS MADE*/
	string selection;
	do{
		std::cout << menu.welcomeMenu();
		std::cin >> selection;
	}while(selection != "1" && selection != "2" && selection != "exit");

	/* IF 'EXIT' IS SELECTED
	 * PRINT GOODBYE MESSAGE AND EXIT*/
	if(selection == "exit"){
		std::cout << menu.goodbye();
		return 0;
	}

	/* IF 'LOGIN' IS SELECTED
	 * PROMPT USER FOR LOGIN CREDENTIALS
	 * CONTINUE REPROMPTING UNTIL SUCCESSFUL LOGIN */
	if(selection == "1"){
		std::string message;
		do{
			client.sendMessage(menu.formatLoginCommand());
			message = client.receiveMessage();
			cout << message << endl;
		}while(message != "<SERVER>  LOGIN SUCCESSFUL\n");
	}

	/* IF 'REGISTER' IS SELECTED
	 * PROMPT USER TO CREATE AN ACCOUNT
	 * REPROMPT IF PROVIDED USER ALREADY EXISTS*/
	else{
		std::string message;
		do{
			client.sendMessage(menu.formatRegisterCommand());
			message = client.receiveMessage();
			cout << "SERVER: " << message << endl;
		}while(message != "<SERVER>  REGISTERED SUCCESSFULLY\n");
		do{
			client.sendMessage(menu.formatLoginCommand());
			message = client.receiveMessage();
			cout << "SERVER: " << message << endl;
		}while(message != "<SERVER>  LOGIN SUCCESSFUL\n");
	}

	// Create a thread for handling all further receive requests
	std::thread receiver(message_handler, client.getSocket());//finish this
	receiver.detach();

	client.sendMessage("/welcome");
	//handle message sending

	std::string message;
	getline(cin, message); //clearing the cin of extra input

	std::cout << menu.mainMenu();

	getline(cin, message);
	int select = getCommand(message);

	while(select != 9){
	switch(select){
		case 1:{ //subscribe to location
			std::string location;
			freeToPrint = false;
			std::cout << "Enter a location to subscribe to: ";
			getline(cin, location);
			freeToPrint = true;
			std::string command = "/subscribe ";
			command += location;
			client.sendMessage(command);
			break;
		}
		case 2:{ //unsubscribe
			std::string location;
			freeToPrint = false;
			std::cout << "Enter a location to Unsubscribe from: ";
			getline(cin, location);
			freeToPrint = true;
			std::string command = "/unsub ";
			command += location;
			client.sendMessage(command);
			break;
		}
		case 3:{ // see locations subbed
			std::string command = "/sublist";
			client.sendMessage(command);
			break;
		}
		case 4:{ // change password
			std::string oldPass;
			std::string newPass;
			freeToPrint = false;
			std::cout << "Current password: ";
			getline(cin, oldPass);
			std::cout << "New password: ";
			getline(cin, newPass);
			freeToPrint = true;
			std::string command = "/chgpss ";
			command += oldPass;
			command += " ";
			command += newPass;
			client.sendMessage(command);
			break;
		}
		case 5:{ // show active users
			std::string command = "/userlist";
			client.sendMessage(command);
			break;
		}
		case 6:{ // send group message to location
			std::string location;
			std::string message;
			freeToPrint = false;
			std::cout << "Location to send message to (no spaces): ";
			getline(cin, location);
			std::cout << "Message: ";
			getline(cin, message);
			freeToPrint = true;
			replace(message.begin(), message.end(), ' ', '~');
			std::string command = "/groupMsg ";
			command += location;
			command += " ";
			command += message;
			client.sendMessage(command);
			break;
		}
		case 7:{ // private message
			std::string user;
			std::string message;
			freeToPrint = false;
			std::cout << "User to send message to: ";
			getline(cin, user);
			std::cout << "Message: ";
			getline(cin, message);
			freeToPrint = true;
			replace(message.begin(), message.end(), ' ', '~');
			std::string command = "/message ";
			command += user;
			command += " ";
			command += message;
			client.sendMessage(command);
			break;
		}
		case 8:{ // last 10 messages
			std::string command = "/recentMessages";
			client.sendMessage(command);
			break;
		}
		default:{
			cout << "INVALID SELECTION\n";
			break;
		}
	}
	if(reprintMenu){
		std::cout << menu.mainMenu();
		reprintMenu = false;
	}
	do{
        getline(cin, message);
    }while(message == "");
    select = getCommand(message);
}

}

void message_handler(int socket){
	char buff[2048];
	int bytes;

	while((bytes = recv(socket, buff, 2048, 0)) > 0){
		while(!freeToPrint){} //wait until you can print to screen
		printf("\n");
		puts(buff);
		memset(buff, 0, 2048);
	}
}

int getCommand(std::string arg){
	if(arg == "/subscribe")
		return 1;
	else if(arg == "/unsub")
		return 2;
	else if(arg == "/sublist")
		return 3;
	else if(arg == "/chgpss")
		return 4;
	else if(arg == "/userlist")
		return 5;
	else if(arg == "/groupMsg")
		return 6;
	else if(arg == "/message")
		return 7;
	else if(arg == "/recentMessages")
		return 8;
	else if(arg == "/logout")
		return 9;
	else
		return -1;
}
