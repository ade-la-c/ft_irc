#include "Client.hpp"

class	ioClient {


public:

	ioClient();
	ioClient( int socket );
	~ioClient();

	void	setSockFd( int sockFd );
	int		getSockFd( void ) const;


private:

	int					_sockFd;
	struct addrinfo		_address;


};
