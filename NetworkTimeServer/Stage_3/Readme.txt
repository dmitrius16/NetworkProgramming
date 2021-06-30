This implemented dual IPv4 and IPv6 version of time server

hints.ai_family = AF_INET6;			<-- only this string need to be changed!
hints.ai_socktype = SOCK_STREAM;
hints.ai_flags = AI_PASSIVE;

For turn on dual mode we need clear IPV6_ONLY flag.
Before bind() call we need:
	int option = 0;
	if (setsockopt(socket_listen, IPPROTO_IPV6, IPV6_V6ONLY, (void*)&option, sizeof(option))) {
		fprintf(stderr, "setsockopt() failed. (%d)\n", GETSOCKETERRNO());
	}
	
IPV6_ONLY flag in IPPROTO_IPV6 is enabled by default, so we clear it by setting in to 0.
