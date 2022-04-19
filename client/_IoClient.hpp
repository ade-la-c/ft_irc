#include "Client.hpp"

class	_IoClient {


public:

	_IoClient();
	_IoClient( int socket );
	~_IoClient();

	void	setSockFd( int sockFd );
	int		getSockFd( void ) const;


private:

	int					_sockFd;
	struct addrinfo		_address;


};
