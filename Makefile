server: WeatherServer.cpp UserHandler.o PortHandler.o Session.o
	g++ -g -Wall UserHandler.o PortHandler.o Session.o WeatherServer.cpp -lpthread -o server

client: weatherClient.cpp tcpClient.o PortHandler.o ClientMenus.o
	g++ -g -Wall weatherClient.cpp tcpClient.o PortHandler.o ClientMenus.o -lpthread -o client

UserHandler.o: UserHandler.cpp
	g++ -c UserHandler.cpp -o UserHandler.o

PortHandler.o: PortHandler.cpp
	g++ -c PortHandler.cpp -o PortHandler.o

Session.o: Session.cpp
	g++ -c Session.cpp -o Session.o

tcpClient.o: tcpClient.cpp tcpClient.hpp
	g++ -c tcpClient.cpp

ClientMenus.o: ClientMenus.hpp ClientMenus.cpp
	g++ -c ClientMenus.cpp

clean:
	 rm *.o server client