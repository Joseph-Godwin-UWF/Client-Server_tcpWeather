DOWNLOADING FILES:
Download and unzip files onto a Linux system.

COMPILING:
Ensure all files are in the same folder
enter the folder on the linux system
	COMPILE SERVER:
	To compile the server type the command:
		/make server
	COMPILING CLIENT:
	To compile the client type the command:
		/make client

	the folder should now contain 2 executables
	labelled 'server' and 'client'

RUNNING THE SERVER:
In the current terminal window (where you just compiled the files)
type the command:
	/server <PORT NUMBER>
where <PORT NUMBER> is replaced with a valid port, for example /server 60045

The server should now be running and waiting to accept connections


RUNNING THE CLIENT(s):
Open a separate window for each client instance
and navigate to the folder containing the project

type the command:
	/client <PORT NUMBER>
where <PORT NUMBER> is the number you previously used to
start the server. If you forgot, refer to the output on
the server's terminal. It will have the port number displayed.

Upon successfully starting the client, prompt instructions to either
	1 - login
	2 - logout
	3 - exit

Upon successfully logging in, you will be prompted with a menu of commands.
type the command you wish to run and follow the dialog boxes.

repeat these instructions for each client you wish to connect.

PITFALLS: Some section of code do not account for inputs with spaces. For
example, you can't subscribe to a location like "gulf breeze" because of the space.
A solution to this that I haven't yet implemented is to receive the input and then
replace all spaces with a special character before sending to the server, and having
the server reparse it. I have implemented this approach with private messaging and group
messages, they can contain full sentences in the message.

The program is CASE-SENSATIVE

ONE USER ALREADY EXISTS AS A LOGIN CREDENTIAL
that login is my name:
	User - joseph
	Pass - godwin