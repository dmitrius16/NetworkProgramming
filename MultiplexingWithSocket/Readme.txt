The select() function has several useful features:
1. It can be used to block until any of the sockets in that set is ready to be read from.
2. It can also be configured to return if a socket is ready to be written to or
	if a socket has an error.
3. Additional we can configure select() to return after a specified time if none of these events take place.


int select(int nfds, fd_set *readfds, fd_set *writefds,
					fd_set *exceptfds, struct timeval *timeout);

Usage select.
Before calling select(), we muse first add our sockets into an fd_set. If we have three sockets
socket_listen, socket_a, socket_b, we add them to an fd_set, like this:

fd_set our_sockets;
FD_ZERO(&our_sockets);
FD_SET(socket_listen, &our_sockets);
FD_SET(socket_a, &our_sockets);
FD_SET(socket_b, &our_sockets);

FD_CLR() - remove socket from fd_set
FD_ISSET() - check for presence of a socket in the set 

FD_ZERO(), FD_SET(), FD_CLR(), FD_ISSET() - used for portability between Berekeley sockets and Winsock.

select() also requires that we pass a number that's larger than the largest socket descriptor we are going to monitor.
(This parameter ignored by Windows, we make it for portability!!!)

	SOCKET max_socket;
	max_socket = socket_listen;
	if( socket_a > max_socket) max_socket = socket_a;
	if (socket_b > max_socket) max_socket = socket_b;

	...
	fd_set copy;
	copy = our_sockets;	< --- make copy our socket set.

	select(max_socket+1, &copy, 0, 0, 0); <--- this call modifies our fd_set of sockets to indicate
											which sockets are ready. For that reason we copy our socket set
											before calling it


	.....

	if (FD_ISSET(socket_listen, &copy)) {
		//listen socket has a new connection
		accept(socket_listen, ...)
	}

	if (FD_ISSET(socket_a, &copy)) {
		//socket_a is ready to be read from 
		recv(socket_a, ...)
	}

	if (FD_ISSET(socket_b, &copy)) {
		//socket_b is ready to be read from 
		recv(socket_b, ...)
	}

In the previous example, we pass our fd_set as second argument to select().
If we wanted to monitor fd_set for writability instead of readability, we would pass our 
fd_set as the third arbument to select(). Likewise, we can monitor a set of sockets for 
exceptions by passing it as the fourth argument to select().

Select timeout!




