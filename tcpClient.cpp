/* Purpose: implementation for tcpClient class
 *
 * Developer: Joseph Godwin
 *
 * Last Updated: 17 April 2022
 * 
 * Course: COP 4635
 */

#include "tcpClient.hpp"

tcpClient::tcpClient(int portNum, string serverIP){
	/* CREATING A CLIENT SOCKET
	 * AF_INET     -> IPv4
	 * SOCK_STREAM -> TCP
	 * 0           -> DEFAULT PROTOCOL  */
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	std::cout << "[+] SOCKET CREATED . . .\n";

	/* SETTING ADDRESS INFO
	 * ERROR CHECKING: ACCEPT PORTS IN RANGE 60,000 - 60,099
	 * DOES NOT ACCEPT HOSTNAMES, ONLY PLAIN IP ADDRESSES
	 */
	std::cout << "[+] GENERATING IP ADDRESS INFO . . .\n";
	setValidFamily("AF_INET");
	setValidPort(portNum);
	setValidIP(serverIP);
	connectToServer(portNum, serverIP);

}

void tcpClient::sendMessage(string message){
	if(connectedToServer){
		send(server_socket, message.c_str(), message.size() + 1, 0);
		return;
	}
	std::cout << "[-] NOT CONNECTED TO A SERVER\n";
}

string tcpClient::receiveMessage(){
	char buff[2048];
	int bytes = recv(server_socket, buff, 2048, 0);
	while(bytes <= 0){
		int bytes = recv(server_socket, buff, 2048, 0);
	}
	string message = string(buff, 0, bytes);
	return message;
}

void tcpClient::setValidFamily(string family){
	if(family == "AF_INET"){
		server_address.sin_family = AF_INET;
	}
	else{
		std::cout << "\t[-] ADDRESS FAMILY INVALID: CLIENT ONLY SUPPORTS \"AF_INET\"\n";
		exit(1);
	}
	std::cout << "\t[+] FAMILY SET TO \"" << family << "\"\n";
}

void tcpClient::setValidPort(int port){
	if(port > 60099 || port < 60000){
		std::cout << "\t[-] INVALID PORT NUMBER, PLEASE USE A PORT# IN RANGE [60000, 60099]\n";
		exit(1);
	}
	server_address.sin_port = htons(port);
	std::cout << "\t[+] CONNECTED TO PORT \"" << port << "\"\n"; 
	return;
}

void tcpClient::setValidIP(string IP){
	stringstream stream(IP);
	string val;
	int count = 0;
	while(getline(stream, val, '.')){
		count++;
		if(stoi(val) > 255 || stoi(val) < 0){
			//ERROR INVALID IP NUMBER
			cout << "\t[-] INVALID IP OCTET AT OCTET[" << count << "]\n";
			exit(1);
		}
	}
	if(count != 4){
		//ERROR IP FORMAT NOT CORRECT
		cout << "\t[-] INVALID IP FORMAT, ONLY " << count << " OCTETS DETECTED, EXPECTED 4.\n";
		exit(1);
	}
	server_address.sin_addr.s_addr = inet_addr(IP.c_str());
	cout << "\t[+] IP VALIDATED: " << IP << endl;
}

void tcpClient::connectToServer(int port, string IP){
	cout << "[+] CONNECTING TO SERVER . . .\n";
	if(connect(server_socket, (struct sockaddr*) &server_address, sizeof(server_address)) < 0){
		cout << "\t[-] ERROR CONNECTING TO SERVER\n";
		connectedToServer = false;
		exit(1);
	}
	connectedToServer = true;
	cout << "\t[+] CONNECTED TO " << IP << ":" << port << endl;
	cout << "==================================================\n\n";
	return;
}
