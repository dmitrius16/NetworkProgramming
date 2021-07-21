UDP client methods:
	Sending data with TCP requires calling connect() to set the remote address and
	establish the TCP connection. Thus, we use send() with TCP sockets, as shown in 
	the following code:

	connect(tcp_socket, peer_address, peer_address_length);
	send(tcp_socket, data, data_length, 0);

	UDP is a connectionless protocol. Therefore, no connection is established before
	sending data. With UDP, data is simply sent and received. We can call connect() and then 
	send, but soket API provides sendto() function:

	sendto(udp_socket, data, data_length, 0, peer_address, peer_address_length);