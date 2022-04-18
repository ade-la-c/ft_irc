#include "_IoClient.hpp"

_IoClient::_IoClient() : _sockFd(-1) {}

_IoClient::_IoClient( int sock ) {

}

_IoClient::~_IoClient() {}


void	_IoClient::setSockFd( int sockFd ) {

	this->_sockFd = sockFd;
}

int		_IoClient::getSockFd( void ) const {

	return this->_sockFd;
}
