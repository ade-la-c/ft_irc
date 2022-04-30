#include "Server.hpp"
#include <sys/select.h>

Server::Server() {}

Server::Server( int port ) : _maxFd(4) {

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
	else if (fdType == WRITEFD)
		FD_SET(fd, &_writeFds);
	else {
		std::cerr << "wrong fdType" << std::endl;
		exit(1);
	}
}

int			Server::doAccept() const {

	int		clientSocket = accept(_servSocket, (SA *)NULL, NULL);

	if (clientSocket < 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}
	std::cout << "New client connection accepted on socket " << clientSocket << std::endl;
	return clientSocket;
}

void		Server::doSelect( fd_set readfds, fd_set writefds ) const {
// std::cout << "select" << std::endl;

	if (select(FD_SETSIZE+1, &readfds, &writefds, NULL, NULL) < 0) {
		perror("select");
		exit(EXIT_FAILURE);
	}
}

bool		Server::doRecv( int fd, fd_set readfds, char buf[512] ) {

	int		nbytes;

	if ((nbytes = recv(fd, buf, 512, 0)) <= 0) {
		if (nbytes == 0)
			std::cout << "Connection has been closed on fd " << fd << std::endl;
		else
			perror("recv");
		close(fd);
		FD_CLR(fd, &readfds);
		FD_CLR(fd, &_readFds);
		return false;
	} else {
	}
	return true;
}

/**
char *		Server::doRecv( int fd, fd_set readfds ) const {
std::cout << "recv" << std::endl;
	char *	buf = NULL;
	int		nbytes;

	try {
		if ((nbytes = recv(fd, buf, sizeof(buf), 0)) <= 0) {

			if (nbytes == 0) {
				std::cout << "Connection has been closed on fd " << fd << std::endl;
			} else {
				close(fd);
				FD_CLR(fd, &readfds);
				throw server_error();
			}
			close(fd);
			FD_CLR(fd, &readfds);
		}
	} catch (const std::exception & e) {
		std::cerr << e.what() << std::endl;
	}
	return buf;
}

// */

void		Server::doSend( response_list responses ) {
		//!	fix send and keep unsended bytes in cache to keep sending after

	std::pair<Client *, char *>		pair;

	while (responses.empty() == false) {

		pair = responses.front();
		responses.pop_front();

		if (send(pair.first->getSockFd(), pair.second, sizeof(pair.second), 0) < 0) {
			perror("send");
			exit(EXIT_FAILURE);
		}
	}

	responses.clear();
}
