#include "SocketAbstract.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

int main() {
#if defined (_WIN32)
	WSDATA d;
	if (WSAStartup(MAKEWORD(2,2), &d)) {
		fprintf(stderr, "Failed to initialize.\n");
		return 1;
	}
#endif
// here we figure out our local address  
	printf("Configuring local address...\n");
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	struct addrinfo *bind_address;
	getaddrinfo(0, "8080", &hints, &bind_address);
//Creating socket
	printf("Creating socket...\n");
	SOCKET socket_listen;	//SOCKET is a Winsock type, in Unix SOCKET - int
	socket_listen = socket(bind_address->ai_family,
							bind_address->ai_socktype,
							bind_address->ai_protocol);

	if (!ISVALIDSOCKET(socket_listen)) {
		fprintf(stderr, "socket failed. (%d)\n", GETSOCKETERRNO());
		return 1;
	}
//binding socket to local address
	printf("Binding socket to local address...\n");
	if (bind(socket_listen,		// bind return 0 is success. Bind fails if the port we are binding to is already in use.
				bind_address->ai_addr, bind_address->ai_addrlen)) {
		fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
		return 1;
	}
	freeaddrinfo(bind_address);	// after we have bound to bind_address, we release address memory

// start listening for connections
	printf("Listening...\n");
	if (listen(socket_listen, 10) < 0) {	//10 - connections it is allowed to queue up.
		fprintf(stderr, "listen() failed. (%d)\n", GETSOCKETERRNO());
		return 1;
	}

//After the socket has begun listening for connecitons, 
//we can accept any incoming connection
	printf("Wainting for connection...\n");
	struct sockaddr_storage client_address;
	socklen_t client_len = sizeof(client_address);
	SOCKET socket_client = accept(socket_listen, (struct sockaddr*) &client_address, &client_len);
	if (!ISVALIDSOCKET(socket_client)) {
		fprintf(stderr, "accept() failed. (%d)\n", GETSOCKETERRNO());
		return 1;
	}
// Output Client address to console:
	printf("Client is connected...\n");
	char address_buffer[100];
	getnameinfo((struct sockaddr*)&client_address, client_len, address_buffer,
				sizeof(address_buffer), 0, 0, NI_NUMERICHOST);
	printf("%s\n", address_buffer);

// Read request 
	printf("Reading request...\n");
	char request[1024];
	int bytes_received = recv(socket_client, request, 1024, 0);	//here if connection is terminated by the client
																//recv return 0 or -1. In production you should check
																//recv function > 0, last param for flags
	printf("Received %d bytes.\n", bytes_received);
	
	//output browser request to console
	printf("%.*s",bytes_received, request);

//send response
	printf("Sending response...\n");
	const char *response = 
			"HTTP/1.1 200 OK\r\n"
			"Connection: close\r\n"
			"Connection-Type: text/plain\r\n\r\n"
			"Local time is: ";
	int bytes_sent = send(socket_client, response, strlen(response), 0);
	printf("Send %d of %d bytes.\n", bytes_sent, (int)strlen(response));

	time_t timer;
	time(&timer);
	char *time_msg = ctime(&timer);
	bytes_sent = send(socket_client, time_msg, strlen(time_msg), 0);
	printf("Sent %d of %d bytes.\n", bytes_sent, (int)strlen(time_msg));

	printf("Closing connection...\n");
	CLOSESOCKET(socket_client);

	printf("Closing listening socket...\n");
	CLOSESOCKET(socket_listen);

#if defined(_WIN32)
	WSACleanup();
#endif

	printf("Finished.\n");
	return 0;
}