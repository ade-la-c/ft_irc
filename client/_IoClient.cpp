#include "_IoClient.hpp"

_IoClient::_IoClient() : _sockFd(-1) {}

_IoClient::_IoClient( int sock ) : _sockFd(sock) {}

_IoClient::_IoClient( _IoClient const & cpy ) : _sockFd(cpy._sockFd) {}

_IoClient::~_IoClient() {}

int		_IoClient::getSockFd( void ) const {

	return this->_sockFd;
}

char *	_IoClient::getBuf( void ) {

	return this->_buf;
}

void	_IoClient::setBuf( char buf[512] ) {

	std::strcpy(_buf, buf);
	bzero(buf, 512);
}

void	_IoClient::setSockFd( int sockFd ) {

	this->_sockFd = sockFd;
}
