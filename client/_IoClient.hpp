#ifndef _IOCLIENT_HPP
# define _IOCLIENT_HPP

class	_IoClient {

	public:
	
		_IoClient();
		_IoClient( int socket );
		_IoClient(_IoClient const & cpy);
		~_IoClient();
	
		void	setSockFd( int sockFd );
		int		getSockFd( void ) const;
		char *	getBuf( void );
	
	
	protected:
	
		int					_sockFd;
		char				_buf[512];

};

#endif
