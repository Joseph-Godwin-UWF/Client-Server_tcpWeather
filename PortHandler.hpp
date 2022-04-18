/* Purpose: Class for validating port connectivity
 *
 * Developer: Joseph Godwin
 *
 * Last Updated: 5 April 2022
 * 
 * Course: COP 4635
 */
#ifndef PORT_HANDLER_HPP
#define PORT_HANDLER_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class PortHandler{
public:
	PortHandler(int, int);
	int validatePort(int argc, char* argv[]);
private:
	int MAXPORT;
	int MINPORT;
};

#endif //PORT_HANDLER_HPP