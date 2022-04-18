/* Purpose: Class for universal client-side program
 *
 * Developer: Joseph Godwin
 *
 * Last Updated: 17 April 2022
 * 
 * Course: COP 4635
 */

#ifndef TCP_CLIENT
#define TCP_CLIENT

#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <string.h>
#include <iostream>
#include <sstream>

using namespace std;

class tcpClient{
public:
	tcpClient(int portNum, string serverIP);
	void sendMessage(string message);
	string receiveMessage();
	int getSocket() {return server_socket;}
private:
	int server_socket;
	struct sockaddr_in server_address;
	int connection;
	bool connectedToServer = false;

	//error handling functions
	void setValidPort(int port);
	void setValidFamily(string family);
	void setValidIP(string IP);
	void connectToServer(int port, string IP);
};

#endif //TCP_CLIENT