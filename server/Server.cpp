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

	} catch (const std::exception & e) {
		std::cerr << e.what() << std::endl;
	}
}

Server::~Server() {

	std::cout << "Closing server" << std::endl;
	close(_servSocket);
}

int			Server::getServSocket() const {

	return this->_servSocket;
}

fd_set		Server::getFdSet( int fdType ) {

	if (fdType != READFD && fdType != WRITEFD) {
		std::cerr << "wrong fdType getFdSet" << std::endl;
		exit(1);
	}
	if (fdType == READFD)
		return _readFds;
	else
		return _writeFds;
}

int			Server::getMaxFd() const {

	return this->_maxFd;
}

void		Server::setMaxFd( int newMaxFd ) {

	this->_maxFd = newMaxFd;
}

void		Server::setFdSet( fd_set set, int fdType ) {

	if (fdType != READFD && fdType != WRITEFD) {
		std::cerr << "wrong fdType getFdSet" << std::endl;
		exit(1);
	}
	if (fdType == READFD)
		_readFds = set;
	else
		_writeFds = set;
}

void		Server::addToFdSet( int fd, int fdType ) {

	if (fdType == READFD)
		FD_SET(fd, &_readFds);
	if (fdType == WRITEFD)
		FD_SET(fd, &_writeFds);
}

int			Server::acceptNewConnection() const {

	int		clientSocket = accept(_servSocket, (SA *)NULL, NULL);

	try {
		if (clientSocket < 0)
			throw server_error();
	} catch (const std::exception & e) {
		std::cerr << e.what() << " accept" << std::endl;
	}
	std::cout << "New client connection accepted on socket " << clientSocket << std::endl;
	return clientSocket;
}

void		Server::doSelect() {

	try {
		if (select(FD_SETSIZE+1, &_readFds, &_writeFds, NULL, NULL) < 0)
			throw server_error();
	} catch (const std::exception & e) {
		std::cerr << e.what() << " select" << std::endl;
	}
}

char *		Server::doRecv( int fd ) const {

	char *	buf = NULL;

	try {
		if (recv(fd, buf, 512, 0) < 1)
			throw server_error();
	} catch (const std::exception & e) {
		std::cerr << e.what() << std::endl;
	}
	return buf;
}

void		Server::doSend( response_list responses ) {

	try {

		std::pair<Client *, char *>		pair;

		while (responses.empty() == false) {

			pair = responses.front();
			responses.pop_front();

			if (send(pair.first->getSockFd(), pair.second, sizeof(pair.second), 0) < 0)
				throw server_error();
		}

	} catch (const std::exception & e) {
		std::cerr << e.what() << std::endl;
	}
	responses.clear();
}
