This implemented IPv6 version of time server

hints.ai_family = AF_INET6;			<-- only this string need to be changed!
hints.ai_socktype = SOCK_STREAM;
hints.ai_flags = AI_PASSIVE;


For navigate web browser, we need use http://[::1]:8080
::1 - is the IPv6 loopback address.