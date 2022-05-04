#include "Server.hpp"
#include "ft_irc.hpp"
#include <sys/select.h>

Server::Server() {}

Server::Server( int port ) : _maxFd(4) {

	int		clientSocket, addrSize;
	SA_IN	servAddr;
	int		yes = 1;

	std::cout << "Initialazing server" << std::endl;

	FD_ZERO(&_readFds);
	FD_ZERO(&_writeFds);

	try {

		if ((_servSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			perror("socket");
			throw init_error();
		}
		servAddr.sin_family			= AF_INET;
		servAddr.sin_addr.s_addr	= INADDR_ANY;
		servAddr.sin_port			= htons(port);

		if (setsockopt(_servSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0)
			exit_error("setsockopt");

		if (bind(_servSocket, (SA *)&servAddr, sizeof(servAddr)) < 0) {
			perror("bind");
			throw init_error();
		}
		if (listen(_servSocket, 20) < 0) {
			perror("listen");
			throw init_error();
		}

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
		exit(EXIT_FAILURE);
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

	if (fdType == READFD) {
		this->_readFds = set;
	} else if (fdType == WRITEFD) {
		this->_writeFds = set;
	} else {
		std::cerr << "wrong fdType getFdSet" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void		Server::addToFdSet( int fd, int fdType ) {

	if (fd < 0)
		exit_error("addToFdSet: negative fd");
	if (fdType == READFD) {
		if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {		//!	fcntl may not be good for all fds
			exit_error("fcntl: ");
			perror("fcntl");
		}
		fdSet(fd, &_readFds);
	} else if (fdType == WRITEFD) {
		if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {
			exit_error("fcntl: ");
			perror("fcntl");
		}
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

void		Server::doSelect( fd_set *readfds, fd_set *writefds ) const {

	if (select(FD_SETSIZE, readfds, writefds, NULL, NULL) < 0) {
		perror("select");
		exit(EXIT_FAILURE);
	}
}

bool		Server::doRecv( int fd, fd_set readfds, char buf[512] ) {

	int		nbytes;
// std::cout <<"prerecv"<<std::endl;
	bzero(buf, 512);
	if ((nbytes = recv(fd, buf, 512, 0)) <= 0) {	// connection close ou error
		if (nbytes == -1) {
			std::cout << fd << std::endl;
			perror("recv");
		} else if (nbytes == 0) {
			fdClr(fd, &readfds);
			fdClr(fd, &_readFds);
			fdClr(fd, &_writeFds);
			close(fd);
			std::cout << "Connection has been closed on fd " << fd << std::endl;
		}
		return false;
		}
	if (nbytes > 0) {
//	std::cout <<"postrecv"<<std::endl;
			return true;
	} else {
		return false;
	}
}

void		Server::doSend( Client * client ) {

	int		sentbytes;

	if (!(client->should_send()))
		return;
//std::cout<<client->response();
	if ((sentbytes = send(client->getSockFd(), client->response().c_str(), client->response().length(), 0)) < 0)
		exit_error("send");
	client->sent_bytes(sentbytes);
	
}

/**

void		Server::doSend( int fd, response_list responses ) {
std::cout << "dosend" << std::endl;
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
			exit(EXIT_FAILURE);				//?
		} else {
			tmp = pair.second.substr(sentbytes);
			pair.second = tmp;
			_responseCache.push_back(pair);
		}
	}

	while (!(responses.empty())) {

		pair = responses.front();
		responses.pop_front();

		if (pair.first->getSockFd() != fd) {
			_responseCache.push_back(pair);
		} else if ((sentbytes = send(pair.first->getSockFd(), pair.second.c_str(), sizeof(pair.second), 0)) < 0) {
			perror("send");
			exit(EXIT_FAILURE);
		} else {
			tmp = pair.second.substr(sentbytes);
			pair.second = tmp;
			_responseCache.push_back(pair);
		}
	}
	responses.clear();
}	//! il est possible qu'il y ait des response_pair "fantomes" de 1 empty byte genre

// */
