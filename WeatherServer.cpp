/* Purpose: Top-Level file for the Weather Server
 *
 * Developer: Joseph Godwin
 *
 * Last Updated: 17 April 2022
 * 
 * Course: COP 4635
 */

#include "PortHandler.hpp"
#include "UserHandler.hpp"
#include "Session.hpp"
#include <thread>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <string>
#include <string.h>
#include <vector>
#include <algorithm>
#include <sstream>
#include <netinet/in.h> 	/* sockaddr_in */
#include <unistd.h>     	/* fork() gethostname()*/
#include <sys/socket.h> 	/* socket functions */
#include <sys/sendfile.h>   /* sendfile() */
#include <sys/select.h>     /* select(), handling multiple connections */
#include <fcntl.h>			/* file control */

const int MAXCONN = 20;
const int BUFFSIZE = 256;
bool LOCK_FILE = false;  // control access to file
bool FILE_EDITED = false;// reassign userHandler if file is changed
UserHandler *userHandler;
Session session;

void handle_connection(int socket);
int getCommand(char* arg);

int main(int argc, char *argv[]){

	userHandler = new UserHandler("userInfo.txt");

	//VALIDATE THE PORT IS IN THE RANGE 60000 - 60099
	PortHandler portHandler(60000, 60099);
	const int PORT_NUM = portHandler.validatePort(argc, argv);
	if(PORT_NUM < 0)
		exit(1);

	/* CREATING SOCKET
	 * AF_INET     : IPv4 ADDRESS FAMILY
	 * SOCK_STREAM : USING TCP CONNECTION
	 * 0           : DEFAULT PROTOCOLS */
	int server_socket = socket(AF_INET, SOCK_STREAM, 0);

	// POPULATING AN ADDRESS STRUCTURE
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(PORT_NUM);   //htons() converts int to network byte order
	server_address.sin_addr.s_addr = INADDR_ANY; // INADDR_ANY connects to all IP addresses for local machine

	/* BINDING SOCKET
	 * (struct sockaddr*) &server_address : CASTING SOCKADDR_IN TO SOCKADDR FOR FUCTION
	 * IF bind() RETURNS 0, BINDING FAILED, PRINT ERROR AND EXIT */
	if( bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0){
		printf("[-] FAILED TO BIND TO PORT\n");
		printf("\tTry using a different port number\n");
		exit(1);
	}
	printf("[+] SUCCESSFULLY BOUND:\n");
	printf("\tBound to port:%d\n", PORT_NUM);

	/* LISTENING FOR CONNECTIONS
	 * IF listen() RETURNS 0, SOCKET IS SUCCESSFULLY LISTENING
	 * EXIT PROGRAM
	 * IF listen() RETURNS -1, SOCKET FAILED TO LISTEN
	 * CONTINUE*/
	if( listen(server_socket, MAXCONN) != 0){ //SOMAXCONN - maximum $ of connections
		printf("[-] LISTENING FOR CONNECTIONS FAILED\n");
		exit(1);
	}


	while(true){
		printf("[+] WAITING FOR CONNECTIONS. . .\n");
		// ACCEPT A CONNECTION
		int client_socket = accept(server_socket, NULL, NULL);
		if(client_socket != -1)
			printf("[+] CONNECTION ACCEPTED\n");
		else{
			printf("[-] ERROR ACCEPTING CONNECTION\n");
			continue;
		}

		if(FILE_EDITED){
			while(LOCK_FILE == true){/* wait for file to unlock */}
			LOCK_FILE = true;
			FILE_EDITED = false;
			userHandler = new UserHandler("userInfo.txt");
			LOCK_FILE = false;
		}

		std::thread handler(handle_connection, client_socket);
		handler.detach();

	}
}

void handle_connection(int socket){
	char* token;
	char* args[3] = {NULL, NULL, NULL};
	char buff[BUFFSIZE];

	while(true){
		//Parse Request Info
		bzero(buff, BUFFSIZE);
		if(read(socket, buff, BUFFSIZE) == 0){ // THIS IS WHERE CLIENT DISCONNECTS UNEXPECTEDLY
			session.removeFromActive(socket);
			close(socket);
			return;
		}
		token = strtok(buff, " ");
			args[0] = token;
		token = strtok(NULL, " ");
			args[1] = token;
		token = strtok(NULL, "\r\n");
			args[2] = token;

		switch(getCommand(args[0])){

			case 1:{ // LOGIN
				std::string username(args[1]);
				std::string password(args[2]);
				if(session.getSocket(username) != -1){
					std::string message = "<SERVER>  ALREADY LOGGED IN\n";
					send(socket, message.c_str(), message.length(), 0);
					break;
				}
				userHandler = new UserHandler("userInfo.txt");
				while(LOCK_FILE == true) {/* wait until file is free */}
				LOCK_FILE = true;
				if(userHandler->validateCredentials(username, password)){
					LOCK_FILE = false;
					session.addToActive(username, socket);
					std::string message = "<SERVER>  LOGIN SUCCESSFUL\n";
					send(socket, message.c_str(), message.length(), 0);
				}
				else{
					LOCK_FILE = false;
					std::string message = "<SERVER>  LOGIN FAILED\n";
					send(socket, message.c_str(), message.length(), 0);
				}
			break;
			}

			case 2:{ // REGISTER
				std::string username(args[1]);
				std::string password(args[2]);
				while(LOCK_FILE == true) {/* wait until file is free */}
				LOCK_FILE = true;
				if(userHandler->registerUser(username, password)){
					LOCK_FILE = false;
					FILE_EDITED = true;
					std::string message = "<SERVER>  REGISTERED SUCCESSFULLY\n";
					send(socket, message.c_str(), message.length(), 0);
				}
				else{
					LOCK_FILE = false;
					std::string message = "<SERVER>  USERNAME ALREADY EXISTS\n";
					send(socket, message.c_str(), message.length(), 0);
				}
			break;
			}

			case 3:{ // SUBSCRIBE
				std::string location(args[1]);
				if(session.subToLocation(socket, args[1])){
					std::string message = "<SERVER>  Subscribed To ";
					message += location;
					message += "\n";
					send(socket, message.c_str(), message.length(), 0);
					session.addMessage(session.getUserIndex(session.getUser(socket)), message);
				}
				else{
					std::string message = "<SERVER>  You're Already Subscribed To ";
					message += location;
					message += "\n";
					send(socket, message.c_str(), message.length(), 0);
					session.addMessage(session.getUserIndex(session.getUser(socket)), message);
				}
			break;
			}

			case 4:{ // UNSUBSCRIBE
				std::string location(args[1]);
				if(session.unsubFromLocation(socket, args[1])){
					std::string message = "<SERVER>  Unsubscribed From ";
					message += location;
					message += "\n";
					send(socket, message.c_str(), message.length(), 0);
					session.addMessage(session.getUserIndex(session.getUser(socket)), message);
				}
				else{
					std::string message = "<SERVER>  You Aren't Subscribed To ";
					message += location;
					message += "\n";
					send(socket, message.c_str(), message.length(), 0);
					session.addMessage(session.getUserIndex(session.getUser(socket)), message);
				}
			break;
			}

			case 5:{ // SUBSCRIPTION LIST
				std::string message = "<SERVER>:\nLocation Subscriptions: \n -----------------------\n";
				std::string locations = session.getSubscriptionList(socket);
				message += locations;
				send(socket, message.c_str(), message.length(), 0);
				session.addMessage(session.getUserIndex(session.getUser(socket)), message);
			break;
			}

			case 6:{ // LOGOUT
				session.removeFromActive(socket);
				std::string message = "<SERVER>  LOGOUT SUCCESSFUL\n";
				send(socket, message.c_str(), message.length(), 0);

			break;
			}

			case 7:{ // ACTIVE USERS
				std::string message = "<SERVER>:\nActive Users: \n -------------\n";
				std::string users = session.getAllActiveUsers();
				message += users;
				send(socket, message.c_str(), message.length(), 0);
				session.addMessage(session.getUserIndex(session.getUser(socket)), message);

			break;
			}

			case 8:{ //CHANGE PASSWORD
				std::string username = session.getUser(socket);
				std::string currentPass = args[1];
				std::string newPass = args[2];
				while(LOCK_FILE == true) {/* wait until file is free */}
				LOCK_FILE = true;
				std::string message;
				if( userHandler->changePassword(username, currentPass, newPass) ){
					LOCK_FILE = false;
					FILE_EDITED = true;
					message = "<SERVER>  Password successfully changed!\n";
					send(socket, message.c_str(), message.length(), 0);
					session.addMessage(session.getUserIndex(session.getUser(socket)), message);

				}
				else{
					LOCK_FILE = false;
					message = "<SERVER>  Old password invalid\n";
					send(socket, message.c_str(), message.length(), 0);
					session.addMessage(session.getUserIndex(session.getUser(socket)), message);
				}
			break;
			}

			case 9:{ // WELCOME MESSAGE
				std::string message = "<SERVER>  WELCOME TO THE SERVER\n";
				send(socket, message.c_str(), message.length(), 0);
				session.addMessage(session.getUserIndex(session.getUser(socket)), message);
				break;
			}


			case 10:{ // SEND GROUP MESSAGE TO LOCATION
				std::string location = args[1];
				std::string sender = session.getUser(socket);
				std::string message = sender;
				message+= " to ";
				message += location;
				message += " :> ";
				message += args[2];
				message += "\n";
				replace(message.begin(), message.end(), '~', ' ');

				// Get all active users
				std::string activeUsers = session.getAllActiveUsers();
				std::stringstream s(activeUsers);

				// Loop through active users
				// send message if subbed to location
				std::string currentUser;
				int indexOfUser;
				
				while(s >> currentUser){
					if(session.getSocket(currentUser) != socket){
						indexOfUser = session.getUserIndex(currentUser);
						if(indexOfUser == -1) break;
						for(int i = 0; i < (int)session.activeUsers[indexOfUser].locations.size(); i++){
							if(session.activeUsers[indexOfUser].locations[i] == location){
								send(session.getSocket(currentUser), message.c_str(), message.length(), 0);
								session.addMessage(session.getUserIndex(currentUser), message);
								break;
							}
						}
					}
				}
				message = "<SERVER>  GROUP MESSAGE SENT!\n";
				send(socket, message.c_str(), message.length(), 0);
				session.addMessage(session.getUserIndex(session.getUser(socket)), message);
			break;
			}

			case 11:{ // PRIVATE MESSAGE
				std::string recepient = args[1];
				std::string sender = session.getUser(socket);
				std::string message = sender;
				message+= " :> ";
				message += args[2];
				message += "\n";
				replace(message.begin(), message.end(), '~', ' ');

				//get user's socket
				int recepientSocket = session.getSocket(recepient);
				if(recepientSocket == -1){
					message = "<SERVER>  " + recepient + " is not online\n";
					send(socket, message.c_str(), message.length(), 0);
					session.addMessage(session.getUserIndex(session.getUser(socket)), message);
				}
				else{
					send(recepientSocket, message.c_str(), message.length(), 0);
					session.addMessage(session.getUserIndex(recepient), message);
					message = "<SERVER>  message sent to " + recepient + "\n";
					send(socket, message.c_str(), message.length(), 0);
					session.addMessage(session.getUserIndex(session.getUser(socket)), message);
				}
			break;
			}

			case 12:{ // SHOW RECENT MESSAGES
				std::string username = session.getUser(socket);
				std::string message = "<SERVER>:\nRecent Messages:\n==============\n";
				message += session.getMessages(username);
				send(socket, message.c_str(), message.length(), 0);
				session.addMessage(session.getUserIndex(session.getUser(socket)), message);
			break;
			}

			default:
			break;
		}
	}
}

int getCommand(char* arg){
	if(strcmp(arg, "/login") == 0)
		return 1;
	else if(strcmp(arg, "/register") == 0)
		return 2;
	else if(strcmp(arg, "/subscribe") == 0)
		return 3;
	else if(strcmp(arg, "/unsub") == 0)
		return 4;
	else if(strcmp(arg, "/sublist") == 0)
		return 5;
	else if(strcmp(arg, "/logout") == 0)
		return 6;
	else if(strcmp(arg, "/userlist") == 0)
		return 7;
	else if(strcmp(arg, "/chgpss") == 0)
		return 8;
	else if(strcmp(arg, "/welcome") == 0)
		return 9;
	else if(strcmp(arg, "/groupMsg") == 0)
		return 10;
	else if(strcmp(arg, "/message") == 0)
		return 11;
	else if(strcmp(arg, "/recentMessages") == 0)
		return 12;
	else
		return -1;
}

