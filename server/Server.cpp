#include "Server.hpp"

Server::Server() {}

Server::Server( int port ) {

	int		clientSocket, addrSize;
	SA_IN	servAddr;

	std::cout << "Initialazing server" << std::endl;

	FD_ZERO(&_readFds);
	FD_ZERO(&_writeFds);

	try {

		if ((_servSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
			throw init_error();
		servAddr.sin_family			= AF_INET;
		servAddr.sin_addr.s_addr	= INADDR_ANY;
		servAddr.sin_port			= htons(port);
		if (bind(_servSocket, (SA *)&servAddr, sizeof(servAddr)) == -1)
			throw init_error();
		if (listen(_servSocket, 20) == -1)
			throw init_error();

	} catch(const std::exception & e) {
		std::cerr << e.what() << std::endl;
	}
}

Server::~Server() {

	std::cout << "Closing server" << std::endl;
	close(_servSocket);
}

fd_set	Server::getFdSet( int fdType ) const {

	if (fdType == READFD)
		return this->_readFds;
	if (fdType == WRITEFD)
		return this->_writeFds;
}

int		Server::getServSocket() const {

	return this->_servSocket;
}

void	Server::addToFdSet( int fd, int fdType ) {

	if (fdType == READFD)
		FD_SET(fd, &_readFds);
	if (fdType == WRITEFD)
		FD_SET(fd, &_writeFds);
}

int		Server::acceptNewConnection() const {

	int		clientSocket = accept(_servSocket, (SA *), NULL, NULL);

	if (clientSocket < 0) {
		error("accept call failed");
		exit(EXIT_FAILURE);
	}
	return clientSocket;
}

void	Server::doSelect() {

	if (select(FD_SETSIZE+1, &_readFds, &_writeFds, NULL, NULL) < 0) {
		error("select failed");
		exit(EXIT_FAILURE);
	}
}
