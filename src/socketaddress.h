<<<<<<< HEAD
#ifndef SOCKET_ADDRESS_H
#define SOCKET_ADDRESS_H

#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <cstring>

class socketaddress
{
	protected:
		int port;
		std::string address;

	public:
		socketaddress(struct sockaddr_in addr)
		{
			port = addr.sin_port;

			char ip[INET_ADDRSTRLEN];
			inet_ntop(addr.sin_family, &(addr.sin_addr), ip, INET_ADDRSTRLEN);

			address = std::string(ip);
		}
		socketaddress(std::string address, int port)
		{
			this->address = address;
			this->port = port;
		}
		struct sockaddr_in get_struct()
		{
			struct sockaddr_in addr;
			memset(&addr, 0, sizeof addr);

			addr.sin_family = AF_INET;
			addr.sin_port = htons(port);

			inet_aton(address.c_str(), &addr.sin_addr);

			return addr;
		}
		int get_port()
		{
			return port;
		}
		std::string get_address()
		{
			return address;
		}
};
#endif
=======
#ifndef SOCKETADDRESS_H
#define SOCKETADDRESS_H

#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include<iostream>

using namespace std;

class SocketAddress
{
protected:
        int port;
        std::string address;

    public:
        SocketAddress();
        SocketAddress(struct sockaddr_in addr);
        SocketAddress(std::string address, int port);
        struct sockaddr_in get_struct();
        int get_port();
        std::string get_address();
};

#endif // SOCKETADDRESS_H
>>>>>>> bbeb5875831d01fb0e3ab16dd0439d37f64f5a4d
