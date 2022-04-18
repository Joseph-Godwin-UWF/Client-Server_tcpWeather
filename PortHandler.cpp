/* Purpose: Implementation for PortHandler class
 *
 * Developer: Joseph Godwin
 *
 * Last Updated: 5 April 2022
 * 
 * Course: COP 4635
 */

#include "PortHandler.hpp"

PortHandler::PortHandler(int min, int max){
	//IF VALUES ARE MISMATCHED, SWAP THEM
	if(max <= min){
		int temp = min;
		min = max;
		max = temp;
	}
	MAXPORT = max;
	MINPORT = min;
}

int PortHandler::validatePort(int argc, char* argv[]){
	int command_port;
	// GETTING PORT_NUM FROM COMMAND LINE ARGUMENT
	switch(argc){
		/* CASE 1: NO ARGUMENT SUPPLIED
		 * PRINT ERROR MESSAGES & REQUEST
		 * PROPER ARGUMENT(S)
		 * TERMINATE PROGRAM */
		case 1:
			printf("[-] NO ARGUMENTS SUPPLIED!\n");
			printf("\tYou must specify a port in range [%d, %d] to connect to\n", MINPORT, MAXPORT);
			return -1;

		case 2:
		/* CASE 2: CORRECT NUMBER OF ARGUMENTS
		 * CHECK THAT THE ARGUMENT IS A VALID PORT NUMBER
		 * IF NOT VALID, THEN PRINT ERROR MESSAGES & REQUEST
		 * FOR VALID PORT NUMBER, EXIT PROGRAM
		 * IF VALID, ACCEPT PORT AND CONTINUE */
			command_port = atoi(argv[1]);  //atoi() converts the string arg to an int
			if(command_port > MAXPORT || command_port < MINPORT){
				printf("[-] INVALID PORT NUMBER!\n");
				printf("\tUse port number in range [%d, %d]\n", MINPORT, MAXPORT);
				return -1;
			}
			printf("[+] PORT ACCEPTED. . .\n");
			return command_port;
			break;

		/* DEFAULT: TOO MANY ARGUMENTS SUPPLIED
		 * PRINT ERROR MESSAGES AND REQUEST
		 * CORRECT NUMBER OF ARGS
		 * TERMINATE PROGRAM */
		default:
			printf("[-] TOO MANY ARGUMENTS\n");
			printf("\tExpecting 1 argument:\n");
			printf("\tUse port number in range [%d, %d]\n", MINPORT, MAXPORT);
			exit(1);
	}
}