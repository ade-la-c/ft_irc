#include "ioClient.hpp"


ioClient::ioClient() : _sockFd(-1) {}

ioClient::ioClient( int sock ) {

	
}

ioClient::~ioClient() {}


void	ioClient::setSockFd( int sockFd ) {

	this->_sockFd = sockFd;
}

int		ioClient::getSockFd( void ) const {

	return this->_sockFd;
}

