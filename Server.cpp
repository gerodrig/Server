#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<iostream>
#include<WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

using namespace std;

int main() {

	//Initialize winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOK = WSAStartup(ver, &wsData);
	if (wsOK != 0)
	{
		cerr << "Can't Initialize winsock! Quitting" << endl;
		return 0;
	}

	//Create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "Can't create a scoket! Quitting" << endl;
		return 0;
	}

	//Bindthe ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(59777); //host to network short 54000
	hint.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");   //could also use inet_pton

	bind(listening, (sockaddr*)&hint, sizeof(hint));



	// Tell winsock the socket for listening
	cout << "Server is actively listening for a connection: ";
	listen(listening, SOMAXCONN);

	//wait for conneciton
	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
	
	char host[NI_MAXHOST];   //Clients remote name
	char service[NI_MAXHOST];  // Service the client is connect on

	ZeroMemory(host, NI_MAXHOST);         //same as memset(host, 0, NI_MAXHOST);
	ZeroMemory(service, NI_MAXHOST);  

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		cout << host << " connected on port " << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << "connected on port " <<
			ntohs(client.sin_port) << endl;
	}		

	//close listening socket
	closesocket(listening);

	//While loop: accept and eacho message back to client
	char buf[4096];

	while (true)
	{
		ZeroMemory(buf, 4096);
		// Wait for client to send data
		int bytesReceived = recv(clientSocket, buf, 4096, 0);

		if (bytesReceived == SOCKET_ERROR)
		{
			cerr << "Error in recv(). Quitting" << endl;
			break;
		}
		if (bytesReceived == 0)
		{
			cout << "Client disconnected " << endl;
			break;
		}

		cout << string(buf, 0, bytesReceived) << endl; //output on server end


		//Echo message back to client
		send(clientSocket, buf, bytesReceived + 1, 0);


	}

	//Close the socket
	closesocket(clientSocket);

	//cleanup winsock
	WSACleanup();

	system("pause");


	return 0;


}