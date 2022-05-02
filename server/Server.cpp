#include "Server.hpp"
#include "ft_irc.hpp"
#include <sys/select.h>

Server::Server() {}

Server::Server( int port ) : _maxFd(4) {

	int		clientSocket, addrSize;
	SA_IN	servAddr;

	std::cout << "Initialazing server" << std::endl;

	FD_ZERO(&_readFds);
	FD_ZERO(&_writeFds);

	try {

		if ((_servSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			throw init_error();
		servAddr.sin_family			= AF_INET;
		servAddr.sin_addr.s_addr	= INADDR_ANY;
		servAddr.sin_port			= htons(port);
		if (bind(_servSocket, (SA *)&servAddr, sizeof(servAddr)) < 0)
			throw init_error();
		if (listen(_servSocket, 20) < 0)
			throw init_error();

	} catch (const std::exception & e) {
		std::cerr << e.what() << std::endl;
		exit(EXIT_FAILURE);
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

	if (fd < 0)
		exit_error("addToFdSet: negative fd");
	if (fdType == READFD) {
		fcntl(fd, F_SETFL, O_NONBLOCK);		//! protéger fcntl
		fdSet(fd, &_readFds);
	} else if (fdType == WRITEFD) {
		fcntl(fd, F_SETFL, O_NONBLOCK);
		fdSet(fd, &_writeFds);
	} else {
		std::cerr << "wrong fdType" << std::endl;
		exit(EXIT_FAILURE);
	}
}

int			Server::doAccept() const {

	int		clientSocket = accept(_servSocket, (SA *)NULL, NULL);

	if (clientSocket < 0) {
		return -1;
	}
	std::cout << "New client connection accepted on socket " << clientSocket << std::endl;
	return clientSocket;
}

void		Server::doSelect( fd_set readfds, fd_set writefds ) const {

	if (select(FD_SETSIZE+1, &readfds, &writefds, NULL, NULL) < 0) {
		perror("select");
		exit(EXIT_FAILURE);
	}
}

bool		Server::doRecv( int fd, fd_set readfds, char buf[512] ) {

	int		nbytes;
// std::cout <<"prerecv"<<std::endl;
	if ((nbytes = recv(fd, buf, 512, 0)) <= 0) {	// connection close ou error
		if (nbytes == -1) {
			perror("recv");

		fdClr(fd, &readfds);
		fdClr(fd, &_readFds);
		fdClr(fd, &_writeFds);
		close(fd);
		std::cout << "Connection has been closed on fd " << fd << std::endl;
		}
		return false;
	} else {
// std::cout <<"postrecv"<<std::endl;
		return true;
	}
}

/**

void		Server::doSend( response_list responses ) {
		//!	fix send and keep unsended bytes in cache to keep sending after
	response_pair		pair;

	while (responses.empty() == false) {

		pair = responses.front();
		responses.pop_front();

		if (send(pair.first->getSockFd(), pair.second.c_str(), sizeof(pair.second), 0) < 0) {
			perror("send");
			exit(EXIT_FAILURE);
		}
	}

	responses.clear();
}

// */

// /**

void		Server::doSend( int fd, response_list responses ) {

	response_pair		pair;
	std::string			tmp;
	int					sentbytes;

	while (!(_responseCache.empty())) {

		pair = _responseCache.front();
		_responseCache.pop_front();

		if (pair.first->getSockFd() != fd) {
			_responseCache.push_back(pair);
		} else if ((sentbytes = send(pair.first->getSockFd(), pair.second.c_str(), sizeof(pair.second), 0)) < 0) {
			perror("send");
			// exit(EXIT_FAILURE);				//?
		} else {
			tmp = pair.second.substr(sentbytes-1, pair.second.size());
			pair.second = tmp;
			_responseCache.push_back(pair);
		}
	}

	while (!(responses.empty())) {

		pair = responses.front();
		responses.pop_front();

		if ((sentbytes = send(pair.first->getSockFd(), pair.second.c_str(), sizeof(pair.second), 0)) < 0) {
			perror("send");
			exit(EXIT_FAILURE);
		} else {
			tmp = pair.second.substr(sentbytes-1, pair.second.size());
			pair.second = tmp;
			_responseCache.push_back(pair);
		}
	}
	responses.clear();
}	//! il est possible qu'il y ait des response_pair "fantomes" de 1 empty byte genre

// */
/**

void		Server::doSend( int fd, response_list responses ) {

	response_pair		pair;
	std::string			tmp;
	int					sentbytes;

	while (!(_responseCache.empty())) {

		pair = _responseCache.front();
		_responseCache.pop_front();

		if ((sentbytes = send(pair.first->getSockFd(), pair.second.c_str(), sizeof(pair.second), 0)) < 0) {
			perror("send");
			exit(EXIT_FAILURE);
		} else {
			tmp = pair.second.substr(sentbytes-1, pair.second.size());
			pair.second = tmp;
			_responseCache.push_back(pair);
		}
	}
}

// */